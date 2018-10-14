#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_mixer.h>

#include "SLY.hpp"

namespace sly
{
	namespace sound
	{
		Music::Music(const std::string& path) : Music(path.c_str()) {}
		Music::Music(const char* path) :
			data_(Mix_LoadMUS(path))
		{
			assert(sly::base::inited);
			assert(ok());
		}

		Music::~Music()
		{
			Mix_FreeMusic(data_);
		}

		/*inline*/ bool Music::ok() const
		{
			return data_ != nullptr;
		}


		/*inline*/ Mix_MusicType Music::getType() const
		{
			assert(ok());
			return Mix_GetMusicType(getData());
		}


		/*inline*/ void Music::play(int loops/* = 0*/) const
		{
			assert(ok());
			sly_assertEqual(Mix_PlayMusic(getData(), loops), 0);
		}

		/*inline*/ void Music::fadein(int ms/* = 0*/, int loops/* = 0*/) const
		{
			assert(ok());
			sly_assertEqual(Mix_FadeInMusic(getData(), loops, ms), 0);
		}

		/*inline*/ void Music::fadeinFrom(
			double pos/* = 0*/,
			int ms/* = 0*/,
			int loops/* = 0*/
		) const
		{
			assert(ok());
			sly_assertEqual(Mix_FadeInMusicPos(getData(), loops, ms, pos), 0);
		}


		/*inline*/ void Music::setData(Mix_Music* data)
		{
			assert(ok());
			data_ = data;
			assert(ok());
		}

		/*inline*/ Mix_Music* Music::getData() const
		{
			assert(ok());
			return data_;
		}
	}
}
