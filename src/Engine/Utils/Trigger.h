#ifndef __TRIGGER_H__
#define __TRIGGER_H__

/*
 */
class CTrigger : public CAutoRefCounter {

	public:
		typedef TReference<CTrigger> Ref;

		/*
		 */
		virtual bool Test(unsigned long nTime) = 0;
	
		/*
		 */
		bool Fire(unsigned long nTime) {
			if(nTime - m_nLastFire >= m_nFireDelay) {
				(*m_refHandler)();

				if(m_bFireOnce) return false;
				m_nLastFire = nTime;
			}
			return true;
		}

	protected:
		CFunctor::Ref m_refHandler;
		bool m_bFireOnce;
		unsigned long m_nFireDelay;
		unsigned long m_nLastFire;

		/*
		 */
		CTrigger(CFunctor *pHandler, bool bFireOnce, unsigned long nFireDelay = 0) {
			m_refHandler = pHandler;
			m_bFireOnce = bFireOnce;
			m_nFireDelay = nFireDelay;
			m_nLastFire = 0;
		}
};

/*
 */
class CTriggerTask : public CKernelTask, public TSingleton<CTriggerTask> {

	protected:
		std::list<CTrigger::Ref> m_list;

	public:

		DEFAULT_TASK_CREATOR(CTriggerTask);

		/*
		 */
		virtual bool Start() {
			return true;
		}
		
		/*
		 */
		virtual void Update() {
			PROFILE("CTriggerTask::Update()", 1);

			unsigned long nTime = CTimerTask::GetRef().GetGameTicks();
			for(std::list<CTrigger::Ref>::iterator it = m_list.begin(); it != m_list.end();) {
				if((*it)->Test(nTime)) it++;
				else it = m_list.erase(it);
			}
		}
		
		/*
		 */
		virtual void Stop() {
			m_list.clear();
		}
		
		/*
		 */
		void Add(CTrigger::Ref ref) {
			m_list.push_back(ref);
		}

		/*
		 */
		void Kill(CTrigger::Ref ref) {
			std::list<CTrigger::Ref>::iterator i = std::find(m_list.begin(), m_list.end(), ref);
			if(i != m_list.end()) m_list.erase(i);
		}
};

/*
 */
template <class T> class TEQTrigger : public CTrigger {

	protected:
		T &m_tTarget;
		T m_tValue;

		/*
		 */
		TEQTrigger(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_tValue = tValue;
		}

	public:
		/*
		 */
		static CTrigger *Create(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TEQTrigger *p = new TEQTrigger(tTarget, tValue, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget == m_tValue) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TNETrigger : public CTrigger {
	
	protected:
		T &m_tTarget;
		T m_tValue;

		/*
		 */
		TNETrigger(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_tValue = tValue;
		}

	public:

		/*
		 */
		static CTrigger *Create(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TNETrigger *p = new TNETrigger(tTarget, tValue, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget != m_tValue)
				return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TGRTrigger : public CTrigger {
	protected:
		T &m_tTarget;
		T m_tValue;

		TGRTrigger(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_tValue = tValue;
		}

	public:

		/*
		 */
		static CTrigger *Create(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TGRTrigger *p = new TGRTrigger(tTarget, tValue, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget > m_tValue) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TGETrigger : public CTrigger {

	protected:
		T &m_tTarget;
		T m_tValue;

		/*
		 */
		TGETrigger(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_tValue = tValue;
		}

	public:
		/*
		 */
		static CTrigger *Create(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TGETrigger *p = new TGETrigger(tTarget, tValue, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget >= m_tValue) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TLTTrigger : public CTrigger {

	protected:
		T &m_tTarget;
		T m_tValue;

		/*
		 */
		TLTTrigger(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_tValue = tValue;
		}

	public:

		/*
		 */
		static CTrigger *Create(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TLTTrigger *p = new TLTTrigger(tTarget, tValue, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget < m_tValue) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TLETrigger : public CTrigger {

	protected:
		T &m_tTarget;
		T m_tValue;

		/*
		 */
		TLETrigger(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_tValue = tValue;
		}

	public:
		
		/*
		 */
		static CTrigger *Create(T &tTarget, T tValue, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TLETrigger *p = new TLETrigger(tTarget, tValue, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget <= m_tValue) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TAndTrigger : public CTrigger {

	protected:
		T &m_tTarget;
		T m_nMask;

		/*
		 */
		TAndTrigger(T &tTarget, T nMask, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_nMask = nMask;
		}

	public:
		
		/*
		 */
		static CTrigger *Create(T &tTarget, int nBit, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TAndTrigger *p = new TAndTrigger(tTarget, nBit, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget & m_nMask) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TOrTrigger : public CTrigger {

	protected:
		T &m_tTarget;
		T m_nMask;

		/*
		 */
		TOrTrigger(T &tTarget, T nMask, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_nMask = nMask;
		}

	public:
		/*
		 */
		static CTrigger *Create(T &tTarget, int nBit, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TOrTrigger *p = new TOrTrigger(tTarget, nBit, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget | m_nMask) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TNandTrigger : public CTrigger {
	protected:
		T &m_tTarget;
		T m_nMask;

		/*
		 */
		TNandTrigger(T &tTarget, T nMask, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_nMask = nMask;
		}

	public:
		/*
		 */
		static CTrigger *Create(T &tTarget, int nBit, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TNandTrigger *p = new TNandTrigger(tTarget, nBit, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(!(m_tTarget & m_nMask)) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TNorTrigger : public CTrigger {
	protected:
		T &m_tTarget;
		T m_nMask;

		/*
		 */
		TNorTrigger(T &tTarget, T nMask, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_nMask = nMask;
		}

	public:
		/*
		 */
		static CTrigger *Create(T &tTarget, int nBit, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TNorTrigger *p = new TNorTrigger(tTarget, nBit, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}
		
		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(!(m_tTarget | m_nMask)) return Fire(nTime);
			return true;
		}
};

/*
 */
template <class T> class TXorTrigger : public CTrigger {
	protected:
		T &m_tTarget;
		T m_nMask;

		/*
		 */
		TXorTrigger(T &tTarget, T nMask, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay), m_tTarget(tTarget) {
			m_nMask = nMask;
		}

	public:
		
		/*
		 */
		static CTrigger *Create(T &tTarget, int nBit, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			TXorTrigger *p = new TXorTrigger(tTarget, nBit, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			if(m_tTarget ^ m_nMask) return Fire(nTime);
			return true;
		}
};

/*
 */
class CTimeTrigger : public CTrigger {

	protected:

		/*
		 */
		CTimeTrigger(int nTicks, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) : CTrigger(pHandler, bFireOnce, nFireDelay) {
			m_nFireDelay = nTicks;
			m_nLastFire = CTimerTask::GetRef().GetGameTicks();
		}

	public:
		
		/*
		 */
		static CTrigger *Create(int nTicks, CFunctor *pHandler, bool bFireOnce=true, unsigned long nFireDelay=0) {
			CTimeTrigger *p = new CTimeTrigger(nTicks, pHandler, bFireOnce, nFireDelay);
			CTriggerTask::GetPtr()->Add(p);
			return p;
		}

		/*
		 */
		virtual bool Test(unsigned long nTime) {
			return Fire(nTime);
		}
};

#endif // __Trigger_h__
