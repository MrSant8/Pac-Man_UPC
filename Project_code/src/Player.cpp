#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
    Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE),
    lastKeyPressed(KEY_NULL) // Inicializamos con KEY_NULL
{
    state = s;
    look = view;
    jump_delay = PLAYER_JUMP_DELAY;
    map = nullptr;
    score = 0;
}

Player::~Player() {}

AppStatus Player::Initialise()
{
    int i;
    const int n = PLAYER_FRAME_SIZE; // El tamaño de cada frame, debería ser 8

    ResourceManager& data = ResourceManager::Instance();
    if (data.LoadTexture(Resource::IMG_PLAYER, "images/eric.png") != AppStatus::OK)
    {
        LOG("Failed to load player sprite texture");
        return AppStatus::ERROR;
    }

    render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
    if (render == nullptr)
    {
        LOG("Failed to allocate memory for player sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);

    // Configuración de las animaciones
    sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0.0f, 0.0f, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0.0f, 0.0f, -(float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
    for (i = 0; i < 4; ++i)
        sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { i * (float)n, 0.0f, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
    for (i = 0; i < 4; ++i)
        sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { i * (float)n, (float)n, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 2 * (float)n, 2 * (float)n, (float)n, (float)n });
    sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 3 * (float)n, 2 * (float)n, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 2 * (float)n, 2 * (float)n, -(float)n, (float)n });
    sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 3 * (float)n, 2 * (float)n, -(float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { (float)n, 3 * (float)n, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { (float)n, 3 * (float)n, -(float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_LADDER_DELAY);
    for (i = 0; i < 4; ++i)
        sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { i * (float)n, 4 * (float)n, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 4 * (float)n, 4 * (float)n, (float)n, (float)n });

    sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY);
    sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { 5 * (float)n, 4 * (float)n, (float)n, (float)n });

    sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

    return AppStatus::OK;
}

void Player::InitScore()
{
    score = 0;
}

void Player::IncrScore(int n)
{
    score += n;
}

int Player::GetScore()
{
    return score;
}

void Player::SetTileMap(TileMap* tilemap)
{
    map = tilemap;
}

bool Player::IsLookingRight() const
{
    return look == Look::RIGHT;
}

bool Player::IsLookingLeft() const
{
    return look == Look::LEFT;
}

bool Player::IsAscending() const
{
    return dir.y < -PLAYER_LEVITATING_SPEED;
}

bool Player::IsLevitating() const
{
    return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}

bool Player::IsDescending() const
{
    return dir.y > PLAYER_LEVITATING_SPEED;
}

bool Player::IsInFirstHalfTile() const
{
    return pos.y % TILE_SIZE < TILE_SIZE / 2;
}

bool Player::IsInSecondHalfTile() const
{
    return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}

void Player::SetAnimation(int id)
{
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetAnimation(id);
    LOG("Setting animation to: %d", id); // Añade esta línea
}

PlayerAnim Player::GetAnimation()
{
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    return (PlayerAnim)sprite->GetAnimation();
}

void Player::Stop()
{
    dir = { 0, 0 };
    state = State::IDLE;
    lastKeyPressed = KEY_NULL; // Reiniciamos la última tecla presionada
    if (IsLookingRight()) SetAnimation((int)PlayerAnim::IDLE_RIGHT);
    else SetAnimation((int)PlayerAnim::IDLE_LEFT);
}

void Player::StartWalkingLeft()
{
    state = State::WALKING;
    look = Look::LEFT;
    SetAnimation((int)PlayerAnim::WALKING_LEFT);
}

void Player::StartWalkingRight()
{
    state = State::WALKING;
    look = Look::RIGHT;
    SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}

void Player::StartClimbingUp() {
    state = State::CLIMBING;
    SetAnimation((int)PlayerAnim::CLIMBING);
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetManualMode();
}

void Player::StartClimbingDown() {
    state = State::CLIMBING;
    SetAnimation((int)PlayerAnim::CLIMBING);
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetManualMode();
}

void Player::ChangeAnimRight()
{
    look = Look::RIGHT;
    switch (state)
    {
    case State::IDLE: SetAnimation((int)PlayerAnim::IDLE_RIGHT); break;
    case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
    case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
    }
}

void Player::ChangeAnimLeft()
{
    look = Look::LEFT;
    switch (state)
    {
    case State::IDLE: SetAnimation((int)PlayerAnim::IDLE_LEFT); break;
    case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
    case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
    }
}

void Player::Update()
{
    if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
        MoveAutomatically();
    }
    else {
        MoveX();
        MoveY();
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->Update();
}

void Player::MoveX() {
    AABB box;
    int prev_x = pos.x;

    bool moveAttempted = false;

    if (IsKeyDown(KEY_LEFT)) {
        moveAttempted = true;
        pos.x -= PLAYER_SPEED;
        if (state != State::WALKING || IsLookingRight()) {
            ChangeAnimLeft();
            StartWalkingLeft();
        }
        lastKeyPressed = KEY_LEFT;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        moveAttempted = true;
        pos.x += PLAYER_SPEED;
        if (state != State::WALKING || IsLookingLeft()) {
            ChangeAnimRight();
            StartWalkingRight();
        }
        lastKeyPressed = KEY_RIGHT;
    }

    box = GetHitbox();
    if (moveAttempted && (map->TestCollisionWallLeft(box) || map->TestCollisionWallRight(box))) {
        pos.x = prev_x;
    }

    if (!moveAttempted) {
        Stop();
    }
}

void Player::MoveY() {
    AABB box;
    int prev_y = pos.y;

    bool moveAttempted = false;

    if (IsKeyDown(KEY_UP)) {
        moveAttempted = true;
        pos.y -= PLAYER_SPEED;
        if (state != State::CLIMBING) {
            StartClimbingUp();
        }
        lastKeyPressed = KEY_UP;
    }
    else if (IsKeyDown(KEY_DOWN)) {
        moveAttempted = true;
        pos.y += PLAYER_SPEED;
        if (state != State::CLIMBING) {
            StartClimbingDown();
        }
        lastKeyPressed = KEY_DOWN;
    }

    box = GetHitbox();
    if (moveAttempted && (map->TestCollisionWallUp(box) || map->TestCollisionWallDown(box))) {
        pos.y = prev_y;
    }

    if (!moveAttempted) {
        Stop();
    }
}

void Player::MoveAutomatically() {
    if (lastKeyPressed == KEY_LEFT || lastKeyPressed == KEY_RIGHT) {
        MoveX();
    }
    else if (lastKeyPressed == KEY_UP || lastKeyPressed == KEY_DOWN) {
        MoveY();
    }
}

void Player::DrawDebug(const Color& col) const
{
    Entity::DrawHitbox(pos.x, pos.y, width, height, col);

    DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
    DrawPixel(pos.x, pos.y, WHITE);
}

void Player::Release()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_PLAYER);

    render->Release();
}
