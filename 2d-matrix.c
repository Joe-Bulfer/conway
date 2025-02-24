//after 8-possible-states.c, I realized this had to be a 2d matrix/grid and render both alive and unalive cells. first place some cells, then press n key for neighbor count of whatever cell your mouse is on.
/*
gcc 2d-matrix.c -o bin/2d-matrix -I ../raylib-quickstart/build/external/raylib-master/src/ -L ../raylib-quickstart/bin/Debug/ -lraylib -lm -lpthread -ldl -lX11 && ./bin/2d-matrix
*/

#include <raylib.h>
#include <raymath.h>
#include <math.h>//for floorf
#include <string.h>//for memset
#define BLOCK_SIZE 20
#define CURSOR_COLOR (Color){255, 5, 5, 128}
#define COLS 50
#define ROWS 50

typedef struct Cell{//for world
    int alive;
    int neighbors;
    int x, y;
}Cell;

//return number of neighbors for given cell (where the mouse in hovering)
//todo: add diagnal remaining 4 
//basically works the same as below if called on each cell of world, though more readable, it is allegedly less efficient.
//NOT USED, WANT TO KEEP JUST FOR READABILITY THOUGH
int ReturnNeighborCount(Cell cell, Cell world[COLS][ROWS]){
    int neighbors = 0;
    if (world[cell.x-1][cell.y].alive) neighbors++;//left
    if (world[cell.x+1][cell.y].alive) neighbors++;//right
    if (world[cell.x][cell.y-1].alive) neighbors++;//up
    if (world[cell.x][cell.y+1].alive) neighbors++;//down
    if (world[cell.x-1][cell.y-1].alive) neighbors++;//top left
    if (world[cell.x+1][cell.y-1].alive) neighbors++;//top right
    if (world[cell.x-1][cell.y+1].alive) neighbors++;//bottom left
    if (world[cell.x+1][cell.y+1].alive) neighbors++;//bottom right
    return neighbors;
}

//deepseek - avoid redundant calculations
void CountAllNeighbors(Cell world[COLS][ROWS]) {
    int temp[COLS][ROWS] = {0}; // Temporary grid to store neighbor counts

    // Compute neighbor counts
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (world[i][j].alive) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue; // Skip the cell itself
                        int nx = i + dx;
                        int ny = j + dy;
                        if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS) {
                            temp[nx][ny]++;
                        }
                    }
                }
            }
        }
    }

    // Update the world grid with neighbor counts
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            world[i][j].neighbors = temp[i][j];
        }
    }
}

//wrote this myself, it works. Could probably be made more efficient like above
void UpdateWorld(Cell world[COLS][ROWS]) {
    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < ROWS; j++) {
            if (!world[i][j].alive) {
                if(world[i][j].neighbors==3){//reproduction
                    world[i][j].alive = 1;
                }
            }
            if (world[i][j].alive) {
                if(world[i][j].neighbors<2){//underpopulation
                    world[i][j].alive = 0;
                }
                if(world[i][j].neighbors==2||world[i][j].neighbors==3){//survive
                    continue;
                }
                if(world[i][j].neighbors>3){//overpopulation
                    world[i][j].alive = 0;
                }
            }
        }
    }
}

int main(){
    int screenWidth, screenHeight;
    screenWidth = 800; screenHeight = 500;
    Cell world[ROWS][COLS] = {0};
    //initialize world, assign x and y to each cell
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            world[i][j].x = i;
            world[i][j].y = j;
        }
    }

    InitWindow(screenWidth,screenHeight,"THE ENTIRE UNIVERSE");

    Vector2 mousePos;
    int worldMousePosX;
    int worldMousePosY;
    Cell currentCellHovering;//wherever the mouse it at
    int neighbors = 0;
    int areaFree;

    SetTargetFPS(30);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        mousePos = GetMousePosition();
        worldMousePosX = floorf(mousePos.x / BLOCK_SIZE);
        worldMousePosY = floorf(mousePos.y / BLOCK_SIZE);
        currentCellHovering = world[worldMousePosX][worldMousePosY];
        //just for drawing cursor (transparent square/cell) over mouse
        mousePos.x = floorf(mousePos.x / BLOCK_SIZE) * BLOCK_SIZE;
        mousePos.y = floorf(mousePos.y / BLOCK_SIZE) * BLOCK_SIZE;

        //place block
        if (IsMouseButtonDown(0)){
            world[worldMousePosX][worldMousePosY].alive = 1;
            //doesn't work cause it's a copy, have to access directly like above
            //currentCellHovering.alive = 1;
        }
        if (IsKeyPressed(KEY_R)){//reset
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    world[i][j].alive = 0;
                }
            }
        }
        if (IsKeyPressed(KEY_N)){//count neighbors (works)
            neighbors = ReturnNeighborCount(world[worldMousePosX][worldMousePosY],world);
        }
        if (IsKeyPressed(KEY_A)) { // Count all neighbors
            CountAllNeighbors(world);
        }
        if (IsKeyPressed(KEY_I)) { // Count all neighbors
            UpdateWorld(world);
        }
        /* if (IsKeyPressed(KEY_A)){//count all neighbors (doesn't work)
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    world[i][j].neighbors = ReturnNeighborCount(world[i][j],world);
                }
            }
        } */
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if(world[i][j].alive){
                    DrawRectangle(i*BLOCK_SIZE,j*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,RED);
                }else {
                    DrawRectangle(i*BLOCK_SIZE,j*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,RAYWHITE);
                }
            }
        }
        //draw transparent cursor
        DrawRectangle(mousePos.x,mousePos.y,BLOCK_SIZE,BLOCK_SIZE,CURSOR_COLOR);

        //debug/print
        DrawText(TextFormat("worldMousePos: %d %d", worldMousePosX,worldMousePosY), 50, 200, 20, RED);
        DrawText(TextFormat("mousePos: %.2f %.2f", mousePos.x,mousePos.y), 50, 220, 20, RED);
        DrawText("Place two blocks, then (r)eset. Test again for 8 possible states.\nOnly test blocks[0] against block[1].", 20, 80, 20, RED);
        DrawText(TextFormat("neighbors: %d", neighbors), 50, 260, 20, RED);
        //DrawText(TextFormat("rows cols: %d %d", rows, cols), 50, 240, 20, RED);
        //below makes sure worldMousePosX and Y line up with given cells x and y
        DrawText(TextFormat("x y : %d %d",
                            world[worldMousePosX][worldMousePosY].x,
                            world[worldMousePosX][worldMousePosY].y
        ), 50, 280, 20, RED);
        DrawText(TextFormat("currentCellHovering.neighbors: %d", currentCellHovering.neighbors), 50, 300, 20, RED);

        EndDrawing();
    }
    CloseWindow();
}
