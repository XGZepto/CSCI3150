#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct process {
    int process_id;
    int arrival_time;
    int service_time;
    int execution_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
} Process;

typedef struct node {
    Process proc;
    int time_slice;
    int allotment_time;
    struct node *next;
} LinkedList, *Node;

LinkedList* Create();
int IsEmpty(LinkedList* Llist);
Node Find(LinkedList* Llist, Process proc);
LinkedList* AddTail(LinkedList* Llist, Process proc);
LinkedList* AddHead(LinkedList* Llist, Process proc);
LinkedList* Add(LinkedList* Llist, Process proc, int position);
LinkedList* DeleteTail(LinkedList* Llist);
LinkedList* DeleteHead(LinkedList* Llist);
LinkedList* Delete(LinkedList* Llist, int position);
void LlistPrint(LinkedList* Llist);

#endif