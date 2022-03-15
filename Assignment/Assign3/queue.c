#include "queue.h"

LinkedQueue* InitQueue(int time_slice){
    LinkedQueue* LQueue = Create();
    LQueue->time_slice = time_slice;
    return LQueue;
}

int IsEmptyQueue(LinkedQueue* LQueue){
    return IsEmpty(LQueue);
}

int Length(LinkedQueue* LQueue){
    int i = 0;
    for (Node pt = LQueue; pt -> next != NULL; pt = pt -> next){
        i++;
    }
    return i;
}

LinkedQueue* EnQueue(LinkedQueue* LQueue, Process proc){
    return AddHead(LQueue, proc);
}

// void EnQueue(LinkedQueue* LQueue, Process proc){
//     AddHead(LQueue, proc);
// }

Process DeQueue(LinkedQueue* LQueue){
    if (IsEmpty(LQueue)){
        printf("Delete Error! Empty!");
        return LQueue->proc;
    }else{
        LinkedQueue* pt = LQueue;
        Node tmp;
        while (pt->next){
            if (pt->next->next == NULL){
                tmp = pt->next;
                pt->next = NULL;
                break;
            }
            pt = pt->next;
        }
        return tmp->proc;
    }
} 

Process FrontQueue(LinkedQueue* LQueue){
    if (IsEmpty(LQueue)){
        printf("Error! Empty!");
        return LQueue->proc;
    }else{
        LinkedQueue* pt = LQueue;
        Node tmp;
        while (pt->next){
            if (pt->next->next == NULL){
                tmp = pt->next;
                break;
            }
            pt = pt->next;
        }
        return tmp->proc;
    }
}

void QueuePrint(LinkedQueue* LQueue){
    // printf("Queue (Sorted by late to early dequeue order): \n");
    if (IsEmpty(LQueue)){
        printf("Empty!\n");
    }else {
        LlistPrint(LQueue);
    }
}
