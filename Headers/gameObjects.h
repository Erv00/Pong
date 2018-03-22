#ifndef gameObjects
#define gameObjects
#include <SDL2/SDL.h>

struct color{
    Uint8 r,g,b,a;
} typedef GO_Color;

struct go_Rect{
    SDL_Rect rect;
    GO_Color color;
} typedef GO_Rect;

struct do_Circle{
  SDL_Point center;
  int radius;
  GO_Color color;  
}typedef GO_Circle;

void RenderRect(SDL_Renderer *r,bool fill,GO_Rect rect);
void RenderCircle(SDL_Renderer *r,bool fill,GO_Circle c);
void InitRect(GO_Rect *rect,int x,int y,int w,int h,Uint8 r,Uint8 g,Uint8 b,Uint8 a);
void InitCircle(GO_Circle *c,int x,int y,int rad,GO_Color color);
void InitColor(GO_Color *c,Uint8 r,Uint8 g,Uint8 b,Uint8 a);
#endif