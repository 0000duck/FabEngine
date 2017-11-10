#pragma once

#include <DirectXComponentsPCH.h>

namespace Fab
{
	class IFactory
	{
	public:
		IFactory();
		virtual ~IFactory() = 0;
	};
}