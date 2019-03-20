#pragma once
#include <vector>
#include <memory>
#include "../RendererMaterial.h"
#include "../../Resource/ShaderResource.h"
#include "Math.h"


namespace K3D12 {
	class ShaderResource;

	struct PrimitiveComponent
	{

		enum class PRIMITIVE_ROOT_IDNEX {
			CAMERA_PARAMATER_INDEX = 0,
			TRANSFORM_PARAMATER_INDEX,
			MATERIAL_PARAMATER_INDEX,
			SHADER_PARAMATER_INDEX
		};

		std::vector<Vertex3D> _vertexes;
		std::vector<unsigned int> _list;
		std::weak_ptr<ShaderResource> _shaderResource;
		RendererMaterial _materialInfo;
		void AttachTexture(std::string texturePath, D3D12_CPU_DESCRIPTOR_HANDLE handle);
		PrimitiveComponent();
		~PrimitiveComponent();
	};
}