#ifndef AITO_VECMATH_H
#define AITO_VECMATH_H

#include "aito.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>


namespace aito
{

// Vectors

typedef glm::vec<3, Float, glm::packed_highp> Vec3f;
typedef glm::vec<3, int, glm::packed_highp> Vec3i;
typedef glm::vec<4, Float, glm::packed_highp> Vec4f;
typedef glm::vec<4, int, glm::packed_highp> Vec4i;

// Matrices

typedef glm::mat<4, 4, Float, glm::packed_highp> Mat4f;
typedef glm::mat<3, 3, Float, glm::packed_highp> Mat3f;

// Point2

template<typename T>
class Point2 : public glm::vec<2, T, glm::packed_highp>
{
public:
	constexpr Point2()
		: glm::vec<2, T, glm::packed_highp>()
	{};
	constexpr Point2(const glm::vec<3, T, glm::packed_highp>& n)
		: glm::vec<2, T, glm::packed_highp>(n)
	{};
	constexpr Point2(T x, T y)
		: glm::vec<2, T, glm::packed_highp>(x, y)
	{};
};

template<typename T>
[[nodiscard]] constexpr Float distance_squared(const Point2<T>& p0, const Point2<T>& p1)
{
	return (p0 - p1).length_squared();
}
template<typename T>
[[nodiscard]] constexpr Float distance(const Point2<T>& p0, const Point2<T>& p1)
{
	return (p0 - p1).length();
}

template<typename T>
[[nodiscard]] constexpr Point2<T> lerp(Float t, const Point2<T>& p0, const Point2<T>& p1)
{
	return (1 - t) * p0 + t * p1;
}

/// <summary>
/// Computes the component-wise minimum of the two points.
/// </summary>
template<typename T>
[[nodiscard]] constexpr Point2<T> min(const Point2<T>& p0, const Point2<T>& p1)
{
	return Point2<T>(
		std::min(p0.x, p1.x),
		std::min(p0.y, p1.y)
		);
}
/// <summary>
/// Computes the component-wise maximum of the two points.
/// </summary>
template<typename T>
[[nodiscard]] constexpr Point2<T> max(const Point2<T>& p0, const Point2<T>& p1)
{
	return Point2<T>(
		std::max(p0.x, p1.x),
		std::max(p0.y, p1.y)
		);
}

template<typename T>
[[nodiscard]] constexpr Point2<T> floor(const Point2<T>& p)
{
	return Point2<T>(std::floor(p.x), std::floor(p.y));
}
template<typename T>
[[nodiscard]] constexpr Point2<T> ceil(const Point2<T>& p)
{
	return Point2<T>(std::ceil(p.x), std::ceil(p.y));
}
template<typename T>
[[nodiscard]] constexpr Point2<T> abs(const Point2<T>& p)
{
	return Point2<T>(std::abs(p.x), std::abs(p.y));
}

template<typename T>
[[nodiscard]] constexpr Point2<T> permute(const Point2<T>& p, size_t x, size_t y)
{
	return Point2<T>(p[x], p[y]);
}

typedef Point2<Float> Point2f;
typedef Point2<int> Point2i;


// Point3

template<typename T>
class Point3 : public glm::vec<3, T, glm::packed_highp>
{
public:
	constexpr Point3()
		: glm::vec<3, T, glm::packed_highp>()
	{};
	constexpr Point3(const glm::vec<3, T, glm::packed_highp>& n)
		: glm::vec<3, T, glm::packed_highp>(n)
	{};
	constexpr Point3(T x, T y, T z)
		: glm::vec<3, T, glm::packed_highp>(x, y, z)
	{};
};

template<typename T>
[[nodiscard]] constexpr Float distance_squared(const Point3<T>& p0, const Point3<T>& p1)
{
	return (p0 - p1).length_squared();
}
template<typename T>
[[nodiscard]] constexpr Float distance(const Point3<T>& p0, const Point3<T>& p1)
{
	return (p0 - p1).length();
}

template<typename T>
[[nodiscard]] constexpr Point3<T> lerp(Float t, const Point3<T>& p0, const Point3<T>& p1)
{
	return (1 - t) * p0 + t * p1;
}

/// <summary>
/// Computes the component-wise minimum of the two points.
/// </summary>
template<typename T>
[[nodiscard]] constexpr Point3<T> min(const Point3<T>& p0, const Point3<T>& p1)
{
	return Point3<T>(
		std::min(p0.x, p1.x),
		std::min(p0.y, p1.y),
		std::min(p0.z, p1.z)
		);
}
/// <summary>
/// Computes the component-wise maximum of the two points.
/// </summary>
template<typename T>
[[nodiscard]] constexpr Point3<T> max(const Point3<T>& p0, const Point3<T>& p1)
{
	return Point3<T>(
		std::max(p0.x, p1.x),
		std::max(p0.y, p1.y),
		std::max(p0.z, p1.z)
		);
}

template<typename T>
[[nodiscard]] constexpr Point3<T> floor(const Point3<T>& p)
{
	return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
}
template<typename T>
[[nodiscard]] constexpr Point3<T> ceil(const Point3<T>& p)
{
	return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
}
template<typename T>
[[nodiscard]] constexpr Point3<T> abs(const Point3<T>& p)
{
	return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
}

template<typename T>
[[nodiscard]] constexpr Point3<T> permute(const Point3<T>& p, size_t x, size_t y, size_t z)
{
	return Point3<T>(p[x], p[y], p[z]);
}


typedef Point3<Float> Point3f;
typedef Point3<int> Point3i;

// Normals

template<typename T>
class Normal3 : public glm::vec<3, T, glm::packed_highp>
{
public:
	constexpr Normal3()
		: glm::vec<3, T, glm::packed_highp>()
	{};
	constexpr Normal3(const glm::vec<3, T, glm::packed_highp>& n)
		: glm::vec<3, T, glm::packed_highp>(n)
	{};
	constexpr Normal3(T x, T y, T z)
		: glm::vec<3, T, glm::packed_highp>(x, y, z)
	{};
};

template<typename T>
[[nodiscard]] constexpr Normal3<T> face_forward(const Normal3<T>& n, const glm::vec<3, T, glm::packed_highp>& v)
{
	return (dot(Normal3<T>(v), n) < static_cast<T>(0)) ? -n : n;
}

template<typename T>
[[nodiscard]] constexpr Normal3<T> abs(const Normal3<T>& n)
{
	return Normal3<T>(std::abs(n.x), std::abs(n.y), std::abs(n.z));
}

template<typename T>
[[nodiscard]] constexpr T dot(const Normal3<T>& n, const Normal3<T>& w)
{
	return n.x * w.x + n.y * w.y + n.z * w.z;
}

template<typename T>
[[nodiscard]] constexpr T abs_dot(const Normal3<T>& n, const Normal3<T>& w)
{
	return std::abs(dot(n, w));
}



typedef Normal3<Float> Normal3f;
typedef Normal3<int> Normal3i;


///////////////////////////////////////////////// Rays

// Forward declare medium
class Medium;

class Ray
{
public:
	// Ray origin
	Point3f o{};
	// Ray direction
	Vec3f d{};
	// Ray origin medium (non-owning pointer).
	const Medium* medium;

	// Ray max distance
	mutable Float t_max;

	// Ray time
	Float time;

public:

	constexpr Ray() : medium(nullptr), t_max(INFINITY), time(0)
	{};
	constexpr Ray(const Point3f& o, const Vec3f& d,
				  Float t_max = INFINITY, Float time = 0,
				  const Medium* medium = nullptr)
		: o(o), d(d), medium(nullptr), t_max(t_max), time(time)
	{};

	[[nodiscard]] constexpr Point3f operator()(Float t) const
	{
		return o + d * t;
	}

};

class RayDifferential : public Ray
{
public:
	Point3f rx_o{}, ry_o{};
	Vec3f rx_d{}, ry_d{};

	bool has_differentials = false;

public:
	constexpr RayDifferential() : has_differentials(false)
	{}
	constexpr RayDifferential(const Point3f& o, const Vec3f& d,
							  Float t_max = INFINITY, Float time = 0,
							  const Medium* medium = nullptr)
		: Ray(o, d, t_max, time, medium), has_differentials(false)
	{}
	constexpr RayDifferential(const Ray& r)
		: Ray(r), has_differentials(false)
	{}

	// Public Methods

	constexpr void scale_differentials(Float s)
	{
		rx_o = o + (rx_o - o) * s;
		ry_o = o + (ry_o - o) * s;
		rx_d = d + (rx_d - d) * s;
		ry_d = d + (ry_d - d) * s;
	}
};


// Quaternion
typedef glm::qua<aito::Float, glm::packed_highp> Quaternion;

}  // namespace aito



#endif // AITO_VECMATH_H