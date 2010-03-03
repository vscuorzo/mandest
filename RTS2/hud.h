#include "audio.h"
//#include "game.h"
//#include "SDL_ffmpeg.h"

#define MENU_X screen->w * .5 - 320
#define MENU_Y screen->h * .5 - 240
#define MAX_MENU_BUTTONS 30

// Used for creating buttons
#define UNIT 0
#define STRUCTURE 1
#define MENU 2
#define PLAYER 3

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
	char info[40];			// tool tip dialogue
	SDL_Surface *bbox;		// bounding box
	SDL_Color fColor;		// font color
}tInfo;

// button info structure for interface
typedef struct buttonInfo
{
	Sprite *img;					// unit icon
	char label[15];					// button label
	int w, h;						// width and height of button
	int x, y;						// location of button (actually the offset for menu buttons)
	void *stats;					// stat structure (uInfo/sInfo)
	gBool draw;						// should this be drawn on the interface
	tInfo tip;						// tool tip information
	void (*action)(Uint8 num);		// function to call when clicked
}bInfo;

// interface stuff
typedef struct hudInfo
{
	Uint8 menu;							// draw main menu
	bInfo mButts[MAX_MENU_BUTTONS];		// menu buttons
	bInfo aUnits[MAX_UNIT_TYPES];		// unit button info
	bInfo aStructs[MAX_STRUCT_TYPES];	// structure button info
	bInfo pButtons[4];					// media player button info
	Sint16 mapx, mapy;					// minimap location
	Sint16 mapw, maph;					// minimap size
	Sint16 infox, infoy;				// infobar location
	Sint16 infow, infoh;				// infobar size
	Sint16 playerx, playery;			// media player location
	Sint16 playerw, playerh;			// media player size
	gBool showPlayer;					// show/hide the player
	Uint8 grabPanel;					// which panel is being grabbed
	Sint16 nx,ny;						// distance from cursor to panel origin
	gBool ttips;						// tool tips on/off
	gBool sSlide;						// is scroll speed slider being moved
	SDL_Rect res;						// screen resolution
}hInfo;

extern hInfo HUD;
extern SDL_Surface *videobuffer;

void updateButtons(Sint8 num);
int bCollide(bInfo *b1, int mx, int my);
void initButtons();
void menuMouseOver(int *mx, int *my);
void saveHUD();
void drawInterface();
void loadHUD(FILE *hud);
void initInterface();
void movePanel(Sint16 x, Sint16 y);

gBool loadSet();
