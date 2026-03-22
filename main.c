#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define MAX_RECORDS 100
#define FILENAME "people.txt"

typedef struct {
    char name[MAX_NAME];
    int birth_year;
    char gender;
    float height;
} Person;

void createFile();
int readFile(Person people[]);
void printPeople(Person people[], int count);
void sortPeople(Person people[], int count, char* sort_fields);

int main() {
    Person people[MAX_RECORDS];
    int count;
    char sort_fields[100];
    
    printf("Programm for work with data about people\n");
    printf("========================================\n\n");
    
    createFile();
    count = readFile(people);
    if (count == 0) {
        printf("No data to process!\n");
        return 1;
    }
    
    printf("Initial data:\n");
    printf("-------------\n");
    printPeople(people, count);
    
    printf("\nEnter fields to sort by, separated by spaces\n");
    printf("Available fields: name, birth, gender, height\n");
    printf("Example: birth name (first by year, then by name)\n");
    printf("Enter: ");
    
    fgets(sort_fields, sizeof(sort_fields), stdin);
    
    sort_fields[strcspn(sort_fields, "\n")] = 0;
    sortPeople(people, count, sort_fields);
    
    printf("\nSorted data:\n");
    printf("--------------\n");
    printPeople(people, count);
    return 0;
}

void createFile() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error creating file!\n");
        return;
    }
    
    fprintf(file, "Ivanov Ivan 1990 M 1.85\n");
    fprintf(file, "Petrova Anna 1995 F 1.68\n");
    fprintf(file, "Sidorov Petr 1988 M 1.92\n");
    fprintf(file, "Kozlova Elena 1990 F 1.75\n");
    fprintf(file, "Smirnov Alexander 1985 M 1.78\n");
    fprintf(file, "Vasilieva Olga 1992 F 1.70\n");
    fprintf(file, "Morozov Dmitry 1988 M 1.88\n");
    fprintf(file, "Volkova Tatyana 1995 F 1.65\n");
    fprintf(file, "Pavlov Andrey 1987 M 1.83\n");
    fprintf(file, "Nikolaeva Svetlana 1990 F 1.72\n");
    
    fclose(file);
    printf("File %s created successfully.\n", FILENAME);
}

int readFile(Person people[]) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Error open file!\n");
        return 0;
    }
    
    int count = 0;
    char line[200];
    
    while (fgets(line, sizeof(line), file) && count < MAX_RECORDS) {
        char first_name[MAX_NAME];
        char last_name[MAX_NAME];
        int year;
        char gender;
        float height;
        
        if (sscanf(line, "%s %s %d %c %f", 
                   last_name,
                   first_name,
                   &year,
                   &gender,
                   &height) == 5) {
            
            strcpy(people[count].name, last_name);
            strcat(people[count].name, " ");
            strcat(people[count].name, first_name);
            
            people[count].birth_year = year;
            people[count].gender = gender;
            people[count].height = height;
            
            count++;
        }
    }
    
    fclose(file);
    return count;
}

void printPeople(Person people[], int count) {
    printf("%-20s %-12s %-8s %-10s\n", "Name/Surname", "Birth Year", "Gender", "Height(M)");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s %-12d %-8c %-10.2f\n",
               people[i].name,
               people[i].birth_year,
               people[i].gender,
               people[i].height);
    }
}

void sortPeople(Person people[], int count, char* sort_fields) {
    char fields_copy[100];
    strcpy(fields_copy, sort_fields);
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            char temp_fields[100];
            strcpy(temp_fields, fields_copy);
            int need_swap = 0;
            char* field = strtok(temp_fields, " ");
            
            while (field != NULL) {
                while (isspace(*field)) field++;
                int comp_result = 0;
                
                if (strcmp(field, "name") == 0) {
                    comp_result = strcmp(people[j].name, people[j+1].name);
                }
                else if (strcmp(field, "birth") == 0) {
                    comp_result = people[j].birth_year - people[j+1].birth_year;
                }
                else if (strcmp(field, "gender") == 0) {
                    comp_result = people[j].gender - people[j+1].gender;
                }
                else if (strcmp(field, "height") == 0) {
                    if (people[j].height < people[j+1].height)
                        comp_result = -1;
                    else if (people[j].height > people[j+1].height)
                        comp_result = 1;
                    else 
                        comp_result = 0;
                }
                
                if (comp_result < 0) {
                    break;
                }
                else if (comp_result > 0) {
                    need_swap = 1;
                    break;
                }
                field = strtok(NULL, " ");
            }
            
            if (need_swap) {
                Person temp = people[j];
                people[j] = people[j+1];
                people[j+1] = temp;
            }
        }
    }
    printf("Sorting is performed by fields: %s\n", sort_fields);
}
