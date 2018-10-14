#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_mixer.h>

#include "SLY.hpp"

namespace sly
{
	namespace sound
	{
		SoundEffect::SoundEffect(
			const std::string& path,
			int selectedChannel/* = -1*/
		) : SoundEffect(path.c_str(), selectedChannel) {}
		SoundEffect::SoundEffect(const char* path,
								 int selectedChannel/* = -1*/
								) :
			selectedChannel_(selectedChannel),
			data_(Mix_LoadWAV(path))
		{
			assert(sly::base::inited);
			assert(ok());
		}

		SoundEffect::~SoundEffect()
		{
			Mix_FreeChunk(getData());
		}

		/*inline*/ bool SoundEffect::ok() const
		{
			return data_ != nullptr &&
				   (selectedChannel_ >= 0 || selectedChannel_ == -1);
		}


		//
		// Channel control
		//

		/*inline*/ int SoundEffect::allocateNewChannel()
		{
			assert(ok());

			sly_assertEqual(Mix_AllocateChannels(1), 1);
			setChannel(Mix_AllocateChannels(-1) - 1); // channelCount - 1

			assert(ok());
			return getChannel();
		}

		/*inline*/ void SoundEffect::pauseChannel() const
		{
			assert(ok());
			assert(getChannel() != -1);

			Mix_Pause(getChannel());
		}

		/*inline*/ void SoundEffect::resumeChannel() const
		{
			assert(ok());
			assert(getChannel() != -1);

			Mix_Resume(getChannel());
		}

		/*inline*/ void SoundEffect::haltChannel() const
		{
			assert(ok());
			assert(getChannel() != -1);

			sly_assertEqual(Mix_HaltChannel(getChannel()), 0);
		}

		/*inline*/ void SoundEffect::expireChannel(int ticks/* = 0*/) const
		{
			assert(ok());
			assert(getChannel() != -1);
			assert(ticks >= 0);

			sly_assertEqual(Mix_ExpireChannel(getChannel(), ticks), 1);
		}

		/*inline*/ void SoundEffect::fadeOutChannel(int ms/* = 0*/) const
		{
			assert(ok());
			assert(getChannel() != -1);
			assert(ms >= 0);

			sly_assertEqual(Mix_FadeOutChannel(getChannel(), ms), 1);
		}

		/*inline*/ int SoundEffect::setChannelVolume(
			int volume/* = MIX_MAX_VOLUME*/
		) const
		{
			assert(getChannel() != -1);
			return Mix_Volume(getChannel(), volume);
		}

		/*inline*/ bool SoundEffect::isChannelAvailable() const
		{
			assert(ok());

			if (getChannel() != -1)
				return !Mix_Playing(getChannel());

			return Mix_Playing(-1) < Mix_AllocateChannels(-1);
		}


		//
		// Players
		//

		/*inline*/ int SoundEffect::play(
			int loops/* = 0*/, int channel/* = -1*/
		) const
		{
			assert(ok());
			assert(loops   >= 0 || loops   ==  0);
			assert(channel >= 0 || channel == -1);
			assert(isChannelAvailable());
			if (!isChannelAvailable()) return -1;

			if (channel == -1 && getChannel() != -1)
				channel = getChannel();

			#if !defined(NDEBUG)
				int res = Mix_PlayChannel(channel, getData(), loops);
				assert(res != -1);
				return res;
			#else
				return Mix_PlayChannel(channel, getData(), loops);
			#endif
		}

		/*inline*/ int SoundEffect::playTimeLimited(
			int ticks /* = -1*/,
			int loops  /* =  0*/,
			int channel/* = -1*/
		) const
		{
			assert(ok());
			assert(ticks   >= 0 || ticks   ==  0);
			assert(loops   >= 0 || loops   ==  0);
			assert(channel >= 0 || channel == -1);
			assert(isChannelAvailable());
			if (!isChannelAvailable()) return -1;

			if (channel == -1 && getChannel() != -1)
				channel = getChannel();

			#if !defined(NDEBUG)
				int res = Mix_PlayChannelTimed(channel, getData(), loops, ticks);
				assert(res != -1);
				return res;
			#else
				return Mix_PlayChannelTimed(channel, getData(), loops, ticks);
			#endif
		}


		//
		// Fadeins
		//

		/*inline*/ int SoundEffect::fadein(
			int ms     /* =  0*/,
			int loops  /* =  0*/,
			int channel/* = -1*/
		) const
		{
			assert(ok());
			assert(loops   >= 0 || loops   ==  0);
			assert(channel >= 0 || channel == -1);
			assert(isChannelAvailable());
			if (!isChannelAvailable()) return -1;

			if (channel == -1 && getChannel() != -1)
				channel = getChannel();

			#if !defined(NDEBUG)
				int res = Mix_FadeInChannel(channel, getData(), loops, ms);
				assert(res != -1);
				return res;
			#else
				return Mix_FadeInChannel(channel, getData(), loops, ms);
			#endif
		}

		/*inline*/ int SoundEffect::fadeinTimeLimited(
			int ms     /* =  0*/,
			int ticks  /* = -1*/,
			int loops  /* =  0*/,
			int channel/* = -1*/
		) const
		{
			assert(ok());
			assert(ticks   >= 0 || ticks   ==  0);
			assert(loops   >= 0 || loops   ==  0);
			assert(channel >= 0 || channel == -1);
			assert(isChannelAvailable());
			if (!isChannelAvailable()) return -1;

			if (channel == -1 && getChannel() != -1)
				channel = getChannel();

			#if !defined(NDEBUG)
				int res =
					Mix_FadeInChannelTimed(channel, getData(), loops, ms, ticks);
				assert(res != -1);
				return res;
			#else
				return Mix_FadeInChannelTimed(channel, getData(), loops, ms, ticks);
			#endif
		}


		//
		// Getters and setters
		//

		/*inline*/ void SoundEffect::setChannel(int selectedChannel)
		{
			assert(ok());
			assert(selectedChannel >= 0 || selectedChannel == -1);

			selectedChannel_ = selectedChannel;

			assert(ok());
		}

		/*inline*/ int SoundEffect::getChannel() const
		{
			assert(ok());
			return selectedChannel_;
		}

		/*inline*/ void SoundEffect::setData(Mix_Chunk* data)
		{
			assert(ok());
			data_ = data;
			assert(ok());
		}

		/*inline*/ Mix_Chunk* SoundEffect::getData() const
		{
			assert(ok());
			return data_;
		}
	}
}
