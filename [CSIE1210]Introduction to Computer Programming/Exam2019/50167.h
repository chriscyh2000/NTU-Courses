#include <stdio.h>
#include <stdbool.h>
double getPrice(int orderItemID, int itemID[], double price[]){
    int j = 0;
    while(itemID[j]){
        if(itemID[j] == orderItemID) return price[j];
        ++j;
    }
    return 0;
}
bool isSale(int orderItemID, int onSaleItemID[]){
    int i = 0;
    while(onSaleItemID[i]){
        if(onSaleItemID[i] == orderItemID) return true;
        ++i;
    }
    return false;
}
double payment(int itemID[], double price[], int orderItemID[], int orderQuantity[], int onSaleItemID[]){
    int i = 0;
    double payment = 0;
    while(orderItemID[i]){
        int j = 0;
        double Price = getPrice(orderItemID[i], itemID, price);
        if(isSale(orderItemID[i], onSaleItemID)) payment += Price * 0.8 * orderQuantity[i];
        else payment += Price * orderQuantity[i];
        ++i;
    }
    if(payment < 490.00) return payment + 80;
    return payment;
}