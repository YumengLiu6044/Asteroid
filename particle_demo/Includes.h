#pragma once
#ifndef INCLUDES_H
#define INCLUDES_H
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "Particle.h"
#include <algorithm>
#include <random>
#include <time.h>
#include <string>
	
//game settings
#define INCLUDES_H
#define window_width 800
#define window_height 600
#define frame_rate 60

//asteroid settings
#define average_asteroid_radius 90
#define asteroid_radius_offset 10
#define average_asteroid_sides 12
#define average_asteroid_rotation 1.0f
#define asteroid_rotation_offset 0.5f
#define asteroid_sides_offset 2
#define average_asteroid_velocity 2.0f
#define asteroid_velocity_offset 0.3f
#define asteroid_separation_angle pi / 6.0f //in radian
#define asteroid_onscreen 6


//player settings
#define player_speed_threshhold 6.5f
#define player_acceleration 0.1f
#define player_decceleration -0.07f
#define pi 3.14159265357f
#define degree pi / 180.0f
#define white sf::Color(255, 255, 255)
#define max_thrusting_time 4

// bullet settings
#define bullet_velo 7.0f
#define bullet_size 1.6f
#define bullet_limit 6


// score setting
#define per_step 10


// particle setting
#define intensity 50
#define explosion_intensity 40


using std::vector;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::sort;

struct MagAndDirection
{
	float mag;
	float direction = 0.0f;
};
 
#endif // !INCLUDES_H

