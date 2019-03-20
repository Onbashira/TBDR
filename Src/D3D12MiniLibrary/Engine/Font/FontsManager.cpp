#include <memory>
#include <string>
#include <Shlwapi.h>
#include "FontsManager.h"
#include "FontData.h"
#include "../Util/Utility.h"


K3D12::FontsManager::FontsManager()
{
}


K3D12::FontsManager::~FontsManager()
{
	Discard();
}


K3D12::FontsManager & K3D12::FontsManager::GetInstance()
{
	static FontsManager instance;
	return instance;
}

void K3D12::FontsManager::LoadFont(std::string fontName, std::string fontsPath, LOGFONT fontParamater)
{
	std::shared_ptr<FontData> font = std::make_shared<FontData>();
	font->Create(fontParamater, fontsPath, fontName);
	_fontsPool.Set(fontName, font);
}

void K3D12::FontsManager::SetFontData(std::string fontName, const FontData & font)
{
	_fontsPool.Set(fontName, font);
}

std::weak_ptr<K3D12::FontData> K3D12::FontsManager::GetFont(std::string fontName)
{
	return _fontsPool.Get(fontName);
}

void K3D12::FontsManager::EraseFont(std::string fontName)
{
	_fontsPool.Erase(fontName);
}

void K3D12::FontsManager::Discard()
{
	_fontsPool.DiscardMap();
}


