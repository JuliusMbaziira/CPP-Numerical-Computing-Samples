#include <iostream>
#include <array>
#include <cmath>

// defines a 3d vector, and a few operators on it
struct Vec3D {
	std::array<double,3> c; //the three vector coordinates

	//returns either constant value or l-value
	const double operator [] (int idx) const { return c[idx]; }
	double& operator[] (int idx) { return c[idx]; }
	
	// Subtracts another vector
	Vec3D operator-(const Vec3D& a) const {return {c[0]-a[0],c[1]-a[1],c[2]-a[2]};}
	
	// dot product with another vector
	double operator*(const Vec3D& a) const {return c[0]*a[0]+c[1]*a[1]+c[2]*a[2];}

	// get the square of the euclidean length
	static double lengthSquared(const Vec3D& c) 
	{return c[0]*c[0]+c[1]*c[1]+c[2]*c[2];}
	
	// get the euclidean length
	static double length(const Vec3D& c) {return sqrt(lengthSquared(c));}

	// Divides by a scalar
	Vec3D operator/(const double a) const {return {c[0]/a,c[1]/a,c[2]/a};}

	// Multiplies by a scalar
	Vec3D operator*(const double a) const {return {c[0]*a,c[1]*a,c[2]*a};}

	// adds one vector onto another
	Vec3D& operator+=(const Vec3D& a) 
	{c[0] += a[0]; c[1] += a[1]; c[2] += a[2]; return *this;}

	// subtracts one vector from another
	Vec3D& operator-=(const Vec3D& a) 
	{c[0] -= a[0]; c[1] -= a[1]; c[2] -= a[2]; return *this;}

	// writes the contents of a vector to a stream (for output) 
	friend std::ostream& operator<<(std::ostream& os, const Vec3D& a) 
	{os << a[0] << ' ' << a[1] << ' ' << a[2]; return os;}

	// reads the contents of a vector from a stream (for output) 
	friend std::istream& operator>>(std::istream& is, Vec3D& a)
	{is >> a[0] >> a[1] >> a[2]; return is;}
};
