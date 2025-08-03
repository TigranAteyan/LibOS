#include <gtest/gtest.h>
#include "../include/File.h"
#include "../include/Directory.h"
#include <filesystem>
#include <fstream>
#include <optional>
#include <algorithm>

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

TEST_F(FileTest, ReadReturnsCorrectContent) {
    File f("test.txt");
    f.openFile();
    f.write("Some content");
    f.closeFile();

    auto content = f.read();
    ASSERT_TRUE(content.has_value());
    EXPECT_EQ(content.value(), "Some content");
}

TEST_F(FileTest, ExistsReturnsTrueForExistingFile) {
    File f("test.txt");
    EXPECT_TRUE(f.exists());
}

TEST_F(FileTest, ExistsReturnsFalseForDeletedFile) {
    File f("test.txt");
    f.removeFile();
    EXPECT_FALSE(f.exists());
}

//     Directory tests

class DirectoryTest : public ::testing::Test {
protected:
    std::string testDirName = "test_dir";

    void SetUp() override {
        fs::remove_all(testDirName);
    }

    void TearDown() override {
        fs::remove_all(testDirName);
    }
};

TEST_F(DirectoryTest, CreateDirectoryOnConstruction) {
    Directory dir(testDirName);
    EXPECT_TRUE(fs::exists(testDirName));
    EXPECT_TRUE(fs::is_directory(testDirName));
}

TEST_F(DirectoryTest, OpenDirectoryLoadsExistingFiles) {
    Directory dir(testDirName);

    std::ofstream(testDirName + "/file1.txt").put('a');
    std::ofstream(testDirName + "/file2.txt").put('b');

    dir.open();

    auto files = dir.getFileNames();

    EXPECT_NE(std::find(files.begin(), files.end(), "file1.txt"), files.end());
    EXPECT_NE(std::find(files.begin(), files.end(), "file2.txt"), files.end());
}

TEST_F(DirectoryTest, AddFileCreatesFileAndUpdatesList) {
    Directory dir(testDirName);
    dir.open();

    dir.addFile("newfile.txt");

    EXPECT_TRUE(fs::exists(testDirName + "/newfile.txt"));

    auto files = dir.getFileNames();
    EXPECT_NE(std::find(files.begin(), files.end(), "newfile.txt"), files.end());
}

TEST_F(DirectoryTest, DeleteFileRemovesFileAndUpdatesList) {
    Directory dir(testDirName);
    dir.open();

    dir.addFile("todelete.txt");
    EXPECT_TRUE(fs::exists(testDirName + "/todelete.txt"));

    dir.deleteFile("todelete.txt");
    EXPECT_FALSE(fs::exists(testDirName + "/todelete.txt"));

    auto files = dir.getFileNames();
    EXPECT_EQ(std::find(files.begin(), files.end(), "todelete.txt"), files.end());
}

TEST_F(DirectoryTest, RemoveDirectoryDeletesAllContents) {
    Directory dir(testDirName);

    std::ofstream(testDirName + "/file1.txt").put('a');
    std::ofstream(testDirName + "/file2.txt").put('b');

    dir.open();
    EXPECT_TRUE(fs::exists(testDirName));

    dir.remove();
    EXPECT_FALSE(fs::exists(testDirName));
}

TEST_F(DirectoryTest, SearchFileReturnsCorrectFile) {
    Directory dir(testDirName);
    dir.open();

    dir.addFile("findme.txt");

    File& f = dir.searchFile("findme.txt");
    EXPECT_EQ(f.getName(), "findme.txt");
}

TEST_F(DirectoryTest, SearchFileThrowsIfNotFound) {
    Directory dir(testDirName);
    dir.open();

    EXPECT_THROW(dir.searchFile("notfound.txt"), std::runtime_error);
}
