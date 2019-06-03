#include "Engine.h"

/*
 */
CPropertySet::CPropertySet(const char *pszPrefix) {
	m_strPrefix = pszPrefix;
}

/*
 */
CPropertySet::CPropertySet(const CPropertySet &prop, const char *pszPrefix) {
	m_strPrefix = pszPrefix;
	*this = prop;
}

/*
 */
void CPropertySet::SetPrefix(const char *pszPrefix) { 
	m_strPrefix = pszPrefix; 
}

/*
 */
const char *CPropertySet::GetPrefix() const { 
	return m_strPrefix.c_str(); 
}

/*
 */
const CPropertySet &CPropertySet::operator=(const CPropertySet &prop) {
	
	std::map<std::string, std::string>::const_iterator i;
	for (i = prop.m_mapProperties.begin(); i != prop.m_mapProperties.end(); i++) {
		if (m_strPrefix.empty() || i->first.find(m_strPrefix) == 0)
			SetProperty(i->first.c_str(), i->second.c_str());
	}

	return *this;
}

/*
 */
const char *CPropertySet::GetRootProperty(const char *pszKey, const char *pszDefault) const {
	
	std::string strKey = pszKey;
	std::map<std::string, std::string>::const_iterator it = m_mapProperties.find(strKey);
	if (it != m_mapProperties.end()) return it->second.c_str();
	
	return pszDefault;
}

/*
 */
const char *CPropertySet::GetProperty(const char *pszKey, const char *pszDefault) const {
	
	std::string strKey = pszKey;
	std::map<std::string, std::string>::const_iterator it = m_mapProperties.find((m_strPrefix.empty() || strKey.find(m_strPrefix) == 0) ? strKey : m_strPrefix + strKey);
	if (it != m_mapProperties.end())
		return it->second.c_str();
	
	return pszDefault;
}

/*
 */
int CPropertySet::GetIntProperty(const char *pszKey, int nDefault) const {
	
	const char *psz = GetProperty(pszKey);
	if (psz != NULL) nDefault = atoi(psz);
	
	return nDefault;
}

/*
 */
float CPropertySet::GetFloatProperty(const char *pszKey, float fDefault) const {
	
	const char *psz = GetProperty(pszKey);
	if (psz != NULL) fDefault = (float)atof(psz);
	
	return fDefault;
}

/*
 */
int CPropertySet::GetInt(const char *key, int def) { 
	return GetIntProperty(key, def); 
}

/*
 */
int CPropertySet::GetRootInt(const char *pszKey, int nDefault) const {
	
	const char *psz = GetRootProperty(pszKey);
	if (psz != NULL) nDefault = atoi(psz);
	
	return nDefault;
}

/*
 */
void CPropertySet::SetProperty(const char *pszKey, const char *pszValue) {
	std::string strKey = pszKey;
	m_mapProperties[(m_strPrefix.empty() || strKey.find(m_strPrefix) == 0) ? strKey : m_strPrefix + strKey] = pszValue;
}

/*
 */
void CPropertySet::SetProperty(const char *pszKey, int nValue) {
	char szValue[20];
	itoa(nValue, szValue, 10);
	SetProperty(pszKey, szValue);
}

/*
 */
void CPropertySet::SetProperty(const char *pszKey, float fValue) {
	char szValue[20];
	sprintf(szValue, "%f", fValue);
	SetProperty(pszKey, szValue);
}

/*
 */
bool CPropertySet::DeleteProperty(const char *pszKey) {
	
	std::string strKey = pszKey;
	std::map<std::string, std::string>::iterator it = m_mapProperties.find((m_strPrefix.empty() || strKey.find(m_strPrefix) == 0) ? strKey : m_strPrefix + strKey);
	
	if (it != m_mapProperties.end()) {
		m_mapProperties.erase(it);
		return true;
	}
	
	return false;
}

/*
 */
int CPropertySet::DeletePrefix(const char *pszPrefix) {
	
	int nErased = 0;
	int nLength = (int)strlen(pszPrefix);
	
	std::map<std::string, std::string>::iterator it = m_mapProperties.begin();
	
	while (it != m_mapProperties.end()) {
		if (memcmp(pszPrefix, it->first.c_str(), nLength) == 0) {
			std::map<std::string, std::string>::iterator itDead = it;
			it++;
			m_mapProperties.erase(itDead);
			nErased++;
		}
		else {
			it++;
		}
	}

	return nErased;
}

/*
 */
void CPropertySet::Clear() {
	m_mapProperties.clear();
}

/*
 */
void CPropertySet::Load(std::istream &in) {
	
	char szBuffer[8192];
	in.getline(szBuffer, 8192);
	
	while (in) {
		int i = 0;
		while (szBuffer[i] != 0 && szBuffer[i] != '#' && szBuffer[i] != '=') i++;

		if (szBuffer[i] == '=') {
			szBuffer[i] = 0;
			
			int j = i + 1;
			while (szBuffer[j] != 0 && szBuffer[j] != '#') j++;
			while (szBuffer[j - 1] <= 0x20) j--;
			
			szBuffer[j] = 0;
			
			if (j > i) {
				if (m_strPrefix.empty() || std::string(&szBuffer[i + 1]).find(m_strPrefix) == 0) SetProperty(szBuffer, &szBuffer[i + 1]);
			}
		}

		in.getline(szBuffer, 8192);
	}
}

/*
 */
void CPropertySet::Save(std::ostream &out) {

	std::map<std::string, std::string>::iterator i;
	
	for (i = m_mapProperties.begin(); i != m_mapProperties.end(); i++) {
		if (m_strPrefix.empty() || i->first.find(m_strPrefix) == 0)
			out << i->first << "=" << i->second << std::endl;
	}
}

/*
 */
void CPropertySet::LoadFile(const char *pszPath) {
	std::ifstream in(pszPath);
	if (in) Load(in);
	in.close();
}

/*
 */
void CPropertySet::SaveFile(const char *pszPath) {
	std::ofstream out(pszPath);
	if (out) Save(out);
	out.close();
}