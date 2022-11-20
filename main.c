#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "SDL_Basics.h"
#include "graph.h"



int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    FILE*reader = fopen("./coordinates.txt", "r");
    if(reader == NULL)
        fprintf(stderr, "failed to open text file\n");

    SDL_Color*palette = initialiseColors();

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
    creatCoordinatesSystem(reader, &g);

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
        SDL_Delay(100);
    }
    fclose(reader);
    free(palette);
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}