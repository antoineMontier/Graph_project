#include "graph.h"
#include <string.h>

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

void displayGraph(SDL_Renderer *r, TTF_Font *f, Graph *g, char *tmp, SDL_Color *c, int edge_x, int edge_y, int width, int height)
{

    // display vertexs
    for (int i = 0; i < g->nb_vertex; i++)
    {
        // color gestion to come
        if (g->vertexs[i].color != NO_COLOR)
        {
            color(r, c[g->vertexs[i].color].r, c[g->vertexs[i].color].g, c[g->vertexs[i].color].b, c[g->vertexs[i].color].a);
            circle(r, edge_x + (g->vertexs[i].x)*width/WIDTH, edge_y + (g->vertexs[i].y)*height/HEIGHT, VERTEX_SIZE, 1);
        }
        color(r, 0, 0, 0, 1); // black border
        circle(r, edge_x + (g->vertexs[i].x)*width/WIDTH, edge_y + (g->vertexs[i].y)*height/HEIGHT, VERTEX_SIZE, 0);
        toChar(tmp, g->vertexs[i].id + 3);
        // printf("%d\n", g->vertexs[i].id);
        text(r, edge_x + (g->vertexs[i].x)*width/WIDTH - VERTEX_SIZE * 0.8, edge_y + (g->vertexs[i].y)*height/HEIGHT - VERTEX_SIZE * 0.8, tmp, f, 0, 0, 0);
    }

    // display aretes
    double sx, sy, ex, ey;
    for (int i = 0; i < g->nb_arete; i++)
    {

        sx =edge_x + (g->vertexs[g->aretes[i].start].x)*width/WIDTH;
        sy = edge_y + (g->vertexs[g->aretes[i].start].y)*height/HEIGHT;
        ex =edge_x + (g->vertexs[g->aretes[i].end].x)*width/WIDTH;
        ey = edge_y + (g->vertexs[g->aretes[i].end].y)*height/HEIGHT;
        color(r, 0, 0, 0, 0);

        double theta = atan2(ey - sy, ex - sx);
        double theta2 = atan2(sy - ey, sx - ex);
        if (g->aretes[i].start != g->aretes[i].end) // exception
        {
            line(r, sx + VERTEX_SIZE * cos(theta), sy + VERTEX_SIZE * sin(theta), ex + VERTEX_SIZE * cos(theta2), ey + VERTEX_SIZE * sin(theta2));
            // arrow end
            triangle(r, ex + (VERTEX_SIZE * 2) * cos(theta2) + VERTEX_SIZE * 0.2 * cos(theta2 + 3.1415 / 2), ey + (VERTEX_SIZE * 2) * sin(theta2) + VERTEX_SIZE * 0.2 * sin(theta2 + 3.1415 / 2),
                     ex + (VERTEX_SIZE * 2) * cos(theta2) + VERTEX_SIZE * 0.2 * cos(theta2 - 3.1415 / 2), ey + (VERTEX_SIZE * 2) * sin(theta2) + VERTEX_SIZE * 0.2 * sin(theta2 - 3.1415 / 2),
                     ex + VERTEX_SIZE * cos(theta2), ey + VERTEX_SIZE * sin(theta2), 1); // end of arrow
        }
        else
        { // reflexive arete exception
            // let's make an artificial circle :
            double precision = 0.5;
            for (int a = sx - VERTEX_SIZE; a < sx + VERTEX_SIZE * 2; a++)
            {
                for (int b = sy; b < sy + VERTEX_SIZE * 2; b++)
                {
                    if (dist(sx, sy, a, b) > VERTEX_SIZE && fabs(dist(sx, sy + VERTEX_SIZE * 1.2, a, b) - VERTEX_SIZE * 0.7) < precision)
                        point(r, a, b);
                }
            }
            triangle(r, sx + VERTEX_SIZE * 0.6, sy + VERTEX_SIZE * 0.8,  // end of arrow
                     sx + VERTEX_SIZE * 0.9, sy + VERTEX_SIZE * 1.1,     // right
                     sx + VERTEX_SIZE * 0.5, sy + VERTEX_SIZE * 1.3, 1); // left
        }
    }
    //display red border :
    color(r, 255, 0, 0, 1);
    rect(r, edge_x, edge_y, width, height, 0); 
}

void circlePoints(Graph *g)
{

    for (int i = 0; i < g->nb_vertex; i++)
    {
        g->vertexs[i].x = cos(i * 2 * 3.1415 / g->nb_vertex) * (fmin(WIDTH, HEIGHT) / 2.1) + WIDTH / 2;
        g->vertexs[i].y = sin(i * 2 * 3.1415 / g->nb_vertex) * (fmin(WIDTH, HEIGHT) / 2.1) + HEIGHT / 2;
    }
}

SDL_Color *initialiseColors()
{
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

    return palette;
}

void weightAsDistance(Graph *g)
{
    for (int i = 0; i < g->nb_arete; i++)
    {
        g->aretes[i].weight = dist(g->vertexs[g->aretes[i].start].x, g->vertexs[g->aretes[i].start].y, g->vertexs[g->aretes[i].end].x, g->vertexs[g->aretes[i].end].y);
    }
}

void creatCoordinatesSystem(const char*file_name, Graph *g)
{   
    FILE*f = fopen(file_name, "r");
    if(f == NULL){
        fprintf(stderr, "failed to open text file\n");
        fclose(f);

        return;
    }

    // first know the min/max value
    double xmin, xmax, ymin, ymax;
    char buffer[128];
    char bin[16];
    double x, y;
    int a, b;

    fgets(buffer, sizeof(buffer), f);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MIN") != 0)
        fprintf(stderr, "no min value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmin, bin, &ymin);

    fgets(buffer, sizeof(buffer), f);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MAX") != 0)
        fprintf(stderr, "no max value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmax, bin, &ymax);

    x = fmin(xmin, xmax);
    y = fmax(xmin, xmax);
    xmin = x;
    xmax = y;
    x = fmin(ymin, ymax);
    y = fmax(ymin, ymax);
    ymin = x;
    ymax = y;
    int links_reading = 0;

    // here, xmin ymin xmax ymax are initialized !
    int i = 0;
    while (!feof(f))
    { // read while end of file isn't reached
        fgets(buffer, sizeof(buffer), f);

        if (!links_reading)
        {
            sscanf(buffer, "%6s", bin);
            if (strcmp(bin, "LINKS") != 0)
            {
                sscanf(buffer, "%lf%3s%lf", &x, bin, &y);
                g->vertexs[i].y = (HEIGHT) - (x - xmin) * HEIGHT / (xmax - xmin);
                g->vertexs[i].x =  (y - ymin) * WIDTH / (ymax - ymin);
                g->vertexs[i].id = i;
                g->vertexs[i].color = NO_COLOR;
                g->nb_vertex = i + 1;
                i++;
                if (i >= g->v)
                {
                    fprintf(stderr, "graph vertex array is too small\n");
                    fclose(f);
                    return;
                }
            }
            else
            {
                links_reading = 1;
                i = 0;
            }
        }
        else if (links_reading)
        {
            sscanf(buffer, "%c", &bin[0]);
            if (bin[0] != '.') 
            {
                sscanf(buffer, "%d%c%d", &a, &bin[0], &b);
                g->aretes[i].start = a - 3;
                g->aretes[i].end = b - 3; //-3 to count after max and min
                g->aretes[i].id = i;
                g->aretes[i].weight = NO_WEIGHT;
                g->nb_arete = i + 1;
                i++;
                if (i >= g->a)
                {
                    fprintf(stderr, "graph arete array is too small\n");
                    fclose(f);
                    return;
                }
            }else{//bin[0] == 'd' ; create a double arete
                sscanf(buffer, "%c%d%c%d",&bin[0], &a, &bin[0], &b);
                g->aretes[i].start = a - 3;
                g->aretes[i].end = b - 3; //-3 to count after max and min
                g->aretes[i].id = i;
                g->aretes[i].weight = NO_WEIGHT;
                g->nb_arete = i + 1;
                i++;
                if (i >= g->a)
                {
                    fprintf(stderr, "graph arete array is too small\n");
                    fclose(f);
                    return;
                }
                g->aretes[i].start = b - 3;
                g->aretes[i].end = a - 3; //-3 to count after max and min
                g->aretes[i].id = i;
                g->aretes[i].weight = NO_WEIGHT;
                g->nb_arete = i + 1;
                i++;
                if (i >= g->a)
                {
                    fprintf(stderr, "graph arete array is too small\n");
                    fclose(f);
                    return;
                }
            }
        }
    }
    fclose(f);
    // printf("%f\n", ymax-ymin);
}