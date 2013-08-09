/******************************************************************************
Copyright (c) 2013 jbyu. All rights reserved.
******************************************************************************/
#ifndef __FONT_CACHE_H__
#define __FONT_CACHE_H__

#include "tinyswf.h"
#include "lru_cache.h"

const int kGLYPH_WIDTH  = 32;

namespace cocos2d {
	class Texture2D;
	class GLProgram;
}

struct GlyphInfo {
	float	advance;
	float	offsetX;
	float	offsetY;
	char	width;
	char	height;
	unsigned short index;
};

class OSFont {
	friend class CCFlashFontHandler;

	typedef const void* Handle;
	typedef LRUCache<wchar_t, GlyphInfo> GlyphCache;

	GlyphCache *_cache;
	cocos2d::Texture2D *_bitmap;
	Handle _font;

public:
	OSFont(const char *font_name, float fontsize, int style);
	~OSFont();

	GlyphInfo* getGlyph(wchar_t code);
	float getLineHeight(void) { return getLineHeight(_font); }

protected:
	// platform-dependent
	static bool initialize(void);
	static void terminate(void);
	static Handle create(const char *font_name, float fontsize, int style);
	static void destroy(const Handle& font);
	static bool makeGlyph(const Handle& font, wchar_t codepoint, GlyphInfo& entry);
	static float getLineHeight(const Handle& font);
	static void* getGlyphBitmap();
}; 

//-----------------------------------------------------------------------------

class CCFlashFontHandler : public tinyswf::FontHandler {
	typedef std::map<std::string, OSFont*> CacheData;

	CacheData _font_cache;
	OSFont* _selectedFont;

	cocos2d::GLProgram *_fontShader;
	int _uvScaleLocation;
	int _colorLocation;

	OSFont* selectFont(const char *font_name, float fontsize, int style);

public:
	CCFlashFontHandler();

	virtual ~CCFlashFontHandler();

	void drawText(const tinyswf::VertexArray& vertices, uint32_t glyphs, const tinyswf::CXFORM& cxform, const tinyswf::TextStyle& style);

	uint32_t formatText(tinyswf::VertexArray& vertices, const tinyswf::RECT& rect, 
		const tinyswf::TextStyle& style, const std::wstring& text);

	GlyphInfo* getGlyph(wchar_t codepoint) {
		return _selectedFont->getGlyph(codepoint);
	}

	float getLineHeight(void) {
		return _selectedFont->getLineHeight();
	}
};


#endif//__FONT_CACHE_H__