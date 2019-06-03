#include "Engine.h"

/*
 */
bool CInputTask::Start() {
	m_pConsole = NULL;
	m_bConsoleActive = false;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	m_pKeys = SDL_GetKeyState(&m_nKeyCount);
	m_nMouseButtons = SDL_GetRelativeMouseState(&m_nMouseX, &m_nMouseY);

	SDL_PumpEvents();
	SDL_PumpEvents();

	return true;
}

/*
 */
void CInputTask::Update() {
	PROFILE("CInputTask::Update()", 1);

	SDL_Event event;

	while (SDL_PollEvent(&event) > 0) {
		std::list<IInputEventListener *>::iterator it = m_listeners.begin();

		switch (event.type) {
			case SDL_ACTIVEEVENT:
			{
				while (it != m_listeners.end()) (*it++)->OnActivate(true);
				break;
			}

			case SDL_QUIT:
			{
				while (it != m_listeners.end()) (*it++)->OnQuit();
				break;
			}

			case SDL_KEYDOWN:
			{
				if (m_pConsole != NULL && event.key.keysym.sym == SDLK_BACKQUOTE) {
					m_bConsoleActive = !m_bConsoleActive;
					break;
				}

				if (m_bConsoleActive) m_pConsole->OnKeyDown(event.key.keysym.sym, event.key.keysym.mod);
				else while (it != m_listeners.end()) (*it++)->OnKeyDown(event.key.keysym.sym, event.key.keysym.mod);
				break;
			}

			case SDL_KEYUP:
			{
				if (m_bConsoleActive) m_pConsole->OnKeyUp(event.key.keysym.sym, event.key.keysym.mod);
				else while (it != m_listeners.end()) (*it++)->OnKeyUp(event.key.keysym.sym, event.key.keysym.mod);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				if (m_bConsoleActive) m_pConsole->OnMouseDown(event.button.button);
				else while (it != m_listeners.end()) (*it++)->OnMouseDown(event.button.button);
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				if (m_bConsoleActive) m_pConsole->OnMouseUp(event.button.button);
				else while (it != m_listeners.end()) (*it++)->OnMouseUp(event.button.button);
				break;
			}
		}
	}

	//SDL_PumpEvents();
	m_pKeys = SDL_GetKeyState(&m_nKeyCount);
	m_nMouseButtons = SDL_GetRelativeMouseState(&m_nMouseX, &m_nMouseY);
}

/*
 */
void CInputTask::Stop() {

}

/*
 */
bool CInputTask::IsKeyDown(int n) { 
	return m_bConsoleActive ? false : m_pKeys[n] != 0; 
}

/*
 */
bool CInputTask::IsMouseButtonDown(int n) { 
	return m_bConsoleActive ? false : (m_nMouseButtons & SDL_BUTTON(n)) != 0; 
}

/*
 */
int CInputTask::GetMouseX()	{ 
	return m_bConsoleActive ? 0 : m_nMouseX; 
}

/*
 */
int CInputTask::GetMouseY()	{ 
	return m_bConsoleActive ? 0 : m_nMouseY; 
}

/*
 */
void CInputTask::SetConsole(IInputEventListener *pConsole) { 
	m_pConsole = pConsole; 
}

/*
 */
bool CInputTask::IsConsoleActive() { 
	return m_bConsoleActive; 
}

/*
 */
void CInputTask::AddInputEventListener(IInputEventListener *pListener) { 
	m_listeners.push_back(pListener); 
}

/*
 */
void CInputTask::RemoveInputEventListener(IInputEventListener *pListener) {
	std::list<IInputEventListener *>::iterator it = m_listeners.begin();
	while (it != m_listeners.end()) {
		if (*it == pListener) it = m_listeners.erase(it);
		else it++;
	}
}