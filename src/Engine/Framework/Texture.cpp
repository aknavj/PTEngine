#include "Engine.h"
#include "Texture.h"

/*******************************************************************************\
*
*	CTexture
*
\******************************************************************************/

/*
 */
CTexture::CTexture() {
	m_nID = (unsigned int)-1;
}

/*
 */
CTexture::CTexture(CPixelBuffer *pBuffer, bool bMipmap) {
	m_nID = (unsigned int)-1;
	Init(pBuffer, bMipmap);
}

/*
 */
CTexture::~CTexture() { 
	Cleanup(); 
}

/*
 */
void CTexture::Cleanup() {
	if (m_nID != (unsigned int)-1) {
		glDeleteTextures(1, &m_nID);
		m_nID = (unsigned int)-1;
	}
}

/*
 */
void CTexture::Enable(int nType) {
	glEnable(nType); 
}

void CTexture::Disable(int nType) {
	glDisable(nType); 
}

/*
 */
DWORD CTexture::GetID()	{ 
	return m_nID; 
}

/*
 */
int CTexture::GetType()	{ 
	return m_nType; 
}

/*
 */
void CTexture::Bind() { 
	if (m_nID != (unsigned int)-1) glBindTexture(m_nType, m_nID); 
}

/*
 */
void CTexture::Enable()	{ 
	if (m_nID != (unsigned int)-1) { 
		Bind(); glEnable(m_nType); 
	} 
}

/*
 */
void CTexture::Disable() { 
	if (m_nID != (unsigned int)-1) glDisable(m_nType); 
}

/*
 */
void CTexture::Init(CPixelBuffer *pBuffer, bool bClamp, bool bMipmap) {
	Cleanup();

	m_nType = (pBuffer->GetDepth() > 1) ? GL_TEXTURE_3D : (pBuffer->GetHeight() > 1) ? GL_TEXTURE_2D : GL_TEXTURE_1D;
	if(m_nType == GL_TEXTURE_2D && pBuffer->GetWidth() != pBuffer->GetHeight()) m_nType = GL_TEXTURE_RECTANGLE_ARB;

	glGenTextures(1, &m_nID);
	Bind();

	glTexParameteri(m_nType, GL_TEXTURE_WRAP_R, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_nType, GL_TEXTURE_MIN_FILTER, bMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

	switch(m_nType) {
		case GL_TEXTURE_1D:
		{
			if (bMipmap) gluBuild1DMipmaps(m_nType, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			else glTexImage1D(m_nType, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		}

		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
		{
			if (bMipmap) gluBuild2DMipmaps(m_nType, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			else glTexImage2D(m_nType, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		}

		case GL_TEXTURE_3D:
		{
			glTexImage3D(m_nType, 0, pBuffer->GetChannels(), pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetDepth(), 0, pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		}
	}

	LOG_GLERROR();
}

/*
 */
void CTexture::Update(CPixelBuffer *pBuffer, int nLevel) {
	Bind();

	switch (m_nType) {
		case GL_TEXTURE_1D:
		{
			glTexSubImage1D(m_nType, nLevel, 0, pBuffer->GetWidth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		}
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
		{
			glTexSubImage2D(m_nType, nLevel, 0, 0, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		}
		case GL_TEXTURE_3D:
		{
			glTexSubImage3D(m_nType, nLevel, 0, 0, 0, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetDepth(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
			break;
		}
	}

	LOG_GLERROR();
}

/*
 */
void CTexture::InitCopy(int x, int y, int nWidth, int nHeight, bool bClamp) {
	Cleanup();

	m_nType = (nHeight == 1) ? GL_TEXTURE_1D : GL_TEXTURE_2D;
	glGenTextures(1, &m_nID);

	Bind();
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_S, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_WRAP_T, bClamp ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(m_nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_nType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	switch(m_nType) {
		case GL_TEXTURE_1D:
		{
			glCopyTexImage1D(m_nType, 0, GL_RGBA, x, y, nWidth, 0);
			break;
		}
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
		{
			glCopyTexImage2D(m_nType, 0, GL_RGBA, x, y, nWidth, nHeight, 0);
			break;
		}
		case GL_TEXTURE_3D:
		{
			//glCopyTexImage3D(m_nType, 0, GL_RGBA, x, y, nWidth, nHeight, 0);
			break;
		}
	}

	LOG_GLERROR();
}

/*
 */
void CTexture::UpdateCopy(int x, int y, int nWidth, int nHeight, int nOffx, int nOffy, int nOffz) {
	Bind();

	switch (m_nType) {
		case GL_TEXTURE_1D:
		{
			glCopyTexSubImage1D(m_nType, 0, nOffx, x, y, nWidth);
			break;
		}
		case GL_TEXTURE_2D:
		case GL_TEXTURE_RECTANGLE_ARB:
		{
			glCopyTexSubImage2D(m_nType, 0, nOffx, nOffy, x, y, nWidth, nHeight);
			break;
		}
		case GL_TEXTURE_3D:
		{
			glCopyTexSubImage3D(m_nType, 0, nOffx, nOffy, nOffz, x, y, nWidth, nHeight);
			break;
		}
	}

	LOG_GLERROR();
}

/*******************************************************************************\
*
*	CTextureArray
*
\******************************************************************************/

/*
 */
CTextureArray::CTextureArray() {
	m_pStack = NULL;
}

/*
 */
CTextureArray::~CTextureArray() {
	Cleanup();
}

/*
 */
void CTextureArray::Init(int nTextureSize, int nPartitionSize, int nChannels, int nFormat, int nDataType) {

	Cleanup();

	m_nTextureSize = nTextureSize;
	m_nPartitionSize = nPartitionSize;
	m_nChannels = nChannels;
	m_nFormat = nFormat;
	m_nDataType = nDataType;
	m_nArrayWidth = m_nTextureSize / m_nPartitionSize;

	m_nStackIndex = 0;
	m_nStackSize = m_nArrayWidth * m_nArrayWidth;
	m_pStack = new int[m_nStackSize];
	for (int n = 0; n < m_nStackSize; n++)
		m_pStack[n] = n;

	m_pb.Init(nTextureSize, nTextureSize, 1, nChannels, nFormat, nDataType);
	memset(m_pb.GetBuffer(), 0xFF, m_pb.GetBufferSize());

	CTexture::Init(&m_pb, true, false);
}

void CTextureArray::Cleanup() {

	if (m_pStack) {
		delete m_pStack;
		m_pStack = NULL;
	}

	CTexture::Cleanup();
}

/*
 */
int CTextureArray::LockTexture() {
	LogAssert(m_nStackIndex < m_nStackSize);
	if (m_nStackIndex >= m_nStackSize) return m_nStackSize;
	return m_pStack[m_nStackIndex++];
}

/*
 */
void CTextureArray::ReleaseTexture(int nTexture) {
	LogAssert(m_nStackIndex > 0 && nTexture >= 0 && nTexture < m_nStackSize);
	if (m_nStackIndex <= 0 || nTexture < 0 || nTexture >= m_nStackSize) return;
	m_pStack[--m_nStackIndex] = nTexture;
}

/*
 */
void CTextureArray::Update(int nTexture, CPixelBuffer *pBuffer) {
	if (nTexture < 0 || nTexture >= m_nStackSize) return;

	Bind();
	int x = nTexture % m_nArrayWidth;
	int y = nTexture / m_nArrayWidth;

	LogAssert(pBuffer->GetWidth() == m_nPartitionSize);
	LogAssert(pBuffer->GetHeight() == m_nPartitionSize);
	LogAssert(pBuffer->GetFormat() == m_nFormat);
	LogAssert(pBuffer->GetDataType() == m_nDataType);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x*m_nPartitionSize, y*m_nPartitionSize, pBuffer->GetWidth(), pBuffer->GetHeight(), pBuffer->GetFormat(), pBuffer->GetDataType(), pBuffer->GetBuffer());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	LOG_GLERROR();
	
	/*
	unsigned char *pSrc = (unsigned char *)pBuffer->GetBuffer();
	for(int i=0; i<m_nPartitionSize; i++) {
		unsigned char *pDest = (unsigned char *)m_pb(x*m_nPartitionSize, y*m_nPartitionSize+i, 0);
		for(int j=0; j<m_nPartitionSize; j++) {
			*pDest++ = *pSrc++;
			*pDest++ = *pSrc++;
			*pDest++ = *pSrc++;
		}
	}
	*/
}

/*
 */
void CTextureArray::Flush() {
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pb.GetWidth(), m_pb.GetHeight(), m_pb.GetFormat(), m_pb.GetDataType(), m_pb.GetBuffer());
}

/*
 */
void CTextureArray::MapCorners(int nTexture, CMatrix4 &mTexture) {

	LogAssert(nTexture >= 0 && nTexture < m_nStackSize);
	if (nTexture < 0 || nTexture >= m_nStackSize) return;
 
	float fXScale = (float)(m_nPartitionSize - 1) / (float)m_nTextureSize;
	float fYScale = (float)(m_nPartitionSize - 1) / (float)m_nTextureSize;
	float fXOffset = ((nTexture % m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
	float fYOffset = ((nTexture / m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
	mTexture.STMatrix(CVector(fXScale, fYScale, 1.0f), CVector(fXOffset, fYOffset, 0.0f));
}

/*
 */
void CTextureArray::MapCorners(int nTexture, CMatrix4 &mTexture, float fXMin, float fYMin, float fXMax, float fYMax) {

	LogAssert(nTexture >= 0 && nTexture < m_nStackSize);
	if (nTexture < 0 || nTexture >= m_nStackSize) return;

	float fXScale = (float)(m_nPartitionSize - 1) / ((float)m_nTextureSize * (fXMax - fXMin));
	float fYScale = (float)(m_nPartitionSize - 1) / ((float)m_nTextureSize * (fYMax - fYMin));
	float fXOffset = ((nTexture % m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
	float fYOffset = ((nTexture / m_nArrayWidth) * m_nPartitionSize + 0.5f) / (float)m_nTextureSize;
	mTexture.STMatrix(CVector(fXScale, fYScale, 1.0f), CVector(fXOffset, fYOffset, 0.0f));
	mTexture.Translate(CVector(-fXMin, -fYMin, 0.0f));
}