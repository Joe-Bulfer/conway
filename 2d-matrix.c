//after 8-possible-states.c, I realized this had to be a 2d matrix/grid and render both alive and unalive cells
/*
gcc checkForNeighbors.c -o checkForNeighbors -I ../projects/raylib-quickstart/build/external/raylib-master/src/ -L ../projects/raylib-quickstart/bin/Debug/ -lraylib -lm -lpthread -ldl -lX11 && ./checkForNeighbors
*/

#include <raylib.h>
#include <raymath.h>
#include <math.h>//for floorf
#include <string.h>//for memset
#define BLOCK_SIZE 20
#define CURSOR_COLOR (Color){255, 5, 5, 128}

typedef struct Cell{//not used yet
    int alive;
    int neighbors;
}Cell;
struct Block{//not used yet
    int alive;
    int neighbors;
    Vector2 pos;
};

//remove this and do a 2d matrix instead
bool CheckForNeighbors(Rectangle blockA, Rectangle blockB){
    if (blockA.y==blockB.y && blockA.x==blockB.x+BLOCK_SIZE){//isLeft
        return true;
    }
    if (blockA.y==blockB.y && blockA.x+BLOCK_SIZE==blockB.x){//isRight
        return true;
    }
    if (blockA.x==blockB.x && blockA.y==blockB.y+BLOCK_SIZE){//isTop
        return true;
    }
    if (blockA.x==blockB.x && blockA.y+BLOCK_SIZE==blockB.y){//isBottom
        return true;
    }
    if (blockA.x==blockB.x-BLOCK_SIZE && blockA.y==blockB.y-BLOCK_SIZE){//isBottomRight
        return true;
    }
    if (blockA.x==blockB.x-BLOCK_SIZE && blockA.y==blockB.y+BLOCK_SIZE){//isTopRight
        return true;
    }
    if (blockA.x==blockB.x+BLOCK_SIZE && blockA.y==blockB.y+BLOCK_SIZE){//isTopLeft
        return true;
    }
    if (blockA.x==blockB.x+BLOCK_SIZE && blockA.y==blockB.y-BLOCK_SIZE){//isBottomLeft
        return true;
    }
    return false;
}

int main(){
    #define COLS 50
    #define ROWS 50
    int screenWidth, screenHeight;
    screenWidth = 800; screenHeight = 500;
    //int cols = screenWidth/10;int rows = screenHeight/10;//rows x cols = total cells
    Cell world[ROWS][COLS] = {0};

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
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    world[i][j].alive = 0;
                }
            }
        }
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                //DrawRectangle(i*BLOCK_SIZE,j*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,GRAY);
                if(world[i][j].alive){
                    DrawRectangle(i*BLOCK_SIZE,j*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,RED);
                }else {
                    DrawRectangle(i*BLOCK_SIZE,j*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,RAYWHITE);
                }
            }
        }
        //draw blocks
        DrawRectangle(mousePos.x,mousePos.y,BLOCK_SIZE,BLOCK_SIZE,CURSOR_COLOR);

        //debug/print
        DrawText(TextFormat("worldMousePos: %d %d", worldMousePosX,worldMousePosY), 50, 200, 20, RED);
        DrawText(TextFormat("mousePos: %.2f %.2f", mousePos.x,mousePos.y), 50, 220, 20, RED);
        DrawText("Place two blocks, then (r)eset. Test again for 8 possible states.\nOnly test blocks[0] against block[1].", 20, 80, 20, RED);
        DrawText(TextFormat("neighbors: %d", neighbors), 50, 260, 20, RED);
        //DrawText(TextFormat("rows cols: %d %d", rows, cols), 50, 240, 20, RED);
        EndDrawing();
    }
    CloseWindow();
}
