#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
using namespace std;

class Driver {
public:
    string name;
    string licenseNumber;
    string cnic;
    string phone;

    Driver(string n, string lic, string c, string p)
        : name(n), licenseNumber(lic), cnic(c), phone(p) {}
};

class Passenger {
public:
    string name;
    string id;
    string phone;
    string password;

    Passenger(string n, string i, string p, string pass)
        : name(n), id(i), phone(p), password(pass) {}
};

class Ride {
public:
    string passengerName;
    string driverName;
    int distance;
    int price;
    int rating;

    Ride(string p, string d, int dist, int cost)
        : passengerName(p), driverName(d), distance(dist), price(cost), rating(0) {}
};

vector<Driver> drivers;
vector<Passenger> passengers;
vector<Ride> rideHistory;

Passenger* loggedInPassenger = 0;
Driver* loggedInDriver = 0;
const int pricePerKm = 50;

bool isDigitsOnly(const string& str) {
    for (int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool isValidPassword(const string& pass) {
    if (pass.length() < 10) return false;
    for (int i = 0; i < pass.length(); i++) {
        if (!isalnum(pass[i])) return false;
    }
    return true;
}

void registerDriver() {
    string name, license, cnic, phone;
    cout << "Enter Driver Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter Driving License Number: ";
    getline(cin, license);

    int attempts = 0;
    while (true) {
        cout << "Enter CNIC (13 digits): ";
        getline(cin, cnic);
        if (cnic.length() == 13 && isDigitsOnly(cnic)) break;

        cout << "? Invalid CNIC. Must be exactly 13 digits.\n";
        attempts++;
        if (attempts >= 3) {
            cout << "? Too many invalid attempts. Registration cancelled.\n";
            return;
        }
    }

    cout << "Enter Phone Number: ";
    getline(cin, phone);

    drivers.push_back(Driver(name, license, cnic, phone));
    cout << "Driver registered successfully!\n";
}

void registerPassenger() {
    string name, id, phone, password, confirmPass;
    cout << "Enter Passenger Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter Passenger ID: ";
    getline(cin, id);
    
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    
    while (true) {
        cout << "Set Password (min 10 alphanumeric characters): ";
        getline(cin, password);
        
        if (!isValidPassword(password)) {
            cout << "? Password must be at least 10 characters and contain only letters/numbers\n";
            continue;
        }
        
        cout << "Confirm Password: ";
        getline(cin, confirmPass);
        
        if (password != confirmPass) {
            cout << "? Passwords do not match. Try again.\n";
        } else {
            break;
        }
    }
    
    passengers.push_back(Passenger(name, id, phone, password));
    cout << "Passenger registered successfully!\n";
}

void loginPassenger() {
    string name, id, password;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter Passenger ID: ";
    getline(cin, id);
    
    cout << "Enter Password: ";
    getline(cin, password);

    for (int i = 0; i < passengers.size(); i++) {
        if (passengers[i].name == name && passengers[i].id == id && passengers[i].password == password) {
            loggedInPassenger = &passengers[i];
            cout << "Login successful! Welcome " << name << "!\n";
            return;
        }
    }
    cout << "Login failed. Invalid credentials.\n";
}

void loginDriver() {
    string name, cnic;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);

    int attempts = 0;
    while (true) {
        cout << "Enter CNIC (13 digits): ";
        getline(cin, cnic);
        if (cnic.length() == 13 && isDigitsOnly(cnic)) break;

        cout << "? Invalid CNIC. Must be exactly 13 digits.\n";
        attempts++;
        if (attempts >= 3) {
            cout << "? Too many invalid attempts. Login cancelled.\n";
            return;
        }
    }

    for (int i = 0; i < drivers.size(); i++) {
        if (drivers[i].name == name && drivers[i].cnic == cnic) {
            loggedInDriver = &drivers[i];
            cout << "Login successful! Welcome " << name << "!\n";
            return;
        }
    }
    cout << "Login failed. Driver not found.\n";
}

void bookRide() {
    if (loggedInPassenger == 0) {
        cout << "You must log in as a passenger first.\n";
        return;
    }

    if (drivers.size() == 0) {
        cout << "No drivers available.\n";
        return;
    }

    int distance;
    cout << "Enter distance you want to travel (km): ";
    cin >> distance;
    int price = distance * pricePerKm;

    srand(time(0));
    int randomIndex = rand() % drivers.size();
    Driver* assignedDriver = &drivers[randomIndex];

    cout << "Driver " << assignedDriver->name << " assigned to your ride.\n";
    cout << "Total price for " << distance << " km is: " << price << " Rs\n";

    char confirm;
    cout << "Do you accept the ride? (Y/N): ";
    cin >> confirm;
    cin.ignore();
    if (confirm == 'Y' || confirm == 'y') {
        rideHistory.push_back(Ride(loggedInPassenger->name, assignedDriver->name, distance, price));
        cout << "Ride booked successfully!\n";
        
        Ride& currentRide = rideHistory.back();
        int rating;
        cout << "Please rate your ride (1-5 stars): ";
        cin >> rating;
        
        if (rating >= 1 && rating <= 5) {
            currentRide.rating = rating;
            cout << "Thank you for your rating of " << rating << " stars!\n";
        } else {
            cout << "Invalid rating. Rating not recorded.\n";
        }
        cin.ignore();
    } else {
        cout << "Ride cancelled.\n";
    }
}

void showRideHistory() {
    cout << "\n--- Ride History ---\n";
    if (rideHistory.empty()) {
        cout << "No rides booked yet.\n";
        return;
    }

    for (int i = 0; i < rideHistory.size(); i++) {
        cout << i + 1 << ". Passenger: " << rideHistory[i].passengerName
             << ", Driver: " << rideHistory[i].driverName
             << ", Distance: " << rideHistory[i].distance << " km"
             << ", Price: " << rideHistory[i].price << " Rs"
             << ", Rating: ";
        
        if (rideHistory[i].rating > 0) {
            for (int j = 0; j < rideHistory[i].rating; j++) {
                cout << "*";
            }
            cout << " (" << rideHistory[i].rating << "/5)";
        } else {
            cout << "Not rated";
        }
        cout << endl;
    }
}

void rateRide() {
    if (loggedInPassenger == 0) {
        cout << "Please log in as passenger first.\n";
        return;
    }
    
    vector<int> unratedIndices;
    cout << "\n--- Unrated Rides ---\n";
    for (int i = 0; i < rideHistory.size(); i++) {
        if (rideHistory[i].passengerName == loggedInPassenger->name && rideHistory[i].rating == 0) {
            cout << unratedIndices.size() + 1 << ". Driver: " << rideHistory[i].driverName
                 << ", Distance: " << rideHistory[i].distance << " km"
                 << ", Price: " << rideHistory[i].price << " Rs\n";
            unratedIndices.push_back(i);
        }
    }
    
    if (unratedIndices.empty()) {
        cout << "No unrated rides found.\n";
        return;
    }
    
    int choice;
    cout << "Select a ride to rate (1-" << unratedIndices.size() << "): ";
    cin >> choice;
    
    if (choice < 1 || choice > unratedIndices.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    
    int rideIndex = unratedIndices[choice - 1];
    int rating;
    cout << "Rate your ride with " << rideHistory[rideIndex].driverName 
         << " (1-5 stars): ";
    cin >> rating;
    
    if (rating >= 1 && rating <= 5) {
        rideHistory[rideIndex].rating = rating;
        cout << "Thank you for your rating of " << rating << " stars!\n";
    } else {
        cout << "Invalid rating. Please enter between 1-5.\n";
    }
}

void showAllDrivers() {
    cout << "\n--- Registered Drivers ---\n";
    if (drivers.empty()) {
        cout << "No drivers registered yet.\n";
        return;
    }
    for (int i = 0; i < drivers.size(); i++) {
        cout << i + 1 << ". Name: " << drivers[i].name
             << ", License: " << drivers[i].licenseNumber
             << ", CNIC: " << drivers[i].cnic
             << ", Phone: " << drivers[i].phone << endl;
    }
}

void showMenu() {
    cout << "\n--- WELCOME TO MY DRIVE ---\n";
    cout << "1. Register Driver\n";
    cout << "2. Register Passenger\n";
    cout << "3. Login as Passenger\n";
    cout << "4. Login as Driver\n";
    cout << "5. Book Ride (Passenger only)\n";
    cout << "6. Show Ride History\n";
    cout << "7. Rate a Ride\n";
    cout << "8. Exit\n";
    cout << "9. Show All Registered Drivers\n";
    cout << "Choose an option: ";
}

int main() {
    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: registerDriver(); break;
            case 2: registerPassenger(); break;
            case 3: loginPassenger(); break;
            case 4: loginDriver(); break;
            case 5: bookRide(); break;
            case 6: showRideHistory(); break;
            case 7: rateRide(); break;
            case 8: cout << "Exiting the app.\n"; return 0;
            case 9: showAllDrivers(); break;
            default: cout << "Invalid choice. Try again.\n";
        }
    }
}

