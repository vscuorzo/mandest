//#include "game.h"
#include "game.h"

//const unsigned int gsize = MAPX*MAPY*.1;

client_t players[MAX_PLAYERS];	// player list
//ent_t ***grid;				// grid overlaying the map, used for collisions and pathfinding
//ent_t *grid[400000];
ent_t *grid[GRID_SIZE];

// scroll speed default
sSpeed = 0.7;

// List of all units
uInfo units[MAX_UNIT_TYPES];

// List of all structures
sInfo structs[MAX_STRUCT_TYPES];

// used for placing buildings
gBool place = false;

// sets functions
void setUnitThink(uInfo *unit, char *cmd)
{
	if( strcmp(cmd, "battleLordThink") == 0 )
		unit->think = battleLordThink;
	else if( strcmp(cmd, "spiderDemonThink") == 0 )
		unit->think = battleLordThink;
}

void setUnitUpdate(uInfo *unit, char *cmd)
{
	if( strcmp(cmd, "battleLordUpdate") == 0 )
		unit->update = battleLordUpdate;
	else if( strcmp(cmd, "spiderDemonUpdate") == 0 )
		unit->update = battleLordUpdate;
}

void setUnitFire(uInfo *unit, char *cmd)
{
	if( strcmp(cmd, "minigun") == 0 )
		unit->fire = minigun;
	else if( strcmp(cmd, "minigun2") == 0 )
		unit->fire = minigun2;
}

void setStructFunc(sInfo *unit, char *cmd)
{
}

// sets all unit data
void loadUnitInfo()
{
	FILE *stats;		// config file
	char cmd[30];		// command string
	uInfo *unit;		// which panel the command applies to
	Uint8 i;
	Uint32 value;
	char svalue[30];

	// Open the config file, load defaults if it fails
	stats = fopen("config/ustats.txt","r");
	if(stats == NULL)
	{
		fprintf(stderr,"ustats.txt failed to open, using defaults\n");

		units[BATTLELORD].health = 20;
		units[BATTLELORD].armor = 0;
		units[BATTLELORD].speed = 2;
		units[BATTLELORD].price = 100;
		units[BATTLELORD].queueTime = 100;
		units[BATTLELORD].scanRange = 1000;
		units[BATTLELORD].fireRange = 210;
		units[BATTLELORD].ROE = 2;
		units[BATTLELORD].ROF = 50;
		units[BATTLELORD].spriteName = "spr/battlelord.gif";
		units[BATTLELORD].r = 255;
		units[BATTLELORD].g = 255;
		units[BATTLELORD].b = 255;
		units[BATTLELORD].sizex = 115;
		units[BATTLELORD].sizey = 125;
		units[BATTLELORD].think = battleLordThink;
		units[BATTLELORD].update = battleLordUpdate;
		units[BATTLELORD].fire = minigun;
		units[BATTLELORD].upFreq = 500;

		units[SPIDERDEMON].health = 10;
		units[SPIDERDEMON].armor = 0;
		units[SPIDERDEMON].speed = 4;
		units[SPIDERDEMON].price = 100;
		units[SPIDERDEMON].queueTime = 250;
		units[SPIDERDEMON].scanRange = 1000;
		units[SPIDERDEMON].fireRange = 300;
		units[SPIDERDEMON].ROE = 2;
		units[SPIDERDEMON].ROF = 30;
		units[SPIDERDEMON].spriteName = "spr/arachnotron.gif";
		units[SPIDERDEMON].r = 255;
		units[SPIDERDEMON].g = 255;
		units[SPIDERDEMON].b = 255;
		units[SPIDERDEMON].sizex = 100;
		units[SPIDERDEMON].sizey = 55;
		units[SPIDERDEMON].think = battleLordThink;
		units[SPIDERDEMON].update = battleLordUpdate;
		units[SPIDERDEMON].fire = minigun2;
		units[SPIDERDEMON].upFreq = 500;
	}

	// FIX ME
/*	else
	{
		// Load settings in file
		while( fgets(cmd,30,stats) != NULL )
		{
			fprintf(stderr,"cmd: %s\n",cmd);

			if( m_strncmp(cmd,"battlelord",11) )
				unit = &battleLord;
			else if( m_strncmp(cmd,"spiderdemon",12) )
				unit = &spiderDemon;

			if(unit == NULL)
			{
				fprintf(stderr,"Unit is null.\n");
				break;
			}

			while( fscanf(stats,"%s%u",&cmd,&value) != NULL )
			{
				fprintf(stderr,"CMD: %s\n",cmd);
				fprintf(stderr,"value: %u\n",value);

				if( m_strncmp(cmd,"health",7) == 0 )
				{
					unit->health = value;
					fprintf(stderr,"health: %i\n",unit->health);
				}
				else if( m_strncmp(cmd,"armor",6) )
					unit->armor = value;
				else if( m_strncmp(cmd,"speed",6) )
					unit->speed = value;
				else if( m_strncmp(cmd,"price",6) )
					unit->price = value;
				else if( m_strncmp(cmd,"queuetime",10) )
					unit->queueTime = value;
				else if( m_strncmp(cmd,"scanrange",10) )
					unit->scanRange = value;
				else if( m_strncmp(cmd,"firerange",10) )
					unit->fireRange = value;
				else if( m_strncmp(cmd,"ROE",4) )
					unit->ROE = value;
				else if( m_strncmp(cmd,"ROF",4) )
					unit->ROF = value;
				else if( m_strncmp(cmd,"spritename",11) )
					unit->spriteName = value;
				else if( m_strncmp(cmd,"r",2) )
					unit->r = value;
				else if( m_strncmp(cmd,"g",2) )
					unit->g = value;
				else if( m_strncmp(cmd,"b",2) )
					unit->b = value;
				else if( m_strncmp(cmd,"sizex",6) )
					unit->sizex = value;
				else if( m_strncmp(cmd,"sizey",6) )
					unit->sizey = value;
			}

			while( fscanf(stats,"%s%s",&cmd,&svalue) != NULL )
			{
				if( m_strncmp(cmd,"think",6) == 0 )
				{
					setUnitThink(unit, cmd);
				}

				else if( m_strncmp(cmd,"update",7) == 0 )
				{
					setUnitUpdate(unit, cmd);
				}

				else if( m_strncmp(cmd,"fire",5) == 0 )
				{
					setUnitFire(unit, cmd);
				}

				
			}
			
			fscanf(stats,"%s%u",&value);
			if( m_strncmp(cmd,"upfreq",7) == 0 )
				unit->upFreq = value;

			continue;
		}
		fclose(stats);
	}*/
}

// sets all structure data
void loadStructInfo()
{
	structs[FACTORY].health = 1000;
	structs[FACTORY].armor = 1000;
	structs[FACTORY].price = 100;
	structs[FACTORY].queueTime = 100;
	structs[FACTORY].spriteName = "images/SPACE3.bmp";
	structs[FACTORY].r = 0;
	structs[FACTORY].g = 0;
	structs[FACTORY].b = 0;
	structs[FACTORY].sizex = 160;
	structs[FACTORY].sizey = 210;
	structs[FACTORY].update = factoryUpdate;
	structs[FACTORY].upFreq = 5555;

	structs[BARRACKS].health = 1000;
	structs[BARRACKS].armor = 1000;
	structs[BARRACKS].price = 100;
	structs[BARRACKS].queueTime = 100;
	structs[BARRACKS].spriteName = "images/SPACE3.bmp";
	structs[BARRACKS].r = 0;
	structs[BARRACKS].g = 0;
	structs[BARRACKS].b = 0;
	structs[BARRACKS].sizex = 160;
	structs[BARRACKS].sizey = 210;
	structs[BARRACKS].update = factory2Update;
	structs[BARRACKS].upFreq = 9999;
}

// selects an entity
void selectEnt(int mx, int my)
{
	ent_t *temp;

/*	Uint8 i,j;

	i = 0;
	while(players[i].used == 1)
	{
		for(j=0;j<MAX_ENTS;j++)
		{
			if( (players[i].ents[j].used != true) && (players[i].ents[j].isDead != false) ) continue;
			
			if( (abs(players[i].ents[j].locx - mx) < 25) && (abs(players[i].ents[j].locy - my) < 25) )
			{
				players[0].sEnt = &(players[i].ents[j]);
				fprintf(stderr,"Ent selected: %p\n",players[0].sEnt);
				return;
			}
		}
		++i;
	}
	//if(players[0].sEnt != NULL)
*/
	temp = collision(NULL,mx+Camera.x,my+Camera.y,25,25);
	if( (temp != NULL) && (temp->used == true) && (temp->isDead == false) )
	{
		players[0].sEnt = temp;
		playSound(players[0].sEnt,SNDFX_SEL);
		fprintf(stderr,"Ent selected: %p\n",players[0].sEnt);
	}
	else
		players[0].sEnt = NULL;
}

// give selected entity an order
void orderEnt(int mx, int my)
{
//	Uint8 i,j;
	ent_t *temp;

	if(players[0].sEnt == NULL || players[0].sEnt->owner != 0 || players[0].sEnt->used == false) return;

/*	i = 0;
	while(players[i].used == 1)
	{
		for(j=0;j<MAX_ENTS;j++)
		{
			if( (players[i].ents[j].used != true) || (players[i].ents[j].isDead == true) ||(players[i].ents[j].owner == players[0].sEnt->owner) ) continue;
			
			if( (abs(players[i].ents[j].locx - mx) < 25) && (abs(players[i].ents[j].locy - my) < 25) )
			{
				temp = &(players[i].ents[j]);
				return;
			}
		}
		++i;
	}
*/
	temp = collision(players[0].sEnt,mx+Camera.x,my+Camera.y,players[0].sEnt->sprite->w,players[0].sEnt->sprite->h);
	if(temp == NULL)
	{
		// set new rally point for structures/waypoint for units
		players[0].sEnt->rallyx = mx+Camera.x;
		players[0].sEnt->rallyy = my+Camera.y;
	

		// if it's a unit, make it move to that location
		if(players[0].sEnt->isUnit == true)
		{
			fprintf(stderr,"Unit ordered to move\n");
			players[0].sEnt->onPath = true;
			players[0].sEnt->destx = mx+Camera.x;
			players[0].sEnt->desty = my+Camera.y;
			players[0].sEnt->target = NULL;
			playSound(players[0].sEnt,SNDFX_MOVE);
		}
		else
			fprintf(stderr,"Structure rally point changed\n");
	}
	else if(players[0].sEnt->isUnit == true)//if(temp->owner != 0)
	{
		players[0].sEnt->rallyx = players[0].sEnt->locx;
		players[0].sEnt->rallyy = players[0].sEnt->locy;
		players[0].sEnt->target = temp;
		players[0].sEnt->onPath = true;
		playSound(players[0].sEnt,SNDFX_ATT);
		fprintf(stderr,"Unit ordered to attack\n");
	}
}

// initialize entity list
void initEntList(Uint8 player)
{
	Uint8 i,j;
	for(i=0;i<MAX_ENTS;i++)
	{
		players[player].ents[i].used = false;
		players[player].ents[i].owner = -1;
		players[player].ents[i].isDead = false;
		players[player].ents[i].xp = 0;
		players[player].ents[i].frame = 0;
		players[player].ents[i].onPath = false;
		players[player].ents[i].ROE = 2;
		players[player].ents[i].qPos = 0;
		for(j=0;j<MAX_UNIT_QUEUE;j++)
		{
			players[player].ents[i].queue[j] = -1;
		}
	}
}

// initialize player list
void initPlayerList(Uint8 numPlayers)
{
	Uint8 i,j;
	for(i=0;i<numPlayers;i++)
	{
		players[i].used = true;
		initEntList(i);
		players[i].score = 0;
		players[i].pop = 0;
		players[i].sEnt = 0;
		players[i].team = 0;
		players[i].inGame = true;
		players[i].qPos = 0;
		players[i].placing = -1;
		for(j=0;j<MAX_STRUCT_QUEUE;j++)
		{
			players[i].sQueue[j] = -1;
		}
	}
}

// start a game
void startGame(Uint8 numPlayers)
{
	/*ent_t ***rptr;
    ent_t **aptr;
	Uint16 rows, cols;
	Uint16 i;

	rows = MAPX*.1;
	cols = MAPY*.1;

	// allocate memory for the grid
	aptr = malloc(rows * cols * sizeof(ent_t));
	rptr = malloc(rows * sizeof(ent_t *));
	for (i = 0; i < rows; i++)
    {
        rptr[i] = aptr + (i * cols);
    }
	grid = rptr;
*/
	initPlayerList(numPlayers);
	loadUnitInfo();
	loadStructInfo();
	closeMenu();
}

// make entities think
void thinkAll()
{
	Uint8 i,j;

	i = 0;
	while(players[i].used == 1)
	{
		for(j=0;j<MAX_ENTS;j++)
		{
			if( (players[i].ents[j].used == true) && (players[i].ents[j].think != NULL) && (players[i].ents[j].isDead == false) )
			{
				players[i].ents[j].think( &(players[i].ents[j]) );
				//fprintf(stderr,"thinking for entity # %i\n",j);
			}
		}
		++i;
	}
}

// update entities
void updateAll()
{
	int i, j;

	i = 0;
	while(players[i].used == 1)
	{
		for(j=0;j<MAX_ENTS;j++)
		{
			if(players[i].ents[j].used == 1)
			{
				// if the ent is not something being built, update it
				if(players[i].ents[j].building == false)
				{
					if(players[i].ents[j].update != NULL)
						players[i].ents[j].update(&(players[i].ents[j]));
				}

				// otherwise continue construction
				else
				{
					building( &(players[i].ents[j]) );
					continue;
				}
			}
		}
		i++;
	}
}

// draw and update all entities
void drawEnts()
{
	Uint8 i,j;
	SDL_Rect bbox;

	i = 0;
	while(players[i].used == 1)
	{
		for(j=0;j<MAX_ENTS;j++)
		{
			// make sure ent is in use and has a sprite before trying to draw it
			if(players[i].ents[j].used == false) continue;
			if(players[i].ents[j].sprite == NULL) continue;
			if(players[i].ents[j].building == true)
			{
				if(players[i].ents[j].isUnit == false)
				{
					SDL_SetAlpha(players[i].ents[j].sprite->image, SDL_SRCALPHA|SDL_RLEACCEL, 150);
					DrawSprite(players[i].ents[j].sprite,buffer,(players[i].ents[j].locx-(players[i].ents[j].sprite->w*.5)),(players[i].ents[j].locy-(players[i].ents[j].sprite->h*.5)),players[i].ents[j].frame);
					SDL_SetAlpha(players[i].ents[j].sprite->image, SDL_SRCALPHA|SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
					continue;
				}
				else
					continue;
			}
			//fprintf(stderr,"ent # %i\n",j);
			
			bbox.x = (players[i].ents[j].locx-(players[i].ents[j].sprite->w*.5));
			bbox.y = (players[i].ents[j].locy-(players[i].ents[j].sprite->h*.5));
			bbox.w = players[i].ents[j].sprite->w;
			bbox.h = players[i].ents[j].sprite->h;
			if(&(players[i].ents[j]) == players[0].sEnt)
				SDL_FillRect ( buffer , &bbox , SDL_MapRGB ( buffer->format , 200 , 200 , 200 )) ;

			DrawSprite(players[i].ents[j].sprite,buffer,(players[i].ents[j].locx-(players[i].ents[j].sprite->w*.5)),(players[i].ents[j].locy-(players[i].ents[j].sprite->h*.5)),players[i].ents[j].frame);
		}
		++i;
	}
}

// spawn an entity and return a pointer to it
ent_t *spawn(int owner)
{
	Uint8 i;
	for(i=0;i<MAX_ENTS;i++)
	{
		if(players[owner].ents[i].used == false)
		{
			players[owner].ents[i].building = true;
			players[owner].ents[i].owner = owner;
			players[owner].ents[i].used = true;
			return &(players[owner].ents[i]);
		}
	}
	return NULL;
}

// Adds unit to the queue, given the ent that spawns it and the unit stats
ent_t *queueUnit(ent_t *par, uInfo unit)
{
	Uint8 i;
	ent_t *ent = spawn(par->owner);
	if(ent == NULL) {return NULL;}
	fprintf(stderr,"QueueUnit called\n");

	ent->sprite = LoadSprite(unit.spriteName,unit.sizex,unit.sizey);
	SDL_SetColorKey(ent->sprite->image, SDL_SRCCOLORKEY , SDL_MapRGB(ent->sprite->image->format, unit.r, unit.g, unit.b));
	ent->owner = par->owner;
	ent->locx = par->locx;
	ent->locy = par->locy + (par->sprite->h * .5) + (ent->sprite->h * .5);
	ent->rallyx = par->rallyx;
	ent->rallyy = par->rallyy + (ent->sprite->h * .5);
	ent->destx = par->rallyx;
	ent->desty = par->rallyy + (ent->sprite->h * .5);
	ent->onPath = true;

	ent->health = unit.health;
	ent->armor = unit.armor;
	ent->speed = unit.speed;
	ent->price = unit.price;
	ent->queueTime = unit.queueTime;
	ent->scanRange = unit.scanRange;
	ent->fireRange = unit.fireRange;
	ent->ROE = unit.ROE;
	ent->think = unit.think;
	ent->update = unit.update;
	ent->fire = unit.fire;
	ent->upFreq = unit.upFreq;
	ent->isUnit = true;
	ent->isDead = true;

	for(i=0;i<5;i++)
	{
		if(unit.sAtt[i] != NULL)
			ent->sAtt[i] = unit.sAtt[i];
		if(unit.sMove != NULL)
			ent->sMove[i] = unit.sMove[i];
		if(unit.sDie[i] != NULL)
			ent->sDie[i] = unit.sDie[i];
		if(unit.sSel[i] != NULL)
			ent->sSel[i] = unit.sSel[i];
		if(unit.sIdle[i] != NULL)
			ent->sIdle[i] = unit.sIdle[i];
	}
	//fprintf(stderr,"ent in use: %i\n",ent->used);
	return ent;
}

// Adds a structure to the queue, given the player that spawns it, a location, and the structure stats
ent_t *queueStructure(int owner, int x, int y, sInfo structure)
{
	ent_t *ent = spawn(owner);
	if(ent == NULL) {return NULL;}

	ent->health = structure.health;
	ent->armor = structure.armor;
	ent->price = structure.price;
	ent->queueTime = structure.queueTime;
	ent->update = structure.update;
	ent->upFreq = structure.upFreq;
	ent->sprite = LoadSprite(structure.spriteName,structure.sizex,structure.sizey);
	SDL_SetColorKey(ent->sprite->image, SDL_SRCCOLORKEY , SDL_MapRGB(ent->sprite->image->format, structure.r, structure.g, structure.b));

	ent->locx = x + Camera.x;
	ent->locy = y + Camera.y;
	ent->rallyx = x + Camera.x;
	ent->rallyy = y + (ent->sprite->h * .5) + 100 + Camera.y;
	ent->isUnit = false;
	ent->isDead = true;
	grid[(int)(ent->locx*ent->locy*.1)] = ent;
	return ent;
}

// Place a structure
void placeStruct(Uint8 num)
{
	players[0].placing = num;
}

void placeStruct2()
{
	int mx = NULL;
	int my = NULL;
	ent_t *temp;
	Uint8 state;
	Sprite *s;
	SDL_Rect bbox;
	Uint8 num = players[0].placing;

	state = SDL_GetMouseState(&mx,&my);
	s = LoadSprite(structs[num].spriteName,structs[num].sizex,structs[num].sizey);

	// if the player right clicks, cancel
	if( state &SDL_BUTTON_RMASK )
		players[0].placing = -1;

	// check for a collision at cursor location
	temp = collision(NULL,mx,my,structs[num].sizex,structs[num].sizey);
	if(temp == NULL)
	{
		if(state &SDL_BUTTON_LMASK && place == true)
		{
			queueStructure(0,mx,my,structs[num]);
			players[0].placing = -1;
			place = false;
		}
		else if( !state &SDL_BUTTON_LMASK )
			place = true;
	}
	// if found, draw a red box
	else
	{
		bbox.x = mx - s->w*.5;
		bbox.y = my - s->h*.5;
		bbox.w = s->w;
		bbox.h = s->h;
		SDL_FillRect(screen, &bbox, SDL_MapRGBA(screen->format, 200, 0, 0, 100) );
	}
	
	if(s != NULL)
	{
		SDL_SetColorKey(s->image, SDL_SRCCOLORKEY, SDL_MapRGB(s->image->format, structs[num].r, structs[num].g, structs[num].b));
		SDL_SetAlpha(s->image,SDL_RLEACCEL|SDL_SRCALPHA,100);
		DrawSprite(s, screen, mx - s->w*.5, my - s->h*.5, 0);
		SDL_SetAlpha(s->image,SDL_RLEACCEL|SDL_SRCALPHA,SDL_ALPHA_OPAQUE);
	}
}

// Dequeue
void dequeueUnit()
{
	if(players[0].pBarr != NULL)
	{
		switch(players[0].pBarr->queue[players[0].pBarr->qPos])
		{
		case BATTLELORD:
			if(queueUnit(players[0].pBarr,units[BATTLELORD]))
			{
				players[0].pBarr->queue[players[0].pBarr->qPos] = -1;
				players[0].pBarr->qPos = (players[0].pBarr->qPos+1)%MAX_UNIT_TYPES;
			}
			break;
		default:
			fprintf(stderr,"Dequeue failed.\n");
		}
	}

	if(players[0].pFact != NULL)
	{
		switch(players[0].pFact->queue[players[0].pFact->qPos])
		{
		case SPIDERDEMON:
			if(queueUnit(players[0].pFact,units[SPIDERDEMON]))
			{
				players[0].pFact->queue[players[0].pFact->qPos] = -1;
				players[0].pFact->qPos = (players[0].pFact->qPos+1)%MAX_UNIT_TYPES;
			}
			break;
		default:
			fprintf(stderr,"Dequeue failed.\n");
		}
	}
}

// Enqueue unit
void enqueueUnit(Uint8 unit)
{
	int i = 0;
	Uint8 *pos;
	Sint16 *q;

	fprintf(stderr,"unit: %i\n",unit);
	switch(unit)
	{
	case BATTLELORD:
		if(players[0].pBarr == NULL)
			return;
		q = &players[0].pBarr->queue[0];
		pos = &players[0].pBarr->qPos;
		break;
	case SPIDERDEMON:
		if(players[0].pFact == NULL)
			return;
		q = &players[0].pFact->queue[0];
		pos = &players[0].pFact->qPos;
		break;
	default:
		fprintf(stderr,"Enqueue unit failed.\n");
		return;
	}

	if(q == NULL)
	{
		fprintf(stderr,"q is null\n");
		return;
	}
	else
		fprintf(stderr,"q is %i\n",*q);

	if(pos == NULL)
	{
		fprintf(stderr,"pos is null\n");
		return;
	}
	else
		fprintf(stderr,"pos is %i\n",*pos);

	// look for an open slot and fill it
	while(i < MAX_UNIT_QUEUE)
	{
		if(q[*pos] < 0)
		{
			q[*pos] = unit;
			fprintf(stderr,"unit: %i\n",q[*pos]);
			return;
		}
		*pos = (*pos+1)%MAX_UNIT_QUEUE;
		++i;
	}
	
	fprintf(stderr,"Enqueue unit failed.\n");
}

// Enqueue structure
void enqueueStruct(Uint8 str)
{
	int i = 0;
	int pos = players[0].qPos;

	fprintf(stderr,"enqueue structure called, str = %i\n",str);
	// look for an open slot and fill it
	while(i < MAX_UNIT_QUEUE)
	{
		if(players[0].sQueue[pos] < 0)
		{
			players[0].sQueue[pos] = str;
			players[0].qPos = pos;
			return;
		}
		pos = (pos+1)%MAX_UNIT_QUEUE;
		++i;
	}
	
	fprintf(stderr,"Enqueue structure failed.\n");
}

// Completely kill a unit. Resets entity data and frees the sprite.
void freeEnt(ent_t *ent)
{
	ent->used = false;
	grid[(int)(ent->locx*ent->locy*.1)] = NULL;
	ent->locx = 0;
	ent->locy = 0;
	ent->rallyx = 0;
	ent->rallyy = 0;
	ent->glocx = 0;
	ent->glocy = 0;
	ent->destx = 0;
	ent->desty = 0;
	ent->target = NULL;
	ent->xp = 0;
	ent->speed = 0;

	// clear sounds

	ent->think = NULL;
	ent->update = NULL;
	ent->fire = NULL;
//	ent->die = NULL;

	FreeSprite(ent->sprite);
	ent->sprite = NULL;
}

// Kills a player
void freePlayer(client_t *player)
{
	Uint8 i=0;

	while(i < MAX_ENTS)
	{
		fprintf(stderr,"Killing ents...\n");
		if(player->ents[i].used != NULL)
			freeEnt(&player->ents[i]);
		i++;
	}

	player->used = false;
	player->score = 0;
	player->pop = 0;
	player->sEnt = NULL;
}

// Ends the game and kills everything
void endGame()
{
	int i;

	i = 0;
	while(i < MAX_PLAYERS)
	{
		fprintf(stderr,"Player %i\n",i+1);
		if(players[i].used == true)
			freePlayer(&players[i]);
		i++;
	}

	resetHUD();
	players[0].used = 1;
}

void initGame()
{
	//HUD.menu = 1;
	//HUD.grabPanel = 0;
	players[0].used = true;
	players[0].sEnt = 0;
	players[0].inGame = false;
}

