

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

struct Customer {
    int number;
    double interArrival;
    double arrivalTime;
    double serviceTime;
    double serviceStart;
    double serviceEnd;
    double waitingTime;
    double timeInSystem;
    double tellerIdleTime;
};

double readPositiveNumber(const string& prompt, double defaultValue) {
    while (true) {
        cout << prompt << " [default " << defaultValue << "]: ";
        string line;
        getline(cin, line);
        if (line.empty()) return defaultValue;
        try {
            size_t pos = 0;
            double value = stod(line, &pos);
            if (pos == line.size() && value > 0) return value;
        } catch (...) {
        }
        cout << "Enter a positive number.\n";
    }
}

int readCustomerCount() {
    while (true) {
        cout << "Number of customers [default 100]: ";
        string line;
        getline(cin, line);
        if (line.empty()) return 100;
        try {
            size_t pos = 0;
            int value = stoi(line, &pos);
            if (pos == line.size() && value > 0 && value <= 100000) return value;
        } catch (...) {
        }
        cout << "Enter a whole number from 1 to 100000.\n";
    }
}

int main() {
    cout << "============================================\n";
    cout << "       BANK QUEUE SIMULATION SYSTEM\n";
    cout << "============================================\n";
    cout << "Configure the simulation. Press Enter to use each default.\n\n";

    int customerCount = readCustomerCount();
    double minInterArrival = readPositiveNumber("Minimum inter-arrival time (minutes)", 1.0);
    double maxInterArrival = readPositiveNumber("Maximum inter-arrival time (minutes)", 5.0);
    while (maxInterArrival < minInterArrival) {
        cout << "Maximum inter-arrival time must be at least the minimum.\n";
        maxInterArrival = readPositiveNumber("Maximum inter-arrival time (minutes)", 5.0);
    }
    double minService = readPositiveNumber("Minimum service time (minutes)", 1.0);
    double maxService = readPositiveNumber("Maximum service time (minutes)", 6.0);
    while (maxService < minService) {
        cout << "Maximum service time must be at least the minimum.\n";
        maxService = readPositiveNumber("Maximum service time (minutes)", 6.0);
    }

    unsigned int seed;
    cout << "Random seed (enter 0 for a different result each run): ";
    string seedLine;
    getline(cin, seedLine);
    try {
        seed = static_cast<unsigned int>(stoul(seedLine));
    } catch (...) {
        seed = 0;
    }
    if (seed == 0) {
        random_device rd;
        seed = rd();
    }

    mt19937 generator(seed);
    uniform_real_distribution<double> interArrivalDistribution(minInterArrival, maxInterArrival);
    uniform_real_distribution<double> serviceDistribution(minService, maxService);

    vector<Customer> customers;
    customers.reserve(customerCount);

    double totalInterArrival = 0.0;
    double totalService = 0.0;
    double totalWaiting = 0.0;
    double totalTimeInSystem = 0.0;
    double totalIdle = 0.0;
    double totalBusy = 0.0;
    double maximumWait = 0.0;
    int customersWhoWaited = 0;

    double previousArrival = 0.0;
    double previousServiceEnd = 0.0;

    for (int i = 0; i < customerCount; ++i) {
        Customer customer{};
        customer.number = i + 1;
        customer.interArrival = (i == 0) ? 0.0 : interArrivalDistribution(generator);
        customer.arrivalTime = (i == 0) ? 0.0 : previousArrival + customer.interArrival;
        customer.serviceTime = serviceDistribution(generator);
        customer.serviceStart = max(customer.arrivalTime, previousServiceEnd);
        customer.serviceEnd = customer.serviceStart + customer.serviceTime;
        customer.waitingTime = customer.serviceStart - customer.arrivalTime;
        customer.timeInSystem = customer.serviceEnd - customer.arrivalTime;
        customer.tellerIdleTime = max(0.0, customer.arrivalTime - previousServiceEnd);

        totalInterArrival += customer.interArrival;
        totalService += customer.serviceTime;
        totalWaiting += customer.waitingTime;
        totalTimeInSystem += customer.timeInSystem;
        totalIdle += customer.tellerIdleTime;
        totalBusy += customer.serviceTime;
        maximumWait = max(maximumWait, customer.waitingTime);
        if (customer.waitingTime > 0.000001) ++customersWhoWaited;

        previousArrival = customer.arrivalTime;
        previousServiceEnd = customer.serviceEnd;
        customers.push_back(customer);
    }

    cout << "\n\n============================================\n";
    cout << "             SIMULATION OUTPUT\n";
    cout << "============================================\n";
    cout << "Customers: " << customerCount << " | Inter-arrival range: "
         << minInterArrival << "-" << maxInterArrival << " minutes | Service range: "
         << minService << "-" << maxService << " minutes\n\n";

    cout << left << setw(5) << "No."
         << right << setw(10) << "Inter-arr."
         << setw(12) << "Arrival"
         << setw(11) << "Service"
         << setw(13) << "Start"
         << setw(12) << "End"
         << setw(11) << "Wait"
         << setw(13) << "In system"
         << setw(11) << "Idle" << '\n';
    cout << string(98, '-') << '\n';

    cout << fixed << setprecision(2);
    for (const Customer& c : customers) {
        cout << left << setw(5) << c.number
             << right << setw(10) << c.interArrival
             << setw(12) << c.arrivalTime
             << setw(11) << c.serviceTime
             << setw(13) << c.serviceStart
             << setw(12) << c.serviceEnd
             << setw(11) << c.waitingTime
             << setw(13) << c.timeInSystem
             << setw(11) << c.tellerIdleTime << '\n';
    }

    double simulationEnd = customers.back().serviceEnd;
    double averageInterArrival = customerCount > 1 ? totalInterArrival / (customerCount - 1) : 0.0;
    double averageService = totalService / customerCount;
    double averageWaiting = totalWaiting / customerCount;
    double averageTimeInSystem = totalTimeInSystem / customerCount;
    double tellerUtilization = simulationEnd > 0.0 ? (totalBusy / simulationEnd) * 100.0 : 0.0;
    double idlePercentage = simulationEnd > 0.0 ? (totalIdle / simulationEnd) * 100.0 : 0.0;
    double probabilityOfWaiting = (static_cast<double>(customersWhoWaited) / customerCount) * 100.0;

    cout << "\n============================================\n";
    cout << "               QUEUE STATISTICS\n";
    cout << "============================================\n";
    cout << "Total customers simulated          : " << customerCount << '\n';
    cout << "Total simulation time              : " << simulationEnd << " minutes\n";
    cout << "Average inter-arrival time         : " << averageInterArrival << " minutes\n";
    cout << "Average service time               : " << averageService << " minutes\n";
    cout << "Average waiting time               : " << averageWaiting << " minutes\n";
    cout << "Average time in system              : " << averageTimeInSystem << " minutes\n";
    cout << "Maximum waiting time                : " << maximumWait << " minutes\n";
    cout << "Customers who waited                : " << customersWhoWaited << '\n';
    cout << "Probability a customer waits        : " << probabilityOfWaiting << "%\n";
    cout << "Total teller busy time              : " << totalBusy << " minutes\n";
    cout << "Total teller idle time              : " << totalIdle << " minutes\n";
    cout << "Teller utilization                  : " << tellerUtilization << "%\n";
    cout << "Teller idle percentage              : " << idlePercentage << "%\n";
    cout << "\nPress Enter to exit...";
    string exitLine;
    getline(cin, exitLine);
    return 0;
}
