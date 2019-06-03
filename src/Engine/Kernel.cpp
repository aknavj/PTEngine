#include "Engine.h"

/*
 */
void CKernel::Insert(CKernelTask::Ref task) {
	
	std::list<CKernelTask::Ref>::iterator it;
	for (it = m_lTasks.begin(); it != m_lTasks.end(); it++) {
		if ((*it)->GetPriority() > task->GetPriority())
			break;
	}
	
	m_lTasks.insert(it, task);
}

/*
 */
CKernel::CKernel() {
	SDL_Init(0);
}

CKernel::~CKernel() {
	SDL_Quit();
}

/*
 */
CKernel *CKernel::Create() {
	return new CKernel();
}

/*
 */
void CKernel::Destroy() {
	delete CKernel::GetPtr();
}

/*
 */
bool CKernel::IsTaskRunning(CKernelTask::Ref task) {
	
	for (std::list<CKernelTask::Ref>::iterator i = m_lTasks.begin(); i != m_lTasks.end(); i++) {
		if (*i == task) return true;
	}
	
	return false;
}

/*
 */
bool CKernel::AddTask(CKernelTask::Ref task) {
	
	if (!task->Start()) return false;
	Insert(task);
	
	return true;
}

/*
 */
void CKernel::RemoveTask(CKernelTask::Ref task) {
	for (std::list<CKernelTask::Ref>::iterator i = m_lTasks.begin(); i != m_lTasks.end(); i++) {
		if (*i == task) task->SetKillFlag();
	}
}

/*
 */
void CKernel::SuspendTask(CKernelTask::Ref task) {
	std::list<CKernelTask::Ref>::iterator i = std::find(m_lTasks.begin(), m_lTasks.end(), task);
	if (i != m_lTasks.end()) {
		m_lTasks.erase(i);
		m_lPaused.push_back(task);
	}
}

/*
 */
void CKernel::ResumeTask(CKernelTask::Ref task) {
	std::list<CKernelTask::Ref>::iterator i = std::find(m_lPaused.begin(), m_lPaused.end(), task);
	if (i != m_lPaused.end()) {
		m_lPaused.erase(i);
		Insert(task);
	}
}

/*
 */
void CKernel::KillAllTasks() {
	for (std::list<CKernelTask::Ref>::iterator i = m_lTasks.begin(); i != m_lTasks.end(); i++)
		(*i)->SetKillFlag();
}

/*
 */
int CKernel::Execute() {

	while (!m_lTasks.empty()) {
		
		PROFILE("Kernel task loop", 0);
		std::list<CKernelTask::Ref>::iterator i = m_lTasks.begin();

		while (i != m_lTasks.end()) {
			if ((*i)->GetKillFlag()) {
				(*i)->Stop();
				i = m_lTasks.erase(i);
			} else {
				(*i)->Update();
				i++;
			}
		}
	}

	return 0;
}