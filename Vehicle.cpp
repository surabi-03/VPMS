#include <iostream>
#include <fstream>
#include <sstream>
#include<string>
#include <ctime>
#include <map>
using namespace std;

const int CAR = 1;
const int SCOOTER = 2;

struct Vehicle {
    int type;               
    int num;                
    int row;               
    int col;                
    time_t arrivalTime;    
    time_t departureTime; 
      
};

int parkinfo[4][10] = {0};  
int vehcount = 0;           
int carcount = 0;           
int scootercount = 0;     

void storeIncomingVehicle(int type, int num, const char* arrivalTime)
{
    ofstream outputFile("IncomingVehicles.txt", ios::app);
    if (outputFile.is_open())
    {
        outputFile << type << "," << num << "," << arrivalTime << endl;
        outputFile.close();
        cout << "\nIncoming vehicle information stored in IncomingVehicles.txt\n";
    }
    else
    {
        cout << "\nUnable to open the file for writing.";
    }
}

void add(int type, int num, int row, int col) 
{
 
    parkinfo[row][col] = num;
    vehcount++;
    if (type == CAR) {
        carcount++;
    } else if (type == SCOOTER) {
        scootercount++;
    }
    time_t now = time(0);
    char* arrivalTime = ctime(&now);
    storeIncomingVehicle(type, num, arrivalTime);
}

void del(Vehicle* v) 
{
    int row = v->row;
    int col = v->col;
    int type = v->type;

    parkinfo[row][col] = 0;
    delete v;
    vehcount--;
    if (type == CAR) {
        carcount--;
    } else if (type == SCOOTER) {
        scootercount--;
    }
}

void getfreerowcol(int type, int arr[]) 
{
    int startRow, endRow;
    if (type == CAR) {
        startRow = 0;
        endRow = 1;
    } else if (type == SCOOTER) {
        startRow = 2;
        endRow = 3;
    }

    for (int r = startRow; r <= endRow; r++) {
        for (int c = 0; c < 10; c++) {
            if (parkinfo[r][c] == 0) {
                arr[0] = r;
                arr[1] = c;
                return;
            }
        }
    }

    arr[0] = -1;
    arr[1] = -1;
}

void getrcbyinfo(int type, int num, int arr[]) 
{
    int startRow, endRow;
    if (type == CAR) {
        startRow = 0;
        endRow = 1;
    } else if (type == SCOOTER) {
        startRow = 2;
        endRow = 3;
    }

    for (int r = startRow; r <= endRow; r++) {
        for (int c = 0; c < 10; c++) {
            if (parkinfo[r][c] == num) {
                arr[0] = r;
                arr[1] = c;
                return;
            }
        }
    }

    arr[0] = -1;
    arr[1] = -1;
}

void saveData()
{
    ofstream outputFile("ParkData.txt");
    if (outputFile.is_open())
    {
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 10; c++)
            {
                if (parkinfo[r][c] != 0)
                {
                    outputFile << r << "," << c << "," << parkinfo[r][c] << endl;
                }
            }
        }
        outputFile.close();
        cout << "\nData saved to ParkData.txt\n";
    }
    else
    {
        cout << "\nUnable to open the file for writing.";
    }
}


float calculateTotalRevenue()
{
    ifstream inputFile("IncomingVehicles.txt");
    if (inputFile.is_open())
    {
        string line;
        int totalVehicles = 0;
        while (getline(inputFile, line))
        {
            totalVehicles++;
        }
        inputFile.close();
        float rate1 = 25.0;
        float rate = 15.0;
        float totalRevenue = (carcount * rate1)+(scootercount*rate);

        return totalRevenue;
    }
    else
    {
        cout << "\nUnable to open the file for reading.";
        return 0.0;
    }
}
float calculateRevenue()
{
    ifstream inputFile("IncomingVehicles.txt");
    if (inputFile.is_open())
    {
        string line;
        float totalRevenue = 0.0;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string token;

            getline(ss, token, ','); // Vehicle Type
            int type = 0;
            try {
                type = stoi(token);
            } catch (const std::exception& e) {
               // cout << "Invalid data format in the file." << endl;
                continue;
            }

            getline(ss, token, ','); // Vehicle Number
            getline(ss, token);      // Arrival Time

            if (type == CAR) {
                totalRevenue += 25.0;
            } else if (type == SCOOTER) {
                totalRevenue += 15.0;
            }
        }

        inputFile.close();

        return totalRevenue;
    }
    else
    {
        cout << "\nUnable to open the file for reading.";
        return 0.0;
    }

}
void display() {
    cout << "\nVehicle Parking Management System";
    cout << "\n--------------------------------";

    cout << "\n\nCAR";
    for (int r = 0; r < 2; r++) {
        cout << "\nRow " << r + 1 << ": ";
        for (int c = 0; c < 10; c++) {
            if (parkinfo[r][c] != 0) {
                cout << parkinfo[r][c] << " ";
            } else {
                cout << " - ";
            }
        }
    }

    cout << "\n\nSCOOTER";
    for (int r = 2; r < 4; r++) {
        cout << "\nRow " << r + 1 << ": ";
        for (int c = 0; c < 10; c++) {
            if (parkinfo[r][c] != 0) {
                cout << parkinfo[r][c] << " ";
            } else {
                cout << " - ";
            }
        }
    }

    saveData();
    cout << "\n\nTotal Vehicles: " << vehcount;
    cout << "\nTotal Cars: " << carcount;
    cout << "\nTotal Scooters: " << scootercount << endl;
}

void displayIncomingVehicles()
{
    ifstream inputFile("IncomingVehicles.txt");
    if (inputFile.is_open())
    {
        string line;
        cout << "\nIncoming Vehicles:\n";
        cout << "-----------------------------------\n";
        cout << "Type\tNumber\tArrival Time\n";
        cout << "-----------------------------------\n";

        map<int, int> carCount;     
        map<int, int> scooterCount; 

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string token;
            Vehicle vehicle;

            getline(ss, token, ','); 
            try {
                vehicle.type = stoi(token);
            } catch (const std::exception& e) {
                continue; 
            }

            getline(ss, token, ','); 
            try {
                vehicle.num = stoi(token);
            } catch (const std::exception& e) {
                continue; 
            }

            getline(ss, token); 

            if (vehicle.type == CAR)
            {
                carCount[vehicle.num]++;
            }
            else if (vehicle.type == SCOOTER)
            {
                scooterCount[vehicle.num]++;
            }

            cout << (vehicle.type == CAR ? "CAR" : "SCOOTER") << "\t";
            cout << vehicle.num << "\t";
            cout << token << endl;
        }

        inputFile.close();

        cout << "-----------------------------------\n";

        int mostVisitedCarNumber = 0;
        int maxCarCount = 0;

        for (const auto& pair : carCount)
        {
            if (pair.second > maxCarCount)
            {
                mostVisitedCarNumber = pair.first;
                maxCarCount = pair.second;
            }
        }

        cout << "Most visited Car number: " << mostVisitedCarNumber << endl;
        cout << "Number of times visited: " << maxCarCount << endl;

       
        int mostVisitedScooterNumber = 0;
        int maxScooterCount = 0;

        for (const auto& pair : scooterCount)
        {
            if (pair.second > maxScooterCount)
            {
                mostVisitedScooterNumber = pair.first;
                maxScooterCount = pair.second;
            }
        }

        cout << "Most visited Scooter number: " << mostVisitedScooterNumber << endl;
        cout << "Number of times visited: " << maxScooterCount << endl;
    }
    else
    {
        cout << "\nUnable to open the file for reading.";
    }
}

int main() {
    string username, password;
    int choice;
    char* at;
    char* dt;
    float cost;
 cout << "\n\t\t\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\n";
    cout << "\t\t\xdb                                               \xdb\n";
    cout << "\t\t\xdb       =============================           \xdb\n";
    cout << "\t\t\xdb        VEHICLE PARKING MANAGEMENT             \xdb\n";
    cout << "\t\t\xdb       =============================           \xdb\n";
    cout << "\t\t\xdb                                               \xdb\n";
    cout << "\t\t\xdb       BY RakshithTarun && Surabi R Rao        \xdb\n";
    cout << "\t\t\xdb                                               \xdb\n";
    cout << "\t\t\xdb                 ISE                           \xdb\n";
    cout << "\t\t\xdb                                               \xdb\n";
    cout << "\t\t\xdb      (1JT20IS030) && (1JT20IS043)             \xdb\n";
    cout << "\t\t\xdb                                               \xdb\n";
    cout << "\t\t\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\xdb\n\n\n";
    cout << " \n\t Press Any Key To Continue:";

    cin.ignore();
    cout << "\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (username == "raks" && password == "subbi") {
        cout << "\nLogin Successful!" << endl;
        while (true) {
            cout << "\n\n*MENU*\n";
            cout << "1. Park a Vehicle\n";
            cout << "2. Remove a Vehicle\n";
            cout << "3. Display Parking Lot\n";
            cout << "4. List of Vehicles Parked\n";
            cout << "5. Revenue of the Day\n";
            cout << "6. Overall Revenue\n";
            cout << "7. Exit\n";
            cout << "Enter your choice (1-7): ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int type, num, row, col,color;
                    cout << "1.CAR\n2.SCOOTER\nEnter Vehicle Type: ";
                    cin >> type;
                    cout << "Enter Vehicle Number: ";
                    cin >> num;
                    if (to_string(num).length() == 4)
		    {
                    int rc[2];
                    getfreerowcol(type, rc);
                    if (rc[0] == -1 && rc[1] == -1) {
                        cout << "\nNo space available for parking the vehicle!" << endl;
                    } else {
                        row = rc[0];
                        col = rc[1];

                        time_t now = time(0);
                        at = ctime(&now);
                        cout << "\nVehicle parked at Row " << row + 1 << ", Col " << col + 1 << " at " << at;
                        Vehicle* v = new Vehicle;
                        v->type = type;
                        v->num = num;
                        v->row = row;
                        v->col = col;
                        v->arrivalTime = now;

                        add(type, num, row, col);
                    }
                    } 
    		   else 
    		   {
       			cout << "Invalid input! Please enter proper vehicle number" << endl;
    		   }
		  
                    break;
                }
                case 2: {
                    int num,type;
                    /*cout << "1.CAR\n2.SCOOTER\nEnter Vehicle Type: ";
                    cin >> type;*/
                    cout << "\nEnter Vehicle Number to remove: ";
                    cin >> num;
		    int rc[2];
                    getrcbyinfo(CAR, num, rc);
                    if (rc[0] == -1 && rc[1] == -1) 
                    {
                        getrcbyinfo(SCOOTER, num, rc);
                        if (rc[0] == -1 && rc[1] == -1) 
                        {
                            cout << "\nVehicle not found in the parking lot!" << endl;
                        } 
                        else 
                        {
                            time_t now = time(0);
                            dt = ctime(&now);
                            cout << "\nVehicle removed from Row " << rc[0] + 1 << ", Col " << rc[1] + 1 << " at " << dt;
			    cost = (dt-at)+15;
			    cout << "Parking cost is :"<< cost;
                            Vehicle* v = new Vehicle;
                            v->type = SCOOTER;
                            v->num = num;
                            v->row = rc[0];
                            v->col = rc[1];
                            v->departureTime = now;

                            del(v);
                        }
                    } 
                    else 
                    {
                        time_t now = time(0);
                        dt = ctime(&now);
                        cout << "\nVehicle removed from Row " << rc[0] + 1 << ", Col " << rc[1] + 1 << " at " << dt;
			cost = (dt-at)+25;
			cout << "Parking cost is:"<< cost;
                        Vehicle* v = new Vehicle;
                        v->type = CAR;
                        v->num = num;
                        v->row = rc[0];
                        v->col = rc[1];
                        v->departureTime = now;

                        del(v);
                    }            
                    break;
                }
                case 3:
                    display();
                    break;
                case 4:
        		displayIncomingVehicles();
       			break;
       		case 5:
       		{
       		    float revenue = calculateTotalRevenue();
                    cout << "\nTotal Revenue for the Day: $" << revenue << endl;
                    break;
                }
                case 6: {
                	float revenue = calculateRevenue();
                	cout<< "\nOverall Revenue is: $" << revenue << endl;
                	break;}
	        case 7:
                    cout << "\nExiting program..." << endl;
                    exit(0);
                default:
                    cout << "\nInvalid choice! Please enter a valid choice (1-4)." << endl;
            }
        }
    } else {
        cout << "\nLogin Failed! Invalid username or password." << endl;
    }

    return 0;
}
