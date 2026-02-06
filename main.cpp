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

    // ========= ARRAY VERSION =========
    while (getline(file, line)) {

        try {
            auto start = high_resolution_clock::now();
            array2D.reserveSeat(line);
            auto end = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(end - start).count();
        }
        catch (invalid_argument&) {

            int previousID;

            // getPreviousPassengerID
            previousID = array2D.getPreviousPassengerID(line);

            // getPassengerDetails (just print)
            auto startGet = high_resolution_clock::now();
            cout << array2D.getPassengerDetails(previousID);
            auto endGet = high_resolution_clock::now();
            getDetailTime += duration_cast<microseconds>(endGet - startGet).count();

            // cancelSeat
            auto startCancel = high_resolution_clock::now();
            array2D.cancelSeat(previousID);
            auto endCancel = high_resolution_clock::now();
            cancelTime += duration_cast<microseconds>(endCancel - startCancel).count();

            // reserve again
            auto startReserve = high_resolution_clock::now();
            array2D.reserveSeat(line);
            auto endReserve = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(endReserve - startReserve).count();
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

    reserveTime = cancelTime = getDetailTime = 0;

    while (getline(file, line)) {

        try {
            auto start = high_resolution_clock::now();
            linkedList.reserveSeat(line);
            auto end = high_resolution_clock::now();
            reserveTime += duration_cast<microseconds>(end - start).count();
        }
        catch (invalid_argument&) {

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
    }

    cout << "\n=== LINKED LIST RESULT ===\n";
    linkedList.printSeatGrid();
    cout << "ReserveSeat Time: " << reserveTime << " microseconds\n";
    cout << "CancelSeat Time: " << cancelTime << " microseconds\n";
    cout << "GetPassengerDetails Time: " << getDetailTime << " microseconds\n";

    return 0;
}
