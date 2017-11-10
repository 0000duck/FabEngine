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
		void                    Initialise();
		void                    InsertShader(std::string name, std::shared_ptr<Shader> shader);
		Shader&                 GetShader(std::string name);
		std::shared_ptr<Shader> GetShaderPtr(std::string name);

	private:
		std::map<std::string, std::shared_ptr<Shader>> _shaders;
	};
}