Command line used to find this crash:

afl-fuzz.exe -t 10000 -i in -o out3 -D C:\Users\User\Downloads\NetSupport\DynamoRIO-Windows-6.2.0-2\bin32 -f C:\Users\User\Desktop\fuzz.rpf -- -target_module ConsoleApplication1.exe -target_method FuzzMe -coverage_module PCICL32.dll -call_convention cdecl -nargs 0 -fuzz_iterations 1000 -- C:\Users\User\Desktop\netsucc\ConsoleApplication1\Debug\ConsoleApplication1.exe

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 0 B.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http:\\lcamtuf.coredump.cx\afl\

Thanks :-)
