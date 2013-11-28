/* 
 * program : testqueue.c
 * by      : Jerry Hsieh
 * date    : Sep. 18, 1991
 * purpose : driver for test queue package
 *
 * history : December 1991 - eb borrowed code from eoneil, installed
 */

#include "queue.h"
/* the actual queue memory objects-- */
Queue q1obj,q2obj;
int main()
{
  int i, c;
  /* NOTE: very important to have ptrs pointing to good objects!! */
  Queue *q1 = &q1obj;	   /* we set up pointers to queue mem objects */
  Queue *q2 = &q2obj;	   /* Now use these ptrs as representing their objs */

  printf("setting up q1 and q2, with 6 spots each\n");
  init_queue(q1, 6); 
  init_queue(q2, 6);

  printf("put 'ab' in q1, dequeue and print: ");
  enqueue(q1,'a');
  enqueue(q1,'b');
  printf(" %c\n", dequeue(q1));

  printf("put 'cdefgh' (last one overflows) in q1:\n");
  /* note that in C, chars are just small ints, can be held in int type */
  for (c = 'c'; c < 'i'; c++)
      if (enqueue(q1, c)<0) printf("overflow\n");

  printf("now try to dequeue 8 elements, 2 will fail:\n");
  for (i = 0 ;i < 8; i ++) {
      char ch;

      printf("now queue1 contains %d elements. ", queuecount(q1));
      ch =  dequeue(q1);
      if (ch > 0)
	  printf(" dequeue returned %c\n", ch);
      else
	  printf(" dequeue failed, queue empty\n");
  }

  printf("\nnow queue1 contains %d elements\n", queuecount(q1));
  printf("Emptyqueue returns %d\n", emptyqueue(q1));
  printf("enqueue ab in q1, d in q2\n");

  enqueue(q1,'a');
  enqueue(q1,'b');
  enqueue(q2,'d');

  printf("got %c from q2 \n", dequeue(q2));
  return 0;
}
