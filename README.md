# NetSupport (LANDesk) research

This is just a loose collection of my old research on LANDesk/NetSupport's replay system from when I was bored in computer class in high school. I saw that there was a tray icon that had an option to "Load Replay..." or something like that, and I also noticed it was running as an admin (!) user. It would be cool to have a LPE using this mechanism if I could exploit the replay parser.

Using WinAFL and DynamoRIO I found a heap overflow, essentially as it parses a file, you pass it a negative size field which leads to a size overflow.
Unfortunately the school's computer was on Windows 10 which has a [very arcane heap](https://www.blackhat.com/docs/us-16/materials/us-16-Yason-Windows-10-Segment-Heap-Internals.pdf), and before I exploited it I graduated. I included the idbs so you can check that logic out yourself.

## Try it yourself

Run `client32.exe /r`, then select `fuzz - Copy.rpf`. It will crash when you try to hit the play button.

To try out the fuzzing harness, copy `crash3.rpf` to `C:\Users\User\Desktop`, and run `ConsoleApplication1.exe`. It will crash.

## Interesting technical parts

This is a relatively standard fuzzing harness but `ConsoleApplication1.cpp` has some novel constructions. Keep in mind that I wrote this over several days in class while being off-task and not paying attention, so the code quality is pretty bad. Lots of inline asm and hungarian notation.

It uses your standard 0xE9 jmp trampolines with stolen bytes. My favorite part is the `FuzzWrapper` pseudo-"exception handler": before it calls the fuzzed code, it preserves all of the registers including `eip` and `esp`. It hooks `AssertionFailed`, and when called from the main hread it simply jumps back to the end of the wrapper safely, wiping out the callstack and resetting the context.

Otherwise, to make it play nice with afl, it hooks several functions:

 - `kernel32!timeGetTime` - just makes it return 0.
 - `kernelbase!GetTickCount` - ditto.
 - `kernel32!CreateThread` - allows or blocks the call based on the caller. Specifically it needs to deal with some netsupport internal event shit passed on certain threads or it hangs. It also keeps track of the thread parameter for an important function we need to call later.

It also hooks some target-specific functions to make it fuzz nicely. Some other hooks are in place as well to make sure the stdin fd is closed after each iteration.

## Other tidbits

There's also a `pcijoin.exe` which seems to do network stuff. There was actually a [remote RCE on another part of LANDesk](https://www.exploit-db.com/exploits/15937/) actually, but I didn't look too much into it. That stuff is in `pcijoin.idb` and `TCCTL32.idb`.

You can use client32.exe to draw on your screen or lock it if you want. I did this to myself accidentally once.
