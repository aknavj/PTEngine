#include "Engine.h"

/******************************************************************************\
*
*	CObjectType
*
\******************************************************************************/

/*
 */
unsigned int CObjectType::GenerateTypeID(const char *psz) {
	unsigned int nHash = 0;
	while (*psz) {
		char c = *psz++;
		if (c >= 'A' && c <= 'Z') c -= 0x20;
		nHash = ((nHash << 3) + (nHash >> 28) + c);
	}
	return nHash;
}

/*
 */
CObjectType::CObjectType() {
	m_strTypeName = "CObjectBase";
	m_nTypeID = 0;
}

/*
 */
CObjectType::CObjectType(CObjectType *pParent, const char *pszTypeName) {
	LogAssert(pParent);
		
	m_refParentType = pParent;
	m_strTypeName = pszTypeName;
	m_nTypeID = GenerateTypeID(m_strTypeName.c_str());
		
	LogDebug("Creating object type %s (TypeID=%u).", m_strTypeName.c_str(), m_nTypeID);
	CObjectType *pFind = FindType(m_nTypeID);
	
	if (pFind) LogError("Object type %s (TypeID=%u) and object type %s (ID=%u) have conflicting type ID's", m_strTypeName.c_str(), m_nTypeID, pFind->GetTypeName(), pFind->GetTypeID());
	if (m_refParentType) m_refParentType->m_lDerivedTypes.push_back(this);
}

/*
 */
CObjectType::~CObjectType() {
	while (!m_lDerivedTypes.empty()) delete m_lDerivedTypes.front();

	if (m_nTypeID) {
		LogDebug("Destroying object type %s (TypeID=%u).", m_strTypeName.c_str(), m_nTypeID);
		LogAssert(m_nReferenceCount == 0);
		
		for (iterator it = m_refParentType->m_lDerivedTypes.begin(); it != m_refParentType->m_lDerivedTypes.end(); it++) {
			if (*it == this) {
					m_refParentType->m_lDerivedTypes.erase(it);
					break;
			}
		}
	}
}

/*
 */
void CObjectType::AddRef() {
	LogSpam("Creating object %s (TypeID=%u).", m_strTypeName.c_str(), m_nTypeID);
	CManualRefCounter::AddRef();
}

/*
 */
void CObjectType::Release() {
	LogSpam("Destroying object %s (TypeID=%u).", m_strTypeName.c_str(), m_nTypeID);
	CManualRefCounter::Release();
}

/*
 */
CObjectType *CObjectType::GetInstance() {
	return &m_root; 
}

/*
 */
CObjectType *CObjectType::FindType(unsigned int nTypeID) {
	return m_root.FindDerivedType(nTypeID); 
}

/*
 */
CObjectType *CObjectType::FindType(const char *pszTypeName) {
	return FindType(GenerateTypeID(pszTypeName)); 
}

/*
 */
CObjectType *CObjectType::FindDerivedType(unsigned int nTypeID) const {
	if (nTypeID == m_nTypeID) return (CObjectType *)this;

	CObjectType *pType = NULL;
	for (const_iterator it = m_lDerivedTypes.begin(); !pType && it != m_lDerivedTypes.end(); it++) pType = (*it)->FindDerivedType(nTypeID);
	
	return pType;
}

/*
 */
CObjectType *CObjectType::FindParentType(unsigned int nTypeID) const {
	CObjectType *pType = (CObjectType *)this;
	while (pType) {
		if (nTypeID == pType->m_nTypeID)
			return pType;
		pType = pType->m_refParentType;
	}
	return NULL;
}

/*
 */
bool CObjectType::IsA(unsigned int nTypeID) const { 
	return FindParentType(nTypeID) != NULL; 
}

/*
 */
bool CObjectType::IsA(const char *pszTypeName) const { 
	return IsA(GenerateTypeID(pszTypeName)); 
}

/*
 */
bool CObjectType::IsA(CObjectType *pType) const { 
	return IsA(pType->GetTypeID()); 
}

/*
 */
unsigned int CObjectType::GetTypeID() const { 
	return m_nTypeID; 
}

/*
 */
const char *CObjectType::GetTypeName() const { 
	return m_strTypeName.c_str(); 
}

/*
 */
CObjectType *CObjectType::GetParentType() const	{ 
	return m_refParentType; 
}

/*
 */
CObjectType::const_iterator CObjectType::begin() const {
	return m_lDerivedTypes.begin(); 
}

/*
 */
CObjectType::const_iterator CObjectType::end() const	{
	return m_lDerivedTypes.end(); 
}

/*
 */
CObjectBase *CObjectType::CreateObject() { 
	return NULL; 
}

/*
 */
CObjectBase *CObjectType::CreateObject(unsigned int nTypeID) {
	CObjectType *pType = m_root.FindDerivedType(nTypeID);
	if (!pType) return NULL;
	return pType->CreateObject();
}

/*
 */
CObjectBase *CObjectType::CreateObject(const char *pszTypeName) {
	return CreateObject(GenerateTypeID(pszTypeName)); 
}