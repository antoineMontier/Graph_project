#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "SDL_Basics.h"
#include "graph.h"
#define IN 1
#define OUT -1
#define NO_ZOOM 0
#define ZOOM_POWER 0.05

int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    SDL_Color *palette = initialiseColors();

    srand(time(0));
    int zoom = NO_ZOOM;
    SDL_Window *w;
    SDL_Renderer *r;

    SDL_Event evt;
    SDL_bool program_launched = SDL_TRUE;
    openSDL(WIDTH, HEIGHT, 0, &w, &r);
    TTF_Font *f;
    setFont(&f, "Roboto-Black.ttf", VERTEX_SIZE * 1.5);

    char *tmp = malloc(10);
    Graph g;
    createGraph(&g, 200, 1000);

    int _w = WIDTH, _h = HEIGHT, m_x = WIDTH / 2, m_y = HEIGHT / 2, click = 0, p_mx = 0, p_my = 0, caps = 0, click_x = -1, click_y = -1, link_mode = 0;
    double c_x = 0, c_y = 0;
    double theta, alpha;
    creatCoordinatesSystem("./coordinates.txt", &g);

    while (program_launched)
    {
        //printf("%d\n", g.nb_arete);

        if (zoom == IN)
        {
            c_x -= WIDTH * ZOOM_POWER / 2;
            c_y -= HEIGHT * ZOOM_POWER / 2;
            _w *= 1 + ZOOM_POWER;
            _h *= 1 + ZOOM_POWER;
            zoom = NO_ZOOM;
        }
        else if (zoom == OUT)
        {
            c_x += WIDTH * ZOOM_POWER / 2;
            c_y += HEIGHT * ZOOM_POWER / 2;
            _w *= 1 - ZOOM_POWER;
            _h *= 1 - ZOOM_POWER;
            zoom = NO_ZOOM;
        }

        background(r, 255, 255, 255, WIDTH, HEIGHT);
        if (click)
        {
            displayGraph(r, f, &g, tmp, palette, c_x, c_y, _w, _h);
            c_x += (-p_mx + m_x) * 7;
            c_y += (-p_my + m_y) * 6;
            p_my = m_y;
            p_mx = m_x;
        }

        else
            displayGraph(r, f, &g, tmp, palette, c_x, c_y, _w, _h);

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

                case SDLK_l:
                    link_mode = !link_mode; // escape the program by pressing esc
                    break;

                case SDLK_CAPSLOCK:
                    caps = !caps;
                    break;

                default:
                    break;
                }
            case SDL_MOUSEWHEEL:
                zoom = evt.wheel.y;
                break;

            case SDL_MOUSEMOTION:
                if (click)
                {
                    p_mx = m_x;
                    p_my = m_y;
                }
                m_x = evt.button.x;
                m_y = evt.button.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (link_mode)
                {
                    //printf("(%d ; %d)       (%d ; %d)\n", evt.button.x, evt.button.y, click_x, click_y);
                    if (linkByClick("./coordinates.txt", &g, click_x, click_y, evt.button.x, evt.button.y, caps, c_x, c_y, _w, _h))
                    {
                        click_x = -1;//avoid repeat
                        click_y = -1;
                    }
                    else
                    {
                        click_x = evt.button.x;
                        click_y = evt.button.y;
                    }
                }
                else
                {
                    p_mx = evt.button.x;
                    p_my = evt.button.y;
                    m_x = evt.button.x;
                    m_y = evt.button.y;
                    click = 1;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                click = 0;
                break;

            default:
                break;
            }
        }
        SDL_RenderPresent(r); // refresh the render
        SDL_Delay(200);
    }
    free(palette);
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    printf("Closed sucessfully !\n");
    return 0;
}