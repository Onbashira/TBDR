#pragma once
#include "../Util/Math.h"
namespace K3D12 {

	struct RendererMaterial
	{
		//拡散反射光 w = alpha
		Vector4 diffuse;
		//鏡面反射光	w = pow
		Vector4 specular;
		//環境光
		Vector3 ambient;
		//自己発光色
		Vector3 emissive;
	};

	struct RendererPBRMaterial
	{
		Vector4 albedo;		//アルベド
		float reflectivity; //金属質感
		float roughness; //microsuface値
	};
}