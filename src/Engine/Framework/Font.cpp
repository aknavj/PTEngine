#include "Engine.h"

/*
 */
CFont::CFont() {
	m_nListBase = -1;
	m_fXPos = 0;
	m_fYPos = 0;
}

/*
 */
CFont::~CFont() { 
	Cleanup(); 
}

/*
 */
void CFont::Init() {
	Cleanup();
	m_nListBase = glGenLists(256);
#ifdef _WIN32
	wglUseFontBitmaps(::wglGetCurrentDC(), 0, 255, m_nListBase);
#endif
}

/*
 */
void CFont::Cleanup() {

	if (m_nListBase != -1) {
		glDeleteLists(m_nListBase, 256);
		m_nListBase = -1;
	}

}

/*
 */
void CFont::SetPosition(int x, int y) {
	m_fXPos = (float)x;
	m_fYPos = (float)y;
}

/*
 */
void CFont::Begin() {
	glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
				int nViewport[4];
				glGetIntegerv(GL_VIEWPORT, nViewport);
				glOrtho(0, nViewport[2], nViewport[3], 0, -1, 1);
}

/*
 */
void CFont::Print(const char *pszMessage) {
	glRasterPos2f(m_fXPos, m_fYPos + 11);
	glListBase(m_nListBase);
	glCallLists(strlen(pszMessage), GL_UNSIGNED_BYTE, pszMessage);
}

/*
 */
void CFont::End() {
			glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}