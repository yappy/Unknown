#pragma once

#include <SDL.h>
#include <array>

namespace appbase {
namespace input {

class InputManager final {
public:
	InputManager(const InputManager &) = delete;
	InputManager & operator =(const InputManager &) = delete;

	InputManager() = default;
	~InputManager() = default;

	void ProcessFrame();

	/*
	 * [index] is SDL_SCANCODE_*
	 */
	static const int KeyCount = 256;
	using KeyState = std::array<uint8_t, KeyCount>;

	const KeyState &GetKeyState();


private:
	KeyState m_keys;
};

}
}
