#ifndef __NOISE_H__
#define __NOISE_H__

/*
 */
class CNoise {

	protected:

		int m_nDimensions;
		unsigned char m_nMap[256];
		float m_nBuffer[256][MAX_DIMENSIONS];

		/*
		 */
		float Lattice(int ix, float fx, int iy = 0, float fy = 0, int iz = 0, float fz = 0, int iw = 0, float fw = 0);

	public:

		/*
		 */
		CNoise() { }
		CNoise(int nDimensions, unsigned int nSeed)	{ Init(nDimensions, nSeed); }

		/*
		 */
		void Init(int nDimensions, unsigned int nSeed);
		float Noise(float *f);
};

/*
 */
class CSeededNoise {

	protected:
		float m_nBuffer[64][64];

		/*
		 */
		float Lattice(int ix, float fx, int iy = 0, float fy = 0, int iz = 0, float fz = 0);

	public:

		/*
		 */
		CSeededNoise()	{ }
		CSeededNoise(unsigned int nSeed) { Init(nSeed); }
		
		/*
		 */
		void Init(unsigned int nSeed);
		float Noise(float *f);
};

/*
 */
class CFractal : public CNoise {

	protected:
		float m_fH;
		float m_fLacunarity;
		float m_fExponent[MAX_OCTAVES];

	public:
		
		/*
		 */
		CFractal()	{}
		CFractal(int nDimensions, unsigned int nSeed, float fH, float fLacunarity);

		/*
		 */
		void Init(int nDimensions, unsigned int nSeed, float fH, float fLacunarity);

		/*
		 */
		float fBm(float *f, float fOctaves);
		float Turbulence(float *f, float fOctaves);
		float Multifractal(float *f, float fOctaves, float fOffset);
		float Heterofractal(float *f, float fOctaves, float fOffset);
		float HybridMultifractal(float *f, float fOctaves, float fOffset, float fGain);
		float RidgedMultifractal(float *f, float fOctaves, float fOffset, float fThreshold);
		float fBmTest(float *f, float fOctaves, float fOffset=-0.1f);
		float fBmTest2(float *f, float fOctaves, float fGain=2.0f, float fOffset=-0.1f);
		float fBmTest3(float *f, float fOctaves, float fGain=2.0f, float fOffset=-0.1f);
};

#endif /* __NOISE_H__ */
