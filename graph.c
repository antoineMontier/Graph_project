#include "graph.h"



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

void circlePoints(Graph*g){
    
    for(int i = 0 ; i < g->nb_vertex ; i++){
        g->vertexs[i].x = cos(i * 2 * 3.1415 /g->nb_vertex) * (fmin(WIDTH, HEIGHT) / 2.1) + WIDTH / 2;
        g->vertexs[i].y = sin(i * 2 * 3.1415 /g->nb_vertex) * (fmin(WIDTH, HEIGHT) / 2.1) + HEIGHT / 2;
    }
}