#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include "logic.h"

// Data storing mechanics explanation.
// Board size is 15x15, as an array. First argument is height, second argument is width.

// When running, the board tile only stores one of CANDY, SUPER, WALL or ROAD.
// In the board configuration, it contains additional PLAYER_POS, GHOST_INKY, GHOST_BLINKY, GHOST_CLYDE and GHOST_PINKY.
// These values are only used for initialization.

// Initialize a game.
// The game variable must already have space allocated.
void init_game(Game* game) {
    uint8_t current_board = 0;

    game->candy_count = 0;
    game->state = PREINIT;

    // Find tiles in this map and assign them.
    uint8_t i, j;
    for(i=0; i<15; i+=1) {
        for(j=0; j<15; j+=1) {
            if(GAME_BOARDS[current_board][i][j] == PLAYER_POS) {
                game->player.xpos = i;
                game->player.ypos = j;
                game->game_map[i][j] = CANDY;
                game->candy_count += 1;
            } else if(GAME_BOARDS[current_board][i][j] == GHOST_BLINKY) {
                game->blinky.xpos = i;
                game->blinky.ypos = j;
                game->game_map[i][j] = CANDY;
                game->candy_count += 1;
            } else if(GAME_BOARDS[current_board][i][j] == GHOST_PINKY) {
                game->pinky.xpos = i;
                game->pinky.ypos = j;
                game->game_map[i][j] = CANDY;
                game->candy_count += 1;
            } else if(GAME_BOARDS[current_board][i][j] == GHOST_INKY) {
                game->inky.xpos = i;
                game->inky.ypos = j;
                game->game_map[i][j] = CANDY;
                game->candy_count += 1;
            } else if(GAME_BOARDS[current_board][i][j] == GHOST_CLYDE) {
                game->clyde.xpos = i;
                game->clyde.ypos = j;
                game->game_map[i][j] = CANDY;
                game->candy_count += 1;
            } else if(GAME_BOARDS[current_board][i][j] == CANDY || GAME_BOARDS[current_board][i][j] == SUPER) {
                game->candy_count += 1;
                game->game_map[i][j] = (Tile)GAME_BOARDS[current_board][i][j];
            } else {    // WALL and WAY.
                game->game_map[i][j] = (Tile)GAME_BOARDS[current_board][i][j];
            }
        }
    }

    game->state = PLAYING;
}

void tickgame(Game* board, char keypress) {
    if(board->state!=PLAYING) return;
    // Move the player
    int16_t new_xpos = board->player.xpos;
    int16_t new_ypos = board->player.ypos;
    bool player_moved = false;

    switch(keypress) {
        case 'w':
        case 'W':
            new_xpos -= 1;
            player_moved = true;
            break;
        case 'S':
        case 's':
            new_xpos += 1;
            player_moved = true;
            break;
        case 'A':
        case 'a':
            new_ypos -= 1;
            player_moved = true;
            break;
        case 'D':
        case 'd':
            new_ypos += 1;
            player_moved = true;
    }
    // Now check if the new coordinate is legal.
    new_xpos = (new_xpos + _BOARD_H) % _BOARD_H;
    new_ypos = (new_ypos + _BOARD_W) % _BOARD_W;

    if(board->game_map[new_xpos][new_ypos] != WALL) {
        // New position is valid. Check if player actually moved.
        if(player_moved) {
            board->player.xpos = new_xpos;
            board->player.ypos = new_ypos;
            // Pick the tile up.
            switch(board->game_map[new_xpos][new_ypos]) {
                case CANDY:
                    board->game_map[new_xpos][new_ypos] = WAY;
                    board->player.points += 10;
                    board->candy_count -= 1;
                    break;
                case SUPER:
                    board->game_map[new_xpos][new_ypos] = WAY;
                    board->player.points += 100;
                    board->player.invincible = 15*60;   // 15 seconds.
                    board->candy_count -= 1;
                    break;
                case WALL:  // which should not happen
                case WAY:
                    break;
            }
        }
    }

    // Ghosts move.
    blinkys_move(board);

    // Check if the player met a ghost.
    if(board->player.invincible > 0) {
        // Ghost gets eaten. Grant points and set ghost away.
        if(board->player.xpos == board->blinky.xpos && board->player.ypos == board->blinky.ypos) {
            // Met Blinky when invincible.
            board->blinky.revive_cntdown = 15*60;
            board->player.points += 200;
        } else if (board->player.xpos == board->inky.xpos && board->player.ypos == board->inky.ypos) {
            // Met inky when invincible.
            board->inky.revive_cntdown = 15*60;
            board->player.points += 200;
        } else if (board->player.xpos == board->pinky.xpos && board->player.ypos == board->pinky.ypos) {
            // Met pinky when invincible.
            board->pinky.revive_cntdown = 15*60;
            board->player.points += 200;
        } else if (board->player.xpos == board->clyde.xpos && board->player.ypos == board->clyde.ypos) {
            // Met clyde when invincible.
            board->clyde.revive_cntdown = 15*60;
            board->player.points += 200;
        }
    } else {
        // Player is not invincible. Gets killed if ghost met.
        // These branches are retained in case some additional things added.
        if(board->player.xpos == board->blinky.xpos && board->player.ypos == board->blinky.ypos) {
            // Met Blinky
            board->state = DIED;
        } else if (board->player.xpos == board->inky.xpos && board->player.ypos == board->inky.ypos) {
            // Met inky
            board->state = DIED;
        } else if (board->player.xpos == board->pinky.xpos && board->player.ypos == board->pinky.ypos) {
            // Met pinky.
            board->state = DIED;
        } else if (board->player.xpos == board->clyde.xpos && board->player.ypos == board->clyde.ypos) {
            // Met clyde.
            board->state = DIED;
        }
    }

    // Check if game finished.
    if(is_over(board)) board -> state = WON;
}

// Check if the current map is finished.
// The whole game is passed so we can maintain the array shape.
static inline bool is_over(Game* board)
{
    return board -> candy_count == 0;
}

// Recursively free a pointer.
void recur_free(LinkedPoints* lp)
{
    if(lp != nullptr) {
        recur_free(lp -> next);
        free(lp);
    }
}

// LinkedPoints as stack: push
// Returns the new head (head will change)
LinkedPoints* stack_push(LinkedPoints* head, const Point* data) {
    LinkedPoints* newPoint = malloc(sizeof(LinkedPoints));
    newPoint->curr = *data;
    newPoint->next = head;
    return newPoint;
}

// Peek the top of the stack.
static inline Point stack_peek(LinkedPoints* head) {
    return head -> curr;
}

// Pop an element. Element will be written to popped_data.
LinkedPoints* stack_pop(LinkedPoints* head, Point* popped_data) {
    *popped_data = head -> curr;
    LinkedPoints* nextptr = head -> next;
    free(head);
    return nextptr;
}

static inline bool stack_isempty(const LinkedPoints* head) {
    return head != nullptr;
}

// Wish I'm using Java?
static inline bool PointEquals(const Point a, const Point b) {
    return a.xpos == b.xpos && a.ypos == b.ypos;
}

// Blinky. Use Dijkstra to find the shortest path to the player.
void blinkys_move(Game* game) {
    LinkedPoints* dijkstra_fringe;
    dijkstra_fringe = nullptr;
    // Put Blinky's current position into fringe.
    int8_t distances[_BOARD_H][_BOARD_W];
    bool dst_finalize[_BOARD_H][_BOARD_W];
    Tile tmp_map[_BOARD_H][_BOARD_W];

    memcpy(tmp_map, game->game_map, sizeof(tmp_map));

    // Mark other ghosts as obstacles.
    tmp_map[game->inky.xpos][game->inky.ypos] = WALL;
    tmp_map[game->clyde.xpos][game->clyde.ypos] = WALL;
    tmp_map[game->pinky.xpos][game->pinky.ypos] = WALL;

    uint8_t i, j;

    for(i = 0; i<_BOARD_H;i+=1) {
        for(j=0; j<_BOARD_W;j+=1) {
            distances[i][j] = 63; //Something really long.
            dst_finalize[i][j] = false; // All not finalized.
        }
    }

    distances[game->player.xpos][game->player.ypos] = 0;

    Point player_pos;
    player_pos.xpos = game->player.xpos;
    player_pos.ypos = game->player.ypos;
    // Propagate from player to ghost.
    stack_push(dijkstra_fringe, &player_pos);

    while(!stack_isempty(dijkstra_fringe)) {
        Point curr_seek;
        dijkstra_fringe = stack_pop(dijkstra_fringe, &curr_seek);
        dst_finalize[curr_seek.xpos][curr_seek.ypos] = true;
        // Investigate UP of current point.
        int16_t nxpos = curr_seek.xpos;
        int16_t nypos = curr_seek.ypos;
        nxpos = (nxpos - 1 + _BOARD_H) % _BOARD_H;

        if(game->game_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = nxpos;
            npt.ypos = nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }

        // Investigate DOWN.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nxpos = (nxpos + 1) % _BOARD_H;

        if(game->game_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = nxpos;
            npt.ypos = nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }

        // Investigate LEFT.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nypos = (nypos - 1 + _BOARD_W) % _BOARD_W;

        if(game->game_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = nxpos;
            npt.ypos = nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }

        // Investigate RIGHT.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nypos = (nypos + 1) % _BOARD_W;

        if(game->game_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = nxpos;
            npt.ypos = nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }
    }

    // Now all tiles are marked with a distance. See where should blinky go.
    uint8_t best_xpos = game->blinky.xpos;
    uint8_t best_ypos = game->blinky.ypos;
    uint8_t best_dist = 63;

    // Investigate UP.
    int16_t nxpos = game->blinky.xpos;
    int16_t nypos = game->blinky.ypos;
    nxpos = (nxpos - 1 + _BOARD_H) % _BOARD_H;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = distances[nxpos][nypos];
        best_xpos = nxpos;
        best_ypos = nypos;
    }

    // Investigate DOWN.
    nxpos = game->blinky.xpos;
    nypos = game->blinky.ypos;
    nxpos = (nxpos + 1) % _BOARD_H;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = distances[nxpos][nypos];
        best_xpos = nxpos;
        best_ypos = nypos;
    }

    // Investigate LEFT.
    nxpos = game->blinky.xpos;
    nypos = game->blinky.ypos;
    nypos = (nypos - 1 + _BOARD_W) % _BOARD_W;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = distances[nxpos][nypos];
        best_xpos = nxpos;
        best_ypos = nypos;
    }

    // Investigate RIGHT.
    nxpos = game->blinky.xpos;
    nypos = game->blinky.ypos;
    nypos = (nypos + 1) % _BOARD_W;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = distances[nxpos][nypos];
        best_xpos = nxpos;
        best_ypos = nypos;
    }

    if(best_dist != 63) {
        game->blinky.xpos = best_xpos;
        game->blinky.ypos = best_ypos;
    }
}

// Pinky. Ambush the player by cutting a way towards the player's moving direction.
void pinkys_move(Game* game) {

}

// Inky. Patrol an area.
void inkys_move(Game*game) {

}

// Clyde. Move randomly.
void clydes_move(Game* game) {

}
