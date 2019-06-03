#include "Engine.h"

/*
 */
bool CVideoTask::Start() {
	m_nWidth = ENGINE_WIDTH;
	m_nHeight = ENGINE_HEIGHT;

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) == -1) {
		LogCritical(SDL_GetError());
		LogAssert(false);
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#if 1
	int nFlags = SDL_OPENGL | SDL_ANYFORMAT;
#else
	int nFlags = SDL_OPENGL | SDL_ANYFORMAT | SDL_FULLSCREEN;
#endif

	if (!SDL_SetVideoMode(m_nWidth, m_nHeight, 32, nFlags)) {
		LogCritical(SDL_GetError());
		LogAssert(false);

		return false;
	}

	glViewport(0, 0, m_nWidth, m_nHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)m_nWidth / (double)m_nHeight, 0.001, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	SDL_ShowCursor(SDL_DISABLE);

	*m_szFrameCount = 0;
	m_nTime = 0;
	m_nFrames = 0;
	m_fFont.Init();

	m_strVendor = (const char *)glGetString(GL_VENDOR);
	m_strRenderer = (const char *)glGetString(GL_RENDERER);
	m_strVersion = (const char *)glGetString(GL_VERSION);
	m_strExtensions = (const char *)glGetString(GL_EXTENSIONS);

	LogInfo("GL_VENDOR = %s", m_strVendor.c_str());
	LogInfo("GL_RENDERER = %s", m_strRenderer.c_str());
	LogInfo("GL_VERSION = %s", m_strVersion.c_str());
	LogInfo("GL_EXTENSIONS = %s", m_strExtensions.c_str());

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		LogCritical((const char *)glewGetErrorString(err));
		LogAssert(false);
		return false;
	}

	SDL_VERSION(&m_sdlInfo.version);
	SDL_GetWMInfo(&m_sdlInfo);

	LOG_GLERROR();
	return true;
}

/*
 */
void CVideoTask::Update() {
	PROFILE("CVideoTask::Update()", 1);

	m_nTime += CTimerTask::GetPtr()->GetFrameTicks();
	if (m_nTime >= 1000) {
		float fFPS = (float)(m_nFrames * 1000) / (float)m_nTime;
		sprintf(m_szFrameCount, "%2.2f FPS", fFPS);
		m_nTime = m_nFrames = 0;
	}
	m_nFrames++;

	if (!CInputTask::GetPtr()->IsConsoleActive()) {
		glDisable(GL_LIGHTING);
		glColor4d(1.0, 1.0, 1.0, 1.0);
		m_fFont.Begin();
		m_fFont.SetPosition(0, 0);
		m_fFont.Print(m_szFrameCount);
		m_fFont.End();
	}

	SDL_GL_SwapBuffers();
	LOG_GLERROR();
}

/*
 */
void CVideoTask::Stop() {
	m_fFont.Cleanup();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

/*
 */
bool CVideoTask::IsNVIDIA()	{
	return memcmp(m_strVendor.c_str(), "NVIDIA", 6) == 0;
}

/*
 */
bool CVideoTask::IsATI() {
	return memcmp(m_strVendor.c_str(), "ATI", 3) == 0;
}

/*
 */
bool CVideoTask::IsGeForceFX() {
	return memcmp(m_strRenderer.c_str(), "GeForce FX", 10) == 0;
}

/*
 */
bool CVideoTask::IsGeForce6() {
	return memcmp(m_strRenderer.c_str(), "GeForce 6", 9) == 0;
}

/*
 */
bool CVideoTask::IsRadeon9() {
	return memcmp(m_strRenderer.c_str(), "RADEON 9", 8) == 0;
}

/*
 */
bool CVideoTask::HasExtension(const char *psz)	{
	return strstr(m_strExtensions.c_str(), psz) != NULL;
}

/*
 */
CFont &CVideoTask::GetFont() {
	return m_fFont;
}

/*
 */
int CVideoTask::GetWidth() {
	return m_nWidth;
}

/*
 */
int CVideoTask::GetHeight()	{
	return m_nHeight;
}