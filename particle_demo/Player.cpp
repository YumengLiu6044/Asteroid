#include "Player.h"
Player::Player(sf::RenderWindow &in_window)
{
	window = &in_window;
	left_arrow = new sf::ConvexShape(3);
	right_arrow = new sf::ConvexShape(3);
	left_arrow->setPoint(0, sf::Vector2f(0.0f, -8.0f));
	left_arrow->setPoint(1, sf::Vector2f(0.0f, 4.0f));
	left_arrow->setPoint(2, sf::Vector2f(-5.0f, 7.0f));
	right_arrow->setPoint(0, sf::Vector2f(0.0f, -8.0f));
	right_arrow->setPoint(1, sf::Vector2f(0.0f, 4.0f));
	right_arrow->setPoint(2, sf::Vector2f(5.0f, 7.0f));
	left_arrow->setFillColor(sf::Color(255, 255, 255));
	right_arrow->setFillColor(sf::Color(255, 255, 255));

	if (!fire_buffer.loadFromFile("fire.wav"))
	{
		cout << "couldn't load from fire.wav" << endl;
	}
	if (!thrust_buffer.loadFromFile("thrust.wav"))
	{
		cout << "couldn't load from thrust.wav" << endl;
	}
	fire_sound.setBuffer(fire_buffer);
	thrust_sound.setBuffer(thrust_buffer);
}

void Player::update()
{
	// update velocity
	if (going)
	{
		if (velo_mag <= player_speed_threshhold)
		{
			velo_mag += acceleration;
		}

	}
	else
	{
		if (velo_mag > 0.0f)
		{
			velo_mag += decceleration;
		}
		else
		{
			velo_mag = 0.0f;
		}
		
		
	}
	//update direction and its offset
	if (turning)
	{
		direction += dire_offset;
		left_arrow->rotate(dire_offset * 180.0f / pi);
		right_arrow->rotate(dire_offset * 180.0f / pi);

	}
	//update position
	x += velo_mag * cos(direction);
	y += velo_mag * sin(direction);
	if (x - 800.0f > 0.0f)
	{
		x = x - 800.0f;
	}
	else if (x < 0.0f)
	{
		x = 800.0f + x;
	}
	if (y - 600.0f > 0.0f)
	{
		y = y - 600.0f;
	}
	else if (y < 0.0f)
	{
		y = 600.0f + y;
	}
	left_arrow->setPosition(x, y);
	right_arrow->setPosition(x, y);
	//update bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i].update();
		if (bullets[i].shouldDestroy())
		{
			bullets.erase(bullets.begin() + i);
		}
	}

}

void Player::dead()
{
	if (!is_dead)
	{
		life--;
	}
	is_dead = true;
	x = window_width / 2.0f;
	y = window_height / 2.0f;
	left_arrow->setPosition(x, y);
	right_arrow->setPosition(x, y);
	bullets.clear();
}

void Player::alive()
{
	is_dead = false;
	velo_mag = 0;
	direction = -1.0f * pi / 2.0f;
	left_arrow->setRotation(0);
	right_arrow->setRotation(0);
}

void Player::draw()
{
	
	window->draw(*left_arrow);
	window->draw(*right_arrow);
	for (int i = 0; i < bullets.size(); i++)
	{
		window->draw(bullets[i]);
	}
}

void Player::redirect(bool turn, std::string direct)
{
	turning = turn;
	if (turning)
	{
		if (direct == "left")
		{
			dire_offset = -1 * abs(dire_offset);
		}
		else
		{
			dire_offset = 1 * abs(dire_offset);
		}
	}
}

void Player::forward(bool go)
{
	going = go;
}

void Player::fire_bullet(bool fire)
{
	if (fire)
	{
		if (fire != space_pressed)
		{
			space_pressed = true;
			if (bullets.size() < bullet_limit)
			{
				Bullet a_bullet(direction, 8.0f * cos(direction) + x, 8.0f * sin(direction) + y);
				bullets.push_back(a_bullet);
			}
			fire_sound.play();
		}
	}
	else
	{
		space_pressed = false;
	}

}

void Player::hyperspace(bool pressed)
{
	if (pressed)
	{
		if (h_pressed != pressed)
		{
			h_pressed = true;
			x = rand() % window_width;
			y = rand() % window_height;
		}
	}
	else
	{
		h_pressed = false;
	}
}

vector<sf::Vector2f> Player::getPoints()
{
	points.clear();
	points.push_back(sf::Vector2f(cos(direction) * 8.0f, sin(direction) * 8.0f));
	points.push_back(sf::Vector2f(cos(direction + 2.5213f) * 8.6f, sin(direction + 2.5213f) * 8.6f));
	points.push_back(sf::Vector2f(cos(direction - 2.5213f) * 8.6f, sin(direction - 2.5213f) * 8.6f));
	return points;
}

sf::Vector2f Player::getPosition()
{
	return sf::Vector2f(x, y);
}
