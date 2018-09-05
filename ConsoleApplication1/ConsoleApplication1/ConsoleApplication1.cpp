// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "PCICL32.h"

HMODULE hMod;

typedef void* (__fastcall vtableOfShit3__ctor) (void* pThis);
typedef void* (__fastcall vTableOfTheShit2__ctor)(void *pThis, DWORD edx, int a2);
typedef void* (__fastcall vTableOfArg2__ctor)(void *pThis, DWORD edx, void* vtos2, void* vtos3, int a4, int a5, DWORD a6, const char** a7, int a8);
typedef ReplayParser_t* (__fastcall ReplayParser__ctor)(void *pThis, DWORD edx, int a2, void* a3, void* a4, int a5);
typedef int(__fastcall ReplayParser__ImportantParseEx)(ReplayParser_t *pThis, DWORD edx, char *filename);
typedef void*(__fastcall MoreBullshitCtor)(void* pThis);
typedef int (__cdecl ReplayParserS2__PlayControlshit)(ReplayParserS2_t *pthis, int message);
typedef int(__cdecl _fclose)(FileDesc_t *pfd);

typedef BOOL (__fastcall ThreadThang__SetEvent)(ThreadThang *pThis);
typedef BOOL (__fastcall ThreadThang__PulseEvent)(ThreadThang *pThis);

#define PLAYCONTROL_PLAY 6613
#define PLAYCONTROL_PAUSE 6614
#define PLAYCONTROL_SEEKL 6616

MoreBullshitCtor* ctor0;
vtableOfShit3__ctor* ctor3;
vTableOfTheShit2__ctor* ctor2;
vTableOfArg2__ctor* ctorArg2;
const char** aClient;
ReplayParser__ctor* ctor;
ReplayParser__ImportantParseEx* fn;
ReplayParserS2__PlayControlshit* playControl;
_fclose* pcicl_fclose;

ThreadThang__SetEvent* pSetEvent;
ThreadThang__PulseEvent* pPulseEvent;

LPTHREAD_START_ROUTINE pcb_ReplayThread;

typedef HANDLE (WINAPI CreateThread_t)(
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ SIZE_T dwStackSize,
	_In_ LPTHREAD_START_ROUTINE lpStartAddress,
	_In_opt_ __drv_aliasesMem LPVOID lpParameter,
	_In_ DWORD dwCreationFlags,
	_Out_opt_ LPDWORD lpThreadId
);

CreateThread_t* oCreateThread;
DWORD jb_CreateThread;
DWORD dwTmp;
ThreadThang* pReplayThreadParam = NULL;

DWORD CALLBACK CbNullThread(LPVOID lpParam)
{
	printf("Nullsub called, lpParam=0x%08x\n", lpParam);
	return 0;
}

DWORD CALLBACK CbNullThreadWithEvent(LPVOID lpParam)
{
	ThreadThang* pThreadThang = (ThreadThang*)lpParam;
	printf("Nullsub+event called, threadThang=0x%08x\n", pThreadThang);
	pSetEvent(pThreadThang);
	pPulseEvent(pThreadThang);
	printf("Pulsed event.\n");

	return 0;
}

LPTHREAD_START_ROUTINE __fastcall hcb_CreateThread(
	DWORD caller,
	_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
	_In_ SIZE_T dwStackSize,
	_In_ LPTHREAD_START_ROUTINE lpStartAddress,
	_In_opt_ __drv_aliasesMem LPVOID lpParameter,
	_In_ DWORD dwCreationFlags,
	_Out_opt_ LPDWORD lpThreadId)
{
	printf("CreateThread from 0x%08x: lpStartAddress=0x%08x, lpParameter=0x%08x\n", caller, lpStartAddress, lpParameter);

	if (lpStartAddress == pcb_ReplayThread) {
		printf("...which is a call to cb_ReplayThread. Saving the lpParameter.\n");
		pReplayThreadParam = (ThreadThang*) lpParameter;
	}

	if (caller == 0x110e0300) // Called from UTIL_CreateThread, we have to deal with the WaitForSingleObject event
		return CbNullThreadWithEvent;
	return CbNullThread;
	//return lpStartAddress;
}

// THICCLIFY

__declspec(naked) void HkCreateThread()
{
	_asm {
		pushad;
		pushfd;

		// args start at esp+28, downwards, left to right
		lea ebp, [esp + 0x28];
		mov ecx, [ebp - 0x4];
		mov edx, [ebp];
		push[ebp + 0x14];
		push[ebp + 0x10];
		push[ebp + 0xc];
		push[ebp + 0x8];
		push[ebp + 0x4];
		//mov eax, [ebp - 0x4];
		//mov[dwTmp], eax;
		call hcb_CreateThread;

		//test eax, eax;
		//jnz allowThread;

		// replace callback with a stub that does nothing
		//lea eax, CbNullThread

		mov [ebp + 0x8], eax; // returned value is substituted threadproc

		//popfd;
		//popad;
		//add esp, 0x1C; // stdcall: 4*6 args + 4 (return address)
		//push dwTmp;
		//ret;

		allowThread:
		popfd;
		popad;

		mov edi, edi; // stolen bytes
		push ebp;
		mov ebp, esp;
		
		push jb_CreateThread;
		ret;
	}
}

DWORD jb_PlayLoop;

void DebugPlayState(ReplayParserSub_t* pRPS) {
	CReplayer* creplayer = pRPS->cReplayer;
	printf("gotoStartAndStop=%d\n", creplayer->goToStartAndStop);
	printf("doReadData=%d\n", creplayer->play);
	printf("importantPlay=%d\n", creplayer->importantPlay);
	printf("importantPlay2=%d\n", creplayer->doTheThang);
	printf("doPlay=%d\n", pRPS->pParentReplayParser->pSub2->doPlay);
	printf("stop=%d\n", creplayer->stop);
	printf("\n");
}

DWORD bAssertionFailed = 0;
int __stdcall hcb_PlayLoop() {
	printf("In our hook :)\n");
	ReplayParserSub_t* pRPS = ((ReplayParserSub_t*)pReplayThreadParam->lpParam);
	CReplayer* creplayer = pRPS->cReplayer;
	DebugPlayState(pRPS);
	if (!pRPS->pParentReplayParser->pSub2->doPlay && !creplayer->doTheThang && !creplayer->importantPlay && !creplayer->play) {
		printf("WE ARE STOPPED.\n");
		creplayer->stop = 1;
	}
	if (bAssertionFailed) {
		printf("Assertion failed earlier.\n");
		creplayer->stop = 1;
	}
	return 0;
}

__declspec(naked) void HkPlayLoop()
{
	__asm {
		pushad;
		pushfd;

		// do ur hook magic here :)
		call hcb_PlayLoop;

		popfd;
		popad;

		mov eax, [esi + 0x6A8]; // stolen bytes
		push jb_PlayLoop;
		ret;
	}
}

DWORD dwMainThreadId;
DWORD eipHandler;

// return nonzero if we should immediately just jump back to the fuzzwrapper epilogue handler
int __stdcall hcb_AssertionFailed(char* szMsg, char* szSrcLine, int code)
{
	printf("Assertion Failed: %s at %s (%d)\n", szMsg, szSrcLine, code);
	bAssertionFailed = 1;
	if (GetCurrentThreadId() == dwMainThreadId) {
		printf("In main thread; skipping to fuzz handler (0x%08x)\n", eipHandler);
		return 1;
	}
	return 0;
}

__declspec(naked) void HkAssertionFailed()
{
	__asm {
		pushad;
		pushfd;

		lea ebp, [esp + 0x28];
		push[ebp + 8];
		push[ebp + 4];
		push[ebp];
		call hcb_AssertionFailed;

		test eax, eax;
		jne gotoHandler

		popfd;
		popad;

		//// stolen bytes
		//push ebp;
		//mov ebp, esp;
		//sub esp, 100;
		//// jump back
		//push jb_AssertionFailed;
		//ret

		ret; //bye bitch
		
gotoHandler:
		jmp[eipHandler];
	}
}

void __stdcall FuzzMain()
{
	void* shit = ctor0(malloc(0x4c));
	*(void**)((DWORD)hMod + 0x1846B8) = shit;
	*(void**)((DWORD)hMod + 0x1846D8) = shit;

	void* vtos3 = ctor3(malloc(8));
	if (!vtos3) {
		printf("ctor3 failed\n");
		return;
	}

	void* vtos2_1 = ctor2(malloc(0x84), NULL, 1);
	if (!vtos2_1) {
		printf("ctor2 failed\n");
		return;
	}

	void* arg2 = ctorArg2(malloc(0x48), NULL, vtos2_1, vtos3, -1, 0, 1, aClient, 0);
	if (!arg2) {
		printf("ctorArg2 failed\n");
		return;
	}

	*(void**)((DWORD)hMod + 0x184714) = arg2;
	void* vtos2_2 = ctor2(malloc(0x84), NULL, 2);
	if (!vtos2_2) {
		printf("ctor2 failed\n");
		return;
	}

	ReplayParser_t* replayParser = ctor(malloc(0x38), NULL, 0, arg2, vtos2_2, 0);
	if (!replayParser) {
		printf("ctor failed\n");
		return;
	}

	printf("ctors called OK!\n");
	fn(replayParser, NULL, R"(C:\Users\User\Desktop\crash3)");
	printf("Parse OK!\n");

	if (!pReplayThreadParam)
	{
		printf("Never saved the cb_ReplayThread lpParam!\n");
		return;
	}

	ReplayParserSub_t* pRPS = ((ReplayParserSub_t*)pReplayThreadParam->lpParam);
	CReplayer* creplayer = pRPS->cReplayer;
	if (!creplayer) {
		printf("creplayer null\n");
		goto ret;
	}
	creplayer->goToStartAndStop = 0;

	ReplayParserS2_t* pRPS2 = creplayer->replayParserSub->pParentReplayParser->pSub2;
	printf("pRPS2 = %x\n", pRPS2);
	if (!pRPS2) {
		printf("pRPS2 NULL\n");
		goto ret;
	}

	int result = playControl(pRPS2, PLAYCONTROL_PLAY);
	printf("playControl returned %d\n", result);
	if (bAssertionFailed) {
		printf("Assertion failed earlier.\n");
		goto ret;
	}
	DebugPlayState(pRPS);

	result = playControl(pRPS2, PLAYCONTROL_SEEKL);
	printf("playControl returned %d\n", result);
	if (bAssertionFailed) {
		printf("Assertion failed earlier.\n");
		goto ret;
	}
	DebugPlayState(pRPS);

	pReplayThreadParam->hEventWait = CreateEventA(0, 0, 0, 0); // renew NULLed hEventWait, gets NULLed at end of UTIL_CreateThread
	//CreateThread(NULL, 0, pcb_ReplayThread, pReplayThreadParam, 0, NULL);
	pcb_ReplayThread(pReplayThreadParam);
	printf("Replay thread exited\n");
	
ret:
	if (creplayer) {
		FileDesc_t *pfd = creplayer->pfd;
		if (pfd)
		{
			pcicl_fclose(pfd);
			printf("Force closed the file.\n");
		}
	}
}

DWORD espHandler;

void __declspec(naked) FuzzWrapper()
{
	// why use structured exception handlers when you can just reinvent the wheel?
	__asm {
		pushad;
		pushfd;
		mov[espHandler], esp;
		mov[eipHandler], offset epilogue;

		call FuzzMain;

epilogue:
		mov esp, [espHandler];
		popfd;
		popad;
		ret;
	}
}

void __cdecl FuzzMe()
{
	dwMainThreadId = GetCurrentThreadId();
	FuzzWrapper();
}

// look: a wheel, shiny and clean
// just like object_write_bencode_c_value :)
int InstallTrampoline(DWORD dwTarget, DWORD dwHook, DWORD cStolenBytes, DWORD* pJumpbackAddr) {
	if (cStolenBytes < 5)
		ExitProcess(1);

	DWORD dwOldProtect;
	BOOL bSuccess = VirtualProtect((LPVOID)dwTarget, cStolenBytes, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	if (!bSuccess) {
		printf("Failed to VirtualProtect 0x%08x!\n", dwTarget);
		ExitProcess(1);
	}

	*(BYTE*)(dwTarget) = 0xE9;
	DWORD srcVa = (DWORD)dwTarget;
	*(DWORD*)((DWORD)dwTarget + 1) = (dwHook - srcVa - 5); // hook
	if (cStolenBytes > 5) { // nop rest
		memset((void*)(dwTarget + 5), 0x90, cStolenBytes - 5);
	}

	VirtualProtect(oCreateThread, cStolenBytes, dwOldProtect, &dwOldProtect);
	if (pJumpbackAddr) {
		*pJumpbackAddr = dwTarget + cStolenBytes;
	}
	return TRUE;
}

DWORD jb_GetTickCount;

int __stdcall hcb_GetTickCount(DWORD dwTime) {
	printf("GetTickCount() returned %d\n", dwTime);
	dwTime = 0;
	printf("Really returning %d\n", dwTime);
	return dwTime;
}

void __declspec(naked) HkGetTickCount() {
	__asm {
		pushad;
		pushfd;

		push offset getTickCountRet;
		mov edi, edi; // stolen bytes
		push ebp;
		mov ebp, esp;
		jmp[jb_GetTickCount];
	getTickCountRet:
		push eax;
		call hcb_GetTickCount;
		xchg[dwTmp], eax;

		popfd;
		popad;
		xchg[dwTmp], eax;
		ret;
	}
}

DWORD jb_timeGetTime;

int __stdcall hcb_timeGetTime(DWORD dwTime) {
	printf("timeGetTime() returned %d\n", dwTime);
	dwTime = 0;
	printf("Really returning %d\n", dwTime);
	return dwTime;
}

void __declspec(naked) HkTimeGetTime() {
	__asm {
		pushad;
		pushfd;

		push offset timeGetTimeRet; // our return address
		push 0; // stolen bytes
		push 0;
		push 0x756A1D70;
		jmp[jb_timeGetTime]; // call
	timeGetTimeRet:
		push eax;
		call hcb_timeGetTime;
		xchg[dwTmp], eax;

		popfd;
		popad;
		xchg[dwTmp], eax;
		ret; // return to original return address
	}
}

DWORD jb_DoSomething;

int __fastcall hcb_DoSomething(ReplayParserSub_t *pthis, DWORD edx, char *pBuf, size_t bufSize, int flag) {
	printf("DoSomething called: %x %s %d %d\n", pthis, pBuf, bufSize, flag);
	return 0;
}

void __declspec(naked) HkDoSomething() {
	__asm {
		pushad;
		pushfd;

		lea ebp, [esp + 0x28];
		push[ebp + 8];
		push[ebp+4];
		push[ebp];
		call hcb_DoSomething;

		popfd;
		popad;

		push ebp;
		mov ebp, esp;
		push ebx;
		push esi;
		jmp[jb_DoSomething];
	}
}

int WINAPI WinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow
	)
{
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "conout$", "w", stdout);

	InstallTrampoline((DWORD)GetProcAddress(GetModuleHandleA("kernel32"), "timeGetTime"), (DWORD)&HkTimeGetTime, 9, &jb_timeGetTime);
	InstallTrampoline((DWORD)GetProcAddress(GetModuleHandleA("kernelbase"), "GetTickCount"), (DWORD)&HkGetTickCount, 5, &jb_GetTickCount);
	InstallTrampoline((DWORD)GetProcAddress(GetModuleHandleA("kernel32"), "CreateThread"), (DWORD)&HkCreateThread, 5, &jb_CreateThread);

	CreateThread(0, 0, CbNullThread, 0, 0, 0);

	hMod = LoadLibraryA("PCICL32.dll");
	if (!hMod) {
		printf("failed to load %d\n", GetLastError());
		goto fail;
	}
	printf("pcicl32 = 0x%08x\n", hMod);

	pSetEvent = (ThreadThang__SetEvent*)((DWORD)hMod + 0xE0350);
	pPulseEvent = (ThreadThang__PulseEvent*)((DWORD)hMod + 0xE0380);
	pcb_ReplayThread = (LPTHREAD_START_ROUTINE)((DWORD)hMod + 0x597E0);

	ctor0 = (MoreBullshitCtor*)((DWORD)hMod + 0xC52D0);
	ctor3 = (vtableOfShit3__ctor*)((DWORD)hMod + 0x73C30);
	ctor2 = (vTableOfTheShit2__ctor*)((DWORD)hMod + 0x194E0);
	ctorArg2 = (vTableOfArg2__ctor*)((DWORD)hMod + 0x4ABD0);
	aClient = (const char**)((DWORD)hMod + 0x16C3C8);
	ctor = (ReplayParser__ctor*)((DWORD)hMod + 0xEFD90);
	fn = (ReplayParser__ImportantParseEx*)((DWORD)hMod + 0xF0430);
	playControl = (ReplayParserS2__PlayControlshit*)((DWORD)hMod + 0xF1820);
	pcicl_fclose = (_fclose*)((DWORD)hMod + 0x12E3FE);

	InstallTrampoline((DWORD)hMod + 0x1F8A0, (DWORD)&HkAssertionFailed, 9, NULL);
	InstallTrampoline((DWORD)hMod + 0x59824, (DWORD)&HkPlayLoop, 6, &jb_PlayLoop);
	InstallTrampoline((DWORD)hMod + 0x5A780, (DWORD)&HkDoSomething, 5, &jb_DoSomething);
	
	FuzzMe();

	system("pause");
    return 0;

fail:
	system("pause");
	return 1;
}

