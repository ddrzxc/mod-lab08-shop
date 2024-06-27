// Copyright 2024 Darya Rybakova
#include "../include/task.h"

Market::Market(int n, int cSpeed, int qLen) {
    for (int i = 0; i < n; i++)
        cashiers.push_back(Cashier(cSpeed));
    queueLen = qLen;
}

void Market::TryAccept(Buyer b) {
    if (buyers.size() == queueLen) {
        //cout << b.id << " rejected!\n";
        bRejected++;
        return;
    }
    //cout << b.id << " accepted\n";
    bAccepted++;
    unique_lock<mutex> lock(mut);
    b.inQueue = chrono::system_clock::now();
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
        auto now = chrono::system_clock::now();
        auto bTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - b.inQueue).count();
        bWaiting += bTime + b.shopCart.size() * cashiers[c].speed;
        if (cashiers[c].t.joinable()) cashiers[c].t.join();
        cashiers[c].Serve(b);
    }
    for (int i = 0; i < cashiers.size(); i++) {
        if (cashiers[i].t.joinable()) cashiers[i].t.join();
        cWork += cashiers[i].workTime;
    }
}

void Market::Work(int bCount, int bFlow, int cSize) {
    this->bCount = bCount;
    work = true;
    thread service(&Market::Serving, this);
    random_device rd;
    mt19937 gen(rd());
    for (int i = 0; i < bCount; i++) {
        Buyer b(i, cSize);
        qAvg += buyers.size();
        this->TryAccept(b);
        int avgTime = 1000 / bFlow;
        uniform_int_distribution<> dist(avgTime-avgTime/2, avgTime+avgTime/2);
        this_thread::sleep_for(chrono::milliseconds(dist(gen)));
    }
    work = false;
    service.join();

}

vector<double> Market::Statistics() {
    return {
        bCount,
        bAccepted,
        bRejected,
        qAvg / bCount,
        bWaiting / bAccepted,
        cWork / cashiers.size()
    };
}

Cashier::Cashier(int cSpeed) {
    speed = cSpeed;
}

void Cashier::Serve(Buyer b) {
    busy = true;
    cartSize = b.shopCart.size();
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
