#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

/*
 */
class CTimerTask : public CKernelTask, public TSingleton<CTimerTask> {

	public:
		DEFAULT_TASK_CREATOR(CTimerTask);

		virtual bool Start();
		virtual void Update();
		virtual void Stop();

		void LockFrameRate(unsigned long nTicks);
		void UnlockFrameRate();

		unsigned long GetGameTicks();
		float GetGameSeconds();

		unsigned long GetFrameTicks();
		float GetFrameSeconds();

	protected:

#if defined(_WIN32)
		double m_dFactor;

		LARGE_INTEGER m_nGameStart;
		unsigned long m_nGameTicks;
		float m_fGameSeconds;

		LARGE_INTEGER m_nFrameStart;
		unsigned long m_nFrameTicks;
		float m_fFrameSeconds;

		unsigned long m_nLockedRate;
#else
		unsigned long m_nGameStart;
		unsigned long m_nGameTicks;
		float m_fGameSeconds;

		unsigned long m_nFrameStart;
		unsigned long m_nFrameTicks;
		float m_fFrameSeconds;
#endif
};

#endif // __TimerTask_h__
