#pragma once

#include <SDL.h>
#include <array>

namespace appbase {
namespace input {

/*
 * [index] is SDL_SCANCODE_* (see SDL_scancode.h)
 * >= 256 codes will not be used...
 */
const int KeyCount = 256;
using KeyState = std::array<uint8_t, KeyCount>;

const char *GetKeyName(int code);

struct MouseState {
	int x, y;
	union {
		bool barray[3];
		struct {
			bool left, middle, right;
		} button;
	};
};


class InputManager final {
public:
	InputManager(const InputManager &) = delete;
	InputManager & operator =(const InputManager &) = delete;

	InputManager() = default;
	~InputManager() = default;

	void ProcessFrame();

	const KeyState &GetKeyState() const
	{
		return m_keys;
	}
	const MouseState &GetMouseState() const
	{
		return m_mouse;
	}

private:
	KeyState m_keys;
	MouseState m_mouse;
};

}
}
