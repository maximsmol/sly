#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "SLY.hpp"

namespace sly
{
	namespace base
	{
		SDL_Window*   window   = nullptr;
		SDL_Renderer* renderer = nullptr;
		SDL_Texture*  buffer   = nullptr;

		void init(const char* winName, int w, int h)
		{
			assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

			window =
				SDL_CreateWindow(
					winName,
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					w, h,
					0
				);
			assert(window != nullptr);

			//
			// Create the renderer

			/*
				Renderer type priority list:
				1. Hardware + VSYNC
				2. Hardware
				3. Software (no VSYNC possible)
				-- FAIL --
			*/
			renderer =
				SDL_CreateRenderer(
					window, -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
					SDL_RENDERER_TARGETTEXTURE
				);
			if (renderer == nullptr)
				renderer =
					SDL_CreateRenderer(
						window, -1,
						SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
					);
			if (renderer == nullptr)
				renderer =
					SDL_CreateRenderer(
						window, -1,
						SDL_RENDERER_TARGETTEXTURE
					);

			assert(renderer != nullptr);


			//
			// Triple buffering (for developer's convenience)
			buffer =
				SDL_CreateTexture(
					renderer, SDL_PIXELFORMAT_ARGB8888,
					SDL_TEXTUREACCESS_TARGET,
					w, h
				);
			assert(buffer != nullptr);


			assert(SDL_SetRenderTarget(renderer, buffer) == 0);

			atexit(quit);
		}

		void quit()
		{
			SDL_DestroyTexture(buffer);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
	}
}
