#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include <time.h>       /* time */
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

    printf("Player MOVE start\n");
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

    // Ghosts move.
    srand(time(nullptr));
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

// MinHeap Push.
// Data in this linked list will be sorted by distance. Smaller distance comes first.
LinkedPoints* stack_push(LinkedPoints* head, const Point* data) {
    if(head == nullptr || distances[head->curr.xpos][head->curr.ypos] >= distances[data->xpos][data->ypos]) {
        LinkedPoints* newPoint = malloc(sizeof(LinkedPoints));
        newPoint->curr = *data;
        newPoint->next = head;
        return newPoint;
    } else {
        return stack_push(head->next, data);
    }
}

// Peek the top of the stack.
static inline Point stack_peek(LinkedPoints* head) {
    return head -> curr;
}

// Pop an element. Element will be written to popped_data.
// Since elements are inserted in order, this method will always return the smallest element.
LinkedPoints* stack_pop(LinkedPoints* head, Point* popped_data) {
    *popped_data = head -> curr;
    LinkedPoints* nextptr = head -> next;
    free(head);
    return nextptr;
}

static inline bool stack_isempty(const LinkedPoints* head) {
    return head == nullptr;
}

// Wish I'm using Java?
static inline bool PointEquals(const Point a, const Point b) {
    return a.xpos == b.xpos && a.ypos == b.ypos;
}

// Temporary distance used for both blinky and pinky.
static int8_t distances[_BOARD_H][_BOARD_W];

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
            game->clyde.xpos = (uint8_t)newx;
            game->clyde.ypos = (uint8_t)newy;
            success = true;
        }
        trials += 1;
	}
}

void print_board(Game* game) {
    for(int i=0; i<15; i++) {
        for(int j=0; j<15; j++) {
            if(i==game->blinky.xpos && j==game->blinky.ypos) {
                printf("B");
				continue;
			} else if(i==game->pinky.xpos && j==game->pinky.ypos) {
			    printf("P");
				continue;
			} else if(i==game->inky.xpos && j==game->inky.ypos) {
			    printf("I");
				continue;
			} else if(i==game->clyde.xpos && j==game->clyde.ypos) {
			    printf("C");
				continue;
			} else if(i==game->player.xpos && j==game->player.ypos) {
			    printf("@");
				continue;
			}
            switch(game->game_map[i][j]){
                case WALL: {
                    printf("█");
                    break;
                }
                case WAY: {
                    printf(" ");
                    break;
                }
                case CANDY: {
                    printf("·");
                    break;
                }
                case SUPER: {
                    printf("¤");
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}
