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
        fprintf(stderr, "you reached the max amount of vertex\n");
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
        // color gestion
        if (g->vertexs[i].color != NO_COLOR)
        {
            color(r, c[g->vertexs[i].color].r, c[g->vertexs[i].color].g, c[g->vertexs[i].color].b, c[g->vertexs[i].color].a);
            circle(r, edge_x + (g->vertexs[i].x) * width / WIDTH, edge_y + (g->vertexs[i].y) * height / HEIGHT, VERTEX_SIZE, 1);
        }
        color(r, 0, 0, 0, 1); // black border
        circle(r, edge_x + (g->vertexs[i].x) * width / WIDTH, edge_y + (g->vertexs[i].y) * height / HEIGHT, VERTEX_SIZE, 0);
        toChar(tmp, g->vertexs[i].id);
        // printf("%d\n", g->vertexs[i].id);
        text(r, edge_x + (g->vertexs[i].x) * width / WIDTH - VERTEX_SIZE * 0.8, edge_y + (g->vertexs[i].y) * height / HEIGHT - VERTEX_SIZE * 0.8, tmp, f, 0, 0, 0);
    }

    // display aretes
    double sx, sy, ex, ey;
    for (int i = 0; i < g->nb_arete; i++)
    {

        sx = edge_x + (g->vertexs[g->aretes[i].start].x) * width / WIDTH;
        sy = edge_y + (g->vertexs[g->aretes[i].start].y) * height / HEIGHT;
        ex = edge_x + (g->vertexs[g->aretes[i].end].x) * width / WIDTH;
        ey = edge_y + (g->vertexs[g->aretes[i].end].y) * height / HEIGHT;
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
    // display red border :
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

void creatCoordinatesSystem(const char *file_coord, const char *file_links, Graph *g)
{
    FILE *fc = fopen(file_coord, "r");
    if (fc == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(fc);
        return;
    }

    // first know the min/max value
    double xmin, xmax, ymin, ymax;
    char buffer[128];
    char bin[16];
    double x, y;
    int a, b;

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MIN") != 0)
        fprintf(stderr, "no min value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmin, bin, &ymin);

    fgets(buffer, sizeof(buffer), fc);
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

    // here, xmin ymin xmax ymax are known
    int i = 0;
    while (!feof(fc))
    { // read while end of file isn't reached
        fgets(buffer, sizeof(buffer), fc);

        if (!links_reading)
        {
            sscanf(buffer, "%6s", bin);
            sscanf(buffer, "%lf%3s%lf", &x, bin, &y);
            if (HEIGHT <= WIDTH)
            {
                g->vertexs[i].y = (HEIGHT) - (x - xmin) * HEIGHT / (xmax - xmin);
                g->vertexs[i].x = (y - ymin) * HEIGHT / (xmax - xmin);
            }
            else
            {
                g->vertexs[i].y = (WIDTH) - (x - xmin) * WIDTH / (ymax - ymin);
                g->vertexs[i].x = (y - ymin) * WIDTH / (ymax - ymin);
            }
            g->vertexs[i].id = i;
            g->vertexs[i].color = NO_COLOR;
            g->nb_vertex = i + 1;
            i++;
            if (i >= g->v)
            {
                fprintf(stderr, "graph vertex array is too small\n");
                fclose(fc);
                return;
            }
        }
    }
    fclose(fc);

    // read links....

    FILE *fl = fopen(file_links, "r");
    if (fl == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(fl);
        return;
    }
    while (!feof(fl))
    {
        fgets(buffer, sizeof(buffer), fl);

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
                fclose(fl);
                return;
            }
        }
        else
        { // bin[0] == 'd' ; create a double arete
            sscanf(buffer, "%c%d%c%d", &bin[0], &a, &bin[0], &b);
            g->aretes[i].start = a - 3;
            g->aretes[i].end = b - 3; //-3 to count after max and min
            g->aretes[i].id = i;
            g->aretes[i].weight = NO_WEIGHT;
            g->nb_arete = i + 1;
            i++;
            if (i >= g->a)
            {
                fprintf(stderr, "graph arete array is too small\n");
                fclose(fl);
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
                fclose(fl);
                return;
            }
        }
    }
    fclose(fl);
    // printf("%f\n", ymax-ymin);
}

int linkByClick(const char *file_links, Graph *g, double x1, double y1, double x2, double y2, int doublelink, int edge_x, int edge_y, int width, int height)
{
    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
        return 0;
    int s = -1, e = -1;

    for (int i = 0; i < g->nb_vertex && s == -1; i++)
        if (dist(x1, y1, edge_x + (g->vertexs[i].x) * width / WIDTH, edge_y + (g->vertexs[i].y) * height / HEIGHT) < VERTEX_SIZE)
            s = i;

    for (int i = 0; i < g->nb_vertex && s != -1 && e == -1; i++)
        if (dist(x2, y2, edge_x + (g->vertexs[i].x) * width / WIDTH, edge_y + (g->vertexs[i].y) * height / HEIGHT) < VERTEX_SIZE)
            e = i;

    if (s == -1 || e == -1) // no clicks in thee vertexs
        return 0;

    FILE *f = fopen(file_links, "a");
    if (f == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(f);

        return 0;
    }

    printf("adding arete between %d and %d \n", s, e);
    addArete(g, g->nb_arete, s, e, NO_WEIGHT);
    /*if (g->nb_arete >= g->a)
    {
        fprintf(stderr, "arrete array isn't big enough\n");
        return;
    }
    g->aretes[g->nb_arete].start = s;
    g->aretes[g->nb_arete].end = e;
    g->aretes[g->nb_arete].id = g->nb_arete;
    g->nb_arete += 1;*/
    if (doublelink)
    {
        addArete(g, g->nb_arete, e, s, NO_WEIGHT);

        /*if (g->nb_arete >= g->a)
        {
            fprintf(stderr, "arrete array isn't big enough\n");
            return;
        }
        g->aretes[g->nb_arete].start = e;
        g->aretes[g->nb_arete].end = s;
        g->aretes[g->nb_arete].id = g->nb_arete;
        g->nb_arete += 1;*/
        fprintf(f, "%s", "."); // double way arrow signature
    }
    fprintf(f, "%d %d\n", s + 3, e + 3);
    fclose(f);
    return 1; // sucess
}

int createVertex(const char* file_coordinates, Graph*g, double cx, double cy, double _w, double _h, double x, double y){
    //first let's know the min and max value :
    FILE *fc = fopen(file_coordinates, "r");
    if (fc == NULL)
    {
        fprintf(stderr, "failed to open text file\n");
        fclose(fc);
        return 0;
    }

    // first know the min/max value
    double xmin, xmax, ymin, ymax;
    char buffer[128];
    char bin[16];
    double _x, _y;

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MIN") != 0)
        fprintf(stderr, "no min value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmin, bin, &ymin);

    fgets(buffer, sizeof(buffer), fc);
    sscanf(buffer, "%3s", bin);
    if (strcmp(bin, "MAX") != 0)
        fprintf(stderr, "no max value in the coordinate file\n");
    sscanf(buffer, "%4s%lf%3s%lf", bin, &xmax, bin, &ymax);

    _x = fmin(xmin, xmax);
    _y = fmax(xmin, xmax);
    xmin = _x;
    xmax = _y;
    _x = fmin(ymin, ymax);
    _y = fmax(ymin, ymax);
    ymin = _x;
    ymax = _y;
    printf("(%f, %f)\t(%f %f)\n",xmin, ymin, xmax, ymax);
    fclose(fc);


    //add vertex on the actual graph :

    //transform x and y into coordinates
    double coordx, coordy, aa, bb;

    aa= (x - cx)/(double)_w;
    bb = (y - cy)/(double)_h;
    addVertex(g, g->nb_vertex - 1, aa*WIDTH, bb*HEIGHT, NO_COLOR);

    coordx = xmin + aa*(xmax - xmin) * (_h/WIDTH);
    coordy = ymin + bb*(ymax - ymin) * (_w/HEIGHT);

    //write the coordinates in the file :
    FILE*fr = fopen(file_coordinates, "a");
    fprintf(fr, "%lf , %lf\n", coordx, coordy);
    /*if (HEIGHT <= WIDTH)
            {
                coordy = (HEIGHT) - (x - xmin) * HEIGHT / (xmax - xmin);
                coordx = (y - ymin) * HEIGHT / (xmax - xmin);
            }
        else
            {
                coordy = (WIDTH) - (x - xmin) * WIDTH / (ymax - ymin);
                coordx = (y - ymin) * WIDTH / (ymax - ymin);
            }*/


    fclose(fr);
    return 1;
}

void minimal_weight_tree(Graph * g, Graph*tree){//using prim algorithm
    addVertex(tree, g->vertexs[0].id, g->vertexs[0].x, g->vertexs[0].y, g->vertexs[0].color);//choose a random vertex
    int arrete_id = 0;
    while(tree->nb_vertex < g->nb_vertex){
        //choose a link : 
        for(int i = 0; i < g->nb_arete; i++){
            //todo : find the minimal weight
            if(isPresentInArray(g->aretes[i].start, tree->aretes, tree->nb_arete) && !isPresentInArray(g->aretes[i].end, tree->aretes, tree->nb_arete)){//arret start must be in tree but not end
                addVertex(tree, g->aretes[i].end, g->vertexs[g->aretes[i].end].x, g->vertexs[g->aretes[i].end].y, g->vertexs[g->aretes[i].end].color);
                addArete(tree, arrete_id++, g->aretes[i].start, g->aretes[i].end, g->aretes[i].weight);
                i = g->nb_arete + 1;//stop the loop
            }else if(i == g->nb_arete)
                fprintf(stderr, "graph isn't connex\n");
        }
    }
}

int isPresentInArray(int value, Arete*array, int size){
    for(int i = 0; i < size ; i++)
        if(array[i].id == value)
            return 1;
    return 0;
}
