// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

//
// This module implements an entry point of the driver.
//

#include "stdafx.h"
#include "log_parser.h"
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

bool AppMain(_In_ const std::vector<std::basic_string<TCHAR>> &args);

////////////////////////////////////////////////////////////////////////////////
//
// variables
//

////////////////////////////////////////////////////////////////////////////////
//
// implementations
//
//

int _tmain(int argc, TCHAR *argv[]) {
  auto exit_code = EXIT_FAILURE;
  try {
    std::vector<std::basic_string<TCHAR>> args;
    for (auto i = 0; i < argc; ++i) {
      args.push_back(argv[i]);
    }
    if (AppMain(args)) {
      exit_code = EXIT_SUCCESS;
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Unhandled exception occurred." << std::endl;
  }
  return exit_code;
}

// A main application loop
_Use_decl_annotations_ bool AppMain(
    const std::vector<std::basic_string<TCHAR>> &args) {
  if (args.size() == 1) {
    std::cout << "Usage:\n"
              << "  >this.exe <log_file_path>\n" << std::endl;
    return false;
  }

  LogParser log_parser(args.at(1));
  for (;;) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    log_parser.ParseFile();
  }
}
