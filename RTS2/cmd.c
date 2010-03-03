//#include "game.h"
#include "cmd.h"

// Find next available resolution
void nextRes()
{
	//SDL_Rect **reslist;
	Uint8 i = 0;

	//reslist = SDL_ListModes(NULL,SDL_FULLSCREEN | SDL_HWSURFACE);

	while( (reslist[i++] != NULL) && (i < 20) )
	{
		if( (reslist[i]->w != HUD.res.w) && (reslist[i]->h != HUD.res.h) )
		{
			if(changeRes(reslist[(i+1)%20]->w, reslist[(i+1)%20]->h, 32, SDL_FULLSCREEN | SDL_HWSURFACE) )
			{
				HUD.res.w = reslist[(i+1)%20]->w;
				HUD.res.h = reslist[(i+1)%20]->h;
				return;
			}
		}
	}
}

// Determine if a menu button is clicked and do something about it
void menuCommand()
{
	int mx, my, i;
	FILE *file;

	if( SDL_GetMouseState(&mx, &my) &SDL_BUTTON_LMASK )
	{
		for(i=0;i<MAX_MENU_BUTTONS;++i)
		{
			HUD.mButts[i].x += MENU_X;
			HUD.mButts[i].y += MENU_Y;
		}

		fprintf(stderr,"menuCommand() mx: %i my: %i\n", mx, my);
		// Button 1
		if( bCollide(&HUD.mButts[0],mx,my) )
		{
			HUD.mButts[0].action(0);
		}	

		// Button 2
		else if( bCollide(&HUD.mButts[1],mx,my) )
		{
			HUD.mButts[1].action(0);
		}

		// Button 3
		else if( bCollide(&HUD.mButts[2],mx,my) )
		{
			HUD.mButts[2].action(0);
		}

		// Button 4
		else if( bCollide(&HUD.mButts[3],mx,my) )
		{
			HUD.mButts[3].action(0);
		}

		// Button 5
		else if( bCollide(&HUD.mButts[4],mx,my) )
		{
			HUD.mButts[4].action(0);
		}
	
		for(i=0;i<MAX_MENU_BUTTONS;++i)
		{
			HUD.mButts[i].x -= MENU_X;
			HUD.mButts[i].y -= MENU_Y;
		}
	}
}

void menuCommand2()
{
	int mx, my;
	FILE *file;

	if( SDL_GetMouseState(&mx, &my) &SDL_BUTTON_LMASK )
	{
		if( (mx > MENU_X + 130) && (mx < (MENU_X + 510)) )
		{
			// Main Menu
			if( HUD.menu == 1 )
			{
				// New game
				if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
				{
					//endGame();
					if(!Mix_PlayingMusic())
						startMusic();
					else if(!players[0].inGame)
					{
						Mix_HookMusicFinished(nextSong);
						Mix_FadeOutMusic(100);
					}
					startGame(MAX_PLAYERS);
				}	

				// Resume
				else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
				{
					if(players[0].inGame)
						HUD.menu = 0;
				}

				// Save/Load
				else if( (my > (MENU_Y + 205)) && (my < (MENU_Y + 275)) )
				{
					HUD.menu = 2;
				}

				// Options
				else if( (my > (MENU_Y + 285)) && (my < (MENU_Y + 355)) )
				{
					HUD.menu = 3;
				}

				// Quit
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					exit(0);
				}
			}

			// Save/Load submenu
			else if( HUD.menu == 2 )
			{
				// Save
				if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
				{
					fprintf(stderr,"Saving not yet implemented.\n");
				}	

				// Load
				else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
				{
					fprintf(stderr,"Loading not yet implemented.\n");
				}

				// Main Menu
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					HUD.menu = 1;
				}
			}

			// Options submenu
			else if( HUD.menu == 3 )
			{
				// Sound
				if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
				{
					HUD.menu = 4;
				}	

				// Controls
				else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
				{
					HUD.menu = 5;
				}

				// HUD
				else if( (my > (MENU_Y + 205)) && (my < (MENU_Y + 275)) )
				{
					HUD.menu = 6;
				}

				// Game
				else if( (my > (MENU_Y + 285)) && (my < (MENU_Y + 355)) )
				{
					HUD.menu = 7;
				}

				// Main Menu
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					HUD.menu = 1;
				}
			}

			// Sound submenu
			else if( HUD.menu == 4 )
			{
				// Volume sliders
				if( (mx > MENU_X + 350) && (mx < (MENU_X + 490)) )
				{
					// Sound effects volume
					if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
					{
						audio.slide = 1;
					}	

					// Music volume
					else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
					{
						audio.slide = 2;
					}
				}

				// Mute
				else if( (my > (MENU_Y + 205)) && (my < (MENU_Y + 275)) )
				{
					audio.mute = 1-audio.mute;
				}

				// Options
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					HUD.menu = 3;
					// ADD save settings
				}
			}

			// ADD Controls submenu
			else if( HUD.menu == 5 )
			{
				// 
				if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
				{
				}	

				// 
				else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
				{
				}

				// 
				else if( (my > (MENU_Y + 205)) && (my < (MENU_Y + 275)) )
				{
				}

				// 
				else if( (my > (MENU_Y + 285)) && (my < (MENU_Y + 355)) )
				{
				}

				// Options
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					HUD.menu = 3;
				}
			}

			// HUD submenu
			else if( HUD.menu == 6 )
			{
				// Alpha slider
				if( (mx > MENU_X + 350) && (mx < (MENU_X + 490)) )
				{
					if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
					{
						aSlide = true;
					}
				}

				// Save
		/*		else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
				{
					saveHUD();
				}

				// Load
				else if( (my > (MENU_Y + 205)) && (my < (MENU_Y + 275)) )
				{
					file = fopen("config/hud.txt","r");
					if(file != NULL)
						loadHUD(file);
					else
						fprintf(stderr,"hud.txt failed to load\n");
				}
*/
				// Options
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					HUD.menu = 3;
				}
			}

			// Game submenu
			else if( HUD.menu == 7 )
			{
				// Scroll sensitivity slider
				if( (mx > MENU_X + 350) && (mx < (MENU_X + 490)) )
				{
					if( (my > (MENU_Y + 40)) && (my < (MENU_Y + 105)) )
					{
						HUD.sSlide = true;
					}
				}

				// Tool tips on/off
				else if( (my > (MENU_Y + 122)) && (my < (MENU_Y + 190)) )
				{
					HUD.ttips = 1 - HUD.ttips;
				}

				// Game res
				else if( (my > (MENU_Y + 205)) && (my < (MENU_Y + 275)) )
				{
					if( (mx > (MENU_X + 350)) && (mx < (MENU_X + 490)) )
					{
						nextRes();
					}
				}

				// Options
				else if( (my > (MENU_Y + 365)) && (my < (MENU_Y + 435)) )
				{
					HUD.menu = 3;
				}
			}
		}
	}
}

void checkCommands(Uint8 *keys, int *mx, int *my)
{
	int i = 0;
	SDL_Rect dest;

	if(HUD.menu == 0)
	{
		// Pan around map
		if( ((*mx > (screen->clip_rect.x+screen->w-15)) || keys[SDLK_RIGHT]) && (Camera.x + 25*sSpeed + Camera.w) < (map->clip_rect.x+map->w) )
		{
			Camera.x += 25*sSpeed;
		}
		else if( ((*mx < (screen->clip_rect.x+15)) || keys[SDLK_LEFT]) && (Camera.x - 25*sSpeed > map->clip_rect.x) )
		{
			Camera.x -= 25*sSpeed;
		}

		if( ((*my > (screen->clip_rect.y+screen->h-15)) || keys[SDLK_DOWN] ) && (Camera.y + 25*sSpeed + Camera.h) < (map->clip_rect.y+map->h) )
		{
			Camera.y += 25*sSpeed;
		}
		else if( ((*my < (screen->clip_rect.y+15)) || keys[SDLK_UP] ) && ((Camera.y - 25*sSpeed) > map->clip_rect.y) )
		{
			Camera.y -= 25*sSpeed;
		}

		// Save the HUD settings
		if(keys[SDLK_h])
		{
			saveSet();
			fprintf(stderr,"HUD saved to hud.txt\n");
		}

		if(HUD.grabPanel == 0)
		{
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK)
			{
				selectEnt(*mx, *my);
			}
			
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_RMASK)
			{
				if(players[0].sEnt != NULL)
					orderEnt(*mx, *my);
			}
		}

		// HUD
		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK)
		{
			// ADD Minimap

			// Infobar
			i = -1;
			while(++i<MAX_UNIT_TYPES)
			{
				if( !HUD.aUnits[i].draw )
					continue;

				//fprintf(stderr,"%i\n",bCollide(&HUD.aUnits[i],*mx,*my));
				if(bCollide(&HUD.aUnits[i],*mx,*my))
				{
					HUD.aUnits[i].action(i);
					fprintf(stderr,"unit button pressed\n");
					break;
				}
			}

			i = -1;
			while(++i<MAX_STRUCT_TYPES) 
			{
				if( !HUD.aStructs[i].draw )
					continue;

				if(bCollide(&HUD.aStructs[i],*mx,*my))
				{
					HUD.aStructs[i].action(i);
					fprintf(stderr,"Structure %i button pressed\n",i);
					break;
				}
			}
			
			// Music Player
			i = -1;
			while(++i < 4) 
			{
				if( !HUD.pButtons[i].draw )
					continue;

				if(bCollide(&HUD.pButtons[i],*mx,*my))
				{
					HUD.pButtons[i].action(i);
					fprintf(stderr,"player button pressed\n");
					break;
				}
			}
		}
	}

	// menu
	if(keys[SDLK_ESCAPE])
	{
		HUD.menu = 1;
	}
	if(keys[SDLK_SPACE])
	{
		if(HUD.pButtons[0].tip.info == NULL)
			return;

		dest.x = HUD.pButtons[0].x + 5;
		dest.y = HUD.pButtons[0].y - HUD.pButtons[0].tip.bbox->h;
		dest.w = HUD.pButtons[0].tip.bbox->w;
		dest.h = HUD.pButtons[0].tip.bbox->h;
		SDL_BlitSurface(HUD.pButtons[0].tip.bbox, NULL, screen, &dest);
	}
}
