// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#ifndef HYPERPLATFORM_LOG_PARSER_LOG_LINE_PARSER_H_
#define HYPERPLATFORM_LOG_PARSER_LOG_LINE_PARSER_H_

#include <string>
#include <memory>
#include "driver_symbol_resolver.h"

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

class LogLineParser {
 public:
  bool Parse(const std::string &log_line);

 private:
  bool TryParseExecutionLog();
  bool TryParseRweLog();

  DriverSymbolResolver driver_symbol_resolver_;
  std::string time_;
  std::uint32_t pid_;
  std::uint32_t tid_;
  std::string message_;
  std::string pointer_size_;
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

#endif  // HYPERPLATFORM_LOG_PARSER_LOG_LINE_PARSER_H_
