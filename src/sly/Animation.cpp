#include <cstdio>
#include <cassert>

#include <SDL.h>
#include <SDL_image.h>

#include "SLY.hpp"

namespace sly
{
	namespace image
	{
		using sly::image::SpriteSheet;
		using sly::image::Image;

		Animation::Animation(const std::string& path,
			                 int rowLength/* = -1*/,
							 int length/* = 0*/,
			 	             int w/* = 32*/, int h/* = 32*/,
				             int x/* = 0*/, int y/* = 0*/) :
			Animation(path.c_str(), rowLength, length, w, h, x, y) {}
		Animation::Animation(const char* path,
			                 int rowLength/* = -1*/,
							 int length/* = 0*/,
			 	             int w/* = 32*/, int h/* = 32*/,
				             int x/* = 0*/, int y/* = 0*/) :
			Animation(new Image(path), rowLength, length, w, h, x, y) {}
		Animation::Animation(Image* img,
							 int rowLength/* = -1*/,
							 int length/* = 0*/,
			 	             int w/* = 32*/, int h/* = 32*/,
				             int x/* = 0*/, int y/* = 0*/) :
			Animation(new SpriteSheet(img, w, h, x, y), rowLength, length) {}
		Animation::Animation(const SpriteSheet* sheet,
							 int rowLength/* = -1*/, int length/* = 0*/) :
			data_(sheet),
			rowLength_(rowLength),
			lastFrame_(0),
			length_(length)
		{
			assert(ok());
		}

		Animation::~Animation()
		{}

		/*inline*/ bool Animation::ok() const
		{
			return data_ != nullptr && (rowLength_ >= 0 || rowLength_ == -1);
		}


		void Animation::render(
			int x/* = 0*/, int y/* = 0*/,
			int w/* = -1*/, int h/* = -1*/) const
		{
			assert(ok());
			getData()->render(
				x, y,
				getFrame()%getRowLength(),
				getFrame()/(getRowLength()+1),
				w, h
			);
		}


		void Animation::setFrame(int frame)
		{
			assert(ok());
			lastFrame_ = frame;
			assert(ok());
		}

		void Animation::nextFrame()
		{
			assert(ok());

			setFrame(getFrame()+1);
			if (getFrame() >= getLength())
				setFrame(0);

			assert(ok());
		}

		int Animation::getFrame() const
		{
			assert(ok());
			return lastFrame_;
		}


		void Animation::setRowLength(int length)
		{
			assert(ok());
			rowLength_ = length;
			assert(ok());
		}

		int Animation::getRowLength() const
		{
			assert(ok());
			return rowLength_;
		}


		void Animation::setData(const SpriteSheet* data)
		{
			assert(ok());
			data_ = data;
			assert(ok());
		}

		const SpriteSheet* Animation::getData() const
		{
			return data_;
		}


		void Animation::setLength(int len)
		{
			assert(ok());
			length_ = len;
			assert(ok());
		}

		int Animation::getLength() const
		{
			assert(ok());
			return length_;
		}
	}
}
