//
// Created by exdisj on 8/8/24.
//

#ifndef JASON_SESSION_H
#define JASON_SESSION_H

#include <optional>
#include <filesystem>

#include "PackageUtility.h"
#include "Package.h"
#include "UnloadedPackage.h"

class Session
{
private:
    std::vector<std::shared_ptr<Package>> packages;
    std::vector<UnloadedPackage> unloadedPackages;
    unsigned long currID;
    bool initiated = false;
    
    using pack_lt = std::vector<std::shared_ptr<Package>>;
    using upack_lt = std::vector<UnloadedPackage>;
    using cp_iter = pack_lt::const_iterator;
    using p_iter = pack_lt::iterator;
    using cu_iter = upack_lt::const_iterator;
    using u_iter = upack_lt::iterator;
    
    [[nodiscard]] unsigned long GetNextID() noexcept { return currID++; }
    
    [[nodiscard]] cp_iter ResolvePackage(const std::string& name) const noexcept;
    [[nodiscard]] cp_iter ResolvePackage(unsigned long ID) const noexcept;
    [[nodiscard]] p_iter ResolvePackage(unsigned long ID) noexcept;

    [[nodiscard]] cu_iter ResolveUnloadedPackage(const std::string& name) const noexcept;
    [[nodiscard]] cu_iter ResolveUnloadedPackage(unsigned long ID) const noexcept;
    [[nodiscard]] u_iter ResolveUnloadedPackage(unsigned long ID) noexcept;

    Session();
public:
    Session(const Session& obj) = delete;
    Session(Session&& obj) noexcept;
    ~Session();

    Session& operator=(const Session& obj) = delete;
    Session& operator=(Session&& obj) noexcept;
    
    std::optional<std::unique_ptr<Session>> StartSession(std::filesystem::path host);
    void Shutdown();
    bool SaveAndClose();
    [[nodiscard]] bool Save() const noexcept;

    //[[nodiscard]] static std::string MakePackageName(const std::filesystem::path& target) noexcept;
    
    [[nodiscard]] std::optional<std::shared_ptr<Package>> GetPackage(const std::string& name) const noexcept;
    [[nodiscard]] std::optional<unsigned long> GetPackageID(const std::string& name) const noexcept;
    
    [[nodiscard]] std::optional<PackageEntryKey> ResolveEntryKey(const std::string& name) const noexcept;
    [[nodiscard]] const PackageEntry& ResolveEntry(const std::string& name) const;
    [[nodiscard]] PackageEntry& ResolveEntry(const std::string& name);

    [[nodiscard]] bool IsPackageLoaded(unsigned long ID) const noexcept;
    [[nodiscard]] bool LoadPackage(unsigned long ID) noexcept;
    [[nodiscard]] bool UnloadPackage(unsigned long ID) noexcept;
    [[nodiscard]] bool UnloadPackage(std::shared_ptr<Package> Pack) noexcept;
};

#endif //JASON_SESSION_H
