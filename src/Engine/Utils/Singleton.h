#ifndef __SINGLETON_H__
#define __SINGLETON_H__

/*
 */
template <class T> class TSingleton {

	private:
		static T *m_pSingleton;

	protected:

		/*
		 */
		TSingleton() {
			LogAssert(!m_pSingleton);
			m_pSingleton = (T *)this; // msvc ignores bug

			// aritmetic pointer offset addr
			//int nOffset = (int)(T*)1 - (int)(TSingleton <T>*)(T*)1;
			//m_pSingleton = (T*)((int)this + nOffset);
		}
	
		/*
		 */
		~TSingleton() {
			LogAssert(m_pSingleton);
			m_pSingleton = NULL;
		}

	public:
		/*
		 */
		static bool IsValid() { 
			return m_pSingleton != NULL; 
		}
	
		/*
		 */
		static T *GetPtr() {
			LogAssert(m_pSingleton);
			return m_pSingleton;
		}

		/*
		 */
		static T &GetRef() {
			LogAssert(m_pSingleton);
			return *m_pSingleton;
		}
};

// static declaration
template <class T> T *TSingleton<T>::m_pSingleton = NULL;

/*
 */
// auto instancing
template <class T> class TAutoSingleton : public TSingleton<T> {

	public:
		static T *GetPtr() {
			if(!TSingleton<T>::IsValid()) new T();
			return TSingleton<T>::GetPtr();
		}

		static T &GetRef() {
			if(!TSingleton<T>::IsValid()) new T();
			return TSingleton<T>::GetRef();
		}
};

#endif // __Singleton_h__

