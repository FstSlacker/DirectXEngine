#pragma once
#include <string>

class FilePathHelper
{
public:
	static std::string GetFileDirectory(std::string filePath);
	static std::string GetFileExtention(std::string filePath);
};

