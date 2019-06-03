#ifndef __GL_FRAMEBUFFER_OBJECT_H__
#define __GL_FRAMEBUFFER_OBJECT_H__

/*
 */
class CGLFrameBufferObject {

	public:

		CGLFrameBufferObject();
		~CGLFrameBufferObject();

		bool IsValid();

		void Cleanup();

		bool Init(int nWidth, int nHeight, int nInternalFormat, bool bDepthBuffer = true, bool bStencilBuffer = false);

		void EnableFrameBuffer();
		void DisableFrameBuffer();

		void EnableTexture();
		void DisableTexture();

	protected:

		GLuint m_nTextureObject;
		GLuint m_nDepthTextureObject;
		GLuint m_nFrameBufferObject;
		GLuint m_nDepthBufferObject;
		GLuint m_nStencilBufferObject;

		// Dimenze textur
		GLuint m_nWidth, m_nHeight;
		GLenum m_nInternalFormat;
		GLenum m_nTextureTarget;
		GLenum m_nFilterMode;

		bool CheckFramebufferStatus();
};

#endif /* __GL_FRAMEBUFFER_OBJECT_H__ */
