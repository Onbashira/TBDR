#pragma once
#include <memory>
#include "../Transform/Transform.h"
#include "../../Resource/ConstantBuffer.h"
#include "../../DescriptorHeap/DescriptorHeap.h"

namespace K3D12 {

	class GraphicsCommandList;

	class GameObject
	{
	private:

	protected:
		Transform		_transform;
		ConstantBuffer	_transformBuffer;
		Vector3			_scale;
		unsigned int	_initializeSize;
		bool			_isEnable;

	public:

	private:

	protected:

		//派生クラスでトランスフォームのバッファーへの書き込み方を定義してほしい
		virtual void UpdateTransformBuffer();
		//バッファリソースにSize分の1次元バッファとして領域を確保する；
		virtual void InitalizeTransformBuffer(size_t size);
		//シェーダーにバインディング
		virtual void SetTransform(int rootParamaterIndex, std::shared_ptr<GraphicsCommandList> list);
	public:

		virtual void Update() = 0;

		void SetPos(const Vector3 pos);

		void SetRotation(const Quaternion rotation);

		void SetEulerAngles(const Vector3 euler);

		void SetScale(const Vector3 scale);

		Vector3 GetPos();

		Quaternion GetRotation();

		Vector3 GetEulerAngles();

		Vector3 GetScale();

		void Enable();

		void Disable();

		bool IsEnable();

		//@fn				オブジェクトの移動（ワールド軸基準）
		//@param			(velocity) 変化量	
		void				Translate(const Vector3& velocity);

		//@fn				オブジェクトの移動（ローカル軸基準）
		//@param			(velocity) 変化量	
		void				Move(const Vector3& velocity);

		//@fn				原点中心回転(四元数の積で計算)
		//@param			(rot) 変化量	
		void				Rotation(const Quaternion& rot);

		//@fnオイラー角で原点中心回転
		//@param			(eulerAngles) 絶対量	
		void				RotationEulerAngles(const Vector3& eulerAngles);

		//@fn軸と回転量で回転
		//@param			(axis)	軸
		//@param			(rad)	弧度
		void				RotationAxisAngles(const Vector3& axis, float rad);

		//@fnポイント周りに軸と回転量で回転
		//@param			(point)	座標
		//@param			(rot)	変化量
		void				RotateAround(const Vector3& point, const  Quaternion& rot);

		//@fnポイント周りに軸と回転量で回転
		//@param			(point)	座標
		//@param			(axis)	軸
		//@param			(rad)	弧度
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		//@fnオブジェクトのローカルForward軸がポイントを見るように回転
		void				LookAt(const Vector3& point, const Vector3& up);

		//@fnワールド座標で見たローカル軸の取得
		OrthonormalBasis	GetLocalAxis();

		//@fnこのオブジェクトのビュー行列を取得
		Matrix				GetView();

		//@fnこのオブジェクトのSRT行列を取得
		Matrix				GetSRTMatrix();

		GameObject();
		GameObject(Transform transform);

		virtual ~GameObject();
	};

}
