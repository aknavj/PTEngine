#include "Engine.h"

/*
 */
bool CTimerTask::Start() {
#if defined(_WIN32)
	m_nLockedRate = 0;
	LARGE_INTEGER nFrequency;
	::QueryPerformanceFrequency(&nFrequency);
	m_dFactor = 1.0 / nFrequency.QuadPart;

	::QueryPerformanceCounter(&m_nGameStart);
	m_nFrameStart.QuadPart = m_nGameStart.QuadPart;
	m_nGameTicks = m_nFrameTicks = 0;
	m_fGameSeconds = m_fFrameSeconds = 0.0f;
#else
	m_nGameStart = m_nFrameStart = SDL_GetTicks();
	m_nGameTicks = m_nFrameTicks = 0;
	m_fGameSeconds = m_fFrameSeconds = 0.0f;
#endif
	return true;
}

/*
 */
void CTimerTask::Update() {
	PROFILE("CTimerTask::Update()", 1);

#if defined (_WIN32)
	if (m_nLockedRate) {
		m_nFrameTicks = m_nLockedRate;
		m_fFrameSeconds = m_nFrameTicks * 0.001f;
		m_nGameTicks += m_nLockedRate;
		m_fGameSeconds = m_nGameTicks * 0.001f;
		return;
	}

	LARGE_INTEGER nPreviousFrame;
	nPreviousFrame.QuadPart = m_nFrameStart.QuadPart;
	::QueryPerformanceCounter(&m_nFrameStart);
	m_fFrameSeconds = (float)((m_nFrameStart.QuadPart - nPreviousFrame.QuadPart) * m_dFactor);
	m_nFrameTicks = (unsigned long)(m_fFrameSeconds * 1000.0f + 0.5f);

	m_fGameSeconds = (float)((m_nFrameStart.QuadPart - m_nGameStart.QuadPart) * m_dFactor);
	m_nGameTicks = (unsigned long)(m_fGameSeconds * 1000.0f + 0.5f);
#else
	unsigned long nPreviousFrame = m_nFrameStart;
	m_nFrameStart = SDL_GetTicks();

	m_nFrameTicks = m_nFrameStart - nPreviousFrame;
	m_fFrameSeconds = m_nFrameTicks * 0.001f;

	m_nGameTicks = m_nFrameStart - m_nGameStart;
	m_fGameSeconds = m_nGameTicks * 0.001f;
#endif
}

/*
 */
void CTimerTask::Stop() {

}

/*
*/
void CTimerTask::LockFrameRate(unsigned long nTicks) {
#if defined(_WIN32)
	m_nLockedRate = nTicks;
#else

#endif
}

/*
*/
void CTimerTask::UnlockFrameRate() {
#if defined(_WIN32)
	m_nLockedRate = 0;
#else

#endif
}

/*
*/
unsigned long CTimerTask::GetGameTicks() {
	return m_nGameTicks;
}

/*
*/
float CTimerTask::GetGameSeconds() {
	return m_fGameSeconds;
}

/*
*/
unsigned long CTimerTask::GetFrameTicks() {
	return m_nFrameTicks;
}

/*
*/
float CTimerTask::GetFrameSeconds()	{
	return m_fFrameSeconds;
}