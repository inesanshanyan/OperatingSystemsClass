# Homework 7 — FCFS and SJF Scheduling

A non-preemptive process scheduling simulator written in C. It runs the same process list through First Come First Served and Shortest Job First, then prints a Gantt chart and the waiting, turnaround, and response times.

## Build and run

From this directory:

```bash
gcc -Wall -Wextra -std=c11 -o scheduler scheduler.c
./scheduler
```

The program asks for the number of processes, then for each process's arrival time and burst time. Process IDs are assigned in input order, starting at 1.

```text
Enter the number of processes: 4
Enter the arrival time and burst time for process 1: 0 8
Enter the arrival time and burst time for process 2: 1 4
Enter the arrival time and burst time for process 3: 2 9
Enter the arrival time and burst time for process 4: 3 5
```

Arrival time must be 0 or greater. Burst time must be greater than 0. At most 100 processes are accepted.

## What is printed

For each algorithm:

- A Gantt chart. The numbers under the bars are the start time of each block and the finish time of the last block. `IDLE` is a gap where no process has arrived yet.
- One row per process, sorted by PID: arrival time (AT), burst time (BT), waiting time (WT), turnaround time (TAT), and response time (RT).
- The average waiting, turnaround, and response times.

Both algorithms are non-preemptive, so response time and waiting time are equal: a process does not leave the CPU until its burst is finished.

## Scheduling rules

**FCFS.** Processes run in arrival order. If two processes arrive at the same time, the smaller PID runs first.

**SJF.** When the CPU is free, the ready process with the shortest burst runs next. A tie on burst time is broken by earlier arrival time, then by smaller PID. A process that has not arrived yet cannot be chosen. If a long process is the only one present, it runs to completion even if a shorter process arrives later.
