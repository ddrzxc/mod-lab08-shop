// Copyright 2024 Darya Rybakova
#include "../include/task.h"

long Factorial(int n) {
    long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n = 2;
    int bFlow = 20;
    int cSpeed = 100;
    int cSize = 5;
    int qLen = 5;
    int bCount = 100;

    int m = qLen;
    double lambda = bFlow;
    double mu = 1000.0 / cSpeed / cSize;
    double ro = lambda / mu;
    double P0 = 0;
    for (int k = 0; k <= n; k++) P0 += pow(ro, k) / Factorial(k);
    for (int k = 1; k <= m; k++) P0 += pow(ro, n+k) / (pow(n, k) * Factorial(n));
    P0 = pow(P0, -1);
    double Pq = (pow(ro, n)/Factorial(n)) * ((1-pow(ro/n, m)) / (1-(ro/n))) * P0;
    double Prej = P0 * pow(ro, n+m) / (pow(n, m) * Factorial(n));
    double Q = 1 - Prej;
    double A = lambda * Q;
    cout << "Вероятность отказа: " << Prej << endl;
    cout << "Относительная пропускная способность: " << Q << endl;
    cout << "Абсолютная пропускная способность: " << A << endl << endl;

    Market market(n, cSpeed, qLen);
    market.Work(bCount, bFlow, cSize);
    vector<double> stat = market.Statistics();
    cout << "Статистика магазина" << endl;
    cout << "Всего покупателей: " << stat[0] << endl;
    cout << "Обслужено покупателей: " << stat[1] << endl;
    cout << "Потеряно покупателей: " << stat[2] << endl;
    cout << "Среднее кол-во покупателей в очереди: " << stat[3] << endl;
    cout << "Среднее время обслуживания покупателя: " << stat[4] << endl;
    cout << "Среднее время работы кассы: " << stat[5] << endl << endl;

    cout << "Расчеты" << endl;
    cout << "Вероятность отказа: " << stat[2] / stat[0] << endl;
    cout << "Относительная пропускная способность: " << stat[1] / stat[0] << endl;
    cout << "Абсолютная пропускная способность: " << stat[1] / stat[0] * bFlow << endl;
}
