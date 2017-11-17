#pragma once

#include <DirectXComponentsPCH.h>
#include "Effect.h"

namespace Fab
{
	class Filter : public IEffect
	{
	public:
		Filter(std::string type);
		~Filter();
		void Initialise();
		void Update(float deltaTime, float totalTime);
		void Draw();

	public:
		static const XMMATRIX FilterNone;
		static const XMMATRIX FilterGreyScale;
		static const XMMATRIX FilterSepia;
		static const XMMATRIX FilterInvert;

	private:
		D3D11RenderSystem& _renderSystem;

		std::string        _type;
	};
}