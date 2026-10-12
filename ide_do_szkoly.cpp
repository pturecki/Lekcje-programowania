
#include "raylib.h"
#include "raymath.h"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f


struct CPoint
{
    float x = 0;
    float y = 0;
    float followSpeed = 1;
    float followXDir = 1;
    
    float localX = 0;
    float localY = 0;
  
    float t = 0;
    float phase = 0;
    float amp = 0;
    float speed = 1;
};


struct CPlayer
{
    float x = 0;
    float y = 0;

    int legsAnimState = 0;
    float legsAnimTime = 0;
    
    float velocityX = 0;
    
    float jumpAnimTime = 0;
};


//


int ScreenWidth = -1;
int ScreenHeight = -1;
int GroundLevel = -1;
CPlayer Player;

CPoint HeadPointA, HeadPointB;
CPoint LeftArmPointA, LeftArmPointB;
CPoint RightArmPointA, RightArmPointB;


//


void DrawAddBg()
{
}

void DrawAddPlayer()
{
}


//


void Init()
{
    ScreenWidth = GetScreenWidth();
    ScreenHeight = GetScreenHeight();
    GroundLevel = ScreenHeight * 0.8f;
    
    RightArmPointB.phase = 0.5f;
    RightArmPointB.speed = 1.25f;
    HeadPointB.followSpeed = 4;
    HeadPointB.followXDir = -1;
    
    
    Player.x = 20;
}

void DrawBg()
{    
    DrawRectangle(0, GroundLevel, ScreenWidth, ScreenHeight - GroundLevel, BROWN);
    
    const char* title = "Ide do szkoly";
    int titleFoneSize = 40;
    int textWidth = MeasureText(title, titleFoneSize);
    
    DrawText(title, ScreenWidth/2 - textWidth/2, 20, titleFoneSize, WHITE);
    
    //
    
    DrawRectangle(ScreenWidth - 200, GroundLevel - 300, 200, 300, WHITE);
    DrawText("SZKOLA", ScreenWidth - 200 + 20, GroundLevel - 300 + 20, 20, BLACK);

    //
    
    DrawAddBg();
    
    //
    
    DrawText("sterowanie - lewo, prawo, skok = spacja", 20, 120, 25, WHITE);
}


void UpdatePoint(float deltaTime, float velocityX, float velocityY, CPoint& A, CPoint& B)
{    
    B.x = Lerp(B.x, Player.x, 0.05f * B.followSpeed);
    B.localX = B.x - Player.x;
    
    B.localX *= B.followXDir;
        
    
    B.y = Lerp(B.y, Player.y, 0.05f * B.followSpeed);
    B.localY = B.y - Player.y;
    
    B.amp = 10;
    B.t += deltaTime * 15 * B.speed;
}


void UpdatePlayer(float deltaTime)
{
    Player.velocityX = 0;
    
    //
    
    if (IsKeyDown(KEY_LEFT))
    {
        Player.velocityX = -150;
        Player.x += deltaTime * Player.velocityX;
        
        if (Player.x < 20)
        {
            Player.x = 20;
        }
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        Player.velocityX = 150;
        Player.x += deltaTime * Player.velocityX;
        
        if (Player.x > ScreenWidth-20)
        {
            Player.x = ScreenWidth-20;
        }
    }
    if (IsKeyDown(KEY_SPACE))
    {
        if (Player.jumpAnimTime <= 0)
        {
            Player.jumpAnimTime = 1;
        }
    }
    
    //
    
    Player.legsAnimTime += deltaTime;
    if (Player.legsAnimTime > 0.15f)
    {
       Player.legsAnimTime = 0;
       Player.legsAnimState = 1 - Player.legsAnimState;
    }
    
    if (Player.velocityX == 0)
    {
        Player.legsAnimState = 0;
    }
    
    //
    
    Player.jumpAnimTime -= deltaTime * 3;
    if (Player.jumpAnimTime < 0)
    {
        Player.jumpAnimTime = 0;
        Player.y = 0;
    }
    
    if (Player.jumpAnimTime > 0)
    {
        Player.legsAnimState = 1;
        Player.y = -sin( 3.14f / 180.0f * 180 * Player.jumpAnimTime) * 80;
    }
    
    //
    
    UpdatePoint(deltaTime, Player.velocityX, 0, HeadPointA, HeadPointB);
    UpdatePoint(deltaTime, Player.velocityX, 0, LeftArmPointA, LeftArmPointB);
    UpdatePoint(deltaTime, Player.velocityX, 0, RightArmPointA, RightArmPointB);
    
}


void DrawPlayer()
{
    float posX = Player.x;
    float posY = Player.y + GroundLevel;
    
    // nogi
    
    float legsPosY = posY - 20;
    float headPosY = posY - 50;
    
    if (Player.legsAnimState == 0)
    {
        DrawLine(posX, posY, posX, legsPosY, DARKGRAY);
    }
    else
    {
        DrawLine(posX - 10, posY, posX, legsPosY, DARKGRAY);
        DrawLine(posX + 10, posY, posX, legsPosY, DARKGRAY);
    }
    
    // brzuch
    
    float w = 10;
    DrawRectangle(posX - w/2, headPosY, w, abs(legsPosY - headPosY), DARKBROWN);
    
    // glowa
    
    float r = 10;
    float hx = HeadPointB.localX;
    float hy = HeadPointB.localY;
    DrawCircle(posX + hx, headPosY - r + hy, r, YELLOW);
    
    // rece
    float sinYL = sin(LeftArmPointB.t + LeftArmPointB.phase) * LeftArmPointB.amp;
    float sinYR = sin(RightArmPointB.t + RightArmPointB.phase) * RightArmPointB.amp;
    
    float l = 20;
    DrawLine(posX + LeftArmPointA.x, headPosY + 5, posX - l + LeftArmPointB.localX, headPosY + 5 + sinYL + LeftArmPointB.localY, DARKGRAY);    
    DrawLine(posX + RightArmPointA.x, headPosY + 5, posX + l + RightArmPointB.localX, headPosY + 5 + sinYR + RightArmPointB.localY, DARKGRAY);
    
    DrawAddPlayer();
    
    // teksty
    if (Player.velocityX == 0)
    {
        const char* txt = "Ktoredy do szkoly ???";
        int fSize = 20;
        int textWidth = MeasureText(txt, fSize);
    
        DrawText(txt, posX - textWidth/2, posY - 150, fSize, WHITE);
    }
}


int main(void)
{    
    int sw = 0;
    int sh = 0;
    InitWindow(sw, sh, "Do szkoly ide sobie");
    SetTargetFPS(60);
    
    Init();
    
    
    
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        UpdatePlayer(deltaTime);

        BeginDrawing();            
            ClearBackground(SKYBLUE);
            DrawBg();
            DrawPlayer();
        EndDrawing();
    }
    CloseWindow();        
    return 0;    
}
