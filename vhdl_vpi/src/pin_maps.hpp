#ifndef PIN_MAPS_HPP
#define PIN_MAPS_HPP

#include <string>
#include <unordered_map>

// clang-format off
// TODO: do we want hashmaps to store this type of data?

const inline std::unordered_map<std::string, std::size_t> SW_PINS = {
    {"PIN_C10", 0}, // SW[0]
    {"PIN_C11", 1}, // SW[1]
    {"PIN_D12", 2}, // ...
    {"PIN_C12", 3}, 
    {"PIN_A12", 4},  
    {"PIN_B12", 5},
    {"PIN_A13", 6},
    {"PIN_A14", 7},
    {"PIN_B14", 8},
    {"PIN_F15", 9}};

const inline std::unordered_map<std::string, std::size_t> LED_PINS = {
    {"PIN_A8", 0},  // LEDR[0]
    {"PIN_A9", 1},  // LEDR[1]
    {"PIN_A10", 2}, // ...
    {"PIN_B10", 3},
    {"PIN_D13", 4},
    {"PIN_C13", 5},
    {"PIN_E14", 6},
    {"PIN_D14", 7},
    {"PIN_A11", 8},
    {"PIN_B11", 9}
};

const inline std::unordered_map<std::string, std::size_t> HEX0_PINS = {
    { "PIN_C14", 0 }, // HEX0[0]
    { "PIN_E15", 1 }, // HEX0[1]
    { "PIN_C15", 2 }, // ...
    { "PIN_C16", 3 },
    { "PIN_E16", 4 },
    { "PIN_D17", 5 }, 
    { "PIN_C17", 6 },
    { "PIN_D15", 7 }
};

const inline std::unordered_map<std::string, std::size_t> HEX1_PINS = {
    { "PIN_C18", 0 }, // HEX1[0]
    { "PIN_D18", 1 }, // HEX1[1]
    { "PIN_E18", 2 }, // ...
    { "PIN_B16", 3 },
    { "PIN_A17", 4 },
    { "PIN_A18", 5 },
    { "PIN_B17", 6 },
    { "PIN_A16", 7 }
};

const inline std::unordered_map<std::string, std::size_t> HEX2_PINS = {
    { "PIN_B20", 0 }, // HEX2[0]
    { "PIN_A20", 1 }, // HEX2[1]
    { "PIN_B19", 2 }, // ...
    { "PIN_A21", 3 },
    { "PIN_B21", 4 },
    { "PIN_C22", 5 },
    { "PIN_B22", 6 },
    { "PIN_A19", 7 }
};

const inline std::unordered_map<std::string, std::size_t> HEX3_PINS = {
    { "PIN_F21", 0 }, // HEX3[0]
    { "PIN_E22", 1 }, // HEX3[1]
    { "PIN_E21", 2 }, // ...
    { "PIN_C19", 3 },
    { "PIN_C20", 4 },
    { "PIN_D19", 5 },
    { "PIN_E17", 6 },
    { "PIN_D22", 7 }
};

const inline std::unordered_map<std::string, std::size_t> HEX4_PINS = {
    { "PIN_F18", 0 }, // HEX4[0]
    { "PIN_E20", 1 }, // HEX4[1]
    { "PIN_E19", 2 }, // ...
    { "PIN_J18", 3 },
    { "PIN_H19", 4 },
    { "PIN_F19", 5 },
    { "PIN_F20", 6 },
    { "PIN_F17", 7 }
};

const inline std::unordered_map<std::string, std::size_t> HEX5_PINS = {
    { "PIN_J20", 0 }, // HEX5[0]
    { "PIN_K20", 1 }, // HEX5[1]
    { "PIN_L18", 2 }, // ...
    { "PIN_N18", 3 },
    { "PIN_M20", 4 },
    { "PIN_N19", 5 },
    { "PIN_N20", 6 },
    { "PIN_L19", 7 }
};
// clang-format on

#endif