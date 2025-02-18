//after 8-possible-states.c, I realized this had to be a 2d matrix/grid and render both alive and unalive cells. first place some cells, then press n key for neighbor count of whatever cell your mouse is on.
/*
gcc 2d-matrix.c -o 2d-matrix -I ../projects/raylib-quickstart/build/external/raylib-master/src/ -L ../projects/raylib-quickstart/bin/Debug/ -lraylib -lm -lpthread -ldl -lX11 && ./2d-matrix
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
int ReturnNeighborCount(Cell cell, Cell world[COLS][ROWS]){
    int neighbors = 0;
    if (world[cell.x-1][cell.y].alive) neighbors++;//left
    if (world[cell.x+1][cell.y].alive) neighbors++;//right
    if (world[cell.x][cell.y-1].alive) neighbors++;//up
    if (world[cell.x][cell.y+1].alive) neighbors++;//down
    return neighbors;
}

int main(){
    int screenWidth, screenHeight;
    screenWidth = 800; screenHeight = 500;
    Cell world[ROWS][COLS] = {0};
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            world[i][j].x = i;
            world[i][j].y = j;
        }
    }

    InitWindow(screenWidth,screenHeight,"BoilerPlate");

    Vector2 mousePos;
    int worldMousePosX;
    int worldMousePosY;
    int neighbors = 0;
    int areaFree;

    SetTargetFPS(30);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        mousePos = GetMousePosition();
        worldMousePosX = floorf(mousePos.x / BLOCK_SIZE);
        worldMousePosY = floorf(mousePos.y / BLOCK_SIZE);
        //just for mouse hovering
        mousePos.x = floorf(mousePos.x / BLOCK_SIZE) * BLOCK_SIZE;
        mousePos.y = floorf(mousePos.y / BLOCK_SIZE) * BLOCK_SIZE;

        //place block
        if (IsMouseButtonDown(0)){
            world[worldMousePosX][worldMousePosY].alive = 1;
        }
        if (IsKeyPressed(KEY_R)){//reset
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    world[i][j].alive = 0;
                }
            }
        }
        if (IsKeyPressed(KEY_N)){//count neighbors
            neighbors = ReturnNeighborCount(world[worldMousePosX][worldMousePosY],world);
        }
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
        EndDrawing();
    }
    CloseWindow();
}
