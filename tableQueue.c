// C program for array implementation of priority queue 
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <time.h>
#include <windows.h>

#define SIZE 100000

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



// A structure to represent a process
typedef struct {
	int id;
	int priority;
	int time;
} process;



// A structure to represent a queue 
struct Queue { 
    int front, size; 
    unsigned capacity; 
    process* array; 
}; 


// function to create a queue 
// of given capacity. 
// It initializes size of queue as 0 
struct Queue* createQueue(unsigned capacity) 
{ 
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;    
    queue->array = (process*)malloc(queue->capacity * sizeof(process));
	 
    return queue; 
}


//puts the process in the place after shifting all the processes after it
void shiftAndPut(struct Queue* queue , process p , int dest ){
	
	int counter;
	for(counter = (queue->size) ; counter > dest  ; counter-- ){
		queue->array[counter] = queue->array[counter-1]; 
		
	}
	queue->array[dest] = p;
}
  

// Function to add an item to the queue. 
// It changes rear and size 
void enqueue(struct Queue* queue, process p) 
{ 
	int caseone = 0, counter;
    if (isFull(queue)) 
        return; 
        
	for(counter = 0 ; counter < queue->size; counter++){
		if(p.priority < queue->array[counter].priority){
			caseone = 1;
			shiftAndPut(queue , p , counter);
			break;
		}
	}
	 if(caseone == 0) shiftAndPut(queue , p , queue-> size);
	 queue->size = queue->size + 1;		
} 


//import the processes and its information from file and put insertionthe given queue
void importprocesses(struct Queue* queue , char* filename){

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
		
		enqueue(queue , p);
		//printf(" %d enqueued \n",p.id);
		
		processTimesArray[p.id].oinsertion= clock() - start;
		
	  }
}
 
// Queue is full when size becomes 
// equal to the capacity 
int isFull(struct Queue* queue) 
{ 
    return (queue->size == queue->capacity); 
} 
 
 
// Queue is empty when size is 0 
int isEmpty(struct Queue* queue) 
{ 
    return (queue->size == 0); 
}


void waitinglist(struct Queue* queue , int prio){
	
	int counter;
	printf("\nthe waiting list of priority %d is: \n", prio);
	for(counter = queue->front+1 ; queue->array[counter].priority == prio ; counter++ )
		printf("%d ",queue->array[counter].id);
}
 
 
// Function to remove an item from queue. 
// It changes front and size 
process dequeue(struct Queue* queue) 
{ 
	clock_t start = clock();
    if (isEmpty(queue)) return; 
    process* temp;
	    
    process p = queue->array[queue->front];
	temp =  queue->array + queue->front;
    queue->front = (queue->front + 1) % queue->capacity; 
    queue->size = queue->size - 1;
    
    processTimesArray[p.id].deletion = clock();
    processTimesArray[p.id].odeletion = clock() - start;
	
	return p; 
} 
  
// Function to get front of queue 
int front(struct Queue* queue) 
{ 
    if (isEmpty(queue)) 
        return -1; 
    return queue->array[queue->front].id; 
} 

void executep(process p){
	Sleep(p.time); 
	printf("\n%d is exectuted successfully\n\n" , p.id);
}


void executeAll(struct Queue* queue){
	
	process p;
	while(!isEmpty(queue)){
		
		p = dequeue(queue);
		executep(p);
		processTimesArray[p.id].execution = p.time;
		waitinglist(queue , p.priority);
	}
	
}



void executeNAll(struct Queue* queue){
	
	process p;
	while(!isEmpty(queue)){
		
		p = dequeue(queue);
		//executep(p);
		processTimesArray[p.id].execution = p.time;
		//waitinglist(queue , p.priority);
	}
	
}


//this function is responsible of calculating the times of each process by adding each time needed like insertion deletion and executing.
// it is called after all the processees have been deleted from the queue after being executed.
void processingTimes(){
	
	int timeCounter;
	double waiting = 0 , turnaround = 0 , insertion = 0 , deletion = 0;
	
	for( timeCounter = 0 ; timeCounter < SIZE ; timeCounter++){
			
			waiting += (processTimesArray[timeCounter].deletion - processTimesArray[timeCounter].insertion);
			
			turnaround += (processTimesArray[timeCounter].deletion - processTimesArray[timeCounter].insertion) + (processTimesArray[timeCounter].execution);
		
			insertion += processTimesArray[timeCounter].oinsertion;
			
			deletion += processTimesArray[timeCounter].odeletion;
		}
		printf("Avarage Waiting: %lf ms \nAvarage Turnaround: %lf ms \nAvarage Insertion: %lf ms \nAvarage Deletion: %lf ms \n" , (waiting/SIZE) , (turnaround/SIZE) , (insertion/SIZE) , (deletion/SIZE) );
}


// Driver program to test above functions./ 
int main() 
{ 
	int u;
	char choice = 'Y';
	int ch = 1;
    struct Queue* queue = createQueue(SIZE); 
  	processTimesArray= (times*)malloc(SIZE * sizeof(times));
  	
	
	printf("\n***************************************************************************************\n");
	printf("\n***************************************************************************************\n");
	printf("\n	WELCOME TO PROCESSES QUEUE IMPLEMENTED WITH THE PRIORITY QUEUE STRUCTURE		   \n");
	printf("\n***************************************************************************************\n");
	printf("\n***************************************************************************************\n");
	Sleep(1000);
	
	printf("\n\n-->We are going to explain the steps here...");
	
	for(u = 3 ; u >= 0 ; u--){
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
				
				printf("\n\n-->After loading, there are two options.\n");
				
				
				printf("-->You can start to simulate the execution of all the processes( choice: 1)\n or you can do it quickly and proceed to the list of times we want to indicate ( choice : 2).\n ");
				
				printf("--> Choose 1 or 2: ");
				scanf("%d", &ch);
				while(ch != 1 && ch != 2){
			
					printf("--> Choose 1 or 2: ");
					scanf("%d", &ch);
				}
				
					if(ch == 1){
						printf("-->Importing...");
						importprocesses(queue , "Proccesses.txt"); 
						printf("\n-->All the processes are inserted.\n");
						executeAll(queue);
					}			
					else{
						printf("-->Importing...");
						importprocesses(queue , "Proccesses.txt"); 
						printf("\n-->All the processes are inserted.\n");
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
