#ifndef _GRAPHICS_
#define _GRAPHICS_
/*
 *			graphics.h
 *			Originaly written by Donald Kehoe for the NJIT Introduction to game development class
 *			This file contains data structures, constants and function prototypes needed to initialize SDL and
 *			to handle sprites and graphics.
 */


#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

/*uint32*/
#define Red_         0xDD0000
#define Green_       0x00DD00
#define Blue_        0x0000DD
#define Yellow_      0xDDDD00
#define Orange_      0xEE5522
#define Violet_      0xDD00DD
#define Brown_       0x663311
#define Grey_        0x888888
#define DarkRed_     0x880000
#define DarkGreen_   0x008800
#define DarkBlue_    0x000088
#define DarkYellow_  0x888800
#define DarkOrange_  0x774411
#define DarkViolet_  0x660066
#define DarkBrown_   0x442205
#define DarkGrey_    0x444444
#define LightRed_    0xFF2222
#define LightGreen_  0x33FF33
#define LightBlue_   0x3333FF
#define LightYellow_ 0xFFFF11
#define LightOrange_ 0xFFDD11
#define LightViolet_ 0xFF55FF
#define LightBrown_  0x886622
#define LightGrey_   0xBBBBBB
#define Black_       0x000000
#define White_       0xFEFEFE
#define Tan_         0xFFDD22
#define Gold_        0xBB9900
#define Silver_      0xAAAABB
#define YellowGreen_ 0x44EE01
#define Cyan_        0x00FFFF
#define Magenta_     0xFF00FF


/*color constants:*/
enum colors {Red = 1,Green = 2,Blue = 3,Yellow = 4,Orange = 5,Violet = 6,Brown = 7,Grey = 8,DarkRed = 9,DarkGreen = 10,
               DarkBlue = 11,DarkYellow = 12,DarkOrange = 13,DarkViolet = 14,DarkBrown = 15,DarkGrey = 16,LightRed =17,
               LightGreen = 18,LightBlue = 19,LightYellow = 20,LightOrange = 21,LightViolet = 22,LightBrown = 23,LightGrey = 24,
               Black = 25,White = 26,Tan = 27,Gold = 28,Silver = 29,YellowGreen = 30,Cyan = 31,Magenta = 32};

typedef struct Sprite_T
{
  SDL_Surface *image;		/*pointer to the actual image in memory*/
  char filename[20];			/*the name of the file, keep them short*/
  int w, h;					/*the width and height of the frames of the sprites, not the file itself*/
  int framesperline;			/*default is 16*/
  int color1,color2,color3;		/*if the file was palette swapped these are the colors that were changed*/
  int used;					/*used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;

typedef struct	/*this didn't need to be its own type, its only used once*/
{
    int xres,yres,depth;
}ScreenData;

int changeRes(int w, int h, int depth, Uint32 flags);
void Init_Graphics();
void DrawPixel(SDL_Surface *screen, Uint8 R, Uint8 G, Uint8 B, int x, int y);
void BlankScreen(SDL_Surface *buf,Uint32 color);
void SwapSprite(SDL_Surface *sprite,int color1,int color2,int color3);
void InitSpriteList();
void FreeSprite(Sprite *img);
Sprite *LoadSprite(char *filename,int sizex, int sizey);		/*simplest file to load*/
Sprite *LoadSpriteAlpha(char *filename,int sizex, int sizey); // loads with alpha channel
Sprite *LoadSwappedSprite(char *filename,int sizex, int sizey, int c1, int c2, int c3);
/*drawsprite and drawspritepart work the same except that Drawsprite will render an entire sprite on the desired srface
  and drawspritepart can have the size of the sprite specified.  Know your sprite when using DSP since you can in theory
  read too far, not that SDL will allow that.*/
void DrawSprite(Sprite *sprite,SDL_Surface *surface,int sx,int sy, int frame);
void CloseSprites();		/*call this before you exit to make sure all the memory that your program used is given back to your OS*/

Uint32 IndexColor(int color);
void FrameDelay(Uint32 delay);
void ResetBuffer();
void NextFrame();
void InitMouse();
void DrawMouse();

SDL_Rect **reslist;

#endif
