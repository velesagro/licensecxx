#include <lcxx/identifiers/hardware.hpp>

#include <nlohmann/json.hpp>

#include <ident_utils/cpu_utils.hpp>
#include <lcxx/encoding.hpp>
#include <lcxx/hash.hpp>
#include <lcxx/identifiers/common.hpp>

namespace lcxx::experimental::ident_utils::cpu {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( cpu_info, revision, max_frequency, serial, model_name);
}
namespace lcxx::experimental::ident_utils::pcb {
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( pcb_info, gsm_imei, serial);
}
namespace lcxx::experimental::identifiers {

    auto operator|( hw_ident_strat const lhs, hw_ident_strat const rhs ) -> hw_ident_strat
    {
        using base_t = std::underlying_type_t< hw_ident_strat >;
        return static_cast< hw_ident_strat >( static_cast< base_t >( lhs ) | static_cast< base_t >( rhs ) );
    }

    auto hardware( hw_ident_strat const strategy ) -> identifier
    {
        auto cpu_info = ident_utils::cpu::get_info();
        auto pcb_info = ident_utils::pcb::get_info();

        nlohmann::json info_json;

        if ( strat_active( strategy, hw_ident_strat::all ) ) {
            auto msg = nlohmann::json{ cpu_info }.dump();
            return { encode::base64( hash::md5( msg ) ), msg };
        }

        if ( strat_active( strategy, hw_ident_strat::cpu ) ) {
            info_json["cpu"] = { cpu_info };
        }
        else {
            if ( strat_active( strategy, hw_ident_strat::cpu_revision ) )
                info_json["revision"] = cpu_info.revision;
            if ( strat_active( strategy, hw_ident_strat::cpu_max_frequency ) )
                info_json["max_frequency"] = cpu_info.max_frequency;
            if ( strat_active( strategy, hw_ident_strat::cpu_serial ) )
                info_json["serial"] = cpu_info.serial;
            if ( strat_active( strategy, hw_ident_strat::cpu_model_name ) )
                info_json["model_name"] = cpu_info.model_name;
        }

        if(strat_active(strategy, hw_ident_strat::gsm_imei))
        {
            info_json["gsm_imei"] = pcb_info.gsm_imei;
        }
        if(strat_active(strategy, hw_ident_strat::pcb_serial))
        {
            info_json["pcb_serial"] = pcb_info.serial;
        }

        auto msg = info_json.dump();
        return { encode::base64( hash::md5( msg ) ), msg };
    }

    auto verify( hw_ident_strat const strategy, std::string_view const hash ) -> bool
    {
        return hash == hardware( strategy ).hash;
    }

}  // namespace lcxx::experimental::identifiers
