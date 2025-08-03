#include <gtest/gtest.h>
#include "../include/File.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class FileTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::vector<std::string> files = {
            "new.txt", "test.txt", "renamed.txt", "new0.txt", "new1.txt"
        };
        for (const auto& filename : files) {
            File f(filename);
            f.removeFile();
        }
        fs::remove_all("temp");
    }
};

TEST_F(FileTest, CreatesFileWithDefaultName) {
    File f;
    EXPECT_TRUE(fs::exists("new.txt"));
}

TEST_F(FileTest, CreatesFileWithCustomName) {
    File f("test.txt");
    EXPECT_TRUE(fs::exists("test.txt"));
}

TEST_F(FileTest, CreatesFileInCustomDirectory) {
    std::string path = "temp";
    File f("test.txt", path);
    EXPECT_TRUE(fs::exists(fs::path(path) / "test.txt"));
}

TEST_F(FileTest, WritesDataToFile) {
    File f("test.txt");
    f.openFile();
    f.write("Hello, world!");
    f.closeFile();

    std::ifstream in("test.txt");
    std::string line;
    std::getline(in, line);
    EXPECT_EQ(line, "Hello, world!");
}

TEST_F(FileTest, IsOpenReturnsTrueIfFileIsOpen) {
    File f("test.txt");
    f.openFile();
    EXPECT_TRUE(f.isOpen());
    f.closeFile();
}

TEST_F(FileTest, CloseFileActuallyClosesFile) {
    File f("test.txt");
    f.openFile();
    f.closeFile();
    EXPECT_FALSE(f.isOpen());
}

TEST_F(FileTest, RemoveFileActuallyDeletesFile) {
    File f("test.txt");
    f.removeFile();
    EXPECT_FALSE(fs::exists("test.txt"));
}

TEST_F(FileTest, RenameFileChangesNameCorrectly) {
    File f("test.txt");
    f.openFile();
    f.closeFile();

    EXPECT_TRUE(fs::exists("test.txt"));  

    f.renameFile("renamed.txt");

    EXPECT_EQ(f.getName(), "renamed.txt");
    EXPECT_TRUE(fs::exists("renamed.txt"));
    EXPECT_FALSE(fs::exists("test.txt"));
}


TEST_F(FileTest, GetPathReturnsFullPath) {
    File f("test.txt", "temp");
    std::string expected = (fs::path("temp") / "test.txt").string();
    EXPECT_EQ(f.getPath(), expected);
}
