// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements a SymbolResolver class.
//

#include "stdafx.h"
#include "log_parser.h"

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

LogParser::LogParser(const std::basic_string<TCHAR>& log_path)
    : log_path_(log_path) {}

// Opens a log file and checks if the length is the same as the last check. If
// not, parse newly added lines.
bool LogParser::ParseFile() {
  std::ifstream log_file(log_path_);
  if (!log_file) {
    return false;
  }
  log_file.seekg(0, log_file.end);
  const auto end_position = log_file.tellg();
  if (end_position < old_position_) {
    old_position_ = 0;
  }
  if (end_position == old_position_) {
    return false;
  }

  // Parse new lines
  log_file.seekg(old_position_, log_file.beg);
  std::string log_line;
  while (!log_file.eof()) {
    std::getline(log_file, log_line);
    log_line_parser_.Parse(log_line);
  }
  old_position_ = end_position;
  return true;
}
