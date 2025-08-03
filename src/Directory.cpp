#include "Directory.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

Directory::Directory(const std::string& name)
    : DirectoryName(name), DirectoryPath(".")
{
    fs::path fullPath = getFullPath();
    std::error_code ec;
    if (!fs::create_directories(fullPath, ec) && !fs::exists(fullPath)) {
        throw std::runtime_error("Can't create directory: " + fullPath.string() + " — " + ec.message());
    }
}

Directory::Directory(const std::string& name, const std::string& path)
    : DirectoryName(name), DirectoryPath(path)
{
    fs::path fullPath = getFullPath();
    std::error_code ec;
    if (!fs::create_directories(fullPath, ec) && !fs::exists(fullPath)) {
        throw std::runtime_error("Can't create directory: " + fullPath.string() + " — " + ec.message());
    }
}

Directory::~Directory() {
    close();
}

fs::path Directory::getFullPath() const {
    return fs::path(DirectoryPath) / DirectoryName;
}

void Directory::open() {
    DirectoryFiles.clear();
    if (!fs::exists(getFullPath()) || !fs::is_directory(getFullPath())) {
        throw std::runtime_error("Directory does not exist: " + getFullPath().string());
    }
    for (const auto& entry : fs::directory_iterator(getFullPath())) {
        if (entry.is_regular_file()) {
            DirectoryFiles.push_back(std::make_shared<File>(entry.path().filename().string(), getFullPath().string()));
        }
    }
}

void Directory::close() {
    DirectoryFiles.clear();
}

void Directory::addFile(const std::string& FileName) {
    fs::path newFilePath = getFullPath() / FileName;
    std::ofstream file(newFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + newFilePath.string());
    }
    file.close(); 

    DirectoryFiles.push_back(std::make_shared<File>(FileName, getFullPath().string()));
}

void Directory::deleteFile(const std::string& FileName) {
    auto it = std::find_if(DirectoryFiles.begin(), DirectoryFiles.end(), [&](const std::shared_ptr<File>& f) {
        return f->getName() == FileName;
    });

    if (it == DirectoryFiles.end()) {
        throw std::runtime_error("File not found: " + FileName);
    }

    (*it)->removeFile();
    DirectoryFiles.erase(it);
}

void Directory::remove() {
    std::error_code ec;
    if (!fs::remove_all(getFullPath(), ec)) {
        throw std::runtime_error("Cannot remove directory: " + getFullPath().string() + " — " + ec.message());
    }
    close();
}

std::vector<std::string> Directory::getFileNames() const {
    std::vector<std::string> names;
    for (const auto& f : DirectoryFiles) {
        names.push_back(f->getName());
    }
    return names;
}

File& Directory::searchFile(const std::string& FileName) {
    auto it = std::find_if(DirectoryFiles.begin(), DirectoryFiles.end(), [&](const std::shared_ptr<File>& f) {
        return f->getName() == FileName;
    });

    if (it == DirectoryFiles.end()) {
        throw std::runtime_error("File not found: " + FileName);
    }
    return **it;
}
