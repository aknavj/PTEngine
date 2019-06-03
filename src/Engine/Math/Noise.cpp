#include "Engine.h"
#include "Random.h"

/*******************************************************************************\
*
*	CNoise
*
\******************************************************************************/

/*
 */
float CNoise::Lattice(int ix, float fx, int iy, float fy, int iz, float fz, int iw, float fw) {
	int n[4] = { ix, iy, iz, iw };
	float f[4] = { fx, fy, fz, fw };
	int i, nIndex = 0;

	for (i = 0; i<m_nDimensions; i++) nIndex = m_nMap[(nIndex + n[i]) & 0xFF];
	
	float fValue = 0;
	for (i = 0; i<m_nDimensions; i++) fValue += m_nBuffer[nIndex][i] * f[i];
	
	return fValue;
}

/*
 */
void CNoise::Init(int nDimensions, unsigned int nSeed) {
	m_nDimensions = MathLib::Min(nDimensions, MAX_DIMENSIONS);
	CRandom r(nSeed);

	int i, j;
	for(i=0; i<256; i++) {
		m_nMap[i] = i;
		for(j=0; j<m_nDimensions; j++)
			m_nBuffer[i][j] = (float)r.RandomD(-0.5, 0.5);
		MathLib::Normalize(m_nBuffer[i], m_nDimensions);
	}

	while(--i) {
		j = r.RandomI(0, 255);
		Swap(m_nMap[i], m_nMap[j]);
	}
}

/*
 */
float CNoise::Noise(float *f) {
	int i, n[MAX_DIMENSIONS];			
	float r[MAX_DIMENSIONS];		
	float w[MAX_DIMENSIONS];	

	for(i=0; i<m_nDimensions; i++) {
		n[i] = MathLib::Floor(f[i]);
		r[i] = f[i] - n[i];
		w[i] = MathLib::Cubic(r[i]);
	}

	float fValue;
	switch (m_nDimensions) {
		case 1:
		{
			fValue = MathLib::Lerp(Lattice(n[0], r[0]),
				Lattice(n[0] + 1, r[0] - 1),
				w[0]);
			break;
		}
		case 2:
		{
			fValue = MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1]),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1]),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1),
				w[0]),
				w[1]);
			break;
		}
		case 3:
		{
			fValue = MathLib::Lerp(MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1], n[2], r[2]),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2], r[2]),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2], r[2]),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2], r[2]),
				w[0]),
				w[1]),
				MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1], n[2] + 1, r[2] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2] + 1, r[2] - 1),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1),
				w[0]),
				w[1]),
				w[2]);
			break;
		}
		case 4:
		{
			fValue = MathLib::Lerp(MathLib::Lerp(MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1], n[2], r[2], n[3], r[3]),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2], r[2], n[3], r[3]),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2], r[2], n[3], r[3]),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2], r[2], n[3], r[3]),
				w[0]),
				w[1]),
				MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1], n[2] + 1, r[2] - 1, n[3], r[3]),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2] + 1, r[2] - 1, n[3], r[3]),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, n[3], r[3]),
				w[0]),
				w[1]),
				w[2]),
				MathLib::Lerp(MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1], n[2], r[2], n[3] + 1, r[3] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2], r[2], n[3] + 1, r[3] - 1),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2], r[2], n[3] + 1, r[3] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2], r[2], n[3] + 1, r[3] - 1),
				w[0]),
				w[1]),
				MathLib::Lerp(MathLib::Lerp(Lattice(n[0], r[0], n[1], r[1], n[2] + 1, r[2] - 1, n[3] + 1, r[3] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2] + 1, r[2] - 1, n[3] + 1, r[3] - 1),
				w[0]),
				MathLib::Lerp(Lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1),
				Lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, n[3] + 1, r[3] - 1),
				w[0]),
				w[1]),
				w[2]),
				w[3]);
			break;
		}
	}

	return MathLib::Clamp(-0.99999f, 0.99999f, fValue*2.0f);
}

/*******************************************************************************\
*
*	CSeededNoise
*
\******************************************************************************/

/*
 */
float CSeededNoise::Lattice(int ix, float fx, int iy, float fy, int iz, float fz) {
	float fValue = m_nBuffer[ix][iy];
	return fValue;
}

/*
 */
void CSeededNoise::Init(unsigned int nSeed) {
	/*
	for(int y=0; y<64; y++)
	{
		float fDy = (float)y/m_nHeight - 0.5f;
		for(int x=0; x<64; x++)
		{
			float fDx = (float)x/m_nWidth - 0.5f;
			float fDist = sqrtf(fDx*fDx + fDy*fDy);
			float fIntensity = expf(-Max(fDist-fSizeDisc,0)*fExpose);
			m_nBuffer[x][y] = 0;
		}
	}
	*/
}

/*
 */
float CSeededNoise::Noise(float *f) {
	
	int i, n[2];		
	float r[2];		
	float w[2];		

	for(i=0; i<2; i++) {
		n[i] = MathLib::Floor(f[i]);
		r[i] = f[i] - n[i];
		w[i] = MathLib::Cubic(r[i]);
	}

	float fValue = MathLib::Lerp(MathLib::Lerp(m_nBuffer[n[0]][n[1]],
							 m_nBuffer[n[0]+1][n[1]],
							 w[0]),
						MathLib::Lerp(m_nBuffer[n[0]][n[1]+1],
							 m_nBuffer[n[0]+1][n[1]+1],
							 w[0]),
						w[1]);
	
	return MathLib::Clamp(-0.99999f, 0.99999f, fValue);
}

/*******************************************************************************\
*
*	CFractal
*
\******************************************************************************/

/*
 */
CFractal::CFractal(int nDimensions, unsigned int nSeed, float fH, float fLacunarity) {
	Init(nDimensions, nSeed, fH, fLacunarity);
}

/*
 */
void CFractal::Init(int nDimensions, unsigned int nSeed, float fH, float fLacunarity) {
	CNoise::Init(nDimensions, nSeed);
	m_fH = fH;
	m_fLacunarity = fLacunarity;
	float f = 1;

	for (int i = 0; i<MAX_OCTAVES; i++)  {
		m_fExponent[i] = powf(f, -m_fH);
		f *= m_fLacunarity;
	}
}

/*
 */
float CFractal::fBm(float *f, float fOctaves) {

	int i;
	float fValue = 0;
	float fTemp[MAX_DIMENSIONS];
	for(i=0; i<m_nDimensions; i++)
		fTemp[i] = f[i];

	for(i=0; i<fOctaves; i++) {
		fValue += Noise(fTemp) * m_fExponent[i];
		for(int j=0; j<m_nDimensions; j++)
			fTemp[j] *= m_fLacunarity;
	}

	//fOctaves
	fOctaves -= (int)fOctaves;
	if(fOctaves > DELTA) fValue += fOctaves * Noise(fTemp) * m_fExponent[i];
	
	return MathLib::Clamp(-0.99999f, 0.99999f, fValue);
}

/*
 */
float CFractal::fBmTest(float *f, float fOctaves, float fOffset) {
	int i, j;
	float fTemp[MAX_DIMENSIONS];

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i] * 2;

	float fValue = Noise(fTemp) + fOffset;
	for(i=1; i<fOctaves; i++) {
		for(j=0; j<m_nDimensions; j++)
			fTemp[j] *= m_fLacunarity;
		fValue += (Noise(fTemp) + fOffset) * m_fExponent[i];
	}

	while(MathLib::Abs(fValue) > 1.0f) {
		if (fValue > 0.0f) fValue = 2 - fValue;
		else fValue = -2 - fValue;
	}

	if(fValue <= 0.0f) fValue = (float)-pow(-fValue, 0.7f);
	else fValue = (float)pow(fValue, 1 + Noise(fTemp) * fValue);
	
	return fValue;
}

/*
 */
float CFractal::fBmTest2(float *f, float fOctaves, float fGain, float fOffset) {

	int i, j;
	float fTemp[MAX_DIMENSIONS];

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i] * 2;

	float fBase = Noise(fTemp) + fOffset;
	for(i=1; i<6; i++) {
		
		for(j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;

		fBase += (Noise(fTemp) + fOffset) * m_fExponent[i];
	}

	fBase *= 0.7f;
	while(MathLib::Abs(fBase) > 1.0f) {
		
		if(fBase > 0.0f) fBase = 2 - fBase;
		else fBase = -2 - fBase;
	}

	fGain *= fBase;

	float fValue = 0.0f;
	float fSignal = 1.0f - MathLib::Abs(Noise(fTemp));
	fSignal *= fSignal;
	fValue += fSignal * m_fExponent[6];
	
	for(i=7; i<13; i++) {

		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
		
		float fWeight = MathLib::Clamp(0.0f, 1.0f, fSignal * fGain);
		
		fSignal = 1.0f - MathLib::Abs(Noise(fTemp));
		fSignal *= fSignal;
		fSignal *= fSignal;
		fSignal *= fWeight;
		
		fValue += (fSignal - 0.5f) * m_fExponent[i];
	}

	if(fBase < 0.0f) fValue = fBase - fValue * MathLib::SqrtWithSign(fBase);
	else fValue = fBase + fValue * MathLib::SqrtWithSign(fBase);
	
	return fValue;
}

/*
 */
float CFractal::fBmTest3(float *f, float fOctaves, float fGain, float fOffset) {

	int i, j;
	float fTemp[MAX_DIMENSIONS];

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i] * 2;

	float fBase = Noise(fTemp) + fOffset;
	for(i=1; i<6; i++) {

		for(j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
		
		fBase += (Noise(fTemp) + fOffset) * m_fExponent[i];
	}

	while(MathLib::Abs(fBase) > 1.0f) {
		if (fBase > 0.0f) fBase = 2 - fBase;
		else fBase = -2 - fBase;
	}

	if(fBase < 0.0f) return fBase;

	float fExtra = fGain * MathLib::Abs(Noise(fTemp)) * m_fExponent[6];
	
	for(i=6; i<12; i++) {

		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
		
		fExtra += fGain * MathLib::Abs(Noise(fTemp)) * m_fExponent[i];
	}

	return fBase - (1-MathLib::Square(1-fExtra)) * MathLib::Sqrt(MathLib::Abs(fBase));
}

/*
 */
float CFractal::Turbulence(float *f, float fOctaves) {

	int i;
	float fValue = 0;
	float fTemp[MAX_DIMENSIONS];

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i];

	for(i=0; i<fOctaves; i++) {
		fValue += MathLib::Abs(Noise(fTemp)) * m_fExponent[i];
		
		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
	}

	// fOctaves
	fOctaves -= (int)fOctaves;
	if(fOctaves > DELTA) fValue += fOctaves * MathLib::Abs(Noise(fTemp) * m_fExponent[i]);
	
	return MathLib::Clamp(-0.99999f, 0.99999f, fValue);
}

/*
 */
float CFractal::Multifractal(float *f, float fOctaves, float fOffset) {
	
	int i;
	float fValue = 1;
	float fTemp[MAX_DIMENSIONS];

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i];

	for(i=0; i<fOctaves; i++) {
		fValue *= Noise(fTemp) * m_fExponent[i] + fOffset;
		
		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
	}

	fOctaves -= (int)fOctaves;
	if(fOctaves > DELTA) fValue *= fOctaves * (Noise(fTemp) * m_fExponent[i] + fOffset);
	
	return MathLib::Clamp(-0.99999f, 0.99999f, fValue);
}

/*
 */
float CFractal::Heterofractal(float *f, float fOctaves, float fOffset) {
	
	int i;
	float fValue = Noise(f) + fOffset;
	float fTemp[MAX_DIMENSIONS];

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i] * m_fLacunarity;

	for(i=1; i<fOctaves; i++) {
		fValue += (Noise(fTemp) + fOffset) * m_fExponent[i] * fValue;
		
		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
	}

	fOctaves -= (int)fOctaves;
	if(fOctaves > DELTA) fValue += fOctaves * (Noise(fTemp) + fOffset) * m_fExponent[i] * fValue;
	
	return MathLib::Clamp(-0.99999f, 0.99999f, fValue);
}

/*
 */
float CFractal::HybridMultifractal(float *f, float fOctaves, float fOffset, float fGain) {
	
	int i;
	float fValue = (Noise(f) + fOffset) * m_fExponent[0];
	float fWeight = fValue;
	float fTemp[MAX_DIMENSIONS];
	
	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i] * m_fLacunarity;

	for(i=1; i<fOctaves; i++) {
		if(fWeight > 1) fWeight = 1;
		
		float fSignal = (Noise(fTemp) + fOffset) * m_fExponent[i];
		
		fValue += fWeight * fSignal;
		fWeight *= fGain * fSignal;
		
		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
	}

	fOctaves -= (int)fOctaves;
	if(fOctaves > DELTA) {
		if(fWeight > 1) fWeight = 1;
		
		float fSignal = (Noise(fTemp) + fOffset) * m_fExponent[i];
		fValue += fOctaves * fWeight * fSignal;
	}

	return MathLib::Clamp(-0.99999f, 0.99999f, fValue);
}

/*
 */
float CFractal::RidgedMultifractal(float *f, float fOctaves, float fOffset, float fGain) {
	
	float fTemp[MAX_DIMENSIONS];
	int i;

	//float fExponent[10] = {1.0f, 0.75f, 0.5625f, 0.4219f, 0.3164f, 0.2373f, 0.1780f, 0.1335f, 0.1001f, 0.0751f};

	for(i=0; i<m_nDimensions; i++) fTemp[i] = f[i] * 2;

	float fValue = MathLib::SquareWithSign(Noise(fTemp));
	for(i=1; i<12; i++) {

		for(int j=0; j<m_nDimensions; j++) fTemp[j] *= m_fLacunarity;
		
		fValue += Noise(fTemp) * m_fExponent[i];
	}

	//fOctaves -= (int)fOctaves;
	//if(fOctaves > DELTA) fValue += fOctaves * Noise(fTemp) * m_fExponent[i];
	
	while(MathLib::Abs(fValue) > 1.0f) {

		if(fValue > 0.0f) fValue = 2 - fValue;
		else fValue = -2 - fValue;
	}

	if(fValue <= 0.0f) fValue = (float)-pow(-fValue, 0.7f);
	else fValue = (float)pow(fValue, 1 + Noise(fTemp) * fValue);
	
	return fValue;
}
