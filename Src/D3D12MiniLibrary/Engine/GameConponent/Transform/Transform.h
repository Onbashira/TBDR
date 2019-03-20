#pragma once
#include "../../Util/Math.h"

namespace K3D12 {

	///オブジェクトの幾何
	class Transform
	{
	private:

	protected:
		///ルートオブジェクトの位置
		Vector3 _pos;

		///ルートオブジェクトの任意軸と回転量
		Quaternion _rotation;

		///保持オイラー角
		Vector3 _eulerAngles;

	public:

	private:

	protected:

	public:

		void SetPos(const Vector3 pos);
		void SetRotation(const Quaternion rotation);
		void SetEulerAngles(const Vector3 euler);

		Vector3 GetPos();
		Quaternion GetRotation();
		Vector3 GetEulerAngles();

		///@fn				オブジェクトの移動（ローカル軸基準）
		///@param			(velocity) 変化量	
		void				Translate(const Vector3& velocity);


		///@fn				オブジェクトの移動（ワールド軸基準）
		///@param			(velocity) 変化量	
		void				Move(const Vector3& velocity);


		///@fn				原点中心回転
		///@param			(rot) 四元数	
		void				Rotation(const Quaternion& rot);

		///@fn				ローカル中心回転
		///@param			(rot) 四元数	
		void				RotationLocalWorld(const Quaternion& rot);

		///@fnオイラー角で原点中心回転
		///@param			(eulerAngles) 変化量	
		void				RotationLocalWorld(const Vector3& eulerAngles);


		///@fnオイラー角で原点中心回転
		///@param			(eulerAngles) 絶対量	
		void				RotationEulerAngles(const Vector3& eulerAngles);

		///@fn軸と回転量で回転
		///@param			(axis)	軸
		///@param			(rad)	弧度
		void				RotationAxisAngles(const Vector3& axis, float rad);

		///@fnポイント周りに軸と回転量で回転
		///@param			(point)	世界座標
		///@param			(rot)	回転量
		void				RotateAround(const Vector3& point, const Quaternion& rot);
		///@fnポイント周りに軸と回転量で回転
		///@param			(point)	世界座標
		///@param			(axis)	軸
		///@param			(rad)	弧度
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		///@fnオブジェクトのローカルForward軸がポイントを見るように回転
		void				LookAt(const Vector3& point, const Vector3& up);

		///@fnワールド座標で見たローカル軸の取得
		OrthonormalBasis	GetLocalAxis();

		///@fnこのオブジェクトのビュー行列を取得
		Matrix				GetView();

		Transform();
		virtual ~Transform();
	};
}