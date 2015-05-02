#include <cstdio>
#include <cstdlib>
#include <cassert>

#include <SDL.h>

#include "SLY.hpp"

int main()
{
	sly::base::init("test", 800, 600);
	SDL_Delay(3000);

	return 0;
}
