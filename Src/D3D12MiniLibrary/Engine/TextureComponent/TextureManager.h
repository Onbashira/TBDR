#pragma once
#include <map>
#include <memory>
#include "../Util/D3D12Common.h"
#include "TextureLoader.h"



namespace K3D12 {
	class DescriptorHeap;
	class TextureLoader;
	class ShaderResource;

	class TextureManager
	{

		friend class TextureLoader;

	private:
		const std::string _nullTextureName = "NULL_TEXTURE";
		//Key = ModelPath,Value = TextureResources;
		std::map < std::string, std::shared_ptr<ShaderResource> >				_modelTextureResourceMap;

	public:

	private:
		//void SetTexture(std::string& path, std::shared_ptr<ShaderResource> resource);

		TextureManager & operator=(const TextureManager& value) { return *this; };
		TextureManager(const TextureManager& other) {};
		TextureManager();
	public:
		static TextureManager& GetInstance() {
			static TextureManager instance;
			return instance;
		};
		std::weak_ptr<ShaderResource> GetSpriteShaderResource(std::string name);
		std::weak_ptr<ShaderResource> GetModelTextureShaderResource(std::string modelPath);
		std::weak_ptr<ShaderResource> GetNullTextureShaderResource();

		//std::shared_ptr<DescriptorHeap> GetDescriptorHeap(std::string path);
		void Discard();
		~TextureManager();
	};

}