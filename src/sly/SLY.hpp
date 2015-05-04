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
				Image(const std::string& path, SDL_Rect* sourceRect = nullptr);
				Image(const char* path, SDL_Rect* sourceRect = nullptr);
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

		class SpriteSheet
		{
			public:
				SpriteSheet(const std::string& path,
					  		int w = 32, int h = 32,
					  		int x = 0, int y = 0);
				SpriteSheet(const char* path,
					 		int w = 32, int h = 32,
							int x = 0, int y = 0);
				SpriteSheet(Image* img,
					 		int w = 32, int h = 32,
							int x = 0, int y = 0);
				~SpriteSheet();

				void render(int x = 0, int y = 0,
							int spriteX = 0, int spriteY = 0,
							int w = -1, int h = -1) const;

				/*inline*/ void setData(Image* data);
				/*inline*/ Image* getData() const;

				/*inline*/ void setX(int x);
				/*inline*/ int getX() const;

				/*inline*/ void setY(int y);
				/*inline*/ int getY() const;

				/*inline*/ void setW(int w);
				/*inline*/ int getW() const;

				/*inline*/ void setH(int h);
				/*inline*/ int getH() const;

			private:
				/*inline*/ bool ok() const;

				Image* data_;
				int x_, y_, w_, h_;
		};

		class Animation
		{
			public:
				Animation(const std::string& path,
						  int rowLength = -1,
						  int length = 0,
					 	  int w = 32, int h = 32,
						  int x = 0, int y = 0);
				Animation(const char* path,
						  int rowLength = -1,
						  int length = 0,
					 	  int w = 32, int h = 32,
						  int x = 0, int y = 0);
				Animation(Image* img,
						  int rowLength = -1,
						  int length = 0,
					 	  int w = 32, int h = 32,
						  int x = 0, int y = 0);
				Animation(const SpriteSheet* sheet,
						  int rowLength = -1, int length = 0);
				~Animation();

				void render(int x = 0, int y = 0,
							int w = -1, int h = -1) const;

				void setFrame(int frame);
				void nextFrame();
				int getFrame() const;

				void setRowLength(int length);
				int getRowLength() const;

				void setData(const SpriteSheet* data);
				const SpriteSheet* getData() const;

				void setLength(int len);
				int getLength() const;

			private:
				/*inline*/ bool ok() const;

				const SpriteSheet* data_;
				int rowLength_;
				int lastFrame_;
				int length_;
		};
	}

	namespace sound
	{
		class SoundEffect
		{
			public:
				SoundEffect(const std::string& path, int selectedChannel = -1);
				SoundEffect(const char* path, int selectedChannel = -1);
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
				// Getters and setters
				/*inline*/ void setChannel(int selectedChannel);
				/*inline*/ int getChannel() const;

				/*inline*/ void setData(Mix_Chunk* data);
				/*inline*/ Mix_Chunk* getData() const;

			private:
				/*inline*/ bool ok() const;

				int selectedChannel_;
				Mix_Chunk* data_;
		};

		class Music
		{
			public:
				Music(const std::string& path);
				Music(const char* path);
				~Music();

				/*inline*/ Mix_MusicType getType() const;

				/*inline*/ void play(int loops = 0) const;
				/*inline*/ void fadein(int ms = 0, int loops = 0) const;
				/*inline*/ void fadeinFrom(
					double pos = 0,
					int ms = 0,
					int loops = 0
				) const;

				/*inline*/ void setData(Mix_Music* data);
				/*inline*/ Mix_Music* getData() const;

			private:
				/*inline*/ bool ok() const;
				Mix_Music* data_;
		};
	}
}

#endif
