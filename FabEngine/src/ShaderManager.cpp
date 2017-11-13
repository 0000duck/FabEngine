#include <DirectXComponentsPCH.h>
#include "ShaderManager.h"

namespace Fab
{
	ShaderManager::ShaderManager()
	{
	}

	ShaderManager::~ShaderManager()
	{
	}

	void ShaderManager::Initialise()
	{
		{
			ShaderPtr color(new Shader(L"shaders/default.fx"));
			color->Compile();
			InsertShader("default", std::move(color));
		}
	}

	void ShaderManager::InsertShader(std::string name, ShaderPtr shader)
	{
		_shaders.insert(std::pair<std::string, ShaderPtr>(name, std::move(shader)));
	}

	Shader& ShaderManager::GetShader(std::string name)
	{
		auto found = _shaders.find(name);
		assert(found != _shaders.end());

		return *found->second;
	}

	ShaderPtr ShaderManager::GetShaderPtr(std::string name)
	{
		auto found = _shaders.find(name);
		assert(found != _shaders.end());

		return found->second;
	}
}