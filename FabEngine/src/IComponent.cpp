#include <DirectXComponentsPCH.h>
#include "IComponent.h"

namespace Fab
{
	IComponent::IComponent()
		: _type(ComponentType::DEFAULT)
	{
	}

	IComponent::IComponent(ComponentType type)
		: _type(type)
	{
	}

	IComponent::~IComponent()
	{
	}

	ComponentType IComponent::GetType()
	{
		return _type;
	}
}