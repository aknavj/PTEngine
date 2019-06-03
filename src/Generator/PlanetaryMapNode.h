#ifndef __PLANETARY_MAP_NODE_H__
#define __PLANETARY_MAP_NODE_H__

#include "PlanetaryMap.h"

// Map surface 16 quads to single node (64 tris and (8+1)x(8+1))
#define NODE_WIDTH			8
#define HEIGHT_MAP_SCALE	2

#define SURFACE_MAP_WIDTH	(NODE_WIDTH+1)
#define SURFACE_MAP_COUNT	(SURFACE_MAP_WIDTH*SURFACE_MAP_WIDTH)

#define HEIGHT_MAP_WIDTH	(NODE_WIDTH*HEIGHT_MAP_SCALE+1)
#define HEIGHT_MAP_COUNT	(HEIGHT_MAP_WIDTH*HEIGHT_MAP_WIDTH)

#define BORDER_MAP_WIDTH	(HEIGHT_MAP_WIDTH+2)
#define BORDER_MAP_COUNT	(BORDER_MAP_WIDTH*BORDER_MAP_WIDTH)

/*
 */
enum {	
	TopLeft = 0,
	TopRight = 1,
	BottomLeft = 2,
	BottomRight = 3
};

/*
 */
enum {
	TopEdge = 0,
	RightEdge = 1,
	BottomEdge = 2,
	LeftEdge = 3
};

/*
 */
enum {
	FaceMask = 0x0007,
	QuadrantMask = 0x0018,
	LevelMask = 0x03E0,
	CameraInMask = 0x0C00,
	NodeDirty = 0x1000,

	BeyondHorizonMask = 0x000F0000,
	OutsideFrustumMask = 0x00F00000,
};

/*
 */
class CNodeBuilder {

	public:

		CPixelBuffer pb;
		CPlanetaryMapCoord coord[BORDER_MAP_COUNT];
		CVector vNormal[HEIGHT_MAP_COUNT];

		/*
		 */
		void ComputeNormals() {
			int nIndex = 0;

			for(int y=0; y<HEIGHT_MAP_WIDTH; y++) {
				int nCoord = (y+1)*BORDER_MAP_WIDTH + 1;
				
				for(int x=0; x<HEIGHT_MAP_WIDTH; x++) {
					CVector vCenter = coord[nCoord].GetPositionVector();
					CVector vNorth = coord[nCoord-BORDER_MAP_WIDTH].GetPositionVector() - vCenter;
					CVector vWest = coord[nCoord-1].GetPositionVector() - vCenter;
					CVector vSouth = coord[nCoord+BORDER_MAP_WIDTH].GetPositionVector() - vCenter;
					CVector vEast = coord[nCoord+1].GetPositionVector() - vCenter;
					vNormal[nIndex] = (vNorth ^ vWest) + (vSouth ^ vEast);
					vNormal[nIndex].Normalize();
					nCoord++;
					nIndex++;
				}
			}
		}
};

extern CNodeBuilder builder;

/*
 */
class CPlanetaryVertex {

	public:
		CVector m_vPosition;
		CVector m_vNormal;
		float m_fTexCoord[2];

	public:
		
		/*
		 */
		CPlanetaryVertex() { }
		CPlanetaryVertex(const CPlanetaryVertex &v) { *this = v; }

		/*
		 */
		void operator=(const CPlanetaryVertex &v) {
			m_vPosition = v.m_vPosition;
			m_vNormal = v.m_vNormal;
			m_fTexCoord[0] = v.m_fTexCoord[0];
			m_fTexCoord[1] = v.m_fTexCoord[1];
		}

		/*
		 */
		void Init(CVector &v) {
			m_vPosition = v;
			m_vNormal = v;
			m_fTexCoord[0] = m_fTexCoord[1] = 0;
		}
};

/*
 */
class CPlanetaryMapNode {

	protected:

		static unsigned short m_nQuadrantOffset[4];
		static unsigned short m_nQuadrantHeightOffset[4];
		static unsigned short m_nFaceNeighbor[6][4];
		static SBufferShape m_shape;

		CPlanetaryMapNode *m_pParent;
		CPlanetaryMapNode *m_pChild[4];
		CPlanetaryMapNode *GetParent() { return m_pParent; }
		void SetParent(CPlanetaryMapNode *p) { m_pParent = p; }
		CPlanetaryMapNode *GetChild(int n) { return m_pChild[n]; }
		void SetChild(int n, CPlanetaryMapNode *p) { m_pChild[n] = p; }

		CGLVertexBufferObject m_boVertex;
		CPlanetaryVertex m_vNode[SURFACE_MAP_COUNT];
		CPixelBuffer m_pbHeightMap;

		unsigned int m_nNodeFlags;				
		CPlanetaryMap *m_pMap;					
		CPlanetaryMapNode *m_pNeighbor[4];		
		unsigned char m_nSort[4];				
		float m_fCorner[4];						

		unsigned short m_nTextureID;			
		unsigned short m_nBumpID;				

	public:

		/*
		 */
		static void InitTables();

		/*
		 */
		CPlanetaryMapNode(CPlanetaryMap *pMap, int nFace);
		CPlanetaryMapNode(CPlanetaryMapNode *pParent, int nQuadrant);
		~CPlanetaryMapNode();

		/*
		 */
		void InitNode(int nFace, int nQuadrant);
		void SetNeighbors(CPlanetaryMapNode *pTop, CPlanetaryMapNode *pRight, CPlanetaryMapNode *pBottom, CPlanetaryMapNode *pLeft) {
			m_pNeighbor[TopEdge] = pTop;
			m_pNeighbor[RightEdge] = pRight;
			m_pNeighbor[BottomEdge] = pBottom;
			m_pNeighbor[LeftEdge] = pLeft;
		}

		/*
		 */
		CPlanetaryMap *GetPlanetaryMap()				{ return m_pMap; }
		short GetFace()									{ return m_nNodeFlags & FaceMask; }
		int GetQuadrant()								{ return (m_nNodeFlags & QuadrantMask) >> 3; }
		int GetLevel()									{ return (m_nNodeFlags & LevelMask) >> 5; }
		bool IsDirty()									{ return (m_nNodeFlags & NodeDirty) != 0; }
		bool IsCameraIn(int n)							{ return (m_nNodeFlags & (1 << (10+n))) != 0; }
		bool IsBeyondHorizon(int n)						{ return (m_nNodeFlags & (1 << (16+n))) != 0; }
		bool IsOutsideFrustum(int n)					{ return (m_nNodeFlags & (1 << (20+n))) != 0; }
		bool HasTexture()								{ return m_nTextureID != (unsigned short)-1; }

		float GetMinX()									{ return m_fCorner[0]; }
		float GetMinY()									{ return m_fCorner[1]; }
		float GetMaxX()									{ return m_fCorner[2]; }
		float GetMaxY()									{ return m_fCorner[3]; }
		float GetMidX()									{ return (m_fCorner[2]+m_fCorner[0]) * 0.5f; }
		float GetMidY()									{ return (m_fCorner[3]+m_fCorner[1]) * 0.5f; }
		float GetWidth()								{ return m_fCorner[2] - m_fCorner[0]; }
		float GetHeight()								{ return m_fCorner[3] - m_fCorner[1]; }

		unsigned short GetIndex(int x, int y)			{ return y * SURFACE_MAP_WIDTH + x; }
		unsigned short GetHeightIndex(int x, int y)		{ return y * HEIGHT_MAP_WIDTH + x; }
		CPlanetaryVertex *GetVertex(int nIndex)			{ return &m_vNode[nIndex]; }
		CVector GetDirectionVector(float x, float y)	{ return ::GetPlanetaryVector(GetFace(), x, y, 1.0f); }
		bool IsLeaf()									{ return (m_pChild[0] == NULL && m_pChild[1] == NULL && m_pChild[2] == NULL && m_pChild[3] == NULL); }

		/*
		 */
		bool AffectsNode(const CPlanetaryMapCoord &qtc, float fRadius) {
			if(qtc.GetFace() == GetFace()) {
				if(qtc.x + fRadius > m_fCorner[0] && qtc.x - fRadius < m_fCorner[2] && qtc.y + fRadius > m_fCorner[1] && qtc.y - fRadius < m_fCorner[3])
					return true;
			}
			return false;
		}

		/*
		 */
		bool HitTest(const CPlanetaryMapCoord &qtc, unsigned char nQuadrant=-1) {
			if(qtc.GetFace() != GetFace()) return false;
			
			switch (nQuadrant) {
				case TopLeft:
				{
					return (qtc.x >= m_fCorner[0] && qtc.x <= MathLib::Avg(m_fCorner[0], m_fCorner[2]) &&
						qtc.y >= m_fCorner[1] && qtc.y <= MathLib::Avg(m_fCorner[1], m_fCorner[3]));
				}
				case TopRight:
				{
					return (qtc.x >= MathLib::Avg(m_fCorner[0], m_fCorner[2]) && qtc.x <= m_fCorner[2] &&
						qtc.y >= m_fCorner[1] && qtc.y <= MathLib::Avg(m_fCorner[1], m_fCorner[3]));
				}
				case BottomLeft:
				{
					return (qtc.x >= m_fCorner[0] && qtc.x <= MathLib::Avg(m_fCorner[0], m_fCorner[2]) &&
						qtc.y >= MathLib::Avg(m_fCorner[1], m_fCorner[3]) && qtc.y <= m_fCorner[3]);

				}
				case BottomRight:
				{
					return (qtc.x >= MathLib::Avg(m_fCorner[0], m_fCorner[2]) && qtc.x <= m_fCorner[2] &&
						qtc.y >= MathLib::Avg(m_fCorner[1], m_fCorner[3]) && qtc.y <= m_fCorner[3]);
				}
			}
			
			return (qtc.x >= m_fCorner[0] && qtc.x <= m_fCorner[2] &&
					qtc.y >= m_fCorner[1] && qtc.y <= m_fCorner[3]);
		}

		/*
		 */
		float GetHeight(const CPlanetaryMapCoord &qtc) {
			int nQuadrant = -1;
			float fMidX = MathLib::Avg(m_fCorner[0], m_fCorner[2]);
			float fMidY = MathLib::Avg(m_fCorner[1], m_fCorner[3]);

			if(qtc.x <= fMidX && qtc.y <= fMidY) nQuadrant = TopLeft;
			else if(qtc.x >= fMidX && qtc.y <= fMidY) nQuadrant = TopRight;
			else if(qtc.x <= fMidX && qtc.y >= fMidY) nQuadrant = BottomLeft;
			else if(qtc.x >= fMidX && qtc.y >= fMidY) nQuadrant = BottomRight;

			if(m_pChild[nQuadrant]) return m_pChild[nQuadrant]->GetHeight(qtc);

			return GetHeightFromMap(qtc);
		}

		/*
		 */
		float GetHeightFromMap(const CPlanetaryMapCoord &qtc) {	
			return GetHeightFromMap(qtc.x, qtc.y);
		}

		/*
		 */
		float GetHeightFromMap(float x, float y) {	
			float fHeight;
			m_pbHeightMap.Interpolate(&fHeight, (x - m_fCorner[0]) / (m_fCorner[2] - m_fCorner[0]), (y - m_fCorner[1]) / (m_fCorner[3] - m_fCorner[1]));
			return fHeight;
		}

		/*
		 */
		CPlanetaryMapCoord GetNearestCoord(const CPlanetaryMapCoord &qtc, unsigned char nQuadrant=-1) {

			float x, y;
			short nFace = GetFace();
			CPlanetaryMapCoord qtcTemp(nFace, qtc.GetDirectionVector());

			switch(nQuadrant) {
				case TopLeft:
				{
					x = MathLib::Max(m_fCorner[0], MathLib::Min(MathLib::Avg(m_fCorner[0], m_fCorner[2]), qtcTemp.x));
					y = MathLib::Max(m_fCorner[1], MathLib::Min(MathLib::Avg(m_fCorner[1], m_fCorner[3]), qtcTemp.y));
					break;
				}
				case TopRight:
				{
					x = MathLib::Max(MathLib::Avg(m_fCorner[0], m_fCorner[2]), MathLib::Min(m_fCorner[2], qtcTemp.x));
					y = MathLib::Max(m_fCorner[1], MathLib::Min(MathLib::Avg(m_fCorner[1], m_fCorner[3]), qtcTemp.y));
					break;
				}
				case BottomLeft:
				{
					x = MathLib::Max(m_fCorner[0], MathLib::Min(MathLib::Avg(m_fCorner[0], m_fCorner[2]), qtcTemp.x));
					y = MathLib::Max(MathLib::Avg(m_fCorner[1], m_fCorner[3]), MathLib::Min(m_fCorner[3], qtcTemp.y));
					break;
				}
				case BottomRight:
				{
					x = MathLib::Max(MathLib::Avg(m_fCorner[0], m_fCorner[2]), MathLib::Min(m_fCorner[2], qtcTemp.x));
					y = MathLib::Max(MathLib::Avg(m_fCorner[1], m_fCorner[3]), MathLib::Min(m_fCorner[3], qtcTemp.y));
					break;
				}
				default:
				{
					x = MathLib::Max(m_fCorner[0], MathLib::Min(m_fCorner[2], qtcTemp.x));
					y = MathLib::Max(m_fCorner[1], MathLib::Min(m_fCorner[3], qtcTemp.y));
					break;
				}
			}
			return CPlanetaryMapCoord(nFace, x, y);
		}

		/*
		 */
		CPlanetaryMapNode *FindQuadrantNeighbor(int nQuadrant, int nEdge, bool bForceSplit=false);

		/*
		 */
		CPlanetaryMapNode *FindNeighbor(int nEdge, bool bForceSplit=false) {
			if(!m_pParent) return m_pNeighbor[nEdge];
			return m_pParent->FindQuadrantNeighbor(GetQuadrant(), nEdge, bForceSplit);
		}

		/*
		 */
		void UpdateNeighbors() {
			for(int i=0; i<4; i++) m_pNeighbor[i] = FindNeighbor(i);
		}

		/*
		 */
		int FindMatchingEdge(int nEdge) {
			for(int i=0; i<4; i++) {
				if(m_pNeighbor[nEdge] && m_pNeighbor[nEdge]->m_pNeighbor[i] == this) return i;
			}
			return -1;
		}

		/* Methods for updating quad tree
		 */
		void Split(int nQuadrant);
		bool CanMerge();
		bool Merge(int nQuadrant);
		void UpdateSurface();
		int DrawSurface();
};

#endif /* __PLANETARY_MAP_NODE_H__ */
