#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

#include "SLY.hpp"

namespace sly
{
	namespace image
	{
		Image::Image(const char* path) :
			data_(nullptr),
			dimensions_()
		{
			assert(sly::base::inited);

			SDL_Surface* surface = IMG_Load(path);
			assert(surface != nullptr);

			data_ = SDL_CreateTextureFromSurface(sly::base::renderer, surface);
			SDL_FreeSurface(surface);

			assert(data_ != nullptr);

			int width_  = 0;
			int height_ = 0;

			assert(
				SDL_QueryTexture(
					data_,
					nullptr, nullptr,
					&width_, &height_
				) == 0
			);

			dimensions_.x = 0;
			dimensions_.y = 0;
			dimensions_.w = width_;
			dimensions_.h = height_;

			assert(ok());
		}

		bool Image::ok() const
		{
			return data_ != nullptr &&
				   dimensions_.w >= 0 && dimensions_.h >= 0 &&
				   dimensions_.x >= 0 && dimensions_.y >= 0;
		}

		void Image::render(
			int x/* = 0*/, int y/* = 0*/, int w/* = -1*/, int h/* = -1*/
		) const
		{
			assert(ok());
			assert(x >= 0);
			assert(y >= 0);
			assert(w >= 0 || w == -1);
			assert(h >= 0 || h == -1);

			SDL_Rect rect = {};
			rect.x = x;
			rect.y = y;
			rect.w = (w == -1 ? dimensions_.w : w);
			rect.h = (h == -1 ? dimensions_.h : h);

			assert(
				SDL_RenderCopy(
					sly::base::renderer,
					getTexture(),
					&dimensions_,
					&rect
				) == 0
			);
		}


		SDL_Rect Image::getSourceRect() const
		{
			assert(ok());
			return dimensions_;
		}

		void Image::setSourceRect(SDL_Rect src)
		{
			assert(ok());
			dimensions_ = src;
			assert(ok());
		}


		SDL_Texture* Image::getTexture() const
		{
			assert(ok());
			return data_;
		}

		void Image::setTexture(SDL_Texture* texture)
		{
			assert(ok());
			data_ = texture;
			assert(ok());
		}
	}
}
