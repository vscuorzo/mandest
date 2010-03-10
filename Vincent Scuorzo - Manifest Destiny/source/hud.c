#include "hud.h"

// hud info
hInfo HUD;

// menu stuff
SDL_Color color = {255,255,255};	// Default text color
SDL_Color oColor = {255,0,0};		// Color on mouse over
SDL_Color bColor = {50,50,50};		// Color for items that are not an option
Uint8 mouseOver = 0;				// Which button the mouse is over

// Check if the cursor is on a button, return 1 if true and 0 otherwise
int bCollide(bInfo *b, int mx, int my)
{
	//fprintf(stderr,"bx: %i by: %i bw: %i bh: %i \nmx: %i my: %i\n",b->x,b->y,b->img->w,b->img->h,mx,my);
	if( (mx > b->x) && (mx < (b->x+b->img->w)) )
	{
		if( (my > b->y) && (my < (b->y+b->img->h)) )
			return 1;
	}
	return 0;
}

// bCollide if button image is not associated with the button itself
int bCollide2(bInfo *b, int mx, int my)
{
	//fprintf(stderr,"bx: %i by: %i bw: %i bh: %i \nmx: %i my: %i\n",b->x,b->y,b->img->w,b->img->h,mx,my);
	if( (mx > b->x) && (mx < (b->x+b->w)) )
	{
		if( (my > b->y) && (my < (b->y+b->h)) )
			return 1;
	}
	return 0;
}

// plays the intro sequence
void playIntro()
{
	SDL_Surface *text;
	int intro = 0;
	int iAlpha = 0;
	int iPhase = 0;
	Sprite *spr;
	gBool fade = true;
	TTF_Font *font;
	SDL_Rect dest;
	int fadeSpeed = 1;
	Uint8 *keys;

	spr = LoadSprite("images/kimmy.png",506,565);
	font = TTF_OpenFont("fonts/font1.ttf",60);

	while(++intro<1000)
	{
		SDL_PumpEvents();
		keys = SDL_GetKeyState(NULL);
		if(keys[SDLK_ESCAPE])
			intro = 1000;
		
		ResetBuffer();
		if(spr != NULL)
		{
			fprintf(stderr,"intro: %i iAlpha: %i iPhase: %i\n",intro,iAlpha,iPhase);
			SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format, 0,0,0));

			switch(iPhase)
			{
			case 0:
				fadeSpeed = 1;
				text = TTF_RenderText_Solid(font,"Vincent Scuorzo Presents",color);
				dest.x = (screen->w-text->w)*.5;
				dest.y = (screen->h-text->h)*.5;
				SDL_SetAlpha( text, SDL_SRCALPHA|SDL_RLEACCEL, iAlpha );
				SDL_BlitSurface(text,NULL,screen,&dest);
				break;
			case 1:
				fadeSpeed = 1;
				SDL_SetAlpha( spr->image, SDL_SRCALPHA|SDL_RLEACCEL, iAlpha );
				DrawSprite(spr,screen,(screen->w-spr->w)*.5,(screen->h-spr->h)*.5,0);
				break;
			default:
				intro = 1000;
			}

			if(iAlpha < (SDL_ALPHA_OPAQUE-fadeSpeed) && fade == true)
				iAlpha+=fadeSpeed;
			else if(iAlpha > SDL_ALPHA_TRANSPARENT && fade == false)
				iAlpha-=fadeSpeed;
			else if(iAlpha >= (SDL_ALPHA_OPAQUE-fadeSpeed) )
			{
				fade = false;
			}
			else if(iAlpha <= SDL_ALPHA_TRANSPARENT)
			{
				fade = true;
				++iPhase;
			}
		}
		else
			intro = 1000;
		NextFrame();
	}
}

void initButtons()
{
	Uint8 i, fontSize;
	TTF_Font *font;
	SDL_Rect dest;
	char price[10];

	fontSize = 10;
	font = TTF_OpenFont("fonts/font1.ttf",fontSize);
	if(font == NULL)
	{
		fprintf(stderr,"Font failed to load.\n");
	}
	
	dest.x = 0;
	dest.y = fontSize+5;

	// set up tool tip surfaces
	for(i=0;i<MAX_UNIT_TYPES;++i)
	{
		HUD.aUnits[i].tip.bbox = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 40, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
		SDL_FillRect(HUD.aUnits[i].tip.bbox,&HUD.aUnits[i].tip.bbox->clip_rect,SDL_MapRGB(HUD.aUnits[i].tip.bbox->format, 100,100,100));
		SDL_SetAlpha(HUD.aUnits[i].tip.bbox, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
	}

	for(i=0;i<MAX_STRUCT_TYPES;++i)
	{
		HUD.aStructs[i].tip.bbox = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 40, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);	
		SDL_FillRect(HUD.aStructs[i].tip.bbox,&HUD.aStructs[i].tip.bbox->clip_rect,SDL_MapRGB(HUD.aStructs[i].tip.bbox->format, 100,100,100));
		SDL_SetAlpha(HUD.aStructs[i].tip.bbox, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
	}

	// Battle Lord
	HUD.aUnits[BATTLELORD].img = LoadSprite("icons/blord.png",36,47);
	HUD.aUnits[BATTLELORD].stats = &units[BATTLELORD];
	HUD.aUnits[BATTLELORD].draw = true;
	HUD.aUnits[BATTLELORD].x = HUD.infox+20;			
	HUD.aUnits[BATTLELORD].y = HUD.infoy+108;
	HUD.aUnits[BATTLELORD].action = enqueueUnit;
	HUD.aUnits[BATTLELORD].tip.fColor.r = 255;
	HUD.aUnits[BATTLELORD].tip.fColor.g = 255;
	HUD.aUnits[BATTLELORD].tip.fColor.b = 255;
	SDL_BlitSurface(TTF_RenderText_Blended(font, "Battle Lord", HUD.aUnits[BATTLELORD].tip.fColor), NULL, HUD.aUnits[BATTLELORD].tip.bbox, NULL);
	sprintf(price, "$%i", units[BATTLELORD].price);
	SDL_BlitSurface(TTF_RenderText_Blended(font, price, HUD.aUnits[BATTLELORD].tip.fColor), NULL, HUD.aUnits[BATTLELORD].tip.bbox, &dest);

	// Spider Demon
	HUD.aUnits[SPIDERDEMON].img = LoadSprite("icons/spider.png",48,27);
	HUD.aUnits[SPIDERDEMON].stats = &units[SPIDERDEMON];
	HUD.aUnits[SPIDERDEMON].draw = true;
	HUD.aUnits[SPIDERDEMON].x = HUD.infox+83;
	HUD.aUnits[SPIDERDEMON].y = HUD.infoy+108;
	HUD.aUnits[SPIDERDEMON].action = enqueueUnit;
	HUD.aUnits[SPIDERDEMON].tip.fColor.r = 255;
	HUD.aUnits[SPIDERDEMON].tip.fColor.g = 255;
	HUD.aUnits[SPIDERDEMON].tip.fColor.b = 255;
	SDL_BlitSurface(TTF_RenderText_Blended(font, "Spider Demon", HUD.aUnits[SPIDERDEMON].tip.fColor), NULL, HUD.aUnits[SPIDERDEMON].tip.bbox, NULL);
	sprintf(price, "$%i", units[SPIDERDEMON].price);
	SDL_BlitSurface(TTF_RenderText_Blended(font, price, HUD.aUnits[SPIDERDEMON].tip.fColor), NULL, HUD.aUnits[SPIDERDEMON].tip.bbox, &dest);

	// Factory
	HUD.aStructs[BARRACKS].img = LoadSprite("icons/blord.png",36,47);
	HUD.aStructs[BARRACKS].stats = &structs[BARRACKS];
	HUD.aStructs[BARRACKS].draw = true;
	HUD.aStructs[BARRACKS].x = HUD.infox+20;
	HUD.aStructs[BARRACKS].y = HUD.infoy+40;
	HUD.aStructs[BARRACKS].action = placeStruct;
	HUD.aStructs[BARRACKS].tip.fColor.r = 255;
	HUD.aStructs[BARRACKS].tip.fColor.g = 255;
	HUD.aStructs[BARRACKS].tip.fColor.b = 255;
	SDL_BlitSurface(TTF_RenderText_Blended(font, "Barracks", HUD.aStructs[BARRACKS].tip.fColor), NULL, HUD.aStructs[BARRACKS].tip.bbox, NULL);
	sprintf(price, "$%i", structs[BARRACKS].price);
	SDL_BlitSurface(TTF_RenderText_Blended(font, price, HUD.aStructs[BARRACKS].tip.fColor), NULL, HUD.aStructs[BARRACKS].tip.bbox, &dest);

	// Factory 2
	HUD.aStructs[FACTORY].img = LoadSprite("icons/blord.png",39,50);
	HUD.aStructs[FACTORY].stats = &structs[FACTORY];
	HUD.aStructs[FACTORY].draw = true;
	HUD.aStructs[FACTORY].x = HUD.infox+83;
	HUD.aStructs[FACTORY].y = HUD.infoy+40;
	HUD.aStructs[FACTORY].action = placeStruct;
	HUD.aStructs[FACTORY].tip.fColor.r = 255;
	HUD.aStructs[FACTORY].tip.fColor.g = 255;
	HUD.aStructs[FACTORY].tip.fColor.b = 255;
	SDL_BlitSurface(TTF_RenderText_Blended(font, "Factory", HUD.aStructs[FACTORY].tip.fColor), NULL, HUD.aStructs[FACTORY].tip.bbox, NULL);
	sprintf(price, "$%i", structs[FACTORY].price);
	SDL_BlitSurface(TTF_RenderText_Blended(font, price, HUD.aStructs[FACTORY].tip.fColor), NULL, HUD.aStructs[FACTORY].tip.bbox, &dest);

	// Media player
	// Previous
	HUD.pButtons[0].x = HUD.playerx + 12;
	HUD.pButtons[0].y = HUD.playery + 5;
	HUD.pButtons[0].w = 13;
	HUD.pButtons[0].h = 7;
	HUD.pButtons[0].draw = true;
	HUD.pButtons[0].action = prev;

	// Play/pause
	HUD.pButtons[1].x = HUD.playerx + 30;
	HUD.pButtons[1].y = HUD.playery + 3;
	HUD.pButtons[1].w = 8;
	HUD.pButtons[1].h = 10;
	HUD.pButtons[1].draw = true;
	HUD.pButtons[1].action = play;

	// Stop
	HUD.pButtons[2].x = HUD.playerx + 42;
	HUD.pButtons[2].y = HUD.playery + 4;
	HUD.pButtons[2].w = 8;
	HUD.pButtons[2].h = 7;
	HUD.pButtons[2].draw = true;
	HUD.pButtons[2].action = stop;

	// Next
	HUD.pButtons[3].x = HUD.playerx + 56;
	HUD.pButtons[3].y = HUD.playery + 5;
	HUD.pButtons[3].w = 13;
	HUD.pButtons[3].h = 7;
	HUD.pButtons[3].draw = true;
	HUD.pButtons[3].action = next;
}

// Close menu
void closeMenu()
{
	HUD.menu = 0;
}

// Highlight menu items on mouse over
void menuMouseOver(int *mx, int *my)
{
	SDL_GetMouseState(mx,my);
	if( (*mx > MENU_X + 130) && (*mx < (MENU_X + 510)) )
	{
		if( (*my > (MENU_Y + 40)) && (*my < (MENU_Y + 105)) )
		{
			mouseOver = 1;
		}
		else if( (*my > (MENU_Y + 122)) && (*my < (MENU_Y + 190)) )
		{
			mouseOver = 2;
		}
		else if( (*my > (MENU_Y + 205)) && (*my < (MENU_Y + 275)) )
		{
			mouseOver = 3;
		}
		else if( (*my > (MENU_Y + 285)) && (*my < (MENU_Y + 355)) )
		{
			mouseOver = 4;
		}
		else if( (*my > (MENU_Y + 365)) && (*my < (MENU_Y + 435)) )
		{
			mouseOver = 5;
		}
		else
		{
			mouseOver = 0;
		}
	}
	else
	{
		mouseOver = 0;
	}
}

// Save current interface settings
void saveHUD()
{
	FILE *hud;

	hud = fopen("config/hud.txt","w");
	if(hud == NULL)
	{
		fprintf(stderr,"HUD save failed.\n");
		return;
	}

	fprintf(hud,"minimap\n%i\n%i\n%i\n%i\n\n",HUD.mapx,HUD.mapy,HUD.mapw,HUD.maph);
	fprintf(hud,"infobar\n%i\n%i\n%i\n%i\n\n",HUD.infox,HUD.infoy,HUD.infow,HUD.infoh);
	fprintf(hud,"player\n%i\n%i\n%i\n%i\n\n",HUD.playerx,HUD.playery,HUD.playerw,HUD.playerh);
	fprintf(hud,"misc\n%i",(int)(255*aLevel));
	fclose(hud);
}

// Load interface settings from a file
void loadHUD(FILE *hud)
{
	char cmd[10];		// command string
	Uint8 panel = 0;	// which panel the command applies to

	while( fgets(cmd, 10, hud) != NULL )
	{
		// find out what these settings apply to, usually a specific panel
		if( m_strncmp(cmd, "minimap",10) )
			panel = 1;
		else if( m_strncmp(cmd, "infobar",10) )
			panel = 2;
		else if( m_strncmp(cmd, "player",10) )
			panel = 3;
		else if( m_strncmp(cmd, "misc",10) )
			panel = 0;
		else
			continue;

		// load the values
		if(panel == 1)
		{
			fgets(cmd, 10, hud);
			HUD.mapx = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.mapy = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.mapw = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.maph = atoi(cmd);
		}
		else if(panel == 2)
		{
			fgets(cmd, 10, hud);
			HUD.infox = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.infoy = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.infow = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.infoh = atoi(cmd);
		}
		else if(panel == 3)
		{
			fgets(cmd, 10, hud);
			HUD.playerx = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.playery = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.playerw = atoi(cmd);

			fgets(cmd, 10, hud);
			HUD.playerh = atoi(cmd);
		}
		else if(panel == 0)
		{
			fgets(cmd, 10, hud);
			aLevel = (float)atoi(cmd)/255.0;
		}
	}
	fclose(hud);
}

void loadDefHUD()
{
	// minimap
	HUD.mapx = 5;
	HUD.mapy = 500;
	HUD.mapw = 250;
	HUD.maph = 250;

	// info bar
	HUD.infox = 300;
	HUD.infoy = 500;
	HUD.infow = 400;
	HUD.infoh = 250;

	// media player
	HUD.playerx = 300;
	HUD.playery = 300;
	HUD.playerw = 200;
	HUD.playerh = 18;

	// transparency
	aLevel = 1.0;
}

// Loads initial interface data
void initInterface()
{
	FILE *hud;			// config file
	Sprite *temp;
	SDL_Surface *menu;

	// Open the config file, load defaults if it fails
	hud = fopen("config/hud.txt","r");
	if(hud == NULL)
	{
		fprintf(stderr,"HUD.txt failed to open, using defaults\n");
		loadDefHUD();
	}

	else
	{
		loadHUD(hud);
	}

	// Load menu sprite
	temp = LoadSprite("hud/menu.png",640,480);
	menu = IMG_Load("hud/menu.png");
	SDL_SetColorKey(menu, 0, 0);
	temp->image = SDL_DisplayFormatAlpha(menu);
	SDL_FreeSurface(menu);

	// set HUD transparency slider info
	aSlider.x = ORIGIN_X + 65;
	aSlider.y = ORIGIN_Y - 180;
	aSlider.w = 50;
	aSlider.h = 24;
	aSlide = false;

	// set scroll speed slider info
	sSlider.x = ORIGIN_X + 65;
	sSlider.y = ORIGIN_Y - 180;
	sSlider.w = 50;
	sSlider.h = 24;
	HUD.sSlide = false;
	
	// start invisible
	alpha = SDL_ALPHA_TRANSPARENT;
	
	// Load button info
	initButtons();

	HUD.grabPanel = 0;
	HUD.menu = 1;
	HUD.ttips = true;
}

// update media player
void updatePlayer()
{
	TTF_Font *font;
	SDL_Rect dest;

	font = TTF_OpenFont("fonts/font1.ttf",8);
	if(font == NULL)
	{
		fprintf(stderr,"Font failed to load.\n");
	}

	dest.x = HUD.playerx+76;
	dest.y = HUD.playery+2;
	SDL_BlitSurface(TTF_RenderText_Blended(font, cSong, color), NULL, screen, &dest);
}

// resets the hud for the next game
void resetHUD()
{
	int i;

	i = 0;
	while(i < MAX_UNIT_TYPES)
	{
		fprintf(stderr,"Clearing unit icons...\n");
		HUD.aUnits[i].draw = false;
		i++;
	}

	i = 0;
	while(i < MAX_STRUCT_TYPES)
	{
		fprintf(stderr,"Clearing structure icons...\n");
		HUD.aStructs[i].draw = false;
		i++;
	}

	HUD.grabPanel = 0;
	HUD.nx = 0;
	HUD.ny = 0;
}

// Show tool tip
void showTip()
{
	SDL_Rect loc;
	int i;

	SDL_GetMouseState(&loc.x,&loc.y);
	
	for(i=0;i<MAX_UNIT_TYPES;++i)
	{
		if( bCollide(&HUD.aUnits[i],loc.x,loc.y) )
		{
			loc.x += Msprite->w;
			SDL_SetAlpha(HUD.aUnits[i].tip.bbox, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
			SDL_BlitSurface(HUD.aUnits[i].tip.bbox, NULL, screen, &loc);
			return;
		}
	}

	for(i=0;i<MAX_STRUCT_TYPES;++i)
	{
		if( bCollide(&HUD.aStructs[i],loc.x,loc.y) )
		{
			
			loc.x += Msprite->w;
			SDL_SetAlpha(HUD.aStructs[i].tip.bbox, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
			SDL_BlitSurface(HUD.aStructs[i].tip.bbox, NULL, screen, &loc);
			return;
		}
	}
}

// Updates a menu slider's location and returns the position as a percentage of the max value
float moveSlider(SDL_Rect *slider, SDL_Rect *slot)
{
	int mx, my;

	SDL_GetMouseState(&mx,&my);

	//fprintf(stderr,"mx: %i my: %i slider.x: %i slot.x: %i slot.w: %i\n",mx,my,slider->x,slot->x,slot->w);
	if(mx < slot->x)
		slider->x = slot->x;
	else if(mx > (slot->x + slot->w) )
		slider->x = slot->x + slot->w;
	else
		slider->x = mx;

	//fprintf(stderr,"mx: %i my: %i slider.x: %i slot.x: %i slot.w: %i\n",mx,my,slider->x,slot->x,slot->w);
	return ((float)slider->x - (float)slot->x)/(float)slot->w;
}

// Allows the user to move the panels of the interface around
void movePanel(Sint16 mx, Sint16 my)
{
	Uint8 i,x,y = 0;

	if(SDL_GetMouseState(NULL,NULL) &SDL_BUTTON_LMASK)
	{
		switch(HUD.grabPanel)
		{
		case 1:		// move the map
			HUD.mapx = mx - HUD.nx;
			HUD.mapy = my - HUD.ny;
			break;
		case 2:		// move the infobar
			HUD.infox = mx - HUD.nx;
			HUD.infoy = my - HUD.ny;
			i=0;
			while(i<MAX_STRUCT_TYPES)
			{
				HUD.aStructs[i].x = HUD.infox + 20 + (63*i);
				HUD.aStructs[i].y = HUD.infoy + 40;
				i++;
			}
			i=0;
			while(i<MAX_UNIT_TYPES)
			{
				HUD.aUnits[i].x = HUD.infox + 20 + (63*i);
				HUD.aUnits[i].y = HUD.infoy + 108;
				i++;
			}
			break;
		case 3:		// move the media player and its buttons
			for(i=0;i<4;i++)
			{
				HUD.pButtons[i].x = mx - HUD.nx + (HUD.pButtons[i].x - HUD.playerx);
				HUD.pButtons[i].y = my - HUD.ny + (HUD.pButtons[i].y - HUD.playery);
			}
			HUD.playerx = mx - HUD.nx;
			HUD.playery = my - HUD.ny;
			break;
		default:		// find out which panel is grabbed and the distance between its origin and the cursor
			if( (mx>HUD.mapx) && (mx<(HUD.mapx + HUD.mapw - 40)) )
			{
				if( (my>HUD.mapy) && (my<HUD.mapy + 20) )
				{
					HUD.grabPanel = 1;
					HUD.nx = mx - HUD.mapx;
					HUD.ny = my - HUD.mapy;
				}
			}
			else if( (mx>HUD.infox) && (mx<HUD.infox + 380) )
			{
				if( (my>HUD.infoy) && (my<HUD.infoy + 20) )
				{
					HUD.grabPanel = 2;
					HUD.nx = mx - HUD.infox;
					HUD.ny = my - HUD.infoy;
				}
			}
			else if( (mx>HUD.playerx) && (mx<HUD.playerx + HUD.playerw) )
			{
				if( (my>HUD.playery) && (my<HUD.playery + HUD.playerh) )
				{
					HUD.grabPanel = 3;
					HUD.nx = mx - HUD.playerx;
					HUD.ny = my - HUD.playery;
				}
			}
			else
			{
				//HUD.nx = mx;
				//HUD.ny = my;
				HUD.grabPanel = 0;
			}
		}
	}
	else
		HUD.grabPanel = 0;
}

// Draws the interface to the screen
void drawInterface()
{
	Sprite *temp;
	int i = -1;
	int x, y;
	TTF_Font *font;
	SDL_Surface *button;
	SDL_Rect dest;
	SDL_Surface icon;

	// fade HUD in/out
	if(HUD.menu != 0 && aSlide == false)
	{
		if(alpha > 4)
			alpha -= 5;
		else
			alpha = SDL_ALPHA_TRANSPARENT;
	}
	else if(alpha < (255*aLevel))
		alpha += 5;
	else
	{
		alpha = 255*aLevel;
	}

	if(HUD.menu > 0)
		SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format, 0,0,0));
	else
	{
		if(players[0].placing > -1)
			placeStruct2();
	}

	// load font for text
	font = TTF_OpenFont("fonts/font1.ttf",30);
	if(font == NULL)
	{
		fprintf(stderr,"Font failed to load.\n");
	}

	// unless alpha > 0, don't bother drawing the interface
	if(alpha > 0)
	{
		// minimap
		temp = LoadSprite("hud/minimap.png",HUD.mapw,HUD.maph);
		SDL_SetAlpha( temp->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
		DrawSprite(temp,screen,HUD.mapx,HUD.mapy,0);

		// info bar
		temp = LoadSprite("hud/infobar.png",HUD.infow,HUD.infoh);
		SDL_SetAlpha( temp->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
		DrawSprite(temp,screen,HUD.infox,HUD.infoy,0);

		// Draw structure icons
		while(++i<MAX_STRUCT_TYPES)
		{
			if( HUD.aStructs[i].draw == false ) continue;

			// Draw background
			temp = LoadSprite("hud/qbutton.png",58,58);
			SDL_SetAlpha( temp->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
			DrawSprite(temp, screen, HUD.aStructs[i].x, HUD.aStructs[i].y, 0);

			// Draw structure icon
			SDL_SetColorKey(HUD.aStructs[i].img->image, SDL_SRCCOLORKEY , SDL_MapRGB(HUD.aStructs[i].img->image->format, 255,255,255));
			SDL_SetAlpha(HUD.aStructs[i].img->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
			DrawSprite(HUD.aStructs[i].img, screen, HUD.aStructs[i].x+((58-HUD.aStructs[i].img->w)*.5), HUD.aStructs[i].y+((58-HUD.aStructs[i].img->h)*.5), 0);
		}

		// Draw unit icons
		i = -1;
		while(++i<MAX_UNIT_TYPES)
		{
			if( HUD.aUnits[i].draw == false ) continue;

			// Draw background
			temp = LoadSprite("hud/qbutton.png",58,58);
			SDL_SetAlpha(temp->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
			DrawSprite(temp,screen,HUD.aUnits[i].x,HUD.aUnits[i].y,0);

			// Draw unit icon
			SDL_SetColorKey(HUD.aUnits[i].img->image, SDL_SRCCOLORKEY , SDL_MapRGB(HUD.aUnits[i].img->image->format, 255,255,255));
			SDL_SetAlpha(HUD.aUnits[i].img->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
			DrawSprite(HUD.aUnits[i].img, screen, HUD.aUnits[i].x+((58-HUD.aUnits[i].img->w)*.5), HUD.aUnits[i].y+((58-HUD.aUnits[i].img->h)*.5), 0);
		}

		// media player bar
		temp = LoadSprite("hud/mplayerbar.png",HUD.playerw,HUD.playerh);
		SDL_SetAlpha( temp->image, SDL_SRCALPHA|SDL_RLEACCEL, alpha );
		DrawSprite(temp,screen,HUD.playerx,HUD.playery,0);
		updatePlayer();

		// tool tips
		if(HUD.ttips)
			showTip();
	}

	// Draw menu
	if( (HUD.menu > 0) && (HUD.menu != 5) )
	{
		temp = LoadSprite("hud/menu.png",640,480);

		// center and draw the menu panel and buttons
		if(screen->w > 640)
			x = screen->w * .5 - 320;
		else
			x = 0;
		if(screen->h > 480)
			y = screen->h * .5 - 240;
		else
			y = 0;

		DrawSprite(temp,screen,x,y,0);
	}

	// Draw the button text
	// Main menu
	if(HUD.menu == 1)
	{
		// New Game
		dest.x = MENU_X+250;
		dest.y = MENU_Y+55;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 1)
			button = TTF_RenderText_Blended(font,"New Game",oColor);
		else
			button = TTF_RenderText_Blended(font,"New Game",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Resume
		dest.x = MENU_X+260;
		dest.y = MENU_Y+138;
		dest.w = 100;
		dest.h = 50;

		if(players[0].inGame)
		{
			if(mouseOver == 2)
				button = TTF_RenderText_Blended(font,"Resume",oColor);
			else
				button = TTF_RenderText_Blended(font,"Resume",color);
		}
		else
			button = TTF_RenderText_Blended(font,"Resume",bColor);

		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Save/Load
		dest.x = MENU_X+245;
		dest.y = MENU_Y+223;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 3)
			button = TTF_RenderText_Blended(font,"Save/Load",oColor);
		else
			button = TTF_RenderText_Blended(font,"Save/Load",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Options
		dest.x = MENU_X+260;
		dest.y = MENU_Y+302;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 4)
			button = TTF_RenderText_Blended(font,"Options",oColor);
		else
			button = TTF_RenderText_Blended(font,"Options",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Quit
		dest.x = MENU_X+285;
		dest.y = MENU_Y+385;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 5)
			button = TTF_RenderText_Blended(font,"Quit",oColor);
		else
			button = TTF_RenderText_Blended(font,"Quit",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);
	}

	// Save/Load submenu
	else if(HUD.menu == 2)
	{
		// Save
		dest.x = MENU_X+275;
		dest.y = MENU_Y+55;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 1)
			button = TTF_RenderText_Blended(font,"Save",oColor);
		else
			button = TTF_RenderText_Blended(font,"Save",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Load
		dest.x = MENU_X+275;
		dest.y = MENU_Y+138;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 2)
			button = TTF_RenderText_Blended(font,"Load",oColor);
		else
			button = TTF_RenderText_Blended(font,"Load",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Main Menu
		dest.x = MENU_X+245;
		dest.y = MENU_Y+385;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 5)
			button = TTF_RenderText_Blended(font,"Main Menu",oColor);
		else
			button = TTF_RenderText_Blended(font,"Main Menu",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);
	}

	// Options submenu
	else if(HUD.menu == 3)
	{
		// Sound
		dest.x = MENU_X+275;
		dest.y = MENU_Y+55;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 1)
			button = TTF_RenderText_Blended(font,"Sound",oColor);
		else
			button = TTF_RenderText_Blended(font,"Sound",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Controls
		dest.x = MENU_X+260;
		dest.y = MENU_Y+138;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 2)
			button = TTF_RenderText_Blended(font,"Controls",oColor);
		else
			button = TTF_RenderText_Blended(font,"Controls",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// HUD
		dest.x = MENU_X+285;
		dest.y = MENU_Y+223;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 3)
			button = TTF_RenderText_Blended(font,"HUD",oColor);
		else
			button = TTF_RenderText_Blended(font,"HUD",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Game
		dest.x = MENU_X+270;
		dest.y = MENU_Y+302;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 4)
			button = TTF_RenderText_Blended(font,"Game",oColor);
		else
			button = TTF_RenderText_Blended(font,"Game",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Main Menu
		dest.x = MENU_X+245;
		dest.y = MENU_Y+385;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 5)
			button = TTF_RenderText_Blended(font,"Main Menu",oColor);
		else
			button = TTF_RenderText_Blended(font,"Main Menu",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);
	}

	// Sound submenu
	else if(HUD.menu == 4)
	{
		// Sound effects volume
		dest.x = MENU_X+140;
		dest.y = MENU_Y+55;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 1)
			button = TTF_RenderText_Blended(font,"Effects Volume",oColor);
		else
			button = TTF_RenderText_Blended(font,"Effects Volume",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Draw slots for volume sliders
		dest.x += 235;
		dest.y += 15;
		temp = LoadSprite("hud/menuslots.png",126,13);
		DrawSprite(temp,screen,dest.x,dest.y,0);

		dest.y += 85;
		temp = LoadSprite("hud/menuslots.png",126,13);
		DrawSprite(temp,screen,dest.x,dest.y,0);

		// Update info based on slider position
		dest.x += 10;
		dest.w = 100;
		if( (audio.slide == 1) )
		{
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK)
			{
				audio.eVol = moveSlider(&audio.evSlider,&dest);
			}
			else
				audio.slide = 0;
		}

		else if( (audio.slide == 2) )
		{
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK)
			{
				audio.mVol = moveSlider(&audio.mvSlider,&dest);
			}
			else
				audio.slide = 0;
		}
		else
		{
			audio.evSlider.x = dest.x + (dest.w*audio.eVol);
			audio.mvSlider.x = dest.x + (dest.w*audio.mVol);
		}

		// Draw sliders
		temp = LoadSprite("hud/slider.png",50,24);
		button = IMG_Load("hud/slider.png");
		SDL_SetColorKey(button, 0, 0);
		temp->image = SDL_DisplayFormatAlpha(button);
		SDL_FreeSurface(button);

		temp = LoadSprite("hud/slider.png",50,24);
		DrawSprite(temp,screen,audio.evSlider.x-(audio.evSlider.w*.5),audio.evSlider.y,0);

		temp = LoadSprite("hud/slider.png",50,24);
		DrawSprite(temp,screen,audio.mvSlider.x-(audio.mvSlider.w*.5),audio.mvSlider.y,0);

		// Music volume
		dest.x = MENU_X+140;
		dest.y = MENU_Y+138;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 2)
			button = TTF_RenderText_Blended(font,"Music Volume",oColor);
		else
			button = TTF_RenderText_Blended(font,"Music Volume",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Mute
		dest.x = MENU_X+275;
		dest.y = MENU_Y+223;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 3)
			button = TTF_RenderText_Blended(font,"Mute",oColor);
		else
			button = TTF_RenderText_Blended(font,"Mute",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Back
		dest.x = MENU_X+285;
		dest.y = MENU_Y+385;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 5)
			button = TTF_RenderText_Blended(font,"Back",oColor);
		else
			button = TTF_RenderText_Blended(font,"Back",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);
	}

	// HUD submenu
	else if(HUD.menu == 6)
	{
		// Alpha level
		dest.x = MENU_X+175;
		dest.y = MENU_Y+55;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 1)
			button = TTF_RenderText_Blended(font,"Alpha Level",oColor);
		else
			button = TTF_RenderText_Blended(font,"Alpha Level",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Draw slot for alpha slider
		dest.x += 200;
		dest.y += 15;
		temp = LoadSprite("hud/menuslots.png",126,13);
		DrawSprite(temp,screen,dest.x,dest.y,0);

		// Update info based on slider position
		dest.x += 10;
		dest.w = 100;
		if( (aSlide == true) )
		{
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK)
			{
				aLevel = moveSlider(&aSlider,&dest);
			}
			else
				aSlide = false;
		}

		else
		{
			aSlider.x = dest.x + (dest.w*aLevel);
		}

		// Draw sliders
		temp = LoadSprite("hud/slider.png",50,24);
		DrawSprite(temp,screen,aSlider.x-(aSlider.w*.5),aSlider.y,0);

		// Save
		dest.x = MENU_X+285;
		dest.y = MENU_Y+138;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 2)
			button = TTF_RenderText_Blended(font,"Save",oColor);
		else
			button = TTF_RenderText_Blended(font,"Save",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Load
		dest.x = MENU_X+285;
		dest.y = MENU_Y+223;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 3)
			button = TTF_RenderText_Blended(font,"Load",oColor);
		else
			button = TTF_RenderText_Blended(font,"Load",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Back
		dest.x = MENU_X+285;
		dest.y = MENU_Y+385;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 5)
			button = TTF_RenderText_Blended(font,"Back",oColor);
		else
			button = TTF_RenderText_Blended(font,"Back",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);
	}

	// Game submenu
	else if(HUD.menu == 7)
	{
		// Scroll speed
		dest.x = MENU_X+175;
		dest.y = MENU_Y+55;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 1)
			button = TTF_RenderText_Blended(font,"Scroll Speed",oColor);
		else
			button = TTF_RenderText_Blended(font,"Scroll Speed",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);

		// Draw slot for scroll speed slider
		dest.x += 200;
		dest.y += 15;
		temp = LoadSprite("hud/menuslots.png",126,13);
		DrawSprite(temp,screen,dest.x,dest.y,0);

		dest.x += 10;
		dest.w = 100;

		if( (HUD.sSlide == true) )
		{
			if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON_LMASK)
			{
				sSpeed = moveSlider(&sSlider,&dest);
			}
			else
				HUD.sSlide = false;
		}

		else
		{
			sSlider.x = dest.x + (dest.w*sSpeed);
		}

		// Draw slider
		temp = LoadSprite("hud/slider.png",50,24);
		DrawSprite(temp,screen,sSlider.x-(sSlider.w*.5),sSlider.y,0);

		// Tool tips on/off
		dest.x = MENU_X+215;
		dest.y = MENU_Y+138;
		dest.w = 100;
		dest.h = 50;

		if(mouseOver == 2)
			button = TTF_RenderText_Blended(font,"Tool Tips",oColor);
		else
			button = TTF_RenderText_Blended(font,"Tool Tips",color);
		if(button != NULL)
		{
			SDL_BlitSurface(button,NULL,screen,&dest);
			SDL_FreeSurface(button);
		}

		dest.x += 150;
		if(HUD.ttips)
			button = TTF_RenderText_Blended(font,"On",color);
		else
			button = TTF_RenderText_Blended(font,"Off",color);
		if(button != NULL)
		{
			SDL_BlitSurface(button,NULL,screen,&dest);
			SDL_FreeSurface(button);
		}

		// Back
		dest.x = MENU_X+285;
		dest.y = MENU_Y+385;
		dest.w = 100;
		dest.h = 50;
		if(mouseOver == 5)
			button = TTF_RenderText_Blended(font,"Back",oColor);
		else
			button = TTF_RenderText_Blended(font,"Back",color);
		if(button != NULL)
			SDL_BlitSurface(button,NULL,screen,&dest);
		SDL_FreeSurface(button);
	}

	if(font != NULL)
		TTF_CloseFont(font);
}

