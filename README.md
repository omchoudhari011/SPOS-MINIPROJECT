# SJF Scheduling for a Manufacturing System

SPOS Lab Mini Project (P1), CE24PCC306

## About the project

In a factory, one machine on the production line gets different tasks like cutting,
drilling, welding, painting and packing. Every task takes a different amount of time.
If the machine just does them in the order they come (FCFS), a small task can get stuck
waiting behind a big one.

So in this project we used Shortest Job First (SJF) scheduling. Whenever the machine
becomes free, it checks which tasks have already arrived and picks the one with the
smallest processing time. Each task is treated like a process, its processing time is
the burst time, and the machine is like the CPU.

We also run FCFS on the same tasks so we can compare the waiting times of both.

## Files

- `sjf_scheduler.cpp` - main program (C++)
- `sample_output.txt` - output of the program for the sample tasks
- `ui-preview/index.html` - simple web page (HTML, CSS, JavaScript) that does the same SJF and FCFS scheduling and shows the Gantt chart. Just open it in a browser

## How to run

```
g++ sjf_scheduler.cpp -o sjf
./sjf
```

Menu of the program:

```
1. Enter production tasks
2. Load sample tasks
3. Show task list
4. Run SJF scheduling
5. Run FCFS scheduling
6. Compare SJF vs FCFS
7. Exit
```

For a quick demo press 2 (sample tasks) and then 4, 5 and 6.

## Sample input

| Task | Name | Arrival time | Processing time |
|---|---|---|---|
| T1 | Cutting | 0 | 8 |
| T2 | Drilling | 1 | 4 |
| T3 | Welding | 2 | 9 |
| T4 | Painting | 3 | 5 |
| T5 | Packing | 4 | 2 |

## Output (SJF)

```
Gantt Chart:
 -----------------------------------
|   T1 |   T5 |   T2 |   T4 |   T3 |
 -----------------------------------
0      8     10     14     19     28

Task  Name          AT   BT   CT  TAT   WT
------------------------------------------
T1    Cutting        0    8    8    8    0
T2    Drilling       1    4   14   13    9
T3    Welding        2    9   28   26   17
T4    Painting       3    5   19   16   11
T5    Packing        4    2   10    6    4

Average Waiting Time    = 8.20
Average Turnaround Time = 13.80
```

At time 0 only T1 has arrived so it runs first. When T1 finishes at 8, all the other
tasks are waiting, so the machine takes T5 (2 min) first, then T2 (4), T4 (5) and
finally T3 (9).

## SJF vs FCFS

|  | FCFS | SJF |
|---|---|---|
| Avg waiting time | 11.40 | 8.20 |
| Avg turnaround time | 17.00 | 13.80 |

For these tasks SJF reduced the average waiting time by about 28%.
Full output is in `sample_output.txt`.

## Theory

CPU scheduling decides which process in the ready queue gets the CPU next. In SJF, the
process with the smallest burst time is picked first. We used the non preemptive
version, which means once a task starts on the machine it runs till it is finished.

SJF gives the minimum average waiting time compared to other non preemptive
algorithms, because short jobs don't have to wait behind long jobs.

The problem with SJF is starvation. If short tasks keep coming, a long task may keep
waiting. This can be solved by aging (slowly increasing the priority of tasks that are
waiting for a long time). Also SJF needs the burst time in advance. In a factory this
is usually known, since every operation has a fixed processing time.

Formulas used:
- Turnaround Time (TAT) = Completion Time - Arrival Time
- Waiting Time (WT) = TAT - Burst Time

## Algorithm

1. Take the number of tasks and for each task its name, arrival time and processing time.
2. Start with time = 0.
3. From the tasks that have arrived and are not done, pick the one with the smallest processing time. If two have the same time, pick the one that came first.
4. If no task has arrived yet, the machine is idle, so jump to the next arrival time.
5. Run the selected task fully. time = time + burst time, and completion time = time.
6. Repeat steps 3 to 5 till all tasks are done.
7. Find TAT and WT of each task and their averages, then print the Gantt chart and table.

## Functions in the program

- `inputTasks()` - takes tasks from the user
- `loadSample()` - loads the 5 sample tasks
- `sjf()` - does the SJF scheduling
- `fcfs()` - does the FCFS scheduling (for comparison)
- `calculate()` - finds TAT and WT
- `printGantt()` and `printTable()` - print the output
- `compare()` - shows average WT and TAT of both algorithms

## Conclusion

We implemented Shortest Job First scheduling in C++ for a manufacturing production
line. For the sample tasks, SJF brought the average waiting time down from 11.40 to 8.20
and the average turnaround time from 17.00 to 13.80 compared to FCFS. So running the
shorter tasks first reduces the overall waiting time in the production process.

## Viva questions

1. What is SJF?
   It is a scheduling algorithm where the job with the smallest burst time runs first.

2. What is the difference between preemptive and non preemptive SJF?
   In preemptive SJF (also called SRTF), if a shorter job arrives the running job is stopped. In non preemptive SJF, which we used, the running job always finishes first.

3. Why does SJF give the lowest average waiting time?
   When a short job runs before a long job, the short job's waiting time goes down by more than the long job's waiting time goes up.

4. What is the main disadvantage of SJF?
   Long jobs can starve, and we need to know the burst time beforehand.

5. How can starvation be removed?
   By aging, which means increasing the priority of a job the longer it waits.

6. What happens if two tasks have the same burst time?
   The task that arrived first gets the machine first.

7. Does SJF finish all the tasks faster than FCFS?
   No. Both finish all tasks at time 28 because the total work is the same. SJF only changes the order, so on average each task waits less.
