#include <cstdio>
#include <csignal>
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

		bool inited = false;

		void handleSignal(int signal);

		void init(const char* winName, int w, int h)
		{
			sly_assertEqual(SDL_Init(SDL_INIT_EVERYTHING), 0);
			sly_assertEqual(
				IMG_Init(IMG_INIT_PNG),
				(IMG_INIT_PNG)
			);
			sly_assertEqual(
				Mix_Init(MIX_INIT_OGG),
				(MIX_INIT_OGG)
			);

			#if !defined(SLY_NOPEN_AUDIO)
				sly_assertEqual(
					Mix_OpenAudio(22050,
					MIX_DEFAULT_FORMAT,
					2, 2048), 0
				);
			#endif

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


			sly_assertEqual(SDL_SetRenderTarget(renderer, buffer), 0);
			sly_assertEqual(SDL_RenderClear(renderer), 0);

			atexit(quit);

			signal(SIGABRT, handleSignal);
			signal(SIGILL , handleSignal);

			#if !defined(NDEBUG)
				printf("Initializing SLY v%d.%d.%d\n",
					SLY_MAJOR_VERSION, SLY_MINOR_VERSION, SLY_PATCHLEVEL
				);
				printf("SLD2 v%d.%d.%d\n",
					SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL
				);
				printf("SLD2_image v%d.%d.%d\n",
					SDL_IMAGE_MAJOR_VERSION,
					SDL_IMAGE_MINOR_VERSION,
					SDL_IMAGE_PATCHLEVEL
				);
				printf("SLD2_ttf v%d.%d.%d\n",
					TTF_MAJOR_VERSION, TTF_MINOR_VERSION, TTF_PATCHLEVEL
				);
				printf("SLD2_mixer v%d.%d.%d\n",
					MIX_MAJOR_VERSION, MIX_MINOR_VERSION, MIX_PATCHLEVEL
				);
			#endif

			inited = true;
		}

		void render()
		{
			sly_assertEqual(SDL_SetRenderTarget(renderer, nullptr), 0);

			// Wiki says to call it even if overwriting every pixel
			sly_assertEqual(SDL_RenderClear(renderer), 0);
			sly_assertEqual(
				SDL_RenderCopy(renderer, buffer, nullptr, nullptr), 0
			);
			SDL_RenderPresent(renderer);

			sly_assertEqual(SDL_SetRenderTarget(renderer, buffer), 0);
		}

		void quit()
		{
			SDL_DestroyTexture(buffer);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);

			#if !defined(SLY_NOPEN_AUDIO)
				Mix_CloseAudio();
			#endif

			Mix_Quit();
			IMG_Quit();
			SDL_Quit();

			inited = false;
		}

		void handleSignal(int)
		{
			printf("SDL2      : %s\n", SDL_GetError());
			printf("SDL2_TTF  : %s\n", TTF_GetError());
			printf("SDL2_image: %s\n", IMG_GetError());
			printf("SDL2_mixer: %s\n", Mix_GetError());
			quit();
		}
	}
}
