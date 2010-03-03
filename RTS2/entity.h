#include "SDL_mixer.h"

#define MAX_UNIT_TYPES 2
#define MAX_STRUCT_TYPES 2

// Unit button indices
#define BATTLELORD 0
#define SPIDERDEMON 1

// Structure button indices
#define BARRACKS 0
#define FACTORY 1

// Contains unit stats
typedef struct unitInfo
{
	char *spriteName;
	Uint8 r, g, b;			// filter color info
	Uint16 sizex, sizey;
	Uint16 price;
	Sint16 health;
	Uint16 armor;
	Uint8 speed;
	Uint16 scanRange;
	Uint16 sightRange;
	Uint16 fireRange;
	Uint8 ROE;
	Uint8 ROF;
	Uint32 nextFire;
	Uint16 upFreq;
	Uint16 queueTime;

	// sound FX file lists
	char sAttF[5][20];
	char sDieF[5][20];
	char sSelF[5][20];
	char sMoveF[5][20];
	char sIdleF[5][20];

	Mix_Chunk *sAtt[5];
	Mix_Chunk *sDie[5];
	Mix_Chunk *sSel[5];
	Mix_Chunk *sMove[5];
	Mix_Chunk *sIdle[5];

	void (*think)(struct ent_s *self);
	void (*update)(struct ent_s *self);
	void (*fire)(struct ent_s *self);
	void (*die)(struct ent_s *self);
}uInfo;

// Contains structure stats
typedef struct structInfo
{
	char *spriteName;
	Uint8 r, g, b;
	Uint16 sizex, sizey;
	Uint16 price;
	Sint16 health;
	Uint16 armor;
	Uint16 scanRange;
	Uint16 sightRange;
	Uint16 fireRange;
	Uint8 ROE;
	Uint8 ROF;
	Uint32 nextFire;
	Uint16 upFreq;
	Uint16 queueTime;

	void (*think)(struct ent_s *self);
	void (*update)(struct ent_s *self);
	void (*fire)(struct ent_s *self);
	void (*die)(struct ent_s *self);
}sInfo;

// List of all units
//extern uInfo *units;
//extern sInfo *structs;

// List of all structures
extern sInfo factory;
extern sInfo factory2;

