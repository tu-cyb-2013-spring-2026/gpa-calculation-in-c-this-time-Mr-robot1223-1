#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char semester[10];
    int year;
    int hours;
    double quality_points;
} Semester;

double gradeToPoints(char grade) {
    switch (toupper(grade)) {
        case 'A': return 4.0;
        case 'B': return 3.0;
        case 'C': return 2.0;
        case 'D': return 1.0;
        case 'F': return 0.0;
        default:  return -1.0;
    }
}

void toUpper(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int semOrder(char *sem) {
    if (strcmp(sem, "FALL")   == 0) return 3;
    if (strcmp(sem, "SUMMER") == 0) return 2;
    if (strcmp(sem, "SPRING") == 0) return 1;
    return 0;
}

int findOrCreate(Semester sems[], int *count, char sem[], int year) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(sems[i].semester, sem) == 0 && sems[i].year == year) {
            return i;
        }
    }

    if (*count >= MAX) {
        printf("Maximum number of semesters (%d) reached.\n", MAX);
        return -1;
    }

    strcpy(sems[*count].semester, sem);
    sems[*count].year           = year;
    sems[*count].hours          = 0;
    sems[*count].quality_points = 0.0;
    (*count)++;
    return (*count) - 1;
}

void sortSemesters(Semester sems[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (sems[i].year < sems[j].year ||
               (sems[i].year == sems[j].year &&
                semOrder(sems[i].semester) < semOrder(sems[j].semester))) {
                Semester temp = sems[i];
                sems[i] = sems[j];
                sems[j] = temp;
            }
        }
    }
}

int main() {
    Semester sems[MAX];
    int count = 0;
    char line[256];

    printf("----> To quit entering grades, enter a blank line.\n");

    while (1) {
        printf("Enter a letter grade and hours for a course with its year and semester: ");

        if (!fgets(line, sizeof(line), stdin)) break;
        if (strcmp(line, "\n") == 0) break;

        char grade;
        char sem[20];
        int  hours, year;

        if (sscanf(line, " %c %d %19s %d", &grade, &hours, sem, &year) != 4) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        double points = gradeToPoints(grade);
        if (points < 0) {
            printf("Invalid grade. Please use A, B, C, D, or F.\n");
            continue;
        }

        if (hours <= 0) {
            printf("Invalid hours. Must be a positive number.\n");
            continue;
        }

        if (year < 1900 || year > 2100) {
            printf("Invalid year. Must be between 1900 and 2100.\n");
            continue;
        }

        toUpper(sem);

        if (strcmp(sem, "FALL")   != 0 &&
            strcmp(sem, "SPRING") != 0 &&
            strcmp(sem, "SUMMER") != 0) {
            printf("Invalid semester. Please use FALL, SPRING, or SUMMER.\n");
            continue;
        }

        int idx = findOrCreate(sems, &count, sem, year);
        if (idx == -1) break;

        sems[idx].hours          += hours;
        sems[idx].quality_points += points * hours;
    }

    sortSemesters(sems, count);

    printf("-------------------------------------\n");
    printf("Unofficial Transcript\n");

    int    total_hours   = 0;
    double total_quality = 0.0;

    for (int i = 0; i < count; i++) {
        if (sems[i].hours > 0) {
            double gpa = sems[i].quality_points / sems[i].hours;
            printf("%-6s %4d (%d hours) %.3f\n",
                   sems[i].semester,
                   sems[i].year,
                   sems[i].hours,
                   gpa);
            total_hours   += sems[i].hours;
            total_quality += sems[i].quality_points;
        }
    }

    printf("\n--\n");
    printf("Total credit hours: %d\n", total_hours);

    if (total_hours > 0) {
        printf("Overall GPA: %.3f\n", total_quality / total_hours);
    } else {
        printf("Overall GPA: 0.000\n");
    }

    printf("-------------------------------------\n");
    return 0;
}