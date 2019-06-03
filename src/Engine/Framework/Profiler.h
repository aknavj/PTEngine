#ifndef __PROFILER_H__
#define __PROFILER_H__

/*
 */
class CProfileTimer {

	public:

		CProfileTimer(std::string strName, CProfileTimer *pParent);
		~CProfileTimer();

		std::string GetName();
		std::string GetProfilerStats(int nLevel = 0);

		void Reset();

		CProfileTimer *GetTimer(std::string strName);

		void StartTimer();
		void StopTimer();

	protected:
		CProfileTimer *m_pParent;
		std::map<std::string, CProfileTimer*> m_mapChildren;

		std::string m_strName;
		unsigned long m_nStartTime;
		unsigned long m_nTotalTime;
		unsigned long m_nCallCount;
};

/*
 */
class CProfileSample;

/*
 */
class CProfiler : public TSingleton<CProfiler> {

	protected:
		LoggingLevel m_nLoggingLevel;
		int m_nProfilingLevel;
		CProfileSample *m_pCurrentSample;
		CProfileTimer *m_pRootTimer;

	public:
		CProfiler(std::string strName, int nProfilingLevel, LoggingLevel nLoggingLevel = Debug);
		~CProfiler();

		bool IsProfiled(int nLevel);
		friend class CProfileSample;
};

/*
 */
class CProfileSample {

	protected:
		CProfileSample *m_pParentSample;
		CProfileTimer *m_pTimer;

	public:
		CProfileSample(std::string strName, int nProfilingLevel);
		~CProfileSample();
};

#define ENABLE_PROFILING
#ifdef ENABLE_PROFILING
	#define PROFILE(name, level) CProfileSample _sample(name, level);
#else
	#define PROFILE(name, level)
#endif

#endif // __Profiler_h__
