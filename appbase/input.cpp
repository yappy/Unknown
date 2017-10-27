#include <input.hpp>
#include <exceptions.hpp>
#include <algorithm>

namespace appbase {
namespace input {

void InputManager::ProcessFrame()
{
	m_keys.fill(0);
	int numkeys = 0;
	const Uint8 *p = ::SDL_GetKeyboardState(&numkeys);
	std::copy_n(p,
		std::min(m_keys.size(), static_cast<decltype(m_keys.size())>(numkeys)),
		m_keys.begin());
}

const InputManager::KeyState &InputManager::GetKeyState()
{
	return m_keys;
}

}
}
