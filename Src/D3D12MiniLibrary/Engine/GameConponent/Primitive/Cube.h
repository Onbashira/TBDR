#pragma once
#include "../../Model/ModelMesh.h"
#include "PrimitiveComponent.h"
#include "PrimitiveObject.h"

namespace K3D12 {
	class Cube :
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
		void AttachTexture(std::string pathName);
		void SetUV(float compressionRatio);

		Cube();
		Cube(Vector3 pos, Vector3 scale);
		~Cube();
	};
}