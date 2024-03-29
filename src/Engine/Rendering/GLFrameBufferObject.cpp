#include "Engine.h"

/*
 */
bool CGLFrameBufferObject::CheckFramebufferStatus() {

	GLenum status = (GLenum)glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
		{
			LogDebug("Framebuffer complete");
			return true;
		}
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		{
			LogError("Unsupported framebuffer format");
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		{
			LogError("Framebuffer incomplete, missing attachment");
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
		{
			LogError("Framebuffer incomplete, duplicate attachment");
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		{
			LogError("Framebuffer incomplete, attached images must have same dimensions");
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		{
			LogError("Framebuffer incomplete, attached images must have same format");
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		{
			LogError("Framebuffer incomplete, missing draw buffer");
			break;
		}
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		{
			LogError("Framebuffer incomplete, missing read buffer");
			break;
		}
		default:
		{
			LogError("Unknown framebuffer status!");
			break;
		}
	}

	return false;
}

/*
 */
CGLFrameBufferObject::CGLFrameBufferObject() {
	m_nTextureObject = 0;
	m_nFrameBufferObject = 0;
	m_nDepthBufferObject = 0;
	m_nStencilBufferObject = 0;
}

/*
 */
CGLFrameBufferObject::~CGLFrameBufferObject() {

}

/*
 */
bool CGLFrameBufferObject::IsValid() { 
	return m_nFrameBufferObject != 0; 
}

/*
 */
void CGLFrameBufferObject::Cleanup() {

	if (m_nStencilBufferObject) glDeleteRenderbuffersEXT(1, &m_nStencilBufferObject);
	if (m_nDepthBufferObject) glDeleteRenderbuffersEXT(1, &m_nDepthBufferObject);
	if (m_nFrameBufferObject) glDeleteFramebuffersEXT(1, &m_nFrameBufferObject);
	if (m_nDepthTextureObject) glDeleteTextures(1, &m_nDepthTextureObject);
	if (m_nTextureObject) glDeleteTextures(1, &m_nTextureObject);
	
	m_nTextureObject = 0;
	m_nFrameBufferObject = 0;
	m_nDepthBufferObject = 0;
	m_nStencilBufferObject = 0;
}

/*
 */
bool CGLFrameBufferObject::Init(int nWidth, int nHeight, int nInternalFormat, bool bDepthBuffer, bool bStencilBuffer) {
	
	Cleanup();

	if (!CVideoTask::GetPtr()->HasExtension("GL_EXT_framebuffer_object")) {
		LogWarning("Framebuffer objects not supported, unable to create fame buffer");
		return false;
	}

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nInternalFormat = nInternalFormat;
	m_nTextureTarget = m_nWidth == m_nHeight ? GL_TEXTURE_2D : GL_TEXTURE_RECTANGLE_ARB;
	m_nFilterMode = m_nTextureTarget == GL_TEXTURE_RECTANGLE_ARB ? GL_NEAREST : GL_LINEAR;

	glGenTextures(1, &m_nTextureObject);
	glBindTexture(m_nTextureTarget, m_nTextureObject);
	glTexImage2D(m_nTextureTarget, 0, m_nInternalFormat, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(m_nTextureTarget, GL_TEXTURE_MIN_FILTER, m_nFilterMode);
	glTexParameterf(m_nTextureTarget, GL_TEXTURE_MAG_FILTER, m_nFilterMode);
	glTexParameterf(m_nTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_nTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	LOG_GLERROR();

	glGenFramebuffersEXT(1, &m_nFrameBufferObject);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBufferObject);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, m_nTextureTarget, m_nTextureObject, 0);
	LOG_GLERROR();

	if (bDepthBuffer) {
		glGenRenderbuffersEXT(1, &m_nDepthBufferObject);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_nDepthBufferObject);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, m_nWidth, m_nHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_nDepthBufferObject);

		/*
		glGenTextures(1, &m_nDepthTextureObject);
		glBindTexture(m_nTextureTarget, m_nDepthTextureObject);
		glTexImage2D(m_nTextureTarget, 0, GL_DEPTH_COMPONENT, m_nWidth, m_nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(m_nTextureTarget, GL_TEXTURE_MAG_FILTER, m_nFilterMode);
		glTexParameteri(m_nTextureTarget, GL_TEXTURE_MIN_FILTER, m_nFilterMode);
		glTexParameteri(m_nTextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_nTextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, m_nTextureTarget, m_nDepthTextureObject, 0);
		*/

		LOG_GLERROR();
	}

	if (bStencilBuffer) {
		glGenRenderbuffersEXT(1, &m_nStencilBufferObject);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_nStencilBufferObject);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX8_EXT, m_nWidth, m_nHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_nStencilBufferObject);
		
		LOG_GLERROR();
	}

	bool bSuccess = CheckFramebufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	if (!bSuccess) Cleanup();

	return bSuccess;
}

/*
 */
void CGLFrameBufferObject::EnableFrameBuffer()	{ 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBufferObject); 
}

/*
 */
void CGLFrameBufferObject::DisableFrameBuffer()	{ 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
}

/*
 */
void CGLFrameBufferObject::EnableTexture() { 
	glBindTexture(m_nTextureTarget, m_nTextureObject); glEnable(m_nTextureTarget); 
}

/*
 */
void CGLFrameBufferObject::DisableTexture()	{ 
	glDisable(m_nTextureTarget); glBindTexture(m_nTextureTarget, 0); 
}