
# Call tracer

This utility uses the `ptrace` API to launch and attach to process and inspect instructions using [capstone](https://www.capstone-engine.org/). 
It will log call functions and display their first 4 argument for both i386 and amd64 binaries/

```bash
$ sudo apt-get install libcapstone-dev
$ make
$ ./build/call_trace 
[-] Usage ./build/call_trace <program> [program args]
```

You can remove certain tracing features with environment variables:
```bash
NO_CALL_TRACE=1 ./build/call_trace <program> [program args] # No call tracing
NO_REG_COUNTS=1 ./build/call_trace <program> [program args] # No register counting
NO_INS_TRACE=1 ./build/call_trace <program> [program args] # No instruction counting
```

amd64 example
```bash
➜  pwn_trace git:(master) ✗ ./build/call_trace ./example/baby2 AAAA
[~] Binary is mapped from 0x400000 to 0x611000
addr : 611000
Call 0x4004E4:
	[RDI] 0x4005b6 -> UHHP : [55A25082A090]
	[RDX] 0x7ffecb339fd3 -> 0x7ffecb339fd3 -> ./example/baby2 : [55A25082A090]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55A25082A090]
Call 0x4031FC:
	[RSI] 0x7ffecb339fd3 -> 0x7ffecb339fd3 -> ./example/baby2 : [55A25082A090]
	[RDX] 0x7ffecb339700 ->  : [55A25082A090]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55A25082A090]
Call 0x403219:
	[RSI] 0x7ffecb339fd3 -> 0x7ffecb339fd3 -> ./example/baby2 : [55A25082A090]
	[RDX] 0x7ffecb339700 ->  : [55A25082A090]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55A25082A090]
Call 0x4005D3:
	[RDI] 0x403258 ->             _)_ : [55A25082A090]
	[RSI] 0x7ffecb339fd3 -> 0x7ffecb339fd3 -> ./example/baby2 : [55A25082A090]
	[RDX] 0x7ffecb339700 ->  : [55A25082A090]
	[RCX] 0x4031d0 -> AWAVAAUATL%.  : [55A25082A090]
            _)_
Call 0x4005DD:
	[RDI] 0x403268 ->          .-'(/ '-. : [55A25082A090]
         .-'(/ '-.
Call 0x4005E7:
	[RDI] 0x40327b ->         /    `    \ : [55A25082A090]
        /    `    \
Call 0x4005F1:
	[RDI] 0x40328f ->        /  -     -  \ : [55A25082A090]
       /  -     -  \
Call 0x4005FB:
	[RDI] 0x4032a4 ->       (`  a     a  `) : [55A25082A090]
      (`  a     a  `)
Call 0x400605:
	[RDI] 0x4032ba ->        \     ^     / : [55A25082A090]
       \     ^     /
Call 0x40060F:
	[RDI] 0x4032cf ->         '. '---' .' : [55A25082A090]
        '. '---' .'
Call 0x400619:
	[RDI] 0x4032e3 ->         .-`'---'`-. : [55A25082A090]
        .-`'---'`-.
Call 0x400623:
	[RDI] 0x4032f7 ->        /           \ : [55A25082A090]
       /           \
Call 0x40062D:
	[RDI] 0x40330c ->       /  / '   ' \  \ : [55A25082A090]
      /  / '   ' \  \
Call 0x400637:
	[RDI] 0x403322 ->     _/  /|       |\  \_ : [55A25082A090]
    _/  /|       |\  \_
Call 0x400641:
	[RDI] 0x40333a ->    `/|\` |+++++++|`/|\` : [55A25082A090]
   `/|\` |+++++++|`/|\`
Call 0x40064B:
	[RDI] 0x403352 ->         /\       /\ : [55A25082A090]
        /\       /\
Call 0x400655:
	[RDI] 0x403366 ->         | `-._.-` | : [55A25082A090]
        | `-._.-` |
Call 0x40065F:
	[RDI] 0x40337a ->         \   / \   / : [55A25082A090]
        \   / \   /
Call 0x400669:
	[RDI] 0x40338e ->         |_ |   | _| : [55A25082A090]
        |_ |   | _|
Call 0x400673:
	[RDI] 0x4033a2 ->         | _|   |_ | : [55A25082A090]
        | _|   |_ |
Call 0x40067D:
	[RDI] 0x4033b6 ->         (ooO   Ooo) : [55A25082A090]
        (ooO   Ooo)
Call 0x400687:
	[RDI] 0x4033d0 -> ------------------------------------ : [55A25082A090]
------------------------------------
Call 0x403189:
	[RDI] 0x7ffecb339fe3 -> AAAA : [55A25082A090]
	[RSI] 0x40a2c8 -> UMDCTF{ftofxcipqzwsbgvozjndeiriujudyjdr} : [55A25082A090]
	[RDX] 0x40a2c8 -> UMDCTF{ftofxcipqzwsbgvozjndeiriujudyjdr} : [55A25082A090]
Call 0x4031B9:
	[RDI] 0x40ef95 -> WRONG, try harder! : [55A25082A090]
	[RSI] 0x40a2c8 -> UMDCTF{ftofxcipqzwsbgvozjndeiriujudyjdr} : [55A25082A090]
WRONG, try harder!
Call 0x40057D:
	[RSI] 0x400570 -> 0x400570 -> =  : [55A25082A090]

116329 : ins executed

19320 : times RAX == 0

11631 : times RCX == 0

14679 : times RDX == 0

5083 : times RDI == 0

24791 : times RSI == 0
```
i386 example
```
➜  pwn_trace git:(master) ✗ ./build/call_trace ./example/crackme0x04 
[~] Binary is mapped from 0x8048000 to 0x804b000
addr : 804b000
Call 0x80483EC:
	[0x8048509] 0x8048509 -> U : [564B6B166090]
	[0xffbf4fcc] 0xffbf4fcc -> 0xffbf4fcc -> ./examplO : [564B6B166090]
	[0x8048570] 0x8048570 -> UWV1Sh : [564B6B166090]
Call 0x8048578:
	[0xffbf4fcc] 0xffbf4fcc -> 0xffbf4fcc -> ./examplO : [564B6B166090]
	[0x8048570] 0x8048570 -> UWV1Sh : [564B6B166090]
Call 0x8048586:
	[0xffbf4fcc] 0xffbf4fcc -> 0xffbf4fcc -> ./examplO : [564B6B166090]
Call 0x8048342:
	[0x8048570] 0x8048570 -> 0x8048570 -> UWV1Sp : [564B6B166090]
	[0x804858b] 0x804858b -> E)9s(ED$ED$E$EF)9r[^_ : [564B6B166090]
Call 0x80483F8:
	[0x8049f20] 0x8049f20 -> 0x8049f20 ->  : [564B6B166090]
	[0xffbf4df8] 0xffbf4df8 -> 0xffbf4df8 -> pM : [564B6B166090]
	[0x8048347] 0x8048347 ->  : [564B6B166090]
Call 0x8048347:
	[0x8048570] 0x8048570 -> 0x8048570 -> UWV1Sp : [564B6B166090]
	[0x804858b] 0x804858b -> E)9s(ED$ED$E$EF)9r[^_ : [564B6B166090]
Call 0x804834C:
	[0x8048570] 0x8048570 -> 0x8048570 -> UWV1Sp : [564B6B166090]
	[0x804858b] 0x804858b -> E)9s(ED$ED$E$EF)9r[^_ : [564B6B166090]
Call 0x804852F:
	[0x804865e] 0x804865e -> IOLI Crackme Level 0x04 : [564B6B166090]
IOLI Crackme Level 0x04
Call 0x804853B:
	[0x8048677] 0x8048677 -> Password:  : [564B6B166090]
Call 0x804854E:
	[0x8048682] 0x8048682 -> %s : [564B6B166090]
	[0xffbf4d30] 0xffbf4d30 -> @46  : [564B6B166090]
Password: AAA
Call 0x8048559:
	[0xffbf4d30] 0xffbf4d30 -> AAA : [564B6B166090]
	[0xffbf4d30] 0xffbf4d30 -> AAA : [564B6B166090]
Call 0x804849E:
	[0xffbf4d30] 0xffbf4d30 -> AAA : [564B6B166090]
	[0xffbf4da8] 0xffbf4da8 ->   : [564B6B166090]
Call 0x80484C9:
	[0xffbf4cfb] 0xffbf4cfb -> A : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Call 0x804849E:
	[0xffbf4d30] 0xffbf4d30 -> AAA : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Call 0x80484C9:
	[0xffbf4cfb] 0xffbf4cfb -> A : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Call 0x804849E:
	[0xffbf4d30] 0xffbf4d30 -> AAA : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Call 0x80484C9:
	[0xffbf4cfb] 0xffbf4cfb -> A : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Call 0x804849E:
	[0xffbf4d30] 0xffbf4d30 -> AAA : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Call 0x8048502:
	[0x8048649] 0x8048649 -> Password Incorrect! : [564B6B166090]
	[0x8048638] 0x8048638 -> %d : [564B6B166090]
	[0xffbf4d04] 0xffbf4d04 -> @M^0M0M : [564B6B166090]
Password Incorrect!
Call 0x8048618:
	[0xffbf4d48] 0xffbf4d48 -> z : [564B6B166090]
Call 0x8048625:
	[0x8048614] 0x8048614 -> US : [564B6B166090]
	[0xffbf4d48] 0xffbf4d48 -> z : [564B6B166090]

122492 : ins executed

13765 : times RAX == 0

7205 : times RCX == 0

24575 : times RDX == 0

10072 : times RDI == 0

4026 : times RSI == 0


```
