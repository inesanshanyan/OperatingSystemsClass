#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

/* pid 0 marks a stretch where the CPU has no ready process. */
typedef struct {
    int pid;
    int start;
    int end;
} Segment;

static int compare_fcfs(const void *left, const void *right) {
    const Process *a = left;
    const Process *b = right;

    if (a->arrival_time != b->arrival_time) {
        return (a->arrival_time > b->arrival_time) - (a->arrival_time < b->arrival_time);
    }
    return (a->pid > b->pid) - (a->pid < b->pid);
}

static int compare_pid(const void *left, const void *right) {
    const Process *a = left;
    const Process *b = right;
    return (a->pid > b->pid) - (a->pid < b->pid);
}

static void set_metrics(Process *process, int start) {
    process->start_time = start;
    process->completion_time = start + process->burst_time;
    process->turnaround_time = process->completion_time - process->arrival_time;
    process->waiting_time = process->turnaround_time - process->burst_time;
    process->response_time = process->start_time - process->arrival_time;
}

static void print_gantt(const Segment *segments, int count) {
    printf("Gantt Chart: ");
    for (int i = 0; i < count; i++) {
        char label[8];
        if (segments[i].pid == 0) {
            snprintf(label, sizeof(label), "IDLE");
        } else {
            snprintf(label, sizeof(label), "P%d", segments[i].pid);
        }
        printf("| %-4s ", label);
    }
    printf("|\n");

    printf("             ");
    for (int i = 0; i < count; i++) {
        char time_text[16];
        int length = snprintf(time_text, sizeof(time_text), "%d", segments[i].start);
        int pad = 7 - length;
        if (pad < 1) {
            pad = 1;
        }
        printf("%s", time_text);
        for (int space = 0; space < pad; space++) {
            putchar(' ');
        }
    }
    printf("%d\n", segments[count - 1].end);
}

static void print_results(const char *title, Process processes[], int n,
                          const Segment *segments, int segment_count) {
    double total_waiting = 0.0;
    double total_turnaround = 0.0;
    double total_response = 0.0;

    qsort(processes, (size_t)n, sizeof(Process), compare_pid);

    printf("=== %s ===\n", title);
    print_gantt(segments, segment_count);
    printf("PID     AT     BT     WT     TAT    RT\n");
    for (int i = 0; i < n; i++) {
        printf("%-8d%-7d%-7d%-7d%-7d%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].response_time);
        total_waiting += processes[i].waiting_time;
        total_turnaround += processes[i].turnaround_time;
        total_response += processes[i].response_time;
    }
    printf("\n");
    printf("Average Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Response Time: %.2f\n", total_response / n);
}

static int next_arrival(const Process processes[], const int done[], int n) {
    int arrival = -1;
    for (int i = 0; i < n; i++) {
        if (!done[i] && (arrival == -1 || processes[i].arrival_time < arrival)) {
            arrival = processes[i].arrival_time;
        }
    }
    return arrival;
}

static void add_idle(Segment segments[], int *segment_count, int start, int end) {
    if (end <= start) {
        return;
    }
    segments[*segment_count].pid = 0;
    segments[*segment_count].start = start;
    segments[*segment_count].end = end;
    (*segment_count)++;
}

void fcfs(const Process input[], int n) {
    Process processes[MAX_PROCESSES];
    Segment segments[MAX_PROCESSES * 2];
    int segment_count = 0;
    int current = 0;

    memcpy(processes, input, (size_t)n * sizeof(Process));
    qsort(processes, (size_t)n, sizeof(Process), compare_fcfs);

    for (int i = 0; i < n; i++) {
        if (current < processes[i].arrival_time) {
            add_idle(segments, &segment_count, current, processes[i].arrival_time);
            current = processes[i].arrival_time;
        }
        set_metrics(&processes[i], current);
        segments[segment_count].pid = processes[i].pid;
        segments[segment_count].start = current;
        segments[segment_count].end = processes[i].completion_time;
        segment_count++;
        current = processes[i].completion_time;
    }

    print_results("First Come First Served (FCFS)", processes, n, segments, segment_count);
}

static int sjf_is_better(const Process *candidate, const Process *best) {
    if (candidate->burst_time != best->burst_time) {
        return candidate->burst_time < best->burst_time;
    }
    if (candidate->arrival_time != best->arrival_time) {
        return candidate->arrival_time < best->arrival_time;
    }
    return candidate->pid < best->pid;
}

void sjf(const Process input[], int n) {
    Process processes[MAX_PROCESSES];
    int done[MAX_PROCESSES] = {0};
    Segment segments[MAX_PROCESSES * 2];
    int segment_count = 0;
    int current = 0;
    int finished = 0;

    memcpy(processes, input, (size_t)n * sizeof(Process));

    while (finished < n) {
        int best = -1;
        for (int i = 0; i < n; i++) {
            if (done[i] || processes[i].arrival_time > current) {
                continue;
            }
            if (best == -1 || sjf_is_better(&processes[i], &processes[best])) {
                best = i;
            }
        }

        if (best == -1) {
            int arrival = next_arrival(processes, done, n);
            add_idle(segments, &segment_count, current, arrival);
            current = arrival;
            continue;
        }

        set_metrics(&processes[best], current);
        segments[segment_count].pid = processes[best].pid;
        segments[segment_count].start = current;
        segments[segment_count].end = processes[best].completion_time;
        segment_count++;
        current = processes[best].completion_time;
        done[best] = 1;
        finished++;
    }

    print_results("Shortest Job First (SJF)", processes, n, segments, segment_count);
}

static int read_processes(Process processes[], int *count) {
    int n;

    printf("Enter the number of processes: ");
    fflush(stdout);
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCESSES) {
        fprintf(stderr, "Number of processes must be between 1 and %d.\n", MAX_PROCESSES);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        int arrival;
        int burst;

        printf("Enter the arrival time and burst time for process %d: ", i + 1);
        fflush(stdout);
        if (scanf("%d %d", &arrival, &burst) != 2 || arrival < 0 || burst <= 0) {
            fprintf(stderr, "Arrival time must be >= 0 and burst time must be > 0.\n");
            return 0;
        }
        processes[i].pid = i + 1;
        processes[i].arrival_time = arrival;
        processes[i].burst_time = burst;
        processes[i].start_time = 0;
        processes[i].completion_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = 0;
    }

    *count = n;
    return 1;
}

int main(void) {
    Process processes[MAX_PROCESSES];
    int n = 0;

    if (!read_processes(processes, &n)) {
        return 1;
    }

    printf("\n");
    fcfs(processes, n);
    printf("\n");
    sjf(processes, n);
    return 0;
}
