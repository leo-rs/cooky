// Jan Leo Ras
// CSCI 4100
// Programming Assignment 4
// Source file for Dining Philosophers simulation
// This program implements a simulation of Djikstra's solution to the Dining Philosophers problem 
// using threads, locks, and condition variables

#include "dining_room.h"

// Tests to see if it is safe for the philosopher with the ID provided to eat
// If it is safe, it changes the philosopher's state to EATING and returns
// true, if not it returns false.
// This function must only be called after table_lock has been acquired
int test_phil(struct dining_room *room,  int phil)
{
	
	if ((room->phil_state[phil] == HUNGRY) && (room->phil_state[left_neighbor(room, phil)] != EATING) && (room->phil_state[right_neighbor(room, phil)] != EATING)) {
		return 1;
	}
	else
	  return 0; 
}

// Starts a dining philosopher simulation
void run_simulation(struct dining_room *room)
{
  //display headings and philosophers initial states
  display_headings(room);
  display_states(room);
  
  //start philosopher thread
  for (int i = 0; i < room->num_phils; i++) {
    pthread_create(&room->phil_threads[i], NULL, start_philosopher, &room->phil_args[i]);
  }
  
  //wait for each thread to complete
  for (int j = 0; j < room->num_phils; j++) {
    pthread_join(room->phil_threads[j], NULL);
  }
  
}

// Simulates a philosopher picking up forks
void grab_forks(struct dining_room *room, int phil)
{
  
  //acquire table lock, set state for phil to HUNGRY, and display curr state of phils
  pthread_mutex_lock(&room->table_lock);
	room->phil_state[phil] = HUNGRY;
	display_states(room);

  //until it is safe to eat, wait on condition variable for phil in the safe_to_eat array
	while (test_phil(room, phil) == 0) {
		pthread_cond_wait(&room->safe_to_eat[phil], &room->table_lock);
	}

  //set curr state of phil to EATING
	room->phil_state[phil] = EATING;
	
	//display current state of phils
	display_states(room);
	
	//release table lock
	pthread_mutex_unlock(&room->table_lock);
  
}

// Simulates a philosopher putting down forks
void release_forks(struct dining_room *room, int phil )
{
  //acquire table lock, set state for phil to THINKING, and display curr state of phils
  pthread_mutex_lock(&room->table_lock);
	room->phil_state[phil] = THINKING;
	display_states(room);

  //get left and right neighbor phil IDs
	int phil_l = left_neighbor(room, phil);
	int phil_r = right_neighbor(room, phil);

  //test if it is safe for each of neighbors to eat
  //if it is, notify phil using appropriate condition in safe_to_eat array
	if (test_phil(room, phil_l) == 1) {
		pthread_cond_signal(&room->safe_to_eat[phil_l]);
	}
	if (test_phil(room, phil_r) == 1) {
		pthread_cond_signal(&room->safe_to_eat[phil_r]);
	}

  //release table lock
	pthread_mutex_unlock(&room->table_lock);
}

// Displays the headings for the state change table
void display_headings(struct dining_room *room)
{
  int phil;
  for(phil = 0; phil < room->num_phils; phil++)
    printf("PHIL %-5d", phil);
  printf("\n");
}

// Displays the current state of all philosophers.
// This should only be called if table_lock has been acquired
void display_states(struct dining_room *room)
{
  int phil;
  for(phil = 0; phil < room->num_phils; phil++) {
      switch(room->phil_state[phil]) {
      case THINKING: printf("%-10s", "THINKING"); break;
      case HUNGRY: printf("%-10s", "HUNGRY"); break;
      case EATING: printf("%-10s", "EATING"); break;
      default: printf("%-10s", "CONFUSED");
      }
    }
  printf("\n");
  fflush(stdout);
}

// Gets the ID of the left neighbor of phil
int left_neighbor(struct dining_room *room, int phil )
{
  return phil == room->num_phils - 1? 0 : phil + 1;
}

// Gets the ID of the right neighbor of phil
int right_neighbor(struct dining_room *room, int phil )
{
  return phil == 0 ? room->num_phils - 1 : phil - 1;
}

// Simulates a philosopher thinking
void think( )
{
  unsigned int seed = time(0);
  srand(seed);
  usleep(rand() % 500000);
}

// Simulates a philosopher eating
void eat( )
{
  unsigned int seed = time(0);
  srand(seed);
  usleep(rand() % 500000);
}

// Code for a philosopher thread to run.
// This function should be used when creating a new philosopher thread
void *start_philosopher( void *the_args )
{
  struct p_args *args = (struct p_args *)the_args;  
  int i;
  for(i = 0; i < args->num_cycles; i++) {
    think();
    grab_forks(args->room, args->phil_num);
    eat();
    release_forks(args->room, args->phil_num);
  }
  return NULL;
}

// Initializes a dining_room structure
// num_phils is the number of philosophers
// num_cycles is the number of times each philosopher tries to eat
void init_dining_room(struct dining_room *room, int num_phils, int num_cycles )
{
  // Check for valid number of philosophers
  if(num_phils > MAX_PHILS || num_phils < 1) {
    fprintf(stderr, "Error: invalid number of philosophers");
    exit(1);
  }

  // Check for valid number of cycles
  if(num_cycles < 1) {
    fprintf(stderr, "Error: invalid number of philosophers");
    exit(1);
  }
  
  // Initialize the simulation parameters
  room->num_phils = num_phils;
  room->num_cycles = num_cycles;
  
  // Initialize the table lock
  pthread_mutex_init(&room->table_lock, NULL);
  
  // Initialize the data in the arrays
  int phil;
  for(phil = 0; phil < num_phils; phil++) {
    room->phil_state[phil] = THINKING;
    pthread_cond_init(&room->safe_to_eat[phil], NULL);
    room->phil_args[phil].phil_num = phil;
    room->phil_args[phil].num_cycles = num_cycles;
    room->phil_args[phil].room = room;
  }
} 
