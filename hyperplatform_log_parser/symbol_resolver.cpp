// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//
#include "stdafx.h"
#include "symbol_resolver.h"
#include "utility.h"

////////////////////////////////////////////////////////////////////////////////
//
// macro utilities
//

// 'to_string' macro to support to switch between ANSI and UNICODE builds.
#define SYMBOLRESOLVER_STRINGIFY(x) #x
#define SYMBOLRESOLVER_TOSTRING(x) SYMBOLRESOLVER_STRINGIFY(x)

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

/*_Use_decl_annotations_*/ SymbolResolver::SymbolResolver()
    : dbghelp_(std::experimental::make_unique_resource(
          ::LoadLibrary(TEXT("DbgHelp.dll")), &::FreeLibrary)),
      SymInitialize_(reinterpret_cast<SymInitializeType>(
          ::GetProcAddress(dbghelp_, SYMBOLRESOLVER_TOSTRING(SymInitialize)))),
      SymCleanup_(reinterpret_cast<SymCleanupType>(
          ::GetProcAddress(dbghelp_, SYMBOLRESOLVER_TOSTRING(SymCleanup)))),
      SymSetOptions_(reinterpret_cast<SymSetOptionsType>(
          ::GetProcAddress(dbghelp_, SYMBOLRESOLVER_TOSTRING(SymSetOptions)))),
      SymGetOptions_(reinterpret_cast<SymGetOptionsType>(
          ::GetProcAddress(dbghelp_, SYMBOLRESOLVER_TOSTRING(SymGetOptions)))),
      SymFromName_(reinterpret_cast<SymFromNameType>(
          ::GetProcAddress(dbghelp_, SYMBOLRESOLVER_TOSTRING(SymFromName)))),
      SymFromAddr_(reinterpret_cast<SymFromAddrType>(
          ::GetProcAddress(dbghelp_, SYMBOLRESOLVER_TOSTRING(SymFromAddr)))),
      SymLoadModuleEx_(reinterpret_cast<SymLoadModuleExType>(::GetProcAddress(
          dbghelp_, SYMBOLRESOLVER_TOSTRING(SymLoadModuleEx)))),
      SymUnloadModule64_(
          reinterpret_cast<SymUnloadModule64Type>(::GetProcAddress(
              dbghelp_, SYMBOLRESOLVER_TOSTRING(SymUnloadModule64)))),
      SymGetSearchPath_(reinterpret_cast<SymGetSearchPathType>(::GetProcAddress(
          dbghelp_, SYMBOLRESOLVER_TOSTRING(SymGetSearchPath)))),
      process_(std::experimental::make_unique_resource(
          ::GetCurrentProcess(), *const_cast<SymCleanupType *>(&SymCleanup_))) {
  if (!dbghelp_ || !SymInitialize_ || !SymCleanup_ || !SymSetOptions_ ||
      !SymGetOptions_ || !SymFromName_ || !SymFromAddr_ || !SymLoadModuleEx_ ||
      !SymUnloadModule64_ || !SymGetSearchPath_) {
    utility::ThrowRuntimeError(
        TEXT("At least one of DbgHelp APIs was not initialized."));
  }

  TCHAR dbghelp_path[MAX_PATH];
  ::GetModuleFileName(dbghelp_.get(), dbghelp_path, _countof(dbghelp_path));

  const auto original_pptions = SymGetOptions_();
  SymSetOptions_(original_pptions | SYMOPT_CASE_INSENSITIVE | SYMOPT_DEBUG |
                 SYMOPT_UNDNAME | SYMOPT_AUTO_PUBLICS | SYMOPT_DEFERRED_LOADS);

  // Use a predefined symbols search path if _NT_SYMBOL_PATH is not registered
  const auto ret =
      ::GetEnvironmentVariable(TEXT("_NT_SYMBOL_PATH"), nullptr, 0);
  TCHAR *path = nullptr;
  if (ret == 0 && ::GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
    path = TEXT(".;srv*.\\Symbols*http://msdl.microsoft.com/download/symbols");
  }

  if (!SymInitialize_(process_, path, FALSE)) {
    utility::ThrowRuntimeError(TEXT("SymInitialize failed."));
  }
}

// Load a module and return its base address
_Use_decl_annotations_ std::uint64_t SymbolResolver::LoadModule(
    const std::basic_string<TCHAR> &module_path) {
  return SymLoadModuleEx_(process_, nullptr, module_path.c_str(), nullptr, 0, 0,
                          nullptr, 0);
}

// Unload a module
_Use_decl_annotations_ bool SymbolResolver::UnloadModule(
    _In_ std::uint64_t BaseAddress) {
  return (SymUnloadModule64_(process_, BaseAddress) != FALSE);
}

// Return an offset of the symbols from the base address of the module
_Use_decl_annotations_ std::uint64_t SymbolResolver::GetOffset(
    const std::basic_string<TCHAR> &symbol_name) const {
  std::uint8_t buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = {};
  auto symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  symbol->MaxNameLen = MAX_SYM_NAME;

  if (!SymFromName_(process_, symbol_name.c_str(), symbol)) {
    return 0;
  }
  return symbol->Address - symbol->ModBase;
}

// Return a symbol name from the address or an empty string on error
_Use_decl_annotations_ std::basic_string<TCHAR> SymbolResolver::GetName(
    std::uint64_t address) const {
  std::uint8_t buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = {};
  auto symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  symbol->MaxNameLen = MAX_SYM_NAME;

  std::uint64_t displacement = 0;
  if (!SymFromAddr_(process_, address, &displacement, symbol)) {
    return {};
  }
  if (!displacement) {
    return symbol->Name;
  }
  std::basic_stringstream<TCHAR> stream;
  stream << "+0x" << std::nouppercase << std::hex << displacement;
  std::basic_string<TCHAR> result(stream.str());
  return symbol->Name + result;
}

// Delete these macros
#undef SYMBOLRESOLVER_STRINGIFY
#undef SYMBOLRESOLVER_TOSTRING
