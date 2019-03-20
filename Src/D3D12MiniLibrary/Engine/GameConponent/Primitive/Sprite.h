#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../../Util/Math.h"
#include "../../Model/ModelMesh.h"
#include "../../Util/Math.h"

namespace K3D12 {

	class FontData;
	class PipelineStateObject;
	class RootSignature;

	//2DSprite　正射影投影で使う
	class Sprite :
		public ModelMesh
	{
	private:
		struct SpriteInfo {
			float	alphaFactor; //0.0~1.0
			Vector2 centerOffset;
			Vector2	topLeftUV;			//Rect           
			Vector2 bottomRightUV;		//Rect
		}_info;

		unsigned int  _height;
		unsigned int  _width;

		std::vector<Vertex2D> _vertexes;
		std::vector<unsigned int> _list;
		std::weak_ptr<ShaderResource> _shaderResource;
	protected:

	public:

	private:

	protected:

	public:

		void Draw();
		void DrawString(std::weak_ptr<FontData> font, std::string str);
		void Initializer();
		void RegistBundle();
		void SetPipelineState(PipelineStateObject* pipelineState);
		void SetRootSignature(RootSignature* rootSignature);
		void AttachTexture(std::string path); //テクスチャアタッチ（すでにセットされたテクスチャの所有権を破棄して新しいテクスチャの所有権を得る）
		void SetRect(float top, float left, float bottom, float right);
		void SetRect(Vector2 topLeft, Vector2 bottomLeft);
		void Translate(Vector2 pos);
		void Move(Vector2 velocity);
		Vector2 GetPos();
		void SetScale(Vector2 scale);
		Vector2 GetScale();
		void Rotate(float deg);
		void Rotate(Quaternion quaternion);
		void Update();
		void LateUpdate();

		Sprite();
		virtual ~Sprite();
	};
}
