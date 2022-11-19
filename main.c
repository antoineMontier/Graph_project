#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NO_COLOR -1


typedef struct{
    int id;//position in the array
    int x;
    int y;
    int card;
    int color;
}Vertex;

typedef struct{
    int id;
    int start;
    int end;
}Arete;

typedef struct{
    int nb_vertex;
    int nb_arete;
    int v, a;//max size
    Vertex * vertexs;//size v
    Arete * aretes;//size a
}Graph;

void createGraph(Graph*, int v, int a);
void destructGraph(Graph*);
void addVertex(Graph*, int id, int x, int y);
void addArete(Graph*, int id, int start, int end);
void printGraph(Graph *g);
void printVertex(Vertex *v);
void printArete(Arete *a);
void createCompleteGraph(Graph*g, int n);

int main(){/*gcc -c -Wall -Wextra main.c && gcc main.o -lm -o main && ./main*/

    Graph k5;
    createGraph(&k5, 10, 100);

    createCompleteGraph(&k5, 5);



    printGraph(&k5);
    destructGraph(&k5);

    return 0;
}

void createGraph(Graph*g, int v, int a){
    g->v = v;
    g->a = a;
    g->vertexs = malloc(v*sizeof(Vertex));
    g->aretes  = malloc(a*sizeof(Arete));
    g->nb_arete = 0;
    g->nb_vertex = 0;
}

void destructGraph(Graph*g){
    g->v = 0;
    g->a = 0;
    free(g->vertexs);
    free(g->aretes);
    g->nb_arete = 0;
    g->nb_vertex = 0;  
}

void addVertex(Graph*g, int id, int x, int y){
    if(g->v <= g->nb_vertex){
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

void addArete(Graph*g, int id, int start_id, int end_id){
    if(g->a <= g->nb_arete){
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

void printGraph(Graph *g){
    printf("graph : %d vertex, %d arete\n", g->nb_vertex, g->nb_arete);
    for(int i = 0 ; i < g->nb_vertex ; i++)
        printVertex((g->vertexs) + i);
    for(int i = 0 ; i < g->nb_arete; i++)
        printArete((g->aretes) + i);;
    printf("\n");
}

void printVertex(Vertex*v){
    printf("===================== Vertex\n----Identifier----\n%d \n-----(x ; y)------\n(%d ; %d)\n-----Cardinal-----\n%d\n------Color-------\n%d\n", v->id, v->x, v->y, v->card, v->color);
}

void printArete(Arete *a){
    printf("=====================  Arete\n----Identifier----\n%d \n  %d -----------> %d\n", a->id, a->start, a->end);
}

void createCompleteGraph(Graph*g, int n){
    if(n <= 0)
        return;
    for(int i = 0 ; i < n ; i++)
        addVertex(g, i, i*20, 20);//add enough vertexs

    for(int i = 0 ; i < n ; i++)
        for(int j = 0 ; j < n ; j++){
            addArete(g, i*n+j, i, j);//connect them
        }
}

















































