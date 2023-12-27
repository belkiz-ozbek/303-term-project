#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

typedef struct{
	char day[9];
	int startingTime;
	int endingTime;
}BlockedHours;

void getFiles(char arr[], const char *forWhat) {
    printf("Please enter the file name for %s: ", forWhat);
    scanf("%s", arr);
    printf("\n");
}

void readingToClassesStruct(FILE *classesFile, Classes classes[]){
	if (classesFile == NULL) {
        perror("Cannot find the file!");
        exit(EXIT_FAILURE); //if the file does not exist we quit
    }
    
    int classesCount = 0;
    char line[MAX_CLASSES];
    while (fgets(line, sizeof(line), classesFile) != NULL) {
        // reading from the file to classes struct
		sscanf(line, "%d,%19[^,],%19[^,],%d", &classes[classesCount].student_id, classes[classesCount].prof_name, classes[classesCount].course_id, &classes[classesCount].exam_duration);

		classesCount++;
    }
   fclose(classesFile);
    
}

void readingToClassroomsStruct(FILE *classroomsFile, Classrooms classrooms[]){
	
	    if (classroomsFile == NULL) {
        perror("Cannot find the file!");
        exit(EXIT_FAILURE); //if the file does not exist we quit
    }
    
    int classroomsCount = 0;
    char line2[MAX_CLASSROOMS];
    while (fgets(line2, sizeof(line2), classroomsFile) != NULL) {
        // reading from the file to struct
        sscanf(line2, "%9[^,],%d", classrooms[classroomsCount].room_id, &classrooms[classroomsCount].capacity);
        classroomsCount++;
    }
    fclose(classroomsFile);
    
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getBlockedHours(BlockedHours blockedHours[], int numOfBlockedHours) {
    int i;
    for (i = 0; i < numOfBlockedHours; i++) {
        printf("Please enter the %dth blocked hour's day: Monday, Tuesday, Wednesday, Thursday, Friday, Saturday: ", i + 1);

        // Use scanf with a format specifier that doesn't allow spaces
        scanf("%9s", blockedHours[i].day);

        // Clear the input buffer
        clearInputBuffer();

        printf("Please enter the %dth blocked hour's starting time (Please enter the hours without comma, 09.00 as 0900): ", i + 1);

        // Add a format specifier for startingTime
        scanf("%d", &blockedHours[i].startingTime);

        // Clear the input buffer
        clearInputBuffer();

        printf("Please enter the %dth blocked hour's ending time (Please enter the hours without comma, 09.00 as 0900): ", i + 1);

        // Add a format specifier for endingTime
        scanf("%d", &blockedHours[i].endingTime);

        // Clear the input buffer
        clearInputBuffer();
    }
}



int main(int argc, char *argv[]) {
	Classes classes[MAX_CLASSES];
	Classrooms classrooms[MAX_CLASSROOMS];
	
	//-----------------Classes csv---------------
	char classesFileName[30];
	getFiles(classesFileName,"for classes"); // we are getting the filename from user
	FILE *classesFile = fopen(classesFileName, "r");
    readingToClassesStruct(classesFile,classes);

    //-----------------------Classrooms csv-------------
    char classroomsFileName[30];
    getFiles(classroomsFileName,"for classrooms");// we are getting the filename from user
	FILE *classroomsFile = fopen(classroomsFileName, "r");
	readingToClassroomsStruct(classroomsFile,classrooms);
	
    int numOfBlockedHours;
    printf("How many blocked hours are there? (Blocked hours on different days are counted separately, and if there is an exam between two blocked hours on the same day, they are also counted separately.)\n");
    scanf("%d",&numOfBlockedHours);
    BlockedHours blockedHours[numOfBlockedHours];
    getBlockedHours(blockedHours,numOfBlockedHours);
    
  
    int i;
            for (i = 0; i < numOfBlockedHours; i++) {
        printf("Blocked Hours  %s  %d  %d\n",blockedHours[i].day, blockedHours[i].startingTime, blockedHours[i].endingTime);
    }
        for (i = 0; i < 100; i++) {
        printf("student ID: %d, Profname: %s, class ID: %s, time: %d\n",
               classes[i].student_id, classes[i].prof_name, classes[i].course_id, classes[i].exam_duration);
    }

        for (i = 0; i <50; ++i) {
        printf("Room ID: %s, Capacity: %d\n", classrooms[i].room_id, classrooms[i].capacity);
    }
	return 0;
}
