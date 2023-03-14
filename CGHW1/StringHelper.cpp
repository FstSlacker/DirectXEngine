#include "StringHelper.h"

std::string FilePathHelper::GetFileDirectory(std::string filePath)
{
    size_t ind1 = filePath.find_last_of('\\');
    size_t ind2 = filePath.find_last_of('/');

    if (ind1 == std::string::npos && ind2 == std::string::npos)
        return {};

    if (ind1 == std::string::npos)
        return filePath.substr(0, ind2);

    if (ind2 == std::string::npos)
        return filePath.substr(0, ind1);

    return filePath.substr(0, std::max(ind1, ind2));
}

std::string FilePathHelper::GetFileExtention(std::string filePath)
{
    size_t ind = filePath.find_last_of('.');

    if (ind == std::string::npos)
        return {};

    return std::string(filePath.substr(ind + 1));
}

std::wstring FilePathHelper::GetFileExtention(std::wstring filePath)
{
    size_t ind = filePath.find_last_of('.');

    if (ind == std::wstring::npos)
        return {};

    return filePath.substr(ind + 1);
}

std::string FilePathHelper::GetFileName(std::string filePath)
{
    size_t ind1 = filePath.find_last_of('\\');
    size_t ind2 = filePath.find_last_of('/');

    if (ind1 == std::string::npos && ind2 == std::string::npos)
        return filePath;

    if (ind1 == std::string::npos)
        return filePath.substr(ind2 + 1);

    if (ind2 == std::string::npos)
        return filePath.substr(ind1 + 1);

    return filePath.substr(std::max(ind1, ind2) + 1);
}

std::wstring FilePathHelper::FindFileInParentDirectories(std::string name, std::string directory)
{
    if (name.size() == 0 || directory.size() == 0)
        return {};

    if (directory[directory.size() - 1] == '/' || directory[directory.size() - 1] == '\\')
    {
        directory = directory.substr(0, directory.size() - 1);
    }


    directory = GetFileDirectory(directory);

    std::wstring wPath = std::wstring(directory.begin(), directory.end());
    std::wstring wName = std::wstring(name.begin(), name.end());

    std::wstring res = FindFileInDirectory(wPath, wName, 3);

    return res;
}

bool FilePathHelper::IsFileExist(std::wstring filePath)
{
    WIN32_FIND_DATA file;
    HANDLE searchHandle = FindFirstFile(filePath.c_str(), &file);

    if (!(searchHandle == INVALID_HANDLE_VALUE))
    {
        FindClose(searchHandle);
        return true;
    }

    return false;
}

bool FilePathHelper::IsFileExist(std::string filePath)
{
    std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());
    return IsFileExist(wFilePath);
}

std::wstring FilePathHelper::FindFileInDirectory(std::wstring path, std::wstring name, UINT depth)
{
    if (depth == 0)
        return L"";

    std::wstring fileFilter = path + L"\\*";
    std::wstring searchingFileName = path + L"\\" + name;

    WIN32_FIND_DATA file;
    HANDLE searchHandle = FindFirstFile(searchingFileName.c_str(), &file);

    if (!(searchHandle == INVALID_HANDLE_VALUE))
    {
        FindClose(searchHandle);
        return searchingFileName;
    }
    searchHandle = FindFirstFile(fileFilter.c_str(), &file);

    if (!(searchHandle == INVALID_HANDLE_VALUE))
    {
        do {
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                std::wstring res = FindFileInDirectory(path + L"\\" + file.cFileName, name, depth - 1);
                if (res != L"")
                {
                    FindClose(searchHandle);
                    return res;
                }
            }
        } while (FindNextFile(searchHandle, &file));
        FindClose(searchHandle);
    }

    return L"";
}

std::wstring StringConverter::StringToWide(std::string str)
{
    std::wstring wStr = std::wstring(str.begin(), str.end());
    return wStr;
}
