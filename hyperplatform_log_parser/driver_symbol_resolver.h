// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#ifndef HYPERPLATFORM_LOG_PARSER_DRIVER_SYMBOL_RESOLVER_H_
#define HYPERPLATFORM_LOG_PARSER_DRIVER_SYMBOL_RESOLVER_H_

#include <tchar.h>
#include <string>
#include <memory>
#include <utility>
#include <chrono>
#include <vector>
#include "symbol_resolver.h"

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

class DriverSymbolResolver {
 public:
  std::basic_string<TCHAR> GetName(std::uint64_t address,
                                   std::uint64_t base_address);

 private:
  using DriverInfo = std::pair<std::uintptr_t, std::basic_string<TCHAR>>;
  using DriverInfoList = std::vector<DriverInfo>;

  bool UpdateDriverList();
  static DriverInfoList GetDriverList();

  SymbolResolver symbol_resolver_;
  DriverInfoList driver_list_;
  std::chrono::time_point<std::chrono::system_clock> list_updated_time_;
  std::uint64_t symbol_module_base_;
  std::uint64_t driver_base_;
};

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

#endif  // HYPERPLATFORM_LOG_PARSER_DRIVER_SYMBOL_RESOLVER_H_
