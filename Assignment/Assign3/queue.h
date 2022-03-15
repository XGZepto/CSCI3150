#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

typedef LinkedList LinkedQueue;

LinkedQueue* InitQueue(int time_slice);
int IsEmptyQueue(LinkedQueue* LQueue);
int Length(LinkedQueue* LQueue);
LinkedQueue* EnQueue(LinkedQueue* LQueue, Process proc);
Process DeQueue(LinkedQueue* LQueue);
Process FrontQueue(LinkedQueue* LQueue);
void QueuePrint(LinkedQueue* LQueue);

#endif