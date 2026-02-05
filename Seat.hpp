#ifndef SEAT_HPP
#define SEAT_HPP

#include <string>
#include <stdexcept>
#include "Reservation.hpp"



using namespace std;

struct Seat {
    private:
        static inline string validColumns = "ABCDEF";

    public:
        int row;
        char column;
        string category;
        Reservation* reservation;

        Seat() : row(1), column('A'), category("Economy"), reservation(nullptr) {}
    
        // Seat can have no reservation if not provided
        Seat(int row, char column, Reservation* reservation = nullptr) :
        row(row), column(column), reservation(reservation) {

            // Safe instantiation from CSV record
            checkFields();

            // Define category based on seat row
            if (row <= 3) category = "First";
            else if (row <= 10) category = "Business";
            else category = "Economy";
        }

        void checkFields() {
            if (row < 1 || row > 30) throw invalid_argument("Invalid seat row. Must be in between 1 to 30.");
            
            bool isValidColumn = false; 

            for (char validColumn : validColumns) {
                if (column == validColumn) isValidColumn = true;
            }
            if (!isValidColumn) throw invalid_argument("Invalid seat column. Must be in between A to F.");
        }

        void allocate(Reservation* reservation) {
            if (this->reservation != nullptr)
                throw logic_error("Failed to allocate seat. Seat already occupied.");
            this->reservation = reservation;
        }

        void deallocate() {
            if (!reservation)
                throw logic_error("Seat already empty.");
            delete reservation;
            reservation = nullptr;
        }

        // Row Index / Seat Row conversion methods
        static int toRowIndex(const int& seatRow) {
            if (seatRow < 1 || seatRow > 30) 
                throw invalid_argument("Invalid seat row. Must be in between 1 to 30.");
            return seatRow - 1;
        }

        static int toSeatRow(const int& rowIndex) {
            if (rowIndex < 0 || rowIndex > 29) 
                throw invalid_argument("Invalid row index. Must be in between 0 to 29.");
            return rowIndex + 1;
        }

        // Col Index / Seat Col conversion methods
        static int toColIndex(const char& seatColumn) {
            for (int i=0; i<validColumns.length(); i++) {
                if (validColumns[i] == seatColumn) {
                    return i;
                }
            }
            throw invalid_argument("Invalid seat column. Must be in between A to F.");
        }

        static char toSeatColumn(const int& colIndex) { 
            if (colIndex < 0 || colIndex > validColumns.length()-1) 
                throw invalid_argument("Invalid column index. Must be in between 0 to 5.");
            return validColumns[colIndex];
        }
};

#endif