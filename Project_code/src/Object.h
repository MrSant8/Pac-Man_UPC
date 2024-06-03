#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	8
#define OBJECT_FRAME_SIZE		8

#define POINTS_APPLE	50
#define POINTS_CHILI	10

enum class ObjectType { APPLE, CHILI };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	//int SoundItem() const;

private:
	ObjectType type;
};

