// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#include "stdafx.h"
#include "driver_symbol_resolver.h"
#include "utility.h"

////////////////////////////////////////////////////////////////////////////////
//
// macro utilities
//

////////////////////////////////////////////////////////////////////////////////
//
// constants and macros
//

////////////////////////////////////////////////////////////////////////////////
//
// types
//

////////////////////////////////////////////////////////////////////////////////
//
// prototypes
//

////////////////////////////////////////////////////////////////////////////////
//
// variables
//

////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

std::basic_string<TCHAR> DriverSymbolResolver::GetName(
    std::uint64_t address, std::uint64_t base_address) {
  if (!address || !base_address) {
    return {};
  }

  UpdateDriverList();

  for (auto &driver : driver_list_) {
    if (driver.first != base_address) {
      continue;
    }

    // Does it need to update a symbol?
    if (driver.first != driver_base_) {
      symbol_resolver_.UnloadModule(symbol_module_base_);
      symbol_module_base_ = symbol_resolver_.LoadModule(driver.second);
      driver_base_ = driver.first;
    }

    if (symbol_module_base_) {
      const auto offset = address - base_address;
      const auto name = symbol_resolver_.GetName(symbol_module_base_ + offset);
      if (!name.empty()) {
        std::tr2::sys::path path(driver.second);
        auto driverBaseName = path.stem().wstring();
        if (driverBaseName == L"ntoskrnl" || driverBaseName == L"ntkrnlpa") {
          driverBaseName = L"nt";
        }
        return driverBaseName + L"!" + name;
      }
    }
  }
  return {};
}

bool DriverSymbolResolver::UpdateDriverList() {
  const auto now = std::chrono::system_clock::now();
  const auto elapsed_seconds = now - list_updated_time_;
  if (elapsed_seconds.count() > 5) {
    list_updated_time_ = now;
    driver_list_ = GetDriverList();
    return true;
  }
  return false;
}

// Get a list of drivers' base addresses and file paths that are currently
// loaded in the kernel.
DriverSymbolResolver::DriverInfoList DriverSymbolResolver::GetDriverList() {
  // Determine the current number of drivers
  DWORD needed = 0;
  std::array<void *, 1000> base_addresses;
  if (!::EnumDeviceDrivers(
          base_addresses.data(),
          static_cast<DWORD>(base_addresses.size() * sizeof(void *)),
          &needed)) {
    utility::ThrowRuntimeError(TEXT("EnumDeviceDrivers failed."));
  }

  TCHAR system_root[MAX_PATH];
  if (!::ExpandEnvironmentStrings(TEXT("%SystemRoot%"), system_root,
                                  _countof(system_root))) {
    utility::ThrowRuntimeError(TEXT("ExpandEnvironmentStrings failed."));
  }

  // Collect their base names
  DriverInfoList list;
  const auto number_of_drivers = needed / sizeof(base_addresses.at(0));
  for (std::uint32_t i = 0; i < number_of_drivers; ++i) {
    std::array<TCHAR, MAX_PATH> path_buffer;
    if (!::GetDeviceDriverFileName(base_addresses.at(i), path_buffer.data(),
                                   static_cast<DWORD>(path_buffer.size()))) {
      utility::ThrowRuntimeError(TEXT("GetDeviceDriverBaseName failed."));
    }

    std::basic_string<TCHAR> path(path_buffer.data());
    if (path.compare(0, 11, TEXT("\\SystemRoot")) == 0) {
      path.replace(0, 11, system_root);
    } else if (path.compare(0, 4, TEXT("\\??\\")) == 0) {
      path.replace(0, 4, TEXT(""));
    }
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);
    list.emplace_back(reinterpret_cast<std::uintptr_t>(base_addresses.at(i)),
                      path);
  }
  return list;
}
