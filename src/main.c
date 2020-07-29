#include <SDL2/SDL.h>

#define FALSE 0
#define TRUE 1
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FPS 60
#define FRAME_TIME_LENGTH (1000 / FPS)


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int is_running = FALSE;
int last_frame_time;

struct player {
    float x;
    float y;
    float width;
    float height;
} player, player2;

struct ball {
    float x;
    float y;
    float width;
    float height;
    float vel;
} ball;


int initialize_window() {
    if (SDL_Init(SDL_INIT_EVERYTHING) !=0){
        fprintf(stderr, "Error initializing SDL. \n");
        return FALSE;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );

    if (!window){
        fprintf(stderr,"Error creating SDL window.\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){
        fprintf(stderr, "Error creating SDL renderer.\n");
    }

    return TRUE;
}

void process_input(void){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:{
        is_running = FALSE;
        break;
    }

    case SDL_KEYDOWN:{
        if (event.key.keysym.sym == SDLK_UP){
            if (player.y!=0) {
                player.y -= 10;
            }
            
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
            if ((player.y + player.height) != WINDOW_HEIGHT){
                player.y += 10;
            }
        }
        else if (event.key.keysym.sym == SDLK_ESCAPE){
            is_running = FALSE;
        }
            
        break;
    }


    default:
        break;
    }
}

void setup(void){
    player.x = 20;
    player.y = 20;
    player.width = 20;
    player.height = 100;

    player2.x = WINDOW_WIDTH - (player.x + player.width) ;
    player2.y = player.y;
    player2.width = player.width;
    player2.height = player.height;

    ball.x = 400;
    ball.y = 400;
    ball.width = 10;
    ball.height = 10;
    ball.vel = -40;
}

void check_collision(){
    //Basic mirror collision
    if ((player.x + player.width) >= ball.x){
        ball.vel *= -1;
    }
}

void update(void){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TIME_LENGTH));
    float delta_time = (SDL_GetTicks() - last_frame_time)/1000.0f;
    last_frame_time = SDL_GetTicks();

    ball.x += ball.vel * delta_time;
    check_collision();

}



void render(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect player1_rect = {
        (int)player.x, 
        (int)player.y, 
        (int)player.width, 
        (int)player.height};
    SDL_RenderFillRect(renderer, &player1_rect);

    SDL_Rect player2_rect = {
        (int)player2.x, 
        (int)player2.y, 
        (int)player2.width, 
        (int)player2.height};
    SDL_RenderFillRect(renderer, &player2_rect);

    SDL_Rect ball_rect = {
    (int)ball.x, 
    (int)ball.y, 
    (int)ball.width, 
    (int)ball.height};
    SDL_RenderFillRect(renderer, &ball_rect);
    
    
    //Swap buffers
    SDL_RenderPresent(renderer);

}


void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main(int argc, char *argv[]){
 
    is_running = initialize_window();

    setup();
    while (is_running){
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}