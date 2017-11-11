#include <DirectXComponentsPCH.h>
#include "Mouse.h"

namespace Fab
{
	Mouse::Mouse()
		: IComponent(ComponentType::MOUSE)
		, _renderSystem(D3D11RenderSystem::GetRenderSystem())
		, _position(XMFLOAT2(0.0f,0.0f))
		, _oldPosition(_position)
		, _centerPosition(_position)
	{
		Initialise();
	}

	Mouse::~Mouse()
	{
	}

	void Mouse::Initialise()
	{
		_oldPosition.x = (float)_renderSystem.GetWindowWidth() / 2;
		_oldPosition.y = (float)_renderSystem.GetWindowHeight() / 2;
		_centerPosition = _oldPosition;

		_mouseButtons.push_back(MouseButton(MouseButtonName::LEFT));
		_mouseButtons.push_back(MouseButton(MouseButtonName::RIGHT));
		_mouseButtons.push_back(MouseButton(MouseButtonName::MIDDLE));
		_mouseButtons.push_back(MouseButton(MouseButtonName::WHOLE));
	}

	void Mouse::Update(MSG* message, float deltaTime, float totalTime)
	{
		_centerPosition.x = (float)_renderSystem.GetWindowWidth() / 2;
		_centerPosition.y = (float)_renderSystem.GetWindowHeight() / 2;

		_position.x = (float) GET_X_LPARAM(message->lParam);
		_position.y = (float) GET_Y_LPARAM(message->lParam);

		_distanceX = _position.x - _centerPosition.x;
		_distanceY = _position.y - _centerPosition.y;

		if (abs(_distanceX) < 25.0f || _position.x == 0.0f)
		{
			_distanceX = 0.0f;
		}

		if (abs(_distanceY) < 25.0f || _position.y == 0.0f)
		{
			_distanceY = 0.0f;
		}

		_oldPosition = _position;

		switch (message->message)
		{
		case WM_LBUTTONDOWN:
			UpdateState(MouseButtonName::LEFT, MouseButtonState::PRESSED);
			break;

		case WM_RBUTTONDOWN:
			UpdateState(MouseButtonName::RIGHT, MouseButtonState::PRESSED);
			break;

		case WM_MBUTTONDOWN:
			UpdateState(MouseButtonName::MIDDLE, MouseButtonState::PRESSED);
			break;

		case WM_LBUTTONUP:
			UpdateState(MouseButtonName::LEFT, MouseButtonState::RELEASED);
			break;

		case WM_RBUTTONUP:
			UpdateState(MouseButtonName::RIGHT, MouseButtonState::RELEASED);
			break;

		case WM_MBUTTONUP:
			UpdateState(MouseButtonName::MIDDLE, MouseButtonState::RELEASED);
			break;

		case WM_MOUSEHOVER:
			UpdateState(MouseButtonName::WHOLE, MouseButtonState::PRESSED);
			break;

		case WM_MOUSELEAVE:
			UpdateState(MouseButtonName::WHOLE, MouseButtonState::RELEASED);
			break;
		}
	}

	void Mouse::UpdateState(MouseButtonName name, MouseButtonState state)
	{
		for (auto button = _mouseButtons.begin(); button != _mouseButtons.end(); button++)
		{
			if ((*button).Name == name)
			{
				(*button).State = state;
			}
		}
	}

	float Mouse::GetDistanceX()
	{
		return _distanceX;
	}

	float Mouse::GetDistanceY()
	{
		return _distanceY;
	}

	XMFLOAT2 Mouse::GetPosition()
	{
		return _position;
	}

	MouseButtonState Mouse::GetState(MouseButtonName name)
	{
		if (std::find(_mouseButtons.begin(), _mouseButtons.end(), name) != _mouseButtons.end())
		{
			for (auto _button : _mouseButtons)
			{
				if (_button.Name == name) return _button.State;
			}
		}
		else
		{
			assert(false);
		}

		return _mouseButtons.begin()->State;
	}
}