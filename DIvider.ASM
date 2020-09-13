format PE console

entry start

include 'win32a.inc'

section '.data' data readable writeable
        resStr db 'Result: %d', 0
        hello db 'Enter dividend and divider',0
        formatNum db '%d', 0
        A dd ?
        B dd ?
        C dd ?
        emptyStr db '%d', 0
        point db ',',0
        NULL = 0


section '.code' code readable executable

        start:
               push hello
               call [printf]
               push A
               push formatNum
               call [scanf]
               push B
               push formatNum
               call [scanf]
               mov eax, [A]
               mov ecx, [B]
               mov edx, 0
               div ecx
               mov [C], edx
               push eax
               push resStr
               call [printf]

               push point
               call [printf]
                    mov ebx, 0
                    lp:
                        mov eax, [C]
                        mov ecx, [B]
                        imul eax, 10
                        mov edx, 0
                        div ecx
                        mov [C], edx
                        push eax
                        push emptyStr
                        call [printf]

                        add ebx, 1
                        cmp ebx, 3
                     jne lp
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