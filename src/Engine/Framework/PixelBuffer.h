#ifndef __PIXEL_BUFFER_H__
#define __PIXEL_BUFFER_H__

#define ALIGN_SIZE		64
#define ALIGN_MASK		(ALIGN_SIZE-1)
#define ALIGN(x)		(((unsigned int)x+ALIGN_MASK) & ~ALIGN_MASK)

/*
 */
typedef enum {
	UnsignedByteType = GL_UNSIGNED_BYTE,
	SignedByteType = GL_BYTE,
	UnsignedShortType = GL_UNSIGNED_SHORT,
	SignedShortType = GL_SHORT,
	UnsignedIntType = GL_UNSIGNED_INT,
	SignedIntType = GL_INT,
	FloatType = GL_FLOAT,
	DoubleType = GL_DOUBLE
} PixelBufferDataType;

/*
 */
inline const int GetDataTypeSize(const int nDataType) {
	int nSize;
	switch (nDataType) {
		case UnsignedByteType:
		case SignedByteType:
		{
			nSize = 1;
			break;
		}
		case UnsignedShortType:
		case SignedShortType:
		{
			nSize = 2;
			break;
		}
		case UnsignedIntType:
		case SignedIntType:
		case FloatType:
		{
			nSize = 4;
			break;
		}
		case DoubleType:
		{
			nSize = 8;
			break;
		}
		default:
		{
			nSize = 0;
			break;
		}
	}

	return nSize;
}

/*
 */
class C3DBuffer {

	public:
	
		/*
 		 */
		C3DBuffer()	{ m_pAlloc = m_pBuffer = NULL; }
		C3DBuffer(const C3DBuffer &buf)	{ *this = buf; }
		C3DBuffer(const int nWidth, const int nHeight, const int nDepth, const int nDataType, const int nChannels=1, void *pBuffer=NULL) {
			m_pAlloc = m_pBuffer = NULL;
			Init(nWidth, nHeight, nDepth, nDataType, nChannels, pBuffer);
		}
	
		/*
		 */
		~C3DBuffer() { Cleanup(); }

		/*
		 */
		void operator=(const C3DBuffer &buf) {
			Init(buf.m_nWidth, buf.m_nHeight, buf.m_nDepth, buf.m_nDataType, buf.m_nChannels);
			memcpy(m_pBuffer, buf.m_pBuffer, GetBufferSize());
		}

		/*
		 */
		bool operator==(const C3DBuffer &buf) {
			return (m_nWidth == buf.m_nWidth && m_nHeight == buf.m_nHeight && m_nDepth == buf.m_nDepth && m_nDataType == buf.m_nDataType && m_nChannels == buf.m_nChannels);
		}

		/*
		 */
		void *operator[](const int n) {
			return (void *)((unsigned int)m_pBuffer + n * m_nElementSize);
		}
		
		/*
		 */
		void *operator()(const int x, const int y, const int z) {
			return (void *)((unsigned int)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * z + y) + x));
		}
		
		/*
		 */
		void *operator()(const float x) {
			int nX = MathLib::Min(m_nWidth-1, MathLib::Max(0, (int)(x*(m_nWidth-1)+0.5f)));
			return (void *)((unsigned long)m_pBuffer + m_nElementSize * nX);
		}
		
		/*
		 */
		void *operator()(const float x, const float y) {
			int nX = MathLib::Min(m_nWidth-1, MathLib::Max(0, (int)(x*(m_nWidth-1)+0.5f)));
			int nY = MathLib::Min(m_nHeight-1, MathLib::Max(0, (int)(y*(m_nHeight-1)+0.5f)));
			return (void *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * nY + nX));
		}
		
		/*
		 */
		void *operator()(const float x, const float y, const float z) {
			int nX = MathLib::Min(m_nWidth-1, MathLib::Max(0, (int)(x*(m_nWidth-1)+0.5f)));
			int nY = MathLib::Min(m_nHeight-1, MathLib::Max(0, (int)(y*(m_nHeight-1)+0.5f)));
			int nZ = MathLib::Min(m_nDepth-1, MathLib::Max(0, (int)(z*(m_nDepth-1)+0.5f)));
			return (void *)((unsigned long)m_pBuffer + m_nElementSize * (m_nWidth * (m_nHeight * nZ + nY) + nX));
		}
		
		/*
		 */
		void Interpolate(float *p, const float x);
		void Interpolate(float *p, const float x, const float y);
		void Interpolate(float *p, const float x, const float y, const float z);
		
		/*
		 */
		void Init(const int nWidth, const int nHeight, const int nDepth, const int nDataType, const int nChannels = 1, void *pBuffer = NULL);
		void Cleanup();
		
		/*
		 */
		int GetWidth() const { return m_nWidth; }
		int GetHeight() const { return m_nHeight; }
		int GetDepth() const { return m_nDepth; }
		int GetDataType() const { return m_nDataType; }
		int GetChannels() const { return m_nChannels; }
		int GetBufferSize() const { return m_nWidth * m_nHeight * m_nDepth * m_nElementSize; }
		void *GetBuffer() const { return m_pBuffer; }

		/*
		 */
		void ClearBuffer() { memset(m_pBuffer, 0, GetBufferSize()); }
		void SwapBuffers(C3DBuffer &buf) {
			LogAssert(*this == buf);
			Swap(m_pAlloc, buf.m_pAlloc);
			Swap(m_pBuffer, buf.m_pBuffer);
		}

	protected:

		int m_nWidth;
		int m_nHeight;
		int m_nDepth;
		int m_nDataType;
		int m_nChannels;
		int m_nElementSize;
		void *m_pAlloc;
		void *m_pBuffer;
};

/*
 */
class CPixelBuffer : public C3DBuffer {

	protected:

		// Format pixel dat( GL_LUMINANCE, GL_RGBA )
		unsigned short m_nFormat;

	public:

		/*
		 */
		CPixelBuffer() : C3DBuffer() { }
		CPixelBuffer(int nWidth, int nHeight, int nDepth, int nChannels=3, int nFormat=GL_RGB, int nDataType=UnsignedByteType) : C3DBuffer(nWidth, nHeight, nDepth, nDataType, nChannels) {
			m_nFormat = nFormat;
		}

		int GetFormat()	{ return m_nFormat; }

		/*
		 */
		void Init(int nWidth, int nHeight, int nDepth, int nChannels=3, int nFormat=GL_RGB, int nDataType=GL_UNSIGNED_BYTE, void *pBuffer=NULL) {
			C3DBuffer::Init(nWidth, nHeight, nDepth, nDataType, nChannels, pBuffer);
			m_nFormat = nFormat;
		}

		/*
		 */
		bool LoadPNG(const char *pszFile);
		bool SavePNG(const char *pszFile);
		bool LoadDDS(const char *pszFile);

		/*
		 */
		void MakeGlow1D();
		void MakeGlow2(float fExpose, float fSizeDisc);
		void Make3DNoise(int nSeed);
		void MakeOpticalDepthBuffer(float fInnerRadius, float fOuterRadius, float fRayleighScaleHeight, float fMieScaleHeight);
		void MakePhaseBuffer(float ESun, float Kr, float Km, float g);
};

#endif // __PIXEL_BUFFER_H__
