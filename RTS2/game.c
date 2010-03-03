//#include <stdlib.h>
//#include "SDL.h"
//#include "SDL_image.h"
//#include "graphics.h"
// vrs5
//#include "hud.h"//"game.h"
#include "cmd.h"

//extern SDL_Surface *screen;
//extern SDL_Surface *buffer; /*pointer to the draw buffer*/
//extern void initInterface();

//extern SDL_Surface *videobuffer;

void Init_All();
void playIntro();

/*this program must be run from the directory directly below images and src, not from within src*/
/*notice the default arguments for main.  SDL expects main to look like that, so don't change it*/
int main(int argc, char *argv[])
{
	//SDL_Surface *map;
	SDL_Rect tileLoc;
	SDL_Surface *temp;
	SDL_Surface *bg;
	int done;
	int keyn;
	int mx,my;
	int i,j;
	char cmd[15];
	Uint8 *keys;
	gBool bPressed = false;
	gBool intro = true;

	Init_All();

	// handle command line arguments
	if(argc>1)
	{
		fprintf(stderr,"Reading command line arguments...\n");
		i = 0;
		while(++i<argc)
		{
			strcpy(cmd,argv[i]);
			fprintf(stderr,"- %s\n",cmd);
			if( strstr(cmd,"quickstart") != NULL )
			{
				intro = false;
				if(!Mix_PlayingMusic())
					startMusic();
				else if(!players[0].inGame)
				{
					Mix_HookMusicFinished(nextSong);
					Mix_FadeOutMusic(100);
				}
				startGame(MAX_PLAYERS);
			}
			else if( strstr(cmd,"mute") != NULL )
			{
				// ADD mute sound effects
				audio.mute = true;
				Mix_VolumeMusic(0);
			}
			else if( strstr(cmd,"nomusic") != NULL )
			{
				audio.mVol = 0;
				Mix_VolumeMusic(0);
			}
			else if( strstr(cmd,"nointro") != NULL )
			{
				intro = false;
			}
			else if( strstr(cmd,"windowed") != NULL )
			{
				if(!changeRes(HUD.res.w, HUD.res.h, 32, SDL_ANYFORMAT | SDL_HWSURFACE))
				{
					j = 0;
					while(reslist[j] != NULL)
					{
						if(changeRes(reslist[j]->w,reslist[j]->h, 32, SDL_ANYFORMAT | SDL_HWSURFACE))
							break;
						++j;
					}
				}
			}
			else if(strstr(cmd,"debug"))
			{
				audio.mVol = 0;
				Mix_VolumeMusic(0);
				intro = false;
				changeRes(1024, 768, 32, SDL_ANYFORMAT|SDL_HWSURFACE);
			}
			else if(strstr(cmd,"map") != NULL)
			{
				// ADD char *mapName; mapName = argv[++i] + extension;
			}
			else
				fprintf(stderr,"  Argument \"%s\" not recognized.\n",cmd);
		}

		fprintf(stderr,"...done.\n");
	}

	temp = IMG_Load("images/grass.jpg");
	if(temp != NULL)						/*ALWAYS check your pointers before you use them*/
		bg = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	if(bg != NULL)
	{
		map = SDL_CreateRGBSurface(SDL_HWSURFACE, MAPX, MAPY, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
		map->clip_rect.x = 0;
		map->clip_rect.y = 0;
		map->clip_rect.w = MAPX;
		map->clip_rect.h = MAPY;
		SDL_FreeSurface(buffer);
		buffer = SDL_DisplayFormat(map);

		tileLoc.x = 0;
		tileLoc.y = 0;
		tileLoc.w = bg->w;
		tileLoc.h = bg->h;
		if(map == NULL)
		{
			fprintf(stderr,"Map failed to load\n");
			return 0;
		}
		//fprintf(stderr,"w: %i h: %i\n",bg->w,bg->h);
		while(tileLoc.x < map->w)
		{
			while(tileLoc.y < map->h)
			{
				//fprintf(stderr,"x: %i y: %i\n",tileLoc.x,tileLoc.y);
				SDL_BlitSurface(bg,NULL,map,&tileLoc);
				tileLoc.y += bg->h;
			}
			tileLoc.y = 0;
			tileLoc.x += bg->w;
			//fprintf(stderr,"x: %i y: %i\n",tileLoc.x,tileLoc.y);
		}
		
		SDL_BlitSurface(map,NULL,buffer,NULL);
	}

	if(intro)
	{
		playIntro();
	}

	done = 0;
	do
	{
		SDL_BlitSurface(map,&Camera,buffer,&Camera); // Draw only the visible piece of the map to "reset" the buffer for the next frame
		drawEnts();
		ResetBuffer();
		drawInterface();
		DrawMouse();
		NextFrame();
		SDL_PumpEvents();

		keys = SDL_GetKeyState(&keyn);

		if( (players[0].sEnt != NULL) && (players[0].sEnt->used == false) )
			players[0].sEnt = NULL;

		if(HUD.menu == 0)
		{
			SDL_GetMouseState(&mx,&my);

			// MOVE to cmd

			movePanel(mx,my);
			thinkAll();
			updateAll();
		}
		else
		{
			menuMouseOver(&mx,&my);
			if(bPressed == false)
			{
				menuCommand();
				bPressed = true;
			}
			if( SDL_GetMouseState(NULL,NULL) == 0 )
				bPressed = false;
		}

		if(!bPressed)
		{
			checkCommands(keys,&mx,&my);
			bPressed = true;
		}

		if( SDL_GetMouseState(NULL,NULL) == 0 )
				bPressed = false;

		updateAudio();
	}while(!done);

	exit(0);		/*technically this will end the program, but the compiler likes all functions that can return a value TO return a value*/
	return 0;
}

void CleanUpAll()
{
	Mix_CloseAudio();
	TTF_Quit();
	CloseSprites();
  /*any other cleanup functions can be added here*/ 
}

void Init_All()
{
	Init_Graphics();
	TTF_Init();
	InitMouse();
	atexit(CleanUpAll);

	initGame();
	initAudio();
	initInterface();
	loadSong("music/intro.mp3");
}

