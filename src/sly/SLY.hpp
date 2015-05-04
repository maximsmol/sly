#if !defined(h_guard_maximsmol_sly_base)
#define h_guard_maximsmol_sly_base

#define SLY_MAJOR_VERSION 0
#define SLY_MINOR_VERSION 0
#define SLY_PATCHLEVEL    0

/*
	Ensures that an instruction is asserted in debug mode
	and still executed in release.
*/
#if !defined(NDEBUG)
	#define sly_assertEqual(condition, value) \
		assert((condition) == (value))

	#define sly_assertNEqual(condition, value) \
		assert((condition) != (value))
#else
	#define sly_assertEqual(condition, value) (condition)
	#define sly_assertNEqual(condition, value) (condition)
#endif

#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

namespace sly
{
	namespace base
	{
		extern SDL_Window*   window;
		extern SDL_Renderer* renderer;
		extern SDL_Texture*  buffer;

		extern bool inited;

		void init(const char* winName, int w, int h);
		void render();
		void quit();
	}

	namespace image
	{
		class Image
		{
			public:
				Image(const char* path, SDL_Rect* sourceRect = nullptr);
				Image(const std::string& path, SDL_Rect* sourceRect = nullptr);
				~Image();

				/*inline*/ void flip(bool flipped = true, bool vertical = false);

				/*inline*/ void flipHorizontaly();
				/*inline*/ void setHorizontalFlip(bool flipped = true);

				/*inline*/ void flipVertically ();
				/*inline*/ void setVerticalFlip(bool flipped = true);

				void render(
					int x = 0, int y = 0, int w = -1, int h = -1
				) const;

				/*inline*/ SDL_Rect getSourceRect() const;
				/*inline*/ void setSourceRect(SDL_Rect src);

				/*inline*/ SDL_Texture* getTexture() const;
				/*inline*/ void setTexture(SDL_Texture* texture);

				/*inline*/ SDL_RendererFlip getFlip() const;
				/*inline*/ void setFlip(SDL_RendererFlip flip);
				/*inline*/ void setFlip(int flip);

			private:
				/*inline*/ bool ok() const;

				SDL_Texture* data_;
				SDL_Rect dimensions_;
				SDL_RendererFlip flip_;
		};
	}

	namespace sound
	{
		class SoundEffect
		{
			public:
				SoundEffect(const char* path, int selectedChannel = -1);
				SoundEffect(const std::string& path, int selectedChannel = -1);
				~SoundEffect();

				//
				// Channel control
				/*inline*/ int allocateNewChannel();
				/*inline*/ void pauseChannel() const;
				/*inline*/ void resumeChannel() const;
				/*inline*/ void haltChannel() const;
				/*inline*/ void expireChannel(int ticks = 0) const;
				/*inline*/ void fadeOutChannel(int ms = 0) const;
				/*inline*/ int setChannelVolume(int volume = MIX_MAX_VOLUME) const;
				/*inline*/ bool isChannelAvailable() const;

				//
				// Players
				/*inline*/ int play(int loops = 0, int channel = -1) const;
				/*inline*/ int playTimeLimited(
					int ticks   = -1,
					int loops   =  0,
					int channel = -1
				) const;

				//
				// Fadeins
				/*inline*/ int fadein(
					int ms = 0,
					int loops = 0,
					int channel = -1
				) const;
				/*inline*/ int fadeinTimeLimited(
					int ms      =  0,
					int ticks   = -1,
					int loops   =  0,
					int channel = -1
				) const;

				//
				// Channel getters and setters
				/*inline*/ void setChannel(int selectedChannel);
				/*inline*/ int getChannel() const;

			private:
				/*inline*/ bool ok() const;

				int selectedChannel_;
				Mix_Chunk* data_;
		};
	}
}

#endif
