/*
if (left or right){
    same pos.y
}
if (top or bottom){
    same pos.x
}
if (left){//b is left of a
    same pos.y
    a.pos.x == b.pos.x+width //+-1
}
if (right){//b is right of a
    same pos.y
    a.pos.x+width == b.pos.x //+-1
}
if (top){
    same pos.x
    a.pos.y == b.pos.y+height //+-1
}
//etc.
*/

/*
gcc life.c -o life -I ../projects/raylib-quickstart/build/external/raylib-master/src/ -L ../projects/raylib-quickstart/bin/Debug/ -lraylib -lm -lpthread -ldl -lX11 && ./life
*/
#include <raylib.h>
#include <raymath.h>
#include <math.h>//for floorf
#include <string.h>//for memset
#define BLOCK_SIZE 20
#define CURSOR_COLOR (Color){255, 5, 5, 128}

int main(){
    int screenWidth, screenHeight;
    screenWidth = 800; screenHeight = 500;
    InitWindow(screenWidth,screenHeight,"BoilerPlate");
    Camera2D camera = { 0 };
    /*
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 2.0f;
    */
    Vector2 mousePos;
    Rectangle blocks[99];
    int blockCount = 0;
    int areaFree;

    int isLeft = 0;
    int isRight = 0;
    int isTop = 0;
    int isBottom = 0;
    int isBottomRight = 0;
    int isTopRight = 0;
    int isTopLeft = 0;
    int isBottomLeft = 0;

    SetTargetFPS(30);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        mousePos = GetMousePosition();
        mousePos.x = floorf(mousePos.x / BLOCK_SIZE) * BLOCK_SIZE;
        mousePos.y = floorf(mousePos.y / BLOCK_SIZE) * BLOCK_SIZE;
        DrawRectangle(mousePos.x,mousePos.y,BLOCK_SIZE,BLOCK_SIZE,CURSOR_COLOR);

        //place block
        if (IsMouseButtonDown(0)){
            areaFree = 1;
            for (int i = 0; i < blockCount; i++) {
                Vector2 blockPos = (Vector2){ blocks[i].x, blocks[i].y };
                if (Vector2Equals(blockPos , mousePos)) {
                    areaFree  = 0;
                    break; }
            }
            if (areaFree){
                blocks[blockCount] = (Rectangle){mousePos.x,mousePos.y,BLOCK_SIZE,BLOCK_SIZE};
                blockCount++;
            }
        }

        //draw blocks
        for (int i = 0; i < blockCount; i++) {
            DrawRectangle(blocks[i].x,blocks[i].y,BLOCK_SIZE,BLOCK_SIZE,RED);
        }
        //are blocks next to eachother
        if (blocks[0].y==blocks[1].y && blocks[0].x==blocks[1].x+BLOCK_SIZE){//b is left of a
            isLeft=1;
        }
        if (blocks[0].y==blocks[1].y && blocks[0].x+BLOCK_SIZE==blocks[1].x){//b is left of a
            isRight=1;
        }
        if (blocks[0].x==blocks[1].x && blocks[0].y==blocks[1].y+BLOCK_SIZE){//b is left of a
            isTop=1;
        }
        if (blocks[0].x==blocks[1].x && blocks[0].y+BLOCK_SIZE==blocks[1].y){//b is left of a
            isBottom=1;
        }
        if (blocks[0].x==blocks[1].x-BLOCK_SIZE && blocks[0].y==blocks[1].y-BLOCK_SIZE){//b is left of a
            isBottomRight=1;
        }
        if (blocks[0].x==blocks[1].x-BLOCK_SIZE && blocks[0].y==blocks[1].y+BLOCK_SIZE){//b is left of a
            isTopRight=1;
        }
        if (blocks[0].x==blocks[1].x+BLOCK_SIZE && blocks[0].y==blocks[1].y+BLOCK_SIZE){//b is left of a
            isTopLeft=1;
        }
        if (blocks[0].x==blocks[1].x+BLOCK_SIZE && blocks[0].y==blocks[1].y-BLOCK_SIZE){//b is left of a
            isBottomLeft=1;
        }
        if (IsKeyPressed(KEY_R)){//reset
            memset(blocks,0,sizeof(blocks));
            blockCount = 0;
        }
        //debug/print
        //DrawText(TextFormat("mousePos: %.2f %.2f", mousePos.x,mousePos.y), 50, 200, 20, RED);
        DrawText("Place two blocks, then (r)eset. Test again for 8 possible states.\nOnly test blocks[0] against block[1].", 20, 80, 20, RED);
        DrawText(TextFormat("blockCount: %d", blockCount), 50, 180, 20, RED);
        DrawText(TextFormat("isLeft: %d", isLeft), 50, 200, 20, RED);
        DrawText(TextFormat("isRight: %d", isRight), 50, 220, 20, RED);
        DrawText(TextFormat("isTop: %d", isTop), 50, 240, 20, RED);
        DrawText(TextFormat("isBottom: %d", isBottom), 50, 260, 20, RED);
        DrawText(TextFormat("isBottomRight: %d", isBottomRight), 50, 280, 20, RED);
        DrawText(TextFormat("isTopRight: %d", isTopRight), 50, 300, 20, RED);
        DrawText(TextFormat("isTopLeft: %d", isTopLeft), 50, 320, 20, RED);
        DrawText(TextFormat("isBottomLeft: %d", isBottomLeft), 50, 340, 20, RED);
        EndDrawing();
    }
    CloseWindow();
}
