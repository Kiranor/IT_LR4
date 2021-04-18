#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <omp.h>
#include <list>

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

    double time = 0;
    int count = 0;
    switch (mode) {
        case 's': {
            auto start = omp_get_wtime();
#pragma omp parallel num_threads(num_threads)
            {
#pragma omp for schedule(static)
                for (int i = 0; i < maximum; ++i) {
                    if (Prime(i) == 1)
                        count++;
                }
#pragma omp barier
            }
            auto end = omp_get_wtime();
            time = end - start;
            break;
        }
        case 'd': {
            auto start = omp_get_wtime();
#pragma omp parallel num_threads(num_threads)
            {
#pragma omp for schedule(dynamic)
                for (int i = 0; i < maximum; ++i) {
                    if (Prime(i) == 1)
                        count++;
                }
#pragma omp barier
            }
            auto end = omp_get_wtime();
            time = end - start;
            break;
        }
    }
    return time;
}

void task_1 () {
#pragma omp parallel num_threads(5)
    cout << "Hello, World!" << endl;
}

void task_2(){

    vector< vector <double> > time(4, vector<double> (5));
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
void task_3 () {


}


int main () {

    //task_1();
    //task_2();
    return 0;

}
