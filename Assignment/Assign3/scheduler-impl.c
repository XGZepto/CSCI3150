#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

// Need to schedule the processes accroding to the arrival time and pid; an pain in the ass due to the way processes are sorted.
int fetch_new_job(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int highest_queue, int next_process_id, int current_time){
    printf("Fetch New Job.\n");

    int batch_bound = next_process_id;

    while(batch_bound < proc_num && current_time >= proc[batch_bound].arrival_time){
        batch_bound++;
    }

    while(next_process_id < batch_bound){
        int batch_time = proc[next_process_id].arrival_time;
        int cur_batch_limit = next_process_id;
        while(proc[cur_batch_limit].arrival_time == batch_time){
            cur_batch_limit++;
        }
        for (int i = cur_batch_limit - 1; i >= next_process_id; i--){
            Process *p = (Process*)malloc(sizeof(Process));
            p->turnaround_time = 0;
            p->service_time = 0;
            p->waiting_time = 0;
            p->completion_time = 0;
            p->process_id = proc[i].process_id;
            p->arrival_time = proc[i].arrival_time;
            p->execution_time = proc[i].execution_time;
            printf("EnQueue: %d\n", p->process_id);
            ProcessQueue[highest_queue] = EnQueue(ProcessQueue[highest_queue], *p);
        }
        next_process_id = cur_batch_limit;
    }
    return batch_bound == proc_num ? -1 : batch_bound;
}

void recursive_free(LinkedQueue* q){
    if (q->next != NULL){
        recursive_free(q->next);
    }
    free(q);
}

void priority_boost(LinkedQueue** ProcessQueue, int highest_queue){
    LinkedQueue* TempQueue = (LinkedQueue*)malloc(sizeof(LinkedQueue));
    TempQueue->next = NULL;
    int active_queue = highest_queue;
    while(active_queue >= 0){
        while(!IsEmptyQueue(ProcessQueue[active_queue])){
            TempQueue = EnQueue(TempQueue, DeQueue(ProcessQueue[active_queue]));
        }
        active_queue--;
    }
    // Brute Force Sorting by Process ID
    int flag = 1;
    while(flag){
        flag = 0;
        Process p;
        Process *pp;
        p.process_id = -1;
        LinkedQueue *pt = TempQueue;
        while(pt->next){
            pt = pt->next;
            if (pt->proc.process_id > p.process_id){
                p = pt->proc;
                p.service_time = 0;
                pp = &pt->proc;
                flag = 1;
            }
        }
        if (!flag) break;
        ProcessQueue[highest_queue] = EnQueue(ProcessQueue[highest_queue], p);
        pp->process_id = -1;
        printf("Boosted %d\n", p.process_id);
    }
    // Free TempQueue
    recursive_free(TempQueue);
}

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

        if (next_process_id >= 0)
            next_process_id = fetch_new_job(proc, ProcessQueue, proc_num, highest_queue, next_process_id, current_time);

        if (current_time && current_time % period == 0){-
            printf("Priority Boost.\n");
            priority_boost(ProcessQueue, highest_queue);
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
            Process current_proc = DeQueue(ProcessQueue[active_queue]);

            int current_pid = current_proc.process_id;
            int real_slice = min(min(current_proc.execution_time, ProcessQueue[active_queue]->time_slice), (period - (current_time % period)));
            int next_time = current_time + real_slice;
            current_proc.execution_time -= real_slice;
            current_proc.service_time += real_slice;        

            printf("Task %d found in active queue %d.\n",current_pid ,active_queue);
            outprint(current_time, next_time, current_pid, current_proc.arrival_time, current_proc.execution_time);

            current_time = next_time;

            if (current_proc.execution_time == 0){
                printf("Task %d finished.\n", current_pid);
            } else if (current_proc.service_time >= ProcessQueue[active_queue]->allotment_time){
                printf("Task %d deprioritised, serviced time %d excceds allotment %d.\n", current_pid, current_proc.service_time, ProcessQueue[active_queue]->allotment_time);
                active_queue = active_queue > 0 ? active_queue - 1 : 0;
                current_proc.service_time = 0;
                ProcessQueue[active_queue] = EnQueue(ProcessQueue[active_queue], current_proc);
            } else{
                printf("Task %d remains in active queue with serviced time %d.\n", current_pid, current_proc.service_time);
                ProcessQueue[active_queue] = EnQueue(ProcessQueue[active_queue], current_proc);
            }
        } else{
            current_time = proc[next_process_id].arrival_time;
        }
    }
}