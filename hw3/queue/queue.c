/*
 * program : queue.c
 * by      : Jerry Hsieh (algorithm from Data structure and algorithms - AHU)
 * date    : Sep. 18, 1991
 * purpose : queue package ADT
 * history :
 */

#include "queue.h"                        

static int addone(Queue *queue, int i);        /* auxiliary function */     

/* ------------------------------------------------------------------------ */
/* algorithm from "Data structure and algorithm" - AHU  p62 */
/* that algorithm actually use maxlength - 1 elements, so we have to add 1 */
/* to the max_chars in order to let the caller use exactly max_chars element*/
/* Thus we need 7 array entries for 6 capacity, or capacity + 1 in general */

/* note: caller is responsible for having allocated space for queue */
int init_queue(Queue *queue, int max_chars)
{
  int maxlength = max_chars + 1 ;         /* waste a byte every time */

  if (maxlength > MAXCHARBUF) {           /* enough memory ? */
#ifdef SAPC
    kprintf("Error : not enough char buffer for this queue !\n");
#else
    printf("Error : not enough char buffer for this queue !\n");
#endif
    return FALSE;
  }
  else {
    queue->front = 1;
    queue->rear = 0;
    queue->count = 0;
    queue->max = maxlength;	/* actually use maxlength memory */
    return TRUE;
  }
}

/* ------------------------------------------------------------------------ */
/* algorithm from "Data structure and algorithm" - AHU  p62 */
int emptyqueue(Queue *queue)
{
  if (addone(queue,queue->rear) == queue->front)
    return TRUE;
  else
    return FALSE;
}


 /* ------------------------------------------------------------------------ */
/* algorithm from "Data structure and algorithm" - AHU  p62 */
int enqueue(Queue *queue, char ch)
{
  if (addone(queue,addone(queue,queue->rear)) == queue->front)
    return FULLQUE;
  else {
    queue->rear = addone(queue,queue->rear);
    queue->ch[queue->rear] = ch;
    queue->count++;
    return ch;           /* successful */
  }
}

/* ------------------------------------------------------------------------ */
/* algorithm from "Data structure and algorithm" - AHU  p62 */
int dequeue(Queue *queue)
{
  char ch;

  if (emptyqueue(queue)) {
    return EMPTYQUE;
  }
  else {
    ch = queue->ch[queue->front];
    queue->front = addone(queue,queue->front);
    (queue->count) --;
    return ch;
  }
}

/* ------------------------------------------------------------------------ */
int queuecount(Queue *queue)
{
  return queue->count;
}


/* ------------------------------------------------------------------------ */
/* algorithm from "Data structure and algorithm" - AHU  p62 */
/* function that return next index, wrapping around in [0, max-1] */
static int addone(Queue *queue, int ind)
{
  return (ind + 1) % queue->max;
}









