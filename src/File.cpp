#include "../include/File.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int File::fileCount = 0;

File::File() 
{
    if (fileCount == 0)
        fileName = "new.txt";
    else 
        fileName = "new" + std::to_string(fileCount) + ".txt";
    filePath = "."; 

    ++fileCount;

    std::filesystem::path fullPath = std::filesystem::path(filePath) / fileName;
    std::filesystem::create_directories(filePath);

    fileStream.open(fullPath, std::ios::out);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Can't create file " + fullPath.string());
    }
}

File::File(const std::string& name) : fileName(name), filePath(".")
{
    std::filesystem::path fullPath = std::filesystem::path(filePath) / fileName;
    fileStream.open(fullPath, std::ios::out);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Can't create file " + fullPath.string());
    }
}

File::File(const std::string& name, const std::string& path) : fileName(name), filePath(path)
{
    std::filesystem::path fullPath = std::filesystem::path(filePath) / fileName;
    std::filesystem::create_directories(filePath);

    fileStream.open(fullPath, std::ios::out);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Can't create file " + fullPath.string());
    }
}

File::~File() {}


void File::openFile()
{
    if (!fileStream.is_open()) {
        std::filesystem::path fullPath = std::filesystem::path(filePath) / fileName;
        fileStream.open(fullPath, std::ios::in | std::ios::out | std::ios::app);
        if (!fileStream.is_open()) {
            throw std::runtime_error("Can't open file " + fullPath.string());
        }
    }
}

void File::closeFile()
{
    if (fileStream.is_open()) {
        fileStream.close();
    }
}

void File::launchFileInEditor() const
{
    std::string path = getPath();
    std::string command = "xdg-open " + path;

    system(command.c_str());
}


void File::write(const std::string& data)
{
    if (!fileStream.is_open()) {
        throw std::runtime_error("File is not open");
    }
    fileStream << data;
}

void File::renameFile(const std::string& newName) {
    std::filesystem::path oldFullPath = getPath();
    std::filesystem::path newFullPath = std::filesystem::path(filePath) / newName;

    if (!std::filesystem::exists(oldFullPath)) {
        throw std::runtime_error("File does not exist to rename");
    }

    std::filesystem::rename(oldFullPath, newFullPath);
    fileName = newName;
}


void File::removeFile()
{
    if (fileStream.is_open()) {
        fileStream.close();
    }

    std::filesystem::path fullPath = std::filesystem::path(filePath) / fileName;
    std::error_code ec;
    if (!std::filesystem::remove(fullPath, ec)) {
        std::cerr << "Failed to delete file " << fullPath << ": " << ec.message() << "\n";
    }
}

bool File::isOpen()
{
    return fileStream.is_open();
}

std::string File::getPath() const
{
    std::filesystem::path fullPath = std::filesystem::path(filePath) / fileName;
    return fullPath.string();
}

std::string File::getName() const { return fileName; }
