#include "raylib.h"
#include "raymath.h"
#include <math.h>       // Нужно для sinf()

//------------------------------------------------------------------------------------
// Начало
//------------------------------------------------------------------------------------
int main()
{
    // Инициализация
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Cuba");

    Model knife = LoadModel("res/Kitchenknife_lowpoly.obj");
    // Настройка камеры
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 30.0f, 20.0f, 30.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 70.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Задаём кол-во ножей для каждого направления
    const int numBlocks = 15;
    float rotato = 0;
    float rotatoSpd = 0.1f;
    
    //60 кадров в секунду было бы славно
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Делаем дело
    while (!WindowShouldClose())    // надоело смотреть на ножи?
    {
        // обновляем значения
        //----------------------------------------------------------------------------------
        double time = GetTime();
        if (IsKeyPressed(KEY_A))
            rotatoSpd = -rotatoSpd;
        rotato += rotatoSpd;
        if (rotato > 6.28)
            rotato = 0;
        knife.transform = MatrixRotateZ(rotato);
        
        float scale = (2.0f + (float)sin(time)) * 0.7f;

        // двигаем камеру туда-сюда
        double cameraTime = time * 0.3;
        camera.position.x = (float)cos(cameraTime) * 40.0f;
        camera.position.z = (float)sin(cameraTime) * 40.0f;
        //----------------------------------------------------------------------------------

        // рисуем...
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawGrid(10, 5.0f);

        for (int x = 0; x < numBlocks; x++)
        {
            for (int y = 0; y < numBlocks; y++)
            {
                for (int z = 0; z < numBlocks; z++)
                {
                    
                    float blockScale = (x + y + z) / 30.0f;

                    // создаём эффект волны
                    float scatter = sinf(blockScale * 20.0f + (float)(time * 4.0f));

                    // вычисляем позицию ножика
                    Vector3 knifePos = {
                        (float)(x - numBlocks / 2) * (scale * 3.0f) + scatter,
                        (float)(y - numBlocks / 2) * (scale * 2.0f) + scatter,
                        (float)(z - numBlocks / 2) * (scale * 3.0f) + scatter
                    };

                    // Задаём тон радуги, зависящий от положения ножей
                    Color knifeColor = ColorFromHSV((float)(((x + y + z) * 18) % 360), 0.75f, 0.9f);

                    // вычисляем размер ножа и рисуем
                    float knifeSize = (2.4f - scale) * blockScale;
                    DrawModel(knife, knifePos, knifeSize, knifeColor);
                    
                }
            }
        }

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadModel(knife);
    // выгружаем модельку
    //--------------------------------------------------------------------------------------
    CloseWindow();        // закрываем окно 
    //--------------------------------------------------------------------------------------

    return 0;
}