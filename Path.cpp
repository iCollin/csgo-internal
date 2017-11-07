#include "Path.h"

namespace Path
{
	std::string MasterDirectory = "H:\\cyan";

	std::string Combine(std::vector<std::string> strs)
	{
		if (strs.size() < 1)
			return "";

		std::string combined = strs[0];

		for (int i = 1; i < strs.size(); i++)
			combined += "\\" + strs[i];

		return combined;
	}

	std::string Get(std::string name)
	{
		return Combine({ MasterDirectory, name });
	}

	std::string Get(std::string name, std::string type)
	{
		return Combine({ MasterDirectory, name + "." + type });
	}

	std::string ConfigDirectory()
	{
		return Combine({ MasterDirectory, "configs" });
	}

	std::string GetConfig(std::string name)
	{
		return Combine({ MasterDirectory, "configs", name });
	}

	std::string GetConfig(std::string name, std::string type)
	{
		return Combine({ MasterDirectory, "configs", name + "." + type });
	}

	std::string GetXMLConfig(std::string name)
	{
		return GetConfig(name, "xml");
	}
}