#ifndef __DATOR_H__
#define __DATOR_H__

/*
 */
template <class T> T ToVal(std::string &str) {
	std::stringstream s;
	s.unsetf(std::ios::skipws);
	s << str;
	T res;
	s >> res;
	return res;
}

/*
 */
template <class T> std::string ToString(T &val) {
	std::stringstream s;
	s.unsetf(std::ios::skipws);
	s << val;
	std::string res;
	s >> res;
	return res;
}

/*
 */
class CDator : public CAutoRefCounter {

	protected:

		/*
		 */
		CDator() {}
		CDator(CDator &d){ *this = d; }

	public:
		/*
		 */
		typedef TReference<CDator> Ref;
		
		/*
		 */
		virtual CDator &operator=(std::string &str) = 0;
		virtual CDator &operator+=(std::string &str) = 0;
		virtual CDator &operator-=(std::string &str) = 0;
		virtual bool operator==(std::string &str) = 0;
		virtual bool operator!=(std::string &str) = 0;

		/*
		 */
		virtual bool HasMultipleValues() = 0;
		virtual operator std::string() = 0;
};

/*
 */
template<class T> class TDator : public CDator {
	protected:
		T &m_data;

	public:
		/*
		 */
		TDator(T &t) : m_data(t) {}

		/*
		 */
		virtual CDator &operator=(std::string &str)	{ m_data = ToVal<T>(str); return *this; }
		virtual CDator &operator+=(std::string &str) { m_data += ToVal<T>(str); return *this; }
		virtual CDator &operator-=(std::string &str) { m_data -= ToVal<T>(str); return *this; }
		virtual bool operator==(std::string &str) { return (str == (std::string)(*this)); }
		virtual bool operator!=(std::string &str) { return !(*this == str); }
		virtual operator std::string() { return ToString<T>(m_data); }
		virtual bool HasMultipleValues() { return false; }

		/*
		 */
		operator T&() { return m_data; }
};

/*
 */
template<class T> class TListDator : public CDator {

	protected:
		std::list<T> &m_data;

	public:
		/*
		 */
		TListDator(std::list<T> &l) : m_data(l) { }

		/*
		 */
		virtual CDator &operator=(std::string &str) {
			m_data.clear();

			size_t nStart = 0;
			size_t nEnd = str.find(';', nStart);

			while(nEnd != (size_t)-1) {
				m_data.push_back(ToVal<T>(str.substr(nStart, nEnd-nStart)));
				nStart = nEnd + 1;
				nEnd = str.find(';', nStart);
			}

			m_data.push_back(ToVal<T>(str.substr(nStart)));
			return *this;
		}

		/*
		 */
		virtual CDator &operator+=(std::string &str) { m_data.push_back(ToVal<T>(str)); return *this; }
		virtual CDator &operator-=(std::string &str) { m_data.remove(ToVal<T>(str)); return *this; }
		virtual bool operator==(std::string &str) { return find(m_data.begin(), m_data.end(), ToVal<T>(str)) != m_data.end(); }
		virtual bool operator!=(std::string &str) { return !(*this == str); }
		
		/*
		 */
		virtual operator std::string() {
			std::stringstream s;
			std::list<T>::iterator it = m_data.begin();

			for(it=m_data.begin(); it!=m_data.end(); it++) {
				if(it != m_data.begin()) s << ';';
				s << *it;
			}
			s << std::ends;
			return s.str();
		}

		/*
		 */
		virtual bool HasMultipleValues() { return true; }
		operator std::list<T>&() { return m_data; }
};

#endif // __DATOR_H__
