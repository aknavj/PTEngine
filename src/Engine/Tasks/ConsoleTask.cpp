#include "Engine.h"

/*
 */
void CConsoleTask::AddRow(std::string str) {
	m_listRows.push_back(str);

	while (m_listRows.size() > CONSOLE_ROW_HISTORY) m_listRows.pop_front();
}

/*
 */
bool CConsoleTask::Start() {
	CInputTask::GetPtr()->SetConsole(this);
	m_nCommandLength = 0;

	return true;
}

void CConsoleTask::Update() {
	PROFILE("CConsoleTask::Update()", 1);
	
	if (!CInputTask::GetPtr()->IsConsoleActive()) return;

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CFont &font = CVideoTask::GetPtr()->GetFont();
	font.Begin();

	glColor4d(0.25f, 0.25f, 0.25f, 0.5f);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, CONSOLE_ROW_HEIGHT*(m_listRows.size() + 1));
		glVertex2f(800, CONSOLE_ROW_HEIGHT*(m_listRows.size() + 1));
		glVertex2f(800, 0);
	glEnd();
	
	glColor4d(1.0, 1.0, 1.0, 1.0);
	int nHeight = 0;
	for (std::list<std::string>::iterator it = m_listRows.begin(); it != m_listRows.end(); it++) {
		font.SetPosition(0, nHeight);
		font.Print(it->c_str());
		nHeight += CONSOLE_ROW_HEIGHT;
	}
	font.SetPosition(0, nHeight);
	m_szCommand[m_nCommandLength] = 0;
	
	std::string str = std::string(CONSOLE_PROMPT) + std::string(m_szCommand) + std::string("_");
	font.Print(str.c_str());
	font.End();

	glDisable(GL_BLEND);
}

/*
 */
void CConsoleTask::Stop() {

}

/*
 */
// Console Input Key Events
void CConsoleTask::OnKeyDown(int nKey, int nMod) {
	switch (nKey) {
		case SDLK_ESCAPE:
		{
			m_nCommandLength = 0;
			break;
		}
		case SDLK_BACKSPACE:
		{
			if (m_nCommandLength > 0)
				m_nCommandLength--;
			break;
		}

		case SDLK_RETURN:
		{
			AddRow(std::string(CONSOLE_PROMPT) + std::string(m_szCommand));
			m_nCommandLength = 0;
			break;
		}

		default:
		{
			if (nKey >= SDLK_SPACE && nKey <= SDLK_z) {
				if (nMod & KMOD_SHIFT) m_szCommand[m_nCommandLength++] = nKey;
				else m_szCommand[m_nCommandLength++] = nKey;
			}
			break;
		}
	}
}