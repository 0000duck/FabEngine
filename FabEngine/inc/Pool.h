#pragma once

#include <DirectXComponentsPCH.h>

using namespace DirectX;

namespace Fab
{
	template <typename Resource>
	class Pool
	{
	public:
		Pool(size_t size)
			: _size(size)
			, _resources(new Resource[_size])
		{

		}

		~Pool()
		{

		}

	private:
		size_t _size;
		std::unique_ptr<Resource[]> _resources;
	};
}