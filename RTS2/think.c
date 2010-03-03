#include "game.h"

// Move to a new location
void move(ent_t *self, Uint32 newx, Uint32 newy)
{
	ent_t *temp;
	Uint32 nx,ny;	// position on the map to try moving to
	Uint32 gx,gy;	// position in the grid
	Uint32 ox,oy;	// old position in the grid
	Uint8 attempts;

	nx = self->locx;
	ny = self->locy;

	if( nx < newx )
		nx += 3;
	else
		nx -= 3;

	if( ny < newy )
		ny += 3;
	else
		ny -= 3;

	temp = collision(self, nx, ny, self->sprite->w, self->sprite->h);
	attempts = 0;
	if( temp != NULL ) 
	{ /*return;*/
		while( (temp != NULL) && (attempts < 10) )
		{
			if( ((temp->locx < nx) && (self->locx < temp->locx)) || ((temp->locx > nx) && (self->locx > temp->locx)) )
			{
				//if( crandom() < 0.5 )
					ny -= 3;
				//else
				//	ny += 3;
			}

			if( ((temp->locy < ny) && (self->locy < temp->locy)) || ((temp->locy > ny) && (self->locy < temp->locy)) )
			{
				//if( crandom() < 0.5 )
					nx -= 3;
				//else
				//	nx += 3;
			}

			temp = collision(self, nx, ny, self->sprite->w, self->sprite->h);
			++attempts;
		}

		self->rallyx = nx;
		self->rallyy = ny;
		self->onPath = true;
	}

	else
	{
		// remove unit from old grid location
		ox = self->locx*.1;
		oy = self->locy*.1;
		//grid[ox][oy] = NULL;
		grid[ox*oy] = NULL;

		// move
		self->locx = nx;
		self->locy = ny;

		// update grid to show new location
		gx = nx * .1;
		gy = ny * .1;
		//grid[px][py] = self;
		grid[gx*gy] = self;
	}
}


void move2(ent_t *self, Uint32 newx, Uint32 newy)
{
	Uint8 attempts;
	Uint16 nx,ny;
	ent_t *temp;

	nx = self->locx;
	ny = self->locy;

	if(self->locx > newx)
	{
		nx -= self->speed; //self->locx -= self->speed;
	}
	else
	{
		nx += self->speed; //self->locx += self->speed;
	}

	if(self->locy > newy)
		ny -= self->speed;//self->locy -= self->speed;
	else
		ny += self->speed;//self->locy += self->speed;

	temp = collision(self, nx, ny, self->sprite->w,self->sprite->h);
		
	if(temp != NULL)
	{
		fprintf(stderr,"Collision detected with object at (%i,%i)\n",temp->locx,temp->locy);
		attempts = 0;
		while( (temp != NULL) && (attempts < 50) )
		{
			// if X of other object is to the right of origin, move left
			if( (temp->locx) > nx)
				nx -= 5; //nx--;
			else
				nx += 5; //nx++;

			// if Y of other object is below origin, move up
			if( (temp->locy) > ny)
				ny -= 5; //ny--;
			else
				ny += 5; //ny++;

			// see if the new location is valid
			temp = collision(self, nx, ny, self->sprite->w,self->sprite->h);
			++attempts;
		}

		if(attempts == 50)
		{
			self->locx = newx;
			self->locy = newy;
		}
		else
		{
			self->rallyx = nx;
			self->rallyy = ny;
			if(temp == NULL)
				fprintf(stderr,"New rally point (%i,%i) set\n",self->rallyx,self->rallyy);
			fprintf(stderr,"Attempts: %i\n",attempts);
			self->onPath = true;
		}
	}

	else
	{
		self->locx = nx;
		self->locy = ny;
	}
	//self->onPath = true;
}

// attack the target, move closer if necessary
void engage(ent_t *self)
{
	Uint16 hx, hy;			// location of enemy unit
	gBool inRangex;			// is the unit close enough to fire
	gBool inRangey;
//	ent_t *temp;			// temp ent for collision detection

	hx = self->target->locx;
	hy = self->target->locy;

	// Add check for ROE choice, use gloc instead of loc for guard mode and limit pursue distance to 50

	// move closer if out of range
	if(abs(hx - self->locx) > (self->fireRange))
	{
		if(self->locx > hx)
		{
			self->rallyx = hx + self->fireRange - 7; //self->locx -= self->speed;
		}
		else
			self->rallyx = hx - self->fireRange + 7;//self->locx += self->speed;
		inRangex = false;
		//self->onPath = true;
	}
	else
		inRangex = true;
	if(abs(hy - self->locy) > (self->fireRange))
	{
		if(self->locy > hy)
			self->rallyy = hy + self->fireRange - 7; //self->locy -= self->speed;
		else
			self->rallyy = hy - self->fireRange + 7; //self->locy += self->speed;
		inRangey = false;
		//self->onPath = true;
	}
	else
		inRangey = true;
/*
	temp = collision(self->rallyx,self->rallyy,self->sprite->w,self->sprite->h);
	if(temp != NULL)
		avoid(self,temp);
*/

	// fire if close enough
	if( (inRangex == true) && (inRangey == true) )
	{
		self->onPath = false;
		if(self->fire != NULL)
		{
			self->fire(self);
			//fprintf(stderr,"Player %i's unit is firing at player %i's\n",self->owner,self->target->owner);
		}
	}
	else
	{
		self->onPath = true;
	}
}

// scan for hostile units in sight and target them
void scanForTarget(ent_t *self)
{
	Uint8 i, j;
	
	i = 0;
	while(players[i].used == 1)
	{
		if(i != self->owner)
		{
			for(j = 0;j<MAX_ENTS;++j)
			{
				// Don't auto-target things that don't exist or are dead
				if(players[i].ents[j].used == false) continue;
				if(players[i].ents[j].isDead == true) continue;

				if( (abs((players[i].ents[j].locx - self->locx) <= self->scanRange)) && (abs(players[i].ents[j].locy - self->locy) <= self->scanRange))
				{
					self->target = &(players[i].ents[j]);
					return;
				}
			}
		}
		++i;
	}
	self->target = NULL;
	return;
}

// Battle Lord
void battleLordThink(ent_t *self)
{
	//ent_t *temp;

	// navigation
	if(self->onPath == false)
	{
		if( (abs((self->locx - self->destx)) > 5) || (abs((self->locy - self->desty)) > 5) )
		{
			move(self, self->destx, self->desty);
			//fprintf(stderr, "moving to destination\n");
		}
		
		// scan for targets and deal with them
		if(self->ROE != 0)
		{
			if( (self->target != NULL) && (self->target->isDead == false) )
			{
				engage(self);
				// fprintf(stderr,"My target's health is %i\n",self->target->health);}fprintf(stderr, "firing\n");
			}
			else
			{
				self->target = NULL;
				scanForTarget(self);
			}
		}		
	}
	else
	{
		if( (abs((self->locx - self->rallyx)) > 5) || (abs((self->locy - self->rallyy)) > 5) )
		{
			move(self, self->rallyx, self->rallyy);
			//fprintf(stderr, "moving to rally point (%i,%i)\n",self->rallyx,self->rallyy);
		}
		else
			self->onPath = false;
	

//		temp = collision(self->rallyx,self->rallyy,self->sprite->w,self->sprite->h);
//			if(temp != NULL)
//				;		
	}
}

