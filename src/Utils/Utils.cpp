#include "Utils.h"

namespace Utils
{
	std::string readWholeFile(std::string_view path)
	{
		std::fstream fs(path.data());

		if (!fs.is_open())
		{
			std::println("Failed to open file : {}", path.data());
			return {};
		}

		std::string buffer, output;

		while (std::getline(fs, buffer))
		{
			output += buffer + '\n';
		}
		return output;
	}

	bool saveStringToFile(std::string_view path, const std::string& data)
	{
		std::fstream fs(path.data(), std::ios::out);

		if (!fs.is_open())
		{
			std::println("Failed to open file : {}", path.data());
			return false;
		}

		fs << data;
		return true;
	}
}

