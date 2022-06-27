/* Jan Leo Ras
 * CSCI 4100 Assignment 3
 * Source file for message queue implementation
 */

#include "msg_queue.h"

/* Initialize the message queue */
void mq_init(struct msg_queue * mq) {
  //initialize queue message count
  mq->in = 0;
  mq->out = 0;
  
  //initialize the lock and condition variables
  pthread_mutex_init(&mq->lock, NULL);
  pthread_cond_init(&mq->msg_added, NULL);
  pthread_cond_init(&mq->msg_removed, NULL);
}

/* Send a message to a message queue */
void mq_send(struct msg_queue * mq, struct mq_msg * message) {
  //acquire the queue lock
  pthread_mutex_lock(&mq->lock);
  
  //while the queue is full, wait on the queue's msg_removed condition variable
  while((mq->in - mq->out) == QUEUE_SIZE) 
  {
    pthread_cond_wait(&mq->msg_removed, &mq->lock);
  }
  
  //copy the message, increment queue
  mq->messages[mq->in - mq->out] = *message;
  mq->in++;
  
  //signal the queue's msg_added condition variable
  pthread_cond_signal(&mq->msg_added);
  
  //release the queue lock
  pthread_mutex_unlock(&mq->lock);
  
}

/* Receive a message from a message queue */
void mq_receive(struct msg_queue *mq, struct mq_msg * message) {
  //acquire the queue lock
  pthread_mutex_lock(&mq->lock);
  
  //while the queue is empty, wait on condition variable
  while((mq->in == mq->out))
  {
    pthread_cond_wait(&mq->msg_added, &mq->lock);
  }
  
  //copy the message and increment variable
  message = &mq->messages[mq->in - mq->out];
  mq->out++;
  
  //signal the queue's msg_removed condition variable
  pthread_cond_signal(&mq->msg_removed);
  
  //release the queue lock
  pthread_mutex_unlock(&mq->lock);
}

