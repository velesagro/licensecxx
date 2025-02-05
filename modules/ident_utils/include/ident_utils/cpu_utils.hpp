#ifndef LCXX__IDENT_UTILS_CPU_UTILS_HPP__
#define LCXX__IDENT_UTILS_CPU_UTILS_HPP__

#include <string>

namespace lcxx::experimental::ident_utils::cpu {

    struct cpu_info {
        std::string revision;
        std::size_t max_frequency;
        std::string serial;
        std::string model_name;
    };

    /**
     * @brief retrieves information about the CPU built into this machine
     *
     * @return cpu_info an information object about the CPU
     */
    auto get_info() -> cpu_info;

}  // namespace lcxx::experimental::ident_utils::cpu

namespace lcxx::experimental::ident_utils::pcb {

    struct pcb_info {
        std::string gsm_imei;
        std::string serial;
    };

    /**
     * @brief retrieves information about the PCB built into this machine
     *
     * @return pcb_info an information object about the PCB
     */
    auto get_info() -> pcb_info;

}  // namespace lcxx::experimental::ident_utils::cpu
#endif  // LCXX__IDENT_UTILS_CPU_UTILS_HPP__
