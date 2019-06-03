#ifndef __TEXTURE_H__
#define __TEXTURE_H__

/*
 */
class CTexture {

	public:

		CTexture();
		CTexture(CPixelBuffer *pBuffer, bool bMipmap = true);
		~CTexture();

		void Cleanup();

		static void Enable(int nType);
		static void Disable(int nType);
	
		DWORD GetID();
		int GetType();
		void Bind();
		void Enable();
		void Disable();

		void Init(CPixelBuffer *pBuffer, bool bClamp=true, bool bMipmap=true);
		void Update(CPixelBuffer *pBuffer, int nLevel=0);

		void InitCopy(int x, int y, int nWidth, int nHeight, bool bClamp=true);
		void UpdateCopy(int x, int y, int nWidth, int nHeight, int nOffx=0, int nOffy=0, int nOffz=0);

	protected:
		int m_nType; // GL_TEXTURE_1D or GL_TEXTURE_2D
		unsigned int m_nID;	// OpenGL-gen ID textures
};

/*
 */
class CTextureArray : public CTexture {

	public:
		CTextureArray();
		~CTextureArray();

		void Init(int nTextureSize, int nPartitionSize, int nChannels, int nFormat, int nDataType);
		void Cleanup();
		
		int LockTexture();
		void ReleaseTexture(int nTexture);
		
		void Update(int nTexture, CPixelBuffer *pBuffer);
		void Flush();
		
		void MapCorners(int nTexture, CMatrix4 &mTexture);
		void MapCorners(int nTexture, CMatrix4 &mTexture, float fXMin, float fYMin, float fXMax, float fYMax);

	protected:
		int m_nTextureSize;
		int m_nPartitionSize;
		int m_nChannels;
		int m_nFormat;
		int m_nDataType;
		int m_nArrayWidth;
		int m_nStackSize;
		int m_nStackIndex;
		int *m_pStack;
		CPixelBuffer m_pb;
};

#endif // __Texture_h__
