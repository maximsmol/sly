#if !defined(h_guard_maximsmol_sly_base)
#define h_guard_maximsmol_sly_base

namespace sly
{
	namespace base
	{
		extern SDL_Window*   window;
		extern SDL_Renderer* renderer;
		extern SDL_Texture*  buffer;

		void init(const char* winName, int w, int h);
		void quit();
	}
}

#endif
