#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <print>

namespace Utils
{
	std::string readWholeFile(std::string_view path);
	bool saveStringToFile(std::string_view path, const std::string& data);
}
