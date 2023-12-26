#include <stdio.h>
#include <stdlib.h>
#define MAX_CLASSES 100 //Change this if the input is large
#define MAX_CLASSROOMS 50 //Change this if the input is large

typedef struct{
	int student_id;
	char prof_name[20];
	char course_id[20];
	int exam_duration;
}Classes;

typedef struct{
	char room_id[10];
	int capacity;
}Classrooms;

int compare(const Classrooms* A, const Classrooms* B){
	int difference= (B->capacity)-(A->capacity);
	
	if(difference==0){
		for(int i=0; i<sizeof(A->room_id)-1; i++){
			if(A->room_id[i] < B->room_id[i]){
				return -1;
			}
			if(A->room_id[i] > B->room_id[i]){
				return 1;
			}
		}
	}
	
	return difference;
}
void Heap(Classrooms ar[], int N, int i){
	
	int big=i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	
	if(left<N && compare(&ar[left],&ar[big]) < 0){
		big=left;
	}
	
	if(right<N && compare(&ar[right],&ar[big]) < 0){
		big=right;
	}
	
	if(big!=i){
		Classrooms temp= ar[i];
		ar[i]=ar[big];
		ar[big]=temp;
		
		Heap(ar, N, big);
	}
}

void sort(Classrooms ar[], int N){
	for(int i=(N/2)-1; i>=0; i--){
		Heap(ar,N,i);
	}
	for(int i=N-1; i>=0; i--){
		Classrooms temp=ar[0];
		ar[0]=ar[i];
		ar[i]=temp;
		
		Heap(ar, i, 0);
	}
}

int main(int argc, char *argv[]) {
	Classes classes[MAX_CLASSES];
	Classrooms classrooms[MAX_CLASSROOMS];
	
	//-----------------Classes csv---------------
	char classesFileName[]="classes.csv"; //write your classes filename here
	FILE *classesFile = fopen(classesFileName, "r");
    if (classesFile == NULL) {
        perror("Cannot find the file!");
        exit(EXIT_FAILURE);
    }


    int classesCount = 0;
    char line[MAX_CLASSES];
    while (fgets(line, sizeof(line), classesFile) != NULL) {
        // reading from the file to struct
		sscanf(line, "%d,%19[^,],%19[^,],%d", &classes[classesCount].student_id, classes[classesCount].prof_name, classes[classesCount].course_id, &classes[classesCount].exam_duration);

		classesCount++;
    }

    fclose(classesFile);
    //-----------------------Classrooms csv-------------
    char classroomsFileName[]="classrooms.csv"; //write your classrooms filename here
	FILE *classroomsFile = fopen(classroomsFileName, "r");
        if (classroomsFile == NULL) {
        perror("Cannot find the file!");
        exit(EXIT_FAILURE);
    }
    
    int classroomsCount = 0;
    char line2[MAX_CLASSROOMS];
    while (fgets(line2, sizeof(line2), classroomsFile) != NULL) {
        // reading from the file to struct
        sscanf(line2, "%9[^,],%d", classrooms[classroomsCount].room_id, &classrooms[classroomsCount].capacity);
        classroomsCount++;
    }
    fclose(classroomsFile);
    
    sort(classrooms, sizeof(classrooms)/sizeof(classrooms[0]));
    
        for (int i = 0; i < classesCount; i++) {
        printf("student ID: %d, Profname: %s, class ID: %s, time: %d\n",
               classes[i].student_id, classes[i].prof_name, classes[i].course_id, classes[i].exam_duration);
    }

        for (int i = 0; i <classroomsCount; ++i) {
        printf("Room ID: %s, Capacity: %d\n", classrooms[i].room_id, classrooms[i].capacity);
    }
    
	return 0;
}
