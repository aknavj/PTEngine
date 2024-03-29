#ifndef __MATH_LIB__H__
#define __MATH_LIB__H__

/*
 */
namespace MathLib {

	/*
	 */
	template <class T> T Abs(T a)					{ return (a < 0 ? -a : a); }
	template <class T> T Min(T a, T b)				{ return (a < b ? a : b); }
	template <class T> T Max(T a, T b)				{ return (a > b ? a : b); }
	template <class T> T Avg(T a, T b)				{ return (a + b) / (T)2; }
	template <class T> T Clamp(T a, T b, T x)		{ return (x < a ? a : (x > b ? b : x)); }
	template <class T> T Lerp(T a, T b, T x)		{ return (a + x * (b - a)); }
	template <class T> T Cubic(T a)					{ return a * a * (3 - 2 * a); }
	template <class T> T Sign(T a)					{ return a < 0 ? (T)-1 : (T)1; }
	template <class T> T Square(T a)				{ return a * a; }
	template <class T> T SquareWithSign(T a)		{ return a < 0 ? -(a * a) : (a * a); }
	template <class T> T Step(T a, T x)				{ return (T)(x >= a); }
	template <class T> T Boxstep(T a, T b, T x)		{ return Clamp(0, 1, (x - a) / (b - a)); }
	template <class T> T Pulse(T a, T b, T x)		{ return (T)((x >= a) - (x >= b)); }

	/*
	 */
	inline float ToRadians(float fDegrees)			{ return fDegrees * 0.01745329f; }
	inline float ToDegrees(float fRadians)			{ return fRadians * 57.295779f; }
	inline float Sin(float a)						{ return sinf(a); }
	inline float Cos(float a)						{ return cosf(a); }
	inline float Tan(float a)						{ return tanf(a); }
	inline float Asin(float a)						{ return asinf(a); }
	inline float Acos(float a)						{ return acosf(a); }
	inline float Atan(float a)						{ return atanf(a); }
	inline float Sqrt(float a)						{ return sqrtf(a); }

	/*
	 */
	inline int Floor(float a)						{ return ((int)a - (a < 0 && a != (int)a)); }
	inline int Ceiling(float a)						{ return ((int)a + (a > 0 && a != (int)a)); }
	inline float SqrtWithSign(float a)				{ return a < 0 ? -sqrtf(-a) : sqrtf(a); }
	inline float Gamma(float a, float g)			{ return powf(a, 1 / g); }
	inline float Bias(float a, float b)				{ return powf(a, logf(b) * LOGHALFI); }
	inline float Expose(float l, float k)			{ return (1 - expf(-l * k)); }

	/*
	 */
	inline float Gain(float a, float b) {
		if (a <= DELTA) return 0;
		if (a >= 1 - DELTA) return 1;

		register float p = (logf(1 - b) * LOGHALFI);
		if (a < 0.5) return powf(2 * a, p) * 0.5f;
		else return 1 - powf(2 * (1 - a), p) * 0.5f;
	}

	/*
	 */
	inline float Smoothstep(float a, float b, float x) {
		if (x <= a) return 0;
		if (x >= b) return 1;
		return Cubic((x - a) / (b - a));
	}

	/*
	 */
	inline float Mod(float a, float b) {
		a -= ((int)(a / b)) * b;
		if (a < 0) a += b;
		return a;
	}

	/*
	 */
	inline void Normalize(float *f, int n) {
		int i;
		float fMagnitude = 0;
		for (i = 0; i<n; i++) fMagnitude += f[i] * f[i];
		fMagnitude = 1 / sqrtf(fMagnitude);
		for (i = 0; i<n; i++) f[i] *= fMagnitude;
	}
};

#endif /* __MATH_H__ */