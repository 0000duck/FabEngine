#pragma once

#include <DirectXComponentsPCH.h>

using namespace DirectX;

namespace Fab
{
	enum class ComponentType
	{
		DEFAULT,
		CAMERA,
		KEYBOARD,
		MOUSE
	};

	class IComponent {
	public:
		IComponent();
		IComponent(ComponentType type);
		virtual ~IComponent() = 0;
		virtual void Initialise() = 0;

	public:
		ComponentType GetType();

	protected:
		ComponentType _type;
	};
}