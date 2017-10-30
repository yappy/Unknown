#include <input.hpp>
#include <exceptions.hpp>
#include <algorithm>
#include "scancodename.hpp"

namespace appbase {
namespace input {

const char *GetKeyName(int code)
{
	return ScanCodeNames.at(code);
}

void InputManager::ProcessFrame()
{
	// key
	{
		m_keys.fill(0);
		int numkeys = 0;
		const Uint8 *p = ::SDL_GetKeyboardState(&numkeys);
		std::copy_n(p,
			std::min(m_keys.size(), static_cast<decltype(m_keys.size())>(numkeys)),
			m_keys.begin());
	}
	// mouse
	{
		auto button = ::SDL_GetMouseState(&m_mouse.x, &m_mouse.y);
		m_mouse.button.left   = (button & SDL_BUTTON_LMASK) != 0;
		m_mouse.button.middle = (button & SDL_BUTTON_MMASK) != 0;
		m_mouse.button.right  = (button & SDL_BUTTON_RMASK) != 0;
	}
}

}
}
