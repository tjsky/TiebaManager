#pragma once

void Delay(DWORD time);
void DoEvents();
LONG WINAPI ExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo);
void CreateDir(const CString& path);
