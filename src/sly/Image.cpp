#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

#include "SLY.hpp"

namespace sly
{
	namespace image
	{
		inline SDL_Rect getSourceRect(SDL_Rect* argRect, SDL_Texture* texture)
		{
			assert(texture != nullptr);
			if (argRect != nullptr)
				return *argRect;

			int width  = 0;
			int height = 0;

			sly_assertEqual(
				SDL_QueryTexture(
					texture,
					nullptr, nullptr,
					&width, &height
				), 0
			);

			return {0, 0, width, height};
		}

		Image::Image(const std::string& path,
					 SDL_Rect* sourceRect/* = nullptr*/) :
			Image(path.c_str(), sourceRect) {}
		Image::Image(const char* path, SDL_Rect* sourceRect/* = nullptr*/) :
			data_(nullptr),
			dimensions_(),
			flip_(SDL_FLIP_NONE)
		{
			assert(sly::base::inited);

			//
			// Load the image into SDL_Texture*
			SDL_Surface* surface = IMG_Load(path);
			assert(surface != nullptr);

			data_ = SDL_CreateTextureFromSurface(sly::base::renderer, surface);
			SDL_FreeSurface(surface);

			assert(data_ != nullptr);

			//
			// Store the dimensions for quick sourceRect building
			dimensions_ = sly::image::getSourceRect(sourceRect, getData());

			assert(ok());
		}

		Image::Image(SDL_Texture* data, SDL_Rect* sourceRect/* = nullptr*/) :
			data_(data),
			dimensions_(),
			flip_(SDL_FLIP_NONE)
		{
			assert(sly::base::inited);
			assert(ok());

			//
			// Store the dimensions for quick sourceRect building
			dimensions_ = sly::image::getSourceRect(sourceRect, getData());

			assert(ok());
		}

		Image::~Image()
		{
			SDL_DestroyTexture(data_);
			data_ = nullptr;
		}

		/*inline*/ bool Image::ok() const
		{
			return data_ != nullptr &&
				   dimensions_.w >= 0 && dimensions_.h >= 0 &&
				   dimensions_.x >= 0 && dimensions_.y >= 0 &&
				   (
				   		(flip_ & (~SDL_FLIP_HORIZONTAL)) &
				   		(~SDL_FLIP_VERTICAL)
				   ) == 0;
		}


		/*inline*/ void Image::flip(
			bool flipped/* = true*/,
			bool vertical/* = false*/
		)
		{
			assert(ok());
			if (flipped)
				setFlip(
					getFlip() |
					(vertical ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL)
				);
			else setFlip(
					getFlip() &
					(~(vertical ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL))
				);
		}

		/*inline*/ void Image::flipHorizontaly()
		{
			assert(ok());
			setHorizontalFlip((getFlip() & SDL_FLIP_HORIZONTAL) == 0);
		}

		/*inline*/ void Image::setHorizontalFlip(bool flipped/* = true*/)
		{
			assert(ok());
			flip(flipped);
		}

		/*inline*/ void Image::flipVertically ()
		{
			assert(ok());
			setHorizontalFlip((getFlip() & SDL_FLIP_VERTICAL) == 0);
		}

		/*inline*/ void Image::setVerticalFlip(bool flipped/* = true*/)
		{
			assert(ok());
			flip(flipped, true);
		}


		void Image::render(
			int x/* = 0*/, int y/* = 0*/, int w/* = -1*/, int h/* = -1*/
		) const
		{
			assert(ok());
			assert(w >= 0 || w == -1);
			assert(h >= 0 || h == -1);

			SDL_Rect rect({});
			rect.x = x;
			rect.y = y;
			rect.w = (w == -1 ? dimensions_.w : w);
			rect.h = (h == -1 ? dimensions_.h : h);

			if (getFlip() == SDL_FLIP_NONE)
				sly_assertEqual(
					SDL_RenderCopy(
						sly::base::renderer,
						getData(),
						&dimensions_,
						&rect
					), 0
				);
			else
			{
				sly_assertEqual(
					SDL_RenderCopyEx(
						sly::base::renderer,
						getData(),
						&dimensions_,
						&rect,
						0, nullptr, getFlip()
					), 0
				);
			}
		}


		/*inline*/ SDL_Rect Image::getSourceRect() const
		{
			assert(ok());
			return dimensions_;
		}

		/*inline*/ void Image::setSourceRect(SDL_Rect src)
		{
			assert(ok());
			assert(src.w >= 0 && src.h >= 0 &&
				   src.x >= 0 && src.y >= 0);

			dimensions_ = src;
			assert(ok());
		}


		/*inline*/ SDL_Texture* Image::getData() const
		{
			assert(ok());
			return data_;
		}

		/*inline*/ void Image::setData(SDL_Texture* texture)
		{
			assert(ok());
			assert(texture != nullptr);

			data_ = texture;
			assert(ok());
		}


		/*inline*/ SDL_RendererFlip Image::getFlip() const
		{
			assert(ok());
			return flip_;
		}

		/*inline*/ void Image::setFlip(SDL_RendererFlip flip)
		{
			assert(ok());
			flip_ = flip;
			assert(ok());
		}

		/*inline*/ void Image::setFlip(int flip)
		{
			assert(ok());
			flip_ = static_cast<SDL_RendererFlip>(flip);
			assert(ok());
		}
	}
}
