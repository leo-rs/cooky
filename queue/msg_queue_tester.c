/* Jan Leo Ras
 * CSCI 4100 Assignment 3
 * This program tests a message queue by creating 3 sending threads and 
 * 4 receiving threads
 */

#include <stdio.h>
#include <stdlib.h>
#include "msg_queue.h"

/* Structure to hold thread arguments */
struct thread_args {
  int t_id;
  int num_msgs;
  struct msg_queue * mq;
};

/* Function prototypes */
void thread_args_init(struct thread_args *args, int t_id, int num_msgs,
		      struct msg_queue * mq);
void * send_msgs(void * args);
void * receive_msgs(void * args);

/* Main function */
int main(int argc, char *argv[]) {

  //declare variable for each sending and receiving thread
  pthread_t sender_0;
  pthread_t sender_1;
  pthread_t sender_2;
  
  pthread_t rec_0;
  pthread_t rec_1;
  pthread_t rec_2;
  pthread_t rec_3;
  
  //declare msg_queue structure and initialize
  struct msg_queue my_msg_queue;
  mq_init(&my_msg_queue);
  
  //declare thread_args structure for each of the sending and receiving threads 
  //and initialize
  struct thread_args args_s0;
  struct thread_args args_s1;
  struct thread_args args_s2;
  
  struct thread_args args_r0;
  struct thread_args args_r1;
  struct thread_args args_r2;
  struct thread_args args_r3;
  
  thread_args_init(&args_s0, 0, 8, &my_msg_queue); 
  thread_args_init(&args_s1, 1, 8, &my_msg_queue); 
  thread_args_init(&args_s2, 2, 8, &my_msg_queue); 
  
  thread_args_init(&args_r0, 0, 6, &my_msg_queue); 
  thread_args_init(&args_r1, 1, 6, &my_msg_queue); 
  thread_args_init(&args_r2, 2, 6, &my_msg_queue); 
  thread_args_init(&args_r3, 3, 6, &my_msg_queue); 
  
  //start each of the sending threads
	pthread_create(&sender_0, NULL, send_msgs, &args_s0);
	pthread_create(&sender_1, NULL, send_msgs, &args_s1);
	pthread_create(&sender_2, NULL, send_msgs, &args_s2);
	
	//start each of the receiving threads
	pthread_create(&rec_0, NULL, receive_msgs, &args_r0);
	pthread_create(&rec_1, NULL, receive_msgs, &args_r1);
	pthread_create(&rec_2, NULL, receive_msgs, &args_r2);
	pthread_create(&rec_3, NULL, receive_msgs, &args_r3);

  //wait for each to complete
  pthread_join(sender_0, NULL);
  pthread_join(sender_1, NULL);
  pthread_join(sender_2, NULL);
  pthread_join(rec_0, NULL);
  pthread_join(rec_1, NULL);
  pthread_join(rec_2, NULL);
  pthread_join(rec_3, NULL);

	return 0;
}

/* Initialize thread_args structure */
void thread_args_init(struct thread_args *args, int t_id, int num_msgs,
		      struct msg_queue * mq) {

	args->t_id = t_id;
	args->num_msgs = num_msgs;
	args->mq = mq;
}

/* Function for sending thread to execute */
void * send_msgs(void * args) {
  //cast void pointer args to a pointer to a thread_args structure
  //declare mq_msg structure
  struct thread_args *real_args = (struct thread_args *) args;
  struct mq_msg my_msg;

  //initialize mq_msg with thread id and msg id
  //send the message to queue using mq_send
	int i;
	for (i = 0; i < real_args->num_msgs; i++) {
		my_msg.t_id = real_args->t_id;
		my_msg.m_id = i;
		mq_send(real_args->mq, &my_msg);
	}

	return NULL;
}

/* Function for receiving thread to execute */
void * receive_msgs(void * args) {
  //cast void pointer args to a pointer to a thread_args structure
  //declare a mq_msg structure
  struct thread_args *real_args = (struct thread_args *) args;
  struct mq_msg my_msg;

  //receive msgs using mq_receive and display
	int i;
	for (i = 0; i < real_args->num_msgs; i++) {
		mq_receive(real_args->mq, &real_args->mq->messages[i]);
		my_msg.t_id = real_args->mq->messages[i].t_id;
		my_msg.m_id = real_args->mq->messages[i].m_id;
		printf("[sender: %d, msg id: %d, receiver: %d\n", my_msg.t_id, my_msg.m_id, real_args->t_id);
	}

	return NULL;
}
