#include <stdio.h>
struct Process {
    int pid, arrival_time, burst_time, priority, remaining_time;
    int completion_time, turnaround_time, waiting_time;
};

// Function to swap two processes
void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

// Function to sort processes by arrival time
void sortByArrival(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                swap(&p[j], &p[j + 1]);
            }
        }
    }
}

// Function to display process table
void displayTable(struct Process p[], int n) {
    printf("\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);
    }
}

// Function to display Gantt Chart
void displayGanttChart(struct Process p[], int n) {
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst_time; j++)
            printf("--");
        printf(" ");
    }
    printf("\n|");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst_time - 1; j++)
            printf(" ");
        printf("P%d", p[i].pid);
        for (int j = 0; j < p[i].burst_time - 1; j++)
            printf(" ");
        printf("|");
    }
    printf("\n ");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst_time; j++)
            printf("--");
        printf(" ");
    }
    printf("\n");

    int current_time = 0;
    printf("%d", current_time);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p[i].burst_time; j++)
            printf("  ");
        current_time = p[i].completion_time;
        printf("%d", current_time);
    }
    printf("\n");
}

// Function for Non-Preemptive Priority Scheduling
void priorityNonPreemptive(struct Process p[], int n) {
    sortByArrival(p, n);
    int completed = 0, current_time = 0;
    float total_tat = 0, total_wt = 0;

    while (completed < n) {
        int idx = -1, highest_priority = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].completion_time == 0) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            current_time += p[idx].burst_time;
            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            total_tat += p[idx].turnaround_time;
            total_wt += p[idx].waiting_time;
            completed++;
        } else {
            current_time++;
        }
    }

    printf("\n========== Non-Preemptive Priority Scheduling ==========\n");
    displayTable(p, n);
    displayGanttChart(p, n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

// Function for Preemptive Priority Scheduling
void priorityPreemptive(struct Process p[], int n) {
    int current_time = 0, completed = 0;
    float total_tat = 0, total_wt = 0;

    while (completed < n) {
        int idx = -1, highest_priority = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            p[idx].remaining_time--;
            current_time++;

            if (p[idx].remaining_time == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
                total_tat += p[idx].turnaround_time;
                total_wt += p[idx].waiting_time;
                completed++;
            }
        } else {
            current_time++;
        }
    }

    printf("\n========== Preemptive Priority Scheduling ==========\n");
    displayTable(p, n);
    displayGanttChart(p, n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time: %.2f\n", total_wt / n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n], p_copy[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time, Burst Time and Priority for Process %d: ", i + 1);
        scanf("%d %d %d", &p[i].arrival_time, &p[i].burst_time, &p[i].priority);
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p_copy[i] = p[i]; // Copying for Non-Preemptive Execution
    }

    priorityNonPreemptive(p_copy, n);
    priorityPreemptive(p, n);

    return 0;
}