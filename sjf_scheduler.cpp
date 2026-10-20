// SPOS Mini Project
// Topic: Manufacturing System using SJF (Shortest Job First) Scheduling
//
// Every task on the production line (cutting, welding, painting etc.) is taken
// as a process. Processing time = burst time and the machine = CPU.
// When the machine is free it picks the shortest task from the tasks that have
// already arrived (non preemptive SJF). FCFS is also done to compare results.

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
using namespace std;

struct Task {
    int id;
    string name;
    int at;    // arrival time
    int bt;    // burst time (processing time)
    int ct;    // completion time
    int tat;   // turnaround time
    int wt;    // waiting time
};

Task t[20];
int n = 0;

// arrays for gantt chart
string gLabel[40];
int gStart[40], gEnd[40];
int gCount = 0;

// reads a number, asks again if user types letters
int readNum() {
    int x;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a number: ";
    }
    return x;
}

void loadSample() {
    string names[5] = {"Cutting", "Drilling", "Welding", "Painting", "Packing"};
    int arr[5] = {0, 1, 2, 3, 4};
    int burst[5] = {8, 4, 9, 5, 2};

    n = 5;
    for (int i = 0; i < n; i++) {
        t[i].id = i + 1;
        t[i].name = names[i];
        t[i].at = arr[i];
        t[i].bt = burst[i];
    }
}

void inputTasks() {
    cout << "Enter number of tasks (max 20): ";
    n = readNum();
    while (n < 1 || n > 20) {
        cout << "Number of tasks should be between 1 and 20: ";
        n = readNum();
    }

    for (int i = 0; i < n; i++) {
        t[i].id = i + 1;
        cout << "\nTask T" << i + 1 << endl;
        cout << "Name (one word): ";
        cin >> t[i].name;

        cout << "Arrival time: ";
        t[i].at = readNum();
        while (t[i].at < 0) {
            cout << "Arrival time cannot be negative, enter again: ";
            t[i].at = readNum();
        }

        cout << "Processing time: ";
        t[i].bt = readNum();
        while (t[i].bt <= 0) {
            cout << "Processing time should be more than 0, enter again: ";
            t[i].bt = readNum();
        }
    }
}

void showTasks() {
    cout << "\nTask  Name        Arrival  Processing\n";
    for (int i = 0; i < n; i++) {
        cout << "T" << left << setw(5) << t[i].id << setw(12) << t[i].name << right
             << setw(7) << t[i].at << setw(12) << t[i].bt << endl;
    }
}

void addToGantt(string label, int start, int end) {
    gLabel[gCount] = label;
    gStart[gCount] = start;
    gEnd[gCount] = end;
    gCount++;
}

void sjf(Task p[]) {
    bool done[20] = {false};
    int time = 0, completed = 0;
    gCount = 0;

    while (completed < n) {
        // find the shortest task among the ones that have arrived
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time) {
                if (idx == -1 || p[i].bt < p[idx].bt)
                    idx = i;
                else if (p[i].bt == p[idx].bt && p[i].at < p[idx].at)
                    idx = i;   // same burst time -> one that came first
            }
        }

        if (idx == -1) {
            // no task has arrived yet, so machine stays idle till next arrival
            int next = 9999;
            for (int i = 0; i < n; i++) {
                if (!done[i] && p[i].at < next)
                    next = p[i].at;
            }
            addToGantt("IDLE", time, next);
            time = next;
        } else {
            addToGantt("T" + to_string(p[idx].id), time, time + p[idx].bt);
            time = time + p[idx].bt;
            p[idx].ct = time;
            done[idx] = true;
            completed++;
        }
    }
}

void fcfs(Task p[]) {
    bool done[20] = {false};
    int time = 0, completed = 0;
    gCount = 0;

    while (completed < n) {
        // pick the task that arrived first
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && (idx == -1 || p[i].at < p[idx].at))
                idx = i;
        }

        if (p[idx].at > time) {
            addToGantt("IDLE", time, p[idx].at);
            time = p[idx].at;
        }
        addToGantt("T" + to_string(p[idx].id), time, time + p[idx].bt);
        time = time + p[idx].bt;
        p[idx].ct = time;
        done[idx] = true;
        completed++;
    }
}

void calculate(Task p[]) {
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
}

float avgWT(Task p[]) {
    float sum = 0;
    for (int i = 0; i < n; i++)
        sum += p[i].wt;
    return sum / n;
}

float avgTAT(Task p[]) {
    float sum = 0;
    for (int i = 0; i < n; i++)
        sum += p[i].tat;
    return sum / n;
}

void printGantt() {
    cout << "\nGantt Chart:\n ";
    for (int i = 0; i < gCount; i++) cout << "-------";
    cout << "\n|";
    for (int i = 0; i < gCount; i++) cout << setw(5) << gLabel[i] << " |";
    cout << "\n ";
    for (int i = 0; i < gCount; i++) cout << "-------";
    cout << "\n0";
    for (int i = 0; i < gCount; i++) cout << setw(7) << gEnd[i];
    cout << endl;
}

void printTable(Task p[]) {
    cout << "\nTask  Name          AT   BT   CT  TAT   WT\n";
    cout << "------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "T" << left << setw(5) << p[i].id << setw(12) << p[i].name << right
             << setw(4) << p[i].at << setw(5) << p[i].bt << setw(5) << p[i].ct
             << setw(5) << p[i].tat << setw(5) << p[i].wt << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time    = " << avgWT(p) << endl;
    cout << "Average Turnaround Time = " << avgTAT(p) << endl;
}

void compare() {
    Task a[20], b[20];
    for (int i = 0; i < n; i++) {
        a[i] = t[i];
        b[i] = t[i];
    }
    sjf(a);
    calculate(a);
    fcfs(b);
    calculate(b);

    float sjfWT = avgWT(a), fcfsWT = avgWT(b);
    float sjfTAT = avgTAT(a), fcfsTAT = avgTAT(b);

    cout << fixed << setprecision(2);
    cout << "\n               FCFS     SJF\n";
    cout << "Avg WT     " << setw(8) << fcfsWT << setw(8) << sjfWT << endl;
    cout << "Avg TAT    " << setw(8) << fcfsTAT << setw(8) << sjfTAT << endl;

    if (sjfWT < fcfsWT)
        cout << "\nSJF reduced the average waiting time by " << (fcfsWT - sjfWT) / fcfsWT * 100 << "%\n";
    else
        cout << "\nNo improvement with SJF for these tasks\n";
}

int main() {
    int choice;
    Task p[20];

    do {
        cout << "\n===== SJF Scheduling - Manufacturing System =====\n";
        cout << "1. Enter production tasks\n";
        cout << "2. Load sample tasks\n";
        cout << "3. Show task list\n";
        cout << "4. Run SJF scheduling\n";
        cout << "5. Run FCFS scheduling\n";
        cout << "6. Compare SJF vs FCFS\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        choice = readNum();

        if (choice >= 3 && choice <= 6 && n == 0) {
            cout << "No tasks entered yet. Use option 1 or 2 first.\n";
            continue;
        }

        switch (choice) {
        case 1:
            inputTasks();
            break;
        case 2:
            loadSample();
            cout << "Sample tasks loaded\n";
            showTasks();
            break;
        case 3:
            showTasks();
            break;
        case 4:
            for (int i = 0; i < n; i++) p[i] = t[i];
            sjf(p);
            calculate(p);
            cout << "\n----- SJF (Non Preemptive) -----\n";
            printGantt();
            printTable(p);
            break;
        case 5:
            for (int i = 0; i < n; i++) p[i] = t[i];
            fcfs(p);
            calculate(p);
            cout << "\n----- FCFS -----\n";
            printGantt();
            printTable(p);
            break;
        case 6:
            compare();
            break;
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice\n";
        }
    } while (choice != 7);

    return 0;
}
