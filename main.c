#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "SDL_Basics.h"
#include "graph.h"
#define IN 1
#define OUT -1
#define NO_ZOOM 0



int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    

    SDL_Color*palette = initialiseColors();

    srand(time(0));
    int zoom = NO_ZOOM;
    SDL_Window *w;
    SDL_Renderer *r;

    SDL_Event evt;
    SDL_bool program_launched = SDL_TRUE;
    openSDL(WIDTH, HEIGHT, 0, &w, &r);
    TTF_Font *f;
    setFont(&f, "Roboto-Black.ttf", VERTEX_SIZE*1.5);

    char *tmp = malloc(10);
    Graph g;
    createGraph(&g, 200, 1000);
    /*addArete(&g, id++, 0, 1, NO_WEIGHT);
    addArete(&g, id++, 0, 7, NO_WEIGHT);
    addArete(&g, id++, 2, 14, NO_WEIGHT);
    addArete(&g, id++, 1, 9, NO_WEIGHT);
    addArete(&g, id++, 4, 5, NO_WEIGHT);
    addArete(&g, id++, 5, 6, NO_WEIGHT);
    addArete(&g, id++, 7, 2, NO_WEIGHT);
    addArete(&g, id++, 14, 4, NO_WEIGHT);
    addArete(&g, id++, 4, 13, NO_WEIGHT);
    addArete(&g, id++, 13, 12, NO_WEIGHT);
    addArete(&g, id++, 12, 6, NO_WEIGHT);
    addArete(&g, id++, 6, 11, NO_WEIGHT);
    addArete(&g, id++, 7, 8, NO_WEIGHT);
    addArete(&g, id++, 9, 3, NO_WEIGHT);
    addArete(&g, id++, 9, 8, NO_WEIGHT);
    addArete(&g, id++, 3, 10, NO_WEIGHT);
    addArete(&g, id++, 14, 15, NO_WEIGHT);
    addArete(&g, id++, 15, 16, NO_WEIGHT);
    addArete(&g, id++, 16, 6, NO_WEIGHT);
    addArete(&g, id++, 16, 5, NO_WEIGHT);
    addArete(&g, id++, 8, 19, NO_WEIGHT);
    addArete(&g, id++, 19, 18, NO_WEIGHT);
    addArete(&g, id++, 19, 20, NO_WEIGHT);
    addArete(&g, id++, 20, 21, NO_WEIGHT);
    addArete(&g, id++, 21, 16, NO_WEIGHT);
    addArete(&g, id++, 10, 17, NO_WEIGHT);
    addArete(&g, id++, 10, 11, NO_WEIGHT);
    addArete(&g, id++, 2, 8, NO_WEIGHT);
    addArete(&g, id++, 8, 3, NO_WEIGHT);
    addArete(&g, id++, 0, 1, NO_WEIGHT);*/


    int _w = WIDTH, _h = HEIGHT;





    while (program_launched)
    {
        if(zoom == IN){
            _w *= 1.01;
            _h *= 1.01;
            zoom = NO_ZOOM;
        }else if(zoom == OUT){
            _w *= 0.99;
            _h *= 0.99;
            zoom = NO_ZOOM;        }
        creatCoordinatesSystem("./coordinates.txt", &g, _w, _h);
        background(r, 255, 255, 255, WIDTH, HEIGHT);
        displayGraph(r, f, &g, tmp, palette);
        SDL_RenderPresent(r); // refresh the render

        while (SDL_PollEvent(&evt))
        { // reads all the events (mouse moving, key pressed...)        //possible to wait for an event with SDL_WaitEvent
            switch (evt.type)
            {

            case SDL_QUIT:
                program_launched = SDL_FALSE; // quit the program if the user closes the window
                break;

            case SDL_KEYDOWN: // SDL_KEYDOWN : hold a key            SDL_KEYUP : release a key
                switch (evt.key.keysym.sym)
                { // returns the key ('0' ; 'e' ; 'SPACE'...)

                case SDLK_ESCAPE:
                    program_launched = SDL_FALSE; // escape the program by pressing esc
                    break;

                default:
                    break;
                }
            case SDL_MOUSEWHEEL:
                zoom = evt.wheel.y;
                break;
            default:
                break;
            }
        }
        SDL_Delay(100);
    }
    free(palette);
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}