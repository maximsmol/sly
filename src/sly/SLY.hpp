#if !defined(h_guard_maximsmol_sly_base)
#define h_guard_maximsmol_sly_base

#define SLY_MAJOR_VERSION 0
#define SLY_MINOR_VERSION 0
#define SLY_PATCHLEVEL    0

#include <string>

#include <SDL.h>

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
				Image(const char* path);
				Image(const std::string path);

				void render(
					int x = 0, int y = 0, int w = -1, int h = -1
				) const;

				SDL_Rect getSourceRect() const;
				void setSourceRect(SDL_Rect src);

				SDL_Texture* getTexture() const;
				void setTexture(SDL_Texture* texture);

			private:
				bool ok() const;

				SDL_Texture* data_;
				SDL_Rect dimensions_;
		};
	}
}

#endif
