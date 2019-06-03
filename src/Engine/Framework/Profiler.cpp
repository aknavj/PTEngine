#include "Engine.h"

/*
 */
CProfileTimer::CProfileTimer(std::string strName, CProfileTimer *pParent) {
	m_strName = strName;
	m_pParent = pParent;
	m_nStartTime = (unsigned long)-1;
	m_nTotalTime = 0;
	m_nCallCount = 0;
}

/*
 */
CProfileTimer::~CProfileTimer() {

	std::map<std::string, CProfileTimer*>::iterator i;
	for (i = m_mapChildren.begin(); i != m_mapChildren.end(); i++) {
		CProfileTimer *pChild = i->second;
		delete pChild;
	}
}

/*
 */
std::string CProfileTimer::GetName() { 
	return m_strName; 
}

/*
 */
std::string CProfileTimer::GetProfilerStats(int nLevel) {
	static char szBuffer[1024];
	
	sprintf(szBuffer, "%8u : %8u : %5.1f : %*.*s%s\n", (unsigned int)m_nCallCount, (unsigned int)m_nTotalTime, m_pParent ? (m_nTotalTime * 100 / (float)m_pParent->m_nTotalTime) : 100.0f, nLevel, nLevel, " ", m_strName.c_str());
	std::string strBuild = szBuffer;
	
	if (nLevel == 0) strBuild = "   Count : Time(ms) :     % : Profile Name\n-------------------------------------------------------------\n" + strBuild;
	for (std::map<std::string, CProfileTimer*>::iterator i = m_mapChildren.begin(); i != m_mapChildren.end(); i++) strBuild += i->second->GetProfilerStats(nLevel + 1);
	
	return strBuild;
}

/*
 */
void CProfileTimer::Reset() {
	m_nTotalTime = 0;
	m_nCallCount = 0;
}

/*
 */
CProfileTimer *CProfileTimer::GetTimer(std::string strName) {
	CProfileTimer *&pTimer = m_mapChildren[strName];
	
	if (pTimer == NULL) pTimer = new CProfileTimer(strName, this);
	
	return pTimer;
}

/*
 */
void CProfileTimer::StartTimer() {
	LogAssert(m_nStartTime == (unsigned long)-1);
	m_nStartTime = SDL_GetTicks();
}

/*
 */
void CProfileTimer::StopTimer() {
	LogAssert(m_nStartTime != (unsigned long)-1);
	unsigned long nEndTime = SDL_GetTicks();
	m_nTotalTime += nEndTime - m_nStartTime;
	m_nStartTime = (unsigned long)-1;
	m_nCallCount++;
}

/******************************************************************************\
*
*	CProfiler
*
\******************************************************************************/

/*
*/
CProfiler::CProfiler(std::string strName, int nProfilingLevel, LoggingLevel nLoggingLevel) {
	m_nLoggingLevel = nLoggingLevel;
	m_nProfilingLevel = nProfilingLevel;
	m_pCurrentSample = NULL;
	m_pRootTimer = new CProfileTimer(strName, NULL);
	m_pRootTimer->StartTimer();
}

CProfiler::~CProfiler() {
	m_pRootTimer->StopTimer();
	Log(m_nLoggingLevel, m_pRootTimer->GetProfilerStats().c_str());
	delete m_pRootTimer;
}

/*
 */
bool CProfiler::IsProfiled(int nLevel) {
	return (m_nProfilingLevel != None && nLevel <= m_nProfilingLevel);
}

/******************************************************************************\
*
*	CProfilerSample
*
\******************************************************************************/

/*
 */
CProfileSample::CProfileSample(std::string strName, int nProfilingLevel) {
	m_pTimer = NULL;
	CProfiler *pProfiler = CProfiler::GetPtr();

	if (pProfiler->IsProfiled(nProfilingLevel)) {
		LogAssert(pProfiler->m_pCurrentSample != this);
		m_pParentSample = pProfiler->m_pCurrentSample;
		pProfiler->m_pCurrentSample = this;
		m_pTimer = (m_pParentSample ? m_pParentSample->m_pTimer : pProfiler->m_pRootTimer)->GetTimer(strName);
		m_pTimer->StartTimer();
	}
}

/*
 */
CProfileSample::~CProfileSample() {
	
	if (m_pTimer) {
		CProfiler *pProfiler = CProfiler::GetPtr();
		LogAssert(pProfiler->m_pCurrentSample == this);
		m_pTimer->StopTimer();
		pProfiler->m_pCurrentSample = m_pParentSample;
	}
}