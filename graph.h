#define NO_COLOR -1
#define NO_WEIGHT -1
#define NB_COLOR 15
#define VERTEX_SIZE 20

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
void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp, SDL_Color *c, int edge_x, int edge_y, int width, int height);  
void circlePoints(Graph*g);
SDL_Color* initialiseColors();
void weightAsDistance(Graph*g);
void creatCoordinatesSystem(const char *file_coord, const char *file_links, Graph *g);
int linkByClick(const char *file_name, Graph *g, double x1, double y1, double x2, double y2, int doublelink, int edge_x, int edge_y, int width, int height);
int createVertex(const char* file_coordinates, Graph*g, double cx, double cy, double _w, double _h, double x, double y);
Graph* minimal_weight_tree(Graph* g);
int isPresentInArray(int value, int*array, int size);