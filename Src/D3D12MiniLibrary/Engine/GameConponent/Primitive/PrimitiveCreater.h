#pragma once
#include <memory>

namespace K3D12 {

	class PipelineStateObject;
	class RootSignature;
	class GraphicsCommandList;

	class Cube;
	class Sphere;
	class Plane;


	class PrimitiveCreater
	{
	private:
		std::weak_ptr<K3D12::RootSignature> _rootSignature;
		std::weak_ptr<K3D12::PipelineStateObject> _graphicsPipelineState;
		std::weak_ptr<K3D12::GraphicsCommandList> _primitiveDrawCommandList;

	public:
	private:
	public:

		Cube * CreateCube();
		Sphere* CreateSphere();
		Plane*	CreatePlane();

		void SetPrimitiveRootSignature(std::weak_ptr<RootSignature> rootSignature);
		void SetPrimitivePipelineState(std::weak_ptr<PipelineStateObject> graphicsPipelineState);
		void SetPrimitiveDrawCommandList(std::weak_ptr<GraphicsCommandList> primitiveDrawCommandList);

		PrimitiveCreater();
		~PrimitiveCreater();
	};
}