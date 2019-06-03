#ifndef __GL_SHADER_OBJECT_H__
#define __GL_SHADER_OBJECT_H__

/*
 */
inline void LogGLInfoLog(GLhandleARB hObj) {
	int nBytes;

	glGetObjectParameterivARB(hObj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &nBytes);

	if(nBytes) {
		char *pInfo = new char[nBytes];

		glGetInfoLogARB(hObj, nBytes, &nBytes, pInfo);
		LogInfo(pInfo);

		delete pInfo;
	}
}

/*
 */
class CGLShaderObject {

	protected:
		GLhandleARB m_hProgram;
		GLhandleARB m_hVertexShader;
		GLhandleARB m_hFragmentShader;
		std::map<std::string, GLint> m_mapParameters;

	public:
		CGLShaderObject();
		~CGLShaderObject();

		bool IsValid();

		void Cleanup();

		bool Init(const char *pszVertexShader, const char *pszFragmentShader);

		void Enable();
		void Disable();

		GLint GetUniformParameterID(const char *pszParameter);

		void SetUniformParameter1i(const char *pszParameter, int n1);
		void SetUniformParameter1f(const char *pszParameter, float p1);
		void SetUniformParameter3f(const char *pszParameter, float p1, float p2, float p3);
};

#endif /* __GL_SHADER_OBJECT_H__ */

