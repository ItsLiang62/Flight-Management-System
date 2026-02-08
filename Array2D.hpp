#include <string>
#include <stdexcept>
#include <iostream>
#include "Seat.hpp"
#include "Reservation.hpp"

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
            if (!seatGrid[r][c].reservation)
                throw invalid_argument("No passenger details found with passenger ID.");
            return 
                to_string(seatGrid[r][c].reservation->passengerID) + " " +
                seatGrid[r][c].reservation->passengerName + "\n";
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

        void reserveSeat(const string& csvRecord) {
            Reservation* reservation = Reservation::getReservation(csvRecord);
            int r = Seat::toRowIndex(reservation->seatRow);
            int c = Seat::toColIndex(reservation->seatColumn);
            
            if (!seatGrid[r][c].reservation) {
                seatGrid[r][c].allocate(reservation);
            } else {
                delete reservation;
                reservation = nullptr;
                throw invalid_argument("Failed to reserve seat. Seat occupied.");
            }
        }

        void cancelSeat(const int& passengerID) {
            for (int r=0; r<rowTotal; r++) {
                for (int c=0; c<colTotal; c++) {
                    if (seatGrid[r][c].reservation &&
                        seatGrid[r][c].reservation->passengerID == passengerID) {
                            seatGrid[r][c].deallocate();
                            return;
                        }                  
                }
            }
            throw invalid_argument("No seat was reserved by passenger ID.");
        }

        int getPreviousPassengerID(const string& csvRecord) {
            int seatRow = Reservation::getSeatRow(csvRecord);
            char seatColumn = Reservation::getSeatColumn(csvRecord);
            for (int r=0; r<rowTotal; r++) {
                for (int c=0; c<colTotal; c++) {
                    if (seatGrid[r][c].reservation &&
                        seatGrid[r][c].reservation->seatRow == seatRow &&
                        seatGrid[r][c].reservation->seatColumn == seatColumn)
                        return seatGrid[r][c].reservation->passengerID;
                }
            }
            throw invalid_argument("No previous passenger found on the to be reserved seat.");
        }

        string getPassengerDetails(const int& passengerID) {
            for (int r=0; r<rowTotal; r++) {
                for (int c=0; c<colTotal; c++) {
                    if (seatGrid[r][c].reservation &&
                        seatGrid[r][c].reservation->passengerID == passengerID)
                        return getPassengerDetailsByIndex(r, c);
                }
            }
            throw invalid_argument("No passenger details found with passenger ID.");
        }

        void listPassengerBySeatRow(const int& seatRow) {
            string passengerManifest = "";
            int r = Seat::toRowIndex(seatRow);
            for (int c=0; c<colTotal; c++) {
                if (seatGrid[r][c].reservation) {
                    string passengerDetails = getPassengerDetailsByIndex(
                        Seat::toRowIndex(seatRow), c
                    );
                    passengerManifest += passengerDetails;
                }
            }
            cout << passengerManifest << endl;
        }

        void listPassengerByCategory(const string& category) {
            string passengerList = "";
            for (int r=0; r<rowTotal; r++) {
                for (int c=0; c<colTotal; c++) {
                    if (seatGrid[r][c].reservation &&
                        seatGrid[r][c].category == category) {
                        string passengerDetails = getPassengerDetailsByIndex(r, c);
                        passengerList += passengerDetails;
                    }
                }
            }
            cout << passengerList << endl;
        }

        void printSeatGrid() {
            cout << "\nOccupied Seats:\n";
            for (int r = 0; r < rowTotal; r++) {
                for (int c = 0; c < colTotal;    c++) {
                    cout << seatGrid[r][c].row << seatGrid[r][c].column << ": ";
                    if (seatGrid[r][c].reservation) {
                        cout << seatGrid[r][c].reservation->passengerID
                            << " " << seatGrid[r][c].reservation->passengerName;
                    }
                    cout << endl;
                }
            }
        }
};