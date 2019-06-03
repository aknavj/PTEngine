#ifndef __PLANETARY_MAP_COORD_H__
#define __PLANETARY_MAP_COORD_H__

/*
 */
enum {
	RightFace = 0,
	LeftFace = 1,
	TopFace = 2,
	BottomFace = 3,
	FrontFace = 4,
	BackFace = 5
};

/*
 */
enum {
	NoState = 0,
	Borrowed = 1
};

// global stuff
extern unsigned char GetFace(const CVector &v);
extern void GetFaceCoordinates(unsigned char nFace, const CVector &v, float &x, float &y);

/*
 */
inline unsigned char GetFaceCoordinates(const CVector &v, float &x, float &y) {
	unsigned char nFace = GetFace(v);
	GetFaceCoordinates(nFace, v, x, y);
	return nFace;
}

/*
 */
extern CVector GetPlanetaryVector(unsigned char nFace, float x, float y, float fLength=1.0f);

/*
 */
class CFaceCoord {

	public:

		float x, y;

	public:
		
		/*
		 */
		CFaceCoord() {}
		
		/*
		 */
		CFaceCoord(const CFaceCoord &c) {
			*this = c;
		}
		
		/*
		 */
		CFaceCoord(unsigned char nFace, const CVector &v) {
			Init(nFace, v);
		}
		
		/*
		 */
		CFaceCoord(float x, float y) {
			Init(x, y);
		}

		/*
		 */
		void Init(unsigned char nFace, const CVector &v) {
			::GetFaceCoordinates(nFace, v, x, y);
		}
		
		/*
		 */
		void Init(float x, float y) {
			this->x = x;
			this->y = y;
		}

		/*
		 */
		const CFaceCoord &operator=(const CFaceCoord &c) {
			x = c.x;
			y = c.y;
			return *this;
		}

		/*
		 */
		CVector GetVector(int nFace, float fSize=1.0f) const {
			return ::GetPlanetaryVector(nFace, x, y, fSize);
		}
};

/*
 */
class CPlanetaryMapCoord : public CFaceCoord {

	protected:
		float m_fHeight;
		unsigned char m_nFace;
		unsigned char m_nState;

	public:
		/*
		 */
		CPlanetaryMapCoord() {}

		/*
		 */
		CPlanetaryMapCoord(const CPlanetaryMapCoord &c) {
			*this = c;
		}

		/*
		 */
		CPlanetaryMapCoord(unsigned char nFace, const CFaceCoord &c, float fHeight=0) {
			Init(nFace, c, fHeight);
		}

		/*
		 */
		CPlanetaryMapCoord(const CVector &v, float fHeight=0) {
			Init(v, fHeight);
		}

		/*
		 */
		CPlanetaryMapCoord(unsigned char nFace, const CVector &v, float fHeight=0) {
			Init(nFace, v, fHeight);
		}

		/*
		 */
		CPlanetaryMapCoord(unsigned char nFace, float x, float y, float fHeight=0) {
			Init(nFace, x, y, fHeight);
		}

		/*
		 */
		void Init(unsigned char nFace, const CFaceCoord &c, float fHeight=0) {
			m_nFace = nFace;
			m_nState = NoState;
			m_fHeight = fHeight;
			x = c.x;
			y = c.y;
		}

		/*
		 */
		void Init(const CVector &v, float fHeight=0) {
			m_nFace = ::GetFace(v);
			m_nState = NoState;
			m_fHeight = fHeight;
			CFaceCoord::Init(m_nFace, v);
		}

		/*
		 */
		void Init(unsigned char nFace, const CVector &v, float fHeight=0) {
			m_nFace = nFace;
			m_nState = NoState;
			m_fHeight = fHeight;
			CFaceCoord::Init(m_nFace, v);
		}

		/*
		 */
		void Init(unsigned char nFace, float x, float y, float fHeight=0) {
			m_nFace = nFace;
			m_nState = NoState;
			m_fHeight = fHeight;
			CFaceCoord::Init(x, y);
		}
	
		/*
		 */
		const CPlanetaryMapCoord &operator=(const CPlanetaryMapCoord &coord) {
			m_nFace = coord.m_nFace;
			m_nState = coord.m_nState;
			m_fHeight = coord.m_fHeight;
			x = coord.x;
			y = coord.y;
			return *this;
		}

		/*
		 */
		void SetState(unsigned char n) { m_nState = n; }
		void SetHeight(float f) { m_fHeight = f; }
		unsigned char GetFace() const { return m_nFace; }
		unsigned char GetState() const { return m_nState; }
		float GetHeight() const { return m_fHeight; }

		CVector GetDirectionVector() const { return CFaceCoord::GetVector(m_nFace); }
		CVector GetPositionVector() const { return GetPositionVector(m_fHeight); }
		CVector GetPositionVector(float fHeight) const	{ return CFaceCoord::GetVector(m_nFace, fHeight); }
};

#endif /* __PLANETARY_MAP_COORD_H__ */
