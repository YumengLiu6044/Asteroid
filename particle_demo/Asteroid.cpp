#include "Asteroid.h"

Asteroid::Asteroid(sf::Vector2f position, float velocity, float velo_direction, float in_direction_offset, sf::RenderWindow &in_window)
{
	this->velo_direction = velo_direction;
	this->velo_mag = velocity;
	window = &in_window;
	dire_offset = in_direction_offset;
	global_center_coordinate = position;
	dx = cos(velo_direction) * velo_mag;
	dy = sin(velo_direction) * velo_mag;
	cos_of_offset = cos(dire_offset);
	sin_of_offset = sin(dire_offset);
}

Asteroid::Asteroid(sf::Vector2f position, float velo_direction, sf::RenderWindow& in_window)
{
	this->velo_direction = velo_direction;
	global_center_coordinate = position;
	//generate points
	int points = randomNumber(average_asteroid_sides - asteroid_sides_offset, average_asteroid_sides + asteroid_sides_offset);
	for (int i = 0; i < points; i++)
	{
		this->addPoint(
			randomNumber(average_asteroid_radius - asteroid_radius_offset, average_asteroid_radius + asteroid_radius_offset),
			randomNumber(0, 360) * degree
		);
	}
	this->sortPairs();
	//generate rotation offset and direction velocity mag
	dire_offset = randomNumber(average_asteroid_rotation - asteroid_rotation_offset, average_asteroid_rotation + asteroid_rotation_offset) * degree;
	if (rand() % 2 == 1)
	{
		dire_offset *= -1;
	}
	velo_mag = randomNumber(average_asteroid_velocity - asteroid_velocity_offset, average_asteroid_velocity + asteroid_velocity_offset);
	//window and other stuff
	window = &in_window;
	dx = cos(velo_direction) * velo_mag;
	dy = sin(velo_direction) * velo_mag;
	cos_of_offset = cos(dire_offset);
	sin_of_offset = sin(dire_offset);
}

void Asteroid::addPoint(float mag, float in_direction)
{
	MagAndDirection stuff;
	stuff.mag = mag;
	stuff.direction = in_direction;
	mag_and_dire.push_back(stuff);
}

void Asteroid::sortPairs()
{
	if (mag_and_dire.size() != 0)
	{
		sort(mag_and_dire.begin(), mag_and_dire.end(), [](MagAndDirection a, MagAndDirection b)
			{
				return a.direction < b.direction;
			});
	}

	lines.clear();
	for (int i = 0; i < mag_and_dire.size(); i++)
	{
		float x = cos(mag_and_dire[i].direction) * mag_and_dire[i].mag + global_center_coordinate.x;
		float y = sin(mag_and_dire[i].direction) * mag_and_dire[i].mag + global_center_coordinate.y;

		lines.push_back(sf::Vertex(sf::Vector2f(x, y)));
	}
	mag_and_dire.push_back(mag_and_dire[0]);
	lines.push_back(lines[0]);
}

void Asteroid::update()
{
	for (int i = 0; i < lines.size(); i++)
	{
		lines[i].position.x -= global_center_coordinate.x;
		lines[i].position.y -= global_center_coordinate.y;
		lines[i].position.x = lines[i].position.x * cos_of_offset - lines[i].position.y * sin_of_offset;
		lines[i].position.y = lines[i].position.x * sin_of_offset + lines[i].position.y * cos_of_offset;
		lines[i].position.x += (global_center_coordinate.x + dx);
		lines[i].position.y += (global_center_coordinate.y + dy);

	}
	global_center_coordinate.x += dx;
	global_center_coordinate.y += dy;
}

void Asteroid::draw()
{
	if (lines.size() > 0)
	{
		window->draw(&lines[0], lines.size(), sf::LinesStrip);
	}
	current_frame++;
}

void Asteroid::setDirectionOffset(float in_direction_offset)
{
	dire_offset = in_direction_offset;
}

float Asteroid::getVeloDire()
{
	return velo_direction;
}

bool Asteroid::shouldDestroy()
{
	if (have_entered)
	{
		if (global_center_coordinate.x < (-1 * (average_asteroid_radius + asteroid_radius_offset)) || global_center_coordinate.x > (window_width + average_asteroid_radius + asteroid_radius_offset))
		{
			return true;
		}
		if (global_center_coordinate.y < -1 * (average_asteroid_radius + asteroid_radius_offset) || global_center_coordinate.y > window_height + average_asteroid_radius + asteroid_radius_offset)
		{
			return true;
		}
		if (size < 1)
		{
			return true;
		}
		if (current_frame > 1000)
		{
			return true;
		}
	}
	else
	{
		if (global_center_coordinate.x > 0 && global_center_coordinate.x < window_width)
		{
			if (global_center_coordinate.y > 0 && global_center_coordinate.y < window_height)
			{
				have_entered = true;
			}
		}
	}
	return false;
}

int Asteroid::randomNumber(int a, int b)
{

	int something = rand() % (b - a);
	something += a;
	return something;
}

float Asteroid::randomNumber(float a, float b)
{
	float r3 = a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
	return r3;
}

float Asteroid::getArea(sf::Vertex A, sf::Vertex B, sf::Vertex C)
{
	//  Ax(By - Cy) + Bx(Cy - Ay) + Cx(Ay - By) divided by 2
	return abs(A.position.x * (B.position.y - C.position.y) + B.position.x * (C.position.y - A.position.y) + C.position.x * (A.position.y - B.position.y)) / 2.0f;
}

bool Asteroid::check_intersection(sf::Vector2f point)
{
	for (int i = 0; i < lines.size() - 1; i++)
	{
		if (lines[i].position == lines[i + 1].position)
		{
			continue;
		}
		if (inTriangle(point, lines[i], lines[i + 1], sf::Vertex(global_center_coordinate)))
		{
			return true;
		}
	}
	return false;
}
void Asteroid::updateVelocityMagAndDirection()
{
	dx = cos(velo_direction) * velo_mag;
	dy = sin(velo_direction) * velo_mag;
}
void Asteroid::resetFrame()
{
	current_frame = 0;
}
bool Asteroid::inTriangle(sf::Vector2f pointy, sf::Vertex a, sf::Vertex b, sf::Vertex c)
{
	float big_s = 0.f;

	big_s += getArea(pointy, a, b);
	big_s += getArea(pointy, b, c);
	big_s += getArea(pointy, a, c);
	float s = getArea(a, b, c);
	if ((int)big_s == (int)s)
	{
		return true;
	}
	return false;
}


