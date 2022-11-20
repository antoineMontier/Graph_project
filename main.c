#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "SDL_Basics.h"
#include "graph.h"



int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    SDL_Color *palette = malloc(NB_COLOR * sizeof(SDL_Color));

    palette[0].r = 252; // yellow
    palette[0].g = 186;
    palette[0].b = 1;

    palette[1].r = 252; // red
    palette[1].g = 3;
    palette[1].b = 3;

    palette[2].r = 147; // lime
    palette[2].g = 252;
    palette[2].b = 0;

    palette[5].r = 3; // soft blue
    palette[5].g = 215;
    palette[5].b = 252;

    palette[3].r = 252; // soft pink
    palette[3].g = 134;
    palette[3].b = 219;

    palette[4].r = 163; // light brown
    palette[4].g = 144;
    palette[4].b = 3;

    palette[10].r = 250; // beige
    palette[10].g = 237;
    palette[10].b = 137;

    palette[6].r = 193; // soft green
    palette[6].g = 255;
    palette[6].b = 143;

    palette[7].r = 86; // kaki
    palette[7].g = 125;
    palette[7].b = 55;

    palette[8].r = 39; // hard green
    palette[8].g = 87;
    palette[8].b = 1;

    palette[9].r = 5; // azur
    palette[9].g = 245;
    palette[9].b = 145;

    palette[11].r = 5; // deep blue
    palette[11].g = 25;
    palette[11].b = 250;

    palette[12].r = 90; // hard magenta
    palette[12].g = 7;
    palette[12].b = 245;

    palette[13].r = 175; // soft magenta
    palette[13].g = 134;
    palette[13].b = 252;

    palette[14].r = 66; // hard brown
    palette[14].g = 58;
    palette[14].b = 1;

    for (int i = 0; i < NB_COLOR; i++)
        palette[i].a = 1;

    srand(time(0));

    SDL_Window *w;
    SDL_Renderer *r;

    SDL_Event evt;
    SDL_bool program_launched = SDL_TRUE;
    openSDL(WIDTH, HEIGHT, 0, &w, &r);
    TTF_Font *f;
    setFont(&f, "Roboto-Black.ttf", 20);

    char *tmp = malloc(10);
    Graph g;
    createGraph(&g, 20, 1000);

    addVertex(&g, 0, 820, 530, 1);
    addVertex(&g, 1, 870, 330, 2);
    addVertex(&g, 2, 920, 230, 3);
    addVertex(&g, 3, 1120, 430, 2);

    addArete(&g, 0, 0, 3, 5);
    addArete(&g, 1, 1, 0, 15);
    addArete(&g, 2, 3, 2, 0);
    addArete(&g, 4, 2, 0, 3);
    addArete(&g, 5, 3, 3, 5);
    circlePoints(&g);

    while (program_launched)
    {

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
            default:
                break;
            }
        }
        SDL_Delay(500);
    }
    free(palette);
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}