#ifndef __PLANETARY_MAP_H__
#define __PLANETARY_MAP_H__

#include "PlanetaryObject.h"
#include "PlanetaryMapCoord.h"

/*
 */
class CPlanetaryMap : public CSRTTransform {

	protected:
		std::list<CPlanetaryFactory::Ref> m_llFactories;	// List of Factories
		CGLShaderObject m_shNoAtmosphere;
		CGLShaderObject m_shGroundFromSpace;
		CGLShaderObject m_shGroundFromAtmosphere;
		CGLShaderObject m_shSkyFromSpace;
		CGLShaderObject m_shSkyFromAtmosphere;

		float m_fRadius;
		float m_fFrustumRadius;
		float m_fMaxHeight;
		float m_fAtmosphereRadius;
		float m_fWavelength[3];
		float m_fEsun;
		float m_fKr;
		float m_fKm;
		float m_fG;
	
		CPlanetaryMapNode *m_pRoot[6];
		unsigned char m_nOrder[6];
		CTextureArray m_texArray;

		float m_fSplitPower;
		float m_fSplitFactor;
		int m_nSplits;
		int m_nMaxSplits;

		CPlanetaryMapCoord m_coordCamera;
		CVector m_vLightPos;
		CVector m_vCamera;
		CQuaternion m_qCamera;
		float m_fHorizon;
		CFrustum m_frustum;

		int m_nMaxDepth;
		int m_nNodes;
		int m_nTriangles;
		int m_nObjects;

	public:

		/*
		 */
		CPlanetaryMap() {
			m_nMaxSplits = 50;
			for(int nFace=0; nFace<6; nFace++) m_pRoot[nFace] = NULL;
		}

		/*
		 */
		CPlanetaryMap(const CPlanetaryMap &) {
			m_nMaxSplits = 50;
			for(int nFace=0; nFace<6; nFace++) m_pRoot[nFace] = NULL;
		}
		
		/*
		 */
		~CPlanetaryMap() {
			Cleanup();
		}

		/*
		 */
		void AddObject(CPlanetaryObject *pObj, bool bGlobal=true);
		void Init(CPropertySet &prop);
		void Cleanup();
		void UpdateSurface(CVector vLightPos, CVector vCamera, CQuaternion qCamera, float fSplitPower, float fSplitFactor);
		void DrawSurface();

		/*
		 */
		float GetRadius()				{ return m_fRadius; }
		float GetFrustumRadius()		{ return m_fFrustumRadius; }
		float GetMaxHeight()			{ return m_fMaxHeight; }
		float GetSplitPower()			{ return m_fSplitPower; }
		float GetSplitFactor()			{ return m_fSplitFactor; }
		int GetMaxSplits()				{ return m_nMaxSplits; }

		/*
		 */
		CTextureArray &GetTextureArray()			{ return m_texArray; }
		const CPlanetaryMapCoord &GetCameraCoord()	{ return m_coordCamera; }
		const CVector &GetCameraPosition()			{ return m_vCamera; }
		const CQuaternion &GetCameraOrientation()	{ return m_qCamera; }
		float GetHorizonDistance()					{ return m_fHorizon; }
		const CFrustum &GetViewFrustum()			{ return m_frustum; }

		/*
		 */
		int GetTriangleCount()	{ return m_nTriangles; }
		int GetObjectCount()	{ return m_nObjects; }
		int GetNodeCount()		{ return m_nNodes; }
		int GetMaxDepth()		{ return m_nMaxDepth; }
		int GetSplitCount()		{ return m_nSplits; }

		/*
		 */
		void UpdateMaxDepth(int n)	{ m_nMaxDepth = MathLib::Max(m_nMaxDepth, n); }
		void IncrementNodeCount()	{ m_nNodes++; }
		bool CanSplit() {
			if(m_nSplits >= m_nMaxSplits) return false;
			m_nSplits++;
			return true;
		}

		bool HasAtmosphere()					{ return m_fAtmosphereRadius > 0.0f; }
		bool IsInAtmosphere()					{ return m_coordCamera.GetHeight() < m_fAtmosphereRadius; }
		float GetHeightAboveGround(const CVector &v);

		/*
		 */
		typedef std::list<CPlanetaryFactory::Ref>::iterator iterator;
		iterator begin()	{ return m_llFactories.begin(); }
		iterator end()		{ return m_llFactories.end(); }

		/*
		 */
		void InitScatteringShader(CGLShaderObject *pShader) {
			CVector vLightDirection = m_vLightPos;
			vLightDirection.Normalize();

			float fRayleighScaleDepth = 0.25f;
			pShader->SetUniformParameter3f("v3CameraPos", m_vCamera.x, m_vCamera.y, m_vCamera.z);
			pShader->SetUniformParameter3f("v3LightPos", vLightDirection.x, vLightDirection.y, vLightDirection.z);
			pShader->SetUniformParameter3f("v3InvWavelength", 1/powf(m_fWavelength[0], 4.0f), 1/powf(m_fWavelength[1], 4.0f), 1/powf(m_fWavelength[2], 4.0f));
			pShader->SetUniformParameter1f("fCameraHeight", m_vCamera.Magnitude());
			pShader->SetUniformParameter1f("fCameraHeight2", m_vCamera.MagnitudeSquared());
			pShader->SetUniformParameter1f("fInnerRadius", m_fRadius);
			pShader->SetUniformParameter1f("fInnerRadius2", m_fRadius*m_fRadius);
			pShader->SetUniformParameter1f("fOuterRadius", m_fAtmosphereRadius);
			pShader->SetUniformParameter1f("fOuterRadius2", m_fAtmosphereRadius*m_fAtmosphereRadius);
			pShader->SetUniformParameter1f("fKrESun", m_fKr*m_fEsun);
			pShader->SetUniformParameter1f("fKmESun", m_fKm*m_fEsun);
			pShader->SetUniformParameter1f("fKr4PI", m_fKr*4.0f*PI);
			pShader->SetUniformParameter1f("fKm4PI", m_fKm*4.0f*PI);
			pShader->SetUniformParameter1f("fScale", 1.0f / (m_fAtmosphereRadius - m_fRadius));
			pShader->SetUniformParameter1f("fScaleDepth", fRayleighScaleDepth);
			pShader->SetUniformParameter1f("fScaleOverScaleDepth", (1.0f / (m_fAtmosphereRadius - m_fRadius)) / fRayleighScaleDepth);
			pShader->SetUniformParameter1f("g", m_fG);
			pShader->SetUniformParameter1f("g2", m_fG*m_fG);
			pShader->SetUniformParameter1i("nSamples", 2);
			pShader->SetUniformParameter1f("fSamples", 2);
			pShader->SetUniformParameter1i("s2Tex", 0);
		}
};

#endif
