// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#include "stdafx.h"
#include "log_line_parser.h"
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

bool LogLineParser::Parse(const std::string &log_line) {
  if (log_line.empty()) {
    return false;
  }

  // Split a log into some basic elements
  const auto log_elements = utility::Split(log_line, '\t');
  time_ = log_elements[0];
  pid_ = std::stoul(log_elements[3]);
  tid_ = std::stoul(log_elements[4]);
  message_ = log_elements[6];

  // Get a digit for a pointer
  SYSTEM_INFO system_info = {};
  GetNativeSystemInfo(&system_info);
  pointer_size_ =
      (system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
          ? "16"
          : "8";

  if (TryParseExecutionLog()) {
    return true;
  }
  return false;
}

// Attempts to parse a line reported for dodgy region execution
bool LogLineParser::TryParseExecutionLog() {
  // clang-format off
  std::regex regex(
    "\\[EXEC\\] \\*\\*\\* "
    "VA = ([[:xdigit:]]{" + pointer_size_ + "}), "
    "PA = [[:xdigit:]]{16}, "
    "Return = ([[:xdigit:]]{" + pointer_size_ + "}), "
    "ReturnBase = ([[:xdigit:]]{" + pointer_size_ + "})");
  // clang-format on

  std::smatch match;
  if (!std::regex_match(message_, match, regex)) {
    return false;
  }

  // Successfully parsed. Print it to STDIO
  const auto executed_address = std::stoull(match[1].str(), nullptr, 16);
  const auto return_address = std::stoull(match[2].str(), nullptr, 16);
  const auto return_base_address = std::stoull(match[3].str(), nullptr, 16);
  const auto symbol =
      driver_symbol_resolver_.GetName(return_address, return_base_address);
  const std::string format_string = "%s %5u:%5u executed %" + pointer_size_ +
                                    "llx, will return to %" + pointer_size_ +
                                    "llx %S\n";
  printf(format_string.c_str(), time_.c_str(), pid_, tid_, executed_address,
         return_address, symbol.c_str());
  return true;
}
