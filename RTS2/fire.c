#include "game.h"

// does damage to a single target, armor and health can be affected differently
void damage(ent_t *self, int damageH, int damageA)
{
	if(self->target == NULL) return;
	//fprintf(stderr,"Player %i's unit says BANG\n", self->owner);

	// check for armor before taking from health
	if(self->target->armor <= 0)
	{
		self->target->health -= damageH;
		//fprintf(stderr,"enemy health is now %i\n",self->target->health);
	}
	else
	{
		self->target->armor -= damageA;
		//fprintf(stderr,"enemy armor is now %i\n",self->target->armor);
	}

	// if the unit died, give xp
	if( (self->target->health <= 0))
	{
		self->target->isDead = true;
		self->xp += 5;
		playSound(self->target,SNDFX_DIE);
		freeEnt(self->target);
		self->target = NULL;
		fprintf(stderr,"Player %i's unit now has %i xp\n",self->owner,self->xp);
	}
}

void minigun(ent_t *self)
{
	if(self->nextFire <= NOW)
	{
		damage(self, 2, 1);
	}

	self->nextFire = NOW + self->ROF;
}

void minigun2(ent_t *self)
{
	if(self->nextFire <= NOW)
	{
		damage(self, 1, 1);
	}

	self->nextFire = NOW + self->ROF;
}

