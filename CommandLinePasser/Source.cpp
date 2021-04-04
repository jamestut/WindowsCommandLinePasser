#include <Windows.h>
#include <stdio.h>
#include <malloc.h>
#include <string>
#include <iostream>
#include <fstream>

#define CONFIGFILE L"parampasscfg.txt"
#define MAX_ARG_SIZE 32767

#define BAIL(msg) {showError((msg)); return 1;}

#define ERR_MSG_CWD L"Error getting CWD."
#define ERR_MSG_MEM_ALLOC L"Error allocating memory."

void showError(LPCWSTR msg) {
	MessageBox(0, msg, L"Error", MB_ICONERROR);
}

size_t readLine(FILE* f, wchar_t* ret, size_t retSize) {
	if (!fgetws(ret, retSize, f))
		return 0;
	auto sz = wcslen(ret) - 1;
	if (ret[sz] == L'\n') {
		ret[sz--] = 0;
		if ((sz >= 0) && (ret[sz] == L'\r'))
			ret[sz--] = 0;
	}
	return sz + 1;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	std::wstring cwd;
	{
		wchar_t cwdr[MAX_PATH];
		auto len = GetModuleFileName(NULL, cwdr, MAX_PATH);
		for (int i = len - 1; i >= 0; --i) {
			if (cwdr[i] == L'\\') {
				cwdr[i + 1] = 0;
				break;
			}
		}
		cwd = cwdr;
	}
	
	// read config file
	FILE* cfgFile;
	auto cfgFilePath = cwd + CONFIGFILE;
	if (_wfopen_s(&cfgFile, cfgFilePath.c_str(), L"r")) {
		wchar_t errMsg[64];
		wsprintf(errMsg, L"Error opening configuration file '%ls'.", CONFIGFILE);
		BAIL(errMsg);
	}

	// determine file path
	std::wstring path;
	{
		wchar_t targetName[MAX_PATH];
		size_t targetNameLen;
		if (!(targetNameLen = readLine(cfgFile, targetName, MAX_PATH)))
			BAIL(L"Cannot read target program name.");
		// absolute or relative?
		bool targetAbsolute = (targetNameLen >= 3) && (targetName[1] == L':') && ((targetName[0] >= L'A' && targetName[0] <= 'Z') || (targetName[0] >= L'A' && targetName[0] <= 'Z'));
		path = targetAbsolute ? targetName : (cwd + targetName);
	}

	static wchar_t passedArgs[MAX_ARG_SIZE];
	size_t passedArgLen = readLine(cfgFile, passedArgs, MAX_ARG_SIZE);

	// append
	if (pCmdLine[0]) {
		// add space before append
		passedArgs[passedArgLen] = L' ';
		wcscpy_s(passedArgs + passedArgLen + 1, MAX_ARG_SIZE - passedArgLen - 1, pCmdLine);
	}
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	if (!CreateProcess(path.c_str(), passedArgs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		BAIL(L"Error starting program.");

	return 0;
}