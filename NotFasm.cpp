// ������� ��������� ���194 ������� 11
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <map>
#include <vector>
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
void function(int index,int n, int *arr, long &maxnumber, bool maximum, vector<string> &maxes) //������� �[i] � A[i+1] + A[i+2] � A[i+3] + � A[j], ������� ���������� ��������� � ����. ������
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
            if (maximum && (max==maxnumber)) //���������� ������ ��������, ��� ��� �������� ������ ���������� ���� �����, ����� � ����� ��������� ��������� �����
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
    long maxnumber=LONG_MIN; // ���� ������������ �����, �������������� ��� ����������� ������
    cout << "Input integer n as size of an array A, n should be >10" << endl;
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

        for (int i = 0; i < 8; i++)
        {
            thr[i] = (new thread{ function, i, n, A, ref(maxnumber), false, ref(maxes) }); //��������� ������, ������� �� ������� ���� �������� � ������������ ���
        }
        for (int i = 0; i < 8; i++)
        {
            thr[i]->join();
            delete(thr[i]);
            thr[i] = (new thread{ function, i, n, A, ref(maxnumber), true, ref(maxes) }); //��������� ����� ������, �� ������ ���������� � ���������� � ���������� �������� i,j � ������ maxes
        }
        for (int i = 0; i < 8; i++)
        {
            thr[i]->join();
            delete(thr[i]);
        }
        cout << "MAX number is " << maxnumber << endl;
        for (size_t i = 0; i < maxes.size(); i++) //������� �� ������� ��� i,j ������� ���� ��������
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