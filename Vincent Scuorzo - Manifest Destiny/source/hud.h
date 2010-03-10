#include "audio.h"
//#include "game.h"
//#include "SDL_ffmpeg.h"

#define MENU_X screen->w * .5 - 320
#define MENU_Y screen->h * .5 - 240

// HUD alpha adjustment
gBool aSlide;				// is alpha slider being moved
SDL_Rect aSlider;			// alpha slider position and size
Sint16 alpha;				// current alpha for panels, used for transparency and fading in/out
float aLevel;				// max alpha level, percentage

// Scroll speed adjustment
SDL_Rect sSlider;

// tool tip info
typedef struct toolTip
{
	SDL_Surface *bbox;
	SDL_Color fColor;
}tInfo;

// button info structure for interface
typedef struct buttonInfo
{
	Sprite *img;					// unit icon
	int w, h;						// width and height of button
	int x, y;						// location of button
	void *stats;					// stat structure (uInfo/sInfo)
	gBool draw;						// should this be drawn on the interface
	tInfo tip;						// tool tip information
	void (*action)(Uint8 num);		// function to call when clicked
}bInfo;

// interface stuff
typedef struct hudInfo
{
	Uint8 menu;							// draw main menu
	bInfo aUnits[MAX_UNIT_TYPES];		// unit button info
	bInfo aStructs[MAX_STRUCT_TYPES];	// structure button info
	Sint16 mapx, mapy;					// minimap location
	Sint16 mapw, maph;					// minimap size
	Sint16 infox, infoy;				// infobar location
	Sint16 infow, infoh;				// infobar size
	Sint16 playerx, playery;			// media player location
	Sint16 playerw, playerh;			// media player size
	gBool showPlayer;					// show/hide the player
	bInfo pButtons[4];					// media player button info
	Uint8 grabPanel;					// which panel is being grabbed
	Sint16 nx,ny;						// distance from cursor to panel origin
	gBool ttips;						// tool tips on/off
	gBool sSlide;						// is scroll speed slider being moved
}hInfo;

extern hInfo HUD;
int bCollide(bInfo *b1, int mx, int my);
void initButtons();
void menuMouseOver(int *mx, int *my);
void saveHUD();
void drawInterface();
void loadHUD(FILE *hud);
void initInterface();
void movePanel(Sint16 x, Sint16 y);
