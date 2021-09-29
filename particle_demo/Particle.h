#ifndef PARTICLE_H
#define PARTICLE_H
#include "Includes.h"


class Particle
{
public:
	Particle(float dire, float dire_offset, sf::Vector2f position, sf::RenderWindow& in_window);
	void emit(bool in_on);
	void setPosition(sf::Vector2f input);
	void shoot();
	void explode();
	void updateDirection(float input);
	void update();
	void draw();
	bool shouldDestroy();
	bool on = false;
	bool hasExploded = false;
	float explostion_radius = 0.0f;
	struct MagAndDirection
	{
		float mag;
		float direction = 0.0f;
	};

	std::vector<sf::CircleShape> particles;
	std::vector<MagAndDirection> mag_and_dire;


private:

	// 255 75 0
	// 255 120 0
	int randomNumber(int a, int b);
	float randomNumber(float a, float b);

	float direction = 0.0f;
	float direction_offset = 0.0f;
	float beginning_velocity = 3.0f;
	float decceleration = -0.05f;
	float dx, dy;
	float dr = 0, dg = 10, db = 10;
	sf::RenderWindow* window;
	sf::Vector2f spawning_origin;

};
#endif;