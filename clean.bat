@echo off
del *.sdf
del /s *.aps
del /a:h *.suo
rmdir /s /q .vs
rmdir /s /q ipch
rmdir /s /q Debug
rmdir /s /q Release
rmdir /s /q x64
rmdir /s /q hyperplatform_log_parser\Debug
rmdir /s /q hyperplatform_log_parser\Release
rmdir /s /q hyperplatform_log_parser\x64
pause
