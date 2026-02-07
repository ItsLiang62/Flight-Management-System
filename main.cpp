#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "Array2D.hpp"
#include "LinkedListSingly.hpp"

using namespace std;
using namespace std::chrono;

int main() {

    Array2D array2D;
    LinkedListSingly linkedList;

    long long reserveTime = 0;
    long long cancelTime = 0;
    long long getDetailTime = 0;

    ifstream file("flight_passenger_data.csv");
    string line;
    getline(file, line);

    // ========= ARRAY VERSION =========
    while (getline(file, line)) {

        try {
            auto start = high_resolution_clock::now();
            array2D.reserveSeat(line);
            auto end = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(end - start).count();
        }
        catch (invalid_argument&) {
            try {
            int previousID = array2D.getPreviousPassengerID(line);

            auto startGet = high_resolution_clock::now();
            cout << array2D.getPassengerDetails(previousID);
            auto endGet = high_resolution_clock::now();
            getDetailTime += duration_cast<microseconds>(endGet - startGet).count();

            auto startCancel = high_resolution_clock::now();
            array2D.cancelSeat(previousID);
            auto endCancel = high_resolution_clock::now();
            cancelTime += duration_cast<microseconds>(endCancel - startCancel).count();

            auto startReserve = high_resolution_clock::now();
            array2D.reserveSeat(line);
            auto endReserve = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(endReserve - startReserve).count();
        }
         catch (invalid_argument&) {
            // 什么都不用做，直接跳过这一行
        }
    }

    }

    cout << "\n=== ARRAY RESULT ===\n";
    array2D.printSeatGrid();
    cout << "ReserveSeat Time: " << reserveTime << " microseconds\n";
    cout << "CancelSeat Time: " << cancelTime << " microseconds\n";
    cout << "GetPassengerDetails Time: " << getDetailTime << " microseconds\n";

    // ========= LINKED LIST =========
    // 👉 reset file pointer
    file.clear();
    file.seekg(0);

    getline(file, line);

    reserveTime = cancelTime = getDetailTime = 0;

    while (getline(file, line)) {

        try {
            auto start = high_resolution_clock::now();
            linkedList.reserveSeat(line);
            auto end = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(end - start).count();
        }
        catch (invalid_argument&) {
            try {
            int previousID = linkedList.getPreviousPassengerID(line);

            auto startGet = high_resolution_clock::now();
            cout << linkedList.getPassengerDetails(previousID);
            auto endGet = high_resolution_clock::now();
            getDetailTime += duration_cast<microseconds>(endGet - startGet).count();

            auto startCancel = high_resolution_clock::now();
            linkedList.cancelSeat(previousID);
            auto endCancel = high_resolution_clock::now();
            cancelTime += duration_cast<microseconds>(endCancel - startCancel).count();

            auto startReserve = high_resolution_clock::now();
            linkedList.reserveSeat(line);
            auto endReserve = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(endReserve - startReserve).count();
        }
        catch (invalid_argument&) {
            // skip
        }
    }

    }

    cout << "\n=== LINKED LIST RESULT ===\n";
    linkedList.printSeatGrid();
    cout << "ReserveSeat Time: " << reserveTime << " microseconds\n";
    cout << "CancelSeat Time: " << cancelTime << " microseconds\n";
    cout << "GetPassengerDetails Time: " << getDetailTime << " microseconds\n";

    return 0;
}
