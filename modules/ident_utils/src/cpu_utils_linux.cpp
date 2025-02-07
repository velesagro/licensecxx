#include <ident_utils/cpu_utils.hpp>

#include <sys/sysinfo.h>
#include <thread>
#include "cryptoauthlib.h"
#include "config.h"
#include <ident_utils/common.hpp>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace lcxx::experimental::ident_utils::cpu {

    auto get_info() -> cpu_info
    {
        cpu_info ci;

        auto try_stoull = []( std::string const & str ) -> unsigned long long {
            try {
                return std::stoull( str );
            }
            catch ( std::invalid_argument & e ) {
                // Should maybe throw again? Or just leave these entries empty
                // Having them empty does not break systems that do not have permissions to access these files
                return {};
            }
        };

        ci.max_frequency = try_stoull( cat_file( "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" ) );
        auto cpu_info    = cat_file( "/proc/cpuinfo" );

        auto parse_cpu_info_attr = []( std::string const & info, std::string_view const key ) -> std::string {
            std::string_view info_v = info;
            if ( auto pos = info_v.find( key ); pos != std::string::npos ) {
                info_v.remove_prefix( pos );
                info_v.remove_prefix( info_v.find( ":" ) + 1 );
                info_v.remove_suffix( info_v.size() - info_v.find( '\n' ) );
                return std::string{ info_v };
            }

            return {};
        };

        ci.revision     = parse_cpu_info_attr( cpu_info, "Revision" );
        ci.model_name = parse_cpu_info_attr( cpu_info, "Model" );
        ci.serial    = try_stoull( parse_cpu_info_attr( cpu_info, "Serial" ) );

        return ci;
    }

}  // namespace lcxx::experimental::ident_utils::cpu

std::string sendATCommand(const std::string& port, const std::string& command) {
    int serialPort = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (serialPort < 0) {
        std::cerr << "Error opening serial port " << port << std::endl;
        return "";
    }

    struct termios tty;
    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Error getting terminal attributes" << std::endl;
        close(serialPort);
        return "";
    }

            // Налаштування порту: 115200 8N1 (8 біт, без парності, 1 стоп-біт)
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 біт даних
    tty.c_iflag &= ~IGNBRK; // Вимкнути обробку розриву
    tty.c_lflag = 0; // Не обробляти введені дані
    tty.c_oflag = 0; // Вимкнути спеціальні режими виводу
    tty.c_cc[VMIN]  = 0; // Немає мінімальної кількості байтів
    tty.c_cc[VTIME] = 0; // Чекати дані в select()

    tcsetattr(serialPort, TCSANOW, &tty);

            // Додаємо `\r\n` до команди та відправляємо
    std::string fullCommand = command + "\r\n";
    write(serialPort, fullCommand.c_str(), fullCommand.size());

            // Читаємо відповідь
    char buffer[256];
    std::string response="n/a";
    fd_set set;
    struct timeval timeout;
    int bytesRead;

    timeout.tv_sec = 2;  // 2 секунди таймаут
    timeout.tv_usec = 0;

    while (true) {
        FD_ZERO(&set);
        FD_SET(serialPort, &set);

        int res = select(serialPort + 1, &set, nullptr, nullptr, &timeout);
        if (res > 0) {
            bytesRead = read(serialPort, buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
                // Якщо є "OK", припиняємо читання
                if (response.find("OK") != std::string::npos) {
                    break;
                }
            }
        } else {
            // Якщо select() повертає 0 — таймаут
            std::cerr << "Timeout reached." << std::endl;
            break;
        }
    }

    close(serialPort);
    return response;
}
std::string cleanResponse(const std::string& response) {
    std::string result = response;

            // Видаляємо всі символи, крім цифр (IMEI містить лише цифри)
    result.erase(std::remove_if(result.begin(), result.end(),
                                  [](unsigned char c) { return !std::isdigit(c); }),
                  result.end());

    return result;
}

namespace lcxx::experimental::ident_utils::pcb {

    auto get_info() -> pcb_info
    {
        pcb_info ci;

        std::string response = sendATCommand("/dev/ttyAMA4", "AT+CGSN");
        std::string imei = cleanResponse(response);
        std::cout << "IMEI: " << imei << std::endl;
        ci.gsm_imei = imei;// read from GSM module

        ATCAIfaceCfg cfg = cfg_ateccx08a_i2c_default;
        ATCA_STATUS status = atcab_init(&cfg);
        uint8_t sn[9];
        if (status == ATCA_SUCCESS)
        {
            status = atcab_read_serial_number(sn);
            if (status == ATCA_SUCCESS){
                for(int i = 0; i < 9; i++) {
                    // Додаємо кожен байт до serial як двозначне шістнадцяткове число
                    ci.serial += std::to_string(sn[i]);
                }
            }
            else
            {
                ci.serial = "0000000000000000";
            }
        }
        else
        {
            ci.serial = "0000000000000000";
        }
        //printf("ci.serial = %s",ci.serial);
        return ci;
    }

}  // namespace lcxx::experimental::ident_utils::pcb
