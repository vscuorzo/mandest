#include "game.h"

#define ORIGIN_X screen->w * .5
#define ORIGIN_Y screen->h * .5

#define MAX_AUDIO_CHANNELS 20

// Background music
Mix_Music *music;

char cSong[40];
char pSong[40];

// Playlist file
FILE *plist;

// Audio properties info
typedef struct audio_properties
{
	SDL_Rect evSlider, mvSlider;	// Volume sliders in sound menu
	Uint8 slide;					// Slider to move, 0 if none
	float eVol;						// Effects volume
	float mVol;						// Music volume
	gBool mute;						// Mute all sound
	gBool paused;					// Music paused
}aProps;

aProps audio;
//extern aProps audio;
void updateAudio();
void stop(int num);
int loadSong(char *song);
void prev(int num);
void nextSong();
void next(int num);
void play(int num);
void startMusic();
