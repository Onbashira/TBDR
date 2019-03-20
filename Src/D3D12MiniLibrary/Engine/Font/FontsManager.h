#pragma once
#include <memory>
#include <string>
#include <Shlwapi.h>
#include "../Util/ManagerComponentBase.h"
#include "FontData.h"

namespace K3D12 {
	class FontData;

	class FontsManager
	{
	private:
		ManagerComponentBase<FontData> _fontsPool;
	public:
	private:
		FontsManager();
		FontsManager(const FontsManager&) {};
		FontsManager(FontsManager&&) {};
		FontsManager& operator=(const FontsManager&&) {};
		FontsManager& operator=(FontsManager&&) {};
	public:

		static FontsManager& GetInstance();

		void LoadFont(std::string fontName, std::string fontsPath, LOGFONT fontParamater);
		void SetFontData(std::string  fontName, const FontData& font);
		std::weak_ptr<FontData> GetFont(std::string  fontName);
		void EraseFont(std::string  fontName);
		void Discard();


		~FontsManager();
	};
}