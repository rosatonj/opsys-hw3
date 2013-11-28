/*
 * file    : queue.h
 * by      : Jerry Hsieh
 * date    : Sep. 18, 1991
 * purpose : queue package header file
 * history :
 */

#ifndef QUEUE_H
#define QUEUE_H

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define EMPTYQUE (-1)            /* if queue is empty */
#define FULLQUE  (-1)            /* if queue is full */

#define MAXCHARBUF 100         /* maximum char size of total char */

typedef struct queue {      
  char ch[MAXCHARBUF];         /* char contain in queue */
  int front;                   /* front index of queue */
  int rear;                    /* rear index of queue */
  int count;                   /* current numbers of element in queue */ 
  int max;                     /* actually use length */
} Queue;


/* functions prototype */
/* initialize a queue with max capacity max_chars, fill in pointed-to
   Queue structure (an empty one was provided by caller)-- */

extern int init_queue(Queue *q, int max_chars);

/* add char ch to the specified queue--returns FULLQUE if q full */
extern int enqueue(Queue *, char);

/* take one char out of spec. queue, rets EMPTYQUE if q empty */
extern int dequeue(Queue *);

/* report on how many chars in queue now */
extern int queuecount(Queue *);

/* returns TRUE or FALSE-- */
extern int emptyqueue(Queue *);

#endif 
