#ifndef __LOG_H__
#define __LOG_H__

/*
 */
enum LoggingLevel {
	None = 0,
	Critical = 1,
	Error = 2,
	Warning = 3,
	Info = 4,
	Notice = 5,
	Debug = 6,
	Spam = 7
};

#include "DateTime.h"

/*
 */
class CLog {

	public:
	
		/*
		 */
		CLog() {
			m_pSingleton = this;
		}

		/*
		 */
		~CLog() {
			m_pSingleton = NULL;
		}

		/*
		 */
		static bool IsValid()	{ 
			return m_pSingleton != NULL; 
		}
	
		/*
		 */
		static CLog *GetPtr() {
			return m_pSingleton;
		}

		/*
		 */
		static CLog &GetRef() {
			return *m_pSingleton;
		}

		/*
		 */
		void Init(int nLoggingLevel, const char *pszLogName) {
			static const char *pszSeverity[] = {"", "CRITICAL", "ERROR", "WARNING", "INFORMATION", "NOTICE", "DEBUG", "SPAM"};
			m_pszSeverity = pszSeverity;
			strcpy(m_szLogName, pszLogName);
			m_nLoggingLevel = nLoggingLevel;
			LogSwitch();
		}

		/*
		 */
		bool IsLogged(LoggingLevel nSeverity) { 
			return (nSeverity <= m_nLoggingLevel); 
		}

		/*
		 */
		void Log(LoggingLevel nSeverity, const char *pszMessage) {
			char szBuffer[256], szDateTime[40];
			CDateTime dtCurrent = CDateTime::GetCurrent();
	#ifdef MODULE_NAME
			sprintf(szBuffer, "%s  Severity: %-12.12s  Module: %-30.30s", dtCurrent.GetString(szDateTime), m_pszSeverity[nSeverity], MODULE_NAME);
	#else
			sprintf(szBuffer, "%s  Severity: %-12.12s", dtCurrent.GetString(szDateTime), m_pszSeverity[nSeverity]);
	#endif

			m_ofLog << szBuffer << std::endl << pszMessage << std::endl << std::endl;
			m_ofLog.flush();
		}

		/*
		 */
		void FormatLog_va(LoggingLevel nSeverity, const char *pszFormat, va_list args) {
			char szBuffer[16384];
			vsprintf(szBuffer, pszFormat, args);
			Log(nSeverity, szBuffer);
		}

		/*
		 */
		void FormatLog(LoggingLevel nSeverity, const char *pszFormat, ...) {
			va_list va;
			va_start(va, pszFormat);
			FormatLog_va(nSeverity, pszFormat, va);
			va_end(va);
		}

	protected:
		static CLog *m_pSingleton;
		const char **m_pszSeverity;
		std::ofstream m_ofLog;
		char m_szLogName[256];
		int m_nLoggingLevel;

		/*
		 */
		void LogSwitch() {
			//log přehození
			char szPath[_MAX_PATH];
			sprintf(szPath, "%s.log", m_szLogName);
			if (!m_ofLog.is_open())
				m_ofLog.open(szPath);
			return;
		}
};

/*
 */
inline bool IsLogged(LoggingLevel nSeverity) {
	if(CLog::IsValid()) return CLog::GetRef().IsLogged(nSeverity);
	return false;
}

/*
 */
inline void Log(LoggingLevel nSeverity, const char *pszMessage) {
	if(IsLogged(nSeverity)) CLog::GetRef().Log(nSeverity, pszMessage);
}

/*
 */
inline void FormatLog(LoggingLevel nSeverity, const char *pszFormat, ...) {
	if(IsLogged(nSeverity)) {
		va_list va;
		va_start(va, pszFormat);
		char szBuffer[16384];
		vsprintf(szBuffer, pszFormat, va);
		CLog::GetRef().Log(nSeverity, szBuffer);
		va_end(va);
	}
}

/*
 */
#define DECLARE_LOG_FUNC(func, level) \
inline void func(const char *pszFormat, ...)\
{\
	va_list va;\
	va_start(va, pszFormat);\
	CLog::GetRef().FormatLog_va(level, pszFormat, va);\
	va_end(va);\
}

DECLARE_LOG_FUNC(Critical_LogFunc, Critical);
DECLARE_LOG_FUNC(Error_LogFunc, Error);
DECLARE_LOG_FUNC(Warning_LogFunc, Warning);
DECLARE_LOG_FUNC(Info_LogFunc, Info);
DECLARE_LOG_FUNC(Notice_LogFunc, Notice);
DECLARE_LOG_FUNC(Debug_LogFunc, Debug);
DECLARE_LOG_FUNC(Spam_LogFunc, Spam);

#define LogCritical	if(IsLogged(Critical)) Critical_LogFunc
#define LogError	if(IsLogged(Error)) Error_LogFunc
#define LogWarning	if(IsLogged(Warning)) Warning_LogFunc
#define LogInfo		if(IsLogged(Info)) Info_LogFunc
#define LogNotice	if(IsLogged(Notice)) Notice_LogFunc
#define LogDebug	if(IsLogged(Debug)) Debug_LogFunc
#define LogSpam		if(IsLogged(Spam)) Spam_LogFunc

#ifdef _DEBUG
#define LogAssert(expr) \
	do {\
		if(!(expr)) {\
			LogCritical("Assertion failure at %s:%d", __FILE__, __LINE__);\
			if(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))\
				_CrtDbgBreak();\
		}\
	} while (0)
#else
#define LogAssert(expr) \
	do {\
		if(!(expr))\
			LogCritical("Assertion failure at %s:%d", __FILE__, __LINE__);\
	} while (0)
#endif

#ifdef WIN32
inline char *FormatLastError(DWORD nLastError, char *pszBuffer, int nLength=8192) {
	if(!::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), pszBuffer, nLength, NULL))
		sprintf(pszBuffer, "GetLastError() returned 0x%X", nLastError);
	return pszBuffer;
}
#endif

#endif // __LOG_H__
