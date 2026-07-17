#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "ex19.h"
#include<assert.h>

int Monster_init(void *self){
	assert(self != NULL);
	Monster *monster = self;
	monster->hit_points = 10;
	return 1;
}

int Monster_attack(void *self,int damage){
	Monster *monster = self;
	printf("You attack %s.\n",monster->_(description));
	monster->hit_points -= damage;
	if(monster->hit_points > 0){
		printf("It still has %d hit points.\n",monster->hit_points);
		return 0;
	}
	printf("It is dead.\n");
	return 1;
}

Object MonsterProto = {
	.init = Monster_init,
	.attack = Monster_attack
};

void *Room_move(void *self, Direction direction){
	Room *room = self;
	Room *next = NULL;

	switch(direction){
		case NORTH: next = room->north;break;
		case SOUTH: next = room->south;break;
		case EAST:  next = room->east; break;
		case WEST:  next = room->west; break;
		default:    next = NULL;       break;		
	}
	if(next == NULL){
		printf("You can't go that derection.\n");
		return NULL;
	}
	printf("You enter: \n");
	next->_(describe)(next);
	return next;
}

int Room_attack(void *self, int damage){
	Room *room = self;
	Monster *monster = room->bad_guy;

	if(monster == NULL){
		printf("There is nothing here to attack.\n");
		return 0;
	}
	int dead = monster->_(attack)(monster,damage);
	if(dead){
		monster->_(destroy)(monster);
		room->bad_guy = NULL;
		return 1;
	}
	return 0;
}

Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack
};

static void destroy_room(Room *room){
	if(room != NULL){
		room->_(destroy)(room);
	}
}

void Map_destroy(void *self){
	Map *map = self;
	if(map == NULL){
		return;
	}
	Room *hall = map->start;
	Room *throne = hall != NULL ? hall->north : NULL;
	Room *arena = hall != NULL ? throne->west : NULL;
	Room *kitchen = hall != NULL ? throne->east : NULL;

	if(arena != NULL && arena->bad_guy != NULL){
		arena->bad_guy->_(destroy)(arena->bad_guy);
		arena->bad_guy = NULL;
	}

	destroy_room(kitchen);
	destroy_room(arena);
	destroy_room(throne);
	destroy_room(hall);

	Object_destroy(map);
}

int Map_init(void *self){
	Map *map = self;
	Room *hall = NEW(Room, "The great hall");
	Room *throne = NEW(Room, "The throne room");
	Room *arena = NEW(Room, "The arena with the minotaur");
	Room *kitchen = NEW(Room, "The kitchen;you find a knife");
	
	if(hall == NULL || throne == NULL || arena == NULL || kitchen == NULL){
		destroy_room(kitchen);
		destroy_room(arena);
		destroy_room(throne);
		destroy_room(hall);
		return 0;
	}
	arena->bad_guy = NEW(Monster, "The evil minotuar");
	if(arena->bad_guy == NULL){
		destroy_room(kitchen);
		destroy_room(arena);
		destroy_room(throne);
		destroy_room(hall);
		return 0;
	}
	hall->north = throne;
	throne->south = hall;
	throne->west = arena;
	throne->east = kitchen;
	arena->east = throne;
	kitchen->west = throne;
	map->start = hall;
	map->location = hall;

	return 1;
}

void *Map_move(void *self, Direction direction){
	Map *map = self;
	Room *next = map->location->_(move)(map->location,direction);
	if(next != NULL){
		map->location = next;
	}
	return next;
}

int Map_attack(void *self, int damage){
	Map *map = self;
	return map->location->_(attack)(map->location,damage);
}

Object MapProto = {
	.init = Map_init,
	.destroy = Map_destroy,
	.move = Map_move,
	.attack = Map_attack
};

static void print_help(void){
	printf("Commands:\n");
	printf("  n = north\n");
	printf("  s = south\n");
	printf("  e = east\n");
	printf("  w = west\n");
	printf("  a = attack\n");
	printf("  l = list exits\n");
	printf("  q = quit\n");
}

static void list_exits(const Map *game){
	printf("You can go: \n");
	if(game->location->north != NULL){
		printf("NORTH\n");
	}
	if(game->location->south != NULL){
		printf("SOUTH\n");
	}
	if(game->location->east != NULL){
		printf("EAST\n");
	}
	if(game->location->west != NULL){
		printf("WEST\n");
	}
}

static int process_input(Map *map){
	printf("\n> ");
	int ch = getchar();
	if(ch == EOF){
		printf("\nGoodBye.\n");
		return 0;
	}
	int extra = 0;
	while((extra = getchar()) != '\n' && extra != EOF){}

	int damage = rand() % 4 + 1;
	switch(ch){
		case 'n':
			map->_(move)(map,NORTH);
			break;
		case 's':
			map->_(move)(map,SOUTH);
			break;
		case 'e':
			map->_(move)(map,EAST);
			break;
		case 'w':
			map->_(move)(map,WEST);
			break;
		case 'a':
			printf("Your attack deals %d damage.\n",damage);
			if(map->_(attack)(map,damage)){
				printf("You defeated the minotaur.\n");
				printf("You Win!\n");
				return 0;
			}
			break;
		case 'l':
			list_exits(map);
			break;
		case 'q':
			printf("You leave the castle.\n");
			return 0;
		default:
			printf("Unkown command '%c'.\n",ch);
			print_help();
			break;
	}
	return 1;
}

int main(void){
	srand((unsigned int)time(NULL));
	
	Map *game = NEW(Map, "The Hall of the Minotaur");
	if(game == NULL){
		fprintf(stderr, "ERROR: failed to creat the game.\n");
		return EXIT_FAILURE;
	}
	printf("You enter ");
	game->location->_(describe)(game->location);
	print_help();

	while(process_input(game)){}

	game->_(destroy)(game);

	return EXIT_SUCCESS;
}


