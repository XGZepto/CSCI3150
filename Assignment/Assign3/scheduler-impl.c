#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }

    int current_time = 0;
    int next_process_id = 0;
    int active_queue = 0;
    int highest_queue = queue_num - 1;

    while(1){
        printf("\n");
        while (next_process_id >= 0 && current_time >= proc[next_process_id].arrival_time){
            Process *p = (Process*)malloc(sizeof(Process));
            memset(p, 0, sizeof(p));
            p->process_id = proc[next_process_id].process_id;
            p->arrival_time = proc[next_process_id].arrival_time;
            p->execution_time = proc[next_process_id].execution_time;
            printf("EnQueue: %d\n", p->process_id);
            ProcessQueue[highest_queue] = EnQueue(ProcessQueue[highest_queue], *p);
            next_process_id++;
            if (next_process_id == proc_num)
                next_process_id = -1;
        }

        // PriorityBoost, not sorted yet.
        if (current_time && current_time % period == 0){-
            printf("Priority Boost.\n");
            active_queue = highest_queue - 1;
            while(active_queue >= 0){
                while(!IsEmptyQueue(ProcessQueue[active_queue])){
                    ProcessQueue[highest_queue] = EnQueue(ProcessQueue[highest_queue], ProcessQueue[active_queue]->next->proc);
                }
                active_queue--;
            }
            if (IsEmptyQueue(ProcessQueue[highest_queue])){
                break;
            }
        }

        // Find Process to Run
        active_queue = highest_queue;
        while(active_queue >= 0){
            if (!IsEmptyQueue(ProcessQueue[active_queue])) break;
            active_queue--;
        }

        printf("current_time: %d\n", current_time);
        printf("next_process_id: %d\n", next_process_id);
        printf("active_queue: %d\n", active_queue);
        

        if (active_queue < 0 && next_process_id < 0){
            break;
        }

        if (active_queue >= 0){
            
            printf("task found in active queue %d.\n", active_queue);
            LlistPrint(ProcessQueue[active_queue]);

            Process current_proc = ProcessQueue[active_queue]->next->proc;
            
            int current_pid = current_proc.process_id;
            int real_slice = min(min(current_proc.execution_time, ProcessQueue[active_queue]->time_slice), (period - (current_time % period)));
            int next_time = current_time + real_slice;
            current_proc.execution_time -= real_slice;
            current_proc.service_time += real_slice;
            outprint(current_time, next_time, current_pid, current_proc.arrival_time, current_proc.execution_time);

            current_time = next_time;

            if (current_proc.execution_time == 0){
                printf("Task %d finished.\n", current_pid);
                ProcessQueue[active_queue] = DeleteHead(ProcessQueue[active_queue]);
            } else if (current_proc.service_time >= ProcessQueue[active_queue]->allotment_time){
                printf("Task %d deprioritised.\n", current_pid);
                current_proc.service_time = 0;
                DeleteHead(ProcessQueue[active_queue]);
                AddTail(ProcessQueue[active_queue-1], current_proc);
            } else{
                printf("Task %d remains in active queue.\n", current_pid);
                DeleteHead(ProcessQueue[active_queue]);
                AddTail(ProcessQueue[active_queue], current_proc);
                LlistPrint(ProcessQueue[active_queue]);

            }
        } else{
            current_time = proc[next_process_id].arrival_time;
        }
    }
}

