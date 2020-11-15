// Миронов Александр БПИ194 Вариант 11
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <map>
#include <vector>
//Итеративный параллелизм
using namespace std;
/**
 * главная функция для потоков, реализует итеративный параллелизм 
 * index - номер потока, так как всего их 8, то они работают через 8 значений, таким образом не пересекаясь друг с другом
 * n - количество элементов в массиве А
 * arr - массив А
 * &maxnumber - ссылка на адрес максимального числа, сюда массивы записывают свой максимум если он превышает тот что по адресу
 * maximum - опция для функции, если она true, то идёт сравнение максимумов, при равенстве i,j отправляются в вектор maxes
 * maxes - вектор строк, который содержит все подходящие i,j в формате строки:"i= "+i+" j= "+ j+"\n"
 */
void function(int index,int n, int *arr, long &maxnumber, bool maximum, vector<string> &maxes) //функция А[i] – A[i+1] + A[i+2] – A[i+3] + … A[j], которая сравнивает результат с макс. числом
{
    long max = arr[index];
    int counter = index;
    while (counter < n)
    {
        for (int j = counter + 1; j < n; j++)
        {
            max = arr[counter];
            bool minus = true;
            for (int i = counter+1; i < j; i++)
            {
                if (minus)
                {
                    max -= arr[i];
                    minus = false;
                }
                else
                {
                    max += arr[i];
                    minus = true;
                }
            }
            if (*&maxnumber < max)
                *&maxnumber = max;
            if (maximum && (max==maxnumber)) //используем вектор стрингов, так как возможен случай одинаковых макс чисел, тогда в конце выведется несколько чисел
                maxes.push_back("i= "+to_string(counter)+" j= "+ to_string(j)+"\n");
        }
        counter += 8;
    }
}
int main()
{
    vector<string>maxes;
    int n = 0;
    thread* thr[8];
    long maxnumber=LONG_MIN; // наше максимальное число, инициализируем его минимальным числом
    cout << "Input integer n as size of an array A, n should be >10" << endl;
    try
    {
        cin >> n;
        while (n < 11) //проверка ввода, если n<11, то запрашиваем заново
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please input correct n" << endl;
            cin >> n;
        }
        int* A = new int[n];
        for (int i = 0; i < n; i++) //генерируем массив А и выводим его элементы в консоль
        {
            A[i] = rand();
            cout << "A[" << i << "]=" << A[i] << endl;
        }

        for (int i = 0; i < 8; i++)
        {
            thr[i] = (new thread{ function, i, n, A, ref(maxnumber), false, ref(maxes) }); //запускаем потоки, которые по функции ищут максимум и переписывают его
        }
        for (int i = 0; i < 8; i++)
        {
            thr[i]->join();
            delete(thr[i]);
            thr[i] = (new thread{ function, i, n, A, ref(maxnumber), true, ref(maxes) }); //запускаем снова потоки, но теперь сравниваем с максимумом и записываем значения i,j в вектор maxes
        }
        for (int i = 0; i < 8; i++)
        {
            thr[i]->join();
            delete(thr[i]);
        }
        cout << "MAX number is " << maxnumber << endl;
        for (size_t i = 0; i < maxes.size(); i++) //выводим из вектора все i,j которые дают максимум
        {
            cout << maxes[i];
        }
    }
    catch(exception ex)
    {
        cout << ex.what();
    }
    return 0;
}