#ifndef OBJECT_H
#define OBJECT_H

#include<stddef.h>

typedef enum Direction{
	NORTH,
	SOUTH,
	EAST,
	WEST
}Direction;

typedef struct Object{
	char *description;
	int (*init)(void *self);
	void (*describe)(void *self);
	void (*destroy)(void *self);
	void *(*move)(void *self, Direction direction);
	int (*attack)(void *self, int damage);
}Object;

int Object_init(void *self);
void Object_describe(void *self);
void Object_destroy(void *self);
void *Object_move(void *self,Direction direction);
int Object_attack(void *self, int damage);

void *Object_new(size_t size, Object proto, const char *description);

#define NEW(T,N) Object_new(sizeof(T),T##Proto,(N))

#define _(N) proto.N

#endif
