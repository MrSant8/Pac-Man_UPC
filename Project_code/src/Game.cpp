#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
    : state(GameState::DESCRIPTION_GAME), scene(nullptr), img_menu(nullptr), img_menu2(nullptr),
    img_menuGAME_OVER(nullptr), img_menuWINNER_LEVEL(nullptr), currentIntroImageIndex(0),
    introImageTime(0.0f), target({}), src({}), dst({})
{
    // Inicializar los arreglos de sonidos y música a valores por defecto
    for (int i = 0; i < 7; i++)
    {
        menuSonidos[i] = { 0 };
        musicaIngame[i] = { 0 };
    }
}

Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}

AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    // Inicializar ventana
    InitWindow((int)w, (int)h, "Pac-Man 1980");

    // Inicialización de la textura de renderizado, usada para mantener el resultado del renderizado y así poder redimensionarlo fácilmente
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    // Cargar recursos
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    // Establecer el frame rate objetivo de la aplicación
    SetTargetFPS(60);
    // Deshabilitar la funcionalidad de cerrar con la tecla Escape
    SetExitKey(0);

    return AppStatus::OK;
}

AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "images/Intro/mainMenuFinal1.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_1, "images/MAINMENUDESCRIPTION.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu2 = data.GetTexture(Resource::IMG_1);

    if (data.LoadTexture(Resource::IMG_2, "images/MenuGameOver.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menuGAME_OVER = data.GetTexture(Resource::IMG_2);

    if (data.LoadTexture(Resource::IMG_3, "images/MenuWInner.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menuWINNER_LEVEL = data.GetTexture(Resource::IMG_3);

    // Cargar las imágenes de la intro
    char filename[64];
    for (int i = 1; i <= 41; i++)
    {
        sprintf_s(filename, sizeof(filename), "images/Intro/mainMenuFinal%d.png", i);
        Texture2D texture = LoadTexture(filename);
        if (texture.id == 0)
        {
            LOG("Failed to load intro image %d", i);
            return AppStatus::ERROR;
        }
        introImages.push_back(texture);
    }

    return AppStatus::OK;
}

AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}

void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}

AppStatus Game::Update()
{
    // Verificar si el usuario intenta cerrar la ventana, ya sea haciendo clic en el botón de cerrar o presionando Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
    case GameState::DESCRIPTION_GAME:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            state = GameState::MAIN_MENU;
        }

        // Actualizar la imagen de la intro
        introImageTime += GetFrameTime();
        if (introImageTime >= INTRO_IMAGE_DURATION)
        {
            introImageTime = 0;
            currentIntroImageIndex++;
            if (currentIntroImageIndex >= introImages.size())
            {
                currentIntroImageIndex = 0;
            }
        }

        break;

    case GameState::MAIN_MENU:
        if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
        if (IsKeyPressed(KEY_SPACE))
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
            state = GameState::PLAYING;
        }
        break;

    case GameState::PLAYING:
        if (IsKeyPressed(KEY_ESCAPE))
        {
            FinishPlay();
            state = GameState::MAIN_MENU;
        }
        else
        {
            // Lógica del juego
            scene->Update();
        }
        break;

    case GameState::WINNER_LEVEL:
        if (IsKeyPressed(KEY_F3)) {
            state = GameState::WINNER_LEVEL;
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            state = GameState::MAIN_MENU;
        }
        break;

    case GameState::GAME_OVER:
        if (IsKeyPressed(KEY_F4)) {
            state = GameState::GAME_OVER;
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            state = GameState::MAIN_MENU;
        }
        break;
    }

    return AppStatus::OK;
}

void Game::Render()
{
    // Dibujar todo en la textura de renderizado, nota que esto no se renderizará en pantalla, aún
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::DESCRIPTION_GAME:
        DrawTexture(introImages[currentIntroImageIndex], 0, 0, WHITE);
        break;

    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;

    case GameState::PLAYING:
        scene->Render();
        break;

    case GameState::GAME_OVER:
        DrawTexture(*img_menuGAME_OVER, 0, 0, WHITE);
        break;

    case GameState::WINNER_LEVEL:
        DrawTexture(*img_menuWINNER_LEVEL, 0, 0, WHITE);
        break;
    }

    EndTextureMode();

    // Dibujar la textura de renderizado en la pantalla, escalada correctamente
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}

void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}

void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_1);
    data.ReleaseTexture(Resource::IMG_2);
    data.ReleaseTexture(Resource::IMG_3);

    // Liberar las texturas de la intro
    for (const Texture2D& texture : introImages)
    {
        UnloadTexture(texture);
    }

    UnloadRenderTexture(target);
}
