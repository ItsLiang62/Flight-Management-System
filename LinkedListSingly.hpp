#include <string>
#include <stdexcept>
#include <iostream>
#include "Seat.hpp"
#include "Reservation.hpp"

using namespace std;

struct Node {
    public:
        Seat seat;
        Node* next;

        Node(Seat seat, Node* next = nullptr) :
        seat(seat), next(next) {}
};

class LinkedListSingly {
    private:
        Node* head = nullptr; // pointer to first node
        
        void insertBack(const Seat& seat) {
            Node* newNodePtr = new Node(seat);

            if (!head) {
                head = newNodePtr;
                return;
            }

            Node* curr = head;
            while (curr->next)
                curr = curr->next;

            curr->next = newNodePtr;
        }

        void createSeats(int rows, int cols) {
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    insertBack(Seat(Seat::toSeatRow(r), Seat::toSeatColumn(c)));
                }
            }
        }

        Node* getNodePtrByIndex(const int& r, const int& c) {
            if (head == nullptr) throw std::logic_error("Empty linked list.");

            // Never reached, for safety only
            if (r < 0 || c < 0 || r >= rowTotal || c >= colTotal) 
                throw invalid_argument("Invalid indices.");

            int index = r * colTotal + c;

            Node* curr = head;
            // If index 3, curr traverses 3 times to point to target node
            for (int i=0; i<index; i++) {
                if (curr->next==nullptr) throw std::out_of_range("Indices out of range");
                curr = curr->next;
            }
            return curr;
        }

        string getPassengerDetailsByNode(Node* nodePtr) {
            if (!nodePtr || !nodePtr->seat.reservation) return "";
            return
                to_string(nodePtr->seat.reservation->passengerID) + " " +
                nodePtr->seat.reservation->passengerName + "\n";
        }

    public:
        int rowTotal = 30;
        int colTotal = 6;

        LinkedListSingly() {
            createSeats(rowTotal, colTotal);
        }

        ~LinkedListSingly() {
            Node* curr = head;
            while (curr) {
                Node* next = curr->next;
                delete curr;
                curr = next;
            }
        }

        void reserveSeat(const string& csvRecord) {
            Reservation* reservation = Reservation::getReservation(csvRecord);
            int r = Seat::toRowIndex(reservation->seatRow);
            int c = Seat::toColIndex(reservation->seatColumn);

            Node* nodePtr = getNodePtrByIndex(r, c);
            if (!nodePtr->seat.reservation) {
                nodePtr->seat.allocate(reservation);
            } else {
                delete reservation;
                reservation = nullptr;
                throw invalid_argument("Failed to reserve seat. Seat occupied.");
            }
        }

        void cancelSeat(const int& passengerID) {
            Node* curr = head;
            while (curr) {
                if (curr->seat.reservation &&
                    curr->seat.reservation->passengerID == passengerID)
                    curr->seat.deallocate();
                    return;
                curr = curr->next;
            }
            throw invalid_argument("No seat was reserved by passenger ID.");
        }

        int getPreviousPassengerID(const string& csvRecord) {
            int seatRow = Reservation::getSeatRow(csvRecord);
            char seatColumn = Reservation::getSeatColumn(csvRecord);
            Node* nodePtr = getNodePtrByIndex(
                Seat::toRowIndex(seatRow),
                Seat::toColIndex(seatColumn)
            );
            return nodePtr->seat.reservation->passengerID;
        }

        string getPassengerDetails(const int& passengerID) {
            Node* curr = head;
            while (curr) {
                if (curr->seat.reservation &&
                    curr->seat.reservation->passengerID == passengerID)
                    return getPassengerDetailsByNode(curr);
                curr = curr->next;
            }
            throw invalid_argument("No passenger details found with passenger ID.");
        }
        
        void listPassengersBySeatRow(const int& seatRow) {
            int r = Seat::toRowIndex(seatRow);
            string passengerList = "";

            Node* curr = head;
            
            // Traverse until end of node with node index tracking
            for (int i=0; curr; i++, curr=curr->next) {
                // Add to list if node on target row and has occupied seat 
                if (i / colTotal == r && curr->seat.reservation)
                    passengerList += getPassengerDetailsByNode(curr);
            }
            cout << passengerList << endl;
        }

        void listPassengersByCategory(const string& category) {
            string passengerList = "";
            Node* curr = head;

            // Traverse until end of node
            while (curr) {
                // Add to list if node on target category and has occupied seat
                if (curr->seat.category == category && curr->seat.reservation)
                    passengerList += getPassengerDetailsByNode(curr);
                curr = curr->next;
            }
            cout << passengerList << endl;
        }

        void printSeatGrid() {
            if (!head) return;

            // Print column headers
            cout << "   ";
            for (char col : "ABCDEF") cout << col << " ";
            cout << endl;

            Node* curr = head;
            for (int r = 0; r < rowTotal; r++) {
                cout.width(2);
                cout << (r + 1) << " ";
                for (int c = 0; c < colTotal; c++) {
                    if (!curr) break;
                    cout << (curr->seat.reservation ? "X" : "O") << " ";
                    curr = curr->next;
                }
                cout << endl;
            }

            // Print list of occupied seats
            cout << "\nOccupied Seats:\n";
            curr = head;
            while (curr) {
                if (curr->seat.reservation) {
                    cout << "Row " << curr->seat.row 
                        << " Column " << curr->seat.column 
                        << " -> ID: " << curr->seat.reservation->passengerID
                        << ", Name: " << curr->seat.reservation->passengerName 
                        << endl;
                }
                curr = curr->next;
            }
        }
};