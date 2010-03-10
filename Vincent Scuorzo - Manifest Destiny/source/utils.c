#include "game.h"

// wrapper for strncmp, because MSVS 2005 is retarded. Returns 1 if strings are the same and 0 if not.
int m_strncmp(char *s1, char *s2, int max)
{
	if(*s1 == '\0' && *s2 != '\0')
		return 0;
	else if(*s1 != '\0' && *s2 == '\0')
		return 0;
	else if(*s1 == '\0' && *s2 == '\0')
		return 1;
	else if(strncmp(s1,s2,max) == max)
		return 1;

	return 0;
}

// returns a random number between 1 and range, inclusive
int random(int range)
{
	// bitshift to make it more random, modulus to keep it within the range
	return (((rand()>>3)%range) + 1);
}

// checks for collisions given the source ent, test location, and size
ent_t *collision2(ent_t *self, int x1, int y1, int w, int h)
{
	Uint8 i,j;
	Uint32 xmin, xmax, ymin, ymax;

	i = 0;
	while(players[i].used == 1)
	{
		for(j=0;j<MAX_ENTS;++j)
		{
			// make sure ent is in use, has a sprite (for size info), and isn't the source ent first
			if( (players[i].ents[j].used == false) || (players[i].ents[j].sprite == NULL) || (&(players[i].ents[j]) == self) )
				continue;
		
			xmin = x1 - (w * .5);
			xmax = xmin + w;
			ymin = y1 - (h * .5);
			ymax = ymin - h;

			// check for a collision
			if( (players[i].ents[j].locx + (players[i].ents[j].sprite->w * .5)) < xmin ) continue;
			if( (players[i].ents[j].locx - (players[i].ents[j].sprite->w * .5)) > xmax ) continue;

			if( (players[i].ents[j].locy + (players[i].ents[j].sprite->h * .5)) < ymin ) continue;
			if( (players[i].ents[j].locy - (players[i].ents[j].sprite->h * .5)) > ymax ) continue;

/*
			if( (abs(x1 - players[i].ents[j].locx) > (.5 * (self->sprite->w + players[i].ents[j].sprite->w))) )
				if( (abs(y1 - players[i].ents[j].locy) > (.5 * (self->sprite->w + players[i].ents[j].sprite->w))) )
					continue;
*/
			// if found, return the address of that entity
			return &(players[i].ents[j]);
		}
		++i;
	}
	return NULL;
}

// grid-based collision detection
ent_t *collision(ent_t *self, int x, int y, int w, int h)
{
	Sint32 minx, miny, maxx, maxy, i, j;

	// determine the bounding box
	minx = (x - (w * .5)) * .1;
	miny = (y - (h * .5)) * .1;
	maxx = minx + (w  * .1);
	maxy = miny + (h  * .1);

	if(minx < 0)
		minx = 0;
	if(miny < 0)
		miny = 0;
/*
	// check the grid locations that this unit needs to enter
	if( grid[minx][miny] != NULL ) return grid[minx][miny];
	if( grid[minx][maxy] != NULL ) return grid[minx][maxy];
	if( grid[maxx][miny] != NULL ) return grid[maxx][miny];
	if( grid[maxx][maxy] != NULL ) return grid[maxx][maxy];
*/
	for(i=minx;i<maxx;i++)
	{
		for(j=miny;j<maxy;j++)
		{
			if(i*j > MAPX*MAPY)
				return NULL;
			/*
			if(grid[i][j] != NULL)
			{
				if( (grid[i][j]->used == false) || (&(*(grid[i][j])) == self) )
					continue;
				//fprintf(stderr,"Collision detected\n");
				return grid[i][j];
			}*/
			if(grid[i*j] != NULL)
			{
				if( (grid[i*j]->used == false) || (&(*(grid[i*j])) == self) )
					continue;
				//fprintf(stderr,"Collision detected\n");
				//fprintf(stderr,"Collision at grid location %i,%i\n",i,j);
				return grid[i*j];
			}
		}
	}

	return NULL;
}

