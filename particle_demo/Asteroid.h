#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H
#include "Includes.h"


class Asteroid
{
public:
	Asteroid(sf::Vector2f position, float velocity, float velo_direction, float in_direction_offset, sf::RenderWindow &in_window);
	Asteroid(sf::Vector2f position, float velo_direction, sf::RenderWindow &in_window);
	void addPoint(float mag, float in_direction); // in local coordinate
	void sortPairs();
	void update();
	void draw();
	void setDirectionOffset(float in_direction_offset);
	float getVeloDire();
	bool shouldDestroy();
	bool check_intersection(sf::Vector2f point);
	void updateVelocityMagAndDirection();
	void resetFrame();


	float dx, dy;
	float velo_direction, velo_mag;
	vector<MagAndDirection> mag_and_dire;
	int size = 3;
	vector<sf::Vertex> lines;
	sf::Vector2f global_center_coordinate;


private:
	float getArea(sf::Vertex A, sf::Vertex B, sf::Vertex C);
	bool inTriangle(sf::Vector2f pointy, sf::Vertex a, sf::Vertex b, sf::Vertex c);
	int randomNumber(int a, int b);
	float randomNumber(float a, float b);

	float asteroid_direction = 0.0f, // rotation offset
		dire_offset;
	float cos_of_offset, sin_of_offset;
	sf::RenderWindow* window;
	bool have_entered = false;
	int current_frame = 0;
};

#endif // !


