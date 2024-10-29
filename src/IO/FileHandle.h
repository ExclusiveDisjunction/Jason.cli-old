//
// Created by exdisj on 8/13/24.
//

#ifndef JASON_PACKAGEHANDLE_H
#define JASON_PACKAGEHANDLE_H

#include <filesystem>
#include <fstream>

#include "../Core/Result.h"

struct FileHandle
{
public:
    FileHandle() : path(), file() {}
    explicit FileHandle(std::filesystem::path path, std::ios::openmode flags = (std::ios::out | std::ios::in));
    FileHandle(const FileHandle& obj) = delete;
    FileHandle(FileHandle&& obj) noexcept;
    ~FileHandle();

    [[nodiscard]] static Result<FileHandle, std::string> TryOpen(std::filesystem::path path, std::ios::openmode flags = (std::ios::out | std::ios::in));

    FileHandle& operator=(const FileHandle& obj) noexcept = delete;
    FileHandle& operator=(FileHandle&& obj) noexcept;

    void Open(std::filesystem::path path, std::ios::openmode flags = (std::ios::out | std::ios::in));
    void Close() noexcept;

    std::filesystem::path path;
    std::fstream file;
};

#endif //JASON_PACKAGEHANDLE_H
