#include "Particle.h"

Particle::Particle(float dire, float dire_offset, sf::Vector2f position, sf::RenderWindow& in_window)
{
	direction = dire;
	direction_offset = dire_offset;
	setPosition(position);
	window = &in_window;
}

void Particle::emit(bool in_on)
{
	on = in_on;
}

void Particle::setPosition(sf::Vector2f input)
{
	spawning_origin = input;
}

void Particle::shoot()
{
	int chance = rand() % 100;
	if (chance < intensity && on)
	{
		sf::CircleShape part(1.4);
		part.setOrigin(1.4, 1.4);
		part.setPosition(spawning_origin);
		part.setFillColor(
			sf::Color(255, randomNumber(75, 120), 0)
		);
		MagAndDirection something;
		something.mag = beginning_velocity;
		something.direction = randomNumber(direction - direction_offset, direction + direction_offset);
		particles.push_back(part);
		mag_and_dire.push_back(something);
	}
}

void Particle::explode()
{
	hasExploded = true;
	dr = 0;
	dg = 0;
	db = 0;
	for (int i = 0; i < explosion_intensity; i++)
	{
		sf::CircleShape part(randomNumber(1.3f, 1.5f));
		part.setOrigin(part.getRadius(), part.getRadius());
		part.setFillColor(sf::Color(244, 244, 244));
		part.setPosition(spawning_origin);
		MagAndDirection stuff;
		stuff.direction = randomNumber(0, 360) * degree;
		stuff.mag = randomNumber(beginning_velocity - 0.4f, beginning_velocity + 0.4f);
		decceleration = -0.07f;
		particles.push_back(part);
		mag_and_dire.push_back(stuff);

	}
}

float getDist(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool Particle::shouldDestroy()
{
	if (particles.size() == 0)
	{
		return false;
	}
	if (hasExploded && (getDist(particles[0].getPosition(), spawning_origin) > explostion_radius))
	{
		return true;
	}
	return false;
}


void Particle::updateDirection(float input)
{
	direction = input;
}

float in_range(float a, float b, float target)
{
	target = std::max(a, std::min(target, b));
	return target;
}
void Particle::update()
{
	for (int i = 0; i < particles.size(); i++)
	{
		mag_and_dire[i].mag += decceleration;
		particles[i].move(mag_and_dire[i].mag * cos(mag_and_dire[i].direction), mag_and_dire[i].mag * sin(mag_and_dire[i].direction));
		sf::Color temp = particles[i].getFillColor();
		temp.r += dr;
		temp.g += dg;
		temp.b += db;
		temp.r = in_range(0, 255, temp.r);
		temp.g = in_range(0, 255, temp.g);
		temp.b = in_range(0, 255, temp.b);

		if (temp == sf::Color(255, 255, 255))
		{
			dr = -30;
			dg = -30;
			db = -30;
		}
		if (temp == sf::Color::Black || mag_and_dire[i].mag < 0)
		{
			particles.erase(particles.begin() + i);
			mag_and_dire.erase(mag_and_dire.begin() + i);
			i--;
		}
		else
		{
			particles[i].setFillColor(temp);
		}
	}
}

void Particle::draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		window->draw(particles[i]);
	}
}

int Particle::randomNumber(int a, int b)
{

	int something = rand() % (b - a);
	something += a;
	return something;
}

float Particle::randomNumber(float a, float b)
{
	float r3 = a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
	return r3;
}
