#ifndef __CG_SHADER_OBJECT_H__
#define __CG_SHADER_OBJECT_H__

#include <Cg/cg.h>
#include <Cg/cgGL.h>

// global variables
static CGcontext m_cgContext;
static CGprofile m_cgVertexProfile;
static CGprofile m_cgFragmentProfile;

/*
 */
class CCGShaderObject {

	protected:
		CGprogram m_cgVertexProgram;
		CGprogram m_cgFragmentProgram;

	public:

		static void HandleCgError();
		static void InitContext();
		static void ReleaseContext();

		CCGShaderObject();
		CCGShaderObject(const char *pszPath);
		~CCGShaderObject();

		void CheckError();

		void Load(const char *pszPath, const char *pszPath2 = NULL);
		void Unload();

		void Enable();
		void Disable();

		void SetTextureParameter(const char *pszParameter, int p1);
		void SetUniformParameter1i(const char *pszParameter, int n1);
		void SetUniformParameter1f(const char *pszParameter, float p1);
		void SetUniformParameter3f(const char *pszParameter, float p1, float p2, float p3);

	
};

#endif // __CG_SHADER_OBJECT_H__

