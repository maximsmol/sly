#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <SDL.h>

#include "SLY.hpp"

const int FPS = 30;
constexpr double deltaT = 1000/FPS;
constexpr double floatDelta = 0.000000000000001;

constexpr double walkingSpeed = 0.3;
constexpr double jumpSpeed = 1;

constexpr double gravitation = jumpSpeed/300;

constexpr double floorSlipperiness = 30;
constexpr double floorFriction = walkingSpeed/(floorSlipperiness+1);


#define isDoubleZero(val) \
	((0 <= (val) && (val) <= floatDelta) || (floatDelta <= (val) && (val) <= 0))
#define scastInt(val) static_cast<int>(val)

int main()
{
	sly::base::init("SLY Test app", 800, 600);

	sly::image::Image img("../res/mario.png");
	sly::sound::SoundEffect jump("../res/jump.wav", 0);

	double playerX = 0;
	double playerY = 0;
	double playerVelocityX = 0;
	double playerVelocityY = 0;

	double maxXVelocity = walkingSpeed+floorSlipperiness;

	SDL_Event event;
	bool running = true;
	bool moveKeys[4] = {};
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
			else if (event.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = event.key.keysym.sym;
				if (key == SDLK_a)
				{
					img.setHorizontalFlip(true);
					moveKeys[0] = true;
				}
				else if (key == SDLK_SPACE) moveKeys[1] = true;
				else if (key == SDLK_d)
				{
					img.setHorizontalFlip(false);
					moveKeys[2] = true;
				}
				else if (key == SDLK_s) moveKeys[3] = true;

				else if (key == SDLK_ESCAPE) running = false;
			}
			else if (event.type == SDL_KEYUP)
			{
				SDL_Keycode key = event.key.keysym.sym;
				if      (key == SDLK_a    ) moveKeys[0] = false;
				else if (key == SDLK_SPACE) moveKeys[1] = false;
				else if (key == SDLK_d    ) moveKeys[2] = false;
				else if (key == SDLK_s    ) moveKeys[3] = false;
			}
		}

		if (moveKeys[0])
		{
			playerVelocityX -= walkingSpeed;
			if (playerVelocityX < -maxXVelocity)
				playerVelocityX = -maxXVelocity;
		}
		if (moveKeys[1] && isDoubleZero(playerY))
		{
			playerVelocityY = jumpSpeed;
			if (jump.isChannelAvailable()) jump.play();
		}
		if (moveKeys[2])
		{
			playerVelocityX += walkingSpeed;
			if (playerVelocityX > maxXVelocity)
				playerVelocityX = maxXVelocity;
		}

		playerX += playerVelocityX*deltaT;
		playerY += playerVelocityY*deltaT;
		if (playerY < floatDelta) playerY = 0;

		if (!isDoubleZero(playerVelocityX))
		{
			if (playerVelocityX > floatDelta)
			{
				playerVelocityX -= floorFriction*deltaT;
				if (playerVelocityX < -floatDelta) playerVelocityX = 0;
			}
			else
			{
				playerVelocityX += floorFriction*deltaT;
				if (playerVelocityX > floatDelta) playerVelocityX = 0;
			}

		}

		if (playerY > floatDelta) playerVelocityY -= gravitation*deltaT;
		else playerVelocityY = 0;


		SDL_RenderClear(sly::base::renderer);

		img.render(scastInt(playerX)+400, 300-scastInt(playerY), 48, 64);

		sly::base::render();


		SDL_Delay(1000/FPS);
	}

	return 0;
}
