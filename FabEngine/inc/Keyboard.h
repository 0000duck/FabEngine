#pragma once

#include <DirectXComponentsPCH.h>
#include "IComponent.h"

using namespace DirectX;

namespace Fab
{
	enum class KeyName
	{
		Z, Q, S, D, E, R, SPACE, ESCAPE, CTRL, SHIFT, TAB,
		ARROW_LEFT, ARROW_RIGHT, ARROW_UP, ARROW_DOWN
	};

	struct Key
	{
		KeyName Name;
		int     Value;
		bool    Pressed;

		Key(KeyName name, int value)
			: Name(name)
			, Value(value)
			, Pressed(false)
		{
		}

		bool Key::operator==(KeyName name) const {
			return name == Name;
		}
	};

	class Keyboard: public IComponent {
	public:
		Keyboard();
		~Keyboard();
		void Update(MSG* message, float deltaTime, float totalTime);
		void Initialise() override;
		bool IsKeyPressed(KeyName name);

	private:
		void UpdateState(Key* key, MSG* message);

	private:
		std::vector<Key> _keys;
	};
}