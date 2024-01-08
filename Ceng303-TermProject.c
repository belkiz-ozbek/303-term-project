#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLASSES 100
#define MAX_CLASSROOMS 50

/// her student ve prof için availablity-time structure(?) lazım
/// 
typedef struct {
    int student_id;
    char prof_name[20];
    char course_id[20];
    int exam_duration;
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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getBlockedHours(BlockedHours blockedHours[], int numOfBlockedHours) {
    int i;
    for (i = 0; i < numOfBlockedHours; i++) {
        printf("Please enter the %dth blocked hour's day: Monday, Tuesday, Wednesday, Thursday, Friday, Saturday: ", i + 1);
        scanf("%9s", blockedHours[i].day);
        clearInputBuffer();

        printf("Please enter the %dth blocked hour's starting time (Please enter the hours without comma, 09.00 as 0900): ", i + 1);
        scanf("%d", &blockedHours[i].startingTime);
        clearInputBuffer();

        printf("Please enter the %dth blocked hour's ending time (Please enter the hours without comma, 09.00 as 0900): ", i + 1);
        scanf("%d", &blockedHours[i].endingTime);
        clearInputBuffer();
    }
}

int compare(const Classrooms *A, const Classrooms *B) {
    int difference = B->capacity - A->capacity;

    if (difference == 0) {
        for (int i = 0; i < sizeof(A->room_id) - 1; i++) {
            if (A->room_id[i] < B->room_id[i]) {
                return 1;
            }
            if (A->room_id[i] > B->room_id[i]) {
                return -1;
            }
        }
    }

    return difference;
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
    for (int i = (N / 2) - 1; i >= 0; i--) {
        Heap(ar, N, i);
    }
    for (int i = N - 1; i >= 0; i--) {
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
	
	for(int j=low; j<=high-1; j++){
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

void setExams(Classes ar[], int index){ //index=0,1,2,3,...,MAX_CLASSES-1
	
	if(index==MAX_CLASSES){ //end of the array
		return;
	}
	
	if(index==0){
		quickSort(ar, 0, MAX_CLASSES-1);
		//check BlockedHours then prof_availablity then student...
		setExams(ar, index+1);
	}else{// index=1,2,3,4,...
		if(strcmp(ar[index].course_id, ar[index-1].course_id)!=0 && strcmp(ar[index].prof_name, ar[index-1].prof_name)!=0){ //index inci array elementin sınıf adı & prof adı kendinden öncekine eşit değilse (öncekinden farklı dersteyiz)
			//check BlockedHours then prof_availablity then student...
		
		}else{// eğer index inci element, 1 önceki elementle aynı sınıfa sahipse -> skip this index
			setExams(ar, size, index+1);
		}
	}
	
	//lazım olacak 
	//int duration = minutesToHours(ar, index);
	return;
}

int main() {
    Classes classes[MAX_CLASSES];
    Classrooms classrooms[MAX_CLASSROOMS];
    BlockedHours blockedHours[MAX_CLASSES];

    int numOfClasses = 0;
    int numOfClassrooms = 0;

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

    // Sorting classrooms
    sort(classrooms, numOfClassrooms);

    int numOfBlockedHours;
    printf("How many blocked hours are there? (Blocked hours on different days are counted separately, and if there is an exam between two blocked hours on the same day, they are also counted separately.)\n");
    scanf("%d", &numOfBlockedHours);
    clearInputBuffer();

    // Getting blocked hours
    getBlockedHours(blockedHours, numOfBlockedHours);

    // Displaying blocked hours
    int i;
    for (i = 0; i < numOfBlockedHours; i++) {
        printf("Blocked Hours %s %d %d\n", blockedHours[i].day, blockedHours[i].startingTime, blockedHours[i].endingTime);
    }

	//////////////////*********************//////////////
	//sort according to exam duration -> course id -> student id
	
	//setExams(classes, 0);
	
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
