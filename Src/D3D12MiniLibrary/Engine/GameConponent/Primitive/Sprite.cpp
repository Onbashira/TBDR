#include "Sprite.h"
#include "Math.h"
#include "PrimitiveComponent.h"
#include "../../TextureComponent/TextureManager.h"
#include "../../Model/ModelMesh.h"
#include "../../Resource/IndexBuffer.h"
#include "../../Resource/VertexBuffer.h"
#include "../../Resource/ShaderResource.h"
#include "../../Font/FontData.h"
#include "../../System/D3D12System.h"
#include "../../Util/Utility.h"
#include "../../Util/Math.h"
#include "../../CommandContext/CommandListLibrary.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../Signature/RootSignature.h"


K3D12::Sprite::Sprite() : _height(0), _width(0)
{
	Initializer();
}


K3D12::Sprite::~Sprite()
{
}

void K3D12::Sprite::Draw()
{
	this->_commandList.lock()->GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->BindingShaderObject();

	this->_commandList.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(0, D3D12System::GetInstance().GetCamera().GetCameraBuffer().GetResource()->GetGPUVirtualAddress());
	ID3D12DescriptorHeap* heap[] = { _heap->GetPtr() };
	this->_commandList.lock()->GetCommandList()->SetDescriptorHeaps(1, heap);
	this->_commandList.lock()->GetCommandList()->ExecuteBundle(_bundleList.GetCommandList().Get());
}

void K3D12::Sprite::DrawString(std::weak_ptr<FontData> font, std::string str)
{
	//HDC hdc = GetDC(NULL);
	//HFONT oldFont = (HFONT)SelectObject(hdc, font.lock()->GetFontHandle());

	//unsigned int code = 0;

	////フォントビットマップの取得
	//TEXTMETRIC textMetric;
	//GetTextMetrics(hdc, &textMetric);
	//GLYPHMETRICS glyphMetrics;

	//const MAT2 mat = { {0,1},{0,0},{0,0},{1,0} };
	//DWORD size = GetGlyphOutline(
	//	hdc,
	//	code,
	//	GGO_GRAY4_BITMAP,
	//	&glyphMetrics,
	//	0,
	//	NULL,
	//	&mat
	//);

	//BYTE* ptr = new BYTE[size];
	//for (int codeNum = 0; codeNum < str.size(); ++codeNum) {

	//	unsigned int code = 0;

	//	//フォントビットマップの取得
	//	TEXTMETRIC textMetric;
	//	GetTextMetrics(hdc, &textMetric);
	//	GLYPHMETRICS glyphMetrics;

	//	const MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 1,0 } };
	//	DWORD size = GetGlyphOutline(
	//		hdc,
	//		code,
	//		GGO_GRAY4_BITMAP,
	//		&glyphMetrics,
	//		0,
	//		NULL,
	//		&mat
	//	);

	//	GetGlyphOutline(
	//		hdc,
	//		code,
	//		GGO_GRAY4_BITMAP,
	//		&glyphMetrics,
	//		size,
	//		ptr,
	//		&mat
	//	);

	//	SelectObject(hdc, oldFont);
	//	ReleaseDC(NULL, hdc);

	//	D3D12_RESOURCE_DESC desc;
	//	memset(&desc, 0, sizeof(D3D12_RESOURCE_DESC));


	//	ShaderResource resource;
	//	{
	//		D3D12_HEAP_PROPERTIES props = {};
	//		props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	//		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	//		props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	//		props.CreationNodeMask = 0;
	//		props.VisibleNodeMask = 0;

	//		D3D12_RESOURCE_DESC resDesc = {};
	//		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//		desc.Width = glyphMetrics.gmCellIncX;
	//		desc.Height = textMetric.tmHeight;
	//		resDesc.DepthOrArraySize = 1;
	//		resDesc.MipLevels = 1;
	//		resDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	//		resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	//		resDesc.SampleDesc.Count = 1;
	//		resDesc.SampleDesc.Quality = 0;
	//		resDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	//		resource.Create(props, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	//	}
	//	UCHAR* buffer;
	//	resource.Map(0, nullptr);
	//	buffer = resource.GetMappedPointer();
	//	int offsetX = glyphMetrics.gmptGlyphOrigin.x;
	//	int offsetY = textMetric.tmAscent - glyphMetrics.gmptGlyphOrigin.y;
	//	int bmpWidth = glyphMetrics.gmBlackBoxX + (4 - (glyphMetrics.gmBlackBoxX % 4)) % 4;
	//	int bmpHeight = glyphMetrics.gmBlackBoxY;
	//	int level = 17; // アルファ値の階層のレベル
	//	int x = 0, y = 0;
	//	DWORD alpha = 0, color = 0;

	//	memset(buffer, 0, textMetric.tmHeight * resource->GetDesc().Width);
	//	for (y = offsetY; y < offsetY + bmpHeight; ++y) {
	//		for (x = offsetX; x < offsetX + bmpWidth; ++x) {
	//			alpha = (255 * ptr[x - offsetX + bmpWidth * (y - offsetY)]) / (level - 1);
	//			color = 0x00ffffff | (alpha << 24);
	//			std::memcpy(buffer + resource->GetDesc().Width * y + 4 * x, &color, sizeof(DWORD));
	//		}
	//	}

	//	resource.Unmap(0, nullptr);
	//}
}

void K3D12::Sprite::Initializer()
{
	this->_transformStartPoint = 0;
	this->_materialStartPoint = _transformStartPoint + 1;
	this->_textureStartPoint = _materialStartPoint + 1;

	unsigned int sufaceCount = 1;
	unsigned int planeVertex = 4;
	Vertex2D plane[4];

	for (unsigned int i = 0; i < planeVertex; ++i) {
		plane[i].pos = Vector2((2.0f*static_cast<float>((i) % 2) - 1.0f), -(2.0f * static_cast<float>((i) % 4 / 2) - 1.0f));
		plane[i].texCoord = { static_cast<float>(i % 2), static_cast<float>(i / 2) };
		_vertexes.push_back(plane[i]);
	}
	unsigned int planeList[] = { 0,1,2,1,3,2 };
	for (unsigned int listIndex = 0; listIndex < sufaceCount; ++listIndex) {
		_list.push_back(planeList[listIndex]);
	}

	// Resource Creation
	{
		_indexBuffer->Create(_list.size() * sizeof(unsigned int), sizeof(unsigned int), &this->_list[0]);
		_vertexBuffer->Create(_vertexes.size() * sizeof(Vertex2D), sizeof(Vertex2D), &this->_vertexes[0]);
		_indexBuffer->SetName("SpriteVertexBuffer");
		_vertexBuffer->SetName("SpriteIndexBuffer");


		InitalizeTransformBuffer(Util::Alignment256Bytes(sizeof(Matrix)));
		_materialBuffer.Create(Util::Alignment256Bytes(sizeof(float)));
		_materialBuffer.SetName("SpriteInfomationBuffer");
	}

	//MaterialFactor Update
	this->_info.alphaFactor = 1.0f;
	this->_info.topLeftUV = Vector2(0.0f, 0.0f);
	this->_info.bottomRightUV = Vector2(1.0f, 1.0f);
	_materialBuffer.Update(&_info, sizeof(SpriteInfo), 0);

	//TransformUpdate
	SetScale(Vector2(1.0f, 1.0f));
	Translate(Vector2(0.0f, 0.0f));
	SetRotation(Quaternion::CreateFromEulerAngles(Vector3::zero));
	GameObject::UpdateTransformBuffer();

	//HeapCreation
	_heap->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 3);

	//ViewCreation
	{
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _transformBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(Transform)));

			_transformBuffer.CreateView(view, _heap->GetCPUHandle(_transformStartPoint));
		}

		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC view{};
			view.BufferLocation = _materialBuffer.GetResource()->GetGPUVirtualAddress();
			view.SizeInBytes = static_cast<unsigned int>(Util::Alignment256Bytes(sizeof(SpriteInfo)));
			_materialBuffer.CreateView(view, _heap->GetCPUHandle(_materialStartPoint));
		}

		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			// 寿命切れリンク切れ判定
			if (_shaderResource.expired()) {
				//もし寿命切れなら
				_shaderResource = TextureManager::GetInstance().GetNullTextureShaderResource();
			}

			srvDesc.Format = _shaderResource.lock()->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			_shaderResource.lock()->CreateView(srvDesc, _heap->GetCPUHandle(_textureStartPoint));

			_width = static_cast<unsigned int>(_shaderResource.lock()->GetResourceDesc()->Width);
			_height = static_cast<unsigned int>(_shaderResource.lock()->GetResourceDesc()->Height);
		}
	}

	//SetBundleDrawCall
	{
		RegistBundle();
	}
}

void K3D12::Sprite::RegistBundle()
{

	_bundleList.GetCommandList()->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	BindingShaderObjectToBundle();
	BindingIndexBuffer(&_bundleList);
	BindingVertexBuffer(&_bundleList);
	BindingDescriptorHeaps(&_bundleList);

	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(1, _heap->GetGPUHandle(_transformStartPoint)); //自身の位置情報
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(2, _heap->GetGPUHandle(_materialStartPoint));	//自身のアルファ乗算値と切り取り矩形
	_bundleList.GetCommandList()->SetGraphicsRootDescriptorTable(3, _heap->GetGPUHandle(_textureStartPoint));	//テクスチャ情報

	_bundleList.GetCommandList()->DrawIndexedInstanced(static_cast<unsigned int>(this->_list.size()), 1, 0, 0, 0);

	_bundleList.GetCommandList()->Close();
}

void K3D12::Sprite::SetPipelineState(PipelineStateObject * pipelineState)
{
	_pipelineState.reset(pipelineState);
}

void K3D12::Sprite::SetRootSignature(RootSignature * rootSignature)
{
	_rootSignature.reset(rootSignature);
}

void K3D12::Sprite::AttachTexture(std::string path)
{
	TextureLoader::GetInstance().LoadSpriteTexture(path, _heap->GetCPUHandle(_textureStartPoint));	//ロード処理
	_shaderResource = TextureManager::GetInstance().GetSpriteShaderResource(path).lock();			//テクスチャゲット
}

void K3D12::Sprite::SetRect(float top, float left, float bottom, float right)
{

	float pos[4] = { left,right,top,bottom };
	for (int i = 0; i < _vertexes.size(); i++) {
		_vertexes[i].texCoord = Vector2(pos[i % 2], pos[(i) % _vertexes.size() / 2 + 2]);
	}

}

void K3D12::Sprite::SetRect(Vector2 topLeft, Vector2 bottomRight)
{

	float pos[4] = { topLeft.x,bottomRight.x,topLeft.y,bottomRight.y };
	for (int i = 0; i < _vertexes.size(); i++) {
		_vertexes[i].texCoord = Vector2(pos[i % 2], pos[(i) % _vertexes.size() / 2 + 2]);
	}

}

void K3D12::Sprite::Translate(Vector2 pos)
{
	GameObject::Translate(Vector3(pos.x, pos.y, 0.0f));
}

void K3D12::Sprite::Move(Vector2 velocity)
{
	K3D12::GameObject::Move(Vector3(velocity.x, velocity.y, 0.0f));
}

Vector2 K3D12::Sprite::GetPos()
{
	return Vector2(GameObject::GetPos().x, GameObject::GetPos().y);
}

void K3D12::Sprite::SetScale(Vector2 scale)
{
	K3D12::GameObject::SetScale(Vector3(scale.x, scale.y, 0.0f));

}

Vector2 K3D12::Sprite::GetScale()
{
	return Vector2(GameObject::GetScale().x, GameObject::GetScale().y);
}

void K3D12::Sprite::Rotate(float deg)
{
	K3D12::GameObject::RotationEulerAngles(Vector3(0.0f, deg, 0.0f));
}

void K3D12::Sprite::Rotate(Quaternion quaternion)
{
	K3D12::GameObject::Rotation(quaternion);
}

void K3D12::Sprite::Update()
{
	K3D12::GameObject::UpdateTransformBuffer();
}

void K3D12::Sprite::LateUpdate()
{

}
