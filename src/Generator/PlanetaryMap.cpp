#include "Engine.h"
#include "PlanetaryMap.h"
#include "PlanetaryMapNode.h"

/*
 */
void CPlanetaryMap::Init(CPropertySet &prop) {

	Cleanup();

	m_texArray.Init(1024, HEIGHT_MAP_WIDTH, 3, GL_RGB, GL_UNSIGNED_BYTE);
	m_shNoAtmosphere.Init("shaders/BumpVert.glsl", "shaders/BumpFrag.glsl");
	m_shGroundFromSpace.Init("shaders/BumpGroundFromSpaceVert.glsl", "shaders/BumpGroundFromSpaceFrag.glsl");
	m_shGroundFromAtmosphere.Init("shaders/BumpGroundFromAtmoVert.glsl", "shaders/BumpGroundFromAtmoFrag.glsl");
	m_shSkyFromSpace.Init("shaders/SkyFromSpaceVert.glsl", "shaders/SkyFromSpaceFrag.glsl");
	m_shSkyFromAtmosphere.Init("shaders/SkyFromAtmosphereVert.glsl", "shaders/SkyFromAtmosphereFrag.glsl");

	// Z�skej v�echny jednoduch� n�staven� pro planety
	sscanf(prop.GetProperty("position"), "%f, %f, %f", &m_vTranslate.x, &m_vTranslate.y, &m_vTranslate.z);
	m_fRadius = prop.GetFloatProperty("radius");
	m_fFrustumRadius = prop.GetFloatProperty("frustum.radius");
	m_fMaxHeight = prop.GetFloatProperty("max.height");
	m_fAtmosphereRadius = prop.GetFloatProperty("atmosphere.radius", 0.0f);

	if(m_fAtmosphereRadius > 0.0f)
	{
		sscanf(prop.GetProperty("atmosphere.wavelength"), "%f, %f, %f", &m_fWavelength[0], &m_fWavelength[1], &m_fWavelength[2]);
		m_fKr = prop.GetFloatProperty("atmosphere.Kr");
		m_fKm = prop.GetFloatProperty("atmosphere.Km");
		m_fEsun = prop.GetFloatProperty("atmosphere.Esun");
		m_fG = prop.GetFloatProperty("atmosphere.g");
	}

	std::string strPrefix = prop.GetPrefix();
	int i;
	for(i=1; ; i++) {
		char szPrefix[256];
		sprintf(szPrefix, "%sfactory.%d.", strPrefix.c_str(), i);
		prop.SetPrefix(szPrefix);
		const char *pszType = prop.GetProperty("type");
		if(!pszType)
			break;

		CPlanetaryFactory *pFactory = (CPlanetaryFactory *)CObjectType::CreateObject(pszType);
		m_llFactories.push_back(pFactory);
		pFactory->Init(prop);
	}

	prop.SetPrefix(strPrefix.c_str());

	int nFace;
	for(nFace=0; nFace<6; nFace++) m_pRoot[nFace] = new CPlanetaryMapNode(this, nFace);

	m_pRoot[RightFace]->SetNeighbors(m_pRoot[TopFace], m_pRoot[BackFace], m_pRoot[BottomFace], m_pRoot[FrontFace]);
	m_pRoot[LeftFace]->SetNeighbors(m_pRoot[TopFace], m_pRoot[FrontFace], m_pRoot[BottomFace], m_pRoot[BackFace]);
	m_pRoot[TopFace]->SetNeighbors(m_pRoot[BackFace], m_pRoot[RightFace], m_pRoot[FrontFace], m_pRoot[LeftFace]);
	m_pRoot[BottomFace]->SetNeighbors(m_pRoot[FrontFace], m_pRoot[RightFace], m_pRoot[BackFace], m_pRoot[LeftFace]);
	m_pRoot[FrontFace]->SetNeighbors(m_pRoot[TopFace], m_pRoot[RightFace], m_pRoot[BottomFace], m_pRoot[LeftFace]);
	m_pRoot[BackFace]->SetNeighbors(m_pRoot[TopFace], m_pRoot[LeftFace], m_pRoot[BottomFace], m_pRoot[RightFace]);
}

/*
 */
void CPlanetaryMap::Cleanup() {
	int nFace;

	for(nFace=0; nFace<6; nFace++) {
		if(m_pRoot[nFace])
			delete m_pRoot[nFace];
		m_pRoot[nFace] = NULL;
	}

	m_llFactories.clear();
}

/*
 */
void CPlanetaryMap::UpdateSurface(CVector vLightPos, CVector vCamera, CQuaternion qCamera, float fSplitPower, float fSplitFactor) {
	PROFILE("CPlanetaryMap::UpdateSurface", 2);

	CMatrix4 mView = BuildModelMatrix();
	glPushMatrix();
	glMultMatrixf(mView);
	m_frustum.Init();
	glPopMatrix();

	m_nSplits = 0;

	m_vLightPos = m_qRotate.UnitInverse().TransformVector(vLightPos - m_vTranslate);
	m_vCamera = m_qRotate.UnitInverse().TransformVector(vCamera - m_vTranslate);
	m_qCamera = m_qRotate.UnitInverse() * qCamera;
	float fAltitude = m_vCamera.Magnitude();
	float fHorizonAltitude = MathLib::Max(fAltitude-m_fRadius, m_fMaxHeight);
	m_fHorizon = sqrtf(fHorizonAltitude*fHorizonAltitude + 2.0f*fHorizonAltitude*m_fRadius);
	
	m_coordCamera.Init(m_vCamera, fAltitude);
	m_fSplitPower = fSplitPower;
	m_fSplitFactor = fSplitFactor;

	float fDistance[6];
	int nFace;
	for(nFace=0; nFace<6; nFace++) fDistance[nFace] = (m_pRoot[nFace]->HitTest(m_vCamera)) ? 0 : m_vCamera.DistanceSquared(m_pRoot[nFace]->GetVertex(40)->m_vPosition);

	m_nOrder[0] = 0; m_nOrder[1] = 1; m_nOrder[2] = 2; m_nOrder[3] = 3; m_nOrder[4] = 4; m_nOrder[5] = 5;

	for(int i=0; i<5; i++) {
		for(int j=0; j<5-i; j++) {
			if(fDistance[m_nOrder[j]] > fDistance[m_nOrder[j+1]]) Swap(m_nOrder[j], m_nOrder[j+1]);
		}
	}

	for(nFace=0; nFace<6; nFace++) m_pRoot[m_nOrder[nFace]]->UpdateSurface();

	for(std::list<CPlanetaryFactory::Ref>::iterator it = m_llFactories.begin(); it != m_llFactories.end(); it++) (*it)->Update();
}

/*
 */
void CPlanetaryMap::DrawSurface() {

	PROFILE("CPlanetaryMap::DrawSurface", 2);

	m_nObjects = 0;
	m_nTriangles = 0;
	m_nMaxDepth = 0;
	m_nNodes = 0;

	CMatrix4 mView = BuildModelMatrix();
	glPushMatrix();
	glMultMatrixf(mView);

	bool bUseTexture = m_pRoot[0]->HasTexture();
	if(bUseTexture) {
		m_texArray.Flush();
		glMatrixMode(GL_TEXTURE);
		m_texArray.Enable();
	}

	CGLShaderObject *pShader;
	if(IsInAtmosphere()) pShader = &m_shGroundFromAtmosphere;
	else if(HasAtmosphere()) pShader = &m_shGroundFromSpace;
	else pShader = &m_shNoAtmosphere;

	if(pShader->IsValid()) {
		pShader->Enable();
		InitScatteringShader(pShader);
	} else {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, CQuaternion(1, 1, 1, 1));
		glLightfv(GL_LIGHT0, GL_POSITION, CQuaternion(m_vLightPos.x, m_vLightPos.y, m_vLightPos.z, 1));
	}

	for(int i=0; i<6; i++) m_nTriangles += m_pRoot[m_nOrder[i]]->DrawSurface();

	if(pShader->IsValid()) pShader->Disable();
	else glDisable(GL_LIGHTING);

	if(HasAtmosphere()) {
		if(IsInAtmosphere()) pShader = &m_shSkyFromAtmosphere;
		else pShader = &m_shSkyFromSpace;

		if(pShader->IsValid()) {
			pShader->Enable();
			InitScatteringShader(pShader);
			glFrontFace(GL_CW);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			GLUquadricObj *pSphere = gluNewQuadric();
			gluSphere(pSphere, m_fAtmosphereRadius, 100, 100);
			gluDeleteQuadric(pSphere);
			glDisable(GL_BLEND);
			glFrontFace(GL_CCW);
			pShader->Disable();
		}
	}

	if(bUseTexture) {
		glMatrixMode(GL_MODELVIEW);
		m_texArray.Disable();
	}

	for(std::list<CPlanetaryFactory::Ref>::iterator it = m_llFactories.begin(); it != m_llFactories.end(); it++) (*it)->Draw();

	glPopMatrix();
}

/*
 */
float CPlanetaryMap::GetHeightAboveGround(const CVector &v) {
	CVector vPos = m_qRotate.UnitInverse().TransformVector(v - m_vTranslate);
	float fAltitude = vPos.Magnitude();
	CPlanetaryMapCoord coord(vPos);
	
	for(int i=0; i<6; i++) {
		if(m_pRoot[i]->HitTest(coord))
			return fAltitude - m_pRoot[i]->GetHeight(coord);
	}
	
	return fAltitude - m_fRadius;
}
