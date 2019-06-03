#include "Engine.h"
#include "MixedHeightMapFactory.h"

/*
 */
void CMixedHeightMapFactory::Init(CPropertySet &prop) {

	CSimpleHeightMapFactory::Init(prop);
	m_nBufferSize = prop.GetIntProperty("buffer.size", 128);

	for(int nFace=0; nFace<6; nFace++) {
		
		m_pb[nFace].Init(m_nBufferSize, m_nBufferSize, 1, 1, GL_ALPHA, GL_FLOAT);
		float *pIndex = (float *)m_pb[nFace].GetBuffer();
		
		for(int y=0; y<m_nBufferSize; y++) {
			for(int x=0; x<m_nBufferSize; x++) {

				CPlanetaryMapCoord coord(nFace, (float)x / (m_nBufferSize-1), (float)y / (m_nBufferSize-1));
				CVector v = coord.GetDirectionVector();

				v *= 2;
				float fValue = Noise(v) + -0.1f;
				for(int i=1; i<5; i++) {
					v *= m_fLacunarity;
					fValue += (Noise(v) + -0.1f) * m_fExponent[i];
				}

				while(MathLib::Abs(fValue) > 1.0f) {
					if(fValue > 0.0f) fValue = 2 - fValue;
					else fValue = -2 - fValue;
				}

				*pIndex = fValue;
				pIndex++;

			}
		}
	}
}

/*
 */
void CMixedHeightMapFactory::BuildNode(CPlanetaryMapNode *pNode) {

	PROFILE("CMixedHeightMapFactory::BuildNode", 3);

	for(int nIndex=0; nIndex<BORDER_MAP_COUNT; nIndex++) {
		float fHeight = GetHeight(builder.coord[nIndex]);
		fHeight = pNode->GetPlanetaryMap()->GetRadius() + fHeight * pNode->GetPlanetaryMap()->GetMaxHeight();
		builder.coord[nIndex].SetHeight(fHeight);
	}
}

/*
 */
float CMixedHeightMapFactory::GetHeight(const CPlanetaryMapCoord &coord) {

	float fBase, fExtra;
	m_pb[coord.GetFace()].Interpolate(&fBase, coord.x, coord.y);

	if(fBase > 0.0f) {

		CVector v = coord.GetDirectionVector();
		v *= powf(m_fLacunarity, 6);

		fExtra = MathLib::Abs(Noise(v)) * m_fExponent[6];

		for(int i=6; i<12; i++) {
			v *= m_fLacunarity;
			fExtra += MathLib::Abs(Noise(v)) * m_fExponent[i];
		}

		fBase = fBase - (1-MathLib::Square(1-fExtra)) * sqrtf(MathLib::Abs(fBase));
	}

	return fBase;
}
