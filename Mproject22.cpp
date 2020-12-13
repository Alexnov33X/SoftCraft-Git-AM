// MProject2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//11. «адача о магазине.¬ магазине работают три отдела, каждый отдел
//обслуживает один продавец.ѕокупатель, зайд€ в магазин, делает покупки в
//произвольных отделах, и если в выбранном отделе продавец не свободен,
//покупатель становитс€ в очередь и засыпает, пока продавец не освободитс€.
//—оздать многопоточное приложение, моделирующее рабочий день магазина.
//#include <iostream>
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio> 
#include <cstdlib>
#include <time.h>
#include <queue> 
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using seconds = chrono::duration<long long>;
pthread_mutex_t* mutex = new pthread_mutex_t[3];
int* id = new int[3];
pthread_cond_t* buy= new pthread_cond_t[3];
pthread_cond_t* sell = new pthread_cond_t[3];


void* Buyer(void* param) {
    int cNum = *((int*)param);
    seconds wait = seconds{ cNum };
    this_thread::sleep_for(wait);
    srand(time(NULL));   
    cout << "Buyer #" << cNum  <<" enters the shop"<< endl;
    while (1) {
        int num = rand() % 3;
        pthread_mutex_t mutexx = mutex[num];
        pthread_mutex_lock(&mutexx);
        while (id[num]!=NULL) {
            cout << "Buyer #" << cNum << " waits Seller#" << num+1 << endl;
            pthread_cond_wait(&sell[num], &mutexx);
        }
        this_thread::sleep_for(1s);
        cout << "Buyer #" << cNum << " deals with Seller #" << num+1 << endl;
        id[num] = cNum;
        pthread_mutex_unlock(&mutexx);
        pthread_cond_broadcast(&buy[num]);
        while (id[num] == cNum)
            this_thread::sleep_for(1s);
        this_thread::sleep_for(5s);
    }
    return NULL;
}
void* Seller(void* param) {
    int pNum = ((pair<int, pthread_mutex_t>*)param)->first;
    pthread_mutex_t mutex= ((pair<int, pthread_mutex_t>*)param)->second;
    while (1) {

        pthread_mutex_lock(&mutex); 
        while (id[pNum-1]==NULL) {
            pthread_cond_wait(&buy[pNum-1], &mutex);
        }
        this_thread::sleep_for(1s);
        cout << "Seller #" << pNum << " is done with Buyer #" << id[pNum - 1] << endl;
        id[pNum - 1] = NULL;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&sell[pNum-1]);
        this_thread::sleep_for(8s);
    }
    return NULL;
}

int main() {
    int i;
    //инициализаци€ мутексов и семафоров
    for (i = 0; i < 3; i++) {
        id[i] = NULL;
        pthread_mutex_init(&mutex[i], NULL);
        pthread_cond_init(&buy[i], NULL);
        pthread_cond_init(&sell[i], NULL);
    }
    //запуск производителей
    pthread_t threadP[3];
    for (i = 0; i < 3; i++) {
        pthread_create(&threadP[i], NULL, Seller,(void*)(new pair<int, pthread_mutex_t> (i+1,mutex[i])));
    }
    this_thread::sleep_for(1s);
    //запуск потребителей
    pthread_t threadC[5];
    int consumers[5];
    for (i = 0; i < 5; i++) {
        consumers[i] = i + 1;
        pthread_create(&threadC[i], NULL, Buyer, (void*)(consumers + i));
    }
    this_thread::sleep_for(40s);
    cout <<endl<< "Shop is closed";
}
