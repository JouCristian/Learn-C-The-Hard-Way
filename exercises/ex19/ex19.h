#ifndef EX19_H
#define EX19_H

#include "object.h"

typedef struct Monster{
	Object proto;
	int hit_points;
}Monster;

int Monster_init(void *self);

int Monster_attack(void *self, int damage);

typedef struct Room{
	Object proto;

	Monster *bad_guy;

	struct Room *north;
	struct Room *south;
	struct Room *east;
	struct Room *west;
}Room;

void *Room_move(void *self, Direction direction);

int Room_attack(void *self, int damage);

typedef struct Map{
	Object proto;
	Room *start;
	Room *location;
}Map;

int Map_init(void *self);

void Map_destroy(void *self);

void *Map_move(void *self, Direction direction);

int Map_attack(void *self, int damage);

#endif
