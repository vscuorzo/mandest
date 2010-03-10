#include "audio.h"

Uint8 sIndex = 0;

// Load sounds
void loadSoundFX()
{
	Uint8 i = 0;
	Uint8 j;

	Mix_Chunk *sound;

	// copy filepaths for sound files
	strcpy(&units[BATTLELORD].sAttF[0][0], "fx/blord/a1.wav");
	strcpy(&units[BATTLELORD].sAttF[1][0], "fx/blord/a2.wav");
	strcpy(&units[BATTLELORD].sDieF[0][0], "fx/blord/d1.wav");
	strcpy(&units[BATTLELORD].sSelF[0][0], "fx/blord/s1.wav");
	strcpy(&units[BATTLELORD].sSelF[1][0], "fx/blord/s2.wav");
	strcpy(&units[BATTLELORD].sSelF[2][0], "fx/blord/s3.wav");
	strcpy(&units[BATTLELORD].sMoveF[0][0], "fx/blord/m1.wav");
	strcpy(&units[BATTLELORD].sMoveF[1][0], "fx/blord/m2.wav");

	strcpy(&units[SPIDERDEMON].sAttF[0][0], "fx/spider/a1.wav");
	strcpy(&units[SPIDERDEMON].sAttF[1][0], "fx/spider/a2.wav");
	strcpy(&units[SPIDERDEMON].sAttF[2][0], "fx/spider/a3.wav");
	strcpy(&units[SPIDERDEMON].sSelF[0][0], "fx/spider/s1.wav");
	strcpy(&units[SPIDERDEMON].sSelF[1][0], "fx/spider/s2.wav");
	strcpy(&units[SPIDERDEMON].sSelF[2][0], "fx/spider/s3.wav");
	strcpy(&units[SPIDERDEMON].sSelF[3][0], "fx/spider/s4.wav");
	strcpy(&units[SPIDERDEMON].sSelF[4][0], "fx/spider/s5.wav");
	strcpy(&units[SPIDERDEMON].sMoveF[0][0], "fx/spider/m1.wav");
	strcpy(&units[SPIDERDEMON].sMoveF[1][0], "fx/spider/m2.wav");

	fprintf(stderr,"Loading sound effects...\n");

	// Open the files
	while(i<MAX_UNIT_TYPES)
	{
		j = 0;
		while( !m_strncmp(&units[i].sAttF[j][0],"\0",1) )
		{
			// Attack
			sound = Mix_LoadWAV(&units[i].sAttF[j][0]);
			if(sound == NULL)
				fprintf(stderr," - \"%s\" failed to load.\n",&units[i].sAttF[j][0]);
			else
			{
				units[i].sAtt[j] = sound;
				Mix_VolumeChunk(sound,audio.eVol*MIX_MAX_VOLUME);
				fprintf(stderr," - \"%s\" loaded.\n",&units[i].sAttF[j][0]);
			}

			// Move
			sound = Mix_LoadWAV(&units[i].sMoveF[j][0]);
			if(sound == NULL)
				fprintf(stderr," - \"%s\" failed to load.\n",&units[i].sMoveF[j][0]);
			else
			{
				units[i].sMove[j] = sound;
				Mix_VolumeChunk(sound,audio.eVol*MIX_MAX_VOLUME);
				fprintf(stderr," - \"%s\" loaded.\n",&units[i].sMoveF[j][0]);
			}

			// Die
			sound = Mix_LoadWAV(&units[i].sDieF[j][0]);
			if(sound == NULL)
				fprintf(stderr," - \"%s\" failed to load.\n",&units[i].sDieF[j][0]);
			else
			{
				units[i].sDie[j] = sound;
				Mix_VolumeChunk(sound,audio.eVol*MIX_MAX_VOLUME);
				fprintf(stderr," - \"%s\" loaded.\n",&units[i].sDieF[j][0]);
			}

			// Idle

			// Select
			sound = Mix_LoadWAV(&units[i].sSelF[j][0]);
			if(sound == NULL)
				fprintf(stderr," - \"%s\" failed to load.\n",&units[i].sSelF[j][0]);
			else
			{
				units[i].sSel[j] = sound;
				Mix_VolumeChunk(sound,audio.eVol*MIX_MAX_VOLUME);
				fprintf(stderr," - \"%s\" loaded.\n",&units[i].sSelF[j][0]);
			}

			++j;
		}

		++i;
	}
	fprintf(stderr,"...done\n");
}

void initAudio()
{
	if( Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096) == -1 )
		fprintf(stderr,"Music failed to load.\n");

	loadSoundFX();

	Mix_AllocateChannels(MAX_AUDIO_CHANNELS);
	audio.eVol = 1;
	audio.mVol = .7;
	audio.evSlider.x = ORIGIN_X + 65;
	audio.evSlider.y = ORIGIN_Y - 180;
	audio.evSlider.w = 50;
	audio.evSlider.h = 24;
	audio.mvSlider.x = ORIGIN_X + 65;
	audio.mvSlider.y = ORIGIN_Y - 95;
	audio.mvSlider.w = 50;
	audio.mvSlider.h = 24;
	audio.mute = false;
	audio.paused = false;
}

// Updates all audio settings
void updateAudio()
{
	Uint8 i = 0;
	Uint8 mVol = Mix_VolumeMusic(-1);

	if( !audio.mute && (mVol < (audio.mVol*MIX_MAX_VOLUME)) )
		Mix_VolumeMusic(++mVol);
	else if(mVol > 0)
		Mix_VolumeMusic(--mVol);

	while(i<MAX_AUDIO_CHANNELS)
	{
		if(Mix_GetChunk(i) != NULL)
			Mix_VolumeChunk(Mix_GetChunk(i),audio.eVol*MIX_MAX_VOLUME);
		++i;
	}
}

// Play a sound
void playSound(ent_t *self, Uint8 type)
{
	sIndex = 1-sIndex;
	switch(type)
	{
	case SNDFX_SEL:
		Mix_PlayChannel(-1,self->sSel[sIndex],0);
		break;
	case SNDFX_MOVE:
		Mix_PlayChannel(-1,self->sMove[sIndex],0);
		break;
	case SNDFX_DIE:
		Mix_PlayChannel(-1,self->sDie[sIndex],0);
		break;
	case SNDFX_ATT:
		Mix_PlayChannel(-1,self->sAtt[sIndex],0);
		break;
	case SNDFX_IDLE:
		Mix_PlayChannel(-1,self->sIdle[sIndex],0);
		break;
	}
}

// Stop
void stop(int num)
{
	if(Mix_PlayingMusic())
	{
		Mix_HookMusicFinished(NULL);
		Mix_FadeOutMusic(10);
		music == NULL;
		strcpy(cSong,"Stopped");
	}
}

// Attempts to load a song, returns 1 on success and 0 on failure
int loadSong(char *song)
{
	//fprintf(stderr,"%s\n",song);
	if(song == NULL)
		return 0;
		
	music = Mix_LoadMUS(song);
	if(music == NULL)
	{
		fprintf(stderr,"Audio file failed to load.\n");
		return 0;
	}
	else
		Mix_FadeInMusic(music,1,300);

	return 1;
}

// Previous
void prev(int num)
{
	stop(0);
	if(loadSong(pSong))
		strcpy(cSong,pSong);
}

// Play next song
void nextSong()
{
	char song[40];
	Uint8 r;

	// if a song is playing now, save it to pSong so we can go back
	if(music != NULL)
	{
		strcpy(pSong,cSong);
	}

	// if the playlist did not load, pick a random default song
	if(plist == NULL)
	{
		r = random(3);
		switch(r)
		{
		case 1:
			loadSong("music/song1.ogg");
			strcpy(cSong,"music/song1.ogg");
			break;
		case 2:
			loadSong("music/song2.ogg");
			strcpy(cSong,"music/song2.ogg");
			break;
		case 3:
			loadSong("music/song3.ogg");
			strcpy(cSong,"music/song3.ogg");
			break;
		}
	}

	// otherwise, attempt to load each item in the playlist until EOF
	else
	{
		while(fgets(song,40,plist))
		{
			if( loadSong(song) )
			{
				strcpy(cSong,song);
				break;
			}
		}
	}

	// When music stops, call nextSong() again
	if(music != NULL)
		Mix_HookMusicFinished(nextSong);
}

// Calls nextSong via a button
void next(int num)
{
	nextSong();
}

// Play/Pause
void play(int num)
{
	if(!Mix_PlayingMusic())
		nextSong();
	else
	{
		if(Mix_PausedMusic())
		{
			audio.paused = false;
			Mix_ResumeMusic();
		}
		else
		{
			audio.paused = true;
			Mix_PauseMusic();
		}
	}
}

// Starts the game music
void startMusic()
{
	// attempt to load playlist
	plist = fopen("config/music.txt","r");
	if(plist == NULL)
	{
		fprintf(stderr,"Music.txt failed to load, using default music.\n");
	}

	nextSong();
}
