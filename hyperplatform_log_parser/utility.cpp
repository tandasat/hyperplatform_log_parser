// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#include "stdafx.h"
#include "utility.h"
#include "scoped_resource/unique_resource.h"
#include "scoped_resource/scope_exit.h"

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

////////////////////////////////////////////////////////////////////////////////
//
// variables
//

////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

// Return an error message of corresponding Win32 error code.
_Use_decl_annotations_ std::basic_string<TCHAR> GetWin32ErrorMessage(
    std::uint32_t error_code) {
  TCHAR *message_raw = nullptr;
  if (!::FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr,
          error_code, LANG_USER_DEFAULT, reinterpret_cast<LPTSTR>(&message_raw),
          0, nullptr)) {
    return TEXT("");
  }
  if (!message_raw) {
    return TEXT("");
  }
  auto message = std::experimental::make_unique_resource(std::move(message_raw),
                                                         &LocalFree);

  const auto length = ::_tcslen(message.get());
  if (!length) {
    return TEXT("");
  }

  if (message.get()[length - 2] == TEXT('\r')) {
    message.get()[length - 2] = TEXT('\0');
  }
  return message.get();
}

// Display an error message with an error message of the current error code.
_Use_decl_annotations_ void PrintErrorMessage(
    const std::basic_string<TCHAR> &message) {
  const auto error_code = ::GetLastError();
  const auto error_message = GetWin32ErrorMessage(error_code);
  ::_ftprintf_s(stderr, _T("%s : %lu(0x%08x) : %s\n"), message.c_str(),
                error_code, error_code, error_message.c_str());
}

// Throw std::runtime_error with an error message.
_Use_decl_annotations_ void ThrowRuntimeError(
    const std::basic_string<TCHAR> &message) {
  const auto error_code = ::GetLastError();
  const auto error_message = GetWin32ErrorMessage(error_code);
  char msg[1024];
#if UNICODE
  static const char kFormatString[] = "%S : %lu(0x%08x) : %S";
#else
  static const char kFormatString[] = "%s : %lu(0x%08x) : %s";
#endif
  StringCchPrintfA(msg, _countof(msg), kFormatString, message.c_str(),
                   error_code, error_code, error_message.c_str());
  throw std::runtime_error(msg);
}

_Use_decl_annotations_ std::wstring StrToWStr(const std::string &str) {
  typedef std::codecvt_utf8<wchar_t> convert_typeX;
  std::wstring_convert<convert_typeX, wchar_t> converterX;

  return converterX.from_bytes(str);
}

_Use_decl_annotations_ std::string WStrToStr(const std::wstring &Wstr) {
  typedef std::codecvt_utf8<wchar_t> convert_typeX;
  std::wstring_convert<convert_typeX, wchar_t> converterX;

  return converterX.to_bytes(Wstr);
}

}  // End of namespace utility
