#ifndef __KERNEL_H__
#define __KERNEL_H__

///////////////////////
// forward declaration
///////////////////////
class CAutoRefCounter;

/*
 */
class CKernelTask : public CAutoRefCounter {

	protected:
		unsigned short m_nPriority;
		bool m_bKillFlag;

		CKernelTask() {
			m_nPriority = 0;
			m_bKillFlag = false;
		}

	public:
		typedef TReference<CKernelTask> Ref;

		void SetKillFlag() { m_bKillFlag = true; }
		bool GetKillFlag() { return m_bKillFlag; }
		unsigned short GetPriority() { return m_nPriority; }

		virtual bool Start() = 0;
		virtual void Stop() = 0;
		virtual void Update() = 0;
		virtual void OnSuspend() { }
		virtual void OnResume() { }
};

/*
*/
#define DEFAULT_TASK_CREATOR(class_name)\
public:\
	typedef TReference<class_name> Ref;\
	static class_name *Create(int nPriority)\
		{\
		LogDebug("Creating kernel task %s", #class_name);\
		class_name *pTask = new class_name();\
		pTask->m_nPriority = nPriority;\
		return pTask;\
		}

/*
 */
class CKernel : public TSingleton<CKernel> {

	public:
		static CKernel *Create();
		static void Destroy();

		bool IsTaskRunning(CKernelTask::Ref task);
		bool AddTask(CKernelTask::Ref task);
		void RemoveTask(CKernelTask::Ref task);
		void SuspendTask(CKernelTask::Ref task);
		void ResumeTask(CKernelTask::Ref task);
		void KillAllTasks();

		int Execute();

	protected:
		std::list<CKernelTask::Ref> m_lTasks;
		std::list<CKernelTask::Ref> m_lPaused;

		void Insert(CKernelTask::Ref task);

		CKernel();
		~CKernel();
};

#endif // __Kernel_h__
