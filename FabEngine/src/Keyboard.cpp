#include <DirectXComponentsPCH.h>
#include "Keyboard.h"

namespace Fab
{
	Keyboard::Keyboard()
		: IComponent(ComponentType::KEYBOARD)
	{
		Initialise();
	}

	Keyboard::~Keyboard()
	{
	}

	void Keyboard::Update(MSG* message, float deltaTime, float totalTime)
	{
		for (auto key = _keys.begin(); key != _keys.end(); key++)
		{
			if ((*key).Value == message->wParam)
			{
				UpdateState(&(*key), message);
			}
		}
	}

	void Keyboard::UpdateState(Key* key, MSG* message)
	{
		switch (message->message)
		{
		case WM_KEYUP: {
			key->Pressed = false;
		}break;

		case WM_KEYDOWN: {
			key->Pressed = true;
		}break;
		}
	}

	void Keyboard::Initialise()
	{
		_keys.push_back(Key(KeyName::Z, 0x5A));
		_keys.push_back(Key(KeyName::Q, 0x51));
		_keys.push_back(Key(KeyName::S, 0x53));
		_keys.push_back(Key(KeyName::D, 0x44));
		_keys.push_back(Key(KeyName::E, 0x45));
		_keys.push_back(Key(KeyName::R, 0x52));
		_keys.push_back(Key(KeyName::SPACE, VK_SPACE));
		_keys.push_back(Key(KeyName::ESCAPE, VK_ESCAPE));
		_keys.push_back(Key(KeyName::CTRL, VK_CONTROL));
		_keys.push_back(Key(KeyName::SHIFT, VK_SHIFT));
		_keys.push_back(Key(KeyName::TAB, VK_TAB));
		_keys.push_back(Key(KeyName::ARROW_LEFT, VK_LEFT));
		_keys.push_back(Key(KeyName::ARROW_RIGHT, VK_RIGHT));
		_keys.push_back(Key(KeyName::ARROW_UP, VK_UP));
		_keys.push_back(Key(KeyName::ARROW_DOWN, VK_DOWN));
	}
		
	bool Keyboard::IsKeyPressed(KeyName name)
	{
		if (std::find(_keys.begin(), _keys.end(), name) != _keys.end())
		{
			for (auto key : _keys)
			{
				if (key.Name == name) return key.Pressed;
			}
		}
		else
		{
			assert(false);
		}

		return _keys.begin()->Pressed;
	}
}