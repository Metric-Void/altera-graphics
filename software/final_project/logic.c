
#ifdef __cplusplus
extern "C" {
#endif

#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include "logic.h"
#include "tile_eng_driver.h"
#include "hardware.h"
#include "updater.h"

// Static function declarations.
static inline bool is_over(Game* board);
static inline Point stack_peek(LinkedPoints* head);
static inline LinkedPoints* stack_pop(LinkedPoints* head, Point* popped_data);
static inline bool PointEquals(const Point a, const Point b);

// Data storing mechanics explanation.
// Board size is 15x15, as an array. First argument is height, second argument is width.

// When running, the board tile only stores one of CANDY, SUPER, WALL or ROAD.
// In the board configuration, it contains additional PLAYER_POS, GHOST_INKY, GHOST_BLINKY, GHOST_CLYDE and GHOST_PINKY.
// These values are only used for initialization.

// Initialize a game.
// The game variable must already have space allocated.
void init_game(Game* game) {
    uint8_t current_board = game->current_board;
    if(current_board >= _BOARD_COUNT) {
    	game->state = WON;
    	return;
    }

    initScreen();
    game->candy_count = 0;
    game->state = PREINIT;
    game->player.invincible = 0;
    game->blinky.revive_cntdown = 0;
    game->pinky.revive_cntdown = 0;
    game->inky.revive_cntdown = 0;
    game->clyde.revive_cntdown = 0;
    game->player.points = 0;

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
        	addPendingPos(board->player.xpos, board->player.ypos);
        	addPendingPos(new_xpos, new_ypos);
            board->player.xpos = (uint8_t)new_xpos;
            board->player.ypos = (uint8_t)new_ypos;
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
                    board->player.invincible = 15;   // 15 seconds.
                    board->candy_count -= 1;
                    break;
                case WALL:  // which should not happen
                case WAY:
                    break;
            }
        }
    }

    if(board->player.points > board->highscore) board->highscore = board->player.points;

    // Ghosts move.
    blinkys_move(board);
    printf("Blinky\n");
    fflush(stdout);
    pinkys_move(board, keypress);
    printf("Pinky\n");
    fflush(stdout);
    clydes_move(board);
    printf("Clyde\n");
    fflush(stdout);
    inkys_move(board);
    printf("Inky DONE\n");
    fflush(stdout);

    // Check if the player met a ghost.
    if(board->player.invincible > 0) {
        // Ghost gets eaten. Grant points and set ghost away.
        if(board->player.xpos == board->blinky.xpos && board->player.ypos == board->blinky.ypos) {
            // Met Blinky when invincible.
            board->blinky.revive_cntdown = 15;
            board->player.points += 200;
        } else if (board->player.xpos == board->inky.xpos && board->player.ypos == board->inky.ypos) {
            // Met inky when invincible.
            board->inky.revive_cntdown = 15;
            board->player.points += 200;
        } else if (board->player.xpos == board->pinky.xpos && board->player.ypos == board->pinky.ypos) {
            // Met pinky when invincible.
            board->pinky.revive_cntdown = 15;
            board->player.points += 200;
        } else if (board->player.xpos == board->clyde.xpos && board->player.ypos == board->clyde.ypos) {
            // Met clyde when invincible.
            board->clyde.revive_cntdown = 15;
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
    if(is_over(board)) {
    	if(board->current_board < _BOARD_COUNT-1){
    		board->current_board += 1;
    		init_game(board);
    	} else {
    		board -> state = WON;
    	}
    }
}

// Check if the current map is finished.
// The whole game is passed so we can maintain the array shape.
static inline bool is_over(Game* board) {
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

// Temporary distance used for both blinky and pinky.
// Also used as index for "minheap"
static int8_t distances[_BOARD_H][_BOARD_W];

// MinHeap Push.
// Data in this linked list will be sorted by distance. Smaller distance comes first.
LinkedPoints* stack_push(LinkedPoints* head, const Point* data) {
    if(head == nullptr || distances[head->curr.xpos][head->curr.ypos] >= distances[data->xpos][data->ypos]) {
        LinkedPoints* newPoint = malloc(sizeof(LinkedPoints));
        newPoint->curr = *data;
        newPoint->next = head;
        return newPoint;
    } else {
        head->next = stack_push(head->next, data);
        return head;
    }
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
    //Blinky is dead
    if(game->blinky.revive_cntdown > 0) {
        game->blinky.revive_cntdown -= 1;
        return;
    }
    LinkedPoints* dijkstra_fringe;
    dijkstra_fringe = nullptr;
    // Put Blinky's current position into fringe.
    bool dst_finalize[_BOARD_H][_BOARD_W];
    Tile tmp_map[_BOARD_H][_BOARD_W];

    // Copy a map, and mark other ghosts as walls.
    memcpy(tmp_map, game->game_map, sizeof(tmp_map));
    // Mark other ghosts as obstacles.
    tmp_map[game->inky.xpos][game->inky.ypos] = WALL;
    tmp_map[game->clyde.xpos][game->clyde.ypos] = WALL;
    tmp_map[game->pinky.xpos][game->pinky.ypos] = WALL;

    uint8_t i, j;

    for(i = 0; i<_BOARD_H;i+=1) {
        for(j = 0; j<_BOARD_W;j+=1) {
            distances[i][j] = 63; //Something really long.
            dst_finalize[i][j] = false; // All not finalized.
        }
    }

    distances[game->player.xpos][game->player.ypos] = 0;

    Point player_pos;
    player_pos.xpos = game->player.xpos;
    player_pos.ypos = game->player.ypos;
    // Propagate from player to ghost.
    dijkstra_fringe = stack_push(dijkstra_fringe, &player_pos);

    while(!stack_isempty(dijkstra_fringe)) {
        Point curr_seek;
        dijkstra_fringe = stack_pop(dijkstra_fringe, &curr_seek);
        dst_finalize[curr_seek.xpos][curr_seek.ypos] = true;
        printf("*");
        fflush(stdout);
        // Investigate UP of current point.
        int16_t nxpos = curr_seek.xpos;
        int16_t nypos = curr_seek.ypos;
        nxpos = (nxpos - 1 + _BOARD_H) % _BOARD_H;

        if(!dst_finalize[nxpos][nypos] && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }

        // Investigate DOWN.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nxpos = (nxpos + 1) % _BOARD_H;

        if(!dst_finalize[nxpos][nypos] && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }

        // Investigate LEFT.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nypos = (nypos - 1 + _BOARD_W) % _BOARD_W;

        if(!dst_finalize[nxpos][nypos] && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe = stack_push(dijkstra_fringe, &npt);
        }

        // Investigate RIGHT.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nypos = (nypos + 1) % _BOARD_W;

        if(!dst_finalize[nxpos][nypos] && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
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
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    // Investigate DOWN.
    nxpos = game->blinky.xpos;
    nypos = game->blinky.ypos;
    nxpos = (nxpos + 1) % _BOARD_H;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    // Investigate LEFT.
    nxpos = game->blinky.xpos;
    nypos = game->blinky.ypos;
    nypos = (nypos - 1 + _BOARD_W) % _BOARD_W;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    // Investigate RIGHT.
    nxpos = game->blinky.xpos;
    nypos = game->blinky.ypos;
    nypos = (nypos + 1) % _BOARD_W;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    if(best_dist != 63) {
    	addPendingPos(game->blinky.xpos, game->blinky.ypos);
    	addPendingPos(best_xpos, best_ypos);
        game->blinky.xpos = best_xpos;
        game->blinky.ypos = best_ypos;
    }

    recur_free(dijkstra_fringe);
}

// Pinky. Ambush the player by cutting a way towards the player's moving direction.

static int16_t goal_pos_x = 0;
static int16_t goal_pos_y = 0;

void det_goal_pos(Game* game, char keypress) {             //function to determine the postion for pinky
    goal_pos_x = game->player.xpos;
    goal_pos_y = game->player.ypos;

	if (keypress == 'w' || keypress == 'W') {
		goal_pos_y = (goal_pos_y - 5 + _BOARD_H) % _BOARD_H;
		while(game->game_map[goal_pos_x][goal_pos_y] == WALL) goal_pos_y = (goal_pos_y + 1) % _BOARD_H;
	}
	if (keypress == 's' || keypress == 'S') {
		goal_pos_y = (game -> player.ypos + 5) % _BOARD_H;
		while(game->game_map[goal_pos_x][goal_pos_y] == WALL) goal_pos_y = (goal_pos_y - 1 + _BOARD_H) % _BOARD_H;
	}
	if (keypress == 'a' || keypress == 'A') {
		goal_pos_x = (game->player.xpos - 5 + _BOARD_W) % _BOARD_W;
		while(game->game_map[goal_pos_x][goal_pos_y] == WALL) goal_pos_x = (goal_pos_x + 1) % _BOARD_W;
	}
	if (keypress == 'd' || keypress == 'D') {
		goal_pos_x = (game->player.xpos + 5) % _BOARD_W;
		while(game->game_map[goal_pos_x][goal_pos_y] == WALL) goal_pos_x = (goal_pos_x - 1 + _BOARD_W) % _BOARD_W;
	}
}

void pinkys_move(Game* game, char keypress) {
    // Pinky is dead.
    if(game->pinky.revive_cntdown > 0) {
        game->pinky.revive_cntdown -= 1;
        return;
    }

    bool dst_finalize[_BOARD_H][_BOARD_W];
    Tile tmp_map[_BOARD_H][_BOARD_W];
    memcpy(tmp_map, game->game_map, sizeof(tmp_map));

    uint8_t i, j;
    for(i = 0; i<_BOARD_H;i+=1) {
        for(j=0; j<_BOARD_W;j+=1) {
            distances[i][j] = 63; //Something really long.
            dst_finalize[i][j] = false; // All not finalized.
        }
    }

    det_goal_pos(game, keypress);          // Determine the goal position of pinky
    distances[goal_pos_x][goal_pos_y] = 0; // Set the distance of goal to 0.

	//Reapply dijkstra to the desired goal pos for pinky
    LinkedPoints* dijkstra_fringe_pinky;
    dijkstra_fringe_pinky = nullptr;

    Point desired_pos;
    desired_pos.xpos = (uint8_t)goal_pos_x;
    desired_pos.ypos = (uint8_t)goal_pos_y;
    // Propagate from desired position to ghost.
    dijkstra_fringe_pinky = stack_push(dijkstra_fringe_pinky, &desired_pos);



    // Mark other ghosts as obstacles.
    tmp_map[game->inky.xpos][game->inky.ypos] = WALL;
    tmp_map[game->clyde.xpos][game->clyde.ypos] = WALL;
    tmp_map[game->blinky.xpos][game->blinky.ypos] = WALL;

    while(!stack_isempty(dijkstra_fringe_pinky)) {
        Point curr_seek;
        printf("*");
        fflush(stdout);
        dijkstra_fringe_pinky = stack_pop(dijkstra_fringe_pinky, &curr_seek);
        dst_finalize[curr_seek.xpos][curr_seek.ypos] = true;
        // Investigate UP of current point.
        int16_t nxpos = curr_seek.xpos;
        int16_t nypos = curr_seek.ypos;
        nxpos = (nxpos - 1 + _BOARD_H) % _BOARD_H;

        if(!dst_finalize[nxpos][nypos] && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe_pinky = stack_push(dijkstra_fringe_pinky, &npt);
        }

        // Investigate DOWN.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nxpos = (nxpos + 1) % _BOARD_H;

        if(!dst_finalize[nxpos][nypos] && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe_pinky = stack_push(dijkstra_fringe_pinky, &npt);
        }

        // Investigate LEFT.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nypos = (nypos - 1 + _BOARD_W) % _BOARD_W;

        if(!dst_finalize[nxpos][nypos]  && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe_pinky = stack_push(dijkstra_fringe_pinky, &npt);
        }

        // Investigate RIGHT.
        nxpos = curr_seek.xpos;
        nypos = curr_seek.ypos;
        nypos = (nypos + 1) % _BOARD_W;

        if(!dst_finalize[nxpos][nypos]  && tmp_map[nxpos][nypos] != WALL) {
            distances[nxpos][nypos] = distances[nxpos][nypos] > distances[curr_seek.xpos][curr_seek.ypos]? distances[curr_seek.xpos][curr_seek.ypos]+1 : distances[nxpos][nypos];
            Point npt;
            npt.xpos = (uint8_t)nxpos;
            npt.ypos = (uint8_t)nypos;
            dijkstra_fringe_pinky = stack_push(dijkstra_fringe_pinky, &npt);
        }
    }

    // Now all tiles are marked with a distance. See where should pinky go.
    uint8_t best_xpos = game->pinky.xpos;
    uint8_t best_ypos = game->pinky.ypos;
    uint8_t best_dist = 63;

    // Investigate UP.
    int16_t nxpos = game->pinky.xpos;
    int16_t nypos = game->pinky.ypos;
    nxpos = (nxpos - 1 + _BOARD_H) % _BOARD_H;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    // Investigate DOWN.
    nxpos = game->pinky.xpos;
    nypos = game->pinky.ypos;
    nxpos = (nxpos + 1) % _BOARD_H;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    // Investigate LEFT.
    nxpos = game->pinky.xpos;
    nypos = game->pinky.ypos;
    nypos = (nypos - 1 + _BOARD_W) % _BOARD_W;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    // Investigate RIGHT.
    nxpos = game->pinky.xpos;
    nypos = game->pinky.ypos;
    nypos = (nypos + 1) % _BOARD_W;

    if(distances[nxpos][nypos] < best_dist) {
        best_dist = (uint8_t)distances[nxpos][nypos];
        best_xpos = (uint8_t)nxpos;
        best_ypos = (uint8_t)nypos;
    }

    if(best_dist != 63) {
    	addPendingPos(game->pinky.xpos, game->pinky.ypos);
    	addPendingPos(best_xpos, best_ypos);
        game->pinky.xpos = best_xpos;
        game->pinky.ypos = best_ypos;
    }

    recur_free(dijkstra_fringe_pinky);
}


static uint8_t ikdirs[4] = {5,5,5,5};
static uint8_t lastdirection = 0;
// Inky. Patrol an area.
// Use a psuedo-Mattrox distribution.
void inkys_move(Game* game) {
    // Inky's dead.
    if(game->inky.revive_cntdown > 0) {
        game->inky.revive_cntdown -= 1;
        return;
    }
    Tile tmp_map[_BOARD_H][_BOARD_W];
    memcpy(tmp_map, game->game_map, sizeof(tmp_map));

    // Mark other ghosts as obstacles.
    tmp_map[game->pinky.xpos][game->pinky.ypos] = WALL;
    tmp_map[game->clyde.xpos][game->clyde.ypos] = WALL;
    tmp_map[game->blinky.xpos][game->blinky.ypos] = WALL;

	// A slightly higher chance to stay the same direction.
	ikdirs[lastdirection] += 3;
	uint8_t nextlast;
    uint8_t direction;
    uint8_t trials = 0;
    bool success = false;

    while(!success && trials < 10) {
        printf("*");
        direction = rand() % (ikdirs[0] + ikdirs[1] + ikdirs[2] + ikdirs[3]);
        uint8_t newx = game->inky.xpos;
        uint8_t newy = game->inky.ypos;

        if(direction < ikdirs[0]) {
            newy -= 1;
            ikdirs[0] += 1;
            nextlast = 0;
        } else if (direction < ikdirs[0] + ikdirs[1]) {
            newy += 1;
            ikdirs[1] += 1;
            nextlast = 1;
        } else if (direction < ikdirs[0] + ikdirs[1] + ikdirs[2]) {
            newx -= 1;
            ikdirs[2] += 1;
            nextlast = 2;
        } else {
            newx += 1;
            ikdirs[3] += 1;
            nextlast = 3;
        }

        newx = (newx + _BOARD_W) % _BOARD_W;
        newy = (newy + _BOARD_H) % _BOARD_H;

        ikdirs[lastdirection] -= 10;
        lastdirection = nextlast;

        if(tmp_map[newx][newy] != WALL) {
        	addPendingPos(game->inky.xpos, game->inky.ypos);
        	addPendingPos(newx, newy);
            game->inky.xpos = newx;
            game->inky.ypos = newy;
            success = true;
        }
        trials += 1;
    }
}

// Clyde. Move randomly.
void clydes_move(Game* game) {
    // Clyde's dead.
    if(game->clyde.revive_cntdown > 0) {
        game->clyde.revive_cntdown -= 1;
        return;
    }
    Tile tmp_map[_BOARD_H][_BOARD_W];
    memcpy(tmp_map, game->game_map, sizeof(tmp_map));

    // Mark other ghosts as obstacles.
    tmp_map[game->pinky.xpos][game->inky.ypos] = WALL;
    tmp_map[game->inky.xpos][game->inky.ypos] = WALL;
    tmp_map[game->blinky.xpos][game->blinky.ypos] = WALL;
    uint8_t rand_direction;
    uint8_t trials = 0;
    bool success = false;
    while(!success && trials < 10) {
        printf("*");
        rand_direction = rand() % 4;
        int16_t newx, newy;
        switch(rand_direction) {
            case 0: {
                newx = game->clyde.xpos + 1;
                newy = game->clyde.ypos;
                break;
            }
            case 1: {
                newx = game->clyde.xpos - 1;
                newy = game->clyde.ypos;
                break;
            }
            case 2: {
                newx = game->clyde.xpos;
                newy = game->clyde.ypos + 1;
                break;
            }
            default: {
                newx = game->clyde.xpos;
                newy = game->clyde.ypos - 1;
                break;
            }
        }
        newx = (newx + _BOARD_W) % _BOARD_W;
        newy = (newy + _BOARD_H) % _BOARD_H;
        if(tmp_map[newx][newy] != WALL) {
            // Add to update queue.
        	addPendingPos(game->clyde.xpos, game->clyde.ypos);
        	addPendingPos(newx, newy);
            game->clyde.xpos = (uint8_t)newx;
            game->clyde.ypos = (uint8_t)newy;
            success = true;
        }
        trials += 1;
	}
}

void blinkyPalette() {
	Palette* binkyPalette;
	binkyPalette = malloc(sizeof(Palette));
	binkyPalette->colors[0] = 0x000000;
	binkyPalette->colors[3] = 0xFF5243;
	binkyPalette->paletteId = 3;
	writePalette(binkyPalette);
	free(binkyPalette);
}

void pinkyPalette() {
	Palette* pinkyPalette;
	pinkyPalette = malloc(sizeof(Palette));
	pinkyPalette->colors[0] = 0x000000;
	pinkyPalette->colors[3] = 0xC92CE8;
	pinkyPalette->paletteId = 4;
	writePalette(pinkyPalette);
	free(pinkyPalette);
}

void inkyPalette(){
	Palette* inkyPalette;
	inkyPalette = malloc(sizeof(Palette));
	inkyPalette->colors[0] = 0x000000;
	inkyPalette->colors[3] = 0x2CD0E8;
	inkyPalette->paletteId = 5;
	writePalette(inkyPalette);
	free(inkyPalette);
}

void clydePalette() {
	Palette* clydePalette = malloc(sizeof(Palette));
	clydePalette->colors[0] = 0x000000;
	clydePalette->colors[3] = 0xFFB732;
	clydePalette->paletteId = 6;
	writePalette(clydePalette);
	free(clydePalette);
}

void gbPalette() {
	Palette* gbPalette = malloc(sizeof(Palette));
	gbPalette->colors[0] = 0x000000;
	gbPalette->colors[1] = 0xFFFFFF;	// White
	gbPalette->colors[2] = 0xFFFFFF;
	gbPalette->colors[3] = 0xFFFF00;	// Yellow
	gbPalette->paletteId = 1;
	writePalette(gbPalette);
	free(gbPalette);
}

void textPalette() {
	Palette* textPalette = malloc(sizeof(Palette));
	textPalette->colors[0] = 0x000000;
	textPalette->colors[1] = 0xFFFFFF;
	textPalette->colors[2] = 0xFFFFFF;
	textPalette->colors[3] = 0xFFFFFF;
	textPalette->paletteId = 0;
	writePalette(textPalette);
	free(textPalette);
}

void wallPalette() {
	Palette* wallPalette = malloc(sizeof(Palette));
	wallPalette->colors[0] = 0x0000FF;
	wallPalette->colors[1] = 0x000000;
	wallPalette->colors[2] = 0x000000;
	wallPalette->colors[3] = 0x000000;
	wallPalette->paletteId = 2;
	writePalette(wallPalette);
	free(wallPalette);
}
// This function initializes the screen.
void initScreen() {
	textPalette();
	clearScreen();

	writeText(0,0,"Score",0);
	writeText(2,1,"000000",0);
	writeText(0,2,"HighScore",0);
	writeText(2,3,"000000",0);

	gbPalette();
	wallPalette();

	blinkyPalette();
	inkyPalette();
	pinkyPalette();
	clydePalette();

	Sprite candy_left;
	Sprite candy_right;
	Sprite scandy_left;
	Sprite scandy_right;
	candy_left.sprite_id = 1;
	candy_right.sprite_id = 2;
	scandy_left.sprite_id = 3;
	scandy_right.sprite_id = 4;
	char candy_matl[16][8] = {
			"        ",
			"        ",
			"        ",
			"        ",
			"        ",
			"      **",
			"     *@@",
			"    *@@@",
			"    *@@@",
			"     *@@",
			"      **",
			"        ",
			"        ",
			"        ",
			"        ",
			"        ",
	};
	char candy_matr[16][8] = {
			"        ",
			"        ",
			"        ",
			"        ",
			"        ",
			"**      ",
			"@@*     ",
			"@@@*    ",
			"@@@*    ",
			"@@*     ",
			"**      ",
			"        ",
			"        ",
			"        ",
			"        ",
			"        ",
	};
	char scandy_matl[16][8] = {
			"        ",
			"        ",
			"       *",
			"      *@",
			"      *@",
			"      *@",
			"   ***@@",
			"  *@@@@@",
			"  *@@@@@",
			"   ***@@",
			"      *@",
			"      *@",
			"      *@",
			"       *",
			"        ",
			"        ",
	};
	char scandy_matr[16][8] = {
			"        ",
			"        ",
			"*       ",
			"@*      ",
			"@*      ",
			"@*      ",
			"@@***   ",
			"@@@@@*  ",
			"@@@@@*  ",
			"@@***   ",
			"@*      ",
			"@*      ",
			"@*      ",
			"*       ",
			"        ",
			"        ",
	};
	parseSprite(&candy_left, candy_matl);
	writeSprite(&candy_left);
	parseSprite(&candy_right, candy_matr);
	writeSprite(&candy_right);
	parseSprite(&scandy_left, scandy_matl);
	writeSprite(&scandy_left);
	parseSprite(&scandy_right, scandy_matr);
	writeSprite(&scandy_right);
}

// Draw the game board.
void drawBoard(Game* game) {
	// Draw the board.
	for(int i=0;i<_BOARD_W;i+=1) {
		for(int j=0;j<_BOARD_H;j+=1) {
			if(i==game->blinky.xpos && j==game->blinky.ypos) {
				continue;
			} else if(i==game->pinky.xpos && j==game->pinky.ypos) {
				continue;
			} else if(i==game->inky.xpos && j==game->inky.ypos) {
				continue;
			} else if(i==game->clyde.xpos && j==game->clyde.ypos) {
				continue;
			} else if(i==game->player.xpos && j==game->player.ypos) {
				continue;
			}
			int curr_tile_xb = j*2+20;
			int curr_tile_y = i;
			switch(game->game_map[i][j]) {
				case WALL: {
					drawTile(curr_tile_xb, curr_tile_y, 2, 0);
					drawTile(curr_tile_xb+1, curr_tile_y, 2, 0);
					break;
				}
				case CANDY: {
					drawTile(curr_tile_xb, curr_tile_y, 1, 129);
					drawTile(curr_tile_xb+1, curr_tile_y, 1, 130);
					break;
				}
				case WAY: {
					drawTile(curr_tile_xb,curr_tile_y, 1, 0);
					drawTile(curr_tile_xb+1,curr_tile_y, 1, 0);
					break;
				}
				case SUPER: {
					drawTile(curr_tile_xb, curr_tile_y, 1, 131);
					drawTile(curr_tile_xb+1, curr_tile_y, 1, 132);
					break;
				}
			}
		}
	}

	// Draw player
	drawTile(game->player.ypos*2+20, game->player.xpos, 1, 1);
	drawTile(game->player.ypos*2+21, game->player.xpos, 1, 2);
	// Draw ghosts.
	drawTile(game->blinky.ypos*2+20, game->blinky.xpos, 3, 3);
	drawTile(game->blinky.ypos*2+21, game->blinky.xpos, 3, 4);
	drawTile(game->pinky.ypos*2+20, game->pinky.xpos, 4, 3);
	drawTile(game->pinky.ypos*2+21, game->pinky.xpos, 4, 4);
	drawTile(game->inky.ypos*2+20, game->inky.xpos, 5, 3);
	drawTile(game->inky.ypos*2+21, game->inky.xpos, 5, 4);
	drawTile(game->clyde.ypos*2+20, game->clyde.xpos, 6, 3);
	drawTile(game->clyde.ypos*2+21, game->clyde.xpos, 6, 4);
}

// Update board. change mimimal tiles.
void updateBoard(Game* game) {
	while(hasRemain()) {
		TileCollection k = getOne();
		if(k.tile_x < 0) return;
		if(k.tile_x==game->blinky.xpos && k.tile_y==game->blinky.ypos) {
			continue;
		} else if(k.tile_x==game->pinky.xpos && k.tile_y==game->pinky.ypos) {
			continue;
		} else if(k.tile_x==game->inky.xpos && k.tile_y==game->inky.ypos) {
			continue;
		} else if(k.tile_x==game->clyde.xpos && k.tile_y==game->clyde.ypos) {
			continue;
		}
		int curr_tile_xb = k.tile_y*2+20;
		int curr_tile_y = k.tile_x;
		switch(game->game_map[k.tile_x][k.tile_y]) {
			case WALL: {
				drawTile(curr_tile_xb, curr_tile_y, 2, 0);
				drawTile(curr_tile_xb+1, curr_tile_y, 2, 0);
				break;
			}
			case CANDY: {
				drawTile(curr_tile_xb, curr_tile_y, 1, 129);
				drawTile(curr_tile_xb+1, curr_tile_y, 1, 130);
				break;
			}
			case WAY: {
				drawTile(curr_tile_xb,curr_tile_y, 1, 0);
				drawTile(curr_tile_xb+1,curr_tile_y, 1, 0);
				break;
			}
			case SUPER: {
				drawTile(curr_tile_xb, curr_tile_y, 1, 131);
				drawTile(curr_tile_xb+1, curr_tile_y, 1, 132);
				break;
			}
		}
	}

	// Draw player
	drawTile(game->player.ypos*2+20, game->player.xpos, 1, 1);
	drawTile(game->player.ypos*2+21, game->player.xpos, 1, 2);
	// Draw ghosts.
	drawTile(game->blinky.ypos*2+20, game->blinky.xpos, 3, 3);
	drawTile(game->blinky.ypos*2+21, game->blinky.xpos, 3, 4);
	drawTile(game->pinky.ypos*2+20, game->pinky.xpos, 4, 3);
	drawTile(game->pinky.ypos*2+21, game->pinky.xpos, 4, 4);
	drawTile(game->inky.ypos*2+20, game->inky.xpos, 5, 3);
	drawTile(game->inky.ypos*2+21, game->inky.xpos, 5, 4);
	drawTile(game->clyde.ypos*2+20, game->clyde.xpos, 6, 3);
	drawTile(game->clyde.ypos*2+21, game->clyde.xpos, 6, 4);

	clearQueue();
    // Draw score.
    char[8] score_c, hiscore_c;
    sprintf(score_c, "  %06d", game->player.points);
    sprintf(hiscore_c, "  %06d", game->highscore);
    writeText(2,1,score_c,0);
    writeText(2,3,hiscore_c,0);
}

#ifdef __cplusplus
}
#endif
