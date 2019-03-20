#pragma once
#include <string>
#include <Shlwapi.h>

namespace K3D12 {
	class FontData
	{
	private:
		LOGFONT _fontParamater;
		HFONT	_font;
		std::string _fontName;
		std::string _fontPath;
	public:
	private:

	public:

		LOGFONT GetPramater();
		HFONT	GetFontHandle();
		void	SetFontHandle(const HFONT& font);
		std::string GetFontName();
		std::string GetFontPath();

		void    Create(LOGFONT paramater, std::string fontName, std::string fontPath);
		void	Discard();

		FontData();
		~FontData();
	};
}