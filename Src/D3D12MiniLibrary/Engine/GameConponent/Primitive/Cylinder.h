#pragma once
#include "../../Model/ModelMesh.h"
#include "PrimitiveComponent.h"

namespace K3D12 {
	class Cylinder :
		public ModelMesh
	{
	private:
	protected:
		PrimitiveComponent _info;
	public:
	private:
	protected:
	public:
		void Initializer();
		void RegistBundle();
		void SetPipelineState(PipelineStateObject* pipelineState);
		void SetRootSignature(RootSignature* rootSignature);
		void Draw();
		void Update();
		void LateUpdate();
		Cylinder();
		~Cylinder();
	};
}