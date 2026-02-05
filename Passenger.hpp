#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <string>
#include <stdexcept>

using namespace std;

struct Passenger {
    public:
        int passengerID;
        string passengerName;

        Passenger(int passengerID, string passengerName) :
        passengerID(passengerID), passengerName(passengerName) {

            // Safe instantiation from CSV record
            if (passengerID < 100000 || passengerID > 109999 || passengerName.empty()) {
                throw invalid_argument("Invalid passenger details.");
            }
        }

        static Passenger* getPassengerPtrByRecord(const string& record) {

            size_t commaPos1 = record.find(',');
            if (commaPos1 == string::npos) {
                throw invalid_argument("Record must contain at least 1 comma.");
            }

            size_t commaPos2 = record.find(',', commaPos1 + 1);

            string passengerID = record.substr(0, commaPos1);
            string passengerName = (commaPos2 == string::npos)
                ? record.substr(commaPos1 + 1)
                : record.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);

            return new Passenger(stoi(passengerID), passengerName);
        } 
};

#endif