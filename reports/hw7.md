**Process Scheduling Report (FCFS and SJF)**

The program in [homeworks/hw7/scheduler.c](../homeworks/hw7/scheduler.c) is a non-preemptive CPU scheduling simulator. It reads a list of processes and runs that same list through First Come First Served and Shortest Job First. Each algorithm has its own function, and `main` calls `fcfs` and then `sjf`.

**Process structure**

Each process is stored in one struct:

- `pid` is assigned from the input order. The first process entered is P1.
- `arrival_time` is when the process becomes ready.
- `burst_time` is how long it needs the CPU.
- `turnaround_time`, `waiting_time`, and `response_time` are filled in after the schedule is built.

The original input is copied before each algorithm sorts or selects processes, so running FCFS does not change the data that SJF sees.

**Formulas**

Both algorithms here are non-preemptive. Once a process starts, it keeps the CPU until its burst is finished. That makes response time the same as waiting time.

For a process that starts at time S and finishes at time C:

```
C  = S + burst time
TAT = C - arrival time
WT  = TAT - burst time
RT  = S - arrival time
```

Waiting time is only the time after arrival. Time before the process exists is not waiting time, and it is not added into the averages. If the CPU has nothing ready, the Gantt chart shows that gap as `IDLE`.

The average of each metric is the sum over all processes divided by the number of processes.

**First Come First Served**

FCFS sorts by arrival time. If two processes have the same arrival time, the smaller PID goes first, which is the order they were typed in. The CPU then walks that list. If the next process has not arrived, the CPU stays idle until it does.

Sample input:

```
P1  arrival 0  burst 8
P2  arrival 1  burst 4
P3  arrival 2  burst 9
P4  arrival 3  burst 5
```

P1 is present at time 0, so it runs from 0 to 8. Waiting time is 0. Turnaround time is 8.

At time 8, P2 has been waiting since time 1. It runs from 8 to 12.

```
WT  = 8 - 1 = 7
TAT = 12 - 1 = 11
RT  = 7
```

P3 runs from 12 to 21. It arrived at 2, so its waiting time is 10 and its turnaround time is 19. P4 runs from 21 to 26. It arrived at 3, so its waiting time is 18 and its turnaround time is 23.

```
Average WT  = (0 + 7 + 10 + 18) / 4 = 8.75
Average TAT = (8 + 11 + 19 + 23) / 4 = 15.25
Average RT  = 8.75
```

The chart for this run is `| P1 | P2 | P3 | P4 |`, with boundaries 0, 8, 12, 21, 26.

**Shortest Job First**

SJF is also non-preemptive. When the CPU becomes free, it looks only at processes that have already arrived and have not run yet. It picks the shortest burst. If two ready processes have the same burst, it picks the earlier arrival. If that is also tied, it picks the smaller PID.

This is not Shortest Remaining Time First. A running process is not stopped when a shorter one arrives.

On the sample input, time 0 has only P1. P2, P3, and P4 are not in the system yet, so P1 must run from 0 to 8 even though its burst is not the shortest in the whole list.

At time 8 all three remaining processes are ready:

```
P2 burst 4
P4 burst 5
P3 burst 9
```

P2 is the shortest, so it runs from 8 to 12. At time 12, P4 is shorter than P3, so P4 runs from 12 to 17. P3 then runs from 17 to 26.

```
P1  WT = 0    TAT = 8    RT = 0
P2  WT = 7    TAT = 11   RT = 7
P4  WT = 9    TAT = 14   RT = 9     start 12, arrival 3
P3  WT = 15   TAT = 24   RT = 15    start 17, arrival 2

Average WT  = (0 + 7 + 15 + 9) / 4 = 7.75
Average TAT = (8 + 11 + 24 + 14) / 4 = 14.25
Average RT  = 7.75
```

The handout's SJF table matches these numbers. Its Gantt chart does not. The handout draws `| P2 | P4 | P1 | P3 |`, but P2 arrives at time 1, so it cannot be the first bar, and the waiting times in that same sample say P1 started at time 0. The program follows the table, not that chart. The chart it prints is `| P1 | P2 | P4 | P3 |`.

SJF gives a lower average wait than FCFS on this input, 7.75 instead of 8.75, because after P1 finishes it runs the burst of 4 before the bursts of 5 and 9. FCFS instead runs the burst of 9 before the burst of 5.

**Edge cases**

Same arrival time does not make the order undefined. FCFS uses PID. SJF still prefers the shorter burst, and uses arrival time and then PID only as tie breaks.

If every process arrives at time 0, FCFS runs P1, P2, P3 in input order. SJF reorders them by burst. For bursts 6, 2, and 4 the SJF order is P2, P3, P1. P2 waits 0, P3 waits 2, and P1 waits 6. The average waiting time is 8 / 3 = 2.67, against 4.67 for FCFS.

A burst tie is broken by arrival time. With P1 (arrival 0, burst 7), P2 (arrival 2, burst 4), P3 (arrival 4, burst 4), and P4 (arrival 1, burst 9), P1 is alone at time 0, so it runs first under both algorithms. After time 7, P2 and P3 have the same burst. P2 arrived earlier, so SJF runs P2, then P3, then P4. FCFS instead keeps arrival order and runs P4 before P2 and P3. The averages are 9.00 for FCFS and 6.50 for SJF.

Idle time is drawn, but it is not charged to a process that has not arrived. For P2 at time 0 for 3 units, P1 at time 4 for 2 units, and P3 at time 10 for 1 unit, the chart is `| P2 | IDLE | P1 | IDLE | P3 |` with boundaries 0, 3, 4, 6, 10, 11. Every waiting time is 0, because each process starts at its arrival time. The idle gaps are only the holes between arrivals.

A single process that arrives at time 5 also starts with an idle gap from 0 to 5. Its waiting time, response time, and the time before arrival are not the same thing. Waiting time stays 0. Turnaround time is its burst, 3.

Invalid input is rejected: no processes, more than 100 processes, a negative arrival time, or a burst time of 0.

**How to run**

```bash
cd homeworks/hw7
gcc -Wall -Wextra -std=c11 -o scheduler scheduler.c
./scheduler
```

Sample input:

```
Enter the number of processes: 4
Enter the arrival time and burst time for process 1: 0 8
Enter the arrival time and burst time for process 2: 1 4
Enter the arrival time and burst time for process 3: 2 9
Enter the arrival time and burst time for process 4: 3 5
```

**Screenshots**

> **[Screenshot 1 — insert here]**
> Terminal showing the sample input above and the full FCFS and SJF output. This is the screenshot the assignment asks for.

> **[Screenshot 2 — insert here, optional]**
> A second run that shows an idle gap or a burst-time tie. Useful input:
>
> ```
> 3
> 4 2
> 0 3
> 10 1
> ```
>
> The chart should contain two `IDLE` blocks, and every waiting time should be 0.00.

Github link: [https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw7](https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw7)
