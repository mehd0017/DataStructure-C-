// C program for array implementation of priority queue to simulate the executions of processes in an operating system.
// this simulation include how the processes are going to be stored in a priority hashqueue and wait for its turn to excute and then dequeued. 

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <time.h>
#include <windows.h>


#define COUNT 100000 // Number of processes that will be stored in the queue.
#define SIZE 2000 // the size of the hash table


//Struct to store the times needed for each process.
//A dynamic array of this struct will be declared 
//in order to store the times that was needed for each processes
// to be inserted in the right place of the queue and the time needed to execute and finally to be dequeued  
typedef struct{
	clock_t insertion;
	clock_t oinsertion;
	clock_t deletion;
	clock_t odeletion;
	double execution;
} times;


// declaration of a global array that can be accessed from anywhere to store the clock values of each process.
times* processTimesArray ;



// A struct to represent a process
typedef struct _process {
	int id;
	int priority;
	int time;
} process;


// A struct to represent a hashqueue
typedef struct _element {
    process* array;
    int arrayIndex;
    int first;
}element;


// function to create a queue
// of given capacity.
// It initializes size of queue as 0
element* createTable(unsigned capacity)
{
	element* queue = (element*)malloc(SIZE * sizeof(element));
    for(int i = 0 ; i < SIZE ; i++)
	{
		queue[i].array = (process*) malloc (100* sizeof(process));
		queue[i].arrayIndex = 0;
		queue[i].first = 0;
	}
	
    return queue;
}

//hash function that will return the index in which the process will be placed in the queue
int hash(int p){
	int h = p % SIZE;
	return h ;	
}


// Function to add an item to the queue.
// It changes rear index of the queue
void insert(element* queue, process p)
{
	int index = hash(p.priority);
	queue[index].array[queue[index].arrayIndex].id = p.id;
	queue[index].array[queue[index].arrayIndex].priority = p.priority;
	queue[index].array[queue[index].arrayIndex].time = p.time;
	queue[index].arrayIndex++;
	processTimesArray[p.id].execution = p.time;
}

//this function is responsible of getting the process out of the hashqueue and return it. and then make the appropriate changes to the variables.
process get(element* queue){
	
	process p;
	process* tempp;
	clock_t start = clock();
	for(int i = 0; i < SIZE ; i++){

		if(queue[i].arrayIndex != 0){
			p = queue[i].array[0];
			queue[i].array++;
			queue[i].arrayIndex--;
			
			processTimesArray[p.id].deletion = clock();
    		processTimesArray[p.id].odeletion = clock() - start;
	
			return p;
		}
	}
}

//import the processes and its information from file and insert it in the given queue
void importprocesses(element* queue , char* filename){

	FILE *f ;
    int input;
    f = fopen(filename , "r");
    clock_t start;

	while(!feof(f)){

		start= clock();

		process p;
		fscanf(f,"%d",&input);
	  	p.id = input;
	  	fscanf(f,"%d",&input);
	  	p.priority = input;
	  	fscanf(f,"%d",&input);
		p.time = input;
		processTimesArray[p.id].insertion= clock();
		insert(queue , p);
		processTimesArray[p.id].oinsertion= clock() - start;
			
	  }
}

//this function is responsible of calculating the times of each process by adding each time needed like insertion deletion and executing. it is called after all the processees have been deleted from the queue after being executed
void processingTimes(){
	
	double waiting = 0 , turnaround = 0 , insertion = 0 , deletion;

	for(int timeCounter = 0 ; timeCounter < COUNT ; timeCounter++){

			waiting += (processTimesArray[timeCounter].deletion - processTimesArray[timeCounter].insertion);

			turnaround += (processTimesArray[timeCounter].deletion - processTimesArray[timeCounter].insertion) + (processTimesArray[timeCounter].execution);

			insertion += processTimesArray[timeCounter].oinsertion;

			deletion += processTimesArray[timeCounter].odeletion;
		}

		printf("\nAvarage Waiting: %lf ms \nAvarage Turnaround: %lf ms \nAvarage Insertion: %lf ms \nAvarage Deletion: %lf ms \n" , (waiting/COUNT) , (turnaround/COUNT) , (insertion/COUNT) , (deletion/COUNT) );
}

//this functin simulate the execution by making the program sleep for a time given for each process. the program will print that it finished executing successfully when it s done sleeping. 
void executep(process p){
	Sleep(p.time);
	printf("\n%d is exectuted successfully\n\n" , p.id);
}


//this function will print the waiting list of the processes that are waiting for its turns to execute.
void waitinglist(element* queue , int prio){

	int counter;
	printf("the waiting list is: \n");
	for(counter = 0  ; counter < queue[prio].arrayIndex ; counter++ )	
			printf("--> %d " , queue[prio].array[counter].id );
}



//this function will start after loading all the information about the processes. it will call the get function which will get the appropriate process from the queue and then call the execute function on it.
void executeNAll(element* queue){
	
	process p;
	for(int i = 0 ; i < COUNT ; i++){
		p = get(queue);
		//executep(p); //this function can be ignored to skip the time of sleeping for each process
	}
}

//this function is similar to the one above but this will print the waiting list after getting each process to see the updated waiting list.
void executeAll(element* queue){
	
	process p;
	for(int i = 0 ; i < COUNT ; i++){
		p = get(queue);
		waitinglist(queue , p.priority);
		executep(p);
	}
}



// Driver program to test above functions./
int main()
{
	int timeCounter;
	char choice = 'Y';
	int ch;
	element* queue = createTable(SIZE);
	processTimesArray= (times*)malloc(COUNT * sizeof(times));
	process p;
	
	printf("\n***************************************************************************************\n");
	printf("\n***************************************************************************************\n");
	printf("\n	WELCOME TO PROCESSES QUEUE IMPLEMENTED WITH THE HASHTABLE STRUCTURE		   \n");
	printf("\n***************************************************************************************\n");
	printf("\n***************************************************************************************\n");
	Sleep(1000);
	
	printf("\n\n-->We are going to explain the steps here...");
	
	for(int u = 3 ; u >= 0 ; u--){
		printf("%d",u);
		Sleep(1000);
	}
	
	printf("\n\n-->First, we are going to import the processes from the file...\n\n");
	Sleep(1000);
	
	printf("\n\n-->Do you want to do that? Y/N... (Default value: Y)");
	scanf("%c", &choice);
	
		if(choice == 'N' || choice == 'n') {
				printf("\nOkey then, Goodbye...");
				exit(0);
			}
			else{
				
				Sleep(1000);
				importprocesses(queue , "Proccesses.txt"); 
				printf("\n-->All the processes are inserted.\n");
				printf("\n\n-->After loading, there are two options.\n");
				
				
				printf("-->You can start to simulate the execution of all the processes( choice: 1)\n or you can do it quickly and proceed to the list of times we want to indicate ( choice : 2).\n ");
				
				while(ch != 1 && ch != 2){
					
					printf("--> Choose 1 or 2: ");
					scanf("%d", &ch);	
				}
				
					if(ch == 1){
						executeAll(queue);
					}			
					else{
						executeNAll(queue);
						printf("\n-->The processes have been executed.\n");
					} 
			}
	Sleep(500);
	printf("\n\n-->Now, we calculated a list of times that we want to see.\n");
	Sleep(1000);
	processingTimes();
	printf("\nThank you good bye...");
	return 0; 
}
