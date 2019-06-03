#ifndef __PROPERTY_SET_H__
#define __PROPERTY_SET_H__

/*
 */
class CPropertySet {

	public:

		CPropertySet(const char *pszPrefix = "");
		CPropertySet(const CPropertySet &prop, const char *pszPrefix = "");

		void SetPrefix(const char *pszPrefix = "");
		const char *GetPrefix() const;

		const CPropertySet &operator=(const CPropertySet &prop);

		const char *GetRootProperty(const char *pszKey, const char *pszDefault = NULL) const;
		const char *GetProperty(const char *pszKey, const char *pszDefault = NULL) const;
		int GetIntProperty(const char *pszKey, int nDefault = 0) const;
		float GetFloatProperty(const char *pszKey, float fDefault = 0.0f) const;
		int GetInt(const char *key, int def = 0);
		int GetRootInt(const char *pszKey, int nDefault = 0) const;

		void SetProperty(const char *pszKey, const char *pszValue);
		void SetProperty(const char *pszKey, int nValue);
		void SetProperty(const char *pszKey, float fValue);
		bool DeleteProperty(const char *pszKey);
		int DeletePrefix(const char *pszPrefix);

		void Clear();

		void Load(std::istream &in);
		void Save(std::ostream &out);

		void LoadFile(const char *pszPath);
		void SaveFile(const char *pszPath);

	protected:
		std::string m_strPrefix;
		std::map<std::string, std::string> m_mapProperties;
};

#endif // __PROPERTY_SET_H__
