#pragma once

#include <DirectXComponentsPCH.h>
#include "Shader.h"

using namespace DirectX;

namespace Fab
{
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();
		void      Initialise();
		void      InsertShader(std::string name, ShaderPtr shader);
		Shader&   GetShader(std::string name);
		ShaderPtr GetShaderPtr(std::string name);

	private:
		std::map<std::string, ShaderPtr> _shaders;
	};
}