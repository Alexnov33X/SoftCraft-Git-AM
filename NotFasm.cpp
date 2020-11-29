// ������� ��������� ���194 ������� 11
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <map>
#include <vector>
#include <omp.h>
#include <time.h> 
//����������� �����������
using namespace std;
/**
 * ������� ������� ��� �������, ��������� ����������� ����������� 
 * index - ����� ������, ��� ��� ����� �� 8, �� ��� �������� ����� 8 ��������, ����� ������� �� ����������� ���� � ������
 * n - ���������� ��������� � ������� �
 * arr - ������ �
 * &maxnumber - ������ �� ����� ������������� �����, ���� ������� ���������� ���� �������� ���� �� ��������� ��� ��� �� ������
 * maximum - ����� ��� �������, ���� ��� true, �� ��� ��������� ����������, ��� ��������� i,j ������������ � ������ maxes
 * maxes - ������ �����, ������� �������� ��� ���������� i,j � ������� ������:"i= "+i+" j= "+ j+"\n"
 */
int main()
{
    vector<string>maxes;
    bool maximum = false;
    int n = 0;
    thread* thr[8];
    long maxnumber=LONG_MIN; // ���� ������������ �����, �������������� ��� ����������� ������
    cout << "Input integer n as size of an array A, n should be >10" << endl;
    srand(time(NULL)); //������ ������ ��-���������� ������
    try
    {
        cin >> n;
        while (n < 11) //�������� �����, ���� n<11, �� ����������� ������
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please input correct n" << endl;
            cin >> n;
        }
        int* A = new int[n];
        for (int i = 0; i < n; i++) //���������� ������ � � ������� ��� �������� � �������
        {
            A[i] = rand();
            cout << "A[" << i << "]=" << A[i] << endl;
        }
        omp:
#pragma omp parallel num_threads(8) //������ ������, ��� �������� 8 �������
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
                        #pragma omp critical //����������� ������, ����� �������� �������� ������� ��� ������� ������ ������ ���������
                        {
                            if (maxnumber < max)
                            {
                                *&maxnumber = max;
                                cout << "This thread " << omp_get_thread_num() << " changed MAX to " << max << endl;
                            }
                        }
                    }
                    if (maximum && (max == maxnumber)) //���������� ������ ��������, ��� ��� �������� ������ ���������� ���� �����, ����� � ����� ��������� ��������� �����
                        maxes.push_back("i= " + to_string(counter) + " j= " + to_string(j) + "\n");
                }
                counter += 8;

            }            
        }
        if (!maximum)
        {
            maximum = true;
            goto omp; //��� ����� � ������ ������, �� ������ � ��� ���� �������� � �� ����� ����� i, j
        }
        cout << "MAX number is " << maxnumber << endl;
        for (size_t i = 0; i < maxes.size(); i++) //������� �� ������� ��� i,j ������� ���� ��������
        {
            cout << maxes[i];
        }
        maxes.clear(); //������ ��� ������, ������ �����
        maxes.~vector();
    }
    catch(exception ex)
    {
        cout << ex.what();
    }
    return 0;
}