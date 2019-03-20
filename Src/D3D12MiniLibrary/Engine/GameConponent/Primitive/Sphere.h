#pragma once
#include "../../Model/ModelMesh.h"
#include "PrimitiveComponent.h"
#include "PrimitiveObject.h"

namespace K3D12 {
	class Sphere :
		public ModelMesh, public PrimitiveObject
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


		Sphere();
		~Sphere();
	};
}