#pragma once
#include "Globals.h"
#include "Scene.h"
#include "raylib.h"
#include <vector>

enum class GameState { MAIN_MENU, PLAYING, SETTINGS, CREDITS, DESCRIPTION_GAME, GAME_OVER, WINNER_LEVEL, PreIntro
};

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_menu, * img_menu2, * img_menuGAME_OVER, * img_menuWINNER_LEVEL;

    Sound menuSonidos[7];
    Music musicaIngame[7];

    // Para trabajar con las unidades originales del juego y luego escalar el resultado
    RenderTexture2D target;
    Rectangle src, dst;

    // Añadir un vector para las imágenes de la intro
    std::vector<Texture2D> introImages;
    int currentIntroImageIndex;

    float introImageTime;
    const float INTRO_IMAGE_DURATION = 0.5f; // Duración en segundos de cada imagen
    const float INTRO_IMAGE_DURATION2 = 0.2f; // Duración en segundos de cada imagen

};
