// Copyright 2024 Darya Rybakova
#include <iostream>
#include <queue>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
using namespace std;

class Buyer {
public:
    int id;
    vector<int> shopCart;
    chrono::system_clock::time_point inQueue;
    Buyer(int id, int cSize);
};

class Cashier {
public:
    Cashier(int cSpeed);
    void Work();
    void Serve(Buyer b);
    bool busy;
    thread t;
    int speed;
    int workTime;
    int cartSize;
};

class Market {
private:
    vector<Cashier> cashiers;
    queue<Buyer> buyers;
    int queueLen;
    void Serving();
    bool work;
    mutex mut;
    double bCount = 0;
    double bAccepted = 0;
    double bRejected = 0;
    double qAvg = 0;
    double bWaiting = 0;
    double cWork = 0;
public:
    Market(int n, int cSpeed, int qLen);
    void TryAccept(Buyer b);
    void Work(int bCount, int bFlow, int cSize);
    vector<double> Statistics();
};
