#ifndef __VIDEO_TASK_H__
#define __VIDEO_TASK_H__

#include <SDL.h>
#include <SDL_syswm.h>

/*
 */
class CVideoTask : public CKernelTask, public TSingleton<CVideoTask> {

	public:
		DEFAULT_TASK_CREATOR(CVideoTask);

		virtual bool Start();
		virtual void Update();
		virtual void Stop();

		bool IsNVIDIA();
		bool IsATI();
		bool IsGeForceFX();
		bool IsGeForce6();
		bool IsRadeon9();
		bool HasExtension(const char *psz);
		CFont &GetFont();

		int GetWidth();
		int GetHeight();

	#ifdef _WIN32
		HWND GetHWnd() { return m_sdlInfo.window; }
		HGLRC GetHGLRC() { return m_sdlInfo.hglrc; }
	#endif

	protected:
		CFont m_fFont;
		char m_szFrameCount[20];
		int m_nTime;
		int m_nFrames;

		std::string m_strVendor;
		std::string m_strRenderer;
		std::string m_strVersion;
		std::string m_strExtensions;

		SDL_SysWMinfo m_sdlInfo;
		int m_nWidth, m_nHeight;
};

#endif // __VIDEO_TASK_H__
