#ifndef __FACTORY_H__
#define __FACTORY_H__

/*
 */
template<class BaseClass, class ObjectClass> BaseClass *CreateObject() {
   return new ObjectClass();
}

/*
 */
template <class BaseClass, class IDType> class TClassFactory {

	protected:
		typedef BaseClass *(*CreateObjectFunc)();
		std::map<IDType, CreateObjectFunc> m_mapObjectClasses;

	public:
		template<class ObjectClass>

		/*
		 */
		bool Register(IDType id) {
			if(m_mapObjectClasses.find(id) != m_mapObjectClasses.end()) return false;
			m_mapObjectClasses[id] = &CreateObject<BaseClass, ObjectClass>;

			return true;
		}

		/*
		 */
		bool Unregister(IDType id) {
			return (m_mapObjectClasses.erase(id) == 1);
		}

		/*
		 */
		BaseClass *Create(IDType id) {
			if(m_mapObjectClasses.find(id) == m_mapObjectClasses.end()) return NULL;
			return (m_mapObjectClasses[id])();
		}
};

#endif /* __FACTORY_H__ */