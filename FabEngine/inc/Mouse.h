#pragma once

#include <DirectXComponentsPCH.h>
#include "IComponent.h"

using namespace DirectX;

namespace Fab
{
	enum class MouseButtonName
	{
		LEFT, RIGHT, MIDDLE, WHOLE
	};

	enum class MouseButtonState
	{
		PRESSED, RELEASED
	};

	struct MouseButton
	{
		MouseButtonName Name;
		MouseButtonState State;

		MouseButton(MouseButtonName name)
			: Name(name)
			, State(MouseButtonState::RELEASED)
		{
		}

		bool MouseButton::operator==(MouseButtonName name) const {
			return name == Name;
		}
	};

	class Mouse : public IComponent {
	public:
		Mouse();
		~Mouse();
		void             Update(MSG* message, float deltaTime, float totalTime);
		void             Initialise() override;

		float            GetDistanceX();
		float            GetDistanceY();
		XMFLOAT2         GetPosition();

		MouseButtonState GetState(MouseButtonName name);

	private:
		void             UpdateState(MouseButtonName name, MouseButtonState state);

	private:
		XMFLOAT2                 _position;
		XMFLOAT2                 _oldPosition;
		XMFLOAT2                 _centerPosition;

		float                    _distanceX;
		float                    _distanceY;

		std::vector<MouseButton> _mouseButtons;
	};
}