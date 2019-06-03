#include "Engine.h"

/******************************************************************************\
*
*	C3DBuffer
*
\******************************************************************************/

/*
 */
void C3DBuffer::Interpolate(float *p, const float x) {
	float fX = x*(m_nWidth - 1);
	int nX = MathLib::Min(m_nWidth - 2, MathLib::Max(0, (int)fX));

	float fRatioX = fX - nX;
	float *pValue = (float *)((unsigned long)m_pBuffer + m_nElementSize * nX);

	for (int i = 0; i<m_nChannels; i++) {
		p[i] = pValue[0] * (1 - fRatioX) + pValue[m_nChannels] * (fRatioX);
		pValue++;
	}
}

/*
 */
void C3DBuffer::Interpolate(float *p, const float x, const float y) {
	float fX = x*(m_nWidth - 1);
	float fY = y*(m_nHeight - 1);

	int nX = MathLib::Min(m_nWidth - 2, MathLib::Max(0, (int)fX));
	int nY = MathLib::Min(m_nHeight - 2, MathLib::Max(0, (int)fY));

	float fRatioX = fX - nX;
	float fRatioY = fY - nY;
	float *pValue = (float *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * nY + nX));

	for (int i = 0; i<m_nChannels; i++) {
		p[i] =	pValue[0] * (1 - fRatioX) * (1 - fRatioY) +
				pValue[m_nChannels * 1] * (fRatioX)* (1 - fRatioY) +
				pValue[m_nChannels*m_nWidth] * (1 - fRatioX) * (fRatioY)+
				pValue[m_nChannels*(m_nWidth + 1)] * (fRatioX)* (fRatioY);
		pValue++;
	}
}

/*
 */
void C3DBuffer::Interpolate(float *p, const float x, const float y, const float z) {
	float fX = x*(m_nWidth - 1);
	float fY = y*(m_nHeight - 1);
	float fZ = z*(m_nDepth - 1);
	
	int nX = MathLib::Min(m_nWidth - 2, MathLib::Max(0, (int)fX));
	int nY = MathLib::Min(m_nHeight - 2, MathLib::Max(0, (int)fY));
	int nZ = MathLib::Min(m_nDepth - 2, MathLib::Max(0, (int)fZ));
	
	float fRatioX = fX - nX;
	float fRatioY = fY - nY;
	float fRatioZ = fZ - nZ;
	float *pValue = (float *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * nZ + nY) + nX));
	float *pValue2 = (float *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * (nZ + 1) + nY) + nX));
	
	for (int i = 0; i<m_nChannels; i++) {
		p[i] =	pValue[0] * (1 - fRatioX) * (1 - fRatioY) * (1 - fRatioZ) +
				pValue[m_nChannels * 1] * (fRatioX)* (1 - fRatioY) * (1 - fRatioZ) +
				pValue[m_nChannels*m_nWidth] * (1 - fRatioX) * (fRatioY)* (1 - fRatioZ) +
				pValue[m_nChannels*(m_nWidth + 1)] * (fRatioX)* (fRatioY)* (1 - fRatioZ) +
				pValue2[0] * (1 - fRatioX) * (1 - fRatioY) * (fRatioZ)+
				pValue2[m_nChannels * 1] * (fRatioX)* (1 - fRatioY) * (fRatioZ)+
				pValue2[m_nChannels*m_nWidth] * (1 - fRatioX) * (fRatioY)* (fRatioZ)+
				pValue2[m_nChannels*(m_nWidth + 1)] * (fRatioX)* (fRatioY)* (fRatioZ);
		pValue++;
		pValue2++;
	}
}

/*
 */
void C3DBuffer::Init(const int nWidth, const int nHeight, const int nDepth, const int nDataType, const int nChannels, void *pBuffer) {

	if (m_pAlloc && m_nWidth == nWidth && m_nHeight == nHeight && m_nDataType == nDataType && m_nChannels == nChannels) return;

	Cleanup();
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nDepth = nDepth;
	m_nDataType = nDataType;
	m_nChannels = nChannels;
	m_nElementSize = m_nChannels * GetDataTypeSize(m_nDataType);
	
	if (pBuffer) m_pBuffer = pBuffer;
	else {
		m_pAlloc = new unsigned char[GetBufferSize() + ALIGN_MASK];
		m_pBuffer = (void *)ALIGN(m_pAlloc);
	}
}

/*
 */
void C3DBuffer::Cleanup() {
	if (m_pAlloc) delete (unsigned char *)m_pAlloc;
	m_pAlloc = m_pBuffer = NULL;
}

/*******************************************************************************\
* 
*	CPixelBuffer
*
\******************************************************************************/

/*
 */
bool CPixelBuffer::LoadPNG(const char *pszFile) {

	FILE *pFile = fopen(pszFile, "rb");
	if(pFile == NULL) {
		LogError("Unable to open %s.", pszFile);
		return false;
	}

	unsigned char header[8];
	fread(header, 1, 8, pFile);
	if(png_sig_cmp(header, 0, 8)) {
		LogError("%s is not a valid PNG file.", pszFile);
		fclose(pFile);
		return false;
	}

	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers;

	if((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
		LogError("png_create_read_struct failed.");
		fclose(pFile);
		return false;
	}

	if((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
		LogError("png_create_info_struct failed.");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(pFile);
		return false;
	}

	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, pFile);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	if(info_ptr->color_type == PNG_COLOR_TYPE_RGB && info_ptr->bit_depth == 8) Init(info_ptr->width, info_ptr->height, 1, 3, GL_RGB);
	else if(info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA && info_ptr->bit_depth == 8) Init(info_ptr->width, info_ptr->height, 1, 4, GL_RGBA);
	else {
		LogError("Attempting to read an unsupported format from %s.", pszFile);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(pFile);
		return false;
	}

	int nRowSize = m_nWidth * m_nChannels;
	png_read_update_info(png_ptr, info_ptr);
	setjmp(png_jmpbuf(png_ptr));
	row_pointers = new png_bytep[m_nHeight];
	for(int y=0; y<m_nHeight; y++) row_pointers[y] = (unsigned char *)m_pBuffer + y * nRowSize;
	png_read_image(png_ptr, row_pointers);
	
	delete row_pointers;

	png_destroy_read_struct(&png_ptr, NULL, NULL);
	fclose(pFile);
	
	return true;
}

/*
 */
bool CPixelBuffer::SavePNG(const char *pszFile) {

	if(m_nDataType != GL_UNSIGNED_BYTE && (m_nChannels != 3 || m_nChannels != 4)) {
		LogError("Attempting to write an unsupported format to %s.", pszFile);
		return false;
	}

	FILE *pFile = fopen(pszFile, "wb");
	if(pFile == NULL) {
		LogError("Unable to create %s.", pszFile);
		return false;
	}

	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers;

	if((png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL) {
		LogError("png_create_write_struct failed.");
		return false;
	}

	if((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
		LogError("png_create_info_struct failed.");
		return false;
	}

	setjmp(png_jmpbuf(png_ptr));
	png_init_io(png_ptr, pFile);
	png_set_IHDR(png_ptr, info_ptr, m_nWidth, m_nHeight, 8, m_nChannels == 4 ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	int nRowSize = m_nChannels * m_nWidth;
	row_pointers = new png_bytep[m_nHeight];
	for(int y=0; y<m_nHeight; y++) row_pointers[y] = (unsigned char *)m_pBuffer + y * nRowSize;
	png_write_image(png_ptr, row_pointers);

	delete row_pointers;

	png_write_end(png_ptr, NULL);
	fclose(pFile);

	return true;
}

/*
 */
void CPixelBuffer::MakeGlow1D() {
	int nIndex=0;
	for(int x=0; x<m_nWidth; x++) {
		float fIntensity = powf((float)x / m_nWidth, 0.75f);
		
		for(int i=0; i<m_nChannels-1; i++) ((unsigned char *)m_pBuffer)[nIndex++] = (unsigned char)255;
		
		((unsigned char *)m_pBuffer)[nIndex++] = (unsigned char)(fIntensity*255 + 0.5f);
	}
}

/*
 */
void CPixelBuffer::MakeGlow2(float fExpose, float fSizeDisc) {
	float f2[2];
	CFractal fractal(2, 1, 0.1f, 2.0f);
	int n = 0;
	for(int y=0; y<m_nHeight; y++) {
		float fDy = (y+0.5f)/m_nHeight - 0.5f;
		for(int x=0; x<m_nWidth; x++) {
			float fDx = (x+0.5f)/m_nWidth - 0.5f;
			float fDist = sqrtf(fDx*fDx + fDy*fDy);
			//float fIntensity = expf(-MathLib::Max(fDist-fSizeDisc,0.0f)*fExpose);
			float fIntensity = 2 - MathLib::Min(2.0f, powf(2.0f, MathLib::Max(fDist-fSizeDisc,0.0f)*fExpose));

			f2[0] = fDx*64.0f;
			f2[1] = fDy*64.0f;
			float fNoise = 1.0f;//fractal.fBm(f2, 4.0f) * 0.25f + 0.75f;
			((unsigned char *)m_pBuffer)[n++] = (unsigned char)(fNoise*fIntensity*255 + 0.5f);
			((unsigned char *)m_pBuffer)[n++] = (unsigned char)(fNoise*fIntensity*255 + 0.5f);
			//((unsigned char *)m_pBuffer)[n++] = (unsigned char)(fNoise*255 + 0.5f);
		}
	}
}

/*
 */
void CPixelBuffer::Make3DNoise(int nSeed) {
	CFractal noise(3, nSeed, 0.5f, 2.0f);
	int n = 0;
	float fValues[3];
	for(int z=0; z<m_nDepth; z++) {
		fValues[2] = (float)z * 0.0625f;
		for(int y=0; y<m_nHeight; y++) {
			fValues[1] = (float)y * 0.0625f;
			for(int x=0; x<m_nWidth; x++) {
				fValues[0] = (float)x * 0.0625f;
				float fIntensity = (noise.fBm(fValues, 4.0f) + 1.0f) * 0.5f;
				if(fIntensity < 0.5f) fIntensity = 0.0f;

				//fIntensity = 1.0f - powf(0.9f, fIntensity*255);
				unsigned char nIntensity = (unsigned char)(fIntensity*255 + 0.5f);
				((unsigned char *)m_pBuffer)[n++] = 255;
				((unsigned char *)m_pBuffer)[n++] = nIntensity;
			}
		}
	}
}

/*
 */
void CPixelBuffer::MakeOpticalDepthBuffer(float fInnerRadius, float fOuterRadius, float fRayleighScaleHeight, float fMieScaleHeight) {
	const int nSize = 128;
	const int nSamples = 50;
	const float fScale = 1.0f / (fOuterRadius - fInnerRadius);

	Init(nSize, nSize, 1, 4, GL_RGBA, GL_FLOAT);
	int nIndex = 0;
	for(int nAngle=0; nAngle<nSize; nAngle++) {

		float fCos = 1.0f - (nAngle+nAngle) / (float)nSize;
		float fAngle = acosf(fCos);
		CVector vRay(sinf(fAngle), cosf(fAngle), 0);
		for(int nHeight=0; nHeight<nSize; nHeight++) {
			float fHeight = DELTA + fInnerRadius + ((fOuterRadius - fInnerRadius) * nHeight) / nSize;
			CVector vPos(0, fHeight, 0);

			float B = 2.0f * (vPos | vRay);
			float Bsq = B * B;
			float Cpart = (vPos | vPos);
			float C = Cpart - fInnerRadius*fInnerRadius;
			float fDet = Bsq - 4.0f * C;
			bool bVisible = (fDet < 0 || (0.5f * (-B - sqrtf(fDet)) <= 0) && (0.5f * (-B + sqrtf(fDet)) <= 0));
			float fRayleighDensityRatio;
			float fMieDensityRatio;
			
			if(bVisible) {
				fRayleighDensityRatio = expf(-(fHeight - fInnerRadius) * fScale / fRayleighScaleHeight);
				fMieDensityRatio = expf(-(fHeight - fInnerRadius) * fScale / fMieScaleHeight);
			} else {
				fRayleighDensityRatio = ((float *)m_pBuffer)[nIndex - nSize*m_nChannels] * 0.75f;
				fMieDensityRatio = ((float *)m_pBuffer)[nIndex+2 - nSize*m_nChannels] * 0.75f;
			}

			C = Cpart - fOuterRadius*fOuterRadius;
			fDet = Bsq - 4.0f * C;
			float fFar = 0.5f * (-B + sqrtf(fDet));

			float fSampleLength = fFar / nSamples;
			float fScaledLength = fSampleLength * fScale;
			CVector vSampleRay = vRay * fSampleLength;
			vPos += vSampleRay * 0.5f;

			float fRayleighDepth = 0;
			float fMieDepth = 0;
			for(int i=0; i<nSamples; i++) {
				float fHeight = vPos.Magnitude();
				float fAltitude = (fHeight - fInnerRadius) * fScale;
				fAltitude = MathLib::Max(fAltitude, 0.0f);
				fRayleighDepth += expf(-fAltitude / fRayleighScaleHeight);
				fMieDepth += expf(-fAltitude / fMieScaleHeight);
				vPos += vSampleRay;
			}

			fRayleighDepth *= fScaledLength;
			fMieDepth *= fScaledLength;

			((float *)m_pBuffer)[nIndex++] = fRayleighDensityRatio;
			((float *)m_pBuffer)[nIndex++] = fRayleighDepth;
			((float *)m_pBuffer)[nIndex++] = fMieDensityRatio;
			((float *)m_pBuffer)[nIndex++] = fMieDepth;
		}
	}
}

/*
 */
void CPixelBuffer::MakePhaseBuffer(float ESun, float Kr, float Km, float g) {
	Km *= ESun;
	Kr *= ESun;
	float g2 = g*g;
	float fMiePart = 1.5f * (1.0f - g2) / (2.0f + g2);

	int nIndex = 0;
	for(int nAngle=0; nAngle<m_nWidth; nAngle++) {
		float fCos = 1.0f - (nAngle+nAngle) / (float)m_nWidth;
		float fCos2 = fCos*fCos;
		float fRayleighPhase = 0.75f * (1.0f + fCos2);
		float fMiePhase = fMiePart * (1.0f + fCos2) / powf(1.0f + g2 - 2.0f*g*fCos, 1.5f);
		((float *)m_pBuffer)[nIndex++] = fRayleighPhase * Kr;
		((float *)m_pBuffer)[nIndex++] = fMiePhase * Km;
	}
}


#ifndef FREE
	#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#define DDS_FOURCC  0x00000004
#define DDS_RGB     0x00000040
#define DDS_RGBA    0x00000041
#define DDS_VOLUME  0x00200000
#define DDS_CUBEMAP 0x00000200

#define DDS_MAGIC ('D'|('D'<<8)|('S'<<16)|(' '<<24))
#define DDS_DXT1 ('D'|('X'<<8)|('T'<<16)|('1'<<24))
#define DDS_DXT3 ('D'|('X'<<8)|('T'<<16)|('3'<<24))
#define DDS_DXT5 ('D'|('X'<<8)|('T'<<16)|('5'<<24))

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

typedef struct {
	unsigned short col0, col1;
	unsigned char row[4];
} DXTColorBlock_t;

typedef struct {
	unsigned short row[4];
} DXT3AlphaBlock_t;

typedef struct {
	unsigned char alpha0, alpha1;
	unsigned char row[6];
} DXT5AlphaBlock_t;

typedef struct {
	unsigned long Size;
	unsigned long Flags;
	unsigned long Height;
	unsigned long Width;
	unsigned long PitchLinearSize;
	unsigned long Depth;
	unsigned long MipMapCount;
	unsigned long Reserved1[11];
	unsigned long pfSize;
	unsigned long pfFlags;
	unsigned long pfFourCC;
	unsigned long pfRGBBitCount;
	unsigned long pfRMask;
	unsigned long pfGMask;
	unsigned long pfBMask;
	unsigned long pfAMask;
	unsigned long Caps1;
	unsigned long Caps2;
	unsigned long Reserved2[3];
} DDS_Header_t;

/*
 */
bool CPixelBuffer::LoadDDS(const char *pszFile) {

	FILE *stream = fopen(pszFile, "rb");
	if(stream == NULL) {
		LogError("Unable to open %s.", pszFile);
		return false;
	}

	DDS_Header_t dds;
	unsigned long magic;
	fread(&magic, sizeof(unsigned long), 1, stream);

	if(magic != DDS_MAGIC) {
		LogError("%s is not a valid DDS file.", pszFile);
		fclose(stream);
		return 0;
	}

	fread(&dds, sizeof(DDS_Header_t), 1, stream);

	if(dds.pfFlags == DDS_RGBA && dds.pfRGBBitCount == 32) Init(dds.Width, dds.Height, 1, 4, GL_RGBA);
	else if (dds.pfFlags == DDS_RGB  && dds.pfRGBBitCount == 32) Init(dds.Width, dds.Height, 1, 4, GL_RGBA);
	else if (dds.pfFlags == DDS_RGB  && dds.pfRGBBitCount == 24) Init(dds.Width, dds.Height, 1, 3, GL_RGB);
	else  {
		LogError("Attempting to read an unsupported format from %s.", pszFile);
		fclose(stream);
		return false;
	}

	fread(m_pBuffer, 1, GetBufferSize(), stream);
	fclose(stream);
	
	return true;
}
