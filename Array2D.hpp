#include <string>
#include <stdexcept>
#include <iostream>
#include "Seat.hpp"
#include "Passenger.hpp"

using namespace std;

class Array2D {
    private:
        Seat** seatGrid;
        int rowTotal = 30;
        int colTotal = 6;

        void allocate() {
            seatGrid = new Seat*[rowTotal];
            for (int r=0; r<rowTotal; r++) {            
                // Each Seat pointer in array points to array of Seat
                seatGrid[r] = new Seat[colTotal];
                for (int c=0; c<colTotal; c++) {
                    seatGrid[r][c] = Seat(Seat::toSeatRow(r), Seat::toSeatColumn(c));
                }
            }
        }

        string getPassengerDetailsByIndex(const int& r, const int& c) {
            if (!seatGrid[r][c].passenger) return "";
            return 
                to_string(seatGrid[r][c].passenger->passengerID) + " " +
                seatGrid[r][c].passenger->passengerName + "\n";
        }

    public:
        Array2D() {
            allocate();
        }

        ~Array2D() {
            for (int r = 0; r < rowTotal; r++) {
                    delete[] seatGrid[r];
                }
            delete[] seatGrid;
        }

        void reserveSeat(const int& seatRow, const char& seatColumn, const string& csvRecord) {
            seatGrid[Seat::toRowIndex(seatRow)][Seat::toColIndex(seatColumn)].allocate(Passenger::getPassengerPtrByRecord(csvRecord));
        }

        void cancelSeat(const int& seatRow, const char& seatColumn) {
            seatGrid[Seat::toRowIndex(seatRow)][Seat::toColIndex(seatColumn)].deallocate();
        }

        string getPassengerDetailsByID(const int& passengerID) {
            for (int r=0; r<rowTotal; r++) {
                for (int c=0; c<colTotal; c++) {
                    if (!seatGrid[r][c].passenger) continue;
                    if (seatGrid[r][c].passenger->passengerID == passengerID) {
                        return getPassengerDetailsByIndex(r, c);
                    }
                }
            }
            throw invalid_argument("No passenger details found with passenger ID.");
        }

        void listPassengerBySeatRow(const int& seatRow) {
            string passengerList = "";
            for (int c=0; c<colTotal; c++) {
                if (!getPassengerDetailsByIndex(Seat::toRowIndex(seatRow), c).empty()) 
                    passengerList += getPassengerDetailsByIndex(Seat::toRowIndex(seatRow), c);
            }
            cout << passengerList << endl;
        }

        void listPassengerByCategory(const string& category) {
            string passengerList = "";
            for (int r=0; r<rowTotal; r++) {
                for (int c=0; c<colTotal; c++) {
                    if (seatGrid[r][c].category == category) {
                        if (!getPassengerDetailsByIndex(r, c).empty()) 
                            passengerList += getPassengerDetailsByIndex(r, c);
                    }
                }
            }
            cout << passengerList << endl;
        }

        void printSeatGrid() {
            // Print column headers
            cout << "   ";
            for (char col : "ABCDEF") cout << col << " ";
            cout << endl;

            for (int r = 0; r < rowTotal; r++) {
                cout.width(2);
                cout << (r + 1) << " ";
                for (int c = 0; c < colTotal; c++) {
                    cout << (seatGrid[r][c].passenger ? "X" : "O") << " ";
                }
                cout << endl;
            }

            // Print list of occupied seats
            cout << "\nOccupied Seats:\n";
            for (int r = 0; r < rowTotal; r++) {
                for (int c = 0; c < colTotal; c++) {
                    if (seatGrid[r][c].passenger) {
                        cout << "Row " << seatGrid[r][c].row
                            << " Column " << seatGrid[r][c].column
                            << " -> ID: " << seatGrid[r][c].passenger->passengerID
                            << ", Name: " << seatGrid[r][c].passenger->passengerName
                            << endl;
                    }
                }
            }
        }
};