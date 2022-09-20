#ifndef LCXX__IDENTIFIERS_HARDWARE_HPP__
#define LCXX__IDENTIFIERS_HARDWARE_HPP__

#include <cstddef>
#include <string>
#include <utility>

#include <lcxx/identifiers/identifier.hpp>

namespace lcxx::identifiers {

    enum class hw_ident_strat : std::int32_t {
        all = 1,
        cpu = 1 << 1,
        // gpu               = 1 << 2,
        // ethernet          = 1 << 3,
        cpu_n_cores       = 1 << 10,
        cpu_n_threads     = 1 << 11,
        cpu_max_frequency = 1 << 12,
        cpu_vendor        = 1 << 13,
        cpu_model_name    = 1 << 14,
    };

    auto operator|( hw_ident_strat const lhs, hw_ident_strat const rhs ) -> hw_ident_strat;

    /**
     * @brief analysis the host machine and creates an identification string from the gathered information. The analyzed
     * aspects depend on the strategy chosen
     *
     * @param strategy the analyzed hardware aspects. Different strategies can be combined through bitwise-or|
     * @return identifier the identifier containing a hash and the clear text result of the hardware analysis
     */
    auto hardware( hw_ident_strat const strategy = hw_ident_strat::all ) -> identifier;

    auto verify( hw_ident_strat const strategy, std::string_view const hash ) -> bool;

}  // namespace lcxx::identifiers

#endif  // LCXX__IDENTIFIERS_HARDWARE_HPP__
