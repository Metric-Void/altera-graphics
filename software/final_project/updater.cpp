/*
 * updater.cpp
 *
 *  Created on: May 3, 2020
 *      Author: leezi
 */

#include "updater.h"

#include <set>
#include <stdint.h>
#include <stdbool.h>
#include "logic.h"

class TileGroup {
public:
	TileCollection k;
	TileGroup(TileCollection i) {
		k = i;
	}
	TileGroup(int16_t x, int16_t y) {
		k.tile_x = x;
		k.tile_y = y;
	}
	bool operator=(const TileGroup& t) const {
		return t.k.tile_x == this->k.tile_x && t.k.tile_y == k.tile_y;
	}
	bool operator<(const TileGroup& t) const {
		return t.k.tile_x < this->k.tile_x || t.k.tile_y < k.tile_y;
	}
};

std::set<TileGroup> workset;

void addToSet(TileCollection k) {
	TileGroup toInsert(k);
	workset.insert(toInsert);
}

void addPendingPos(int16_t x, int16_t y) {
	TileGroup toInsert(x,y);
	workset.insert(toInsert);
}

TileCollection getOne() {
	std::set<TileGroup>::iterator tempit = workset.begin();
	if(tempit != workset.end()) {
		TileCollection first = (*tempit).k;
		workset.erase(*tempit);
		return first;
	} else {
		TileCollection wrong;
		wrong.tile_x = -1;
		wrong.tile_y = -1;
		return wrong;
	}
}

bool hasRemain() {
	std::set<TileGroup>::iterator tempit = workset.begin();
	return tempit != workset.end();
}

void clearQueue() {
	workset.clear();
}
