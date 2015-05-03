#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <SDL.h>

#include "SLY.hpp"

int main()
{
	sly::base::init("test", 800, 600);

	sly::image::Image img("../res/mario.png");
	img.render(0, 0, 48, 64);

	sly::base::render();

	SDL_Event event;
	bool running = true;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) running = false;
		}
	}

	return 0;
}
