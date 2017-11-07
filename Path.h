#pragma once

#include <string>
#include <vector>

namespace Path
{
	extern std::string MasterDirectory;

	std::string Combine(std::vector<std::string> strs);

	std::string Get(std::string name);

	std::string Get(std::string name, std::string type);

	std::string ConfigDirectory();

	std::string GetConfig(std::string name);

	std::string GetConfig(std::string name, std::string type);

	std::string GetXMLConfig(std::string name);
};