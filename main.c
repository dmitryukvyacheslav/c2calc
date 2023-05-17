#include <ncurses.h>
#include "stack.h"
// макс. вершин
#define VTX_MAX 16

void check_cc(int vertices, int adj_m[VTX_MAX][VTX_MAX]){
    int cnt = 0;
    int visited[VTX_MAX] = {0};
    

    for (int i = 0; i < vertices; i++) {
        if (visited[i]) continue;
        struct stack* adj_to_sel = stack_create(VTX_MAX);
        stack_push(adj_to_sel, i);
        
        printw("(");
        while (stack_size(adj_to_sel)) {
            int vtx = stack_pop(adj_to_sel);
            if (!visited[vtx]) {
                printw("%d,", vtx+1);
                visited[vtx]++;
                for (int j = 0; j < vertices; j++) {
                    if (visited[j]) continue;
                    if (adj_m[i][j]) stack_push(adj_to_sel, j);
                }
            }
        }
        stack_free(adj_to_sel);
        int y, x;
        getyx(stdscr, y, x);
        move(y, x-1);
        printw(") ");
        cnt += 1;
    }
}

void render_table(int vertices, int ypos, int xpos, int adj_m[VTX_MAX][VTX_MAX]){
    clear();
    for (int i = 0; i < vertices; i++) 
        adj_m[i][i] = 1; // A vertex is always adjacent to itself
    printw("c2calc (c) Dmitryuk Vyacheslav IS-241\n");
    printw("[i], [o] - remove/add vertices\n");
    printw("[s] - set/unset adjacency\n");
    printw("arrow keys - navigate the adjacency table\n\n");
    attron(COLOR_PAIR(1));
    printw("Vertices: %02d/%02d\n", vertices, VTX_MAX);
    attroff(COLOR_PAIR(1));
    printw("    ");
    for(int i = 0; i < vertices; i++)
        printw("v%02d ", i+1);
    printw("\n");
    for(int y = 0; y < vertices; y++){
        printw("v%02d ", y+1);
        for(int x = 0; x < vertices; x++)
            printw(" %d  ", adj_m[y][x]);
        printw("\n");   
    }
    attron(COLOR_PAIR(1));
    printw("\nConnected components: \n");
    check_cc(vertices, adj_m);
    attroff(COLOR_PAIR(1));
    move(7+ypos, 5+4*xpos);
    refresh();
}

int main(void) {
    int adj_m[VTX_MAX][VTX_MAX] = {0};
    int vertices = 2;
    // ui
    int xpos = 0, ypos = 0;

    
    
    initscr();
    start_color();
    noecho();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    keypad(stdscr, TRUE);
    // render the table
    render_table(vertices, ypos, xpos, adj_m);
    int ch;
    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_DOWN:
                ypos++;
                break;
            case KEY_UP:
                ypos--;
                break;
            case KEY_LEFT:
                xpos--;
                break;
            case KEY_RIGHT:
                xpos++;
                break;
            case 'o':
                vertices++;
                break;
            case 'i':
                vertices--;
                break; 
            case '1':
                adj_m[ypos][xpos] = 1;
                adj_m[xpos][ypos] = 1;
                break;   
            case '0':
                adj_m[ypos][xpos] = 0;
                adj_m[xpos][ypos] = 0;
                break;
            case 's':
                adj_m[ypos][xpos] = !adj_m[ypos][xpos];
                if (xpos != ypos) adj_m[xpos][ypos] = !adj_m[xpos][ypos];
                break;                            
        }
        if (vertices < 1) vertices = 1;
        if (vertices > VTX_MAX) vertices = VTX_MAX;
        if (xpos < 0) xpos = 0;
        if (xpos >= vertices) xpos = vertices-1;
        if (ypos < 0) ypos = 0;
        if (ypos >= vertices) ypos = vertices-1;
        render_table(vertices, ypos, xpos, adj_m);
    }
    endwin();
    return 0;
}

