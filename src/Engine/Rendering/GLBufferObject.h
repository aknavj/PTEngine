#ifndef __GL_BUFFER_OBJECT_H__
#define __GL_BUFFER_OBJECT_H__

/*
 */
struct SPNCVertex {
	CVector p;
	CVector n;
	unsigned char c[4];
};

/*
 */
struct SBufferShape {
	short m_nPositionOffset;
	short m_nNormalOffset;
	short m_nColorOffset;
	short m_nTexCoord[4];
	short m_nTexCoordOffset[4];
	short m_nVertexSize;

	/*
	 */
	void Init(bool bHasPosition, bool bHasNormal, bool bHasColor, int *nTexCoord) {
		int nOffset = 0;
		
		if(bHasPosition) {
			m_nPositionOffset = nOffset;
			nOffset += sizeof(CVector);
		} else {
			m_nPositionOffset = -1;
		}

		if(bHasNormal) {
			m_nNormalOffset = nOffset;
			nOffset += sizeof(CVector);
		} else {
			m_nNormalOffset = -1;
		}

		if(bHasColor) {
			m_nColorOffset = nOffset;
			nOffset += 4 * sizeof(unsigned char);
		} else { 
			m_nColorOffset = -1;
		}
			
		for(int i=0; i<4; i++) {
			
			if(nTexCoord && nTexCoord[i]) {
				m_nTexCoord[i] = nTexCoord[i];
				m_nTexCoordOffset[i] = nOffset;
				nOffset += m_nTexCoord[i] * sizeof(float);
			} else {
				m_nTexCoord[i] = 0;
				m_nTexCoordOffset[i] = -1;
			}

		}

		m_nVertexSize = nOffset;
	}
};

/*
 */
class CGLVertexBufferObject {

	protected:
		unsigned int m_nBufferObject;
		void *m_pBuffer;

	public:
		CGLVertexBufferObject();
		~CGLVertexBufferObject();

		void Init(SBufferShape *pObject, int nElements, void *pBuffer = NULL, GLenum nUsage = GL_STATIC_DRAW_ARB);
		void Cleanup();

		void *Lock();
		void Unlock();

		void Enable(SBufferShape *pObject);
		void Disable(SBufferShape *pObject);
};

/*
 */
class CGLIndexBufferObject {

	protected:
		unsigned int m_nBufferObject;
		int m_nElements;

	public:
		CGLIndexBufferObject();
		~CGLIndexBufferObject();

		void Init(int nElements, void *pBuffer = NULL, GLenum nUsage = GL_DYNAMIC_DRAW_ARB);

		void *Lock();
		void Unlock();

		void Cleanup();
		void Draw(GLenum nMode = GL_TRIANGLES);
};

#endif // __GLBufferObject_h__
