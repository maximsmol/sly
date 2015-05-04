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

constexpr double gravitation = jumpSpeed/(FPS*10);

constexpr double floorSlipperiness = FPS;
constexpr double floorFriction = walkingSpeed/(floorSlipperiness+1);


#define isDoubleZero(val) \
	((0 <= (val) && (val) <= floatDelta) || (floatDelta <= (val) && (val) <= 0))
#define scastInt(val) static_cast<int>(val)

int main()
{
	sly::base::init("SLY Test app", 800, 600);

	SDL_Rect rect({});

	sly::image::Image img("../res/SMB.png");
	rect.x = 0;
	rect.y = 32;
	rect.w = 16;
	rect.h = 16;
	img.setSourceRect(rect);

	sly::image::Image deadImg("../res/SMB.png");
	rect.x = 96;
	rect.y = 32;
	rect.w = 16;
	rect.h = 16;
	deadImg.setSourceRect(rect);

	sly::image::Image marioSpritesheet("../res/SMB.png");
	sly::image::Animation mario(&marioSpritesheet, 6, 3, 16, 16, 16, 32);

	sly::image::Animation sMario(&marioSpritesheet, 6, 3, 16, 32, 16, 0);
	sly::image::Animation* playerSprite = &mario;

	sly::sound::SoundEffect jump("../res/jump.wav", 0);
	sly::sound::Music theme("../res/overworld.ogg");
	sly::sound::Music died("../res/died.ogg");
	theme.play(-1);

	double playerX = 0;
	double playerY = 0;
	double playerVelocityX = 0;
	double playerVelocityY = 0;
	int playerH = 62;

	double maxXVelocity = walkingSpeed+floorSlipperiness;

	SDL_Event event;
	int skipPhysTicks = 0;
	int animationTicks = 0;
	bool running = true;
	bool dead = false;
	bool moveKeys[4] = {};
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
			else if (event.type == SDL_KEYDOWN)
			{
				SDL_Keycode key = event.key.keysym.sym;
				if (!dead)
				{
					if (!moveKeys[0] && key == SDLK_a)
					{
						marioSpritesheet.setHorizontalFlip(true);
						playerSprite->setFrame(3);
						moveKeys[0] = true;
					}
					else if (!moveKeys[1] && key == SDLK_SPACE)
						moveKeys[1] = true;
					else if (!moveKeys[2] && key == SDLK_d)
					{
						marioSpritesheet.setHorizontalFlip(false);
						playerSprite->setFrame(3);
						moveKeys[2] = true;
					}
					else if (!moveKeys[3] && key == SDLK_s) moveKeys[3] = true;

					else if (key == SDLK_t)
					{
						playerSprite = &sMario;
						playerH = 128;
						playerY += 62;
					}
					else if (key == SDLK_y)
					{
						playerSprite = &mario;
						playerH = 62;
						playerY -= 62;
					}
				}

				if (key == SDLK_ESCAPE) running = false;
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

		if (skipPhysTicks > 0) skipPhysTicks--;
		if (skipPhysTicks == 0)
		{
			if (!dead)
			{
				if (moveKeys[0])
				{
					if (animationTicks % (FPS/6) == 0)
						playerSprite->nextFrame();

					playerVelocityX -= walkingSpeed;
					if (playerVelocityX < -maxXVelocity)
						playerVelocityX = -maxXVelocity;
				}
				if (moveKeys[1] && (playerY == playerH))
				{
					playerVelocityY = jumpSpeed;
					if (jump.isChannelAvailable()) jump.play();
				}
				if (moveKeys[2])
				{
					if (animationTicks % (FPS/6) == 0)
						playerSprite->nextFrame();

					playerVelocityX += walkingSpeed;
					if (playerVelocityX > maxXVelocity)
						playerVelocityX = maxXVelocity;
				}

				if (!moveKeys[0] && !moveKeys[2]) playerSprite->setFrame(-1);
			}

			playerX += playerVelocityX*deltaT;
			playerY += playerVelocityY*deltaT;
			if (!dead && playerY < playerH) playerY = playerH;

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

			if (dead || playerY > playerH)
				playerVelocityY -= gravitation*deltaT;
			else playerVelocityY = 0;

			if (!dead && (playerX > 336 || playerX < -400 || playerY > 300))
			{
				Mix_HaltMusic();
				died.play();
				dead = true;
				skipPhysTicks = 18;

				playerSprite = &mario;
				playerH = 62;
				playerSprite->setFrame(5);

				playerVelocityX = 0;
				playerVelocityY = jumpSpeed/2;

				if (playerX < -400) playerX = -400;
				if (playerX > 336) playerX = 336;
				if (playerY > 300) playerY = 300;
			}

			if (playerY > 300) playerY = 300;
			if (dead && playerY < -300)
			{
				running = false;
				playerY = -300;
			}
		}

		if (!dead && playerY > playerH)
			playerSprite->setFrame(4);

		SDL_RenderClear(sly::base::renderer);

		/*if (!dead)
			img.render(scastInt(playerX)+400, 300-scastInt(playerY), 64, 64);
		else
			deadImg.render(
				scastInt(playerX)+400,
				300-scastInt(playerY),
				64, 64);*/
		playerSprite->render(
			scastInt(playerX)+400, 300-scastInt(playerY), 64, playerH
		);

		sly::base::render();

		SDL_Delay(1000/FPS);
		animationTicks++;
	}

	running = dead;
	while (running && Mix_PlayingMusic() == 1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
			else if (event.type == SDL_KEYDOWN &&
					 event.key.keysym.sym == SDLK_ESCAPE) running = false;
		}
	}

	return 0;
}
