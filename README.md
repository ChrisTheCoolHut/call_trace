
# Call tracer

This utility uses the `ptrace` API to launch and attach to process and inspect instructions using [capstone](https://www.capstone-engine.org/). 
It will log call functions and display their first 4 argument for both i386 and amd64 binaries/

```bash
$ make
$ ./build/call_trace 
[-] Usage ./build/call_trace <program> [program args]
```

```bash
./build/call_trace /home/chris/projects/call_trace/example/baby2 AAAA
target started. will run '/home/chris/projects/call_trace/example/baby2'
[~] Binary is mapped from 0x400000 to 0x611000
addr : 611000
debugger started

Call 0x4004E4:
	[RDI] 0x4005b6 -> UHHP : [55DD340BCA90]
	[RSI] 0x2
	[RDX] 0x7ffedbdf2fad -> 0x7ffedbdf2fad -> /home/chris/projects/easy-re/baby2 : [55DD340BCA90]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55DD340BCA90]
Call 0x4031FC:
	[RDI] 0x2
	[RSI] 0x7ffedbdf2fad -> 0x7ffedbdf2fad -> /home/chris/projects/easy-re/baby2 : [55DD340BD2B0]
	[RDX] 0x7ffedbdf10e0 ->  : [55DD340BD2B0]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55DD340BD2B0]
Call 0x403219:
	[RDI] 0x2
	[RSI] 0x7ffedbdf2fad -> 0x7ffedbdf2fad -> /home/chris/projects/easy-re/baby2 : [55DD340BDAD0]
	[RDX] 0x7ffedbdf10e0 ->  : [55DD340BDAD0]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55DD340BDAD0]
Call 0x4005D3:
	[RDI] 0x403258 ->             _)_ : [55DD340BE2F0]
	[RSI] 0x7ffedbdf2fad -> 0x7ffedbdf2fad -> /home/chris/projects/easy-re/baby2 : [55DD340BE2F0]
	[RDX] 0x7ffedbdf10e0 ->  : [55DD340BE2F0]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55DD340BE2F0]
            _)_
Call 0x4005DD:
	[RDI] 0x403268 ->          .-'(/ '-. : [55DD340BEB40]
	[RSI] 0xE1A2A0
	[RDX] 0x0
	[RCX] 0x7FE4955B31E7
         .-'(/ '-.
.... SNIP ....
Call 0x400673:
	[RDI] 0x4033a2 ->         | _|   |_ | : [55DD340C67F0]
	[RSI] 0xE1A2A0
	[RDX] 0x0
	[RCX] 0x7FE4955B31E7
        | _|   |_ |
Call 0x40067D:
	[RDI] 0x4033b6 ->         (ooO   Ooo) : [55DD340C7040]
	[RSI] 0xE1A2A0
	[RDX] 0x0
	[RCX] 0x7FE4955B31E7
        (ooO   Ooo)
Call 0x400687:
	[RDI] 0x4033d0 -> ------------------------------------ : [55DD340C7890]
	[RSI] 0xE1A2A0
	[RDX] 0x0
	[RCX] 0x7FE4955B31E7
------------------------------------
Call 0x403189:
	[RDI] 0x7ffedbdf2fd0 -> AAAA : [55DD340C80E0]
	[RSI] 0x40a2c8 -> UMDCTF{ftofxcipqzwsbgvozjndeiriujudyjdr} : [55DD340C80E0] # <-- Suspicious looking call with a CTF flag
	[RDX] 0x40a2c8 -> UMDCTF{ftofxcipqzwsbgvozjndeiriujudyjdr} : [55DD340C80E0]
	[RCX] 0x7FE4955B31E7
Call 0x4031B9:
	[RDI] 0x40ef95 -> WRONG, try harder! : [55DD340C8930]
	[RSI] 0x40a2c8 -> UMDCTF{ftofxcipqzwsbgvozjndeiriujudyjdr} : [55DD340C8930]
	[RDX] 0x55
	[RCX] 0xFFFFFFFF
WRONG, try harder!
Call 0x40057D:
	[RDI] 0x7FE4956E2968
	[RSI] 0x400570 -> 0x400570 -> =  : [55DD340C9180]
	[RDX] 0x1
	[RCX] 0x0
the child executed 25 calls
```