#ifndef TETRIS_H
#define TETRIS_H

#include <stdint.h>
#include <stdbool.h>

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20

// om cellen är 0 är den tom, sedan representeras färger via 1-7
typedef enum {
    CELL_EMPTY = 0,
    PIECE_I, // I block (ljus blå)
    PIECE_O, // 2x2 (gul)
    PIECE_T, // T block (lila)
    PIECE_S, // squiggly 1 (grön)
    PIECE_Z, // squiggly 2 (röd)
    PIECE_J, // Bakvänd L block (blå)
    PIECE_L // vanliga L blocket (orange)
} PieceType;

// Aktiv eller kommande Tetrino
typedef struct {
    PieceType type;
    uint8_t rotation; // rotationerna räknas 0-3
    int8_t x;         // Kolumn 0-9
    int8_t y;         // Rad (0-19)
} Piece;

// Hela spelets tillstånd som skickas till skrämen
typedef struct {
    uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
    Piece current;
    Piece next;
    uint32_t score;
    uint8_t level;
    bool game_over;
    bool dirty; // för att kolla om vi måste rita om skärmen
} GameState;

// Funktionssignaturer för spellogiken
void tetris_init(GameState *state);
void tetris_move_horizontal(GameState *state, int8_t dx); // dx = -1 (vänster) eller +1 (höger)
void tetris_rotate(GameState *state);
void tetris_soft_drop(GameState *state);
void tetris_tick(GameState *state); // Anropas vid timerns tick som fungerar som gravitationen

#endif