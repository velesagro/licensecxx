#include <iostream>

#include <lcxx/identifiers/hardware.hpp>
#include <lcxx/identifiers/os.hpp>
#include <lcxx/lcxx.hpp>
#include "../../../keys/include/keys/private_key.hpp"

auto main() -> int
{
    lcxx::license license;

    // Push optional data into the license file
    license.push_content( "Company", "VelesAgro" );
    license.push_content( "pcb_serial", "13555571262047198238");
    license.push_content( "os_architecture", "aarch64");
    license.push_content( "os_name", "Linux");
    license.push_content( "os_pc_name", "b2qt-raspberrypi4-64");
    license.push_content( "max_frequency", "1500000");
    license.push_content( "model_name", " Raspberry Pi Compute Module 4 Rev 1.1");
    license.push_content( "revision", " b03141");
    license.push_content( "serial", "\u001b");
    auto key = lcxx::crypto::load_key( std::string{ lcxx::keys::private_key }, lcxx::crypto::key_type::private_key );
    std::cout << lcxx::to_json( license, key ).dump( 4 ) << std::endl;
}
