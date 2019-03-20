#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../Util/D3D12Common.h"
#include "../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {

	class ShaderResource;
	class GraphicsCommandList;
	class CommandQueue;

	class TextureLoader
	{
	private:
		HRESULT LoadUpdateSubResource(std::shared_ptr<GraphicsCommandList> list, std::weak_ptr<ShaderResource> resource, std::string path);
		TextureLoader();
	public:
		static TextureLoader & GetInstance() {
			static TextureLoader loader;
			return loader;
		}
		void LoadSpriteTexture(std::shared_ptr<GraphicsCommandList> commandList, CommandQueue* commandQueue, std::string path);
		void LoadSpriteTexture(std::string path, D3D12_CPU_DESCRIPTOR_HANDLE handle);
		void LoadModelTexture(std::shared_ptr<GraphicsCommandList> commandList, CommandQueue* commandQueue, std::string modelName, std::vector<std::string>& paths);
		void LoadModelTexture(std::shared_ptr<GraphicsCommandList> commandList, CommandQueue* commandQueue, DescriptorHeap& heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths);

		~TextureLoader();
	};
}