#include <ident_utils/cpu_utils.hpp>

#include <sys/sysinfo.h>
#include <thread>

#include <ident_utils/common.hpp>

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

namespace lcxx::experimental::ident_utils::pcb {

    auto get_info() -> pcb_info
    {
        pcb_info ci;

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

        ci.gsm_imei = "1";// read from GSM module
        ci.serial    = "2"; // read from crypto IC

        return ci;
    }

}  // namespace lcxx::experimental::ident_utils::pcb
