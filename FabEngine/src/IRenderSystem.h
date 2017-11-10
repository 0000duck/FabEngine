#pragma once

namespace Fab
{
	class IRenderSystem
	{
	public:
		IRenderSystem();
		virtual ~IRenderSystem();
		virtual HRESULT InitDevice() = 0;
		virtual HRESULT ResetDevice() = 0;
		virtual void CleanUp() = 0;
		virtual void Draw() = 0;
	};
}