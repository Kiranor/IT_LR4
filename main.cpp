#include <iostream>
#include <chrono>
#include <cmath>
using namespace std;
int Prime(unsigned long a)
{
    unsigned long i;
    if (a == 2)
        return 1;
    if (a == 0 || a == 1 || a % 2 == 0)
        return 0;
    for(i = 3; i*i <= a && a % i; i += 2)
        ;
    return i*i > a;
}

void task_1()
{
#pragma omp parallel num_threads(5)
    cout << "Hello, World!" << endl;
}

void task_2() {
    int N1 = 1000000;
    int N2 = 10000000;
    int count;
    int num_threads[5] = {1, 2, 4, 16, 32};
    double time[4][5];

    for (int j = 0; j < 5; ++j) {
        count = 0;
        cout << num_threads[j] << endl;
        auto start = chrono::system_clock::now();
        #pragma parallel num_threads(num_threads[j])
            #pragma omp for schedule(static)
                for (int i = 0; i < N1; ++i) if (Prime(i) == 1) count++;

        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed = end - start;

        time[0][j] = elapsed.count();
    }
    for (int j = 0; j < 5; ++j) {
        count = 0;
        cout << num_threads[j] << endl;
        auto start = chrono::system_clock::now();
        #pragma parallel num_threads(num_threads[j])
            #pragma omp for schedule(dynamic)
                for (int i = 0; i < N1; ++i) if (Prime(i) == 1) count++;

        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed = end - start;

        time[1][j] = elapsed.count();
    }

    for (int j = 0; j < 5; ++j) {
        count = 0;
        cout << num_threads[j] << endl;
        auto start = chrono::system_clock::now();
        #pragma parallel num_threads(num_threads[j])
            #pragma omp for schedule(static)
                for (int i = 0; i < N2; ++i) if (Prime(i) == 1) count++;

        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed = end - start;

        time[2][j] = elapsed.count();
    }
    for (int j = 0; j < 5; ++j) {
        count = 0;
        cout << num_threads[j] << endl;
        auto start = chrono::system_clock::now();
        #pragma parallel num_threads(num_threads[j])
            #pragma omp for schedule(dynamic)
                for (int i = 0; i < N2; ++i) if (Prime(i) == 1) count++;

        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed = end - start;

        time[3][j] = elapsed.count();
    }

    for (int i = 0; i < 4; ++i) {
        if (i % 2 == 0) cout << "          1      2     4     16      32\n";
        for (int j = 0; j < 5; ++j) {
            if ((j == 0) and (i % 2 == 0)) cout << "Static  ";
            if ((j == 0) and (i % 2 != 0)) cout << "Dynamic ";

            cout << round(time[i][j] * 10000) / 10000 << ' ';
        }
        cout << endl;
    }
}


int main() {



    //task_1();
    task_2();
    return 0;

}
