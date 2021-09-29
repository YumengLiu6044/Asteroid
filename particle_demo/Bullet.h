#ifndef BULLET_H
#define BULLET_H
#include "Includes.h"
class Bullet : public sf::CircleShape
{
public:
	Bullet(float direction, float x, float y);
	void update();
	bool shouldDestroy();
private:
	float speed = 0.3f;
	bool should_destroy = false;
	float this_direction = 0.0f;

};
#endif
