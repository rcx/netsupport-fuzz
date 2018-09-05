/*
   This file has been generated by IDA.
   It contains local type definitions from
   the type library 'PCICL32'
*/

struct ReplayParser_t;
struct vt_ReplayParserSub_t;
struct vt_ReplayParser_t;
struct ReplayParserSub_t;
struct ReplayParserS2_t;
struct CReplayer;
struct ReplayerSub_t;
struct ThreadThang;
struct ReplayerSSS_t;
struct ReplayerSS_t;
struct Wtf_t;
struct ReplayParserS2S;
struct ReplayerSSSS_t;
struct vt_ReplayParserS2S;
struct DataChunk_t;

struct __declspec(align(4)) FileDesc_t
{
	FILE *pFile;
	DWORD pad1;
	DWORD pad2;
	DWORD flags;
	DWORD fhandle;
	DWORD pad4;
	DWORD pad5;
	DWORD buf;
	CRITICAL_SECTION criticalSect;
};

/* 306 */
struct __declspec(align(4)) ReplayHdr_t
{
  DWORD magic;
  DWORD field1;
  DWORD wParam;
  DWORD bufFileOffset;
  WORD dataChunkCnt;
  WORD pad1;
  DWORD pad3;
  DWORD pad4;
  DWORD pad5;
  DWORD pad6;
  DWORD pad7;
  DWORD pad8;
  DWORD pad9;
};

/* 307 */
struct __declspec(align(4)) ReplayParser_t
{
  vt_ReplayParser_t *p_vft;
  DWORD pad0;
  ReplayParserS2_t *pSub2;
  DWORD pad2;
  DWORD pad3;
  DWORD pad4;
  DWORD pad5;
  DWORD pad6;
  DWORD pad7;
  DWORD pad8;
  DWORD pad9;
  DWORD pad10;
  ReplayParserSub_t *pSub;
  DWORD pad12;
  DWORD pad13;
};

/* 308 */
struct vt_ReplayParser_t
{
  int (__thiscall *vf0)(ReplayParser_t *pthis);
  int (__thiscall *vf1)(ReplayParser_t *tphis);
  int (__thiscall *vf2)(ReplayParser_t *thpis);
  int (__thiscall *vf3)(ReplayParser_t *thips);
  int (__thiscall *vf4)(ReplayParser_t *thisp);
  int (__thiscall *vf5)(ReplayParser_t *thisp);
};

/* 313 */
struct __declspec(align(4)) ReplayParserSub_t
{
  vt_ReplayParserSub_t *p_vft;
  DWORD pad0;
  DWORD pad1;
  DWORD pad2;
  ReplayerSSSS_t *pThang;
  CReplayer *cReplayer;
  DWORD pad5;
  DWORD pad6;
  DWORD pad7;
  DWORD pad8;
  DWORD pad9;
  ReplayParser_t *pParentReplayParser;
};

/* 314 */
struct __declspec(align(4)) vt_ReplayParserSub_t
{
  DWORD pad0;
  DWORD pad1;
  DWORD pad2;
  DWORD pad3;
  void (__thiscall *vf4)(int, char *, int, char *, int, int, int);
  DWORD pad5;
  DWORD pad6;
  DWORD vf7;
  DWORD vf8;
  DWORD vf9;
  DWORD vf10;
  DWORD vf11;
  DWORD vf12;
  DWORD vf13;
  DWORD vf14;
  DWORD vf15;
  DWORD vf16;
  DWORD vf17;
  DWORD vf18;
  DWORD vf19;
  void (__thiscall *doSomethingWithTheSmartBuf)(ReplayParserSub_t *, char **smartBuf, int bufSize, signed int flag);
  DWORD vf21;
};

/* 317 */
struct __declspec(align(4)) CReplayer
{
  void **vmt;
  ReplayParserSub_t *replayParserSub;
  char pad[1504];
  DWORD mystery000;
  DWORD mystery001;
  DWORD mystery002;
  DWORD mystery003;
  DWORD mystery004;
  DWORD mystery005;
  DWORD mystery006;
  DWORD mystery007;
  DWORD mystery008;
  DWORD mystery009;
  DWORD mystery0010;
  DWORD mystery0011;
  HANDLE hWaitObject;
  CRITICAL_SECTION criticalSection;
  ThreadThang *threadThang;
  DWORD stop;
  FileDesc_t *pfd;
  DWORD importantPlay;
  DWORD doTheThang;
  DWORD goToStartAndStop;
  DWORD play;
  DWORD mystery4;
  ReplayHdr_t header;
  DataChunk_t *pReplayData;
  DWORD timeThing;
  DWORD curwParam;
  DWORD mystery6;
  DWORD mystery7;
  DWORD somethingAudioBuf;
  DWORD mystery9;
  DWORD mystery10;
  DWORD mystery11;
  DWORD subObject;
  char pad2[964];
};

/* 321 */
struct vt_ReplayerSSSS
{
  DWORD vf0;
  DWORD vf1;
  DWORD vf2;
  DWORD vf3;
  DWORD vf4;
};

/* 322 */
struct ReplayerSSSS_t
{
  vt_ReplayerSSSS *vmt;
};

/* 327 */
struct __declspec(align(4)) ReplayParserS2_t
{
  DWORD pad0;
  DWORD pad1;
  DWORD pad2;
  DWORD pad3;
  ReplayParserSub_t *replayerSS;
  char pad40[80];
  DWORD mystery0;
  DWORD mystery01;
  char pad4[1380];
  ReplayParserS2S *pSub2;
  DWORD doPlay;
  DWORD mystery3;
  DWORD mystery4;
  DWORD mystery5;
  DWORD mystery6;
  DWORD mystery7;
  DWORD mystery8;
  char pad5[500];
};

/* 328 */
struct ReplayParserS2S
{
  vt_ReplayParserS2S *pVmt;
};

/* 329 */
struct vt_ReplayParserS2S
{
  DWORD vf[65];
};

/* 330 */
struct __declspec(align(4)) BufDescriptor_t
{
  DWORD size;
  DWORD field1;
  DWORD timeSmth;
};

/* 331 */
struct SmortBuf_t
{
  BufDescriptor_t desc;
  char *buf;
};

/* 335 */
struct __declspec(align(4)) DataChunk_t
{
  DWORD fileOffset;
  DWORD wParam;
};

/* 338 */
struct __declspec(align(4)) ThreadThang
{
  DWORD arg2;
  DWORD counterThing;
  LPVOID lpParam;
  DWORD pad1;
  DWORD pad2;
  DWORD pCounterThing2;
  DWORD pad3;
  HANDLE hEventWait;
};
