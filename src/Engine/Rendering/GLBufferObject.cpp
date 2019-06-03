#include "Engine.h"

/*
 */
CGLVertexBufferObject::CGLVertexBufferObject() {
	m_nBufferObject = (unsigned int)-1;
	m_pBuffer = NULL;
}

/*
 */
CGLVertexBufferObject::~CGLVertexBufferObject() {
	Cleanup();
}

/*
 */
void CGLVertexBufferObject::Init(SBufferShape *pObject, int nElements, void *pBuffer, GLenum nUsage) {
	
	Cleanup();

	if (!CVideoTask::GetPtr()->HasExtension("GL_ARB_vertex_buffer_object")) {
		int nSize = pObject->m_nVertexSize * nElements;
		m_pBuffer = new char[nSize];
	
		if (pBuffer) memcpy(m_pBuffer, pBuffer, nSize);
	} else {
		glGenBuffersARB(1, &m_nBufferObject);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nBufferObject);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, nElements * pObject->m_nVertexSize, pBuffer, nUsage);
	}

	LOG_GLERROR();
}

/*
 */
void CGLVertexBufferObject::Cleanup() {
	
	if (m_pBuffer != NULL) {
		delete (char *)m_pBuffer;
		m_pBuffer = NULL;
	}

	if (m_nBufferObject != (unsigned int)-1) {
		glDeleteBuffersARB(1, &m_nBufferObject);
		m_nBufferObject = (unsigned int)-1;
	}
}

/*
 */
void *CGLVertexBufferObject::Lock() {
	if (m_pBuffer != NULL) return m_pBuffer;
	return glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
}

/*
 */
void CGLVertexBufferObject::Unlock() {
	if (m_pBuffer != NULL) return;
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
}

/*
 */
void CGLVertexBufferObject::Enable(SBufferShape *pObject) {
	
	if (m_pBuffer == NULL) glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nBufferObject);
	
	if (pObject->m_nPositionOffset != -1) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, pObject->m_nVertexSize, (void *)((unsigned long)m_pBuffer + pObject->m_nPositionOffset));
	}

	if (pObject->m_nNormalOffset != -1) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, pObject->m_nVertexSize, (void *)((unsigned long)m_pBuffer + pObject->m_nNormalOffset));
	}

	if (pObject->m_nColorOffset != -1) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, pObject->m_nVertexSize, (void *)((unsigned long)m_pBuffer + pObject->m_nColorOffset));
	}

	for (int i = 3; i >= 0; i--) {
		if (pObject->m_nTexCoordOffset[i] != -1) {
			glClientActiveTexture(GL_TEXTURE0 + i);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(pObject->m_nTexCoord[i], GL_FLOAT, pObject->m_nVertexSize, (void *)((unsigned long)m_pBuffer + pObject->m_nTexCoordOffset[i]));
		}
	}

	LOG_GLERROR();
}

/*
 */
void CGLVertexBufferObject::Disable(SBufferShape *pObject) {
	
	if (pObject->m_nPositionOffset != -1) glDisableClientState(GL_VERTEX_ARRAY);
	if (pObject->m_nNormalOffset != -1) glDisableClientState(GL_NORMAL_ARRAY);
	if (pObject->m_nColorOffset != -1) glDisableClientState(GL_COLOR_ARRAY);
	
	for (int i = 3; i >= 0; i--) {
		if (pObject->m_nTexCoordOffset[i] != -1) {
			glClientActiveTexture(GL_TEXTURE0 + i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	LOG_GLERROR();
}

/*
 */
CGLIndexBufferObject::CGLIndexBufferObject() {

}

CGLIndexBufferObject::~CGLIndexBufferObject() {

}

/*
 */
void CGLIndexBufferObject::Init(int nElements, void *pBuffer, GLenum nUsage) {
	m_nElements = nElements;
	
	glGenBuffersARB(1, &m_nBufferObject);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_nBufferObject);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, nElements * sizeof(unsigned short), pBuffer, nUsage);
	
	LOG_GLERROR();
}

/*
 */
void *CGLIndexBufferObject::Lock() {
	return glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
}

/*
 */
void CGLIndexBufferObject::Unlock() {
	glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
}

/*
 */
void CGLIndexBufferObject::Cleanup() {
	glDeleteBuffersARB(1, &m_nBufferObject);
}

/*
 */
void CGLIndexBufferObject::Draw(GLenum nMode) {
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_nBufferObject);
	glDrawElements(nMode, m_nElements, GL_UNSIGNED_SHORT, 0);
	LOG_GLERROR();
}