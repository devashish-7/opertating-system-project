#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

void* ta_act();
void* student_act( void* student_id );

#define NUM_WAITING_CHAIRS 7
#define DEFAULT_NUM_STUDENTS 10

sem_t sem_ta;
sem_t sem_students;
pthread_mutex_t mutex_thread;
int next_teaching_pos = 0;
int ta_sleeping_flag = 0;
int next_seating_pos = 0;
int waiting_r_chairs[7];
int number_stu_waiting = 0;

int main( int arg, char **argc ){
	int i;
	int stu_num;

	if (arg > 1 ) {
		if ( isNumber( argc[1] ) == 1) {
			stu_num = atoi( argc[1] );
		}
		else {
			printf("Invalid input.");
			return 0;
		}
	}
	else {
		stu_num = DEFAULT_NUM_STUDENTS;
	}

	int stu_ids[stu_num];
	pthread_t students[stu_num];
	pthread_t ta;

    sem_init( &sem_ta, 0, 1 );
	sem_init( &sem_students, 0, 0 );
	
	pthread_mutex_init( &mutex_thread, NULL );
	pthread_create( &ta, NULL, ta_act, NULL );
	for( i = 0; i < stu_num; i++ )
	{
		stu_ids[i] = i + 1;
		pthread_create( &students[i], NULL, student_act, (void*) &stu_ids[i] );
	}

	pthread_join(ta, NULL);
	for( i =0; i <stu_num; i++ )
	{
		pthread_join( students[i],NULL );
	}

	return 0;
}

void* student_act( void* student_id ) {

	int id_student = *(int*)student_id;

	while(1) {
		if ( isWaiting( id_student ) == 1 ) 
		{ continue; }
		int time = rand() % 5;
		printf( "\tStudent %d is programming for %d seconds.\n", id_student, time );
		sleep( time );

		pthread_mutex_lock( &mutex_thread );

		if( number_stu_waiting < NUM_WAITING_CHAIRS ) {

			waiting_r_chairs[next_seating_pos] = id_student;
			number_stu_waiting++;
			printf( "\t\tStudent %d takes a seat. Students waiting = %d.\n", id_student, number_stu_waiting );
			next_seating_pos = ( next_seating_pos + 1 ) % NUM_WAITING_CHAIRS;

			pthread_mutex_unlock( &mutex_thread );
			sem_post( &sem_students );
			sem_wait( &sem_ta );

		}
		else {

			pthread_mutex_unlock( &mutex_thread );
			printf( "\t\t\tStudent %d will try later.\n",id_student );

		}

	}

}

void* ta_act() {

	printf( "Checking for students.\n" );

	while( 1 ) {

		if ( number_stu_waiting > 0 ) {

			ta_sleeping_flag = 0;
			sem_wait( &sem_students );
			pthread_mutex_lock( &mutex_thread );
			int h_t = rand() % 5;

			printf( "Helping a student for %d seconds. Students waiting = %d.\n",
			 h_t, (number_stu_waiting - 1) );
			printf( "Student %d receiving help.\n",
			waiting_r_chairs[next_teaching_pos] );

			waiting_r_chairs[next_teaching_pos]=0;
			number_stu_waiting--;
			next_teaching_pos = ( next_teaching_pos + 1 ) % NUM_WAITING_CHAIRS;

			sleep( h_t );

			pthread_mutex_unlock( &mutex_thread );
			sem_post( &sem_ta );

		}
		else {

			if ( ta_sleeping_flag == 0 ) {

				printf( "No students waiting. Sleeping.\n" );
				ta_sleeping_flag = 1;

			}

		}

	}

}

int isWaiting( int student_id ) {
	int i;
	for ( i = 0; i < 3; i++ ) {
		if ( waiting_r_chairs[i] == student_id ) { return 1; }
	}
	return 0;
}
int isNumber(char number[])
{
    int i;
		for ( i = 0 ; number[i] != 0; i++ )
    {
        if (!isdigit(number[i]))
            return 0;
    }
    return 1;
}


