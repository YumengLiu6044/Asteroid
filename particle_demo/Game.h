#pragma once
#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "Asteroid.h"
#include "Includes.h"

class Game
{
public:
	Game();
	void run();
private:
	void update();
	void processEvents();
	void render();
	void processKeyEvent(sf::Keyboard::Key key, bool pressed);
	void generateAsteroid();
	int randomNumber(int a, int b);
	float randomNumber(float a, float b);
	void intersections();
	void separateAsteroid(int index);
	void initialize_texts();
	void titleScreen();
	void endGame();
	void transitionScreen();
	void setTextCenter(sf::Text& text);

	sf::ContextSettings setting;
	sf::RenderWindow mWindow;
	sf::Font vector_font;
	sf::Text score_text, life_text, title_text, menu, game_over_text;
	Player* mPlayer;
	vector<Asteroid> asteroids;
	int current_frame = 0;
	float* chances[4] = {};
	int chance_of_spawn = 60;
	sf::ConvexShape left_arrow;
	sf::ConvexShape right_arrow;
	Particle* tail;
	vector<Particle> explosions;
	sf::SoundBuffer explosion_buffer;
	sf::Sound explosion_sound;

	int ax = -1 * (int)(window_width * 0.125);
	int bx = window_width - ax;
	int ay = -1 * (int)(window_height * 0.125);
	int by = window_height - ay;
	float top_left, top_right, bottom_left, bottom_right;
	float offset = 30.0f;
	int previous = 0;
	int first = 0;
	int score = 0;
	int frame = 0;
	int highest_score = 0;
	float color_increment = 0;
	bool has_input = false;
	std::string game_event = "title";
};

#endif