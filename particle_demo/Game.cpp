#include "Game.h"

Game::Game() :
	setting(0, 0, 2),
	mWindow(sf::VideoMode(window_width, window_height), "Asteroid", 7U, setting)
{
	srand(time(NULL));
	chances[0] = &top_left;
	chances[1] = &top_right;
	chances[2] = &bottom_right;
	chances[3] = &bottom_left;
	for (int i = 0; i < 4; i++)
	{
		*chances[i] = 25.5f;
	}
	*chances[0] = 0;
	mWindow.setFramerateLimit(frame_rate);
	mPlayer = new Player(mWindow);
	left_arrow = *(mPlayer->left_arrow);
	right_arrow = *(mPlayer->right_arrow);
	for (int i = 0; i < 3; i++)
	{
		sf::Vector2f leftpoint = left_arrow.getPoint(i), 
			rightpoint = right_arrow.getPoint(i);
		left_arrow.setPoint(i, sf::Vector2f(leftpoint.x * 2, leftpoint.y * 2));
		right_arrow.setPoint(i, sf::Vector2f(rightpoint.x * 2, rightpoint.y * 2));

	}
	left_arrow.setOrigin(-10, -16);
	right_arrow.setOrigin(0, -16);
	left_arrow.setFillColor(sf::Color(150, 150, 150));
	right_arrow.setFillColor(sf::Color(150, 150, 150));

	initialize_texts();
	tail = new Particle(pi / 2.0f, pi / 6.0f, sf::Vector2f(0, 0), mWindow);
	std::ifstream file("highest_score.txt", std::ios::in);
	file >> highest_score;
	file.close();

	if (!explosion_buffer.loadFromFile("bangMedium.wav"))
	{
		cout << "Coudln't load from bangMedium.wav" << endl;
	}
	explosion_sound.setBuffer(explosion_buffer);
}

void Game::run()
{
	while (mWindow.isOpen())
	{
		if (game_event == "title")
		{
			titleScreen();
		}
		else if (game_event == "game_on")
		{
			if (mPlayer->life == 0)
			{
				game_event = "transition";
				frame = 0;
			}
			update();
			processEvents();
			render();
			// 2. make particle effect
			// 3. make background
			// 4. add music

		}
		else if (game_event == "transition")
		{
			transitionScreen();
		}
		else
		{
			endGame();
		}
	}
}

void Game::update()
{
	score_text.setString("Highest score: " + std::to_string(highest_score) + "\tscore: " + std::to_string(score));
	int something = rand() % 10000;
	if (!mPlayer->is_dead)
	{
		if (something < chance_of_spawn)
		{
			generateAsteroid();
		}
		mPlayer->update();
	}
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i].update();
		if (asteroids[i].shouldDestroy())
		{
			asteroids.erase(asteroids.begin() + i);
			i--;
		}
	}
	sf::Vector2f vec = sf::Vector2f(cos(mPlayer->direction + pi) * 4.0f, sin(mPlayer->direction + pi) * 4.0f);
	sf::Vector2f playcord = mPlayer->getPosition();
	tail->setPosition(sf::Vector2f(playcord.x + vec.x, playcord.y + vec.y));
	tail->updateDirection(mPlayer->direction + pi);
	tail->update();
	if (tail->on)
	{
		tail->shoot();
	}
	for (int i = 0; i < explosions.size(); i++)
	{
		explosions[i].update();
		if (explosions[i].shouldDestroy())
		{
			explosions.erase(explosions.begin() + i);
			i--;
		}
	}

	this->intersections();
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mWindow.close();
			break;
		case sf::Event::KeyPressed:
			processKeyEvent(event.key.code, true);
			has_input = true;
			break;
		case sf::Event::KeyReleased:
			processKeyEvent(event.key.code, false);
			has_input = false;
			break;

		}
	}
}

void Game::render()
{
	mWindow.clear(sf::Color(0, 0, 0));
	frame++;
	frame = frame % frame_rate;
	if (!mPlayer->is_dead)
	{
		mPlayer->draw();
		frame = 0;
	}
	else
	{
		if (frame < 40 && mPlayer->life != 0)
		{
			mWindow.draw(menu);

		}
	}
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i].draw();
	}
	mWindow.draw(score_text);
	mWindow.draw(life_text);
	left_arrow.setPosition(life_text.getPosition().x, life_text.getPosition().y + 30);
	right_arrow.setPosition(life_text.getPosition().x + 10, life_text.getPosition().y + 30);

	for (int i = 0; i < mPlayer->life; i++)
	{
		mWindow.draw(left_arrow);
		mWindow.draw(right_arrow);
		left_arrow.move(25, 0);
		right_arrow.move(25, 0);
	}
	tail->draw();
	for (int i = 0; i < explosions.size(); i++)
	{
		explosions[i].draw();
	}
	mWindow.display();
}

void Game::processKeyEvent(sf::Keyboard::Key key, bool pressed)
{
	if (mPlayer->is_dead)
	{
		if (!has_input && key == sf::Keyboard::B)
		{
			mPlayer->alive();
		}
		tail->emit(false);
	}
	if (key == sf::Keyboard::W)
	{
		mPlayer->forward(pressed);
		if (!mPlayer->is_dead)
		{

			tail->emit(pressed);

		}
	}
	else if (key == sf::Keyboard::A)
	{
		mPlayer->redirect(pressed, "left");
	}
	else if (key == sf::Keyboard::D)
	{
		mPlayer->redirect(pressed, "right");
	}
	else if (key == sf::Keyboard::Space)
	{
		mPlayer->fire_bullet(pressed);
	}
	else if (key == sf::Keyboard::H)
	{
		mPlayer->hyperspace(pressed);
	}
}

void Game::generateAsteroid()
{
	sf::Vector2f global_center_coordinate;
	float velo_direction = 0.0f;
	while (first == previous)
	{
		first = rand() % 4;
	}
	switch (first)
	{
	case 0:
		global_center_coordinate.x = ax;
		global_center_coordinate.y = ay;
		velo_direction = randomNumber(45.0f - offset, 45.0f + offset) * degree;
		break;
	case 1:
		global_center_coordinate.x = bx;
		global_center_coordinate.y = ay;
		velo_direction = randomNumber(45.0f + 90.0f - offset, 45.0f + 90.0f + offset);
		break;
	case 2:
		global_center_coordinate.x = bx;
		global_center_coordinate.y = by;
		velo_direction = randomNumber(225.0f - offset, 225.0f + offset) * degree;
		break;
	case 3:
		global_center_coordinate.x = ax;
		global_center_coordinate.y = by;
		velo_direction = randomNumber(315.0f - offset, 315.0f + offset) * degree;
		break;

	}
	previous = first;
	Asteroid another(global_center_coordinate, velo_direction, mWindow);
	asteroids.push_back(another);
}
int Game::randomNumber(int a, int b)
{

	int something = rand() % (b - a);
	something += a;
	return something;
}

float Game::randomNumber(float a, float b)
{
	float r3 = a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
	return r3;
}

void Game::intersections()
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		for (int j = 0; j < mPlayer->bullets.size(); j++)
		{
			if (asteroids[i].check_intersection(mPlayer->bullets[j].getPosition()))
			{
				mPlayer->bullets.erase(mPlayer->bullets.begin() + j);
				score += (4 - asteroids[i].size) * per_step;
				Particle exp(0.0f, pi, asteroids[i].global_center_coordinate, mWindow);
				exp.explostion_radius = (asteroids[i].size) * 50;
				exp.explode();
				explosions.push_back(exp);
				separateAsteroid(i);
				explosion_sound.play();
				break;
			}

		}
	}
	if (!mPlayer->is_dead)
	{
		for (int i = 0; i < asteroids.size(); i++)
		{
			for (auto k : mPlayer->getPoints())
			{
				if (asteroids[i].check_intersection(k + mPlayer->getPosition()))
				{
					Particle exp(0.0f, pi, mPlayer->getPosition(), mWindow);
					exp.explostion_radius = 100;
					exp.explode();
					explosions.push_back(exp);
					explosion_sound.play();

					mPlayer->dead();
					tail->emit(false);
				}
			}

		}

	}
}

// asteroids erase process has bug
void Game::separateAsteroid(int index)
{
	asteroids[index].size--;
	Asteroid something = asteroids[index];
	something.resetFrame();
	for (int i = 0; i < something.mag_and_dire.size(); i++)
	{
		something.mag_and_dire[i].mag = something.mag_and_dire[i].mag / 2.0f;
	}
	something.sortPairs();
	something.velo_mag += 1.0f;
	if (something.size > 0)
	{
		something.velo_direction -= asteroid_separation_angle;
		something.updateVelocityMagAndDirection();
		asteroids[index] = something;

		something.velo_direction += 2.0 * asteroid_separation_angle;
		something.updateVelocityMagAndDirection();
		asteroids.push_back(something);
	}
	else
	{
		asteroids.erase(asteroids.begin() + index);
	}
	return;
}

void Game::initialize_texts()
{
	if (!vector_font.loadFromFile("vector.ttf"))
	{
		cout << "Con't load file" << endl;
		return;
	}
	score_text.setFont(vector_font);
	score_text.setString("score: " + score);
	score_text.setFillColor(sf::Color::Black);
	score_text.setOutlineColor(sf::Color::White);
	score_text.setOutlineThickness(0.5f);
	score_text.setCharacterSize(20);
	score_text.setPosition(230, 10);


	life_text = score_text;
	life_text.setString("life: ");
	life_text.setPosition(60, 10);


	title_text.setFont(vector_font);
	title_text.setString("ASTEROID");
	title_text.setFillColor(sf::Color::Black);
	title_text.setOutlineColor(sf::Color::White);
	title_text.setOutlineThickness(1.2);
	title_text.setCharacterSize(80);
	title_text.setLetterSpacing(2);
	title_text.setStyle(sf::Text::Bold);
	setTextCenter(title_text);


	menu.setFont(vector_font);
	menu.setString("press any key");
	menu.setFillColor(sf::Color(150, 150, 150));
	menu.setOutlineColor(sf::Color(150, 150, 150));
	menu.setOutlineThickness(0.2);
	menu.setCharacterSize(30);
	setTextCenter(menu);
	title_text.setPosition(400, 300 - 70);
	menu.setPosition(400, 300 + 60);

	game_over_text = title_text;
	game_over_text.setString("GAME OVER");
	setTextCenter(game_over_text);
	game_over_text.setPosition(title_text.getPosition());
	game_over_text.setOutlineColor(sf::Color::Black);
}

void Game::titleScreen()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) { 
			mWindow.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			game_event = "game_on";
			menu.setLetterSpacing(1.2);
			menu.setString("Press B for \nBack to life");
			menu.setPosition(400, 300);
		}
	}

	mWindow.clear(sf::Color::Black);
	mWindow.draw(title_text);
	if (frame < 40)
	{
		mWindow.draw(menu);
		
	}
	mWindow.display();
	frame++;
	frame = frame % frame_rate;
}

void Game::endGame()
{
	menu.setString("Why are you still here?\n       get out!");
	setTextCenter(menu);
	menu.setPosition(400, 360);
	std::ofstream file("highest_score.txt", std::ios::out);
	file << std::max(score, highest_score);
	file.close();
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}
	if (game_over_text.getOutlineColor().r < 230)
	{
		game_over_text.setOutlineColor(sf::Color((int)color_increment, (int)color_increment, (int)color_increment));
	}
	mWindow.clear(sf::Color::Black);
	mWindow.draw(game_over_text);
	if (frame / frame_rate > 10)
	{
		mWindow.draw(menu);

	}
	mWindow.display();
	frame++;
	color_increment += 0.5;
}

void Game::transitionScreen()
{
	mWindow.clear(sf::Color::Black);
	int color = 0;
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i].update();
		for (int j = 0; j < asteroids[i].lines.size(); j++)
		{
			color = asteroids[i].lines[j].color.r;
			color--;
			if (color > 0)
			{
				asteroids[i].lines[j].color = sf::Color(color, color, color);
			}
			else
			{
				game_event = "game_over";
			}
		}
		asteroids[i].draw();
	}
	color = life_text.getOutlineColor().r;
	color--;
	if (color > 0)
	{
		life_text.setOutlineColor(sf::Color(color, color, color));
		score_text.setOutlineColor(sf::Color(color, color, color));
	}
	else
	{
		game_event = "game_over";
	}
	mWindow.draw(life_text);
	mWindow.draw(score_text);
	mWindow.display();


}

void Game::setTextCenter(sf::Text& text)
{
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
}
