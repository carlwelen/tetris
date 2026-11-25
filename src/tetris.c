#include "tetris.h"

/*
lista över bitarnas roationer då de alla får plats i 
4x4 representerar vi dem i hex
*/
static const uint16_t ROTATIONS[7][4] = {
    {0x0f00, 0x2222, 0x00f0, 0x4444}, // I
    {0x6600, 0x6600, 0x6600, 0x6600}, // O
    {0x4e00, 0x4640, 0x0e40, 0x4c40}, // T
    {0x6C00, 0x4620, 0x06C0, 0x8C40}, // S
    {0xC600, 0x2640, 0x0C60, 0x4C80}, // Z
    {0x8E00, 0x6440, 0x0E20, 0x44C0}, // J
    {0x2E00, 0x4460, 0x0E80, 0xC440}  // L
};

// kollar inom rutan r,c vilka platser blocken faktiskt sitter på
static bool shape_has_block(PieceType type, uint8_t rot, int r, int c) {
    if(type == CELL_EMPTY) return false;
    uint16_t mask = ROTATIONS[type - 1][rot & 3]; // hämtar ut biten binärt, -1 för att vi saknar CELL_EMPTY och maskar med 3 för att hålla roationen mellan 0 och 3
    int bit_index = 15 - (r * 4 + c); // omvandlar bitindex från 2d till 1d
    return (mask >> bit_index) & 1;
}

