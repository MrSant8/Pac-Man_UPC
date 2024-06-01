#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

    // -1: área cubierta por una entidad
    EMPTY = -1,
    //  0: baldosa de aire
    AIR = 0,

    // 0 < id < 50: baldosas estáticas
    BLOCK_SQUARE1_TL = 1, BLOCK_SQUARE1_TR, BLOCK_SQUARE1_BL, BLOCK_SQUARE1_BR,
    BLOCK_SQUARE2_TL, BLOCK_SQUARE2_TR, BLOCK_SQUARE2_BL, BLOCK_SQUARE2_BR,
    BLOCK_VERT2_T, BLOCK_VERT2_B, BLOCK_HORIZ2_L, BLOCK_HORIZ2_R, BLOCK_BLUE,
    BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
    BLOCK_BEAM_L, BLOCK_BEAM_R,

    LADDER_L = 20, LADDER_R, LADDER_TOP_L, LADDER_TOP_R,
    LOCK_RED = 30, LOCK_YELLOW,
    LASER_L = 40, LASER_R,

    // 50 <= id < 100: baldosas especiales
    DOOR = 50,
    KEY_RED = 60, YELLOW_KEY, ITEM_APPLE, ITEM_CHILI,
    LASER = 70, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,

    // id >= 100: ubicaciones iniciales de las entidades
    PLAYER = 100, AMIGO1,

    //Intervalos
    STATIC_FIRST = BLOCK_SQUARE1_TL,
    STATIC_LAST = LASER_R,
    SOLID_FIRST = BLOCK_SQUARE1_TL,
    SOLID_LAST = BLOCK_BEAM_R,
    SPECIAL_FIRST = DOOR,
    SPECIAL_LAST = LASER,
    ENTITY_FIRST = PLAYER,
    ENTITY_LAST = PLAYER
};

class TileMap
{
public:
    TileMap();
    ~TileMap();

    AppStatus Initialise();
    AppStatus Load(int data[], int w, int h);
    void Update();
    void Render();
    void Release();

    bool TestCollisionCeiling(const AABB& box, int* py) const;

    // Prueba de colisiones con las paredes
    bool TestCollisionWallLeft(const AABB& box) const;
    bool TestCollisionWallRight(const AABB& box) const;
    bool TestCollisionWallUp(const AABB& box) const;
    bool TestCollisionWallDown(const AABB& box) const;

    // Prueba de colisión con el suelo y actualización de 'py' con la posición y máxima
    // y para evitar la penetración de la baldosa en el suelo, es decir, la posición y del píxel
    // sobre la baldosa en el suelo.
    // Baldosa en el suelo = baldosa sólida (bloques) o partes superiores de escaleras.
    bool TestCollisionGround(const AABB& box, int* py) const;

    // Prueba si hay una baldosa en el suelo un píxel debajo de la caja dada
    bool TestFalling(const AABB& box) const;

    // Prueba si la caja está en la escalera y actualiza 'px' con la posición x-centro de la escalera
    bool TestOnLadder(const AABB& box, int* px) const;

    // Prueba si la caja está en la parte superior de la escalera y actualiza 'px' con la posición x-centro de la escalera
    bool TestOnLadderTop(const AABB& box, int* px) const;

    bool TestCollisionCeiling(const Point& p, int distance) const;

private:
    void InitTileDictionary();

    Tile GetTileIndex(int x, int y) const;
    bool IsTileSolid(Tile tile) const;
    bool IsTileLadderTop(Tile tile) const;
    bool IsTileLadder(Tile tile) const;
    bool CollisionX(const Point& p, int distance) const;
    bool CollisionY(const Point& p, int distance) const;
    int GetLadderCenterPos(int pixel_x, int pixel_y) const;

    // Mapa de baldosas
    Tile* map;

    // Tamaño del mapa de baldosas
    int size, width, height;

    // Diccionario de marcos de baldosas
    std::unordered_map<int, Rectangle> dict_rect;

    Sprite* laser;

    // Hoja de baldosas
    const Texture2D* img_tiles, * img_enemy_red;
};
