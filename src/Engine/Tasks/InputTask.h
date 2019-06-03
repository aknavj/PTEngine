#ifndef __INPUT_TASK_H__
#define __INPUT_TASK_H__

/*
 */
interface  IInputEventListener {

	public:
		virtual void OnKeyDown(int nKey, int nMod) {}
		virtual void OnKeyUp(int nKey, int nMod) {}
		virtual void OnMouseDown(int nButton) {}
		virtual void OnMouseUp(int nButton) {}
		virtual void OnActivate(bool bActive) {}
		virtual void OnQuit() {}
};

/*
 */
class CInputTask : public CKernelTask, public TSingleton<CInputTask> {

	public:
		DEFAULT_TASK_CREATOR(CInputTask);

		virtual bool Start();
		virtual void Update();
		virtual void Stop();

		bool IsKeyDown(int n);
		bool IsMouseButtonDown(int n);
		int GetMouseX();
		int GetMouseY();

		void SetConsole(IInputEventListener *pConsole);
		bool IsConsoleActive();
		void AddInputEventListener(IInputEventListener *pListener);
		void RemoveInputEventListener(IInputEventListener *pListener);

	protected:
		int m_nKeyCount;
		unsigned char *m_pKeys;

		int m_nMouseX, m_nMouseY;
		unsigned int m_nMouseButtons;

		std::list<IInputEventListener *> m_listeners;

		IInputEventListener *m_pConsole;
		bool m_bConsoleActive;
};

#endif // __INPUT_TASK_H__
