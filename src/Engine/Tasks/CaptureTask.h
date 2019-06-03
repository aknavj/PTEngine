#if defined (_WIN32)

#ifndef __CAPTURE_TASK_H__
#define __CAPTURE_TASK_H__

#include <vfw.h>

/*
 */
class CCaptureTask : public CKernelTask, public TSingleton<CCaptureTask> {

	public:
		DEFAULT_TASK_CREATOR(CCaptureTask);

		void SetPath(const char *pszPath);

		virtual bool Start();
		virtual void Update();
		virtual void Stop();

		static HBITMAP CaptureScreen();

		void OpenAVI();
		void CloseAVI();

	protected:
		std::string m_strPath;
		PAVIFILE m_pAVIFile;
		PAVISTREAM m_pAVIStream;
		PAVISTREAM m_pAVICompressed;
		int m_nFrames;
};

#endif // __CAPTURE_TASK_H__
#endif // _WIN32
