#include "raylib.h"
#include <iostream>
using namespace std;

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    
};



bool isOnGround(AnimData data, int windowHeight)
{
    if(data.pos.y >= windowHeight - data.rec.height)
        return true;
    return false;
}

bool CheckCollisionRecs(AnimData& scarf, AnimData& nebula, int windowHeight)
{
    int left = scarf.pos.x;
    int right = scarf.pos.x + scarf.rec.width;
    if(nebula.pos.x + 20 >= left && nebula.pos.x + 20 <= right && isOnGround(scarf,windowHeight))
        return true;
    return false;    
}
void updateAnimData(AnimData& data, float dT, int nebVel)
{
    data.runningTime += dT;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > 7)
            data.frame = 0;        
    } 
    data.pos.x += nebVel * dT;
}

int main()
{
    const int windowWidth = 512;
    const int windowHeight = 360;
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 360;

    InitWindow(windowWidth,windowHeight,"Dapper Dasher Thong Nguyen");
    SetTargetFPS(60);

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowWidth/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowHeight - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;



    const int sizeOfNebulae = 6;

    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
   

    AnimData nebulae[sizeOfNebulae];
    for(int i = 0; i<sizeOfNebulae; i++)
    {
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].pos.x = windowWidth + 300*i;
        nebulae[i].pos.y = static_cast<float>(windowHeight - nebula.height/8.0);
        nebulae[i].rec.height = static_cast<float>(nebula.height/8);
        nebulae[i].rec.width = static_cast<float>(nebula.width/8);
        nebulae[i].rec.x = 0; 
        nebulae[i].rec.y = 0;
        nebulae[i].frame = 0;
    }
    
    //finish line
    float finishLine = nebulae[sizeOfNebulae- 1].pos.x;

    int nebVel = -200;

    //Rectangle Dimension
  
    int velocity = 0;

    //Gravity
    const int gravity = 1000;
    bool isInAir;
    const int jumpVel = -600;


    //scrolling background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX = 0;

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX = 0;

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0;

    bool collisions = false;

    while(!WindowShouldClose())
    {
        float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);          
        //draw the background
        bgX -= 20 * dT;
        if(bgX <= -background.width * 2)
            bgX = 0;
        
        mgX -= 40*dT;
        if(mgX <= -2*midground.width)
            mgX = 0.0;

        fgX -= 80* dT;
        if(fgX <= -2*foreground.width)
            fgX = 0.0;

        
       

        Vector2 bg1Pos{bgX,0.0};
        DrawTextureEx(background,bg1Pos,0.0,2.0,WHITE);

        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background,bg2Pos,0.0,2.0,WHITE);

        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground,mg1Pos,0.0,2.0,WHITE);

        Vector2 mg2Pos{mgX + 2* midground.width, 0.0};
        DrawTextureEx(midground,mg2Pos,0.0,2.0,WHITE);

        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground,fg1Pos,0.0,2.0,WHITE);

        Vector2 fg2Pos{fgX + 2* foreground.width, 0.0};
        DrawTextureEx(foreground,fg2Pos,0.0,2.0,WHITE);
  
        if(collisions)
        {
            DrawText("You lose",windowWidth/4,windowHeight/2,20,BLUE);
        }
        else if(finishLine <= scarfyData.pos.x)
        {
            DrawText("You win",windowWidth/4,windowHeight/2,20,BLUE);
        }
        else
        {
        //perform Ground check
        if(isOnGround(scarfyData,windowHeight))
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
             velocity += gravity*dT;
             isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir)
            velocity += jumpVel;

        //apply gravity
        scarfyData.pos.y += velocity * dT;

        scarfyData.runningTime += dT;
        if(scarfyData.runningTime >= scarfyData.updateTime)
        {
            if(!isInAir)
            {
            scarfyData.runningTime = 0;
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;
            if(scarfyData.frame == scarfy.width / scarfyData.rec.width)
                scarfyData.frame = 0;
            }
        }
        
        for(int i = 0 ; i<sizeOfNebulae; i++)
        {
            updateAnimData(nebulae[i],dT,nebVel);
            DrawTextureRec(nebula,nebulae[i].rec,nebulae[i].pos,RED); 
            if(CheckCollisionRecs(scarfyData,nebulae[i],windowHeight))
            {
                collisions = true;
                break;
            }
        }
        finishLine += nebVel*dT;
        DrawTextureRec(scarfy,scarfyData.rec,scarfyData.pos,WHITE);
        }
        
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}