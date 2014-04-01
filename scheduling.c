/*******************************************************************************
*
* CprE 308 Scheduling Lab
*
* scheduling.c
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_PROCESSES 20

struct process
{
  /* Values initialized for each process */
  int arrivaltime;  /* Time process arrives and wishes to start */
  int runtime;      /* Time process requires to complete job */
  int priority;     /* Priority of the process */

  /* Values algorithm may use to track processes */
  int starttime;
  int endtime;
  int flag;
  int remainingtime;
};

/* Forward declarations of Scheduling algorithms */
void first_come_first_served(struct process *proc);
void shortest_remaining_time(struct process *proc);
void round_robin(struct process *proc);
void round_robin_priority(struct process *proc);

int main()
{
  int i;
  struct process proc[NUM_PROCESSES],      /* List of processes */
                 proc_copy[NUM_PROCESSES]; /* Backup copy of processes */

  /* Seed random number generator */
  /*srand(time(0));*/  /* Use this seed to test different scenarios */
  srand(0xC0FFEE);     /* Used for test to be printed out */

  /* Initialize process structures */
  for(i=0; i<NUM_PROCESSES; i++)
  {
    proc[i].arrivaltime = rand()%100;
    proc[i].runtime = (rand()%30)+10;
    proc[i].priority = rand()%3;
    proc[i].starttime = 0;
    proc[i].endtime = 0;
    proc[i].flag = 0;
    proc[i].remainingtime = 0;
  }

  /* Show process values */
  printf("Process\tarrival\truntime\tpriority\n");
  for(i=0; i<NUM_PROCESSES; i++)
    printf("%d\t%d\t%d\t%d\n", i, proc[i].arrivaltime, proc[i].runtime,
           proc[i].priority);

  /* Run scheduling algorithms */
  printf("\n\nFirst come first served\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  first_come_first_served(proc_copy);

  printf("\n\nShortest remaining time\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  shortest_remaining_time(proc_copy);

  printf("\n\nRound Robin\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin(proc_copy);

  printf("\n\nRound Robin with priority\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin_priority(proc_copy);

  return 0;
}

void first_come_first_served(struct process *proc)
{
  /* Implement scheduling algorithm here */
  int i = 0;
  int run = 1;
  int average = 0;
  int current_time = 1;
  int proc_to_run = -1;
  int smallest_time = 0;

  // Start processing the processes
  while(run)
  {
    proc_to_run = -1;
    smallest_time = 2137483647;
    // find first arrival time, from end of array to beginning
    for(i = 0; i < NUM_PROCESSES; i++)
    {
      struct process *p = &proc[i];
      if(p->arrivaltime < smallest_time && !p->flag)
      {
        proc_to_run = i;
        smallest_time = p->arrivaltime;
      }
    }

    // Run the process
    if (proc_to_run >= 0)
    {
      struct process *p = &proc[proc_to_run];
      p->starttime = current_time;
      p->endtime = p->starttime + p->runtime;
      p->flag = 1;
      current_time += p->runtime;
      average += p->endtime - p->arrivaltime;

      // print info to the screen
      printf("Process %d started at time %d\n", proc_to_run, p->starttime);
      printf("Process %d finished at time %d\n", proc_to_run, p->endtime);
    }
    // There were no processes left to run
    else
    {
      printf("Average time from arrival to finish is %d\n", average / NUM_PROCESSES);
      run = 0;
    }

  }

}

void shortest_remaining_time(struct process *proc)
{
  /* Implement scheduling algorithm here */
  int i = 0;
  int run = 1;
  int average = 0;
  int current_time = 1;
  int proc_to_run = -1;
  int smallest_time = 0;

  // Start processing the processes
  while(run)
  {
    proc_to_run = -1;
    smallest_time = 2137483647;
    // find first arrival time, from end of array to beginning
    for(i = 0; i < NUM_PROCESSES; i++)
    {
      struct process *p = &proc[i];
      if(p->runtime < smallest_time && !p->flag)
      {
        proc_to_run = i;
        smallest_time = p->runtime;
      }
    }

    // Run the process
    if (proc_to_run >= 0)
    {
      struct process *p = &proc[proc_to_run];
      p->starttime = current_time;
      p->endtime = p->starttime + p->runtime;
      p->flag = 1;
      current_time += p->runtime;
      average += p->endtime - p->arrivaltime;

      // print info to the screen
      printf("Process %d started at time %d\n", proc_to_run, p->starttime);
      printf("Process %d finished at time %d\n", proc_to_run, p->endtime);
    }
    // There were no processes left to run
    else
    {
      printf("Average time from arrival to finish is %d\n", average / NUM_PROCESSES);
      run = 0;
    }

  }
}

void round_robin(struct process *proc)
{
  /* Implement scheduling algorithm here */
  int i = 0;
  int run = 1;
  int mod = 0;
  int average = 0;
  int current_time = 1;

  // Start processing the processes
  while(run)
  {
    mod = 0;
    // find first arrival time, from end of array to beginning
    for(i = 0; i < NUM_PROCESSES; i++)
    {
      struct process *p = &proc[i];

      // Prepare each process to run
      if(p->arrivaltime <= current_time && !p->flag)
      {
        mod = 1;
        if(!p->starttime)
        {
          p->starttime = current_time;
          p->remainingtime = p->runtime;
        }

        // Decrement the current process and check if done
        if(!p->remainingtime--)
        {
          p->endtime = current_time;
          p->flag = 1;
          average += p->endtime - p->arrivaltime;
          printf("Process %d started at time %d\n", i, p->starttime);
          printf("Process %d finished at time %d\n", i, p->endtime);
        }
        // Increment the current time
        current_time++;
      }
    }
    // Check if loop is complete
    if(!mod)
    {
      printf("Average time from arrival to finish is %d\n", average / NUM_PROCESSES);
      run = 0;
    }

  }
}

void round_robin_priority(struct process *proc)
{
  /* Implement scheduling algorithm here */
  int i = 0;
  int run = 1;
  int mod = 0;
  int average = 0;
  int proc_to_run = -1;
  int current_time = 1;
  int current_priority = 2;

  // Start processing the processes
  while(run)
  {
    mod = 0;
    // find first arrival time, from end of array to beginning
    for(i = 0; i < NUM_PROCESSES; i++)
    {
      struct process *p = &proc[i];

      // Prepare each process to run
      if(p->arrivaltime <= current_time && !p->flag && p->priority == 2)
      {
        mod = 1;
        if(!p->starttime)
        {
          p->starttime = current_time;
          p->remainingtime = p->runtime;
        }

        // Decrement the current process and check if done
        if(!p->remainingtime--)
        {
          p->endtime = current_time;
          p->flag = 1;
          average += p->endtime - p->arrivaltime;
          printf("Process %d started at time %d\n", i, p->starttime);
          printf("Process %d finished at time %d\n", i, p->endtime);
        }
        // Increment the current time
        current_time++;
      }
      // Prepare each process to run
      else if(p->arrivaltime <= current_time && !p->flag && p->priority == 1 && !mod)
      {
        mod = 1;
        if(!p->starttime)
        {
          p->starttime = current_time;
          p->remainingtime = p->runtime;
        }

        // Decrement the current process and check if done
        if(!p->remainingtime--)
        {
          p->endtime = current_time;
          p->flag = 1;
          average += p->endtime - p->arrivaltime;
          printf("Process %d started at time %d\n", i, p->starttime);
          printf("Process %d finished at time %d\n", i, p->endtime);
        }
        // Increment the current time
        current_time++;
      }
      // Prepare each process to run
      else if(p->arrivaltime <= current_time && !p->flag && p->priority == 0 && !mod)
      {
        mod = 1;
        if(!p->starttime)
        {
          p->starttime = current_time;
          p->remainingtime = p->runtime;
        }

        // Decrement the current process and check if done
        if(!p->remainingtime--)
        {
          p->endtime = current_time;
          p->flag = 1;
          average += p->endtime - p->arrivaltime;
          printf("Process %d started at time %d\n", i, p->starttime);
          printf("Process %d finished at time %d\n", i, p->endtime);
        }
        // Increment the current time
        current_time++;
      }
    }
    // Check if loop is complete
    if(!mod)
    {
      printf("Average time from arrival to finish is %d\n", average / NUM_PROCESSES);
      run = 0;  
    }

  }
}

