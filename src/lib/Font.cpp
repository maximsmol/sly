#include <cstdio>
#include <cassert>
#include <cstdint>

#include <SDL.h>
#include <SDL_ttf.h>

#include "SLY.hpp"

namespace sly
{
	namespace text
	{
		void setByteOrderSwapped(bool swapped/* = true*/)
		{
			assert(sly::base::inited);
			TTF_ByteSwappedUNICODE(swapped);
		}

		Font::Font(const std::string& path, int size/* = 16*/,
				   int face/* = 0*/) :
			Font(path.c_str(), size, face) {}
		Font::Font(const char* path, int size/* = 16*/, int face/* = 0*/) :
			data_(TTF_OpenFontIndex(path, size, face))
		{
			assert(sly::base::inited);
			assert(ok());
		}

		Font::~Font()
		{
			TTF_CloseFont(data_);
			data_ = nullptr;
		}

		bool Font::ok() const
		{
			return data_ != nullptr;
		}


		//
		// Render to texture
		//

		//
		// Solid rendering
		#define __font_getTexture(method, data)                          \
			SDL_Surface* surf = method(getData(), data, color);          \
			assert(surf != nullptr);                                     \
		                                                                 \
			SDL_Texture* res =                                           \
				SDL_CreateTextureFromSurface(sly::base::renderer, surf); \
			SDL_FreeSurface(surf);                                       \
			assert(res != nullptr)

		SDL_Texture* Font::getTexure(const std::string& text,
									 SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			return getTexure(text.c_str(), color);
		}

		SDL_Texture* Font::getTexure(const char* text,
									 SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderText_Solid, text);
			return res;
		}

		SDL_Texture* Font::getTexure(const Uint16* text,
									 SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderUNICODE_Solid, text);
			return res;
		}

		SDL_Texture* Font::getTexure(char ch,
									 SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderGlyph_Solid, static_cast<Uint16>(ch));
			return res;
		}

		SDL_Texture* Font::getTexure(Uint16 ch,
									 SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderGlyph_Solid, ch);
			return res;
		}

		//
		// Shaded rendering
		#define __font_getTextureShaded(method, data)                    \
			SDL_Surface* surf = method(getData(), data, color, bgColor); \
			assert(surf != nullptr);                                     \
		                                                                 \
			SDL_Texture* res =                                           \
				SDL_CreateTextureFromSurface(sly::base::renderer, surf); \
			SDL_FreeSurface(surf);                                       \
			assert(res != nullptr)

		SDL_Texture* Font::getShadedTexure(
			const std::string& text,
			SDL_Color color/* = __FGCOLOR*/,
			SDL_Color bgColor/* = __BGCOLOR*/) const
		{
			assert(ok());
			return getShadedTexure(text.c_str(), color, bgColor);
		}

		SDL_Texture* Font::getShadedTexure(
			const char* text,
			SDL_Color color/* = __FGCOLOR*/,
			SDL_Color bgColor/* = __BGCOLOR*/) const
		{
			assert(ok());
			__font_getTextureShaded(TTF_RenderText_Shaded, text);
			return res;
		}

		SDL_Texture* Font::getShadedTexure(
			const Uint16* text,
			SDL_Color color/* = __FGCOLOR*/,
			SDL_Color bgColor/* = __BGCOLOR*/) const
		{
			assert(ok());
			__font_getTextureShaded(TTF_RenderUNICODE_Shaded, text);
			return res;
		}

		SDL_Texture* Font::getShadedTexure(
			char ch,
			SDL_Color color/* = __FGCOLOR*/,
			SDL_Color bgColor/* = __BGCOLOR*/) const
		{
			assert(ok());
			__font_getTextureShaded(TTF_RenderGlyph_Shaded,
									static_cast<Uint16>(ch));
			return res;
		}

		SDL_Texture* Font::getShadedTexure(
			Uint16 ch,
			SDL_Color color/* = __FGCOLOR*/,
			SDL_Color bgColor/* = __BGCOLOR*/) const
		{
			assert(ok());
			__font_getTextureShaded(TTF_RenderGlyph_Shaded, ch);
			return res;
		}

		#undef __font_getTextureShaded

		//
		// Blended rendering
		SDL_Texture* Font::getBlendedTexure(
			const std::string& text,
			SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			return getBlendedTexure(text.c_str(), color);
		}

		SDL_Texture* Font::getBlendedTexure(
			const char* text,
			SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderText_Blended, text);
			return res;
		}

		SDL_Texture* Font::getBlendedTexure(
			const Uint16* text,
			SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderUNICODE_Blended, text);
			return res;
		}

		SDL_Texture* Font::getBlendedTexure(
			char ch,
			SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderGlyph_Blended,
							  static_cast<Uint16>(ch));
			return res;
		}

		SDL_Texture* Font::getBlendedTexure(
			Uint16 ch,
			SDL_Color color/* = __FGCOLOR*/) const
		{
			assert(ok());
			__font_getTexture(TTF_RenderGlyph_Blended, ch);
			return res;
		}

		#undef __font_getTexture

		//
		// Font information
		//

		int Font::getMaxAscent() const
		{
			assert(ok());
			return TTF_FontAscent(getData());
		}

		int Font::getMaxDescent() const
		{
			assert(ok());
			return TTF_FontDescent(getData());
		}

		int Font::getMaxHeight() const
		{
			assert(ok());
			return TTF_FontHeight(getData());
		}

		int Font::getLineSkip() const
		{
			assert(ok());
			return TTF_FontLineSkip(getData());
		}

		long Font::getFontfaceCount() const
		{
			assert(ok());
			return TTF_FontFaces(getData());
		}

		bool Font::isMonospaced() const
		{
			assert(ok());
			return TTF_FontFaceIsFixedWidth(getData());
		}

		char* Font::getFontFamily() const
		{
			assert(ok());
			return TTF_FontFaceFamilyName(getData());
		}

		char* Font::getFaceStyle() const
		{
			assert(ok());
			return TTF_FontFaceStyleName(getData());
		}


		//
		// Glyph availability
		//

		bool Font::hasCharacter(Uint16 ch) const
		{
			assert(ok());
			return TTF_GlyphIsProvided(getData(), ch) != 0;
		}

		int Font::getCharCode(Uint16 ch) const
		{
			assert(ok());
			return TTF_GlyphIsProvided(getData(), ch);
		}


		//
		// Character information
		//

		GlyphMetrics Font::getCharMetrics(Uint16 ch) const
		{
			assert(ok());
			assert(hasCharacter(ch));

			GlyphMetrics res({});
			sly_assertEqual(
				TTF_GlyphMetrics(
					getData(), ch,
					&(res.minX), &(res.maxX),
					&(res.minY), &(res.maxY),
					&(res.advance)
				), 0
			);

			return res;
		}

		//
		// X axis information
		int Font::getMinX(Uint16 ch) const
		{
			assert(ok());
			assert(hasCharacter(ch));
			int res = 0;

			sly_assertEqual(
				TTF_GlyphMetrics(
					getData(), ch,
					&res, nullptr, nullptr, nullptr, nullptr
				), 0);

			return res;
		}

		int Font::getMaxX(Uint16 ch) const
		{
			assert(ok());
			assert(hasCharacter(ch));
			int res = 0;

			sly_assertEqual(
				TTF_GlyphMetrics(
					getData(), ch,
					nullptr, &res, nullptr, nullptr, nullptr
				), 0);

			return res;
		}

		//
		// Y axis information
		int Font::getMinY(Uint16 ch) const
		{
			assert(ok());
			assert(hasCharacter(ch));
			int res = 0;

			sly_assertEqual(
				TTF_GlyphMetrics(
					getData(), ch,
					nullptr, nullptr, &res, nullptr, nullptr
				), 0);

			return res;
		}

		int Font::getMaxY(Uint16 ch) const
		{
			assert(ok());
			assert(hasCharacter(ch));
			int res = 0;

			sly_assertEqual(
				TTF_GlyphMetrics(
					getData(), ch,
					nullptr, nullptr, nullptr, &res, nullptr
				), 0);

			return res;
		}

		//
		// Advance value getter
		int Font::getAdvance(Uint16 ch) const
		{
			assert(ok());
			assert(hasCharacter(ch));
			int res = 0;

			sly_assertEqual(
				TTF_GlyphMetrics(
					getData(), ch,
					nullptr, nullptr, nullptr, nullptr, &res
				), 0);

			return res;
		}


		//
		// Text size getters
		//

		//
		// Both dimensions
		SDL_Rect Font::getTextSize(const std::string& text) const
		{
			assert(ok());
			return getTextSize(text.c_str());
		}

		SDL_Rect Font::getTextSize(const Uint16* text) const
		{
			assert(ok());

			SDL_Rect res({});
			sly_assertEqual(TTF_SizeUNICODE(getData(), text, &res.w, &res.h), 0);

			return res;
		}

		SDL_Rect Font::getTextSize(const char* text) const
		{
			assert(ok());

			SDL_Rect res({});
			sly_assertEqual(TTF_SizeText(getData(), text, &res.w, &res.h), 0);

			return res;
		}

		//
		// Width
		int Font::getTextWidth(const std::string& text) const
		{
			assert(ok());
			return getTextWidth(text.c_str());

		}

		int Font::getTextWidth(const Uint16* text) const
		{
			assert(ok());

			int res = 0;
			sly_assertEqual(TTF_SizeUNICODE(getData(), text, &res, nullptr), 0);

			return res;
		}

		int Font::getTextWidth(const char* text) const
		{
			assert(ok());

			int res = 0;
			sly_assertEqual(TTF_SizeText(getData(), text, &res, nullptr), 0);

			return res;
		}

		//
		// Height
		int Font::getTextHeight(const std::string& text) const
		{
			assert(ok());
			return getTextHeight(text.c_str());
		}

		int Font::getTextHeight(const Uint16* text) const
		{
			assert(ok());

			int res = 0;
			sly_assertEqual(TTF_SizeUNICODE(getData(), text, nullptr, &res), 0);

			return res;
		}

		int Font::getTextHeight(const char* text) const
		{
			assert(ok());

			int res = 0;
			sly_assertEqual(TTF_SizeText(getData(), text, nullptr, &res), 0);

			return res;
		}


		//
		// Getters and setters of
		//

		//
		// Data
		void Font::setData(TTF_Font* data)
		{
			assert(ok());
			data_ = data;
			assert(ok());
		}

		TTF_Font* Font::getData() const
		{
			assert(ok());
			return data_;
		}

		//
		// The whole style
		void Font::setStyle(int style) const
		{
			assert(ok());
			TTF_SetFontStyle(getData(), style);
			assert(ok());
		}

		int Font::getStyle() const
		{
			assert(ok());
			return TTF_GetFontStyle(getData());
		}

		//
		// Boldness
		void Font::setBold(bool isBold/* = true*/) const
		{
			assert(ok());

			if (isBold)
				setStyle(getStyle() | TTF_STYLE_BOLD);
			else
				setStyle(getStyle() & ~TTF_STYLE_BOLD);

			assert(ok());
		}

		bool Font::isBold() const
		{
			assert(ok());
			return getStyle() * TTF_STYLE_BOLD;
		}

		//
		// Italic setting
		void Font::setItalic(bool isItalic/* = true*/) const
		{
			assert(ok());

			if (isItalic)
				setStyle(getStyle() | TTF_STYLE_ITALIC);
			else
				setStyle(getStyle() & ~TTF_STYLE_ITALIC);

			assert(ok());
		}

		bool Font::isItalic() const
		{
			assert(ok());
			return getStyle() * TTF_STYLE_ITALIC;
		}

		//
		// Underline
		void Font::setUnderlined(bool isUnderlined/* = true*/) const
		{
			assert(ok());

			if (isUnderlined)
				setStyle(getStyle() | TTF_STYLE_UNDERLINE);
			else
				setStyle(getStyle() & ~TTF_STYLE_UNDERLINE);

			assert(ok());
		}

		bool Font::isUnderlined() const
		{
			assert(ok());
			return getStyle() * TTF_STYLE_UNDERLINE;
		}

		//
		// Strikethrough
		void Font::setStrikethrough(bool isStrikethrough/* = true*/) const
		{
			assert(ok());

			if (isStrikethrough)
				setStyle(getStyle() | TTF_STYLE_STRIKETHROUGH);
			else
				setStyle(getStyle() & ~TTF_STYLE_STRIKETHROUGH);

			assert(ok());
		}

		bool Font::isStrikethrough() const
		{
			assert(ok());
			return getStyle() * TTF_STYLE_STRIKETHROUGH;
		}

		//
		// Outline
		void Font::setOutline(int outline/* = 0*/) const
		{
			assert(ok());
			assert(outline >= 0);

			if (getOutline() == outline) return; // Wiki suggests this
			TTF_SetFontOutline(getData(), outline);

			assert(ok());
		}

		int Font::getOutline() const
		{
			assert(ok());
			return TTF_GetFontOutline(getData());
		}

		//
		// Hinting
		void Font::setHinting(int hinting/* = TTF_HINTING_NORMAL*/) const
		{
			assert(ok());
			TTF_SetFontHinting(getData(), hinting);
			assert(ok());
		}

		int Font::getHinting() const
		{
			assert(ok());
			return TTF_GetFontHinting(getData());
		}

		//
		// Kerning
		void Font::useKerning(bool use/* = true*/) const
		{
			assert(ok());
			TTF_SetFontKerning(getData(), use);
			assert(ok());
		}

		bool Font::usesKerning() const
		{
			assert(ok());
			return TTF_GetFontKerning(getData());
		}
	}
}
