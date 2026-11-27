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

/*
kollar inom 4x4 rutan r,c vilka platser blocken faktiskt sitter på
*/ 
static bool shape_has_block(PieceType type, uint8_t rot, int r, int c) {
    if(type == CELL_EMPTY) return false;
    uint16_t mask = ROTATIONS[type - 1][rot & 3]; // hämtar ut biten binärt, -1 för att vi saknar CELL_EMPTY och maskar med 3 för att hålla roationen mellan 0 och 3
    int bit_index = 15 - (r * 4 + c); // omvandlar bitindex från 2d till 1d
    return (mask >> bit_index) & 1;
}


/*
https://stackoverflow.com/questions/7602919/how-do-i-generate-random-numbers-without-rand-function
LSFR (Linear Feedback Shift Register)
för att undvika att använda rand()
kör XOR på bit 0,2,3,5 ; maskar så vi får sista biten
"puttar av" sista biten och lägger till den maskade biten längsfram
mod 7 för att få 0-6 sen +1 för att få index på tetrominon
*/
static uint16_t lsfr = 0xACE1u;

static PieceType random_piece_type(void) {
    uint16_t bit = ((lsfr >> 0) ^ (lsfr >> 2) ^ (lsfr >> 3) ^ (lsfr >> 5)) & 1;
    lsfr = (lsfr >> 1) | (bit << 15);
    return (PieceType)((lsfr % 7) + 1);
}


/*
kollar om p får plats utan att krocka med väggar, golvet eller andra bitar
kollar även inom varje 4x4 för att se för varje bit om den krockar med något
*/
static bool is_valid_position(const GameState *state, Piece p) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (!shape_has_block(p.type, p.rotation, r, c)){
                continue;
            }
            int board_x = p.x + c;
            int board_y = p.y + r;

            if (board_x < 0 || board_x >= BOARD_WIDTH || board_y >= BOARD_HEIGHT) return false;

            if (board_y >= 0 && state->board[board_x][board_y] != CELL_EMPTY) return false;
        }

    }
    return true;
}


/*
spawnar in nya tetrominor högst upp på planen i mitten med rot 0
kollar även om biten direkt spawnar i ett annat block då är spelet slut
har inte testat än så vet inte om den täcker alla fall för game over
*/
static void spawn_piece(GameState *state) {
    state->current = state->next;
    state->current.x = 3;
    state->current.y = 0; // 0 just nu för att, kanske skaffa buffert sen för att spawna in dem ovanför
    state->current.rotation = 0;

    state->next.type = random_piece_type();
    state->next.rotation = 0;

    if (!is_valid_position(state, state->current)) state->game_over = true;
}


/*
tömmer brädet sätter, återstället score, level och gameover bool, 
spawnar en ny tetromino samt sätter dirty till true så att skrämen ska uppdatera
*/
void tetris_init(GameState *state) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            state->board[y][x] = CELL_EMPTY;
        }
    }
    state->score = 0;
    state->level = 1;
    state->game_over = false;

    state->next.type = random_piece_type();
    spawn_piece(state);
    state->dirty = true;
}


/*
försöker flytta biten sidledes med dx genom att använda sim som kollar
om det får plats innan den flyttas
*/
void tetris_horizontal_move(GameState *state, int8_t dx) {
    Piece sim = state->current;
    sim.x += dx;

    if(is_valid_position(state, sim)) {
        state->current = sim;
        state->dirty = true;
    }
}