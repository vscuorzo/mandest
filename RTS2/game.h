#include <stdlib.h>
#include <string.h>
//#include "SDL.h"
//#include "SDL_image.h"
#include "graphics.h"
#include "entity.h"

#define MAX_PLAYERS 3
#define MAX_ENTS 50
#define MAX_UNIT_TYPES 2
#define MAX_STRUCT_TYPES 2
#define MAX_UNIT_QUEUE 20
#define MAX_STRUCT_QUEUE 10
#define MAPX 4000
#define MAPY 4000
#define GRID_SIZE 2000000

#define SNDFX_SEL 0
#define SNDFX_MOVE 1
#define SNDFX_DIE 2
#define SNDFX_ATT 3
#define SNDFX_IDLE 4


SDL_Surface *map;

// scroll speed
float sSpeed;

//SDL_Surface *map;

typedef enum {false, true}gBool;

// entity structure for units/objects
typedef struct ent_s
{
	// pathfinding
	Uint32 locx;					// current location
	Uint32 locy;	
	Uint32 destx;					// destination
	Uint32 desty;
	Uint32 rallyx;					// rally point/temporary waypoint
	Uint32 rallyy;
	Uint32 glocx;					// guard location
	Uint32 glocy;
	gBool onPath;					// is this unit moving to a waypoint

	//Uint16 price;					// cost of this unit
	Sint16 health;
	Uint16 armor;
	Uint16 price;
	gBool used;						// is this ent in use
	Sint8 owner; 					// which player owns this unit
	gBool isDead;					// treat this unit as dead **does not free entity**
	gBool isUnit;					// is this a unit
	gBool building;					// is this ent being built
	struct ent_s *target;			// unit being targeted
	Uint16 xp;						// unit experience
	Uint8 speed;					// unit speed
	Uint16 scanRange;				// how far does this unit scan for enemies
	Uint16 sightRange;				// how far can this unit see
	Uint16 fireRange;				// how far can this unit engage a target from
	Uint8 ROF;						// rate of fire
	unsigned int nextFire;			// next time ent can fire their weapon
	Sprite *sprite;					// sprite image
	unsigned int frame;				// current frame of animation
	Uint32 nextUpdate;				// next time ent should update
	Uint16 upFreq;					// how many frames to wait before an update
	Uint16 queueTime;				// remaining build time
	Uint8 ROE;						// rules of engagement - 0 for passive, 1 for guarding, 2 for aggressive
	Sint16 queue[MAX_UNIT_QUEUE];	// production queue
	Uint8 qPos;						// queue position

	// sounds
	Mix_Chunk *sAtt[5];				// attack sound
	Mix_Chunk *sDie[5];				// die sound
	Mix_Chunk *sSel[5];				// select sound
	Mix_Chunk *sMove[5];			// move sound
	Mix_Chunk *sIdle[5];			// idle sound

	// functions
	void (*think)(struct ent_s *self);
	void (*update)(struct ent_s *self);
	void (*fire)(struct ent_s *self);
	void (*die)(struct ent_s *self);
}ent_t;

// client structure for players
typedef struct client_s
{
	ent_t ents[MAX_ENTS];				// entity list
	gBool used;							// is there a client here
	Uint8 faction;
	Sint32 score;
	Uint8 pop;							// population, number of units
	Uint8 team;
	ent_t *sEnt;						// selected entity
	gBool inGame;
	gBool sLoc;							// selecting a location for rally points or building placement
	ent_t *pFact;						// primary factory
	ent_t *pBarr;						// primary barracks
	Sint16 sQueue[MAX_STRUCT_QUEUE];	// construction queue
	Uint8 qPos;							// queue position
	Sint8 placing;						// value of building being placed, 0 if none
}client_t;

// entity.c
void loadUnitInfo();
void loadStructInfo();
void selectEnt(int mx, int my);
void orderEnt(int mx, int my);
void initEntList(Uint8 player);
void initPlayerList(Uint8 numPlayers);
void startGame(Uint8 numPlayers);
void thinkAll();
void updateAll();
void drawEnts();
ent_t *spawn(int owner);
ent_t *queueUnit(ent_t *par, uInfo unit);
ent_t *queueStructure(int owner, int x, int y, sInfo structure);
void placeStruct(Uint8 num);
void placeStruct2();
void dequeueUnit();
void dequeueStruct();
void enqueueUnit(Uint8 num);
void enqueueStruct(Uint8 str);
void freeEnt(ent_t *ent);
void endGame();
void initGame();
extern client_t players[];
//extern ent_t ***grid;
extern ent_t *grid[];
extern uInfo units[];
extern sInfo structs[];


// update.c
void building(ent_t *self);
void battleLordUpdate(ent_t *self);
void factoryUpdate(ent_t *self);
void factory2Update(ent_t *self);

// think.c
void battleLordThink(ent_t *self);

// fire.c
void minigun(ent_t *self);
void minigun2(ent_t *self);

// utils.c
void trim(char *s1);
int m_strncmp(char *s1, char *s2, int max);
int random(int range);
ent_t *collision(ent_t *self, int x, int y, int w, int h);
//ent_t *findradius (ent_t *from, vec3_t org, int rad);

// hud.c
void closeMenu();
void resetHUD();

// audio.c
void initAudio();
void playSound(ent_t *self, Uint8 type);

// fio.c


extern SDL_Rect Camera;
extern SDL_Surface *buffer; /*pointer to the draw buffer*/
extern SDL_Surface *screen;
extern Sprite *Msprite;
extern Uint32 NOW;	// current clock tick, used for timers
