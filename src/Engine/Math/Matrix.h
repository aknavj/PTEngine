#ifndef __MATRIX_H__
#define __MATRIX_H__

/*
 */
template <class T> class TVector {

	public:
		T x, y, z;

		TVector()									{}
		TVector(const T a, const T b, const T c)	{ x = a; y = b; z = c; }
		TVector(const T t)							{ *this = t; }
		TVector(const T *pt)						{ *this = pt; }
		TVector(const TVector<T> &v)				{ *this = v; }

		operator TVector<float>()					{ return TVector<float>((float)x, (float)y, (float)z); }
		operator TVector<double>()					{ return TVector<double>((double)x, (double)y, (double)z); }
		operator T*()								{ return &x; }
		T &operator[](const int n)					{ return (&x)[n]; }
		operator const T*() const					{ return &x; }
		T operator[](const int n) const				{ return (&x)[n]; }
		TVector<T> operator-() const				{ return TVector<T>(-x, -y, -z); }

		void operator=(const T t)					{ x = y = z = t; }
		void operator=(const T *pt)					{ x = pt[0]; y = pt[1]; z = pt[2]; }
		void operator=(const TVector<T> &v)			{ x = v.x; y = v.y; z = v.z; }
		bool operator==(TVector<T> &v) const		{ return (MathLib::Abs(x - v.x) <= (T)0.001f && MathLib::Abs(y - v.y) <= (T)0.001f && MathLib::Abs(z - v.z) <= (T)0.001f); }
		bool operator!=(TVector<T> &v) const		{ return !(*this == v); }

		TVector<T> operator+(const T t) const		{ return TVector<T>(x+t, y+t, z+t); }
		TVector<T> operator-(const T t) const		{ return TVector<T>(x-t, y-t, z-t); }
		TVector<T> operator*(const T t) const		{ return TVector<T>(x*t, y*t, z*t); }
		TVector<T> operator/(const T t) const		{ return TVector<T>(x/t, y/t, z/t); }
		void operator+=(const T t)					{ x += t; y += t; z += t; }
		void operator-=(const T t)					{ x -= t; y -= t; z -= t; }
		void operator*=(const T t)					{ x *= t; y *= t; z *= t; }
		void operator/=(const T t)					{ x /= t; y /= t; z /= t; }

		TVector<T> operator+(const TVector<T> &v) const	{ return TVector<T>(x+v.x, y+v.y, z+v.z); }
		TVector<T> operator-(const TVector<T> &v) const	{ return TVector<T>(x-v.x, y-v.y, z-v.z); }
		TVector<T> operator*(const TVector<T> &v) const	{ return TVector<T>(x*v.x, y*v.y, z*v.z); }
		TVector<T> operator/(const TVector<T> &v) const	{ return TVector<T>(x/v.x, y/v.y, z/v.z); }
		void operator+=(const TVector<T> &v)		{ x += v.x; y += v.y; z += v.z; }
		void operator-=(const TVector<T> &v)		{ x -= v.x; y -= v.y; z -= v.z; }
		void operator*=(const TVector<T> &v)		{ x *= v.x; y *= v.y; z *= v.z; }
		void operator/=(const TVector<T> &v)		{ x /= v.x; y /= v.y; z /= v.z; }

		T operator|(const TVector<T> &v) const		{ return x*v.x + y*v.y + z*v.z; }
		TVector<T> operator^(const TVector<T> &v) const	{ return TVector<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
		void operator^=(const TVector<T> &v)		{ *this = *this ^ v; }

		T MagnitudeSquared() const					{ return x*x + y*y + z*z; }
		T Magnitude() const							{ return (T)sqrtf((float)MagnitudeSquared()); }
		T DistanceSquared(const TVector<T> &v) const{ return (*this - v).MagnitudeSquared(); }
		T Distance(const TVector<T> &v) const		{ return (*this - v).Magnitude(); }
		TVector<T> Midpoint(const TVector<T> &v) const	{ return CVector((*this - v) / 2 + v); }
		TVector<T> Average(const TVector<T> &v) const	{ return CVector((*this + v) / 2); }

		void Normalize()							{ *this /= Magnitude(); }
		double Angle(const TVector<T> &v) const		{ return MathLib::Acos(*this | v); }
		
		TVector<T> Reflect(const TVector<T> &n) const {
			T t = (T)Magnitude();
			TVector<T> v = *this / t;
			return (v - n * (2 * (v | n))) * t;
		}

		TVector<T> Rotate(const T tAngle, const TVector<T> &n) const {
			T tCos = (T)MathLib::Cos(tAngle);
			T tSin = (T)MathLib::Sin(tAngle);
			return TVector<T>(*this * tCos + ((n * *this) * (1 - tCos)) * n + (*this ^ n) * tSin);
		}
};

/*
 */
typedef TVector<float> CVector;
typedef TVector<double> CDoubleVector;

/*
 */
template <class T> inline TVector<T> NormalVector(const TVector<T> &v1, const TVector<T> &v2) {
	TVector<T> v = v1 ^ v2;
	v.Normalize();
	return v;
}

/*
 */
template <class T> inline TVector<T> NormalVector(const TVector<T> &p1, const TVector<T> &p2, const TVector<T> &p3) {
	return NormalVector(p2-p1, p3-p1);
}

/*
 */
template <class T> inline TVector<T> DirectionVector(const TVector<T> &p1, const TVector<T> &p2) {
	TVector<T> v = p2 - p1;
	v.Normalize();
	return v;
}

/*
 */
class CMatrix3;
class CQuaternion {

	public:

		float x, y, z, w;

		CQuaternion()								{}
		CQuaternion(float a, float b, float c, float d)	{ x = a; y = b; z = c; w = d; }
		CQuaternion(float f)						{ *this = f; }
		CQuaternion(const float *p)					{ *this = p; }
		CQuaternion(const CVector &v)				{ *this = v; }
		CQuaternion(const CQuaternion &q)			{ *this = q; }
		CQuaternion(const CMatrix3 &m)				{ *this = m; }
		CQuaternion(const CVector &v, const float f){ SetAxisAngle(v, f); }

		operator float*()							{ return &x; }
		float &operator[](const int n)				{ return (&x)[n]; }
		operator const float*() const				{ return &x; }
		float operator[](const int n) const			{ return (&x)[n]; }
		CQuaternion operator-() const				{ return CQuaternion(-x, -y, -z, -w); }

		void operator=(float f)						{ x = y = z = w = f; }
		void operator=(const float *p)				{ x = p[0]; y = p[1]; z = p[2]; w = p[3]; }
		void operator=(const CVector &v)			{ x = v.x; y = v.y; z = v.z; w = 0; }
		void operator=(const CQuaternion &q)		{ x = q.x; y = q.y; z = q.z; w = q.w; }
		void operator=(const CMatrix3 &m);

		CQuaternion operator+(float f) const		{ return CQuaternion(x+f, y+f, z+f, w+f); }
		CQuaternion operator-(float f) const		{ return CQuaternion(x-f, y-f, z-f, w-f); }
		CQuaternion operator*(float f) const		{ return CQuaternion(x*f, y*f, z*f, w*f); }
		CQuaternion operator/(float f) const		{ return CQuaternion(x/f, y/f, z/f, w/f); }
		void operator+=(float f)					{ x+=f; y+=f; z+=f; w+=f; }
		void operator-=(float f)					{ x-=f; y-=f; z-=f; w-=f; }
		void operator*=(float f)					{ x*=f; y*=f; z*=f; w*=f; }
		void operator/=(float f)					{ x/=f; y/=f; z/=f; w/=f; }

		CQuaternion operator+(const CQuaternion &q) const	{ return CQuaternion(x+q.x, y+q.y, z+q.z, w+q.w); }
		CQuaternion operator-(const CQuaternion &q) const	{ return CQuaternion(x-q.x, y-q.y, z-q.z, w-q.w); }
		CQuaternion operator*(const CQuaternion &q) const	{ return CQuaternion(w*q.x+x*q.w+y*q.z-z*q.y, w*q.y+y*q.w+z*q.x-x*q.z, w*q.z+z*q.w+x*q.y-y*q.x, w*q.w-x*q.x-y*q.y-z*q.z); }	// 16 muls, 12 adds
		void operator+=(const CQuaternion &q)	{ x+=q.x; y+=q.y; z+=q.z; w+=q.w; }
		void operator-=(const CQuaternion &q)	{ x-=q.x; y-=q.y; z-=q.z; w-=q.w; }
		void operator*=(const CQuaternion &q)	{ *this = *this * q; }

		float MagnitudeSquared() const			{ return x*x + y*y + z*z + w*w; }
		float Magnitude() const					{ return sqrtf(MagnitudeSquared()); }
		void Normalize()						{ *this /= Magnitude(); }

		CQuaternion Conjugate() const			{ return CQuaternion(-x, -y, -z, w); }
		CQuaternion Inverse() const				{ return Conjugate() / MagnitudeSquared(); }
		CQuaternion UnitInverse() const			{ return Conjugate(); }
		CVector TransformVector(const CVector &v) const;

		void SetAxisAngle(const CVector &vAxis, float fAngle) {
			float f = fAngle * 0.5f;
			*this = vAxis * MathLib::Sin(f);
			w = MathLib::Cos(f);
		}

		void GetAxisAngle(CVector &vAxis, float &fAngle) const {
			fAngle = acosf(w);
			vAxis = *this / MathLib::Sin(fAngle);
			fAngle *= 2.0f;
		}

		CVector GetViewAxis() const {
			float x2 = x + x, y2 = y + y, z2 = z + z;
			float xx = x * x2, xz = x * z2;
			float yy = y * y2, yz = y * z2;
			float wx = w * x2, wy = w * y2;
			return -CVector(xz+wy, yz-wx, 1-(xx+yy));
		}

		CVector GetUpAxis() const {
			float x2 = x + x, y2 = y + y, z2 = z + z;
			float xx = x * x2, xy = x * y2;
			float yz = y * z2, zz = z * z2;
			float wx = w * x2, wz = w * z2;
			return CVector(xy-wz, 1-(xx+zz), yz+wx);
		}

		CVector GetRightAxis() const {
			float /*x2 = x + x, */y2 = y + y, z2 = z + z;
			float xy = x * y2, xz = x * z2;
			float yy = y * y2, zz = z * z2;
			float wy = w * y2, wz = w * z2;
			return CVector(1-(yy+zz), xy+wz, xz-wy);
		}
};

/*
 */
class CMatrix3 {

	public:

		// same as OpenGL Matrix
		union {
			float f[9];
			float m[3][3];
		};

		CMatrix3()								{}
		CMatrix3(float _f)						{ *this = _f; }
		CMatrix3(const float *p)				{ *this = p; }
		CMatrix3(const CQuaternion &q)			{ *this = q; }
		CMatrix3(const CMatrix3 &mat)			{ *this = mat; }

		void ZeroMatrix()						{ *this = 0.0f; }
		void IdentityMatrix()					{ *this = 0.0f; f[0] = f[4] = f[8] = 1; }

		operator float*()						{ return f; }
		operator const float*() const			{ return f; }
		float &operator()(int i, int j)			{ return m[i][j]; }
		float operator()(int i, int j) const	{ return m[i][j]; }

		void operator=(float _f)				{ f[0] = f[1] = f[2] = f[3] = f[4] = f[5] = f[6] = f[7] = f[8] = _f; }
		void operator=(const float *p)			{ f[0] = p[0]; f[1] = p[1]; f[2] = p[2]; f[3] = p[3]; f[4] = p[4]; f[5] = p[5]; f[6] = p[6]; f[7] = p[7]; f[8] = p[8]; }
		void operator=(const CMatrix3 &mat)		{ f[0] = mat.f[0]; f[1] = mat.f[1]; f[2] = mat.f[2]; f[3] = mat.f[3]; f[4] = mat.f[4]; f[5] = mat.f[5]; f[6] = mat.f[6]; f[7] = mat.f[7]; f[8] = mat.f[8]; }
		void operator=(const CQuaternion &q);

		CMatrix3 operator*(const CMatrix3 &mat) const {
			CMatrix3 mRet;
			mRet.m[0][0] = m[0][0]*mat.m[0][0] + m[1][0]*mat.m[0][1] + m[2][0]*mat.m[0][2];
			mRet.m[0][1] = m[0][1]*mat.m[0][0] + m[1][1]*mat.m[0][1] + m[2][1]*mat.m[0][2];
			mRet.m[0][2] = m[0][2]*mat.m[0][0] + m[1][2]*mat.m[0][1] + m[2][2]*mat.m[0][2];
			mRet.m[1][0] = m[0][0]*mat.m[1][0] + m[1][0]*mat.m[1][1] + m[2][0]*mat.m[1][2];
			mRet.m[1][1] = m[0][1]*mat.m[1][0] + m[1][1]*mat.m[1][1] + m[2][1]*mat.m[1][2];
			mRet.m[1][2] = m[0][2]*mat.m[1][0] + m[1][2]*mat.m[1][1] + m[2][2]*mat.m[1][2];
			mRet.m[2][0] = m[0][0]*mat.m[2][0] + m[1][0]*mat.m[2][1] + m[2][0]*mat.m[2][2];
			mRet.m[2][1] = m[0][1]*mat.m[2][0] + m[1][1]*mat.m[2][1] + m[2][1]*mat.m[2][2];
			mRet.m[2][2] = m[0][2]*mat.m[2][0] + m[1][2]*mat.m[2][1] + m[2][2]*mat.m[2][2];
			return mRet;
		}

		void operator*=(const CMatrix3 &mat)			{ *this = *this * mat; }
		CMatrix3 operator*(const CQuaternion &q) const	{ return *this * CMatrix3(q); }
		void operator*=(const CQuaternion &q)			{ *this = *this * CMatrix3(q); }

		CVector operator*(const CVector &v) const {
			return CVector(m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z,
						   m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z,
						   m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z);
		}

		CVector TransformVector(const CVector &v) const {
			return CVector(m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z,
						   m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z,
						   m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z);
		}

		CVector TransformNormal(const CVector &v) const {
			return CVector(m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
						   m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
						   m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z);
		}

		CMatrix3 Transpose() const {
			CMatrix3 mat;
			mat.m[0][0] = m[0][0]; mat.m[1][0] = m[0][1]; mat.m[2][0] = m[0][2];
			mat.m[0][1] = m[1][0]; mat.m[1][1] = m[1][1]; mat.m[2][1] = m[1][2];
			mat.m[0][2] = m[2][0]; mat.m[1][2] = m[2][1]; mat.m[2][2] = m[2][2];
			return mat;
		}

		CMatrix3 Inverse() const {
			return Transpose();
		}
};

/*
 */
inline void CMatrix3::operator=(const CQuaternion &q) {
	float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
	float xx = q.x * x2, xy = q.x * y2, xz = q.x * z2;
	float yy = q.y * y2, yz = q.y * z2, zz = q.z * z2;
	float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;
	float fTemp = 1-xx;

	m[0][0] = 1-(yy+zz);	m[1][0] = xy-wz;		m[2][0] = xz+wy;
	m[0][1] = xy+wz;		m[1][1] = fTemp-zz;		m[2][1] = yz-wx;
	m[0][2] = xz-wy;		m[1][2] = yz+wx;		m[2][2] = fTemp-yy;
}

/*
 */
inline void CQuaternion::operator=(const CMatrix3 &m) {
	float tr = m(0, 0) + m(1, 1) + m(2, 2);
	if(tr > 0.0f) {
		float s = sqrtf(tr + 1.0f);
		w = s * 0.5f;
		s = 0.5f / s;
		x = (m(1, 2) - m(2, 1)) * s;
		y = (m(2, 0) - m(0, 2)) * s;
		z = (m(0, 1) - m(1, 0)) * s;
	} else {
		const int nIndex[3] = {1, 2, 0};
		int i, j, k;
		i = 0;

		if(m(1, 1) > m(i, i)) i = 1;
		if(m(2, 2) > m(i, i)) i = 2;
		
		j = nIndex[i];
		k = nIndex[j];

		float s = sqrtf((m(i, i) - (m(j, j) + m(k, k))) + 1.0f);
		(*this)[i] = s * 0.5f;
		
		if(s != 0.0) s = 0.5f / s;
		
		(*this)[j] = (m(i, j) + m(j, i)) * s;
		(*this)[k] = (m(i, k) + m(k, i)) * s;
		(*this)[3] = (m(j, k) - m(k, j)) * s;
	}
}

/*
 */
inline CVector CQuaternion::TransformVector(const CVector &v) const {
	return CMatrix3(*this) * v;
}

/*
 */
inline CQuaternion Slerp(const CQuaternion &q1, const CQuaternion &q2, const float t) {

	float fScale0, fScale1;
	double dCos = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	if((1.0 - MathLib::Abs(dCos)) > DELTA) {
		double dTemp = MathLib::Acos(MathLib::Abs(dCos));
		double dSin = MathLib::Sin(dTemp);
		fScale0 = (float)(MathLib::Sin((1.0 - t) * dTemp) / dSin);
		fScale1 = (float)(MathLib::Sin(t * dTemp) / dSin);
	} else {
		fScale0 = 1.0f - t;
		fScale1 = t;
	}
	if(dCos < 0.0) fScale1 = -fScale1;

	return (q1 * fScale0) + (q2 * fScale1);
}

/*
 */
class CMatrix4 {

	public:
		// Same as OpenGL
		union {
			float f[16];
			float m[4][4];
		};

		CMatrix4()								{}
		CMatrix4(float _f)						{ *this = _f; }
		CMatrix4(const float *p)				{ *this = p; }
		CMatrix4(const CQuaternion &q)			{ *this = q; }
		CMatrix4(const CMatrix3 &mat)			{ *this = mat; }
		CMatrix4(const CMatrix4 &mat)			{ *this = mat; }

		operator float*()						{ return f; }
		operator const float*() const			{ return f; }
		float &operator()(int i, int j)			{ return m[i][j]; }
		float operator()(int i, int j) const	{ return m[i][j]; }

		void operator=(float _f)				{ f[0] = f[1] = f[2] = f[3] = f[4] = f[5] = f[6] = f[7] = f[8] = f[9] = f[10] = f[11] = f[12] = f[13] = f[14] = f[15] = _f; }
		void operator=(const float *p)			{ f[0] = p[0]; f[1] = p[1]; f[2] = p[2]; f[3] = p[3]; f[4] = p[4]; f[5] = p[5]; f[6] = p[6]; f[7] = p[7]; f[8] = p[8]; f[9] = p[9]; f[10] = p[10]; f[11] = p[11]; f[12] = p[12]; f[13] = p[13]; f[14] = p[14]; f[15] = p[15]; }
		void operator=(const CMatrix3 &mat)		{ f[0] = mat.f[0]; f[1] = mat.f[1]; f[2] = mat.f[2]; f[3] = 0; f[4] = mat.f[3]; f[5] = mat.f[4]; f[6] = mat.f[5]; f[7] = 0; f[8] = mat.f[6]; f[9] = mat.f[7]; f[10] = mat.f[8]; f[11] = 0; f[12] = 0; f[13] = 0; f[14] = 0; f[15] = 1; }
		void operator=(const CMatrix4 &mat)		{ f[0] = mat.f[0]; f[1] = mat.f[1]; f[2] = mat.f[2]; f[3] = mat.f[3]; f[4] = mat.f[4]; f[5] = mat.f[5]; f[6] = mat.f[6]; f[7] = mat.f[7]; f[8] = mat.f[8]; f[9] = mat.f[9]; f[10] = mat.f[10]; f[11] = mat.f[11]; f[12] = mat.f[12]; f[13] = mat.f[13]; f[14] = mat.f[14]; f[15] = mat.f[15]; }
		void operator=(const CQuaternion &q)	{ *this = CMatrix3(q); }

		void ZMatrix()							{ *this = 0.0f; }
		void IMatrix()							{ *this = 0.0f; f[0] = f[5] = f[10] = f[15] = 1; }
		
		void SMatrix(const CVector &vScale) {
			m[0][0] = vScale.x; m[1][0] = 0.0f;     m[2][0] = 0.0f;     m[3][0] = 0.0f;
			m[0][1] = 0.0f;     m[1][1] = vScale.y; m[2][1] = 0.0f;     m[3][1] = 0.0f;
			m[0][2] = 0.0f;     m[1][2] = 0.0f;     m[2][2] = vScale.z; m[3][2] = 0.0f;
			m[0][3] = 0.0f;     m[1][3] = 0.0f;     m[2][3] = 0.0f;     m[3][3] = 1.0f;
		}
		
		void TMatrix(const CVector &vPos) {
			m[0][0] = 1.0f; m[1][0] = 0.0f; m[2][0] = 0.0f; m[3][0] = vPos.x;
			m[0][1] = 0.0f; m[1][1] = 1.0f; m[2][1] = 0.0f; m[3][1] = vPos.y;
			m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 1.0f; m[3][2] = vPos.z;
			m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f; m[3][3] = 1.0f;
		}
		
		void STMatrix(const CVector &vScale, const CVector &vPos) {
			m[0][0] = vScale.x; m[1][0] = 0.0f;     m[2][0] = 0.0f;     m[3][0] = vPos.x;
			m[0][1] = 0.0f;     m[1][1] = vScale.y; m[2][1] = 0.0f;     m[3][1] = vPos.y;
			m[0][2] = 0.0f;     m[1][2] = 0.0f;     m[2][2] = vScale.z; m[3][2] = vPos.z;
			m[0][3] = 0.0f;     m[1][3] = 0.0f;     m[2][3] = 0.0f;     m[3][3] = 1.0f;
		}

		// Hromadné metody pøemìn
		CMatrix4 operator*(float fScale) const {
			CMatrix4 mRet = *this;
			mRet.m[0][0] *= fScale; mRet.m[1][0] *= fScale; mRet.m[2][0] *= fScale;
			mRet.m[0][1] *= fScale; mRet.m[1][1] *= fScale; mRet.m[2][1] *= fScale;
			mRet.m[0][2] *= fScale; mRet.m[1][2] *= fScale; mRet.m[2][2] *= fScale;
			return mRet;
		}

		void operator*=(float fScale) {
			// 9 muls
			m[0][0] *= fScale; m[1][0] *= fScale; m[2][0] *= fScale;
			m[0][1] *= fScale; m[1][1] *= fScale; m[2][1] *= fScale;
			m[0][2] *= fScale; m[1][2] *= fScale; m[2][2] *= fScale;
		}

		CMatrix4 operator*(const CMatrix4 &mat) const {
			// 36 muls, 27 pøidá
			CMatrix4 mRet;
			mRet.m[0][0] = m[0][0]*mat.m[0][0]+m[1][0]*mat.m[0][1]+m[2][0]*mat.m[0][2];
			mRet.m[1][0] = m[0][0]*mat.m[1][0]+m[1][0]*mat.m[1][1]+m[2][0]*mat.m[1][2];
			mRet.m[2][0] = m[0][0]*mat.m[2][0]+m[1][0]*mat.m[2][1]+m[2][0]*mat.m[2][2];
			mRet.m[3][0] = m[0][0]*mat.m[3][0]+m[1][0]*mat.m[3][1]+m[2][0]*mat.m[3][2]+m[3][0];
			mRet.m[0][1] = m[0][1]*mat.m[0][0]+m[1][1]*mat.m[0][1]+m[2][1]*mat.m[0][2];
			mRet.m[1][1] = m[0][1]*mat.m[1][0]+m[1][1]*mat.m[1][1]+m[2][1]*mat.m[1][2];
			mRet.m[2][1] = m[0][1]*mat.m[2][0]+m[1][1]*mat.m[2][1]+m[2][1]*mat.m[2][2];
			mRet.m[3][1] = m[0][1]*mat.m[3][0]+m[1][1]*mat.m[3][1]+m[2][1]*mat.m[3][2]+m[3][1];
			mRet.m[0][2] = m[0][2]*mat.m[0][0]+m[1][2]*mat.m[0][1]+m[2][2]*mat.m[0][2];
			mRet.m[1][2] = m[0][2]*mat.m[1][0]+m[1][2]*mat.m[1][1]+m[2][2]*mat.m[1][2];
			mRet.m[2][2] = m[0][2]*mat.m[2][0]+m[1][2]*mat.m[2][1]+m[2][2]*mat.m[2][2];
			mRet.m[3][2] = m[0][2]*mat.m[3][0]+m[1][2]*mat.m[3][1]+m[2][2]*mat.m[3][2]+m[3][2];
			mRet.m[0][3] = mRet.m[1][3] = mRet.m[2][3] = 0;
			mRet.m[3][3] = 1;
			return mRet;
		}

		void operator*=(const CMatrix4 &mat)			{ *this = *this * mat; }
		CMatrix4 operator*(const CQuaternion &q) const	{ return *this * CMatrix4(q); }
		void operator*=(const CQuaternion &q)			{ *this = *this * CMatrix4(q); }

		CVector operator*(const CVector &v) const {
			return CVector(m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0],
						   m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1],
						   m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]);
		}

		CVector TransformVector(const CVector &v) const {
			return CVector(m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0],
						   m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1],
						   m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]);
		}

		CVector TransformNormal(const CVector &v) const {
			return CVector(m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
						   m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
						   m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z);
		}

		CMatrix4 Transpose() const {
			CMatrix4 mat;
			mat.m[0][0] = m[0][0]; mat.m[1][0] = m[0][1]; mat.m[2][0] = m[0][2]; mat.m[3][0] = m[0][3];
			mat.m[0][1] = m[1][0]; mat.m[1][1] = m[1][1]; mat.m[2][1] = m[1][2]; mat.m[3][1] = m[1][3];
			mat.m[0][2] = m[2][0]; mat.m[1][2] = m[2][1]; mat.m[2][2] = m[2][2]; mat.m[3][2] = m[2][3];
			mat.m[0][3] = m[3][0]; mat.m[1][3] = m[3][1]; mat.m[2][3] = m[3][2]; mat.m[3][3] = m[3][3];
			return mat;
		}

		void Scale(const CVector &vScale) {
			m[0][0] *= vScale.x; m[1][0] *= vScale.y; m[2][0] *= vScale.z;
			m[0][1] *= vScale.x; m[1][1] *= vScale.y; m[2][1] *= vScale.z;
			m[0][2] *= vScale.x; m[1][2] *= vScale.y; m[2][2] *= vScale.z;
		}
	
		void Translate(const CVector &vPos) {
			m[3][0] = m[0][0]*vPos.x+m[1][0]*vPos.y+m[2][0]*vPos.z+m[3][0];
			m[3][1] = m[0][1]*vPos.x+m[1][1]*vPos.y+m[2][1]*vPos.z+m[3][1];
			m[3][2] = m[0][2]*vPos.x+m[1][2]*vPos.y+m[2][2]*vPos.z+m[3][2];
		}
};


/*
 */
class CSRTTransform {
	public:
		float m_fScale;
		CQuaternion m_qRotate;
		CVector m_vTranslate;

	public:

		CSRTTransform() : m_fScale(1.0f), m_qRotate(0.0f, 0.0f, 0.0f, 1.0f), m_vTranslate(0.0f) {

		}

		void operator=(const CSRTTransform &srt) {
			m_fScale = srt.m_fScale;
			m_qRotate = srt.m_qRotate;
			m_vTranslate = srt.m_vTranslate;
		}

		CSRTTransform operator*(const CSRTTransform &srt) const {
			CSRTTransform tRet;
			tRet.m_fScale = m_fScale * srt.m_fScale;
			tRet.m_qRotate = srt.m_qRotate * m_qRotate;
			tRet.m_vTranslate = m_qRotate.TransformVector(srt.m_vTranslate) * m_fScale + m_vTranslate;
			return tRet;
		}

		CVector operator*(const CVector &v) const {
			return m_qRotate.TransformVector(v) * m_fScale + m_vTranslate;
		}

		CVector TransformVector(const CVector &v) const {
			return m_qRotate.TransformVector(v) * m_fScale + m_vTranslate;
		}

		CVector TransformNormal(const CVector &v) const {
			return m_qRotate.UnitInverse().TransformVector(v);
		}

		CSRTTransform Inverse() const {
			CSRTTransform ret;
			ret.m_fScale = 1/m_fScale;
			ret.m_qRotate = m_qRotate.UnitInverse();
			ret.m_vTranslate = ret.m_qRotate.TransformVector(m_vTranslate) * -ret.m_fScale;
			return ret;
		}

		void Scale(float f) {
			m_fScale *= f;
		}
	
		void Translate(const CVector &v) {
			m_vTranslate += v;
		}
	
		void Rotate(const CQuaternion &q) {
			m_qRotate = q * m_qRotate;
		}

		CMatrix4 BuildModelMatrix() const {
			CMatrix4 mat = m_qRotate;
			mat.m[3][0] = m_vTranslate.x;
			mat.m[3][1] = m_vTranslate.y;
			mat.m[3][2] = m_vTranslate.z;
			mat *= m_fScale;
			return mat;
		}

		CMatrix4 BuildViewMatrix() const	{ return Inverse().BuildModelMatrix(); }
		CVector GetViewAxis() const			{ return m_qRotate.GetViewAxis(); }
		CVector GetUpAxis() const			{ return m_qRotate.GetUpAxis(); }
		CVector GetRightAxis() const		{ return m_qRotate.GetRightAxis(); }
		CVector GetPosition() const			{ return m_vTranslate; }
		void SetPosition(const CVector &v)	{ m_vTranslate = v; }
};

#define COLOR_CTOF(x)	(x/256.0f)
#define COLOR_FTOC(x)	(ColorClamp(x*256))

inline unsigned char ColorClamp(int n)				{ return (unsigned char)(n < 0 ? 0 : n > 255 ? 255 : n); }
inline unsigned char ColorClamp(float f)			{ return (unsigned char)(f < 0 ? 0 : f > 255 ? 255 : f); }

/*
 */
class CColor {

	public:
		unsigned char r, g, b, a;

		/*
		 */
		CColor() {}
		
		/*
		 */
		CColor(int r, int g, int b, int a=255)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
		
		/*
		 */
		CColor(float r, float g, float b, float a=1.0f)
		{
			this->r = COLOR_FTOC(r);
			this->g = COLOR_FTOC(g);
			this->b = COLOR_FTOC(b);
			this->a = COLOR_FTOC(a);
		}

		/*
		 */
		CColor operator*(const float f) const		{ return CColor(ColorClamp(r*f), ColorClamp(g*f), ColorClamp(b*f), ColorClamp(a*f)); }
		CColor operator+(const CColor c) const		{ return CColor(ColorClamp(r+c.r), ColorClamp(g+c.g), ColorClamp(b+c.b), ColorClamp(a+c.a)); }
		operator unsigned char*()					{ return &r; }
};

#endif /* __MATRIX_H__ */
