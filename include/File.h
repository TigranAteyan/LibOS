#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <optional>

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
    std::optional<std::string> read();

    void renameFile(const std::string& newName);
    void removeFile();

    bool isOpen() const noexcept;
    bool exists() const noexcept;

    std::string getPath() const noexcept;
    std::string getName() const noexcept;

private:
    std::string fileName;
    std::string filePath;

    std::ofstream fileStream;
    static int fileCount;
};
