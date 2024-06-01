
#include "Entity.h"
#include <cmath>
#include "Globals.h"

Entity::Entity(const Point& p, int w, int h) : pos(p), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr), render2(nullptr)
{
}
Entity::Entity(const Point& p, int w, int h, int frame_w, int frame_h) : pos(p), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
}
Entity::~Entity()
{
	if (render != nullptr)
	{
		delete render;
		render = nullptr;
	}
}
void Entity::SetPos(const Point& p)
{
	pos = p;
}
void Entity::Update()
{
	pos += dir;
}
AABB Entity::GetHitbox() const
{
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
Point Entity::GetRenderingPosition() const
{
	Point p;
	p.x = pos.x + width / 2 - frame_width / 2;
	p.y = pos.y - (frame_height-1);
	return p;
}
void Entity::Draw() const
{
	Point p = GetRenderingPosition();
	render->Draw(p.x, p.y);
}
void Entity::DrawTint(const Color& col) const
{
	Point p = GetRenderingPosition();
	render->DrawTint(p.x, p.y, col);
}
void Entity::DrawHitbox(const Color& col) const
{
	Color c = col;
	c.a = 128;  // 50% transparent

	// Dibujar hitbox la mitad del tamaño original
	int halfWidth = width / 2;
	int halfHeight = height / 2;
	int halfPosX = pos.x + halfWidth / 2;
	int halfPosY = pos.y - (height - 1) + halfHeight / 2;

	render->DrawBox(halfPosX, halfPosY, halfWidth, halfHeight, c);
	render->DrawCorners(halfPosX, halfPosY, halfWidth, halfHeight);
}

void Entity::DrawHitbox(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;  // 50% transparent

	// Dibujar hitbox la mitad del tamaño original
	int halfWidth = w / 2;
	int halfHeight = h / 2;
	int halfPosX = x + halfWidth / 2;
	int halfPosY = y - (h - 1) + halfHeight / 2;

	render->DrawBox(halfPosX, halfPosY, halfWidth, halfHeight, c);
	render->DrawCorners(halfPosX, halfPosY, halfWidth, halfHeight);
}
