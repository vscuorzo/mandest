#include "hud.h"

// hud info
hInfo HUD;

// menu stuff
SDL_Color color = {255,255,255};	// Default text color
SDL_Color oColor = {255,0,0};		// Color on mouse over
SDL_Color bColor = {50,50,50};		// Color for items that are not an option
Uint8 mouseOver = 0;				// Which button the mouse is over

// Menu button 1
void mButt1(Uint8 n)
{
	// action is based on the menu tier
	switch(HUD.menu)
	{
	case 1:
		if(players[0].inGame)
			endGame();
		if(!Mix_PlayingMusic())
			startMusic();
		else if(!players[0].inGame)
		{
			Mix_HookMusicFinished(nextSong);
			Mix_FadeOutMusic(100);
		}
		startGame(MAX_PLAYERS);
		break;
	case 2:
		fprintf(stderr,"Saving not yet implemented.\n");
		break;
	case 3:
		HUD.menu = 4;
		break;
	case 4:
		audio.slide = 1;
		break;
	case 5:
		break;
	case 6:
		aSlide = true;
		break;
	case 7:
		HUD.sSlide = true;
		break;
	}
}

void mButt2(Uint8 n)
{
	switch(HUD.menu)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}
}

void mButt3(Uint8 n)
{
	switch(HUD.menu)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}
}

void mButt4(Uint8 n)
{
	switch(HUD.menu)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}
}

void mButt5(Uint8 n)
{
	switch(HUD.menu)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	}
}

// Update buttons. If num > -1, only update that button.
void updateButtons(Sint8 num)
{
	int i;
	fprintf(stderr,"num: %i\n",num);

	if(num > -1)
	{
		HUD.mButts[num].x = MENU_X + 130;
		HUD.mButts[num].y = MENU_Y + 40;
	}
	else
	{
		for(i=0;i<MAX_MENU_BUTTONS;++i)
		{
			// if it's a valid button, it will have an x value
			if(HUD.mButts[i].draw == NULL)
				break;

			HUD.mButts[i].x = MENU_X + 130;
			HUD.mButts[i].y = MENU_Y + 40 + (i*85);
			fprintf(stderr,"x: %i y: %i\n",HUD.mButts[i].x,HUD.mButts[i].y);
		}
	}
}

// Check if the cursor is on a button, return 1 if true and 0 otherwise
int bCollide(bInfo *b, int mx, int my)
{
	//fprintf(stderr,"bx: %i by: %i bw: %i bh: %i \nmx: %i my: %i\n",b->x,b->y,b->img->w,b->img->h,mx,my);
	// if you can't see it, you can't use it
	if(b->draw == false)
		return 0;

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

// Create a tool tip
void makeTip(bInfo *button, char *tip, SDL_Color fcolor)
{
	Uint8 fontSize;
	TTF_Font *font;
	SDL_Surface *text;

	fontSize = 10;
	font = TTF_OpenFont("fonts/font1.ttf",fontSize);
	if(font == NULL)
	{
		fprintf(stderr,"Font failed to load.\n");
	}

	fprintf(stderr,"%p\n",button);

	if(tip == NULL)
		return;

	// assign text color to the one specified or the default
	if(button == NULL || fcolor.r == NULL)
		button->tip.fColor = color;
	else
		button->tip.fColor = fcolor;

	strncpy(button->tip.info,tip,40);

	// create the bounding box
	text = TTF_RenderText_Blended(font, tip, button->tip.fColor);
	button->tip.bbox = SDL_CreateRGBSurface(SDL_HWSURFACE, text->clip_rect.w, text->clip_rect.h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	SDL_FillRect(button->tip.bbox, &button->tip.bbox->clip_rect, SDL_MapRGB(button->tip.bbox->format, 100,100,100));
	SDL_SetAlpha(button->tip.bbox, SDL_SRCALPHA|SDL_RLEACCEL, alpha );

	SDL_BlitSurface(text, NULL, button->tip.bbox, NULL);
	SDL_FreeSurface(text);
}

// Create a button
bInfo *makeButton(int type, int x, int y, int w, int h, void *action(Uint8), char *label)
{
	int i = 0;

	switch(type)
	{
	case UNIT:
		while(HUD.aUnits[i].draw != NULL)
		{
			++i;
			continue;
		}
		HUD.aUnits[i].draw = true;
		HUD.aUnits[i].x = x;
		HUD.aUnits[i].y = y;
		HUD.aUnits[i].w = w;
		HUD.aUnits[i].h = h;
		HUD.aUnits[i].action = action;
		if(label != NULL)
			strncpy(HUD.aUnits[i].label,label,15);
		return &HUD.aUnits[i];
	case STRUCTURE:
		while(HUD.aStructs[i].draw != NULL)
		{
			++i;
			continue;
		}
		HUD.aStructs[i].draw = true;
		HUD.aStructs[i].x = x;
		HUD.aStructs[i].y = y;
		HUD.aStructs[i].w = w;
		HUD.aStructs[i].h = h;
		HUD.aStructs[i].action = action;
		if(label != NULL)
			strncpy(HUD.aStructs[i].label,label,15);
		return &HUD.aStructs[i];
	case MENU:
		while(HUD.mButts[i].draw != NULL)
		{
			++i;
			continue;
		}
		HUD.mButts[i].draw = true;
		HUD.mButts[i].x = x;
		HUD.mButts[i].y = y;
		HUD.mButts[i].w = w;
		HUD.mButts[i].h = h;
		HUD.mButts[i].action = action;
		if(label != NULL)
			strncpy(HUD.mButts[i].label,label,15);
		return &HUD.mButts[i];
	case PLAYER:
		while(HUD.pButtons[i].draw != NULL)
		{
			++i;
			continue;
		}
		HUD.pButtons[i].draw = true;
		HUD.pButtons[i].x = x;
		HUD.pButtons[i].y = y;
		HUD.pButtons[i].w = w;
		HUD.pButtons[i].h = h;
		HUD.pButtons[i].action = action;
		if(label != NULL)
			strncpy(HUD.pButtons[i].label,label,15);
		return &HUD.pButtons[i];
	}
}

void initButtons()
{
	Uint8 i, fontSize;
	char tip[40];
	bInfo *bu;

	// Battle Lord
	bu = makeButton(UNIT, HUD.infox+20, HUD.infoy+108, 58, 58, enqueueUnit, NULL);
	bu->img = LoadSprite("icons/blord.png",36,47);
	bu->stats = &units[BATTLELORD];
	sprintf(tip, "Battle Loard $%i", units[BATTLELORD].price);
	makeTip(bu, tip, color);

	// Spider Demon
	bu = makeButton(UNIT, HUD.infox+83, HUD.infoy+108, 58, 58, enqueueUnit, NULL);
	bu->img = LoadSprite("icons/spider.png",48,27);
	bu->stats = &units[SPIDERDEMON];
	sprintf(tip, "Spider Demon $%i", units[SPIDERDEMON].price);
	makeTip(bu, tip, color);

	// Factory
	bu = makeButton(STRUCTURE, HUD.infox+20, HUD.infoy+40, 58, 58, placeStruct, NULL);
	bu->img = LoadSprite("icons/blord.png",36,47);
	bu->stats = &structs[BARRACKS];
	sprintf(tip, "Barracks $%i", structs[BARRACKS].price);
	makeTip(bu, tip, color);

	// Factory 2
	bu = makeButton(STRUCTURE, HUD.infox+83, HUD.infoy+40, 58, 58, placeStruct, NULL);
	bu->img = LoadSprite("icons/blord.png",39,50);
	bu->stats = &structs[FACTORY];
	sprintf(tip, "Factory $%i", structs[FACTORY].price);
	makeTip(bu, tip, color);

	// Media player
	// Previous
	bu = makeButton(PLAYER,HUD.playerx + 12,HUD.playery + 5,13,7,prev,NULL);
	//makeTip(bu, "Previous", color);

	// Play/pause
	makeButton(PLAYER,HUD.playerx + 30,HUD.playery + 3,8,10,play,NULL);

	// Stop
	makeButton(PLAYER,HUD.playerx + 42,HUD.playery + 4,8,7,stop,NULL);

	// Next
	makeButton(PLAYER,HUD.playerx + 56,HUD.playery + 5,13,7,next,NULL);

	// Menu buttons
	// New Game
	makeButton(MENU,130,40,380,65,mButt1,"New Game");

	// Resume
	makeButton(MENU,130,122,380,65,mButt2,"Resume");

	// Save/Load
	makeButton(MENU,130,205,380,65,mButt3,"Save/Load");

	// Options
	makeButton(MENU,130,285,380,65,mButt4,"Options");

	// Quit
	makeButton(MENU,130,365,380,65,mButt2,"Quit");


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
/*
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
*/
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
/*	hud = fopen("config/hud.txt","r");
	if(hud == NULL)
	{
		fprintf(stderr,"HUD.txt failed to open, using defaults\n");
		loadDefHUD();
	}

	else
	{
		loadHUD(hud);
	}
*/	
	// load the defaults but override them with whatever custom settings are available
	loadDefHUD();
	loadSet();

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
	int x, y, mx, my;
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

		// center and draw the menu panel and button backgrounds
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
	if(HUD.menu > 0)
	{
		for(i=0;i<MAX_MENU_BUTTONS;++i)
		{
			if(HUD.mButts[i].draw == NULL)
				continue;

			HUD.mButts[i].x += MENU_X;
			HUD.mButts[i].y += MENU_Y;

			dest.x = HUD.mButts[i].x;
			dest.y = HUD.mButts[i].y;
			dest.w = HUD.mButts[i].w;
			dest.h = HUD.mButts[i].h;
			SDL_GetMouseState(&mx,&my);

			if(bCollide(&HUD.mButts[i],mx,my))//mouseOver == 1)
				button = TTF_RenderText_Blended(font,HUD.mButts[i].label,oColor);
			else
				button = TTF_RenderText_Blended(font,HUD.mButts[i].label,color);
			if(button != NULL)
			{
				dest.x += (dest.w*0.5) - (button->w*0.5);
				dest.y += (dest.h*0.5) - (button->h*0.5);
				SDL_BlitSurface(button,NULL,screen,&dest);
			}

			HUD.mButts[i].x -= MENU_X;
			HUD.mButts[i].y -= MENU_Y;

			SDL_FreeSurface(button);
		}
	}

/*	if(HUD.menu == 1)
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
*/
	if(font != NULL)
		TTF_CloseFont(font);
}

