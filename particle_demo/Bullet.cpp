#include "Bullet.h"

Bullet::Bullet(float direction, float x, float y)
{
	this_direction = direction;
	this->setFillColor(sf::Color(255, 255, 255));
	this->setRadius(bullet_size);
	this->setOrigin(bullet_size, bullet_size);
	this->setPosition(sf::Vector2f(x, y));
	this->rotate(direction * 180 / pi);
	
}

void Bullet::update()
{
	this->move(sf::Vector2f(cos(this_direction) * bullet_velo, sin(this_direction) * bullet_velo));
	float x = this->getPosition().x;
	float y = this->getPosition().y;
	should_destroy = (x < 0 || x > window_width) || (y < 0 || y > window_height);
}

bool Bullet::shouldDestroy()
{
	return should_destroy;
}
