#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_CLASSES 100
#define MAX_CLASSROOMS 50
#define MAX_BLOCKED_HOURS 30
#define START_TIME 540   // 9:00 AM in minutes (9 * 60)
#define END_TIME 1080    // 6:00 PM in minutes (18 * 60)
#define TIME_SLOT 30  

//ramdomness ortadan kaldırılabilirse sonuç bulma şansımız daha çok!!!!!
//dakika sasat dönüşümlerinden emin değilim dikkatlice incelnemsi gerek
//available methodlarını kullanarak checkSchedule yazmaya çalıştım olmadı ama yine de silmedim isterseniz siz de deneyebilirsiniz

const char *weekdays[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const int dayDuration = 540; // 9 hours in minutes
int numOfDays = 6; // Number of days in a week
int numOfClasses = 0;
int numOfClassrooms = 0;

typedef struct {
    int student_id;
    char prof_name[20];
    char course_id[20];
    int exam_duration;
    int assigned; // 1 if assigned, 0 if not
} Classes;

typedef struct {
    char room_id[10];
    int capacity;
} Classrooms;

typedef struct {
    char day[9];
    int startingTime;
    int endingTime;
} BlockedHours;

typedef struct {
    int courseIndex;
    int roomIndex;
    char day[9];
    int startingTime;
} Schedule;

void getRandomDay(char day[9]) {
    char days[6][9] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    int randomDayIndex = rand() % 6;
    strcpy(day, days[randomDayIndex]);
}
int getRandomStartingTime() {
    return START_TIME + (rand() % ((END_TIME - START_TIME) / TIME_SLOT)) * TIME_SLOT;
}
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
        line[strcspn(line, "\n")] = '\0';

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
        line[strcspn(line, "\n")] = '\0';

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
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0') {
            continue;
        }

        sscanf(line, "%9[^,],%d,%d", blockedHoursArray[blockedHoursCount].day,
               &blockedHoursArray[blockedHoursCount].startingTime, &blockedHoursArray[blockedHoursCount].endingTime);

        blockedHoursCount++;
    }

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



int isClassroomAvailable(Classrooms classroom, int day, int timeSlot, int examDuration, Schedule schedule[], int scheduleSize) {
    for (int i = 0; i < scheduleSize; i++) {
        if (strcmp(schedule[i].day, weekdays[day]) == 0 &&
            schedule[i].startingTime <= timeSlot && timeSlot <= schedule[i].startingTime + examDuration) {
            return 0;
        }
    }
    return 1;
}

int isStudentAvailable(int studentID, int day, int timeSlot, int examDuration, Schedule schedule[], int scheduleSize, Classes classes[]) {
    for (int i = 0; i < scheduleSize; i++) {
        if (strcmp(schedule[i].day, weekdays[day]) == 0 &&
            schedule[i].startingTime <= timeSlot + examDuration &&
            timeSlot <= schedule[i].startingTime + classes[schedule[i].courseIndex].exam_duration &&
            studentID == classes[schedule[i].courseIndex].student_id) {
            return 0;
        }
    }
    return 1;
}

int isProfAvailable(const char *profName, int day, int timeSlot, int examDuration, Schedule schedule[], int scheduleSize, Classes classes[]) {
    for (int i = 0; i < scheduleSize; i++) {
        if (strcmp(schedule[i].day, weekdays[day]) == 0 &&
            schedule[i].startingTime <= timeSlot && timeSlot <= schedule[i].startingTime + examDuration &&
            strcmp(profName, classes[schedule[i].courseIndex].prof_name) == 0) {
            return 0;
        }
    }
    return 1;
}

// Function we will  use  in the backtracking step
int checkSchedule(const Schedule *currentSchedule, const Classes *classes, const Classrooms *classrooms, const BlockedHours *blockedHours, int numScheduledExams) {
    for (int i = 0; i < numScheduledExams; i++) {
        // Checkking student availablity
        if (currentSchedule[i].courseIndex != -1 &&
            classes[currentSchedule[i].courseIndex].student_id == classes[numScheduledExams].student_id &&
            strcmp(classes[currentSchedule[i].courseIndex].course_id, classes[numScheduledExams].course_id) != 0) {
            return 0;  // there şs a conflict ---->Student has two exams at the same time
        }

        // Checking  professor availablity
        if (currentSchedule[i].courseIndex != -1 &&
            strcmp(classes[currentSchedule[i].courseIndex].prof_name, classes[numScheduledExams].prof_name) == 0) {
            return 0;  // there is a conflict--->Professor has two exams at the same time
             
        }
    }

    // Checking the room capacity 
    int roomCapacity = classrooms[currentSchedule[numScheduledExams].roomIndex].capacity;
    if (roomCapacity >= classes[numScheduledExams].exam_duration / 2) {
        return 0; 
    }

    // Checking the conflict of blocked hours
    for (int i = 0; i < numScheduledExams; i++) {
        if (strcmp(currentSchedule[i].day, blockedHours[numScheduledExams].day) == 0 &&
            currentSchedule[i].startingTime >= blockedHours[numScheduledExams].startingTime &&
            currentSchedule[i].startingTime < blockedHours[numScheduledExams].endingTime) {
            return 0; 
        }
    }

    return 1;  // a valid schedule is found!!!!
}

void printExamSchedule(const Schedule *schedule, const Classes *classes, const Classrooms *classrooms, int numScheduledExams) {
    printf("Exam Schedule:\n");
    printf("%-10s %-20s %-10s %-15s %-10s\n", "Day", "Starting Time", "Room", "Prof Name", "Course ID");

    for (int i = 0; i < numScheduledExams; i++) {
        int classIndex = schedule[i].courseIndex;
        int roomIndex = schedule[i].roomIndex;

        printf("%-10s %-20d %-10s %-15s %-10s\n",
               schedule[i].day,
               schedule[i].startingTime,
               classrooms[roomIndex].room_id,
               classes[classIndex].prof_name,
               classes[classIndex].course_id);
    }
}

void backtrackToFindSchedule(Schedule *currentSchedule, const Classes *classes, const Classrooms *classrooms, const BlockedHours *blockedHours, int numScheduledExams, int numCourses, int numRooms, int numBlockedHours) {
  if (numScheduledExams == numCourses) {
        printf("all exams are scheduled");
        return;
    }

    for (int j = 0; j < numCourses; j++) {
      
        if (currentSchedule[j].courseIndex == -1) {
            for (int i = 0; i < numRooms; i++) {
                currentSchedule[j].courseIndex = j;
                currentSchedule[j].roomIndex = i;

                if (checkSchedule(currentSchedule, classes, classrooms, blockedHours, numScheduledExams)) {
                    getRandomDay(currentSchedule[j].day);
                    currentSchedule[j].startingTime = getRandomStartingTime();

                    // recursive backtrack
                    backtrackToFindSchedule(currentSchedule, classes, classrooms, blockedHours, numScheduledExams + 1, numCourses, numRooms, numBlockedHours);

                    //remove assignment to backtrack
                    currentSchedule[j].courseIndex = -1;
                    currentSchedule[j].roomIndex = -1;
                    getRandomDay(currentSchedule[j].day);
                    currentSchedule[j].startingTime = getRandomStartingTime();
                } else {
                    //bu kısımdan emin değilim
                    currentSchedule[j].courseIndex = -1;
                    currentSchedule[j].roomIndex = -1;
                    getRandomDay(currentSchedule[j].day);
                    currentSchedule[j].startingTime = getRandomStartingTime();
                }
            }
        }
    }

}


int main() {
    Classes classes[MAX_CLASSES];
    Classrooms classrooms[MAX_CLASSROOMS];
    BlockedHours blockedHours[MAX_CLASSES];
    Schedule currentSchedule[MAX_CLASSES];

    int numOfClasses = 0;
    int numOfClassrooms = 0;
    int numOfBlockedHours = 0;
    int numScheduledExams=0;

    char classesFileName[30];
    getFiles(classesFileName, "for classes");
    FILE *classesFile = fopen(classesFileName, "r");
    readingToClassesStruct(classesFile, classes, &numOfClasses);

    char classroomsFileName[30];
    getFiles(classroomsFileName, "for classrooms");
    FILE *classroomsFile = fopen(classroomsFileName, "r");
    readingToClassroomsStruct(classroomsFile, classrooms, &numOfClassrooms);

    char blockedHoursFileName[30];
    getFiles(blockedHoursFileName, "for blocked hours");
    FILE *blockedHoursFile = fopen(blockedHoursFileName, "r");
    readingToBlockedHoursStruct(blockedHoursFile, blockedHours, &numOfBlockedHours);

    sort(classrooms, numOfClassrooms);
    
     for (int i = 0; i < MAX_CLASSES; i++) {
        currentSchedule[i].courseIndex = -1;
        currentSchedule[i].roomIndex = -1;
        getRandomDay(currentSchedule[i].day);
        currentSchedule[i].startingTime = getRandomStartingTime();
    }

    backtrackToFindSchedule(currentSchedule, classes, classrooms, blockedHours, 0, numOfClasses, numOfClassrooms, numOfBlockedHours);
    printExamSchedule(currentSchedule, classes, classrooms, numScheduledExams);
    fclose(classesFile);
    fclose(classroomsFile);
    return 0;
}

