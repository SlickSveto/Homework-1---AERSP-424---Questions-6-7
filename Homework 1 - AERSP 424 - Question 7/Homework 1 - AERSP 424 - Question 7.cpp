/******************************************************************************

AERSP 424: Advanced Computer Programming

Homework 1 Spring 2024
Question 6 and 7

Svetoslav Trayanov 2/19/2024

*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>

// Define the structure for a flight
struct Flight {
    std::string origin;
    std::string destination;
    int distance;
};

// Forward declaration of Pilot class
class Pilot;

// Define the Plane class
class Plane {
private:
    double pos;
    double vel;
    double distance;
    bool at_SCE;
    std::string origin;
    std::string destination;
    std::vector<Flight> flights;

public:
    // Constructor
    Plane(std::string from, std::string to) : origin(from), destination(to) {
        // Define the flights available
        flights = {
            {"SCE", "PHL", 160},
            {"SCE", "ORD", 640},
            {"SCE", "EWR", 220}
        };

        // Find the distance for the flight
        bool found = false;
        for (const auto& flight : flights) {
            if ((flight.origin == origin && flight.destination == destination) ||
                (flight.origin == destination && flight.destination == origin)) {
                distance = flight.distance;
                found = true;
                break;
            }
        }

        // Initialize position, velocity, and at_SCE status
        pos = 0;
        vel = 0;
        at_SCE = false;

        // Print plane creation information
        std::cout << "Plane created at: " << this << std::endl;
        std::cout << "Flight from " << origin << " to " << destination << std::endl;
    }

    // Destructor
    ~Plane() {
        std::cout << "Plane Destroyed" << std::endl;
        std::cout << "############################################ End of Question 6 ##################################" << std::endl;
        delete myPlane; //always delete pointer, so there no memeory leaks
    }

    // Function to operate the plane
    void operate(double dt) {
        if (pos < distance) {
            pos += vel * dt;
            at_SCE = true;
        }
        else if (destination == "SCE") {
            at_SCE = true;
            std::string temp = origin;
            origin = destination;
            destination = temp;
            pos = 0;
        }
        else {
            std::string temp = origin;
            origin = destination;
            destination = temp;
            pos = 0;
        }
    }

    // Function to switch pilots
    void switchPilot(Pilot* inFlightPilot, Pilot* atGatePilot) {
        if (myPlane == inFlightPilot) {
            myPlane = atGatePilot;
        }
        else {
            myPlane = inFlightPilot;
        }
    }

    // Getter and setter functions
    double getPos() const { return pos; }
    std::string getOrigin() const { return origin; }
    std::string getDestination() const { return destination; }
    bool getAtSCE() const { return at_SCE; }
    double getVel() const { return vel; }
    void setVel(double newVel) { vel = newVel; }

    // Pointer to the pilot currently in control
    Pilot* myPlane;
};

// Define the Pilot class
class Pilot {
private:
    std::string name;

public:
    // Constructor
    Pilot(std::string pilotName) : name(pilotName) {
        // Print pilot creation information
        std::cout << "Pilot " << name << " with certificate number " << &name << " is at the gate, ready to board the plane." << std::endl;
    }

    // Destructor
    ~Pilot() {
        std::cout << name << " is out of the plane.\n";
        delete myPlane; //Q7 deletes plane so Plane destroyed is shown on the printed output at the bottom. 
    }

    // Getter for pilot name
    std::string getName() const {
        return name;
    }

    // Pointer to Plane
    Plane* myPlane;
};

int main() {

    std::cout << "############################################### Question 6 #####################################" << std::endl;

    // Create two pilots
    Pilot Pilot1("Alpha");
    Pilot Pilot2("Bravo");

    // Initialize the plane
    double airplaneSpeed = 500;
    std::string originAirport = "SCE";
    std::string destinationAirport = "EWR";
    Plane* plane = new Plane(originAirport, destinationAirport);
    plane->setVel(airplaneSpeed);

    // Assign the plane to Pilot1
    Pilot1.myPlane = plane;
    Pilot2.myPlane = {};
    plane->myPlane = &Pilot1;

    double airTime;
    double timeStep = 50;
    int maxIterations = 20;

    for (int i = 0; i < maxIterations; i++) {
        // Find the current pilot
        Pilot* nowPilot = nullptr;
        Pilot* oldPilot = nullptr;
        if (plane->myPlane == &Pilot1) {
            nowPilot = &Pilot1;
            oldPilot = &Pilot2;
        }
        else {
            nowPilot = &Pilot2;
            oldPilot = &Pilot1;
        }
        std::cout << "Pilot " << nowPilot->getName() << " with certificate number " << nowPilot << " is in control of a plane: " << plane << std::endl;
        std::cout << "Pilot " << oldPilot->getName() << " with certificate number " << oldPilot << " is in control of a plane: " << Pilot2.myPlane << std::endl;

        // Simulate a flight
        if (i == 0) {
            airTime = 0;
        }
        else {
            airTime = timeStep / 3600;
        }
        plane->operate(airTime);

        // Check if landed at SCE
        if (plane->getAtSCE()) {
            std::cout << "The plane " << plane << " is at SCE." << std::endl;

            // Switch pilots
            if (nowPilot == &Pilot1) {
                plane->switchPilot(&Pilot1, &Pilot2);
            }
            else {
                plane->switchPilot(&Pilot2, &Pilot1);
            }
        }

    }


    return 0;
}