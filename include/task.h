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
    Buyer(int id, int cSize);
};

class Cashier {
private:
    int speed;
    int workTime;
    int cartSize;
public:
    Cashier(int cSpeed);
    void Work();
    void Serve(Buyer b);
    bool busy;
    thread t;
};

class Market {
private:
    vector<Cashier> cashiers;
    queue<Buyer> buyers;
    int queueLen;
    void Serving();
    bool work;
    mutex mut;
public:
    Market(int n, int cSpeed, int qLen);
    void TryAccept(Buyer b);
    void Work(int bCount, int bFlow, int cSize);
};
