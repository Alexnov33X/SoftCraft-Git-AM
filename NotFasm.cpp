// Миронов Александр БПИ194 Вариант 11

#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <map>
#include <vector>
//Итеративный параллелизм
using namespace std;
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
        for (int i = 0; i < n; i++)
        {
            A[i] = rand();
            cout << "A[" << i << "]=" << A[i] << endl;
        }

        for (int i = 0; i < 8; i++)
        {
            thr[i] = (new thread{ function, i, n, A, ref(maxnumber), false, ref(maxes) });
        }
        for (int i = 0; i < 8; i++)
        {
            thr[i]->join();
            delete(thr[i]);
            thr[i] = (new thread{ function, i, n, A, ref(maxnumber), true, ref(maxes) });
        }
        for (int i = 0; i < 8; i++)
        {
            thr[i]->join();
            delete(thr[i]);
        }
        cout << "MAX number is " << maxnumber << endl;
        for (size_t i = 0; i < maxes.size(); i++)
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
