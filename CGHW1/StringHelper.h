#pragma once
#include <string>
#include <Windows.h>
#include <iostream>

class FilePathHelper
{
public:
	static std::string GetFileDirectory(std::string filePath);
	static std::string GetFileExtention(std::string filePath);
	static std::wstring GetFileExtention(std::wstring filePath);
	static std::string GetFileName(std::string filePath);
	static std::wstring FindFileInParentDirectories(std::string name, std::string directory);
	static bool IsFileExist(std::wstring filePath);
	static bool IsFileExist(std::string filePath);

private:
	static std::wstring FindFileInDirectory(std::wstring path, std::wstring name, UINT depth);
};

