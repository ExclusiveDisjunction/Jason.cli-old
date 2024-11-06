//
// Created by exdisj on 8/7/24.
//

#ifndef JASON_PACKAGEENTRY_H
#define JASON_PACKAGEENTRY_H

#include <string>
#include <filesystem>
#include <optional>

#include "PackageEntryKey.h"
#include "PackageEntryIndex.h"
#include "../Calc/VariableType.h"

class PackageReference;
class Package;

class PackageEntry
{
private:
    std::optional<std::unique_ptr<VariableType>> data = {};
    std::weak_ptr<PackageReference> parent;
    PackageEntryIndex index;
    bool thisModified = false;

    void SetModified(bool New) noexcept
    {
        this->thisModified = New;
        this->index.IsModified(New);
    }

public:
    PackageEntry(PackageEntryIndex&& index, std::weak_ptr<PackageReference> parent);
    PackageEntry(const PackageEntry& obj) = delete;
    PackageEntry(PackageEntry&& obj) = default;
    ~PackageEntry();

    friend class Package;

    PackageEntry& operator=(const PackageEntry& obj) = delete;
    PackageEntry& operator=(PackageEntry&& obj) noexcept = default;

    /// @breif Writes the data of the Entry if it is loaded, fails if otherwise.
    [[nodiscard]] bool WriteData(PackagePager& pager) noexcept;
    [[nodiscard]] bool DisplayData(std::ostream& out) const noexcept;
    /// @brief  Writes the data of the Entry if it is loaded to the path at GetPath(), fails if otherwise. 
    [[nodiscard]] bool WriteData() noexcept;

    /// @brief  Reads from a specified input stream, only looking for the data. 
    void Load(std::istream& in);
    /// @brief Reads from the path located at GetPath(), only looking for the data. If the path could not be resolves, it attempts to create it. If it fails to create, it will return false, otherwise, data will be nullptr. 
    void Load();
    [[nodiscard]] bool LoadNoThrow(std::string& errorMessage) noexcept;
    /// @brief Removes the 'Data' item from memory without deleting the file.
    void Unload() noexcept;
    /// @brief Deletes 'Data' from memory & the file system.
    void Reset() noexcept;

    [[nodiscard]] const VariableType& Data() const;
    [[nodiscard]] bool Data(std::unique_ptr<VariableType>&& New) noexcept;

    [[nodiscard]] std::optional<bool> HasData() const noexcept;
    [[nodiscard]] bool IsModified() const noexcept;

    void LoadImmediate(bool New) noexcept;
    void IsReadOnly(bool New) noexcept;
    
    [[nodiscard]] std::filesystem::path GetPath() const;
    [[nodiscard]] const PackageEntryIndex& GetIndex() const noexcept;
};


#endif //JASON_PACKAGEENTRY_H
