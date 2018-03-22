#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Headers/gameObjects.h"

void RenderRect(SDL_Renderer *r,bool fill,GO_Rect rect){
    SDL_SetRenderDrawColor(r,rect.color.r,rect.color.g,rect.color.b,rect.color.a);
    if(fill){
        SDL_RenderFillRect(r,&rect.rect);
    }else{
        SDL_RenderDrawRect(r,&rect.rect);
    }
}

void RenderCircle(SDL_Renderer *r,bool fill,GO_Circle c){
    SDL_SetRenderDrawColor(r,c.color.r,c.color.g,c.color.b,c.color.a);
    if(fill){
        for(int w=0;w<c.radius*2;w++){
            for(int h=0;h<c.radius*2;h++){
                int dx = c.radius-w;
                int dy = c.radius-h;

                if((dx*dx + dy*dy) <= (c.radius*c.radius)){
                    SDL_RenderDrawPoint(r,c.center.x+dx,c.center.y+dy);
                }
            }
        }
    }else{
        printf("This is not implemented yet!\n");
    }
}

void InitRect(GO_Rect *rect,int x,int y,int w,int h,Uint8 r,Uint8 g,Uint8 b,Uint8 a){
    rect->rect.x = x;
    rect->rect.y = y;
    rect->rect.w = w;
    rect->rect.h = h;

    rect->color.r = r;
    rect->color.g = g;
    rect->color.b = b;
    rect->color.a = a;
}

void InitCircle(GO_Circle *c,int x,int y,int rad,GO_Color color){
    c->center.x = x;
    c->center.y = y;
    c->color = color;
    c->radius = rad;
}

void InitColor(GO_Color *c,Uint8 r,Uint8 g,Uint8 b,Uint8 a){
    c->r = r;
    c->g = g;
    c->b = b;
    c->a = a;
}