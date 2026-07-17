#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "object.h"

int Object_init(void *self){
	(void)self;
	return 1;
}

void Object_describe(void *self){
	Object *obj = self;
	if(obj != NULL && obj->description != NULL){
		printf("%s.\n",obj->description);
	}
}

void Object_destroy(void *self){
	Object *obj = self;
	if(obj == NULL){
		return;
	}
	free(obj->description);
	free(obj);
}

void *Object_move(void *self, Direction direction){
	(void)self;
	(void)direction;

	printf("You can't go that direction.\n");

	return NULL;
}

int Object_attack(void *self, int damage){
	(void)self;
	(void)damage;
	printf("You can't attack that.\n");
	return 0;
}

void *Object_new(size_t size, Object proto, const char *description){
	if(size < sizeof(Object) || description == NULL){
		return NULL;
	}
	if(proto.init == NULL){
		proto.init = Object_init;
	}
	if(proto.describe == NULL){
		proto.describe = Object_describe;
	}
	if(proto.destroy == NULL){
		proto.destroy = Object_destroy;
	}
	if(proto.move == NULL){
		proto.move = Object_move;
	}
	if(proto.attack == NULL){
		proto.attack = Object_attack;
	}
	Object *object = calloc(1,size);
	if(object == NULL) return NULL;
	*object = proto;
	object->description = strdup(description);
	if(object->description == NULL){
		free(object);
		return NULL;
	}
	if(!object->init(object)){
		object->destroy(object);
		return NULL;
	}
	return object;
}














