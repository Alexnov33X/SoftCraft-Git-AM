format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strInput   db 'Input year, it should be >0 and <10000 ', 0
        strIncorrect db '%d is not correct year', 10, 0
        strScanInt   db '%d', 0                                  ; string format to input int number
        strApril     db 'Next Easter will be %d of April', 10, 0
        strMay       db 'Next Easter will be %d of May', 10, 0

        a            dd ?   ; we use variables a-f to store information needed to use the Gauss algorithm
        b            dd ?
        cc           dd ?   ; word c is reserved in FASM, so we use cc
        d            dd ?
        e            dd ?
        f            dd ?
        year         dd ?
        NULL = 0

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
        push strInput   ;we print message, that asks user to input the desired year
        call [printf]

        push year       ;We input the year from the user
        push strScanInt
        call [scanf]
        mov eax, [year]

        cmp eax, 0      ;Checking for correct input
        jle fail
        cmp eax, 10000
        jge fail
Gauss:
        mov ecx, 19     ;a=year%19
        mov edx, 0
        div ecx
        mov [a], edx

        mov eax, [year]  ;b=year%4
        mov ecx, 4
        xor edx, edx
        div ecx
        mov [b], edx

        mov eax, [year]  ;cc=year%7
        mov ecx, 7
        xor edx, edx
        div ecx
        mov [cc], edx

        mov eax, [a]     ;d=(19*a+15)%30
        imul eax, 19
        add eax, 15
        mov ecx, 30
        xor edx, edx
        div ecx
        mov [d], edx

        mov eax, [b]   ;e=(2*b + 4*c + 6*d + 6)%7
        imul eax, 2
        mov ebx, [cc]
        imul ebx, 4
        add eax, ebx
        mov ebx, [d]
        imul ebx, 6
        add ebx, 6
        add eax, ebx
        mov ecx, 7
        xor edx, edx
        div ecx
        mov [e], edx

        mov eax, [d]       ;f=e+d
        add eax, [e]
        mov [f], eax

endOfGauss:
        mov eax, [f]     ;if f<=26 than we go to April
        cmp eax, 26
        jle April

        sub eax, 26     ;else we print May, with date = f-26
        push eax
        push strMay
        call [printf]

finishP:                  ;procedure for exiting the program after any input
        call [getch]
        push NULL
        call [ExitProcess]

fail:                    ; procedure for outputing warning for incorrect input, it also finishes program after any input
        push eax
        push strIncorrect
        call [printf]
        call [getch]
        push NULL
        call [ExitProcess]

April:                  ; procedure for calculating date in April, date = f+4
         add eax,4
         push eax
         push strApril
         call [printf]
         jmp finishP    ; here we jump straigth to finish

;-------------------------------third act - including HeapApi--------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'