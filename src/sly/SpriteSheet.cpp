#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

#include "SLY.hpp"

namespace sly
{
	namespace image
	{
		// using sly::image::Image;

		SpriteSheet::SpriteSheet(const std::string& path,
			  		int w/* = 32*/, int h/* = 32*/,
			  		int x/* = 0*/, int y/* = 0*/) :
			SpriteSheet(path.c_str(), w, h, x, y) {}
		SpriteSheet::SpriteSheet(const char* path,
			 		int w/* = 32*/, int h/* = 32*/,
					int x/* = 0*/, int y/* = 0*/) :
			SpriteSheet(new Image(path), w, h, x, y) {}
		SpriteSheet::SpriteSheet(Image* img,
			 		int w/* = 32*/, int h/* = 32*/,
					int x/* = 0*/, int y/* = 0*/) :
			data_(img),

			x_(x),
			y_(y),
			w_(w),
			h_(h)
		{
			assert(ok());
		}

		SpriteSheet::~SpriteSheet()
		{}

		/*inline*/ bool SpriteSheet::ok() const
		{
			return data_ != nullptr &&
				   x_ >= 0 && y_ >= 0 &&
				   w_ >= 0 && h_ >= 0;
		}


		void SpriteSheet::render(
			int x/* = 0*/,
			int y/* = 0*/,
			int spriteX/* = 0*/,
			int spriteY/* = 0*/,
			int w/* = -1*/,
			int h/* = -1*/
		) const
		{
			assert(ok());
			assert(x >= 0);
			assert(y >= 0);

			SDL_Rect srcRect({});
			srcRect.x = getX()+spriteX*getW();
			srcRect.y = getY()+spriteY*getH();
			srcRect.w = getW();
			srcRect.h = getH();

			getData()->setSourceRect(srcRect);
			getData()->render(x, y,
							  w == -1 ? getW() : w,
							  h == -1 ? getH() : h);
		}


		/*inline*/ void SpriteSheet::setData(Image* data)
		{
			assert(ok());
			data_ = data;
			assert(ok());
		}

		/*inline*/ Image* SpriteSheet::getData() const
		{
			assert(ok());
			return data_;
		}

		/*inline*/ void SpriteSheet::setX(int x)
		{
			assert(ok());
			x_ = x;
			assert(ok());
		}

		/*inline*/ int SpriteSheet::getX() const
		{
			assert(ok());
			return x_;
		}

		/*inline*/ void SpriteSheet::setY(int y)
		{
			assert(ok());
			y_ = y;
			assert(ok());
		}

		/*inline*/ int SpriteSheet::getY() const
		{
			assert(ok());
			return y_;
		}

		/*inline*/ void SpriteSheet::setW(int w)
		{
			assert(ok());
			w_ = w;
			assert(ok());
		}

		/*inline*/ int SpriteSheet::getW() const
		{
			assert(ok());
			return w_;
		}

		/*inline*/ void SpriteSheet::setH(int h)
		{
			assert(ok());
			h_ = h;
			assert(ok());
		}

		/*inline*/ int SpriteSheet::getH() const
		{
			assert(ok());
			return h_;
		}
	}
}
