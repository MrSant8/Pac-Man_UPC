#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::DESCRIPTION_GAME;
    scene = nullptr;
    img_menu = nullptr;
    img_menu2 = nullptr;

    target = {};
    src = {};
    dst = {};
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

    //Initialise window
    InitWindow((int)w, (int)h, "Pac-Man 1980");

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

   
   // 
   // 
    //menuSonidos[0] = LoadSound("Sounds/pacman_beginning.ogg"); // sonido pre partida
    //menuSonidos[1] = LoadSound("Sounds/pacman_chomp.wav"); // sonido muerte
    //menuSonidos[2] = LoadSound("Sounds/pacman_deadth.wav"); // sonido comer fruta
    //menuSonidos[3] = LoadSound("Sounds/pacman_eatfruit.wav"); // sonido comer fruta
    //menuSonidos[4] = LoadSound("Sounds/pacman_eatghost.wav"); // sonido comer fruta
    //menuSonidos[5] = LoadSound("Sounds/pacman_extrapac.wav"); // sonido comer fruta
    //menuSonidos[6] = LoadSound("Sounds/pacman_intermission.wav"); // sonido comer fruta
    //menuSonidos[7] = LoadSound("Sounds/pacman_ringtone.wav"); // sonido comer fruta
    //menuSonidos[8] = LoadSound("Sounds/pacman_fingtone_interlude.wav"); // sonido comer fruta

    //musicaIngame[0] = LoadMusicStream("Sounds/pacman_beginning.ogg"); // sonido pre partida
    //musicaIngame[1] = LoadMusicStream("Sounds/pacman_chomp.ogg"); // sonido comer puntos
    //musicaIngame[2] = LoadMusicStream("Sounds/pacman_deadth.ogg"); // sonido morir
    //musicaIngame[3] = LoadMusicStream("Sounds/pacman_eatfruit.ogg"); // sonido comer fruta
    //musicaIngame[4] = LoadMusicStream("Sounds/pacman_eatghost.ogg"); // sonido comer ghost
    //musicaIngame[5] = LoadMusicStream("Sounds/pacman_extrapac.ogg"); // sonido comer extra
    //musicaIngame[6] = LoadMusicStream("Sounds/pacman_intermission.ogg"); // sonido intermission
    //musicaIngame[7] = LoadMusicStream("Sounds/pacman_ringtone.ogg"); // sonido ringtone
    //musicaIngame[8] = LoadMusicStream("Sounds/pacman_fingtone_interlude.ogg"); // sonido ringtonex2 


    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);



    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/mainMenuFinal18.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);


    ResourceManager& data2 = ResourceManager::Instance();

    if (data2.LoadTexture(Resource::IMG_1, "images/MAINMENUDESCRIPTION.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu2 = data2.GetTexture(Resource::IMG_1);


    ResourceManager& data3 = ResourceManager::Instance();

    if (data3.LoadTexture(Resource::IMG_2, "images/MenuGameOver.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menuGAME_OVER = data3.GetTexture(Resource::IMG_2);


    ResourceManager& data4 = ResourceManager::Instance();

    if (data4.LoadTexture(Resource::IMG_3, "images/MenuWInner.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menuWINNER_LEVEL = data4.GetTexture(Resource::IMG_3);


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
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;


    switch (state)
    {
       
        case GameState::DESCRIPTION_GAME:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::MAIN_MENU;
            }
            break;

        case GameState::MAIN_MENU: 
           
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
            }
            break;

        case GameState::PLAYING:  
//            PlayMusicStream(musicaIngame[0]);
          //   PlaySound(LoadSound[0]);

            //PlayMusicStream(musicaIngame[0]);
            //UpdateMusicStream(musicaIngame[0]); // bucle
            //    StopMusicStream(musicaIngame[0]); // parar musica
            //    PlaySound(musica[0]) // sonidos al teclear
 
         


            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;
            }
            else
            {
                //Game logic
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
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    state = GameState::MAIN_MENU;
                }
            }

          
            break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {

    case GameState::DESCRIPTION_GAME:
        DrawTexture(*img_menu2, 0, 0, WHITE);
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

    //Draw render texture to screen, properly scaled
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

    UnloadRenderTexture(target);
}