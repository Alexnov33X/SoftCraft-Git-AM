format PE console

entry start

include 'win32a.inc'

section '.data' data readable writeable
        resStr db 'Result: %d', 0
        hello db 'Enter number to add 10',0
        formatNum db '%d', 0
        A dd ?
        B dd ?
        C dd ?
        NULL = 0


section '.code' code readable executable

        start:
               push hello
               call [printf]
               push C
               push formatNum
               call [scanf]
               mov [B], 10
                   mov ecx, [C]
                   add ecx, [B]
                   push ecx
                    push resStr
                   call [printf]
                call [getch]
                push NULL
                call [ExitProcess]

section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               getch, '_getch',\
               scanf, 'scanf'