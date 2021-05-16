#include <iostream>
#include <cmath>
#include <omp.h>
#include <random>
#include <stdlib.h>
#include <ctime>
#include <synchapi.h>


using namespace std;

int Prime (unsigned long a) {
    unsigned long i;
    if (a == 2)
        return 1;
    if (a == 0 || a == 1 || a % 2 == 0)
        return 0;
    for (i = 3; i * i <= a && a % i; i += 2);
    return i * i > a;
}

double PrimeCount (int num_threads, int maximum, char mode) {

    double time;
    int count = 0;
    //omp_set_num_threads(num_threads);
    switch (mode) {
        case 's': {
            //auto start = omp_get_wtime();
            auto start = clock();
#pragma omp parallel num_threads(num_threads)
            {
#pragma omp for schedule(static)
                for (int i = 0; i < maximum; ++i) {
                    if (Prime(i) == 1)
                        count++;
                }
//#pragma omp barier
            }
            //auto end = omp_get_wtime();
            auto end = clock();
            time = end - start;
            break;
        }
        case 'd': {
            //auto start = omp_get_wtime();
            auto start = clock();
#pragma omp parallel num_threads(num_threads)
            {
#pragma omp for schedule(dynamic, 10000)
                for (int i = 0; i < maximum; ++i) {
                    if (Prime(i) == 1)
                        count++;
                }
//#pragma omp barier
            }
            //auto end = omp_get_wtime();
            auto end = clock();
            time = end - start;
            break;
        }
    }
    return time / CLOCKS_PER_SEC;
}

void task_1 () {
#pragma omp parallel num_threads(5)
    cout << "Hello, World!" << endl;
}

void task_2(){

    vector<vector <double> > time(4, vector<double> (5));
    int N[4] = {1000000, 1000000, 10000000, 10000000};
    int row = 0;
    for (int numThreads = 1; numThreads <= 32; numThreads *= 2) {
        if (numThreads == 8) continue;
        for (int line = 0; line < 4; ++line) {
            if (line == 0 or line == 2) {
                //cout << PrimeCount(numThreads, N[line], 's') << endl;
                time[line][row] = round(PrimeCount(numThreads, N[line], 's') * 1000) / 1000;
            }
            if (line == 1 or line == 3) {
                //cout << PrimeCount(numThreads, N[line], 'd') << endl;
                time[line][row] = round(PrimeCount(numThreads, N[line], 'd') * 1000) / 1000;
            }
            //cout << time[line][row] << endl;
        }
        row++;
    }

    for (int i = 0; i < 4; ++i) {
        if (i == 0 or i == 2) cout << "Time for N = " << N[i] << endl;
        if (i % 2 == 0) cout << "          1     2     4    16    32\n";
        for (int j = 0; j < 5; ++j) {
            if ((j == 0) and (i % 2 == 0)) cout << "Static  ";
            if ((j == 0) and (i % 2 != 0)) cout << "Dynamic ";
            cout << time[i][j] << ' ';
        }
        cout << endl;
        if (i == 1) cout << endl;
    }

}

//TODO Сделать 3 задание
void task_3 (int length, int magicNumber, int numThreads) {

    //OMP_CANCELLATION = true;
    int N[length];
    int iterations[numThreads];// = new int[numThreads];
    bool found = false;

    for (int i = 0; i < numThreads; ++i) iterations[i] = 0;

    srand(time(NULL));
    for (int i = 0; i < length; ++i) N[i] = rand() % length + 1;
    cout << "Array:" << endl;
    //for (const auto &item : N) cout << item << ' ';
    cout << endl;

    int part = length / numThreads;
    cout << "Job per thread = " << part <<endl;
    cout << "Magic number = " << magicNumber <<endl;

    //auto start = omp_get_wtime();
#pragma omp parallel num_threads(numThreads) shared(iterations)
    {
        int i = 0;
        int threadNumber = omp_get_thread_num();
        int start = threadNumber * part;
        int end = start + part;
        //cout << "threadNumber = " << threadNumber << endl;
        //cout << "part = " << part <<  endl;
        //cout << "start = " << start <<  endl;
        //cout << "end = " << end <<  endl;
//#pragma omp barrier
        while(start <= end)
        {
            //Sleep(1);
            if (found) break;
            if (N[start] == magicNumber){
                iterations[threadNumber]++;
                found = true;
                cout << "Thread "<< threadNumber << " have found magic number!" << endl;
                break;
            }
            else {iterations[threadNumber]++; start++;}
        }
    }
    if (found) {
        cout << "Iterations per thread:" << endl;
        for (const auto item : iterations) cout << item << " ";
        cout << endl;
    }
    else {
        cout << "Magic number not found." << endl; cout << "Iterations per thread:" << endl;
        for (const auto item : iterations) cout << item << " ";
        cout << endl;
    }
/*#pragma omp parallel /*shared(found, iterations) num_threads(numThreads)
    {
#pragma omp for schedule(static)
            for (int i = 0; i < length; ++i) {
                iterations[omp_get_thread_num()]++;
#pragma omp critical
            {

                if (found == true) {
                    //break;
#pragma omp cancel for //lation point for
                }
                if (N[i] == magicNumber) {
                    found = true;
#pragma omp cancel for //lation point for
                    //break;
                }
            }
        }
    }*/
    //auto end = omp_get_wtime();
    //cout << end - start;


}

int main () {

    cout << "========================================" << endl;
    //task_1();
    cout << "========================================" << endl;
    //task_2();
    cout << "========================================" << endl;
    while (true) {
        int length = 20000, magicNumber = 3, numTreads = 4;
        //cout << "Input values (length magicNumber numTreads):" << endl;
        //cin >> length >> magicNumber >> numTreads;
        //cout << "Magic number = " << magicNumber << endl;
        task_3(length, magicNumber, numTreads);
        getchar();
    }
    return 0;

}
