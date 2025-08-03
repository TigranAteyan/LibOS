#pragma once

#include <string>
#include <fstream>
#include <filesystem>

class File {
public:
    File();
    File(const std::string& name);
    File(const std::string& name, const std::string& path);
    ~File();

    void openFile();
    void closeFile();
    void launchFileInEditor() const;
    void write(const std::string& data);
    void renameFile(const std::string& newName);
    void removeFile();

    bool isOpen();
    std::string getPath() const;
    std::string getName() const;

private:
    std::string fileName;
    std::string filePath;

    std::ofstream fileStream;
    static int fileCount;
};
