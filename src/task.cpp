// Copyright 2024 Darya Rybakova
#include "../include/task.h"

Market::Market(int n, int cSpeed, int qLen) {
    for (int i = 0; i < n; i++)
        cashiers.push_back(Cashier(cSpeed));
    queueLen = qLen;
}

void Market::TryAccept(Buyer b) {
    if (buyers.size() == queueLen) {
        cout << b.id << " rejected!\n";
        return;
    }
    cout << b.id << " accepted\n";
    unique_lock<mutex> lock(mut);
    buyers.push(b);
}

void Market::Serving() {
    while (!buyers.empty() || work) {
        if (buyers.empty()) continue;
        int c = -1;
        for (int i = 0; i < cashiers.size(); i++) {
            if (!cashiers[i].busy) {
                c = i;
                break;
            }
        }
        if (c == -1) continue;
        Buyer b = buyers.front();
        buyers.pop();
        if (cashiers[c].t.joinable()) cashiers[c].t.join();
        cashiers[c].Serve(b);
    }
    for (int i = 0; i < cashiers.size(); i++)
        if (cashiers[i].t.joinable()) cashiers[i].t.join();
}

void Market::Work(int bCount, int bFlow, int cSize) {
    work = true;
    thread service(&Market::Serving, this);

    for (int i = 0; i < bCount; i++) {
        Buyer b(i, cSize);
        this->TryAccept(b);
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    work = false;
    service.join();
}

Cashier::Cashier(int cSpeed) {
    speed = cSpeed;
}

void Cashier::Serve(Buyer b) {
    busy = true;
    cartSize = 2;
    t = thread(&Cashier::Work, this);
}

void Cashier::Work() {
    workTime += cartSize * speed;
    this_thread::sleep_for(chrono::milliseconds(cartSize * speed));
    busy = false;
}

Buyer::Buyer(int id, int cSize) {
    this->id = id;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(cSize-cSize/2, cSize+cSize/2);
    shopCart = vector<int>(dist(gen));
}
