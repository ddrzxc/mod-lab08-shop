// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(market, test1) {
    Buyer b(2, 8);
    EXPECT_EQ(2, b.id);
    EXPECT_EQ(true, 4 <= b.shopCart.size() && b.shopCart.size() <= 12);
}

TEST(market, test2) {
    Buyer b(2, 8);
    int bCart = b.shopCart.size();
    Cashier c(100);
    c.Serve(b);
    c.t.join();
    EXPECT_EQ(bCart * 100, c.workTIme);
    EXPECT_EQ(false, c.busy);
}

TEST(market, test3) {
    int n = 2;
    int bFlow = 20;
    int cSpeed = 100;
    int cSize = 5;
    int qLen = 0;
    int bCount = 100;
    Market market(n, cSpeed, qLen);
    market.Work(bCount, bFlow, cSize);
    vector<double> stat = market.Statistics();
    EXPECT_EQ(0, stat[1]);
}

TEST(market, test4) {
    int n = 20;
    int bFlow = 20;
    int cSpeed = 10;
    int cSize = 5;
    int qLen = 5;
    int bCount = 100;
    Market market(n, cSpeed, qLen);
    market.Work(bCount, bFlow, cSize);
    vector<double> stat = market.Statistics();
    EXPECT_EQ(0, stat[2]);
}

TEST(market, test5) {
    int n = 2;
    int bFlow = 20;
    int cSpeed = 100;
    int cSize = 5;
    int qLen = 1;
    int bCount = 100;
    Market market(n, cSpeed, qLen);
    market.Work(bCount, bFlow, cSize);
    vector<double> stat = market.Statistics();
    EXPECT_EQ(true, stat[3] >= 0.7);
}