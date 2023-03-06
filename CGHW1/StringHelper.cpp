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

    return std::string(0, std::max(ind1, ind2));
}

std::string FilePathHelper::GetFileExtention(std::string filePath)
{
    size_t ind = filePath.find_last_of('.');

    if (ind == std::string::npos)
        return {};

    return std::string(filePath.substr(ind + 1));
}
