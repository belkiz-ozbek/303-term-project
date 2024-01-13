#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLASSES 100
#define MAX_CLASSROOMS 50
#define MAX_BLOCKED_HOURS 30

/// her student ve prof için availablity-time structure(?) lazım
/// 

typedef struct {
    char prof_name[20];
    int is_available; // 1 if available, 0 if not
} ProfessorAvailability;

typedef struct {
    int student_id;
    int is_available; // 1 if available, 0 if not
} StudentAvailability;

typedef struct {
    int student_id;
    char prof_name[20];
    char course_id[20];
    int exam_duration;
    int assigned; // 1 if assigned, 0 if not	
} Classes; //lecture

typedef struct {
    char room_id[10];
    int capacity;
} Classrooms;

typedef struct {
    char day[9];
    int startingTime;
    int endingTime;
} BlockedHours;

void getFiles(char arr[], const char *forWhat) {
    printf("Please enter the file name for %s: ", forWhat);
    scanf("%s", arr);
    printf("\n");
}

void readingToClassesStruct(FILE *classesFile, Classes classes[], int *numOfClasses) {
    if (classesFile == NULL) {
        perror("Error opening classes file");
        exit(EXIT_FAILURE);
    }

    int classesCount = 0;
    char line[MAX_CLASSES];
    while (fgets(line, sizeof(line), classesFile) != NULL && classesCount < MAX_CLASSES) {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Reading from the file to classes struct
        sscanf(line, "%d,%19[^,],%19[^,],%d",
               &classes[classesCount].student_id, classes[classesCount].prof_name,
               classes[classesCount].course_id, &classes[classesCount].exam_duration);

        classesCount++;
    }
    *numOfClasses = classesCount;
    fclose(classesFile);
}

void readingToClassroomsStruct(FILE *classroomsFile, Classrooms classrooms[], int *numOfClassrooms) {
    if (classroomsFile == NULL) {
        perror("Error opening classrooms file");
        exit(EXIT_FAILURE);
    }

    int classroomsCount = 0;
    char line[MAX_CLASSROOMS];
    while (fgets(line, sizeof(line), classroomsFile) != NULL && classroomsCount < MAX_CLASSROOMS) {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Reading from the file to struct
        sscanf(line, "%9[^,],%d", classrooms[classroomsCount].room_id, &classrooms[classroomsCount].capacity);

        classroomsCount++;
    }
    *numOfClassrooms = classroomsCount;
    fclose(classroomsFile);
}

void readingToBlockedHoursStruct(FILE *blockedHoursFile, BlockedHours blockedHoursArray[], int *numOfBlockedHours) {
    if (blockedHoursFile == NULL) {
        perror("Error opening blocked hours file");
        exit(EXIT_FAILURE);
    }

    int blockedHoursCount = 0;
    char line[MAX_BLOCKED_HOURS];
    
    while (fgets(line, sizeof(line), blockedHoursFile) != NULL && blockedHoursCount < MAX_BLOCKED_HOURS) {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';

        // Check if the line is empty
        if (line[0] == '\0') {
            // Skip empty line
            continue;
        }

        // Reading from the file to struct
        sscanf(line, "%9[^,],%d,%d", blockedHoursArray[blockedHoursCount].day, &blockedHoursArray[blockedHoursCount].startingTime, &blockedHoursArray[blockedHoursCount].endingTime);

        blockedHoursCount++;
    }
	printf("%d",blockedHoursCount);
    *numOfBlockedHours = blockedHoursCount;
    fclose(blockedHoursFile);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int compare(const Classrooms *a, const Classrooms *b) {
    return ((Classrooms *)b)->capacity - ((Classrooms *)a)->capacity;
}

void Heap(Classrooms ar[], int N, int i) {
    int big = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < N && compare(&ar[left], &ar[big]) < 0) {
        big = left;
    }

    if (right < N && compare(&ar[right], &ar[big]) < 0) {
        big = right;
    }

    if (big != i) {
        Classrooms temp = ar[i];
        ar[i] = ar[big];
        ar[big] = temp;

        Heap(ar, N, big);
    }
}

void sort(Classrooms ar[], int N) {
	int i;
    for (i = (N / 2) - 1; i >= 0; i--) {
        Heap(ar, N, i);
    }
    for (i = N - 1; i >= 0; i--) {
        Classrooms temp = ar[0];
        ar[0] = ar[i];
        ar[i] = temp;

        Heap(ar, i, 0);
    }
}

/// Sorting Classes according to exam_duration -> course_id -> student_id
int partition(Classes ar[], int low, int high){
	Classes pivot= ar[high];
	int i = low-1;
	int j;
	for(j=low; j<=high-1; j++){
		if(ar[j].exam_duration <= pivot.exam_duration){
			if(ar[j].exam_duration == pivot.exam_duration){ //course_id comparison
				if(strcmp(ar[j].course_id, pivot.course_id)>=0){
					if(strcmp(ar[j].course_id, pivot.course_id)==0){
						if(ar[j].student_id < pivot.student_id){
							i++;
							Classes temp= ar[i];
							ar[i]=ar[j];
							ar[j]=temp;
							//swap
						}
					}
				}else{
					i++;
					Classes temp= ar[i];
					ar[i]=ar[j];
					ar[j]=temp;
					//swap
				}
			}
		}else{
			i++;
			Classes temp= ar[i];
			ar[i]=ar[j];
			ar[j]=temp;
			//swap
		}
	}
	
	Classes temp= ar[i+1];
	ar[i+1]=ar[high];
	ar[high]=temp;
	//swap
	
	return i+1;
}

void quickSort(Classes ar[], int low, int high){
	if(low<high){
		int pivot= partition(ar, low, high);
		quickSort(ar, low, pivot-1);
		quickSort(ar, pivot+1, high);
	}
}

//////

int minutesToHours(Classes ar[], int i){
	int hour= ar[i].exam_duration / 60;
	int min= ar[i].exam_duration %60;
	
	return min+(hour*100);
}

void initializeAvailability(StudentAvailability studentAvailabilities[], ProfessorAvailability profAvailabilities[], int numOfStudents, int numOfProfs) {
    // Initialize student availabilities
    for (int i = 0; i < numOfStudents; i++) {
        studentAvailabilities[i].student_id = i + 1; // Assuming student IDs start from 1
        studentAvailabilities[i].is_available = 1; // Initially, all students are available
    }

    // Initialize professor availabilities
    for (int i = 0; i < numOfProfs; i++) {
        strcpy(profAvailabilities[i].prof_name, ""); // Initialize with an empty string
        profAvailabilities[i].is_available = 1; // Initially, all professors are available
    }
}

int isStudentAvailable(StudentAvailability studentAvailabilities[], int student_id) {
    return studentAvailabilities[student_id - 1].is_available;
}

void setStudentUnavailable(StudentAvailability studentAvailabilities[], int student_id) {
    studentAvailabilities[student_id - 1].is_available = 0;
}

int isProfAvailable(ProfessorAvailability profAvailabilities[], const char *prof_name) {
    for (int i = 0; i < MAX_CLASSES; i++) {
        if (strcmp(profAvailabilities[i].prof_name, prof_name) == 0 && profAvailabilities[i].is_available) {
            return 1; // Prof is available
        }
    }
    return 0; // Prof is not available
}

void setProfUnavailable(ProfessorAvailability profAvailabilities[], const char *prof_name) {
    for (int i = 0; i < MAX_CLASSES; i++) {
        if (strcmp(profAvailabilities[i].prof_name, prof_name) == 0) {
            profAvailabilities[i].is_available = 0; // Mark prof as unavailable
            break;
        }
    }
}


void setExams(Classes ar[], int index, StudentAvailability studentAvailabilities[], ProfessorAvailability profAvailabilities[], int numOfClasses) {
    if (index == numOfClasses) {
        return; // End of the array
    }

    if (index == 0) {
        quickSort(ar, 0, numOfClasses - 1);
        // Check BlockedHours, Professors, and Students availability
        setExams(ar, index + 1, studentAvailabilities, profAvailabilities, numOfClasses);
    } else {
        if (strcmp(ar[index].course_id, ar[index - 1].course_id) != 0 || strcmp(ar[index].prof_name, ar[index - 1].prof_name) != 0) {
            // Check BlockedHours, Professors, and Students availability
            setExams(ar, index + 1, studentAvailabilities, profAvailabilities, numOfClasses);
        } else {
            // If index-th element is for the same class and professor as the previous one, check availability
            if (isStudentAvailable(studentAvailabilities, ar[index].student_id) && isProfAvailable(profAvailabilities, ar[index].prof_name)) {
                // Assign exam and mark student and professor as unavailable for this time slot
                printf("Assigning exam for Student ID: %d, Prof Name: %s, Course ID: %s, Exam Duration: %d\n",
                       ar[index].student_id, ar[index].prof_name, ar[index].course_id, ar[index].exam_duration);
                setStudentUnavailable(studentAvailabilities, ar[index].student_id);
                setProfUnavailable(profAvailabilities, ar[index].prof_name);
            }
            // Check BlockedHours, Professors, and Students availability
            setExams(ar, index + 1, studentAvailabilities, profAvailabilities, numOfClasses);
        }
    }
}

int main() {
    Classes classes[MAX_CLASSES];
    Classrooms classrooms[MAX_CLASSROOMS];
    BlockedHours blockedHours[MAX_BLOCKED_HOURS];
    StudentAvailability studentAvailabilities[MAX_CLASSES];
    ProfessorAvailability profAvailabilities[MAX_CLASSES];

    int numOfClasses = 0;
    int numOfClassrooms = 0;
    int numOfBlockedHours = 0;

    // Reading classes file
    char classesFileName[30];
    getFiles(classesFileName, "for classes");
    FILE *classesFile = fopen(classesFileName, "r");
    readingToClassesStruct(classesFile, classes, &numOfClasses);

    // Reading classrooms file
    char classroomsFileName[30];
    getFiles(classroomsFileName, "for classrooms");
    FILE *classroomsFile = fopen(classroomsFileName, "r");
    readingToClassroomsStruct(classroomsFile, classrooms, &numOfClassrooms);


    //Reading blocked hours file
    char blockedHoursFileName[30];
    getFiles(blockedHoursFileName, "for blocked hours");
    FILE *blockedHoursFile = fopen(blockedHoursFileName, "r");
    readingToBlockedHoursStruct(blockedHoursFile,blockedHours,&numOfBlockedHours);
    
    // Sorting classrooms
    sort(classrooms, numOfClassrooms);

    // Displaying blocked hours
    int i;
    for (i = 0; i < numOfBlockedHours; i++) {
        printf("Blocked Hours %s %d %d\n", blockedHours[i].day, blockedHours[i].startingTime, blockedHours[i].endingTime);
    }

	//////////////////*********************//////////////
	//sort according to exam duration -> course id -> student id
	
	 initializeAvailability(studentAvailabilities, profAvailabilities, MAX_CLASSES, MAX_CLASSES);
    setExams(classes, 0, studentAvailabilities, profAvailabilities, numOfClasses);

	
    // Displaying classes
    for (i = 0; i < numOfClasses; i++) {
        printf("student ID: %d, Profname: %s, class ID: %s, time: %d\n",
               classes[i].student_id, classes[i].prof_name, classes[i].course_id, classes[i].exam_duration);
    }

    // Displaying classrooms
    for (i = 0; i < numOfClassrooms; ++i) {
        printf("Room ID: %s, Capacity: %d\n", classrooms[i].room_id, classrooms[i].capacity);
    }

    fclose(classesFile);
    fclose(classroomsFile);

    return 0;
}
