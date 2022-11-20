#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "SDL_Basics.h"

#define NO_COLOR -1
#define NO_WEIGHT -1
#define NB_COLOR 15

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
    double weight;
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
void addVertex(Graph *, int id, int x, int y, int color);
void addArete(Graph *g, int id, int start_id, int end_id, double weight);
void printGraph(Graph *g);
void printVertex(Vertex *v);
void printArete(Arete *a);
void createCompleteGraph(Graph *g, int n);
void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp, SDL_Color *c);

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

void addVertex(Graph *g, int id, int x, int y, int color)
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
    if (color == -2)
        v.color = rand() % NB_COLOR;
    else
        v.color = color;
    (g->vertexs)[g->nb_vertex] = v;
    (g->nb_vertex)++;
}

void addArete(Graph *g, int id, int start_id, int end_id, double weight)
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
    a.weight = weight;
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
    printf("=====================  Arete\nIdentifier--weight\n%d           %f\n  %d -----------> %d\n", a->id, a->weight, a->start, a->end);
}

void createCompleteGraph(Graph *g, int n)
{
    if (n <= 0)
        return;
    for (int i = 0; i < n; i++)
    {
        g->vertexs[i].x = cos(i * 2 * 3.1415 / (float)n) * (fmin(WIDTH, HEIGHT) / 2.1) + WIDTH / 2;
        g->vertexs[i].y = sin(i * 2 * 3.1415 / (float)n) * (fmin(WIDTH, HEIGHT) / 2.1) + HEIGHT / 2;
        g->vertexs[i].id = i; // add vertexs and create a circle with their coordinates
        g->vertexs[i].color = i % NB_COLOR;
        g->vertexs[i].card = n; // is the card increased if a connexion exists or if an arrow is on the vertex ???
        g->nb_vertex = i + 1;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            g->aretes[i * n + j].id = i * n + j;
            g->aretes[i * n + j].start = i;
            g->aretes[i * n + j].end = j;
            g->nb_arete = i * n + j + 1;
            // addArete(g, i * n + j, i, j); // connect them
        }
}

void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp, SDL_Color *c)
{
    int size = 20;

    // display vertexs
    for (int i = 0; i < g->nb_vertex; i++)
    {
        // color gestion to come
        if (g->vertexs[i].color != NO_COLOR)
        {
            color(r, c[g->vertexs[i].color].r, c[g->vertexs[i].color].g, c[g->vertexs[i].color].b, c[g->vertexs[i].color].a);
            circle(r, g->vertexs[i].x, g->vertexs[i].y, size, 1);
        }
        color(r, 0, 0, 0, 1); // black border
        circle(r, g->vertexs[i].x, g->vertexs[i].y, size, 0);
        toChar(tmp, g->vertexs[i].id);
        // printf("%d\n", g->vertexs[i].id);
        text(r, g->vertexs[i].x - size / 2, g->vertexs[i].y - size / 2, tmp, f, 0, 0, 0);
    }

    // display aretes
    double sx, sy, ex, ey;
    for (int i = 0; i < g->nb_arete; i++)
    {
        
            sx = g->vertexs[g->aretes[i].start].x;
            sy = g->vertexs[g->aretes[i].start].y;
            ex = g->vertexs[g->aretes[i].end].x;
            ey = g->vertexs[g->aretes[i].end].y;
            color(r, 0, 0, 0, 0);

            double theta = atan2(ey - sy, ex - sx);
            double theta2 = atan2(sy - ey, sx - ex);
        if (g->aretes[i].start != g->aretes[i].end)//exception 
        {
            line(r, sx + size * cos(theta), sy + size * sin(theta), ex + size * cos(theta2), ey + size * sin(theta2));
            // arrow end
            triangle(r, ex + (size * 2) * cos(theta2) + size * 0.2 * cos(theta2 + 3.1415 / 2), ey + (size * 2) * sin(theta2) + size * 0.2 * sin(theta2 + 3.1415 / 2),
                     ex + (size * 2) * cos(theta2) + size * 0.2 * cos(theta2 - 3.1415 / 2), ey + (size * 2) * sin(theta2) + size * 0.2 * sin(theta2 - 3.1415 / 2),
                     ex + size * cos(theta2), ey + size * sin(theta2), 1); // end of arrow
        }else{//reflexive arete exception
            //let's make an artificial circle :
            double precision = 0.5;
            for(int a = sx - size ; a < sx + size*2 ; a++){
                for(int b = sy ; b < sy + size*2 ; b++){
                    if(dist(sx, sy, a, b) > size && fabs(dist(sx, sy + size*1.2, a, b)-size*0.7) < precision)
                        point(r, a, b);
                }
            }
            triangle(r, sx+size*0.6, sy + size*0.8,//end of arrow
                        sx+size*0.9, sy + size*1.1,//right
                        sx+size*0.5, sy + size*1.3,1);//left
        }
    }
}
