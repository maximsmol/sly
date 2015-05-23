#if !defined(h_guard_maximsmol_sly_base)
#define h_guard_maximsmol_sly_base

#define SLY_MAJOR_VERSION 0
#define SLY_MINOR_VERSION 1
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
#include <cstdint>

#include <SDL.h>
#include <SDL_ttf.h>
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

	namespace text
	{
		struct GlyphMetrics
		{
			public:
				int minX, maxX;
				int minY, maxY;
				int advance;
		};

		void setByteOrderSwapped(bool swapped = true);

		class Font
		{
			public:
				Font(const std::string& path, int size = 16, int face = 0);
				Font(const char* path, int size = 16, int face = 0);
				~Font();

				#define __FGCOLOR {255, 255, 255, 255}
				SDL_Texture* getTexure(const std::string& text,
									   SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getTexure(const char* text,
									   SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getTexure(const Uint16* text,
									   SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getTexure(char ch,
									   SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getTexure(Uint16 ch,
									   SDL_Color color = __FGCOLOR) const;

				#define __BGCOLOR {0, 0, 0, 255}
				SDL_Texture* getShadedTexure(
					const std::string& text,
					SDL_Color color = __FGCOLOR,
					SDL_Color bgcolor =__BGCOLOR) const;
				SDL_Texture* getShadedTexure(
					const char* text,
					SDL_Color color = __FGCOLOR,
					SDL_Color bgcolor = __BGCOLOR) const;
				SDL_Texture* getShadedTexure(
					const Uint16* text,
					SDL_Color color = __FGCOLOR,
					SDL_Color bgcolor = __BGCOLOR) const;
				SDL_Texture* getShadedTexure(
					char ch,
					SDL_Color color = __FGCOLOR,
					SDL_Color bgcolor = __BGCOLOR) const;
				SDL_Texture* getShadedTexure(
					Uint16 ch,
					SDL_Color color = __FGCOLOR,
					SDL_Color bgcolor = __BGCOLOR) const;
				#undef __BGCOLOR

				SDL_Texture* getBlendedTexure(
					const std::string& text,
					SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getBlendedTexure(
					const char* text,
					SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getBlendedTexure(
					const Uint16* text,
					SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getBlendedTexure(
					char ch,
					SDL_Color color = __FGCOLOR) const;
				SDL_Texture* getBlendedTexure(
					Uint16 ch,
					SDL_Color color = __FGCOLOR) const;
				#undef __FGCOLOR

				int getMaxAscent() const;
				int getMaxDescent() const;
				int getMaxHeight() const;
				int getLineSkip() const;
				long getFontfaceCount() const;
				bool isMonospaced() const;
				char* getFontFamily() const;
				char* getFaceStyle() const;

				bool hasCharacter(Uint16 ch) const;
				int getCharCode(Uint16 ch) const;


				GlyphMetrics getCharMetrics(Uint16 ch) const;

				int getMinX(Uint16 ch) const;
				int getMaxX(Uint16 ch) const;

				int getMinY(Uint16 ch) const;
				int getMaxY(Uint16 ch) const;

				int getAdvance(Uint16 ch) const;


				SDL_Rect getTextSize(const std::string& text) const;
				SDL_Rect getTextSize(const Uint16* text) const;
				SDL_Rect getTextSize(const char* text) const;

				int getTextWidth(const std::string& text) const;
				int getTextWidth(const Uint16* text) const;
				int getTextWidth(const char* text) const;

				int getTextHeight(const std::string& text) const;
				int getTextHeight(const Uint16* text) const;
				int getTextHeight(const char* text) const;


				void setData(TTF_Font* data);
				TTF_Font* getData() const;

				void setStyle(int style) const;
				int getStyle() const;

				void setBold(bool isBold = true) const;
				bool isBold() const;

				void setItalic(bool isItalic = true) const;
				bool isItalic() const;

				void setUnderlined(bool isUnderlined = true) const;
				bool isUnderlined() const;

				void setStrikethrough(bool isStrikethrough = true) const;
				bool isStrikethrough() const;

				void setOutline(int outline = 0) const;
				int getOutline() const;

				void setHinting(int hinting = TTF_HINTING_NORMAL) const;
				int getHinting() const;

				void useKerning(bool use = true) const;
				bool usesKerning() const;

			private:
				bool ok() const;

				TTF_Font* data_;
		};
	}

	namespace image
	{
		class Image
		{
			public:
				Image(const std::string& path, SDL_Rect* sourceRect = nullptr);
				Image(const char* path, SDL_Rect* sourceRect = nullptr);
				Image(SDL_Texture* data, SDL_Rect* sourceRect = nullptr);
				~Image();

				void flip(bool flipped = true, bool vertical = false);

				void flipHorizontaly();
				void setHorizontalFlip(bool flipped = true);

				void flipVertically ();
				void setVerticalFlip(bool flipped = true);

				void render(
					int x = 0, int y = 0, int w = -1, int h = -1
				) const;

				SDL_Rect getSourceRect() const;
				void setSourceRect(SDL_Rect src);

				SDL_Texture* getData() const;
				void setData(SDL_Texture* texture);

				SDL_RendererFlip getFlip() const;
				void setFlip(SDL_RendererFlip flip);
				void setFlip(int flip);

			private:
				bool ok() const;

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
				SpriteSheet(SDL_Texture* texture,
					 		int w = 32, int h = 32,
							int x = 0, int y = 0);
				SpriteSheet(Image* img,
					 		int w = 32, int h = 32,
							int x = 0, int y = 0);

				void render(int x = 0, int y = 0,
							int spriteX = 0, int spriteY = 0,
							int w = -1, int h = -1) const;

				void setData(Image* data);
				Image* getData() const;

				void setX(int x);
				int getX() const;

				void setY(int y);
				int getY() const;

				void setW(int w);
				int getW() const;

				void setH(int h);
				int getH() const;

			private:
				bool ok() const;

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
				Animation(SDL_Texture* texture,
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
				bool ok() const;

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
				int allocateNewChannel();
				void pauseChannel() const;
				void resumeChannel() const;
				void haltChannel() const;
				void expireChannel(int ticks = 0) const;
				void fadeOutChannel(int ms = 0) const;
				int setChannelVolume(int volume = MIX_MAX_VOLUME) const;
				bool isChannelAvailable() const;

				//
				// Players
				int play(int loops = 0, int channel = -1) const;
				int playTimeLimited(
					int ticks   = -1,
					int loops   =  0,
					int channel = -1
				) const;

				//
				// Fadeins
				int fadein(
					int ms = 0,
					int loops = 0,
					int channel = -1
				) const;
				int fadeinTimeLimited(
					int ms      =  0,
					int ticks   = -1,
					int loops   =  0,
					int channel = -1
				) const;

				//
				// Getters and setters
				void setChannel(int selectedChannel);
				int getChannel() const;

				void setData(Mix_Chunk* data);
				Mix_Chunk* getData() const;

			private:
				bool ok() const;

				int selectedChannel_;
				Mix_Chunk* data_;
		};

		class Music
		{
			public:
				Music(const std::string& path);
				Music(const char* path);
				~Music();

				Mix_MusicType getType() const;

				void play(int loops = 0) const;
				void fadein(int ms = 0, int loops = 0) const;
				void fadeinFrom(
					double pos = 0,
					int ms = 0,
					int loops = 0
				) const;

				void setData(Mix_Music* data);
				Mix_Music* getData() const;

			private:
				bool ok() const;
				Mix_Music* data_;
		};
	}
}

#endif
