#pragma once

#include "File.h"

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <stdexcept>

class Directory
{
public:
    Directory(const std::string& name);
    Directory(const std::string& name, const std::string& path);
    ~Directory();

    void open();   
    void close();  

    void addFile(const std::string& FileName);     
    void deleteFile(const std::string& FileName);  
    void remove();  
    std::vector<std::string> getFileNames() const; 
    File& searchFile(const std::string& FileName); 

private:
    std::string DirectoryName;
    std::string DirectoryPath;

    std::vector<std::shared_ptr<File>> DirectoryFiles;

    std::filesystem::path getFullPath() const;
};
