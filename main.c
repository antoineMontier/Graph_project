#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL_Basics.h"

#define NO_COLOR -1

typedef struct
{
    int id; // position in the array
    int x;
    int y;
    int card;
    int color;
} Vertex;

typedef struct
{
    int id;
    int start;
    int end;
} Arete;

typedef struct
{
    int nb_vertex;
    int nb_arete;
    int v, a;        // max size
    Vertex *vertexs; // size v
    Arete *aretes;   // size a
} Graph;

void createGraph(Graph *, int v, int a);
void destructGraph(Graph *);
void addVertex(Graph *, int id, int x, int y);
void addArete(Graph *, int id, int start, int end);
void printGraph(Graph *g);
void printVertex(Vertex *v);
void printArete(Arete *a);
void createCompleteGraph(Graph *g, int n);
void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp);

int main()
{ /*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    SDL_Window *w;
    SDL_Renderer *r;

    SDL_Event evt;
    SDL_bool program_launched = SDL_TRUE;
    openSDL(WIDTH, HEIGHT, 0, &w, &r);
    TTF_Font *f;
    setFont(&f, "Roboto-Black.ttf", 20);

    char *tmp = malloc(10);
    Graph g;
    createGraph(&g, 100, 10000);

    int i = 0;

    while (program_launched && i <= 50)
    {
        createCompleteGraph(&g, i);
        background(r, 255, 255, 255, WIDTH, HEIGHT);

        displayGraph(r, f, &g, tmp);
        i++;
        SDL_Delay(500);
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
    }
    free(tmp);
    TTF_CloseFont(f);
    destructGraph(&g);
    closeSDL(&w, &r);
    return 0;
}

void createGraph(Graph *g, int v, int a)
{
    g->v = v;
    g->a = a;
    g->vertexs = malloc(v * sizeof(Vertex));
    g->aretes = malloc(a * sizeof(Arete));
    g->nb_arete = 0;
    g->nb_vertex = 0;
}

void destructGraph(Graph *g)
{
    g->v = 0;
    g->a = 0;
    free(g->vertexs);
    free(g->aretes);
    g->nb_arete = 0;
    g->nb_vertex = 0;
}

void addVertex(Graph *g, int id, int x, int y)
{
    if (g->v <= g->nb_vertex)
    {
        printf("you reached the max amount of vertex\n");
        return;
    }
    Vertex v;
    v.id = id;
    v.x = x;
    v.y = y;
    v.card = 0;
    v.color = NO_COLOR;
    (g->vertexs)[g->nb_vertex] = v;
    (g->nb_vertex)++;
}

void addArete(Graph *g, int id, int start_id, int end_id)
{
    if (g->a <= g->nb_arete)
    {
        printf("you reached the max amount of arete\n");
        return;
    }
    Arete a;
    a.id = id;
    a.start = start_id;
    a.end = end_id;
    (g->aretes)[g->nb_arete] = a;
    (g->nb_arete)++;

    (g->vertexs)[start_id].card++;
    (g->vertexs)[end_id].card++;
}

void printGraph(Graph *g)
{
    printf("graph : %d vertex, %d arete\n", g->nb_vertex, g->nb_arete);
    for (int i = 0; i < g->nb_vertex; i++)
        printVertex((g->vertexs) + i);
    for (int i = 0; i < g->nb_arete; i++)
        printArete((g->aretes) + i);
    ;
    printf("\n");
}

void printVertex(Vertex *v)
{
    printf("===================== Vertex\n----Identifier----\n%d \n-----(x ; y)------\n(%d ; %d)\n-----Cardinal-----\n%d\n------Color-------\n%d\n", v->id, v->x, v->y, v->card, v->color);
}

void printArete(Arete *a)
{
    printf("=====================  Arete\n----Identifier----\n%d \n  %d -----------> %d\n", a->id, a->start, a->end);
}

void createCompleteGraph(Graph *g, int n)
{
    if (n <= 0)
        return;
    for (int i = 0; i < n; i++){
        g->vertexs[i].x = cos(i * 2 * 3.1415 / (float)n) * (fmin(WIDTH, HEIGHT)/2) + WIDTH / 2;
        g->vertexs[i].y = sin(i * 2 * 3.1415 / (float)n) * (fmin(WIDTH, HEIGHT)/2) + HEIGHT / 2;
        g->vertexs[i].id = i;// add vertexs and create a circle with their coordinates
        g->nb_vertex = i+1;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            g->aretes[i*n + j].id = i*n + j;
            g->aretes[i*n + j].start = i;
            g->aretes[i*n + j].end = j;
            g->nb_arete = i*n+j+1;
            //addArete(g, i * n + j, i, j); // connect them
        }
}

void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp)
{
    int size = 20;

    // display vertexs
    for (int i = 0; i < g->nb_vertex; i++)
    {
        // color gestion to come
        color(r, 0, 0, 0, 0);
        circle(r, g->vertexs[i].x, g->vertexs[i].y, size, 0);
        toChar(tmp, g->vertexs[i].id);
        // printf("%d\n", g->vertexs[i].id);
        text(r, g->vertexs[i].x - size / 2, g->vertexs[i].y - size / 2, tmp, f, 0, 0, 0);
    }

    // display aretes
    double sx, sy, ex, ey;
    double start_angle, end_angle;
    for (int i = 0; i < g->nb_arete; i++)
    {
        sx = g->vertexs[g->aretes[i].start].x;
        sy = g->vertexs[g->aretes[i].start].y;
        ex = g->vertexs[g->aretes[i].end].x;
        ey = g->vertexs[g->aretes[i].end].y;
        color(r, 0, 0, 0, 0);

        if (ex != sx || ey != sy)
        {
            double theta = atan2(ey - sy, ex - sx);
            double theta2 = atan2(sy - ey, sx - ex);

            line(r, sx + size * cos(theta), sy + size * sin(theta), ex + size * cos(theta2), ey + size * sin(theta2));
            // arrow end
            triangle(r, ex + (size * 2) * cos(theta2) + size * 0.2 * cos(theta2 + 3.1415 / 2), ey + (size * 2) * sin(theta2) + size * 0.2 * sin(theta2 + 3.1415 / 2),
                     ex + (size * 2) * cos(theta2) + size * 0.2 * cos(theta2 - 3.1415 / 2), ey + (size * 2) * sin(theta2) + size * 0.2 * sin(theta2 - 3.1415 / 2),
                     ex + size * cos(theta2), ey + size * sin(theta2), 1); // end of arrow
        }
    }
}
