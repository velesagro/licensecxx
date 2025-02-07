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
    license.push_content( "hardware", lcxx::experimental::identifiers::hardware().hash );
    license.push_content( "os", lcxx::experimental::identifiers::os().hash );
    auto key = lcxx::crypto::load_key( std::string{ lcxx::keys::private_key }, lcxx::crypto::key_type::private_key );
    std::cout << lcxx::to_json( license, key ).dump( 4 ) << std::endl;
}
