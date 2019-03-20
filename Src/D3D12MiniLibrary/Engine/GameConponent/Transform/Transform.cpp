#include <utility>
#include "Transform.h"
#include "../../Util/Math.h"



///
///
///
///DirectXのワールド変換行列計算は通常の逆の順序で行う
///　→各頂点　＊　回転行列　＊　移動→　みたいな感じで
///逆にHLSLは通常の計算同様の順序で行う
///	 ←移動　＊	回転行列　＊　各頂点←
///
//検証１：ローカルのモデルに行列を掛ける
//モデルは自分の空間（ローカル空間）に存在する
//モデルのポリゴンを構成する「頂点座標」の値がローカル空間の座標系で表現されているはずなので
//このモデルに対して座標変換行列Mを適用すると、このモデルの頂点座標は何らかの別の位置に移動する。
///
//DirectXの行列形式では、右へ右へと座標変換行列を掛け算していくと、
//それらは全て変換先に考えている世界の座標系を基準とする事になる
///
//なので、今のままだと世界軸基準になるので、ローカル基準になるように変更する
///
///

K3D12::Transform::Transform() :
	_pos(), _rotation(Quaternion::CreateIdentity())
{

}


K3D12::Transform::~Transform()
{
	_pos = Vector3::zero;
	_rotation = Quaternion::CreateIdentity();


}


void K3D12::Transform::SetPos(const Vector3  pos)
{
	_pos = pos;
}

void K3D12::Transform::SetRotation(const Quaternion rotation)
{
	_rotation = rotation;
	_eulerAngles = _rotation.EulerAngles();
}

void K3D12::Transform::SetEulerAngles(const Vector3 eulerAugles)
{
	_eulerAngles = eulerAugles;
	_rotation = std::move(Quaternion::CreateFromEulerAngles(eulerAugles));
}

Vector3 K3D12::Transform::GetPos()
{
	return _pos;
}

Quaternion  K3D12::Transform::GetRotation()
{
	return	_rotation;
}

Vector3 K3D12::Transform::GetEulerAngles()
{
	return _rotation.EulerAngles();
}

void K3D12::Transform::Translate(const Vector3 & velocity)
{
	_pos += velocity;
}

void K3D12::Transform::Move(const Vector3 & velocity)
{
	_pos += Vector3::Rotate(velocity, _rotation);

}

void K3D12::Transform::Rotation(const Quaternion & rot)
{
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void K3D12::Transform::RotationLocalWorld(const Quaternion & rot)
{
	//ローカル軸で回転させたい
	auto axis = GetLocalAxis();


}

void K3D12::Transform::RotationLocalWorld(const Vector3 & eulerAngles)
{
	//ローカル軸で回転させたい
	auto axis = GetLocalAxis();
	auto uRot = std::move(Quaternion::CreateFromAxisAngles(axis.u, DegToRad(eulerAngles.x)));
	auto vRot = std::move(Quaternion::CreateFromAxisAngles(axis.v, DegToRad(eulerAngles.y)));
	auto wRot = std::move(Quaternion::CreateFromAxisAngles(axis.w, DegToRad(eulerAngles.z)));

}

void K3D12::Transform::RotationEulerAngles(const Vector3 & eulerAngles)
{
	_eulerAngles += eulerAngles; 
	_rotation = std::move(Quaternion::CreateFromEulerAngles(_eulerAngles));
}

void K3D12::Transform::RotationAxisAngles(const Vector3 & axis, float rad)
{
	_rotation *= std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void K3D12::Transform::RotateAround(const Vector3 & point, const Quaternion & rot)
{
	auto v = std::move(_pos - point);
	auto vv = std::move(Vector3::Rotate(v, rot));
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
	_pos = std::move(vv + (point));
}

void K3D12::Transform::RotateAround(const Vector3 & point, const Vector3 & axis, float rad)
{
	auto q = std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	RotateAround(point, q);
}


void K3D12::Transform::LookAt(const Vector3 & target, const Vector3 & up)
{
	//ターゲットをローカル空間に持っていく
	Vector3 localTargetPos = target;
	localTargetPos = Vector3::Transform(localTargetPos, GetView());
	_rotation = std::move(Quaternion::CreateFromRotationMatrix(Matrix::ExtractRotationMatrix((Matrix::CreateLookAt(Vector3::zero, localTargetPos, up)))).Normalize());
	_eulerAngles = _rotation.EulerAngles();
}

OrthonormalBasis K3D12::Transform::GetLocalAxis()
{
	OrthonormalBasis orthonomal;
	auto mat = std::move(Matrix::CreateFromQuaternion(_rotation));
	orthonomal.u = Vector3(mat.axisX).Normalize();
	orthonomal.v = Vector3(mat.axisY).Normalize();
	orthonomal.w = Vector3(mat.axisZ).Normalize();
	return orthonomal;
}

Matrix K3D12::Transform::GetView()
{
	//回転行列
	auto mat = Matrix::CreateFromQuaternion(_rotation);

	mat._41 = _pos.x;
	mat._42 = _pos.y;
	mat._43 = _pos.z;
	mat._44 = 1.0f;

	mat = Matrix::Invert(mat);
	//mat.EpsilonCheck();

	return mat;
}

