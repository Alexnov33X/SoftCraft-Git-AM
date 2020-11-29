// ћиронов јлександр Ѕѕ»194 ¬ариант 11
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <map>
#include <vector>
#include <omp.h>
#include <time.h> 
//»теративный параллелизм
using namespace std;
/**
 * главна€ функци€ дл€ потоков, реализует итеративный параллелизм 
 * index - номер потока, так как всего их 8, то они работают через 8 значений, таким образом не пересека€сь друг с другом
 * n - количество элементов в массиве ј
 * arr - массив ј
 * &maxnumber - ссылка на адрес максимального числа, сюда массивы записывают свой максимум если он превышает тот что по адресу
 * maximum - опци€ дл€ функции, если она true, то идЄт сравнение максимумов, при равенстве i,j отправл€ютс€ в вектор maxes
 * maxes - вектор строк, который содержит все подход€щие i,j в формате строки:"i= "+i+" j= "+ j+"\n"
 */
int main()
{
    vector<string>maxes;
    bool maximum = false;
    int n = 0;
    thread* thr[8];
    long maxnumber=LONG_MIN; // наше максимальное число, инициализируем его минимальным числом
    cout << "Input integer n as size of an array A, n should be >10" << endl;
    srand(time(NULL)); //теперь рандом по-насто€щему рандом
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
        for (int i = 0; i < n; i++) //генерируем массив ј и выводим его элементы в консоль
        {
            A[i] = rand();
            cout << "A[" << i << "]=" << A[i] << endl;
        }
        omp:
#pragma omp parallel num_threads(8) //создаЄм секцию, где работают 8 потоков
        {       
            int index = omp_get_thread_num();
            long max;
            int counter = index;
            while (counter < n)
            {
                for (int j = counter + 1; j < n; j++)
                {
                    max = A[counter];
                    bool minus = true;
                    for (int i = counter + 1; i < j; i++)
                    {
                        if (minus)
                        {
                            max -= A[i];
                            minus = false;
                        }
                        else
                        {
                            max += A[i];
                            minus = true;
                        }
                    }
                    if (maxnumber < max)
                    {
                        #pragma omp critical //критическа€ секци€, чтобы избежать коллизии потоков при попытке записи нового максимума
                        {
                            if (maxnumber < max)
                            {
                                *&maxnumber = max;
                                cout << "This thread " << omp_get_thread_num() << " changed MAX to " << max << endl;
                            }
                        }
                    }
                    if (maximum && (max == maxnumber)) //используем вектор стрингов, так как возможен случай одинаковых макс чисел, тогда в конце выведетс€ несколько чисел
                        maxes.push_back("i= " + to_string(counter) + " j= " + to_string(j) + "\n");
                }
                counter += 8;

            }            
        }
        if (!maximum)
        {
            maximum = true;
            goto omp; //идЄм снова в секцию поиска, но теперь у нас есть максимум и мы можем найти i, j
        }
        cout << "MAX number is " << maxnumber << endl;
        for (size_t i = 0; i < maxes.size(); i++) //выводим из вектора все i,j которые дают максимум
        {
            cout << maxes[i];
        }
        maxes.clear(); //чистим наш вектор, пам€ть важна
        maxes.~vector();
    }
    catch(exception ex)
    {
        cout << ex.what();
    }
    return 0;
}