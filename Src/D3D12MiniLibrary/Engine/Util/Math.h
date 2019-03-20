#pragma once
#include <comdef.h>
#include <DirectXMath.h>

namespace {
	//--------------------------------------------------------------------------------------------------
	//Constant Variables
	//--------------------------------------------------------------------------------------------------
	static constexpr float F_PI = 3.1415926535897932384626433832795f;		//	 π
	static constexpr float F_2PI = 6.283185307179586476925286766559f;		//	 2π
	static constexpr float F_1DIVPI = 0.31830988618379067153776752674503f;  //	 1/π
	static constexpr float F_PIDIV2 = 1.5707963267948966192313216916398f;   //	 π/2
	static constexpr float F_PIDIV3 = 1.0471975511965977461542144610932f;   //	 π/3
	static constexpr float F_PIDIV4 = 0.78539816339744830961566084581988f;  //	 π/4
	static constexpr float F_PIDIV6 = 0.52359877559829887307710723054658f;  //	 π/6
	static constexpr float F_EPSILON = 1.192092896e-07f;                    //	 マシンイプシロン(float)

	static constexpr double D_PI = 3.1415926535897932384626433832795;		//	 π
	static constexpr double D_2PI = 6.283185307179586476925286766559;		//	 2π
	static constexpr double D_1DVIPI = 0.31830988618379067153776752674503;	//	 1/π
	static constexpr double D_PIDIV2 = 1.5707963267948966192313216916398;	//	 π/2
	static constexpr double D_PIDIV3 = 1.0471975511965977461542144610932;	//	 π/3
	static constexpr double D_PIDIV4 = 0.78539816339744830961566084581988;	//	 π/4
	static constexpr double D_PIDIV6 = 0.52359877559829887307710723054658;	//	 π/6
	static constexpr double D_EPSILON = 2.2204460492503131e-016;			//	 マシンイプシロン(double)

	static constexpr float ONB_EPSILON = 0.01f;                             // 正規直交規定を算出する際に用いるイプシロン値

																			//二つの値から大きい値を求める
	template< typename T >inline T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	};

	//二つの値から小さい値を求める
	template< typename T >inline T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	};

	//値を最大値と最小値の範囲内にクランピングする
	template< typename T >inline T Clamp(const T& value, const T& mini, const T& maxi)
	{
		return Max(mini, Min(maxi, value));
	};

	//与えられた値を0～1に制限する
	template< typename T >inline T Saturate(const T& value)
	{
		return Clamp(value, static_cast<T>(0), static_cast<T>(1));
	};

	//符号付かどうかを判定
	template< typename T >inline T Sign(const T& value) {
		return (value < static_cast<T>(0)) ? static_cast<T>(-1) : static_cast<T>(1);
	};

}

//--------------------------------------------------------------------------------------------------
// Forward Declarations
//--------------------------------------------------------------------------------------------------
struct Vector2;
struct Vector3;
struct Vector4;
struct Quaternion;
struct Matrix;
//--------------------------
//Utility
//--------------------------

//角度変換　度→ラジアン
float		DegToRad(float deg);
//角度変換　度→ラジアン
float		RadToDeg(float rad);

//値がゼロであるかどうか
bool		IsZero(float value);
bool		IsZero(double value);

//値が等価かどうか
bool		IsEqual(float a, float b);
bool		IsEqual(double a, double b);

//非数かどうか
bool		IsNan(float value);

//無限大かどうか
bool		IsInf(float value);

//階乗
unsigned int Fact(unsigned int value);

//二重階乗
unsigned int DoubleFact(unsigned int value);

//バーンスタイン
float		Bernstein(unsigned int n, unsigned int i, float t);

//二項係数
float		Binormal(unsigned int n, unsigned int k);

//順列
unsigned int Perm(unsigned int n, unsigned int r);
//組み合わせ
unsigned int Comb(unsigned int n, unsigned int r);

//ちょっと早いSQRT
float		QuickSqrt(float value);

//フレネル項
float		Fresnel(float n1, float n2, float cosTheta);
double		Fresnel(double n1, double n2, double cosTheta);

//線形補間
float		Lerp(float a, float b, float amount);
double		Lerp(double a, double b, double amount);

//--------------------------
//Vector2
//--------------------------
struct  Vector2 {
	//==================================
	//フレンド
	//==================================
	friend Vector2 operator* (float scalar, const Vector2& value);
public:
	//==================================
	//メンバ
	//==================================
	float x;
	float y;

	static const Vector2 up;
	static const Vector2 left;
	static const Vector2 down;
	static const Vector2 right;
	static const Vector2 zero;
	static const Vector2 one;

private:
	//==================================
	//メソッド
	//==================================
public:
	Vector2();
	Vector2(const float x, const float y);
	Vector2(const Vector2& other);
	Vector2(Vector2&& other);
	~Vector2();

	Vector2& operator = (const Vector2& value);
	Vector2& operator = (Vector2&& value);
	Vector2& operator +=(const Vector2& value);
	Vector2& operator -=(const Vector2& value);
	Vector2& operator *=(float scalar);
	Vector2& operator /=(float scalar);
	Vector2  operator + ()const;
	Vector2	 operator - ()const;
	Vector2	 operator + (const Vector2& value)const;
	Vector2	 operator - (const Vector2& value)const;
	Vector2  operator * (float scalar);
	Vector2  operator / (float scalar);
	bool	 operator== (const Vector2& value)const;
	bool	 operator!= (const Vector2& value)const;

	//	ベクトルの大きさを求める
	float Length()const;

	//	ベクトルの大きさの二乗値を求める
	float LengthSqr()const;

	//正規化
	Vector2& Normalize();

	//セーフな正規化
	Vector2& SafeNormalize(const Vector2& value);

	//各成分の絶対値の取得
	static Vector2	Abs(const Vector2& value);
	static void		Abs(const Vector2& value, Vector2& result);

	//与えられた範囲内にクランピング
	static Vector2	Clamp(const Vector2& value, const Vector2& min, const Vector2& max);
	static void		Clamp(const Vector2& value, const Vector2& min, const Vector2& max, Vector2& result);

	//値を0～１に制限する
	static Vector2	Saturate(const Vector2& value);
	static void		Saturate(const Vector2& value, Vector2& result);

	//二つのベクトル間の距離を求める
	static float	Distance(const Vector2& a, const Vector2& b);
	static void		Distance(const Vector2& a, const Vector2& b, float& result);

	//二つのベクトル間のの距離の二乗値を求める
	static float	DistanceSqr(const Vector2& a, const Vector2& b);
	static void		DistanceSqr(const Vector2& a, const Vector2& b, float& result);

	//内積
	static float	Dot(const Vector2& a, const Vector2& b);
	static void		Dot(const Vector2& a, const Vector2& b, float& result);

	//与えられた値の正規化
	static Vector2	Normalize(const Vector2& value);
	static void		Normalize(const Vector2& value, Vector2& result);

	//与えられた値から正規化を試みる
	static Vector2	SafeNormalize(const Vector2& value, const Vector2& set);
	static void		SafeNormalize(const Vector2& value, const Vector2& set, Vector2& result);

	//交差角を求める
	static float	ComputeCrossingAngle(const Vector2& a, const Vector2& b);
	static void		ComputeCrossingAngle(const Vector2& a, const Vector2& b, float result);

	//各成分の最小値を求めます
	static Vector2	Min(const Vector2& a, const Vector2& b);
	static void		Min(const Vector2& a, const Vector2& b, Vector2& result);

	//各成分の最大値を求めます
	static Vector2	Max(const Vector2& a, const Vector2& b);
	static void		Max(const Vector2& a, const Vector2& b, Vector2& result);

	//入射ベクトルと法線から反射ベクトルを求める
	static Vector2	Reflect(const Vector2& in, const Vector2& norm);
	static void		Reflect(const Vector2& in, const Vector2& norm, Vector2& result);

	//入射ベクトルと法線と屈折角から屈折ベクトルを求める
	static Vector2	Reflect(const Vector2& in, const Vector2& norm, const float eta);
	static void		Reflect(const Vector2& in, const Vector2& norm, const float eta, Vector2& result);

	//重心座標を求める
	static Vector2	Barycentric(const Vector2& a, const Vector2& b, const Vector2& c, const float f, const float g);
	static void		Barycentric(const Vector2& a, const Vector2& b, const Vector2& c, const float f, const float g, Vector2& result);

	//エルミートスプライン補間を行う
	static Vector2	Hermite(const Vector2& a, const Vector2& t1, const Vector2& b, const Vector2& t2, const float amount);
	static void		Hermite(const Vector2& a, const Vector2& t1, const Vector2& b, const Vector2& t2, const float amount, Vector2& result);

	//Catmull-Rom補間を行う
	static Vector2  CatmullRom(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, const float amount);
	static void		CatmullRom(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, const float amount, Vector2& result);

	//線形補間を行う
	static Vector2	Lerp(const Vector2& a, const Vector2& b, const float amount);
	static void		Lerp(const Vector2& a, const Vector2& b, const float amount, Vector2& result);

	//三次方程式より、二つの値を補完する
	static Vector2	SmoothStep(const Vector2& a, const Vector2& b, const float amount);
	static void		SmoothStep(const Vector2& a, const Vector2& b, const float amount, Vector2 &result);

	//行列を用いて変換
	static Vector2	Transform(const Vector2& position, const Matrix& matrix);
	static void		Transform(const Vector2& position, const Matrix& matrix, Vector2 &result);

	//行列を用いて法線を変換
	static Vector2	TransformNormal(const Vector2& normal, const Matrix& matrix);
	static void		TransformNormal(const Vector2& normal, const Matrix& matrix, Vector2 &result);

	//行列を用いてベクトルを変換し，変換結果をw=1に射影
	static Vector2	TransformCoord(const Vector2& coords, const Matrix& matrix);
	static void		TransformCoord(const Vector2& coords, const Matrix& matrix, Vector2 &result);

private:

};

//==================================
//Vector3
//==================================
struct  Vector3 {
	//==================================
	//フレンド
	//==================================
	friend Vector3 operator* (float scalar, const Vector3& value);
	friend DirectX::XMFLOAT3 operator +(DirectX::XMFLOAT3& a, const Vector3& b);
	friend DirectX::XMFLOAT3 operator -(DirectX::XMFLOAT3& a, const Vector3& b);

public:
	//==================================
	//メンバ
	//==================================
	float x;
	float y;
	float z;

	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;
	static const Vector3 down;
	static const Vector3 back;
	static const Vector3 left;
	static const Vector3 zero;
	static const Vector3 one;

private:
	//==================================
	//メソッド
	//==================================
public:
	Vector3();
	Vector3(const float x, const float y, const float z);
	Vector3(const Vector3& nVec);
	Vector3(Vector3&& other);
	Vector3(const Vector4& nVec);
	Vector3(Vector4&& other);
	Vector3(const DirectX::XMFLOAT3& other);
	Vector3(DirectX::XMFLOAT3&& other);
	~Vector3();

	Vector3& operator = (const Vector3& value);
	Vector3& operator = (Vector3&& value);
	Vector3& operator = (const Vector4& value);
	Vector3& operator = (Vector4&& value);

	Vector3& operator = (const DirectX::XMFLOAT3& value);
	Vector3& operator = (DirectX::XMFLOAT3&& value);
	Vector3& operator +=(const Vector3& value);
	Vector3& operator +=(const DirectX::XMFLOAT3& value);
	Vector3& operator -=(const Vector3& value);
	Vector3& operator -=(const DirectX::XMFLOAT3& value);
	Vector3& operator *=(float scalar);
	Vector3& operator /=(float scalar);
	Vector3  operator + ()const;
	Vector3	 operator - ()const;
	Vector3	 operator + (const Vector3& value)const;
	Vector3	 operator + (const DirectX::XMFLOAT3& value)const;
	Vector3	 operator - (const Vector3& value)const;
	Vector3	 operator - (const DirectX::XMFLOAT3& value)const;
	Vector3  operator * (float scalar);
	Vector3  operator / (float scalar);
	bool	 operator== (const Vector3& value)const;
	bool	 operator== (const DirectX::XMFLOAT3& value)const;
	bool	 operator!= (const Vector3& value)const;
	bool	 operator!= (const DirectX::XMFLOAT3& value)const;

	//ベクトルの大きさを取得
	float Length()const;

	//ベクトルの大きさの二乗値を取得
	float LengthSqr()const;

	//正規化
	Vector3& Normalize();

	//正規化を試みる
	Vector3& SafeNormalize(const Vector3& value);

	//各成分の絶対値を取得
	static Vector3				Abs(const Vector3& value);
	static void					Abs(const Vector3& value, Vector3& result);

	//与えられた値を範囲をクランプ
	static Vector3				Clamp(const Vector3& value, const Vector3& min, const Vector3& max);
	static void					Clamp(const Vector3& value, const Vector3& min, const Vector3& max, Vector3& result);

	//与えられた空間ベクトルを長さでクランプ
	static Vector3				ClampLength(const Vector3& value, float min, float max);
	static Vector3				ClampLength(const Vector3& value, float min, float max, Vector3& result);

	//与えられた値を0～1で制限する
	static Vector3				Saturate(const Vector3& value);
	static void					Saturate(const Vector3& value, Vector3& result);

	//二つのベクトル間の距離を求める
	static float				Distance(const Vector3& a, const Vector3& b);
	static void					Distance(const Vector3& a, const Vector3& b, float& result);

	//二つのベクトル間の距離の二乗値を求める
	static float				DistanceSqr(const Vector3& a, const Vector3& b);
	static void					DistanceSqr(const Vector3& a, const Vector3& b, float& result);

	//内積
	static float				Dot(const Vector3& a, const Vector3& b);
	static void					Dot(const Vector3& a, const Vector3& b, float& result);

	//外積
	static Vector3				Cross(const Vector3& a, const Vector3& b);
	static void					Cross(const Vector3& a, const Vector3& b, Vector3& result);

	//正規化
	static Vector3				Normalize(const Vector3& value);
	static void					Normalize(const Vector3& value, Vector3& result);

	//正規化を試みる
	static Vector3				SafeNormalize(const Vector3& value, const Vector3& set);
	static void					SafeNormalize(const Vector3& value, const Vector3& set, Vector3& result);

	//法線を求める
	static Vector3				ComputeNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
	static void					ComputeNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3, Vector3& result);

	//クアッドの法線を求める
	static Vector3				ComputeQuadNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
	static void					ComputeQuadNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, Vector3& result);

	//交差角を求める
	static float				ComputeCrossingAngle(const Vector3& a, const Vector3& b);
	static void					ComputeCrossingAngle(const Vector3& a, const Vector3& b, float result);

	//各成分の最小値を求める
	static Vector3				Min(const Vector3& a, const Vector3& b);
	static void					Min(const Vector3& a, const Vector3& b, Vector3& result);

	//各成分の最大値を求める
	static Vector3				Max(const Vector3& a, const Vector3& b);
	static void					Max(const Vector3& a, const Vector3& b, Vector3& result);

	//入射ベクトルと法線で反射ベクトルを求める
	static Vector3				Reflect(const Vector3& in, const Vector3& norm);
	static void					Reflect(const Vector3& in, const Vector3& norm, Vector3& result);

	//入射ベクトルと法線と屈折角から屈折ベクトルを求める
	static Vector3				Reflect(const Vector3& in, const Vector3& norm, const float eta);
	static void					Reflect(const Vector3& in, const Vector3& norm, const float eta, Vector3& result);

	//重心を求める
	static Vector3				Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const float f, const float g);
	static void					Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const float f, const float g, Vector3& result);

	//エルミートスプライン補間をおこなう
	static Vector3				Hermite(const Vector3& a, const Vector3& t1, const Vector3& b, const Vector3& t2, const float amount);
	static void					Hermite(const Vector3& a, const Vector3& t1, const Vector3& b, const Vector3& t2, const float amount, Vector3& result);

	//Catmull-Rom曲線補間を行う
	static Vector3				CatmullRom(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, const float amount);
	static void					CatmullRom(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, const float amount, Vector3& result);

	//線形補間を行う
	static Vector3				Lerp(const Vector3& a, const Vector3& b, const float amount);
	static void					Lerp(const Vector3& a, const Vector3& b, const float amount, Vector3& result);

	//二つの値を補完する
	static Vector3				SmoothStep(const Vector3& a, const Vector3& b, const float amount);
	static void					SmoothStep(const Vector3& a, const Vector3& b, const float amount, Vector3 &result);

	//行列を用いて変換する
	static Vector3				Transform(const Vector3& position, const Matrix& matrix);
	static void					Transform(const Vector3& position, const Matrix& matrix, Vector3 &result);

	//行列を用いて法線を変換する
	static Vector3				TransformNormal(const Vector3& normal, const Matrix& matrix);
	static void					TransformNormal(const Vector3& normal, const Matrix& matrix, Vector3 &result);

	//行列を用いてベクトルを変換し，変換結果をw=1に射影
	static Vector3				TransformCoord(const Vector3& coords, const Matrix& matrix);
	static void					TransformCoord(const Vector3& coords, const Matrix& matrix, Vector3 &result);

	//スカラー三重積
	static float				ScalarTriple(const Vector3& a, const Vector3& b, const Vector3& c);
	static void					ScalarTriple(const Vector3& a, const Vector3& b, const Vector3& c, float& result);

	//ベクトル三重積
	static Vector3				VectorTriple(const Vector3& a, const Vector3& b, const Vector3& c);
	static void					VectorTriple(const Vector3& a, const Vector3& b, const Vector3& c, Vector3& result);

	//四元数で回転
	static Vector3				Rotate(const Vector3& value, const Quaternion& rotation);
	static void 				Rotate(const Vector3& value, const Quaternion& rotation, Vector3& result);

	//四元数で逆回転
	static Vector3				InverseRotate(const Vector3& value, const Quaternion& rotation);
	static void 				InverseRotate(const Vector3& value, const Quaternion& rotation, Vector3& result);

	//DirectXmathとの互換性
	static DirectX::XMFLOAT3	Store(const Vector3& value);
	static void					Store(const Vector3& value, DirectX::XMFLOAT3& result);

private:

};

//==================================
//Vector4
//==================================
struct  Vector4 {
	//==================================
	//フレンド
	//==================================
	friend Vector4 operator* (float scalar, const Vector4& value);

public:
	//==================================
	//メンバ
	//==================================

	float x;
	float y;
	float z;
	float w;
private:
	//==================================
	//メソッド
	//==================================
public:
	Vector4();
	Vector4(const Vector4& value);
	Vector4(Vector4&& other);
	Vector4(const DirectX::XMFLOAT4& value);
	Vector4(DirectX::XMFLOAT4&& other);
	Vector4(const Quaternion& value);
	Vector4(Quaternion&& other);
	Vector4(const float x, const float y, const float z, float w);
	Vector4(const Vector2& value, const float z, float w);
	Vector4(const DirectX::XMFLOAT2& value, const float z, float w);
	Vector4(const Vector3& value, float w);
	Vector4(const DirectX::XMFLOAT3& value, float w);
	~Vector4();

	Vector4& operator = (const Vector4& value);
	Vector4& operator = (Vector4&& value);
	Vector4& operator = (const Vector3& value);
	Vector4& operator = (const DirectX::XMFLOAT4& value);
	Vector4& operator = (DirectX::XMFLOAT4&& value);
	Vector4& operator = (Quaternion&& value);
	Vector4& operator +=(const Vector4& value);
	Vector4& operator +=(DirectX::XMFLOAT4& value);
	Vector4& operator -=(const Vector4& value);
	Vector4& operator -=(DirectX::XMFLOAT4& value);
	Vector4& operator *=(float scalar);
	Vector4& operator /=(float scalar);
	Vector4  operator + ()const;
	Vector4	 operator - ()const;
	Vector4	 operator + (const Vector4& value)const;
	Vector4	 operator + (const DirectX::XMFLOAT4& value)const;
	Vector4	 operator - (const Vector4& value)const;
	Vector4	 operator - (const DirectX::XMFLOAT4& value)const;
	Vector4  operator * (float scalar);
	Vector4  operator / (float scalar);
	bool	 operator== (const Vector4& value)const;
	bool	 operator== (const DirectX::XMFLOAT4& value)const;
	bool	 operator!= (const Vector4& value)const;
	bool	 operator!= (const DirectX::XMFLOAT4& value)const;

	//ベクトルの大きさを取得
	float Length()const;
	//ベクトルの大きさの二乗値を取得
	float LengthSqr()const;

	//正規化
	Vector4& Normalize();
	//正規化を試みる
	Vector4& SafeNormalize(const Vector4& set);

	//XYZ成分の取得

	//各成分の絶対値を求める
	static Vector4	Abs(const Vector4& value);
	static void		Abs(const Vector4& value, Vector4& result);

	//与えられた値を範囲でクランピングする
	static Vector4	Clamp(const Vector4& value, const Vector4& min, const Vector4& max);
	static void		Clamp(const Vector4& value, const Vector4& min, const Vector4& max, Vector4& result);

	//与えられた値を0～1で制限する
	static Vector4	Saturate(const Vector4& value);
	static void		Saturate(const Vector4& value, Vector4& result);

	//二つのベクトル間の距離を求める
	static float	Distance(const Vector4& a, const Vector4& b);
	static void		Distance(const Vector4& a, const Vector4& b, float& result);

	//二つのベクトル間の距離の二乗値を求める
	static float	DistanceSqr(const Vector4& a, const Vector4& b);
	static void		DistanceSqr(const Vector4& a, const Vector4& b, float& result);

	//内積
	static float	Dot(const Vector4& a, const Vector4& b);
	static void		Dot(const Vector4& a, const Vector4& b, float& result);

	//正規化
	static Vector4	Normalize(const Vector4& value);
	static void		Normalize(const Vector4& value, Vector4& result);

	//正規化を試みる
	static Vector4	SafeNormalize(const Vector4& value, const Vector4& set);
	static void		SafeNormalize(const Vector4& value, const Vector4& set, Vector4& result);

	//交差角を求める
	static float	ComputeCrossingAngle(const Vector4& a, const Vector4& b);
	static void		ComputeCrossingAngle(const Vector4& a, const Vector4& b, float result);

	//各成分の最小値をもとめる
	static Vector4	Min(const Vector4& a, const Vector4& b);
	static void		Min(const Vector4& a, const Vector4& b, Vector4& result);

	//各成分の最大値をもとめる　
	static Vector4	Max(const Vector4& a, const Vector4& b);
	static void		Max(const Vector4& a, const Vector4& b, Vector4& result);

	//重心を求める
	static Vector4	Barycentric(const Vector4& a, const Vector4& b, const Vector4& c, const float f, const float g);
	static void		Barycentric(const Vector4& a, const Vector4& b, const Vector4& c, const float f, const float g, Vector4& result);

	//エルミートスプライン補間を行う
	static Vector4	Hermite(const Vector4& a, const Vector4& t1, const Vector4& b, const Vector4& t2, const float amount);
	static void		Hermite(const Vector4& a, const Vector4& t1, const Vector4& b, const Vector4& t2, const float amount, Vector4& result);

	//Catmull-Romスプライン補間を行う
	static Vector4  CatmullRom(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const float amount);
	static void		CatmullRom(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const float amount, Vector4& result);

	//線形補間を行う
	static Vector4	Lerp(const Vector4& a, const Vector4& b, const float amount);
	static void		Lerp(const Vector4& a, const Vector4& b, const float amount, Vector4& result);

	//二つの値を補完する
	static Vector4	SmoothStep(const Vector4& a, const Vector4& b, const float amount);
	static void		SmoothStep(const Vector4& a, const Vector4& b, const float amount, Vector4 &result);

	//行列を用いて変換する
	static Vector4	Transform(const Vector4& position, const Matrix& matrix);
	static void		Transform(const Vector4& position, const Matrix& matrix, Vector4 &result);
private:

};

//==================================
//Matrix
//==================================
struct  Matrix {
	//==============================
	//フレンド
	//==============================
	friend Matrix operator * (float scalar, const Matrix& matrix);
	//==============================
	//メンバ
	//==============================
public:
	union {
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		struct
		{
			Vector4 axisX;
			Vector4 axisY;
			Vector4 axisZ;
			Vector4 axisW;
		};

		float matrix[4][4];
	};
private:
	//==============================
	//メソッド
	//==============================


public:
	Matrix();
	Matrix(const Matrix& mat);
	Matrix(Matrix&& mat);
	Matrix(const DirectX::XMMATRIX& mat);
	Matrix(DirectX::XMMATRIX&& mat);
	Matrix(const float m11, const float m12, const float m13, const float m14,
		const float m21, const float m22, const float m23, const float m24,
		const float m31, const float m32, const float m33, const float m34,
		const float m41, const float m42, const float m43, const float m44
	);
	Matrix(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
	Matrix(const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2, const DirectX::XMFLOAT4& v3, const DirectX::XMFLOAT4& v4);
	~Matrix();

	float& operator()(unsigned int row, unsigned int col);
	const float& operator()(unsigned int row, unsigned int col) const;

	Matrix& operator = (const Matrix& value);
	Matrix& operator = (Matrix&& value);
	Matrix& operator = (const DirectX::XMMATRIX& value);
	Matrix& operator = (DirectX::XMMATRIX&& value);
	Matrix& operator *=(const Matrix& value);
	Matrix& operator *=(const DirectX::XMMATRIX& value);
	Matrix& operator +=(const Matrix& value);
	Matrix& operator +=(const DirectX::XMMATRIX& value);
	Matrix& operator -=(const Matrix& value);
	Matrix& operator -=(const DirectX::XMMATRIX& value);
	Matrix& operator *=(float scalar);
	Matrix& operator /=(float scalar);
	Matrix  operator + ()const;
	Matrix	operator - ()const;
	Matrix	operator * (const Matrix& value)const;
	Matrix	operator * (const DirectX::XMMATRIX& value)const;
	Matrix	operator + (const Matrix& value)const;
	Matrix	operator + (const DirectX::XMMATRIX& value)const;
	Matrix	operator - (const Matrix& value)const;
	Matrix	operator - (const DirectX::XMMATRIX& value)const;
	Matrix  operator * (float scalar);
	Matrix  operator / (float scalar);
	bool	operator== (const Matrix& value)const;
	bool	operator== (const DirectX::XMMATRIX& value)const;
	bool	operator!= (const Matrix& value)const;
	bool	operator!= (const DirectX::XMMATRIX& value)const;

	// 行列式を求める
	float						Determinant()const;

	//単位行列を求める
	Matrix&						Identity();

	//拡縮成分を抜き出す
	Vector3						Scale();

	//移動成分を抜き出す
	Vector3						Movement();

	//単位行列を作成
	static Matrix				CreateIdentity();

	//行列が単位行列か
	static bool					IsIdentity(const Matrix &value);

	//転置する
	static Matrix				Transpose(const Matrix& value);
	static void					Transpose(const Matrix& value, Matrix& result);

	//行列を乗算する
	static Matrix				Multiply(const Matrix& a, const Matrix& b);
	static void					Multiply(const Matrix& a, const Matrix& b, Matrix& result);
	static Matrix				Multiply(const Matrix& value, const float scalar);
	static void					Multiply(const Matrix& value, const float scalar, Matrix& result);

	//転置乗算
	static Matrix				MultiplyTranspose(const Matrix& a, const Matrix& b);
	static void					MultiplyTranspose(const Matrix& a, const Matrix& b, Matrix& result);

	//逆行列をもとめる
	static Matrix				Invert(const Matrix& value);
	static void					Invert(const Matrix& value, Matrix& result);

	//拡縮用行列の作成
	static Matrix				CreateScaleMatrix(const float scale);
	static void					CreateScaleMatrix(const float scale, Matrix&);
	static Matrix				CreateScaleMatrix(const float sx, const float sy, const float sz);
	static void					CreateScaleMatrix(const float sx, const float sy, const float sz, Matrix& result);
	static Matrix				CreateScaleMatrix(const Vector3& scale);
	static void					CreateScaleMatrix(const Vector3& scale, Matrix& result);

	//移動用行列の作成
	static Matrix				CreateTranslationMatrix(const float tx, const float ty, const float tz);
	static void					CreateTranslationMatrix(const float tx, const float ty, const float tz, Matrix& result);
	static Matrix				CreateTranslationMatrix(const Vector3& scale);
	static void					CreateTranslationMatrix(const Vector3& scale, Matrix& result);

	//X軸周りの回転行列の作成
	static Matrix				CreateRotationX(const float rad);
	static void					CreateRotationX(const float rad, Matrix& result);

	//Y軸周りの回転行列の作成
	static Matrix				CreateRotationY(const float rad);
	static void					CreateRotationY(const float rad, Matrix& result);

	//Z軸周りの回転行列の作成
	static Matrix				CreateRotationZ(const float rad);
	static void					CreateRotationZ(const float rad, Matrix& result);

	//四元数から行列を作成
	static Matrix				CreateFromQuaternion(const Quaternion& q);
	static void					CreateFromQuaternion(const Quaternion& q, Matrix& result);

	//軸とラジアン角から回転行列を作成
	static Matrix				CreateFromAxisAngles(const Vector3& axis, float rad);
	static void					CreateFromAxisAngles(const Vector3& axis, float rad, Matrix& result);

	//ヨーピッチロールから回転行列を作る
	static Matrix				CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll);
	static void					CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, Matrix& result);

	//ビュー行列を作成（ルックアット行列）
	static Matrix				CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& upward);
	static void					CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& upward, Matrix& result);

	//ルックアット行列をつくるビュー行列ではない
	static Matrix				CreateLookAtObject(const Vector3& position, const Vector3& target, const Vector3& upward, const Vector3& rightward);
	static void					CreateLookAtObject(const Vector3& position, const Vector3& target, const Vector3& upward, const Vector3& rightward, Matrix& result);

	//ビュー行列を作成(ルックトゥ行列)
	static Matrix				CreateLookTo(const Vector3& position, const Vector3& viewDir, const Vector3& upward);
	static void					CreateLookTo(const Vector3& position, const Vector3& viewDir, const Vector3& upward, Matrix& result);

	//ビュー行列から回転行列を抜き出す
	static Matrix				ExtractRotationMatrix(Matrix view);
	static void					ExtractRotationMatrix(Matrix view, Matrix& result);

	//高さと幅から射影行列を作成
	static Matrix				CreatePerspective(const float width, const float height, const float nearClip, const float farClip);
	static void					CreatePerspective(const float width, const float height, const float nearClip, const float farClip, Matrix& result);

	//FOVから射影行列を作成
	static Matrix				CreatePerspectiveFOV(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip);
	static void					CreatePerspectiveFOV(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip, Matrix& result);

	//四頂点から射影行列を作成
	static Matrix				CreatePerspectiveOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip);
	static void					CreatePerspectiveOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip, Matrix& result);

	//正射影行列を作成
	static Matrix				CreateOrthographic(const float width, const float height, const float nearClip, const float farClip);
	static void					CreateOrthographic(const float width, const float height, const float nearClip, const float farClip, Matrix& result);

	//正射影行列を作成
	static Matrix				CreateOrthographicOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip);
	static void					CreateOrthographicOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip, Matrix& result);

	//線形補間
	static Matrix				Lerp(const Matrix& a, const Matrix& b, const float amount);
	static void					Lerp(const Matrix& a, const Matrix& b, const float amount, Matrix& result);

	void EpsilonCheck();

	//DirectXmathとの互換性
	static DirectX::XMMATRIX	Store(const Matrix& mat);
	static void					Store(const Matrix& mat, DirectX::XMMATRIX& result);

private:
};

//===================================
//Quaternion
//===================================
struct  Quaternion {
	//==================================
	//メンバ
	//==================================
public:
	float x;
	float y;
	float z;
	float w;
private:
	//==================================
	//メソッド
	//==================================
public:
	Quaternion();
	Quaternion(const Quaternion& other);
	Quaternion(Quaternion&& other);
	Quaternion(const Vector4& other);
	Quaternion(Vector4&& other);
	Quaternion(const float x, const float y, const float z, const float w);
	~Quaternion() {};

	Quaternion&			operator =  (const Quaternion& value);
	Quaternion&			operator =  (Quaternion&& value);
	Quaternion&			operator =  (const Vector4& value);
	Quaternion&			operator =  (Vector4&& value);
	Quaternion&			operator +=	(const Quaternion& value);
	Quaternion&			operator -=	(const Quaternion& value);
	Quaternion&			operator *=	(const Quaternion& value);
	Quaternion&			operator *=	(float scalar);
	Quaternion&			operator /=	(float scalar);
	Quaternion			operator +	()const;
	Quaternion			operator -	()const;
	Quaternion			operator *	(const Quaternion& value)const;
	Quaternion			operator +	(const Quaternion& value)const;
	Quaternion			operator -	(const Quaternion& value)const;
	Quaternion			operator *	(float scalar)const;
	Quaternion			operator /	(float scalar)const;
	bool				operator ==	(const Quaternion& value)const;
	bool				operator !=	(const Quaternion& value)const;

	//大きさを取得
	float				Length()const;
	//大きさの二乗値を取得
	float				LengthSqr()const;
	//オイラー角を取得
	Vector3				EulerAngles();

	//正規化
	Quaternion&			Normalize();
	//正規化を試みる
	Quaternion&			SafeNormalize(const Quaternion& set);

	//単位四元数を求める
	Quaternion&			Identity();

	//単位四元数を作成
	static Quaternion	CreateIdentity();

	//単位四元数かどうか
	static bool			IsIdentity(const Quaternion& value);

	//ノルムが１かどうか
	static bool			IsUnit(const Quaternion& value);

	//乗算
	static Quaternion	Multiply(const Quaternion& a, const Quaternion& b);
	static void			Multiply(const Quaternion& a, const Quaternion& b, Quaternion& reslut);

	//内積
	static float		Dot(const Quaternion& a, const Quaternion& b);
	static void			Dot(const Quaternion& a, const Quaternion& b, float& reslut);

	//共役な四元数を求める
	static Quaternion	Conjugate(const Quaternion& value);
	static void			Conjugate(const Quaternion& value, Quaternion& result);

	//正規化
	static Quaternion	Normalize(const Quaternion& value);
	static void			Normalize(const Quaternion& value, Quaternion& result);

	//正規化を試みる
	static Quaternion	SafeNormalize(const Quaternion& value, const Quaternion& set);
	static void			SafeNormalize(const Quaternion& value, const Quaternion& set, Quaternion& result);

	//ラジアン角から四元数を作成
	static Quaternion	CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll);
	static void			CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, Quaternion& result);

	//オイラー角から四元数を作成
	static Quaternion	CreateFromEulerAngles(const float x, const float y, const float z);
	static Quaternion	CreateFromEulerAngles(const Vector3& eulerAngles);
	static void			CreateFromEulerAngles(const float x, const float y, const float z, Quaternion& result);
	static void			CreateFromEulerAngles(const Vector3& eulerAngles, Quaternion& result);

	//軸とラジアン角から四元数を作成
	static Quaternion	CreateFromAxisAngles(const Vector3& axis, float radian);
	static void			CreateFromAxisAngles(const Vector3& axis, float radian, Quaternion& result);

	//回転行列から四元数を作成
	static Quaternion	CreateFromRotationMatrix(const Matrix& value);
	static void			CreateFromRotationMatrix(const Matrix& value, Quaternion& result);

	//球面線形補間
	static Quaternion	Slerp(const Quaternion& a, const Quaternion& b, float amount);
	static void			Slerp(const Quaternion& a, const Quaternion& b, float amount, Quaternion& result);
	//球面四角形補間
	static Quaternion	Squad(const Quaternion& value, const Quaternion& a, const Quaternion& b, const Quaternion& c, float amount);
	static void			Squad(const Quaternion& value, const Quaternion& a, const Quaternion& b, const Quaternion& c, float amount, Quaternion& result);

private:
};

//===================================
//OrthonormalBasis
//===================================
struct  OrthonormalBasis {
	//==================================
	//メンバ
	//==================================
public:
	Vector3 u;
	Vector3 v;
	Vector3 w;
private:
	//==================================
	//メソッド
	//==================================
public:
	OrthonormalBasis();
	OrthonormalBasis(const OrthonormalBasis& value);
	OrthonormalBasis(OrthonormalBasis&& value);
	OrthonormalBasis(const Vector3& nU, const Vector3& nV, const Vector3& nW);

	void InitFromU(const Vector3& nU);
	void InitFromV(const Vector3& nV);
	void InitFromW(const Vector3& nW);
	void InitFromUV(const Vector3& nU, const Vector3& nV);
	//void InitFromVU(const Vector3& nV, const Vector3& nU);
	void InitFromUW(const Vector3& nU, const Vector3& nW);
	//void InitFromWU(const Vector3& nW, const Vector3& nU);
	void InitFromVW(const Vector3& nV, const Vector3& nW);
	//void InitFromWV(const Vector3& nW, const Vector3& nV);

	OrthonormalBasis& operator = (const OrthonormalBasis& value);
	OrthonormalBasis& operator = (OrthonormalBasis&& value);

	bool operator == (const OrthonormalBasis& value)const;
	bool operator != (const OrthonormalBasis& value)const;

};

//イージング関数群
namespace Easing {

	namespace Linear {
		float  In(float startValue, float changeValue, float time, float duration);
	};
	namespace Quadratic {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
	namespace Cubic {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
	namespace Quartic {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
	namespace Quintic {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
	namespace Shinusoidal {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
	namespace Exponential {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
	namespace Circuler {
		float  In(float startValue, float changeValue, float time, float duration);
		float  Out(float startValue, float changeValue, float time, float duration);
		float  InOut(float startValue, float changeValue, float time, float duration);
	};
};//Easing

struct Vertex3D {
	Vector3 pos;
	Vector3	normal;
	Vector2	texCoord;
};

struct Vertex2D {
	Vector2 pos;
	Vector2	texCoord;
};

//DirectXMathOperator
inline DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) {
	return DirectX::XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
};

inline DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) {
	return DirectX::XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
};

inline DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a) {
	return DirectX::XMFLOAT3(a.x, a.y, a.z);
};

inline DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a) {
	return DirectX::XMFLOAT3(-a.x, -a.y, -a.z);
};