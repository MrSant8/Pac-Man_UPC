#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 
#define PLAYER_FRAME_SIZE		8
#define ENEMIES_FRAME_SIZE      8

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	8
#define PLAYER_PHYSICAL_HEIGHT	8

#define PLAYER_SPEED            1
#define PLAYER_LADDER_SPEED     0.5f


//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		0.5

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		0.5

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	0.5

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			0.5

//Logic states
enum class State { IDLE, WALKING, FALLING, CLIMBING, DEAD };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	NUM_ANIMATIONS, ENEMIE_WALK_RIGHT
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();


private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicClimbing();
	void MoveAutomatically();

	// Nueva variable para almacenar la última tecla presionada
	int lastKeyPressed;
	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartClimbingUp();
	void StartClimbingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;
	int jump_delay;

	TileMap *map;

	int score;
};

