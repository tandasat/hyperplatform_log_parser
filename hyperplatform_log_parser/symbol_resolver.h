// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module declares interfaces of a SymbolResolver class.
//

#ifndef HYPERPLATFORM_LOG_PARSER_SYMBOL_RESOLVER_H_
#define HYPERPLATFORM_LOG_PARSER_SYMBOL_RESOLVER_H_

#include <tchar.h>
#include <cstdint>
#include <string>

// Support UNICODE build for DbgHelp.h
#if defined(UNICODE)
#define DBGHELP_TRANSLATE_TCHAR
#endif

// Note from MSDN:
//      All DbgHelp functions, such as this one, are single threaded.
//      Therefore, calls from more than one thread to this function will
//      likely result in unexpected behavior or memory corruption.
//      To avoid this, you must synchronize all concurrent calls
//      from more than one thread to this function.
#pragma warning(push)
#pragma warning(disable : 4091)
#include <DbgHelp.h>
#pragma warning(pop)

// Original headers
#include "scoped_resource/scope_exit.h"
#include "scoped_resource/unique_resource.h"

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

class SymbolResolver {
 public:
  SymbolResolver();

  std::uint64_t LoadModule(_In_ const std::basic_string<TCHAR> &module_path);

  bool UnloadModule(_In_ std::uint64_t base_address);

  std::uint64_t GetOffset(
      _In_ const std::basic_string<TCHAR> &symbol_name) const;

  std::basic_string<TCHAR> GetName(_In_ std::uint64_t address) const;

 private:
  using SymInitializeType = decltype(&SymInitialize);
  using SymCleanupType = decltype(&SymCleanup);
  using SymSetOptionsType = decltype(&SymSetOptions);
  using SymGetOptionsType = decltype(&SymGetOptions);
  using SymFromNameType = decltype(&SymFromName);
  using SymFromAddrType = decltype(&SymFromAddr);
  using SymLoadModuleExType = decltype(&SymLoadModuleEx);
  using SymUnloadModule64Type = decltype(&SymUnloadModule64);
  using SymGetSearchPathType = decltype(&SymGetSearchPath);

  std::experimental::unique_resource<HMODULE, decltype(&::FreeLibrary)>
      dbghelp_;
  const SymInitializeType SymInitialize_;
  const SymCleanupType SymCleanup_;
  const SymSetOptionsType SymSetOptions_;
  const SymGetOptionsType SymGetOptions_;
  const SymFromNameType SymFromName_;
  const SymFromAddrType SymFromAddr_;
  const SymLoadModuleExType SymLoadModuleEx_;
  const SymUnloadModule64Type SymUnloadModule64_;
  const SymGetSearchPathType SymGetSearchPath_;
  const std::experimental::unique_resource<HANDLE, SymCleanupType> process_;
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

#endif  // HYPERPLATFORM_LOG_PARSER_SYMBOL_RESOLVER_H_
