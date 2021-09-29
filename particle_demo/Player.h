#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "Includes.h"
#include "Bullet.h"
class Player
{
public:
	Player(sf::RenderWindow &in_window);
	void update();
	void dead();
	void alive();
	void draw();
	void redirect(bool turn, std::string direct);
	void forward(bool go);
	void fire_bullet(bool fire);
	void hyperspace(bool pressed);
	vector<sf::Vector2f> getPoints();
	sf::Vector2f getPosition();
	vector<Bullet> bullets;
	vector<sf::Vector2f> points;
	sf::ConvexShape* left_arrow;
	sf::ConvexShape* right_arrow;
	int life = 3;
	float direction = -1 * pi / 2.0f;
	bool is_dead = false;

private:
	sf::RenderWindow* window;
	float x = 400.0f, y = 300.0f, velo_mag = 0.0f, dire_offset = pi / 60.0f, acceleration = player_acceleration, decceleration = player_decceleration;
	float sin_of_offset = sin(dire_offset);
	float cos_of_offset = cos(dire_offset);
	bool turning = false, going = false, space_pressed = false, h_pressed = false;
	sf::SoundBuffer fire_buffer, thrust_buffer;
	sf::Sound fire_sound, thrust_sound;
	
};
#endif
