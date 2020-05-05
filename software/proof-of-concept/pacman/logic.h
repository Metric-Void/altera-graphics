#ifndef LOGIC_H_
#define LOGIC_H_

#ifndef nullptr
#define nullptr 0x00000000
#endif // nullptr

#define _BOARD_W 15
#define _BOARD_H 15
#define _BOARD_COUNT 1

#include <stdint.h>
#include <stdbool.h>

typedef struct _point {
    uint8_t xpos;
    uint8_t ypos;
} Point;

// This is used for storing path.
typedef struct _lplist {
    Point curr;
    struct _lplist* next;
} LinkedPoints;

typedef struct _player {
    uint8_t xpos;       // Position X. First axis.
    uint8_t ypos;       // Position Y, Second axis.
    uint16_t points;    // Score.
    uint16_t invincible;     // Invincible countdown
} Player;

typedef struct _ghost {
    uint8_t xpos;
    uint8_t ypos;
    uint8_t target_xpos;
    uint8_t target_ypos;    // Some ghosts will need these parameter.
    uint16_t revive_cntdown;    // Revive after countdown.
} Ghost;

typedef enum _tile_runtime {
    WALL = 0,
    CANDY = 1,
    SUPER = 2,
    WAY = 3,
} Tile;

typedef enum _tile_type {
    WALLk = 0,
    CANDYk = 1,
    SUPERk = 2,          // Super candy. Turns player invincible.
    WAYk = 3,
    GHOST_BLINKY = 4,   // Follow (chase) player. Red
    GHOST_PINKY = 5,    // Ambush. Pink
    GHOST_INKY = 6,     // Patrol. Cyan
    GHOST_CLYDE = 7,    // Random.
    PLAYER_POS = 8,      // Mostly used for initializing the game.
} TileExpanded;

typedef enum _game_state {
    PREINIT = 0,
    PLAYING = 1,
    DIED = 2,
    WON = 3,
} GameState;

typedef struct _game_board {
    Tile game_map[_BOARD_H][_BOARD_W];
    uint8_t current_board;
    uint16_t highscore;
    Player player;
    Ghost blinky;
    Ghost pinky;
    Ghost inky;
    Ghost clyde;
    GameState state;
    uint16_t candy_count;   // This will be used to determine if map has been finished
} Game;

// Game boards. May have multiple levels, so an array.
static TileExpanded GAME_BOARDS[_BOARD_COUNT][_BOARD_H][_BOARD_W] = {
    {
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0},
        {0, 2, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 0},
        {1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1},
        {0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0},
        {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0, 4, 5, 1, 6, 7, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0},
        {0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0},
        {1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1},
        {0, 2, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 2, 0},
        {0, 0, 1, 0, 1, 1, 1, 8, 1, 1, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}
    }
};

// Used for MinHeap.
static int8_t distances[_BOARD_H][_BOARD_W];

void init_game(Game* game);
void tickgame(Game* board, char keypress);
static inline bool is_over(Game* board);
void recur_free(LinkedPoints* lp);
LinkedPoints* stack_push(LinkedPoints* head, const Point* data);
static inline Point stack_peek(LinkedPoints* head);
static inline LinkedPoints* stack_pop(LinkedPoints* head, Point* popped_data);
static inline bool PointEquals(const Point a, const Point b);
static inline bool stack_isempty(const LinkedPoints* head);
void blinkys_move(Game* game);
void det_goal_pos(Game* game, char keypress);
void pinkys_move(Game* game, char keypress);
void inkys_move(Game* game);
void clydes_move(Game* game);
void print_board(Game* game);

#endif // LOGIC_H_
