/*
 * updater.h
 *
 *  Created on: May 3, 2020
 *      Author: leezi
 */

#ifndef UPDATER_H_
#define UPDATER_H_

#include "logic.h"

#ifdef __cplusplus
extern "C" {
#endif

void addToSet(TileCollection k);
void addPendingPos(int16_t x, int16_t y);
TileCollection getOne();
bool hasRemain();
void clearQueue();

#ifdef __cplusplus
}
#endif

#endif /* UPDATER_H_ */
