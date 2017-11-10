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
			std::shared_ptr<Shader> color(new Shader(L"shaders/color.fx"));
			color->Compile();
			InsertShader("color", color);
		}
	}

	void ShaderManager::InsertShader(std::string name, std::shared_ptr<Shader> shader)
	{
		_shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(name, shader));
	}

	Shader& ShaderManager::GetShader(std::string name)
	{
		auto found = _shaders.find(name);
		assert(found != _shaders.end());

		return *found->second;
	}

	std::shared_ptr<Shader> ShaderManager::GetShaderPtr(std::string name)
	{
		auto found = _shaders.find(name);
		assert(found != _shaders.end());

		return found->second;
	}
}