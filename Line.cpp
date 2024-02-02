// This program simulates serving customers at an ice cream store.
// It will randomly generate a service time and tolerance time for each customer according to the paramaters.
// A report will be posted at the end detailing how many customers served, how many customers' tolerances were exceeded, etc.
//
// This program was made by Lucas VanWagner

#include <iostream>
#include <fstream>

using namespace std;

class WaitingObject {
private:
    int timeEntered;
    int tolerance;
    int serviceTime;

public:
    // Class constructor and getter and setter methods
    WaitingObject(int timeIn, int toleranceIn, int serviceIn)
        : timeEntered(timeIn), tolerance(toleranceIn), serviceTime(serviceIn) {}

    int getTimeEntered() const {
        return timeEntered;
    }

    int getTolerance() const {
        return tolerance;
    }

    int getServiceTime() const {
        return serviceTime;
    }

    void setTimeEntered(int timeIn) {
        timeEntered = timeIn;
    }

    void setTolerance(int toleranceIn) {
        tolerance = toleranceIn;
    }

    void setServiceTime(int serviceIn) {
        serviceTime = serviceIn;
    }
};

class Node {
public:
    WaitingObject data;
    Node* next;

    Node(const WaitingObject& obj) : data(obj), next(nullptr) {}
};

// Making a linked list
class LinkedList {
private:
    Node* head;
    Node* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_back(const WaitingObject& obj) {
        Node* newNode = new Node(obj);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void pop_front() {
        if (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (head == nullptr) {
                tail = nullptr;
            }
        }
    }

    bool empty() const {
        return head == nullptr;
    }

    WaitingObject& front() {
        return head->data;
    }
};

class IceCreamSimulation {
private:
    int minTolerance;
    int maxTolerance;
    int minServiceTime;
    int maxServiceTime;
    int currentTime;
    int arrivalRate;
    int simulationTime;
    int customersServed;
    int totalTimeWasted;
    int totalCustomersExceededTolerance;
    int customersInLine;
    LinkedList line;

public:
    IceCreamSimulation(int minToleranceIn, int maxToleranceIn, int minServiceTimeIn, int maxServiceTimeIn, int arrivalRateIn, int simulationTimeIn)
        : currentTime(0), minTolerance(minToleranceIn), maxTolerance(maxToleranceIn), 
          minServiceTime(minServiceTimeIn), maxServiceTime(maxServiceTimeIn),
          arrivalRate(arrivalRateIn), simulationTime(simulationTimeIn), customersServed(0), 
          totalTimeWasted(0), totalCustomersExceededTolerance(0), customersInLine(0) {}

    void simulate() {
        while (currentTime < simulationTime) {
            // Check for a new customer
            if (rand() % arrivalRate == 0) {
                int tolerance = rand() % (maxTolerance - minTolerance + 1) + minTolerance;
                int serviceTime = rand() % (maxServiceTime - minServiceTime + 1) + minServiceTime;
                WaitingObject newCustomer(currentTime, tolerance, serviceTime);
                line.push_back(newCustomer);
                customersInLine++;
            }

            
            if (!line.empty()) {
                
                // Decrement service time and tolerance time of the customer
                line.front().setServiceTime(line.front().getServiceTime() - 1);

                line.front().setTolerance(line.front().getTolerance() - 1);

                // Check if tolerance has been reached
                if (line.front().getTolerance() == 0) {
                    totalCustomersExceededTolerance++;
                }

                // Check if service time has expired
                if (line.front().getServiceTime() == 0) {
                    customersServed++;
                    customersInLine--;
                    line.pop_front();
                }
                

                
            } else {
                totalTimeWasted++;
            }

            // Increment current time
            currentTime++;
        }

        // Calculate wait time
        int averageWaitTime = simulationTime / customersServed;

        // Output results
        cout << "Simulation complete" << endl << endl;

        cout << "Number of customers remaining in line: " << customersInLine << endl;
        cout << "Number of customers helped: " << customersServed << endl;
        cout << "Total amount of time with an empty line (minutes): " << totalTimeWasted / 60 << endl;
        cout << "Average customer wait time (minutes): " << averageWaitTime / 60 << endl;
        cout << "Number of customers not returning: " << totalCustomersExceededTolerance << endl;
    }
};

int main() {
    // Get input file name
    string fileName;
    cout << "Please enter the filename containing simulation parameters: ";
    cin >> fileName;

    // Read parameters from the file
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }

    string ph; // Placeholders for the unneeded strings
    int minTolerance, maxTolerance, minServiceTime, maxServiceTime, arrivalRate, simulationTime;
    inputFile >> ph >> ph >> ph >> minTolerance;
    inputFile >> ph >> ph >> ph >> maxTolerance;
    inputFile >> ph >> ph >> ph >> ph >> minServiceTime;
    inputFile >> ph >> ph >> ph >> ph >> maxServiceTime;
    inputFile >> ph >> ph >> ph >> arrivalRate;
    inputFile >> ph >> ph >> ph >> simulationTime;

    // Close the file
    inputFile.close();

    cout << "Parameters follow:" << endl << endl;

    cout << "Tolerance range: " << minTolerance << " - " <<  maxTolerance << endl;
    cout << "Service Time range: " << minServiceTime << " - " <<  maxServiceTime << endl;
    cout << "Average number of seconds between new arrivals: " << arrivalRate << endl << endl;

    cout << "Simulation begins..." << endl;

    // Run the simulation
    IceCreamSimulation simulation(minTolerance, maxTolerance, minServiceTime, maxServiceTime, arrivalRate, simulationTime);
    simulation.simulate();

    return 0;
}