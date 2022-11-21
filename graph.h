#define NO_COLOR -1
#define NO_WEIGHT -1
#define NB_COLOR 15
#define VERTEX_SIZE 10

#include "SDL_Basics.h"

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
void circlePoints(Graph*g);
SDL_Color* initialiseColors();
void weightAsDistance(Graph*g);
void creatCoordinatesSystem(const char*file_name, Graph *g,  int width, int height);
