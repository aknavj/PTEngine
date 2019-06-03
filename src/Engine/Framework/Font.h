#ifndef __FONT_H__
#define __FONT_H__

/*
 */
class CFont {

	public:
		CFont();
		~CFont();

		void Init();
		void Cleanup();
		void SetPosition(int x, int y);

		void Begin();
		void Print(const char *pszMessage);
		void End();

	protected:
		int m_nListBase;
		float m_fXPos;
		float m_fYPos;
};

#endif // __Font_h__

