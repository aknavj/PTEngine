#include "Engine.h"
#include "PlanetaryMapNode.h"

CNodeBuilder builder;
SBufferShape CPlanetaryMapNode::m_shape;

unsigned short CPlanetaryMapNode::m_nQuadrantOffset[4] = {0, 4, 36, 40};
unsigned short CPlanetaryMapNode::m_nQuadrantHeightOffset[4] = {0, 8, 136, 144};

unsigned short CPlanetaryMapNode::m_nFaceNeighbor[6][4];

/* Helper class
 */
class CInitPlanetaryMapNode {

	public:
		CInitPlanetaryMapNode()
		{
			CPlanetaryMapNode::InitTables();
		}
};
CInitPlanetaryMapNode g_cInitPlanetaryMapNode;

/*
 */
void CPlanetaryMapNode::InitTables() {

	int nTexCoord[4] = {2, 0, 0, 0};
	m_shape.Init(true, true, false, nTexCoord);

	m_nQuadrantOffset[TopLeft] = 0;
	m_nQuadrantOffset[TopRight] = (SURFACE_MAP_WIDTH-1) / 2;
	m_nQuadrantOffset[BottomRight] = (SURFACE_MAP_COUNT-1) / 2;
	m_nQuadrantOffset[BottomLeft] = m_nQuadrantOffset[BottomRight] - m_nQuadrantOffset[TopRight];

	m_nQuadrantHeightOffset[TopLeft] = 0;
	m_nQuadrantHeightOffset[TopRight] = (HEIGHT_MAP_WIDTH-1) / 2;
	m_nQuadrantHeightOffset[BottomRight] = (HEIGHT_MAP_COUNT-1) / 2;
	m_nQuadrantHeightOffset[BottomLeft] = m_nQuadrantHeightOffset[BottomRight] - m_nQuadrantHeightOffset[TopRight];

	m_nFaceNeighbor[FrontFace][TopEdge] = TopFace;
	m_nFaceNeighbor[FrontFace][RightEdge] = RightFace;
	m_nFaceNeighbor[FrontFace][BottomEdge] = BottomFace;
	m_nFaceNeighbor[FrontFace][LeftEdge] = LeftFace;

	m_nFaceNeighbor[BackFace][TopEdge] = TopFace;
	m_nFaceNeighbor[BackFace][RightEdge] = LeftFace;
	m_nFaceNeighbor[BackFace][BottomEdge] = BackFace;
	m_nFaceNeighbor[BackFace][LeftEdge] = RightFace;

	m_nFaceNeighbor[RightFace][TopEdge] = TopFace;
	m_nFaceNeighbor[RightFace][RightEdge] = BackFace;
	m_nFaceNeighbor[RightFace][BottomEdge] = BottomFace;
	m_nFaceNeighbor[RightFace][LeftEdge] = FrontFace;
	
	m_nFaceNeighbor[LeftFace][TopEdge] = TopFace;
	m_nFaceNeighbor[LeftFace][RightEdge] = FrontFace;
	m_nFaceNeighbor[LeftFace][BottomEdge] = BottomFace;
	m_nFaceNeighbor[LeftFace][LeftEdge] = BackFace;

	m_nFaceNeighbor[TopFace][TopEdge] = BackFace;
	m_nFaceNeighbor[TopFace][RightEdge] = RightFace;
	m_nFaceNeighbor[TopFace][BottomEdge] = FrontFace;
	m_nFaceNeighbor[TopFace][LeftEdge] = LeftFace;

	m_nFaceNeighbor[BottomFace][TopEdge] = FrontFace;
	m_nFaceNeighbor[BottomFace][RightEdge] = RightFace;
	m_nFaceNeighbor[BottomFace][BottomEdge] = BackFace;
	m_nFaceNeighbor[BottomFace][LeftEdge] = LeftFace;
}

/*
 */
CPlanetaryMapNode::CPlanetaryMapNode(CPlanetaryMap *pMap, int nFace) {
	m_pParent = NULL;
	m_pChild[0] = m_pChild[1] = m_pChild[2] = m_pChild[3] = NULL;
	m_pMap = pMap;
	InitNode(nFace, -1);
}

/*
 */
CPlanetaryMapNode::CPlanetaryMapNode(CPlanetaryMapNode *pParent, int nQuadrant) {
	m_pParent = pParent;
	m_pChild[0] = m_pChild[1] = m_pChild[2] = m_pChild[3] = NULL;
	m_pMap = pParent->m_pMap;
	InitNode(pParent->GetFace(), nQuadrant);
}

/*
 */
CPlanetaryMapNode::~CPlanetaryMapNode() {
	if(m_nBumpID != (unsigned short)-1) m_pMap->GetTextureArray().ReleaseTexture(m_nBumpID);
	if(HasTexture()) m_pMap->GetTextureArray().ReleaseTexture(m_nTextureID);

	m_boVertex.Cleanup();

	if(m_pParent) {
		m_pParent->m_pChild[GetQuadrant()] = NULL;
		
		for(int i=0; i<4; i++) {
			if(m_pNeighbor[i]) {
				for(int j=0; j<4; j++)
					if(m_pNeighbor[i]->m_pNeighbor[j] == this)
						m_pNeighbor[i]->m_pNeighbor[j] = NULL;
			}
		}
	}

	for(CPlanetaryMap::iterator it = m_pMap->begin(); it != m_pMap->end(); it++) {
		if((*it)->AffectsNode(this))
			(*it)->DestroyNode(this);
	}
}

/*
 */
void CPlanetaryMapNode::InitNode(int nFace, int nQuadrant) {
	m_nTextureID = (unsigned short)-1;
	m_nBumpID = (unsigned short)-1;

	int nLevel = m_pParent ? m_pParent->GetLevel() + 1 : 0;
	m_nNodeFlags = NodeDirty | ((nLevel << 5) & LevelMask) | ((nQuadrant << 3) & QuadrantMask) | (nFace & FaceMask);
	m_nSort[0] = 0; m_nSort[1] = 1; m_nSort[2] = 2; m_nSort[3] = 3;

	if(m_pParent) {

		m_pParent->m_pChild[nQuadrant] = this;
		UpdateNeighbors();

		for(int i=0; i<4; i++) {
			if(m_pNeighbor[i])
				m_pNeighbor[i]->UpdateNeighbors();
		}
	}

	switch(nQuadrant) {
		case TopLeft:
		{
			m_fCorner[0] = m_pParent->m_fCorner[0];
			m_fCorner[1] = m_pParent->m_fCorner[1];
			m_fCorner[2] = m_pParent->m_fCorner[0] + (m_pParent->m_fCorner[2] - m_pParent->m_fCorner[0]) * 0.5f;
			m_fCorner[3] = m_pParent->m_fCorner[1] + (m_pParent->m_fCorner[3] - m_pParent->m_fCorner[1]) * 0.5f;
			break;
		}
		case TopRight:
		{
			m_fCorner[0] = m_pParent->m_fCorner[0] + (m_pParent->m_fCorner[2] - m_pParent->m_fCorner[0]) * 0.5f;
			m_fCorner[1] = m_pParent->m_fCorner[1];
			m_fCorner[2] = m_pParent->m_fCorner[2];
			m_fCorner[3] = m_pParent->m_fCorner[1] + (m_pParent->m_fCorner[3] - m_pParent->m_fCorner[1]) * 0.5f;
			break;
		}
		case BottomLeft:
		{
			m_fCorner[0] = m_pParent->m_fCorner[0];
			m_fCorner[1] = m_pParent->m_fCorner[1] + (m_pParent->m_fCorner[3] - m_pParent->m_fCorner[1]) * 0.5f;
			m_fCorner[2] = m_pParent->m_fCorner[0] + (m_pParent->m_fCorner[2] - m_pParent->m_fCorner[0]) * 0.5f;
			m_fCorner[3] = m_pParent->m_fCorner[3];
			break;
		}
		case BottomRight:
		{
			m_fCorner[0] = m_pParent->m_fCorner[0] + (m_pParent->m_fCorner[2] - m_pParent->m_fCorner[0]) * 0.5f;
			m_fCorner[1] = m_pParent->m_fCorner[1] + (m_pParent->m_fCorner[3] - m_pParent->m_fCorner[1]) * 0.5f;
			m_fCorner[2] = m_pParent->m_fCorner[2];
			m_fCorner[3] = m_pParent->m_fCorner[3];
			break;
		}
		default:
		{
			m_fCorner[0] = 0.0f;
			m_fCorner[1] = 0.0f;
			m_fCorner[2] = 1.0f;
			m_fCorner[3] = 1.0f;
			break;
		}
	}

	int x, y, nIndex = 0;
	float fXOffset = (m_fCorner[2] - m_fCorner[0]) / (HEIGHT_MAP_WIDTH-1);
	float fYOffset = (m_fCorner[3] - m_fCorner[1]) / (HEIGHT_MAP_WIDTH-1);
	float fY = m_fCorner[1] - fYOffset;

	for(y=0; y<BORDER_MAP_WIDTH; y++) {

		float fX = m_fCorner[0] - fXOffset;

		for(x=0; x<BORDER_MAP_WIDTH; x++) {
			builder.coord[nIndex++].Init(nFace, fX, fY, m_pMap->GetRadius());
			fX += fXOffset;
		}
		fY += fYOffset;
	}


	for(CPlanetaryMap::iterator it = m_pMap->begin(); it != m_pMap->end(); it++) {
		if((*it)->AffectsNode(this)) (*it)->BuildNode(this);
	}

	if(builder.pb.GetBuffer() != NULL) {
		m_nTextureID = m_pMap->GetTextureArray().LockTexture();
		m_pMap->GetTextureArray().Update(m_nTextureID, &builder.pb);
	}

	builder.ComputeNormals();

	m_nBumpID = m_pMap->GetTextureArray().LockTexture();
	unsigned char *pBuffer = (unsigned char *)builder.pb.GetBuffer();
	nIndex = 0;

	for(y=0; y<HEIGHT_MAP_WIDTH; y++) {
		for(x=0; x<HEIGHT_MAP_WIDTH; x++) {
			CVector vNormal = builder.vNormal[nIndex];
			vNormal *= 128.0f / vNormal.Magnitude();
			*pBuffer++ = (unsigned char)(vNormal.x + 128.0f);
			*pBuffer++ = (unsigned char)(vNormal.y + 128.0f);
			*pBuffer++ = (unsigned char)(vNormal.z + 128.0f);
			nIndex++;
		}
	}

	m_pMap->GetTextureArray().Update(m_nBumpID, &builder.pb);

	m_pbHeightMap.Init(SURFACE_MAP_WIDTH, SURFACE_MAP_WIDTH, 1, 1, GL_ALPHA, GL_FLOAT);
	float *pfHeightMap = (float *)m_pbHeightMap.GetBuffer();
	nIndex = 0;
	
	for(y=0; y<SURFACE_MAP_WIDTH; y++) {
		int nCoord = (y*HEIGHT_MAP_SCALE+1) * BORDER_MAP_WIDTH + 1;
		int nNormal = y*HEIGHT_MAP_SCALE * HEIGHT_MAP_WIDTH;
		
		for(x=0; x<SURFACE_MAP_WIDTH; x++) {
			*pfHeightMap++ = builder.coord[nCoord].GetHeight();
			CPlanetaryVertex *pVertex = &m_vNode[nIndex++];
			pVertex->m_vPosition = builder.coord[nCoord].GetPositionVector();
			pVertex->m_vNormal = builder.vNormal[nNormal];
			pVertex->m_fTexCoord[0] = builder.coord[nCoord].x;
			pVertex->m_fTexCoord[1] = builder.coord[nCoord].y;
			nCoord += HEIGHT_MAP_SCALE;
			nNormal += HEIGHT_MAP_SCALE;
		}
	}

	m_boVertex.Init(&m_shape, SURFACE_MAP_COUNT, m_vNode);
}

/*
 */
CPlanetaryMapNode *CPlanetaryMapNode::FindQuadrantNeighbor(int nQuadrant, int nEdge, bool bForceSplit)
{
	static const unsigned char nTopLeftTopEdge[6] = { BottomRight, TopLeft, TopRight, BottomLeft, BottomLeft, TopRight };
	static const unsigned char nTopLeftLeftEdge[6] = { TopRight, TopRight, TopLeft, BottomRight, TopRight, TopRight };
	static const unsigned char nTopRightTopEdge[6] = { TopRight, BottomLeft, TopLeft, BottomRight, BottomRight, TopLeft };
	static const unsigned char nTopRightRightEdge[6] = { TopLeft, TopLeft, TopRight, BottomLeft, TopLeft, TopLeft };
	static const unsigned char nBottomLeftBottomEdge[6] = { TopRight, BottomLeft, TopLeft, BottomRight, TopLeft, BottomRight };
	static const unsigned char nBottomLeftLeftEdge[6] = { BottomRight, BottomRight, TopRight, BottomLeft, BottomRight, BottomRight };
	static const unsigned char nBottomRightBottomEdge[6] = { BottomRight, TopLeft, TopRight, BottomLeft, TopRight, BottomLeft };
	static const unsigned char nBottomRightRightEdge[6] = { BottomLeft, BottomLeft, TopLeft, BottomRight, BottomLeft, BottomLeft };

	CPlanetaryMapNode *pNeighbor = NULL;
	int nChild;
	switch(nQuadrant)
	{
		case TopLeft:
			switch(nEdge)
			{
				case TopEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? BottomLeft : nTopLeftTopEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
				case RightEdge:
					pNeighbor = m_pChild[TopRight];
					break;
				case BottomEdge:
					pNeighbor = m_pChild[BottomLeft];
					break;
				case LeftEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? TopRight : nTopLeftLeftEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
			}
			break;
		case TopRight:
			switch(nEdge)
			{
				case TopEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? BottomRight : nTopRightTopEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
				case RightEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? TopLeft : nTopRightRightEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
				case BottomEdge:
					pNeighbor = m_pChild[BottomRight];
					break;
				case LeftEdge:
					pNeighbor = m_pChild[TopLeft];
					break;
			}
			break;
		case BottomLeft:
			switch(nEdge)
			{
				case TopEdge:
					pNeighbor = m_pChild[TopLeft];
					break;
				case RightEdge:
					pNeighbor = m_pChild[BottomRight];
					break;
				case BottomEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? TopLeft : nBottomLeftBottomEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
				case LeftEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? BottomRight : nBottomLeftLeftEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
			}
			break;
		case BottomRight:
			switch(nEdge)
			{
				case TopEdge:
					pNeighbor = m_pChild[TopRight];
					break;
				case RightEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? BottomLeft : nBottomRightRightEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
				case BottomEdge:
					if(m_pNeighbor[nEdge])
					{
						nChild = (m_pNeighbor[nEdge]->GetFace() == GetFace()) ? TopRight : nBottomRightBottomEdge[GetFace()];
						if(bForceSplit)
							m_pNeighbor[nEdge]->Split(nChild);
						pNeighbor = m_pNeighbor[nEdge]->m_pChild[nChild];
					}
					break;
				case LeftEdge:
					pNeighbor = m_pChild[BottomLeft];
					break;
			}
			break;
	}
	
	return pNeighbor;
}

/*
 */
void CPlanetaryMapNode::Split(int nQuadrant) {

	if(m_pChild[nQuadrant]) return;

	switch (nQuadrant) {
		case TopLeft:
		{
			if (!FindNeighbor(TopEdge, true) || !FindNeighbor(LeftEdge, true))
				return;
			break;
		}
		case TopRight:
		{
			if (!FindNeighbor(TopEdge, true) || !FindNeighbor(RightEdge, true))
				return;
			break;
		}
		case BottomLeft:
		{
			if (!FindNeighbor(BottomEdge, true) || !FindNeighbor(LeftEdge, true))
				return;
			break;
		}
		case BottomRight:
		{
			if (!FindNeighbor(BottomEdge, true) || !FindNeighbor(RightEdge, true))
				return;
			break;
		}
	}

	if(m_pMap->CanSplit())
		m_pChild[nQuadrant] = new CPlanetaryMapNode(this, nQuadrant);
}

/*
 */
bool CPlanetaryMapNode::CanMerge() {

	if(!IsLeaf()) return false;

	if(m_pNeighbor[TopEdge] && (FindQuadrantNeighbor(TopLeft, TopEdge) || FindQuadrantNeighbor(TopRight, TopEdge))) return false;
	if(m_pNeighbor[RightEdge] && (FindQuadrantNeighbor(TopRight, RightEdge) || FindQuadrantNeighbor(BottomRight, RightEdge))) return false;
	if(m_pNeighbor[BottomEdge] && (FindQuadrantNeighbor(BottomRight, BottomEdge) || FindQuadrantNeighbor(BottomLeft, BottomEdge))) return false;
	if(m_pNeighbor[LeftEdge] && (FindQuadrantNeighbor(BottomLeft, LeftEdge) || FindQuadrantNeighbor(TopLeft, LeftEdge))) return false;

	return true;
}

/*
 */
bool CPlanetaryMapNode::Merge(int nQuadrant) {

	CPlanetaryMapNode *pNode = m_pChild[nQuadrant];
	if(!pNode) return true;

	bool bSuccess = true;
	if(!pNode->Merge(0)) bSuccess = false;
	if(!pNode->Merge(1)) bSuccess = false;
	if(!pNode->Merge(2)) bSuccess = false;
	if(!pNode->Merge(3)) bSuccess = false;
	if(!bSuccess || !pNode->CanMerge()) return false;

	delete m_pChild[nQuadrant];

	return true;
}

/*
 */
void CPlanetaryMapNode::UpdateSurface() {

	CVector vCamera = m_pMap->GetCameraPosition();
	m_nNodeFlags &= ~(CameraInMask | BeyondHorizonMask | OutsideFrustumMask);

	short nFace = GetFace();
	float fMid[2] = {MathLib::Avg(m_fCorner[0], m_fCorner[2]), MathLib::Avg(m_fCorner[1], m_fCorner[3])};
	float x[2] = {MathLib::Avg(m_fCorner[0], fMid[0]), MathLib::Avg(m_fCorner[2], fMid[0])};
	float y[2] = {MathLib::Avg(m_fCorner[1], fMid[1]), MathLib::Avg(m_fCorner[3], fMid[1])};
	CPlanetaryMapCoord cQuadrant[4] = {
		CPlanetaryMapCoord(nFace, x[0], y[0]),
		CPlanetaryMapCoord(nFace, x[1], y[0]),
		CPlanetaryMapCoord(nFace, x[0], y[1]),
		CPlanetaryMapCoord(nFace, x[1], y[1])
	};

	CVector vQuadrant[4] = {
		cQuadrant[0].GetPositionVector(m_pMap->GetFrustumRadius()),
		cQuadrant[1].GetPositionVector(m_pMap->GetFrustumRadius()),
		cQuadrant[2].GetPositionVector(m_pMap->GetFrustumRadius()),
		cQuadrant[3].GetPositionVector(m_pMap->GetFrustumRadius()),
	};

	float fDistance[4] = {
		vCamera.Distance(vQuadrant[0]),
		vCamera.Distance(vQuadrant[1]),
		vCamera.Distance(vQuadrant[2]),
		vCamera.Distance(vQuadrant[3]),
	};

	float xCoord, yCoord;
	GetFaceCoordinates(nFace, vCamera, xCoord, yCoord);
	if(xCoord < fMid[0]) {
		if(yCoord < fMid[1]) {
			m_nSort[0] = 0;	
			m_nSort[1] = 1;
			m_nSort[2] = 2;	
			m_nSort[3] = 3;	
		} else {
			m_nSort[0] = 2;	
			m_nSort[1] = 0;
			m_nSort[2] = 3;	
			m_nSort[3] = 1;
		}
	} else {
		if(yCoord < fMid[1]) {
			m_nSort[0] = 1;
			m_nSort[1] = 0;
			m_nSort[2] = 3;
			m_nSort[3] = 2;
		} else {
			m_nSort[0] = 3;
			m_nSort[1] = 1;
			m_nSort[2] = 2;
			m_nSort[3] = 0;
		}
	}

	float fQuadrantSize = vQuadrant[TopLeft].Distance(vQuadrant[BottomRight] * (m_pMap->GetRadius()/m_pMap->GetFrustumRadius()));
	float fRadius = MathLib::Max(fQuadrantSize, m_pMap->GetMaxHeight());

	fQuadrantSize = (m_fCorner[2]-m_fCorner[0]) * m_pMap->GetRadius();
	float fPriority = (fQuadrantSize <= m_pMap->GetMaxHeight()*0.001f) ? 0.0f : m_pMap->GetSplitFactor() * powf(fQuadrantSize, m_pMap->GetSplitPower());

	for(int i=0; i<4; i++) {
		int nQuadrant = m_nSort[i];

		if(HitTest(m_pMap->GetCameraCoord(), nQuadrant)) m_nNodeFlags |= (nQuadrant << 10) & CameraInMask;
		else {
			if(m_pMap->GetHorizonDistance() > 0.0f && fDistance[nQuadrant]-fRadius > m_pMap->GetHorizonDistance()) {
				m_nNodeFlags |= 1 << (16+nQuadrant);
				Merge(nQuadrant);
				continue;
			}

			if(!m_pMap->GetViewFrustum().IsInFrustum(vQuadrant[nQuadrant], fRadius)) {
				m_nNodeFlags |= 1 << (20+nQuadrant);
				Merge(nQuadrant);
				continue;
			}
		}

		if(fDistance[nQuadrant] > fPriority) {
			Merge(nQuadrant);
			continue;
		}

		Split(nQuadrant);
		if(m_pChild[nQuadrant]) m_pChild[nQuadrant]->UpdateSurface();
	}
}

/*
 */
int CPlanetaryMapNode::DrawSurface() {

	m_pMap->UpdateMaxDepth(GetLevel());

	static const unsigned short nFan[10] = {20, 22, 12, 2, 10, 18, 28, 38, 30, 22};
	static const unsigned short nOther[4][6] = {{0, 10, 2, 2, 12, 4}, {4, 12, 22, 22, 30, 40}, {40, 30, 38, 38, 28, 36}, {36, 28, 18, 18, 10, 0}};
	static const unsigned short nEdge[4][2] = {{1, 3}, {13, 31}, {39, 37}, {27, 9}};
	static unsigned short nMesh[48];
	static unsigned short nMeshIndex;
	int i, n, nQuadrant;
	int nTriangles = 0;

	bool bUseTexture = HasTexture() && (!m_pChild[0] || !m_pChild[1] || !m_pChild[2] || !m_pChild[3]);
	if(bUseTexture) {
		CMatrix4 mTexture;
		glActiveTexture(GL_TEXTURE1);
		m_pMap->GetTextureArray().MapCorners(m_nBumpID, mTexture, m_fCorner[0], m_fCorner[1], m_fCorner[2], m_fCorner[3]);
		glPushMatrix();
		glLoadMatrixf(mTexture);

		glActiveTexture(GL_TEXTURE0);
		m_pMap->GetTextureArray().MapCorners(m_nTextureID, mTexture, m_fCorner[0], m_fCorner[1], m_fCorner[2], m_fCorner[3]);
		glPushMatrix();
		glLoadMatrixf(mTexture);
	}

	for(i=0; i<4; i++) {
		nQuadrant = m_nSort[i];
		if(IsBeyondHorizon(nQuadrant) || IsOutsideFrustum(nQuadrant)) continue;

		if(m_pChild[nQuadrant]) nTriangles += m_pChild[nQuadrant]->DrawSurface();
		else {
			m_pMap->IncrementNodeCount();

			m_boVertex.Enable(&m_shape);

			for(n=0; n<10; n++) nMesh[n] = nFan[n] + m_nQuadrantOffset[nQuadrant];

			glDrawElements(GL_TRIANGLE_FAN, 10, GL_UNSIGNED_SHORT, nMesh);
			nTriangles += 8;

			nMeshIndex = 0;
			for(n=0; n<4; n++) {

				bool bNeighborSplit = FindQuadrantNeighbor(nQuadrant, n) != NULL;

				nMesh[nMeshIndex++] = nOther[n][0] + m_nQuadrantOffset[nQuadrant];
				nMesh[nMeshIndex++] = nOther[n][1] + m_nQuadrantOffset[nQuadrant];
				if(bNeighborSplit) {
					nMesh[nMeshIndex++] = nEdge[n][0] + m_nQuadrantOffset[nQuadrant];
					nMesh[nMeshIndex++] = nEdge[n][0] + m_nQuadrantOffset[nQuadrant];
					nMesh[nMeshIndex++] = nOther[n][1] + m_nQuadrantOffset[nQuadrant];
				}
				nMesh[nMeshIndex++] = nOther[n][2] + m_nQuadrantOffset[nQuadrant];

				nMesh[nMeshIndex++] = nOther[n][3] + m_nQuadrantOffset[nQuadrant];
				nMesh[nMeshIndex++] = nOther[n][4] + m_nQuadrantOffset[nQuadrant];
				if(bNeighborSplit) {
					nMesh[nMeshIndex++] = nEdge[n][1] + m_nQuadrantOffset[nQuadrant];
					nMesh[nMeshIndex++] = nEdge[n][1] + m_nQuadrantOffset[nQuadrant];
					nMesh[nMeshIndex++] = nOther[n][4] + m_nQuadrantOffset[nQuadrant];
				}

				nMesh[nMeshIndex++] = nOther[n][5] + m_nQuadrantOffset[nQuadrant];
			}

			glDrawElements(GL_TRIANGLES, nMeshIndex, GL_UNSIGNED_SHORT, nMesh);
			nTriangles += nMeshIndex/3;
		}
	}

	if(bUseTexture) {
		glClientActiveTexture(GL_TEXTURE1);
		glActiveTexture(GL_TEXTURE1);
		glPopMatrix();
		glClientActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE0);
		glPopMatrix();
	}

	return nTriangles;
}
