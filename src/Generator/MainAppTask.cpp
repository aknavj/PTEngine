#include "Engine.h"
#include "PlanetaryMap.h"
#include "MixedHeightMapFactory.h"
#include "GrassFactory.h"

DECLARE_CORE_GLOBALS;

/*
 */
class CAppTask : public TSingleton<CAppTask>, public CKernelTask, public IInputEventListener {

	protected:
		std::vector<CPlanetaryMap> m_vPlanet;
		int m_nPolygonMode;
		bool m_bUpdate;
		bool m_bHDR;
		bool m_bHDRSquare;
		float m_fSplitFactor;
		float m_fExposure;

		CGLShaderObject m_shHDR;
		CGLFrameBufferObject m_fb;
		CVector m_vLightPos;

	public:
		DEFAULT_TASK_CREATOR(CAppTask);

		virtual void OnQuit() {
			CKernel::GetPtr()->KillAllTasks();
		}

		virtual void OnKeyDown(int nKey, int nMod) {
			
			switch(nKey) {
				case SDLK_ESCAPE:
				{
					OnQuit();
					break;
				}
#ifdef _WIN32
				case SDLK_c:
				{
					if (CCaptureTask::IsValid() && CKernel::GetPtr()->IsTaskRunning(CCaptureTask::GetPtr()))
						CKernel::GetPtr()->RemoveTask(CCaptureTask::GetPtr());
					else
						CKernel::GetPtr()->AddTask(CCaptureTask::IsValid() ? CCaptureTask::GetPtr() : CCaptureTask::Create(11000));
					break;
				}
#endif
				case SDLK_h:
				{
					m_bHDR = !m_bHDR;
					break;
				}
				case SDLK_p:
				{
					m_nPolygonMode = (m_nPolygonMode == GL_FILL) ? GL_LINE : GL_FILL;
					break;
				}
				case SDLK_u:
				{
					m_bUpdate = !m_bUpdate;
					break;
				}
				case SDLK_PLUS:
				case SDLK_KP_PLUS:
				case SDLK_EQUALS:
				{
					m_fSplitFactor += 0.1f;
					break;
				}
				case SDLK_MINUS:
				case SDLK_KP_MINUS:
				{
					m_fSplitFactor -= 0.1f;
					break;
				}
			}
		}

		/*
		 */
		virtual bool Start() {			
			CSimpleHeightMapFactoryType::Load();
			CSimpleCraterFactoryType::Load();
			CSimpleColorMapFactoryType::Load();
			CMixedHeightMapFactoryType::Load();
			CGrassFactoryType::Load();

			CInputTask::GetPtr()->AddInputEventListener(this);
			CCameraTask::GetPtr()->SetPosition(CVector(0, 0, 15.0f));
			CCameraTask::GetPtr()->SetThrust(1.0f);

			m_nPolygonMode = GL_FILL;
			m_bHDR = true;
			m_bUpdate = true;
			m_fSplitFactor = 16.0f;
			m_fExposure = 2.0f;

			m_vPlanet.reserve(50);
			CPropertySet prop;
			prop.LoadFile(ENGINE_CONFIG);
			for(int i=1; ; i++) {
				char szPrefix[256];
				sprintf(szPrefix, "planet.%d.", i);
				prop.SetPrefix(szPrefix);
				if(prop.GetProperty("position") == NULL)
					break;
				m_vPlanet.push_back(CPlanetaryMap());
				m_vPlanet.back().Init(prop);
			}

			m_bHDRSquare = !CVideoTask::GetPtr()->IsNVIDIA();
			if(CVideoTask::GetPtr()->IsNVIDIA()) {
				m_fb.Init(CVideoTask::GetPtr()->GetWidth(), CVideoTask::GetPtr()->GetHeight(), GL_FLOAT_RGBA16_NV);
				if(m_fb.IsValid()) m_shHDR.Init("shaders/HDRVert.glsl", "shaders/HDRRectFrag.glsl");
			} else if(CVideoTask::GetPtr()->IsATI()) {
				m_fb.Init(1024, 1024, GL_RGBA_FLOAT16_ATI);
				if(m_fb.IsValid()) m_shHDR.Init("shaders/HDRVert.glsl", "shaders/HDRSquareFrag.glsl");
			} else {
				m_fb.Init(1024, 1024, GL_RGBA16F_ARB);
				if(m_fb.IsValid()) m_shHDR.Init("shaders/HDRVert.glsl", "shaders/HDRSquareFrag.glsl");
			}

			m_vLightPos = CVector(-100, 0, 0);

			return true;
		}

		/*
		 */
		virtual void Update() {
			int i;

			PROFILE("CAppTask::Update()", 1);

			if(m_bHDR && m_fb.IsValid()) {
				m_fb.EnableFrameBuffer();
				if (m_bHDRSquare) glViewport(0, 0, CVideoTask::GetPtr()->GetWidth(), CVideoTask::GetPtr()->GetHeight());
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			// fast colision
			CVector vCamera = CCameraTask::GetPtr()->GetPosition();
			for(i=0; i<(int)m_vPlanet.size(); i++) {
				if(m_vPlanet[i].GetHeightAboveGround(vCamera) < 0.0f)
					CCameraTask::GetPtr()->Bounce();
			}

			// update
			if(m_bUpdate) {

				CQuaternion q;
				q.SetAxisAngle(CVector(0, 1, 0), 0.1f * CTimerTask::GetPtr()->GetFrameTicks() / 1000.0f);
				m_vLightPos = q.TransformVector(m_vLightPos);

				for(i=0; i<(int)m_vPlanet.size(); i++)
					m_vPlanet[i].UpdateSurface(m_vLightPos, vCamera, CCameraTask::GetPtr()->GetCamera().m_qRotate, 1.25f, m_fSplitFactor);
			}

			// render planets
			glPolygonMode(GL_FRONT, m_nPolygonMode);
			for(i=0; i<(int)m_vPlanet.size(); i++)
				m_vPlanet[i].DrawSurface();
			glPolygonMode(GL_FRONT, GL_FILL);

			if(m_bHDR && m_fb.IsValid()) {
				m_fb.DisableFrameBuffer();

				if(m_bHDRSquare) glViewport(0, 0, CVideoTask::GetPtr()->GetWidth(), CVideoTask::GetPtr()->GetHeight());

				CVideoTask::GetPtr()->GetFont().Begin();

				m_fb.EnableTexture();
				m_shHDR.Enable();
				m_shHDR.SetUniformParameter1f("fExposure", m_fExposure);
				m_shHDR.SetUniformParameter1i("s2Test", 0);

				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glBegin(GL_QUADS);
				{
					if(m_bHDRSquare) {
						glTexCoord2f(0, 0); glVertex2i(0, CVideoTask::GetPtr()->GetHeight());
						glTexCoord2f(1, 0); glVertex2i(CVideoTask::GetPtr()->GetWidth(), CVideoTask::GetPtr()->GetHeight());
						glTexCoord2f(1, 1); glVertex2i(CVideoTask::GetPtr()->GetWidth(), 0);
						glTexCoord2f(0, 1); glVertex2i(0,  0);
					} else {
						glTexCoord2f(0, 0); glVertex2i(0, CVideoTask::GetPtr()->GetHeight());
						glTexCoord2f(CVideoTask::GetPtr()->GetWidth(), 0); glVertex2i(CVideoTask::GetPtr()->GetWidth(), CVideoTask::GetPtr()->GetHeight());
						glTexCoord2f(CVideoTask::GetPtr()->GetWidth(), CVideoTask::GetPtr()->GetHeight()); glVertex2i(CVideoTask::GetPtr()->GetWidth(), 0);
						glTexCoord2f(0, CVideoTask::GetPtr()->GetHeight()); glVertex2i(0,  0);
					}
				}

				glEnd();
				
				m_shHDR.Disable();
				m_fb.DisableTexture();
				
				CVideoTask::GetPtr()->GetFont().End();
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			char szBuffer[256];
			CFont &fFont = CVideoTask::GetPtr()->GetFont();
			fFont.Begin();
			sprintf(szBuffer, "Depth: %d, Nodes: %d, Triangles: %d", m_vPlanet[0].GetMaxDepth(), m_vPlanet[0].GetNodeCount(), m_vPlanet[0].GetTriangleCount());
			fFont.SetPosition(0, 15);
			fFont.Print(szBuffer);
			sprintf(szBuffer, "Split Factor (+/-): %-2.2f", m_fSplitFactor);
			fFont.SetPosition(0, 30);
			fFont.Print(szBuffer);
			fFont.End();
		}

		/*
		 */
		virtual void Stop() {

			m_fb.Cleanup();
			m_shHDR.Cleanup();
			m_vPlanet.clear();
			
			CPlanetaryObjectType::Unload();
			CInputTask::GetPtr()->RemoveInputEventListener(this);

			CSimpleHeightMapFactoryType::Unload();
			CSimpleCraterFactoryType::Unload();
			CSimpleColorMapFactoryType::Unload();
			CMixedHeightMapFactoryType::Unload();
			CGrassFactoryType::Unload();
		}
};

/*
 */
int main(int argc, char *argv[]) {

	CLog log;
	
	log.Init(Debug, ENGINE_LOG);
	LogInfo("Starting app (%s)", g_strBuildStamp.c_str());
	
	CProfiler profile("main", 3, Info);

	CKernel *pKernel = CKernel::Create();
	pKernel->AddTask(CTimerTask::Create(10));
	pKernel->AddTask(CInputTask::Create(20));
	pKernel->AddTask(CInterpolatorTask::Create(30));
	pKernel->AddTask(CTriggerTask::Create(40));
	pKernel->AddTask(CCameraTask::Create(50));
	pKernel->AddTask(CConsoleTask::Create(9000));
	pKernel->AddTask(CVideoTask::Create(10000));
	pKernel->AddTask(CAppTask::Create(100));

	pKernel->Execute();
	pKernel->Destroy();

	CPlanetaryObjectType::Unload();

	LogInfo("Closing app");

	return 0;
}
