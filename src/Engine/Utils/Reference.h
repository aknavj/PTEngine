#ifndef __REFERENCE_H__
#define __REFERENCE_H__

/*
 */
template <class T> class TReference {

	protected:
		T *m_pObject;

	public:
	
		/*
		 */
		TReference(T *p=NULL) {
			if(m_pObject = p) m_pObject->AddRef();
		}

		/*
		 */
		TReference(const TReference &ref) {
			if(m_pObject = ref.m_pObject) m_pObject->AddRef();
		}

		/*
		 */
		~TReference() {
			if(m_pObject) m_pObject->Release();
		}

		/*
		 */
		void AddRef() { m_pObject->AddRef(); }
		void Release() { m_pObject->Release(); m_pObject = NULL; }

		/*
		 */
		operator T*() const {
			return m_pObject;
		}
	
		/*
		 */
		T& operator*() const {
			LogAssert(m_pObject);
			return *m_pObject;
		}
	
		/*
		 */
		T* operator->() const {
			LogAssert(m_pObject);
			return m_pObject;
		}

		/*
		 */
		TReference &operator=(const TReference &ref) {
			if(m_pObject != ref.m_pObject) {
				if(m_pObject) m_pObject->Release();
				if(m_pObject = ref.m_pObject) m_pObject->AddRef();
			}
			return *this;
		}

		/*
		 */
		TReference &operator=(T* p) {
			if(m_pObject != p) {
				if(m_pObject) m_pObject->Release();
				if(m_pObject = p) m_pObject->AddRef();
			}
			return *this;
		}

		/*
		 */
		bool operator==(T* p) const	{ return m_pObject == p; }
		bool operator!=(T* p) const	{ return m_pObject != p; }
		bool operator==(const TReference &ref) const { return m_pObject == ref.m_pObject; }
		bool operator!=(const TReference &ref) const { return m_pObject != ref.m_pObject; }

		// reflection
		bool eq(const TReference &ref) const { return (*m_pObject) == (*ref.m_pObject); }
		bool ne(const TReference &ref) const { return (*m_pObject) != (*ref.m_pObject); }
		bool gt(const TReference &ref) const { return (*m_pObject) > (*ref.m_pObject); }
		bool lt(const TReference &ref) const { return (*m_pObject) < (*ref.m_pObject); }
		bool ge(const TReference &ref) const { return (*m_pObject) >= (*ref.m_pObject); }
		bool le(const TReference &ref) const { return (*m_pObject) <= (*ref.m_pObject); }
};

/*
 */
class CManualRefCounter {

	protected:
		unsigned short m_nReferenceCount;
		CManualRefCounter()	{ m_nReferenceCount = 0; }
		~CManualRefCounter() { LogAssert(m_nReferenceCount == 0); }

	public:
		unsigned short AddRef() {
			LogAssert(GetReferenceCount() < 0x7FFF);
			m_nReferenceCount++;
			//m_nReferenceCount = InterlockedIncrement((LPLONG)&m_nReferenceCount);
			return GetReferenceCount();
		}

		unsigned short Release() {
			LogAssert(GetReferenceCount() > 0);
			m_nReferenceCount--;
			//m_nReferenceCount = InterlockedDecrement((LPLONG)&m_nReferenceCount);
			return GetReferenceCount();
		}

		unsigned short GetReferenceCount()	{ return (m_nReferenceCount & 0x7FFF); }
		bool IsDead() { return (m_nReferenceCount & 0x8000) == 0; }
		void Kill()	{ m_nReferenceCount |= 0x8000; }
};

/*
 */
class CAutoRefCounter : public CManualRefCounter {
	protected:
		virtual ~CAutoRefCounter() { }
	public:
		void Release() { if (!CManualRefCounter::Release()) delete this; }
};

/*
*/
template <class T> class TAutoRefCounter : public CManualRefCounter {
	public:
		void Release() { if (!CManualRefCounter::Release()) delete (T *)this; }
		typedef TReference<T> Ref;
};

/*
*/
class CAbstractRefCounter : public CManualRefCounter {
	protected:
		virtual void Destroy() = 0;
		virtual ~CAbstractRefCounter()	{}
	public:
		void Release() { if (!CManualRefCounter::Release()) Destroy(); }
};

#endif /* __REFERENCE_H__ */
