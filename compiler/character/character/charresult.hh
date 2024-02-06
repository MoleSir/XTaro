#pragma once

#include <vector>
#include <string>
#include <map>

namespace xtaro::character
{
    
    struct CharacterResult
    {
        std::vector<double> slews {};
        std::vector<double> loads {};

        std::map<std::string, std::vector<double>> cellTable {
            {std::string{"delay_lh"}, std::vector<double>{}},
            {std::string{"delay_hl"}, std::vector<double>{}},
            {std::string{"slew_lh"},  std::vector<double>{}},
            {std::string{"slew_hl"},  std::vector<double>{}},
        };
        
        std::map<std::string, std::vector<double>> constraintTable {
            {std::string{"setup_times_HL"}, std::vector<double>{}},
            {std::string{"setup_times_LH"}, std::vector<double>{}},
            {std::string{"hold_times_LH"}, std::vector<double>{}},
            {std::string{"hold_times_HL"}, std::vector<double>{}},
        };
    };

}