hyperplaform_log_parser
========================

Introduction
-------------

hyperplaform_log_parser is a user-mode program parsing logs created by
HyperPlatform. hyperplaform_log_parser monitors updates in a log file and parses
log lines if any new entries are added. Also, hyperplaform_log_parser is able to
resolve symbol names if possible.

Currently, hyperplaform_log_parser supports logs from MemoryMon.


Installation and Uninstallation
--------------------------------

Run hyperplaform_log_parser.exe with a path to a log file. For example,

    > hyperplaform_log_parser.exe C:\Windows\MemoryMon.log

hyperplaform_log_parser.exe is able to use symbol information if a specified log
file is created by the current boot session, internet connection is available
and accompaning Windbg DLLs (dbghelp.dll and symsrv.dll) are stored in the same
folder as hyperplaform_log_parser.exe.


Output
-------

All logs are printed out to STDIO. There is an example of outout with symbol
information:
    
    08:47:07.276     4:    0 executed fffffa800194a468, will return to fffff80002ad8c1c nt!KiRetireDpcList+0x1bc
    08:47:07.276     4:    0 executed fffffa8003d46007, will return to fffffa800194a4ad
    08:47:07.276     4:    0 executed fffffa8003d47580, will return to fffffa8003d460b0
    08:47:07.291     4:   64 executed fffffa8003d4ae1c, will return to fffff80002ad7b69 nt!ExpWorkerThread+0x111
    08:45:07.265     4:   64 executed fffffa8002626629, will return to                4


Supported Platforms
----------------------
- x86 and x64 Windows 7, 8.1 and 10


License
--------
This software is released under the MIT License, see LICENSE.
