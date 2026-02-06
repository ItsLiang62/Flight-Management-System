#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include <string>
#include <stdexcept>
#include <regex>

using namespace std;

struct Reservation {
    public:
        int passengerID;
        string passengerName;
        int seatRow;
        char seatColumn;

        Reservation(int passengerID, string passengerName, int seatRow, char seatColumn) : 
            passengerID(passengerID), 
            passengerName(passengerName), 
            seatRow(seatRow), 
            seatColumn(seatColumn) {

            // Safe instantiation from CSV record
            if (passengerID < 100000 || passengerID > 109999 || passengerName.empty()) {
                throw invalid_argument("Invalid passenger details.");
            }
        }

        static Reservation* getReservation(const string& csvRecord) {

            // Ensure valid CSV record
            regex csvRecordPattern(R"(^(10\d{4}),([a-zA-Z\s]+),([1-9]|1[0-9]|2[0-9]|30),([A-F]),(Economy|Business|First)$)");
            smatch matches;

            if (regex_match(csvRecord, matches, csvRecordPattern)) {
                int passengerID = stoi(matches[1]);
                string passengerName = matches[2];
                int seatRow = stoi(matches[3]);
                char seatColumn = matches[4].str()[0];

                return new Reservation(passengerID, passengerName, seatRow, seatColumn);
            }
            throw invalid_argument("Invalid CSV record format.");
        } 

        static int getSeatRow(const string& csvRecord) {

            Reservation* newReservation = getReservation(csvRecord);
            int seatRow = newReservation->seatRow;
            delete newReservation;
            newReservation = nullptr;
            return seatRow;
        }

        static char getSeatColumn(const string& csvRecord) {

            Reservation* newReservation = getReservation(csvRecord);
            int seatColumn = newReservation->seatColumn;
            delete newReservation;
            newReservation = nullptr;
            return seatColumn;
        }
};

#endif