#ifndef __OBJECT_H__
#define __OBJECT_H__

///////////////////////
// forward declaration
///////////////////////
class CObjectBase;

/*
 */
class CObjectType : public CManualRefCounter {

	public:
		typedef TReference<CObjectType> Ref;
		typedef std::list<CObjectType *>::iterator iterator;
		typedef std::list<CObjectType *>::const_iterator const_iterator;

	protected:
		static CObjectType m_root;

		unsigned int m_nTypeID;
		std::string m_strTypeName;
		CObjectType::Ref m_refParentType;
		std::list<CObjectType *> m_lDerivedTypes;

		/*
		 */
		static unsigned int GenerateTypeID(const char *psz);

	public:
	
		/*
		 */
		CObjectType();

		/*
		 */
		CObjectType(CObjectType *pParent, const char *pszTypeName);

		virtual ~CObjectType();

		void AddRef();

		void Release();

		static CObjectType *GetInstance();
		static CObjectType *FindType(unsigned int nTypeID);
		static CObjectType *FindType(const char *pszTypeName);

		CObjectType *FindDerivedType(unsigned int nTypeID) const;
		CObjectType *FindParentType(unsigned int nTypeID) const;

		bool IsA(unsigned int nTypeID) const;
		bool IsA(const char *pszTypeName) const;
		bool IsA(CObjectType *pType) const;

		unsigned int GetTypeID() const;
		const char *GetTypeName() const;
		CObjectType *GetParentType() const;
		const_iterator begin() const;
		const_iterator end() const;

		virtual CObjectBase *CreateObject();
		static CObjectBase *CreateObject(unsigned int nTypeID);
		static CObjectBase *CreateObject(const char *pszTypeName);
};

/*
 */
template <class ObjectType, class ParentType> class TObjectType : public CObjectType, public TAutoSingleton<ObjectType> {
	public:
		TObjectType(const char *pszObjectName) : CObjectType(ParentType::GetInstance(), pszObjectName) {}
};

/*
 */
#define DECLARE_GENERIC_TYPE_CLASS(ObjectClass, ParentTypeClass)					\
	class ObjectClass##Type : public TObjectType<ObjectClass##Type, ParentTypeClass>	\
	{																					\
	public:																				\
		ObjectClass##Type() : TObjectType<ObjectClass##Type, ParentTypeClass>(#ObjectClass) {}\
		static CObjectType *GetInstance()	{ return GetPtr(); }						\
		static void Load()					{ new ObjectClass##Type(); }				\
		static void Unload()				{ delete ObjectClass##Type::GetPtr(); }		\
		virtual CObjectBase *CreateObject()	{ return new ObjectClass(this); }			\
	}

/*
 */
class CObjectBase : public CAutoRefCounter {
	
	protected:
		const CObjectType::Ref m_refType;

	protected:
		
		/*
		 */
		CObjectBase(CObjectType *pType=CObjectType::GetInstance()) : m_refType(pType)	{}
		virtual ~CObjectBase() {}

	public:
		typedef TReference<CObjectBase> Ref;

		/*
		 */
		CObjectType *GetType() const { return m_refType; }
		
		/*
		 */
		bool IsA(unsigned int nTypeID) const { return m_refType->IsA(nTypeID); }
		bool IsA(const char *pszTypeName) const	{ return m_refType->IsA(pszTypeName); }
		bool IsA(CObjectType *pType) const { return m_refType->IsA(pType); }
		unsigned int GetTypeID() const { return m_refType->GetTypeID(); }
		const char *GetTypeName() const	{ return m_refType->GetTypeName(); }
};

#endif /* __OBJECT_H__ */
