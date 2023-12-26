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
    
    int i;
        for (i = 0; i < classesCount; i++) {
        printf("student ID: %d, Profname: %s, class ID: %s, time: %d\n",
               classes[i].student_id, classes[i].prof_name, classes[i].course_id, classes[i].exam_duration);
    }

        for (i = 0; i <classroomsCount; ++i) {
        printf("Room ID: %s, Capacity: %d\n", classrooms[i].room_id, classrooms[i].capacity);
    }
	return 0;
}
