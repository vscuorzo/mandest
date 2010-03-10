#include "game.h"

void building(ent_t *self)
{
	if(NOW >= self->nextUpdate)
	{
		self->queueTime--;
		self->nextUpdate = NOW + 10;
	}

	if(self->queueTime <= 0)
	{
		self->building = false;
		self->isDead = false;
		self->nextUpdate = NOW + self->upFreq;
	}
}

void battleLordUpdate(ent_t *self)
{
	if(self->isDead == false)
	{
		if(NOW >= self->nextUpdate)
		{
			//self->frame = (++self->frame)%4;
			self->nextUpdate = NOW + self->upFreq;
		}
	}
}

void factoryUpdate(ent_t *self)
{
	if(NOW >= self->nextUpdate)
	{
		if(players[0].pFact == NULL)
			players[0].pFact = self;
		dequeueUnit();
		//queueUnit(self, units[BATTLELORD]);
		self->nextUpdate = NOW + self->upFreq;
	}
}

void factory2Update(ent_t *self)
{
	if(NOW >= self->nextUpdate)
	{
		if(players[0].pBarr == NULL)
			players[0].pBarr = self;
		//queueUnit(self, units[SPIDERDEMON]);
		dequeueUnit();
		self->nextUpdate = NOW + self->upFreq;
	}
}
