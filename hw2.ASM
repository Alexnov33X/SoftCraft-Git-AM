;вариант 11 - ДЗ2
;Миронов Александр Алексеевич БПИ194
;условие: Разработать программу, которая вводит одномерный массив A[N], формирует из элементов массива A новый массив B по правилам,
;указанным в таблице, и выводит его. Память под массивы может выделяться как статически, так и динамически по выбору разработчика.

;Разбить решение задачи на функции следующим образом:

;Ввод и вывод массивов оформить как подпрограммы.
;Выполнение задания по варианту оформить как процедуру
;Организовать вывод как исходного, так и сформированного массивов
;Указанные процедуры могут использовать данные напрямую (имитация процедур без параметров). Имитация работы с параметрами также допустима.
;11 Вариант - за исключением первого положительного элемента
format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strVecSize     db 'size of vector? Max allowed size is 100: ', 0
        strOut         db 'Array B: ',10, 0
        strEntry       db 'Array A: ',10, 0
        strIncorSize   db 'Incorrect size of vector = %d', 10, 0
        strVecElemI    db '[%d]= ', 0
        strScanInt     db '%d', 0
        strVecElemOut  db '[%d] = %d', 10, 0

        checker      dd 1
        vec_size     dd 0
        i            dd ?
        tmp          dd ?
        tmp2         dd ?
        tmpStack     dd ?
        vec          rd 100
        NULL = 0

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
        call VectorInput
        push strOut
        call [printf]
        call VectorOut
        call [getch]
        push NULL
        call [ExitProcess]

;--------------------------------------------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        push strEntry
        call [printf]
        add esp, 4

        mov eax, [vec_size]
        cmp eax, 0
        jg  getVector
failSafe:
        push [vec_size]
        push strIncorSize
        call [printf]
        call [getch]
        push NULL
        call [ExitProcess]

getVector:
        cmp eax,100
        jg  failSafe
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
VectorOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, vec            ; ebx = &vec
putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx
        cmp [checker], 1    ;we remove element only once so there is a checker for that
        je Task
Continue:
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret
Task:
        mov [tmp2], ebx
        cmp dword [ebx], 0
        jge RemoveElem
        jmp Continue
RemoveElem:
        mov ebx, [tmp2]
        add ebx, 4
        mov [checker],0
        dec [vec_size]
        jmp putVecLoop
                                                 
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