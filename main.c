#define SPEED 5
#define WIDTH 1000
#define HEIGHT 600
#define RESET
#define TEXTURE
//#define FIRST_IMPLEMENTATION
#include <stdbool.h>
#include "Headers/gameObjects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static bool *keys;
static float velX = 0,velY=0;
static int pOne=0,pTwo=0;
static bool isVelSet = false;
static SDL_Renderer *render;
static GO_Rect p1,p2;
static GO_Circle ball;
static GO_Color ball_color;
static TTF_Font *Font;// = TTF_OpenFont("Arial.ttf",24);
static SDL_Color textColor = {255,255,255};
static SDL_Surface *p1ScoreS;
static SDL_Surface *p2ScoreS;
static SDL_Texture *p1ScoreT;
static SDL_Texture *p2ScoreT;
static SDL_Rect p1messageR,p2messageR;
static char *s1;// = calloc(3,sizeof(char));
static char *s2; 

#pragma region Utils
float RandomFloat(float x){
    return ((float)rand()/(float)(RAND_MAX)) * x;
}

void IntToString(char *s,int i){
    sprintf(s,"%i",i);
}
#ifdef TEXTURE
void SetTextures(){
    if(p1ScoreS != NULL){
        SDL_FreeSurface(p1ScoreS);
        SDL_FreeSurface(p2ScoreS);
    }

    IntToString(s1,pOne);
    IntToString(s2,pTwo);
    p1ScoreS = TTF_RenderText_Solid(Font,s1,textColor);
    p2ScoreS = TTF_RenderText_Solid(Font,s2,textColor);
    p1ScoreT = SDL_CreateTextureFromSurface(render,p1ScoreS);
    p2ScoreT = SDL_CreateTextureFromSurface(render,p2ScoreS);
    p1messageR.x=50;
    p1messageR.y=0;
    p1messageR.w=50;
    p1messageR.h=50;

    p2messageR.x=WIDTH-100;
    p2messageR.y=0;
    p2messageR.w=50;
    p2messageR.h=50;
}
#endif

int roundI(float x){
    if(x-0.5 >= (int)x){
        return ((int)x)+1;
    }else{
        return ((int)x);
    }
}

float RandomSpeed(){
    float x;
    do{
        x = (RandomFloat(2)-1)*5.0;
    }while(x<3.5 && x> -3.5);
}
#pragma endregion
#ifndef FIRST_IMPLEMENTATION
bool Intersect(GO_Rect r,GO_Circle c){
    if(c.center.x+c.radius >= r.rect.x && c.center.x+c.radius <= r.rect.x+r.rect.w){
        if(c.center.y+c.radius>=r.rect.y && c.center.y-c.radius<=r.rect.y+r.rect.h){
            return true;
        }
    }
    if(c.center.x-c.radius <= r.rect.x+r.rect.w && c.center.x-c.radius >= r.rect.x){
        if(c.center.y+c.radius>=r.rect.y && c.center.y-c.radius<=r.rect.y+r.rect.h){
            return true;
        }
    }
    return false;
}
#endif

void Reset(){
    velX = 0.0;
    velY = 0.0;
    isVelSet = false;

    InitCircle(&ball,500,300,15,ball_color);
    InitRect(&p1,10,275,10,50,255,255,255,255);
    InitRect(&p2,1000-10-10,275,10,50,255,255,255,255);
}

void SetSpeeds(){
    velX=RandomSpeed();
    velY=RandomSpeed();
    isVelSet = true;
}
void Move(GO_Rect *o1,GO_Rect *o2,GO_Circle *c){
    if(*(keys+SDL_SCANCODE_W)){
        if(o1->rect.y >0)
            o1->rect.y-=SPEED;
    }
    if(*(keys+SDL_SCANCODE_S)){
        if(o1->rect.y < 600-50)
            o1->rect.y+=SPEED;
    }
    if(*(keys+SDL_SCANCODE_UP)){
        if(o2->rect.y >0)
            o2->rect.y-=SPEED;
    }
    if(*(keys+SDL_SCANCODE_DOWN)){
        if(o2->rect.y < 600-50)
            o2->rect.y+=SPEED;
    }

    #ifdef FIRST_IMPLEMENTATION
    if(c->center.x+c->radius < 50){
        if(!(c->center.y+c->radius>=o1->rect.y && c->center.y-c->radius<=o1->rect.y+o1->rect.w)){
            printf("Player 1 losed\n");
        }else{
            velX = velX*(-1);
        }
    }
    if(c->center.x+c->radius > 1000-25){
        if(!(c->center.y+c->radius>=o2->rect.y && c->center.y-c->radius<=o2->rect.y+o2->rect.w)){
            printf("Player 2 losed\n");
        }else{
            velX = velX*(-1);
        }
    }
    #else
    if(Intersect(*o1,*c) && velX < 0){
        velX = velX*(-1);
        if(velX>0){
            velX+=RandomFloat(1);
        }else{
            velX-=RandomFloat(1);
        }
        if(velY>0){
            velY+=RandomFloat(1);
        }else{
            velY-=RandomFloat(1);
        }
        #ifdef DEBUG
        printf("The current speeds are %f %f\n",velX,velY);
        #endif
    }else if(Intersect(*o2,*c) && velX>0){
        velX = velX*(-1);
        if(velX>0){
            velX+=RandomFloat(1);
        }else{
            velX-=RandomFloat(1);
        }
        if(velY>0){
            velY+=RandomFloat(1);
        }else{
            velY-=RandomFloat(1);
        }
        #ifdef DEBUG
        printf("The current speeds are %f %f\n",velX,velY);
        #endif
    }

    if(c->center.x<0){
        printf("Player 1 loses!\n");
        pTwo++;
        printf("The current stance is %i | %i!\n",pOne,pTwo);
        #ifdef TEXTURE
        SetTextures();
        #endif
        #ifdef RESET
        Reset();
        #endif
    }
    if(c->center.x>WIDTH){
        printf("Player 2 loses!\n");
        pOne++;
        printf("The current stance is %i | %i!\n",pOne,pTwo);
        #ifdef TEXTURE
        SetTextures();
        #endif
        #ifdef RESET
        Reset();
        #endif
    }
    #endif

    if((c->center.y-c->radius < 0 && velY<0) || (c->center.y+c->radius > 600 && velY>0)){
        velY = velY*(-1);
    }
    c->center.x+=roundI(velX);
    c->center.y+=roundI(velY);

}

int main(int argc,char **argv){
    #ifdef TEXTURE
    SDL_Init(SDL_INIT_VIDEO);
    if(TTF_Init()<0){
        printf("Failed to initialise TTF\n");
        return -1;
    }
    #endif
    //Intit
    //Vars
    #ifdef TEXTURE
    s1 = calloc(3,sizeof(char));
    s2 = calloc(3,sizeof(char));
    Font = TTF_OpenFont("arial.ttf",25);
    if(Font == NULL){
        printf("Failed to open font\n");
        return -1;
    }else{
        printf("Opened ttf\n");
    }
    #endif

    //Keyarray
    keys = calloc(284,sizeof(bool));
    if(keys == NULL){
        printf("Key array creation failed! Not enough memory! The size of a bool is %lui\n",sizeof(bool));
        return -1;
    }
    //Textures
    //p1ScoreS = TTF_RenderText_solid(Font);


    SDL_Window *window = SDL_CreateWindow("The perfect game",0,0,WIDTH,HEIGHT,SDL_WINDOW_OPENGL);
    render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    bool shouldRun = true;
    SDL_Event ev;


    InitColor(&ball_color,255,255,255,255);
    InitCircle(&ball,500,300,15,ball_color);
    InitRect(&p1,10,275,10,50,255,255,255,255);
    InitRect(&p2,1000-10-10,275,10,50,255,255,255,255);
    SetTextures();
    while(shouldRun){
        while(SDL_PollEvent(&ev)){
            //printf("Event type: %i\n",ev.type);
            if(ev.type == SDL_KEYDOWN){
                #ifdef DEBUG
                printf("The user has pressed a key: %i\n",ev.key.keysym.scancode);
                #endif
                *(keys+ev.key.keysym.scancode) = true;
                if(ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    shouldRun = false;
                }else if(ev.key.keysym.scancode == SDL_SCANCODE_SPACE){
                    if(!isVelSet){
                        //Set speeds
                        SetSpeeds();
                    }
                }else if(ev.key.keysym.scancode == SDL_SCANCODE_R){
                    //Reinit
                    Reset();
                }
            }else if(ev.type == SDL_KEYUP){
                #ifdef DEBUG
                printf("The user has released a key: %i\n",ev.key.keysym.scancode);
                #endif
                *(keys+ev.key.keysym.scancode) = false;
            }
            if(ev.type == SDL_QUIT){
                shouldRun = false;
            }
        }

        SDL_SetRenderDrawColor(render,0,0,0,255);
        SDL_RenderClear(render);

        Move(&p1,&p2,&ball);
        SDL_RenderCopy(render,p1ScoreT,NULL,&p1messageR);
        SDL_RenderCopy(render,p2ScoreT,NULL,&p2messageR);

        RenderRect(render,true,p1);
        RenderRect(render,true,p2);
        RenderCircle(render,true,ball);

        SDL_RenderPresent(render);
    }

    //Resource cleanup
    free(keys);
    SDL_FreeSurface(p1ScoreS);
    SDL_FreeSurface(p2ScoreS);
    SDL_DestroyTexture(p1ScoreT);
    SDL_DestroyTexture(p2ScoreT);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    #ifdef TEXTURE
    TTF_CloseFont(Font);
    TTF_Quit();
    #endif
    SDL_Quit();
}