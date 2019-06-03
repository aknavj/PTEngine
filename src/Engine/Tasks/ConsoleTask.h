#ifndef __CONSOLE_TASK_H__
#define __CONSOLE_TASK_H__

#define CONSOLE_ROW_HEIGHT	60
#define CONSOLE_ROW_HISTORY	60
#define CONSOLE_PROMPT		"> "

/*
 */
class CConsoleTask : public CKernelTask, public IInputEventListener, public TSingleton<CConsoleTask> {

		DEFAULT_TASK_CREATOR(CConsoleTask);

	public:

		virtual bool Start();
		virtual void Update();
		virtual void Stop();

		virtual void OnSuspend() { }
		virtual void OnResume() { }

		virtual void OnKeyDown(int nKey, int nMod);

	protected:
		std::list<std::string> m_listRows;
		char m_szCommand[256];
		int m_nCommandLength;

		void AddRow(std::string str);
};

#endif // __ConsoleTask_h__
