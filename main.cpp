#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Structure to represent a plant
struct Plant {
    int ID;
    string Name;
    string growthStage;
    int sunlight;
    int water;
    int temp;
};

// Function to simulate the plant growth over a number of days
string simulateGrowth(Plant& plant, string stage, int sunlight, int water, int temp, int days) {
    if (stage.empty()) {
        stage = "Seedling"; // Default to Seedling if no growth stage is set
    }

    const int daysSeedling = 7, daysMature = 10, daysBlooming = 12;

    while (days > 0) {
        // Logic for plant growth stages: Seedling -> Mature -> Blooming -> Stunted
        if (stage == "Seedling" && days >= daysSeedling && sunlight >= plant.sunlight &&
            water >= plant.water && temp >= plant.temp - 2 && temp <= plant.temp + 2) {
            stage = "Mature";
            days -= daysSeedling;
            cout << "Plant has changed from seedling stage to mature.\n";
        } else if (stage == "Mature" && days >= daysMature && sunlight >= plant.sunlight + 2 &&
                   water >= plant.water + 2 && temp >= plant.temp - 3 && temp <= plant.temp + 3) {
            stage = "Blooming";
            days -= daysMature;
            cout << "Plant has changed from mature stage to blooming.\n";
        } else if (stage == "Blooming" && days >= daysBlooming && 
                   (sunlight < plant.sunlight + 4 || water < plant.water + 4 || 
                    temp < plant.temp - 4 || temp > plant.temp + 4)) {
            stage = "Stunted";
            days -= daysBlooming;
            cout << "Plant has changed from blooming stage to stunted.\n";
            break;
        } else if ((stage == "Seedling" || stage == "Mature") && 
                   (sunlight < plant.sunlight || water < plant.water || 
                    temp < plant.temp - 2 || temp > plant.temp + 2)) {
            cout << "Due to drought condition, plant is now stunted.\n";
            stage = "Stunted";
            break;
        } else if (stage == "Blooming") {
            cout << "Plant has reached its maximum growth, which is blooming.\n";
            break;
        } else if (stage == "Stunted") {
            cout << "Plant got stunted... No further growth is possible.\n";
            break;
        } else {
            cout << "Conditions not met for growth.\n";
            break;
        }
    }

    return stage;
}

// Function to load plants from a file
void loadPlants(const string& filename, vector<Plant>& plants) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    Plant plant;
    // Reading plant data from file
    while (file >> plant.ID) {
        file.ignore();
        getline(file, plant.Name);
        file >> plant.sunlight >> plant.water >> plant.temp;
        file.ignore();
        getline(file, plant.growthStage);

        plants.push_back(plant); // Adding plant to vector
    }
    file.close();
}

// Function to save plant data to a file
void savePlants(const string& filename, const vector<Plant>& plants) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error saving file!" << endl;
        return;
    }

    // Writing plant data to file
    for (const auto& plant : plants) {
        file << plant.ID << "\n"
             << plant.Name << "\n"
             << plant.sunlight << "\n"
             << plant.water << "\n"
             << plant.temp << "\n"
             << plant.growthStage << "\n";
    }
    file.close();
}

// Function to add a new plant record to the file
void addPlantRecord(const string& filename) {
    ofstream file(filename, ios::app); // Open file in append mode
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    Plant plant;
    cout << "Enter ID: ";
    cin >> plant.ID;
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, plant.Name);

    // Validating sunlight and water input
    do {
        cout << "Enter Sunlight (0-10): ";
        cin >> plant.sunlight;
    } while (plant.sunlight < 0 || plant.sunlight > 10);

    do {
        cout << "Enter Water (0-10): ";
        cin >> plant.water;
    } while (plant.water < 0 || plant.water > 10);

    cout << "Enter Temp (in Celsius): ";
    cin >> plant.temp;

    // Determining initial growth stage based on input
    if (plant.sunlight <= 4 && plant.water == 3) {
        plant.growthStage = "Seedling";
    } else if (plant.sunlight >= 7 && plant.sunlight < 10 && plant.water == 5 && plant.temp > 15 && plant.temp < 30) {
        plant.growthStage = "Mature";
    } else if (plant.sunlight == 10 && plant.water == 8 && plant.temp > 20 && plant.temp < 25) {
        plant.growthStage = "Blooming";
    } else {
        plant.growthStage = "Stunted";
    }

    // Saving the new plant record to file
    file << plant.ID << "\n"
         << plant.Name << "\n"
         << plant.sunlight << "\n"
         << plant.water << "\n"
         << plant.temp << "\n"
         << plant.growthStage << "\n";

    file.close();
    cout << "Plant record added successfully!" << endl;
}

// Function to search for a plant by its ID and display its details
void searchPlantByID(const string& filename, int id) {
    vector<Plant> plants;
    loadPlants(filename, plants);

    // Searching for the plant by ID
    for (const auto& plant : plants) {
        if (plant.ID == id) {
            cout << "Plant ID: " << plant.ID << endl;
            cout << "Plant Name: " << plant.Name << endl;
            cout << "Sunlight: " << plant.sunlight << endl;
            cout << "Water Supply: " << plant.water << endl;
            cout << "Temperature: " << plant.temp << endl;
            cout << "Growth Stage: " << plant.growthStage << endl;
            return;
        }
    }
    cout << "Plant with ID " << id << " not found!" << endl;
}

// Function to display details of all plants
void displayAllPlants(const string& filename) {
    vector<Plant> plants;
    loadPlants(filename, plants);

    // Displaying all plant records
    for (const auto& plant : plants) {
        cout << "--------------------\n";
        cout << "Plant ID: " << plant.ID << endl;
        cout << "Plant Name: " << plant.Name << endl;
        cout << "Sunlight: " << plant.sunlight << endl;
        cout << "Water Supply: " << plant.water << endl;
        cout << "Temperature: " << plant.temp << endl;
        cout << "Growth Stage: " << plant.growthStage << endl;
    }
}

// Function to analyze and count the plants in each growth stage
void analyzeGrowthStages(const string& filename) {
    vector<Plant> plants;
    loadPlants(filename, plants);

    int seedling = 0, mature = 0, blooming = 0, stunted = 0;

    // Counting the plants in each stage
    for (const auto& plant : plants) {
        if (plant.growthStage == "Seedling")
            seedling++;
        else if (plant.growthStage == "Mature")
            mature++;
        else if (plant.growthStage == "Blooming")
            blooming++;
        else if (plant.growthStage == "Stunted")
            stunted++;
    }

    // Displaying growth stage analysis
    cout << "Growth Stage Analysis:" << endl;
    cout << "Seedling Plants: " << seedling << endl;
    cout << "Mature Plants: " << mature << endl;
    cout << "Blooming Plants: " << blooming << endl;
    cout << "Stunted Plants: " << stunted << endl;
}

// Function to simulate the growth of a specific plant by ID
void simulatePlantGrowth(const string& filename) {
    vector<Plant> plants;
    loadPlants(filename, plants);

    int plantID;
    cout << "Enter the Plant ID to simulate growth: ";
    cin >> plantID;

    bool found = false;

    // Searching for the plant by ID
    for (auto& plant : plants) {
        if (plant.ID == plantID) {
            found = true;
            string stage = plant.growthStage;
            int sunlight, water, temp, days;

            cout << "Current Growth Stage: " << stage << endl;
            cout << "Enter Sunlight Input (0-10): ";
            cin >> sunlight;
            cout << "Enter Water Input (0-10): ";
            cin >> water;
            cout << "Enter Temperature Input (°C): ";
            cin >> temp;
            cout << "Enter Number of Days to Simulate: ";
            cin >> days;

            // Simulating plant growth
            string newStage = simulateGrowth(plant, stage, sunlight, water, temp, days);

            if (newStage != stage) {
                plant.growthStage = newStage;
                cout << "New Growth Stage: " << newStage << endl;
                savePlants(filename, plants); // Saving updated plant data
                cout << "Plant record updated successfully!" << endl;
            } else {
                cout << "Growth stage remains unchanged: " << stage << endl;
            }
            break;
        }
    }

    if (!found) {
        cout << "Plant with ID " << plantID << " not found!" << endl;
    }
}

// Main function for the menu-driven system
int main() {
    const string filename = "plant.txt";
    int choice;
    do {
        cout << "\nPlant Management System" << endl;
        cout << "1. Add Plant Record" << endl;
        cout << "2. Search Plant by ID" << endl;
        cout << "3. Display All Plants" << endl;
        cout << "4. Analyze Growth Stages" << endl;
        cout << "5. Simulate Plant Growth" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        // Processing user's choice
        switch (choice) {
            case 1:
                addPlantRecord(filename);
                break;
            
            case 2: {
                int id;
                cout << "Enter Plant ID to search: ";
                cin >> id;
                searchPlantByID(filename, id);
                break;
                }
            case 3:
                displayAllPlants(filename);
                break;
            case 4:
                analyzeGrowthStages(filename);
                break;
            case 5:
                simulatePlantGrowth(filename);
                break;
            case 6:
                cout << "Exiting program..." << endl;
                break;
            default:
                cerr << "Invalid choice! Try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
