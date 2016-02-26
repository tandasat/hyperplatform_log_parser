// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#ifndef HYPERPLATFORM_LOG_PARSER_UTILITY_H_
#define HYPERPLATFORM_LOG_PARSER_UTILITY_H_

#include <string>
#include <vector>

namespace utility {
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

std::basic_string<TCHAR> GetWin32ErrorMessage(_In_ std::uint32_t error_code);

void PrintErrorMessage(_In_ const std::basic_string<TCHAR> &message);

[[noreturn]] void ThrowRuntimeError(
    _In_ const std::basic_string<TCHAR> &message);

std::wstring StrToWStr(_In_ const std::string &str);

std::string WStrToStr(_In_ const std::wstring &Wstr);

////////////////////////////////////////////////////////////////////////////////
//
// variables
//

////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

template <typename T>
std::vector<std::basic_string<T>> Split(_In_ const std::basic_string<T> &str,
                                        _In_ T delim) {
  std::vector<std::basic_string<T>> elems;
  std::basic_stringstream<T> ss(str);
  std::basic_string<T> item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

}  // End of namespace utility

#endif  // HYPERPLATFORM_LOG_PARSER_UTILITY_H_
