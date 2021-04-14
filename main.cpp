#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

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

void PrimeCount (double (&time), int num_threads, int maximum, char mode) {

    int count = 0;
    switch (mode) {
        case 's': {
            auto start = chrono::system_clock::now();
#pragma omp parallel num_threads(num_threads)
            {
#pragma omp for schedule(static)
                for (int i = 0; i < maximum; ++i) {
                    if (Prime(i) == 1)
                        count++;
                }
#pragma omp barier
            }
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed = end - start;
            time = elapsed.count();
            break;
        }
        case 'd': {
            auto start = chrono::system_clock::now();
#pragma omp parallel num_threads(num_threads)
            {
#pragma omp for schedule(dynamic)
                for (int i = 0; i < maximum; ++i) {
                    if (Prime(i) == 1)
                        count++;
                }
#pragma omp barier
            }
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed = end - start;
            time = elapsed.count();
            break;
        }
    }
}

void task_1 () {
#pragma omp parallel num_threads(5)
    cout << "Hello, World!" << endl;
}
//TODO Починить подсчет времени
void task_2(){

    vector< vector <double> > time(4, vector<double> (5));
    int N[2] = {1000000, 10000000};

    for (int i = 1; i <= 32; i *= 2) {
        int t = 0;
        int k = 0;
        if (i == 8) continue;
        for (int j = 0; j < 4; ++j) {
            if (j == 0 or j == 2)
                if(j == 0) k = 0;
                else k = 1;
            PrimeCount(time[j][t], i, N[k], 's');
            if (j == 1 or j == 3)
                if(j == 1) k = 0;
                else k = 1;
                PrimeCount(time[j][t], i, N[k], 'd');
        t++;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if (i % 2 == 0) cout << "          1     2     4    16    32\n";
        for (int j = 0; j < 5; ++j) {
            if ((j == 0) and (i % 2 == 0)) cout << "Static  ";
            if ((j == 0) and (i % 2 != 0)) cout << "Dynamic ";

            cout << round(time[i][j] * 1000) / 1000 << ' ';
        }
        cout << endl;
    }
}

//TODO Сделать 3 задание
void task_3 () {


}


int main () {

    //task_1();
    task_2();
    //cout << time[0][0];
    return 0;

}
