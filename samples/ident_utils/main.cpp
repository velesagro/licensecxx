#include <iostream>

#include <nlohmann/json.hpp>

#include <ident_utils/cpu_utils.hpp>
#include <ident_utils/os_utils.hpp>

using namespace lcxx::experimental::ident_utils;

namespace lcxx::experimental::ident_utils::cpu {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( cpu_info, revision, max_frequency, serial, model_name);
}
namespace lcxx::experimental::ident_utils::pcb {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( pcb_info, gsm_imei, serial);
}
namespace lcxx::experimental::ident_utils::os {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( os_info, os_name, os_architecture, os_pc_name, user_name, user_groups, user_uid,
                                        user_gids );
}

auto main() -> int
{
    auto print = []( auto const & desc, auto const & info ) {
        std::cout << desc << ": " << nlohmann::json( info ).dump( 4 ) << std::endl;
    };

    print( "cpu", cpu::get_info() );
    print( "pcb", pcb::get_info() );
    print( "os", os::get_info() );
    // print( "disk", disk::get_info() );

    return 0;
}
