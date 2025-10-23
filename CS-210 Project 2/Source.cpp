#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// Forward declaration for the main application class
class CalculatorApp;


 // This class ensures all input parameters are grouped logically.
class InvestmentData {
public:
    double initialInvestmentAmount = 0.0;
    double monthlyDepositAmount = 0.0;
    double annualInterestRate = 0.0; // Percentage
    int numberOfYears = 0;
};

// Hold the detailed results of monthly calculations,
struct ReportData {
    int month = 0;
    double openingBalance = 0.0;
    double depositedAmount = 0.0;
    double total = 0.0;
    double interestEarned = 0.0;
    double closingBalance = 0.0;
};

 // Manages user input, validation, calculation, and report generation.
class CalculatorApp {
private:
    InvestmentData m_investmentData;
    vector<ReportData> m_monthlyReport;

     // Clears console screen.
    void clearScreen() const {
#ifdef _WIN32
        system("cls");
#endif
    }

    // Initial input screen layout.
void displayHeader(const string& title) const {
    cout << string(32, '*') << endl;
    cout << "*** " << left << setw(24) << title << "***" << endl;
    cout << string(32, '*') << endl;
}

    // Make sure input is correct.
    double getInputParameter(const string& prompt) {
        double value;
        while (true) {
            cout << prompt << flush;
            if (cin >> value) {
                if (value >= 0) {
                    return value;
                }
                else {
                    cout << "Error: Value must be non-negative. Please try again." << endl;
                }
            }
            else {
                cout << "Error: Invalid input. Please enter a numerical value." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // Make sure input is correct.
    int getIntegerInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt << flush;
            if (cin >> value) {
                if (value > 0) {
                    return value;
                }
                else {
                    cout << "Error: Value must be a positive integer. Please try again." << endl;
                }
            }
            else {
                cout << "Error: Invalid input. Please enter an integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // Yearly Data Calculations.
    void calculateYearlyData(double monthlyDeposit,
        vector<double>& outYearEndBalances,
        vector<double>& outYearEndInterest,
        vector<ReportData>* outMonthlyData = nullptr) {

        double currentBalance = m_investmentData.initialInvestmentAmount;
        double monthlyInterestRate = (m_investmentData.annualInterestRate / 100.0) / 12.0;
        int numberOfMonths = m_investmentData.numberOfYears * 12;

        outYearEndBalances.clear();
        outYearEndInterest.clear();
        if (outMonthlyData) {
            outMonthlyData->clear();
        }

        double yearTotalInterest = 0.0;

        for (int month = 1; month <= numberOfMonths; ++month) {

            ReportData currentMonthData;

            currentMonthData.openingBalance = currentBalance;
            currentMonthData.depositedAmount = monthlyDeposit;

            currentMonthData.total = currentMonthData.openingBalance + currentMonthData.depositedAmount;

            currentMonthData.interestEarned = currentMonthData.total * monthlyInterestRate;

            currentMonthData.closingBalance = currentMonthData.total + currentMonthData.interestEarned;

            currentBalance = currentMonthData.closingBalance;
            yearTotalInterest += currentMonthData.interestEarned;

            currentMonthData.month = month;

            if (outMonthlyData) {
                outMonthlyData->push_back(currentMonthData);
            }

            if (month % 12 == 0) {
                outYearEndBalances.push_back(currentBalance);
                outYearEndInterest.push_back(yearTotalInterest);

                yearTotalInterest = 0.0;
            }
        }
    }

    

    // Dollar sign alignment.
    void printDollarAmount(double amount, int width) const {
        cout << setw(width) << right;
        cout << "$" << fixed << setprecision(2) << amount;
    }

    // Year End Report Display.
    void displayYearEndReport(const string& reportTitle,
        const vector<double>& balances,
        const vector<double>& interests) const {

        cout << endl;
        const int YEAR_W = 0;
        const int BALANCE_W = 50; 
        const int INTEREST_W = 50;

        cout << string(120, '=') << endl;
        cout << "*** " << reportTitle << "  ***" << endl;
        cout << string(120, '=') << endl;

        cout << setw(YEAR_W) << left << "Year";

        cout << right
            << setw(BALANCE_W) << "Year End Balance"
            << setw(INTEREST_W) << "Year End Earned Interest"
            << endl;

        cout << string(120, '-') << endl;

        cout << fixed << setprecision(2);

        for (size_t i = 0; i < balances.size(); ++i) {

            cout << setw(YEAR_W) << left << i;
            printDollarAmount(balances[i], BALANCE_W);
            printDollarAmount(interests[i], INTEREST_W);

            cout << endl;
        }
        cout << string(120, '-') << endl;
    }


    

    // Monthly Report Display.
    void displayMonthlyReportTable(const vector<ReportData>& monthlyData) const {
        clearScreen();
        cout << endl;
        cout << string(120, '=') << endl;
        cout << "                                              *** DETAILED MONTHLY BREAKDOWN ***                                     " << endl;
        cout << string(120, '=') << endl;

        const int MONTH_W = 8;     
        const int OPEN_W = 25;    
        const int DEPOSIT_W = 25;  
        const int INTEREST_W = 20; 
        const int CLOSE_W = 25;    

        cout << setw(MONTH_W) << left << "Month"
            << setw(OPEN_W ) << right << "Opening Amount"
            << setw(DEPOSIT_W) << right << "Deposited Amount"
            << setw(INTEREST_W) << right << "Interest"
            << setw(CLOSE_W) << right << "Closing Balance"
            << endl;

        cout << string(120, '-') << endl;

        cout << fixed << setprecision(2);

        for (const auto& data : monthlyData) {
            cout << setw(MONTH_W) << left << data.month;
            cout << right;
            printDollarAmount(data.openingBalance, OPEN_W);
            printDollarAmount(data.depositedAmount, DEPOSIT_W);
            printDollarAmount(data.interestEarned, INTEREST_W);
            cout << setw(CLOSE_W) << data.closingBalance;
            cout << endl;
        }

        cout << string(120, '-') << endl;

        cout << "\nPress enter key to continue..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

// Application loop.
public:
    void run() {
        bool running = true;
        while (running) {
            getInputParameters();

            cout << "\nPress enter key to continue to the reports..." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();

            clearScreen();

            displayAllYearEndReports();

            char monthlyChoice;
            cout << "\nWould you like to see the DETAILED MONTHLY BREAKDOWN? (Y/N): " << endl;
            cin >> monthlyChoice;

            if (toupper(monthlyChoice) == 'Y') {
                displayMonthlyReportTable(m_monthlyReport);
            }

            char choice;
            cout << "\nPress 'C' to perform a new calculation or 'Q' to quit: " << endl;
            cin >> choice;

            if (toupper(choice) == 'Q') {
                running = false;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "\nThank you for using the Airgead Banking App. Goodbye!" << endl;
    }


    // Collects and validates all required investment parameters from the user.
    void getInputParameters() {
        clearScreen();
        displayHeader("DATA INPUT");

        m_investmentData.initialInvestmentAmount = getInputParameter("Initial Investment Amount: $");
        m_investmentData.monthlyDepositAmount = getInputParameter("Monthly Deposit: $");
        m_investmentData.annualInterestRate = getInputParameter("Annual Interest (%): ");
        m_investmentData.numberOfYears = getIntegerInput("Number of Years: ");

        clearScreen();
        displayHeader("CONFIRM INPUT");
        cout << "Initial Investment Amount: $" << fixed << setprecision(2) << m_investmentData.initialInvestmentAmount << endl;
        cout << "Monthly Deposit: $" << m_investmentData.monthlyDepositAmount << endl;
        cout << "Annual Interest (%): " << m_investmentData.annualInterestRate << endl;
        cout << "Number of Years: " << m_investmentData.numberOfYears << endl;
    }


    // Generation and display of both year-end investment reports.
    void displayAllYearEndReports() {
        vector<double> balances;
        vector<double> interests;

        calculateYearlyData(0.0, balances, interests);
        displayYearEndReport("Balance and Interest Without Additional Monthly Deposits",
            balances, interests);

        calculateYearlyData(m_investmentData.monthlyDepositAmount, balances, interests, &m_monthlyReport);
        displayYearEndReport("Balance and Interest With Additional Monthly Deposits",
            balances, interests);
    }
};

// Main function to run the application
int main() {
    cout << fixed << setprecision(2);

    CalculatorApp app;

    app.run();

    return 0;
}