#pragma once

#include <initializer_list>
#include <iostream>
#include <vector>


namespace math
{

const float kPi{ 3.14159265f };

/// Degrees to radians conversion
/// @param[in] degrees Angle in degrees
/// @return Angle in radians
float radians( const float degrees );

/// Radians to degrees conversion
/// @param[in] radians Angle in radians
/// @return Angle in degrees
float degrees( const float radians );


class Size
{
  public:
	static Size null;

	Size();
	Size( int w, int h );

	Size&      operator*=( const int& f );
	Size&      operator*=( const float& f );
	Size&      operator/=( const int& i );
	const Size operator/( const int& i ) const;

	const bool operator==( const Size& other ) const;

	friend std::ostream& operator<<( std::ostream& os, const Size& v );

	int width;
	int height;
};


class Vec2
{
  public:
	static Vec2 zero;

	Vec2();
	Vec2( const float xx, const float yy );

	void normalize();

	Vec2&      operator=( const Vec2& other );
	Vec2&      operator+=( const Vec2& other );
	const Vec2 operator+( const Vec2& other ) const;

	const bool operator==( const Vec2& other ) const;

	friend std::ostream& operator<<( std::ostream& os, const Vec2& v );

	float x;
	float y;
};


class Vec3
{
  public:
	static Vec3 zero;

	Vec3( const float xx = 0.0f, const float yy = 0.0f, const float zz = 0.0f );
	Vec3( const std::vector<float>& v );
	Vec3( const Vec3& other );
	Vec3( Vec3&& other );

	static Vec3 cross( const Vec3& a, const Vec3& b );

	void set( const float xx, const float yy, const float zz );
	void normalize();

	Vec3& operator=( const Vec3& other );
	Vec3& operator+=( const Vec3& other );
	Vec3  operator+( const Vec3& other ) const;
	Vec3  operator-( const Vec3& other ) const;
	Vec3  operator-() const;

	const bool operator==( const Vec3& other ) const;

	float x;
	float y;
	float z;
};

Vec3 operator*( float c, const Vec3& v );

Vec3 lerp( const Vec3& a, const Vec3& b, const float t );

std::ostream& operator<<( std::ostream& os, const Vec3& v );


class Quat
{
  public:
	static Quat identity;

	Quat( float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f );

	void normalize();

	Quat operator-() const;

	Quat operator+( const Quat& other ) const;
	Quat operator-( const Quat& other ) const;

	float x;
	float y;
	float z;
	float w;
};

Quat operator*( float c, const Quat& q );

float dot( const Quat& a, const Quat& b );

float length( const Quat& q );

Quat slerp( Quat a, Quat b, float t );


class Mat4
{
  public:
	static Mat4 zero;
	static Mat4 identity;

	Mat4();
	Mat4( std::initializer_list<float> l );
	Mat4( const float* const m );
	Mat4( const Quat& quat );

	float&       operator[]( const int index );
	const float& operator[]( const int index ) const;
	Mat4&        operator=( const Mat4& matrix );
	Mat4&        operator+=( const Mat4& matrix );
	const Mat4   operator+( const Mat4& other ) const;
	Mat4&        operator*=( const Mat4& matrix );
	const Mat4   operator*( const Mat4& other ) const;
	Vec3 operator*( const Vec3& v ) const;

	const bool operator==( const Mat4& other ) const;

	void translate( const Vec3& vec );
	void translateX( const float amount );
	void translateY( const float amount );
	void translateZ( const float amount );

	void scale( const Vec3& scale );
	void scaleX( const float scale );
	void scaleY( const float scale );
	void scaleZ( const float scale );

	void rotate( const Quat& quat );
	void rotateX( const float radians );
	void rotateY( const float radians );
	void rotateZ( const float radians );

	float matrix[16];
};


class Rectangle
{
  public:
	static Rectangle zero;

	Rectangle();

	const bool operator==( const Rectangle& other ) const;

	/// Tests whether (x, y) is inside the rectangle
	bool contains( float x, float y );

	/// Tests whether this rectangle intersects another one
	bool intersects( const Rectangle& other );
	/// Tests whether this rectangle intersects another one
	bool intersects( const Rectangle* other );

	float x;
	float y;
	float width;
	float height;
};


}  // namespace math
