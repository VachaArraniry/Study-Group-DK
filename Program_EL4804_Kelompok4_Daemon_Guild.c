#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data_File
{
    char name[100], class[100], year[100], password[100];
    char professor[100], className[100], description[100], date[100];
    int nim;
};

int blacklistAmount = 0, classAmount = 0, professorAmount = 0;

FILE *fileStudentData = "studetData.bin";
FILE *fileRequest = "request.bin";
FILE *fileClass = "class.bin";
FILE *fileBlacklist = "blackList.bin";
FILE *filefile = "filefile.bin";

void main() {
    int inp_choice;

    while (1)
    {
        system("cls");
        printf("\nHello, are you a ... \n");
        printf("  1. Student\n");
        printf("  2. Professor\n");
        printf("  3. Admin\n");
        printf("Your selection \t : ");
        scanf("%d", &inp_choice);

        switch (inp_choice) {
        case 1:
            studentMenu();
            break;
        case 2:
            teacherMenu();
            break;
        case 3:
            adminMenu();
            break;
        default:
            printf("\t  Oops Error 404\n");
            printf("\tYour selected page\n\t  does not exist");
            printf("\n\n");
            printf("      Press any key to go back");
            system("pause");
        }
    }
}

void studentMenu() {
    int inp_choice;

    while (1)
    {
        system("cls");
        printf("\n=== Student Menu ===\n");
        printf("  1. Login\n");
        printf("  2. Registration\n");
        printf("  3. Back to Main Menu\n");
        printf("Your choice \t : ");
        scanf("%d", &inp_choice);

        switch (inp_choice) {
        case 1:
            studentLogin();
            break;
        case 2:
            studentRegistration();
            break;
        case 3:
            return;
        default:
            printf("Invalid choice! Try again.\n");
            system("pause");
        }
    }
}

void studentRegistration () {
    struct Data_File lrVar;

    printf("\n=== Student Registration ===\n");
    printf("Name \t\t : ");
    scanf(" %[^\n]s", lrVar.name);
    printf("Year \t\t : ");
    scanf(" %[^\n]s", lrVar.year);
    printf("NIM \t\t : ");
    scanf("%d", &lrVar.nim);
    printf("Password \t : ");
    scanf(" %[^\n]s", lrVar.password);

    saveStudentData(lrVar);
    printf("You can now login\n");
    system("pause");
}

void saveStudentData(struct Data_File lrVar) {
    FILE *fileData = fopen(fileStudentData, "ab");
    if (fileData == NULL)
    {
        printf("Error");
        return;
    }

    fwrite(&lrVar, sizeof(struct Data_File), 1, fileData);
    fclose(fileData);
}

void studentLogin() {
    struct Data_File lrVar;
    char name [100], password[100];
    int try = 3;

    while (try > 0)
    {
        system("cls");
        printf("\n=== Student Login ===\n");
        printf("Name \t\t : ");
        scanf(" %[^\n]s", name);
        printf("Password \t : ");
        scanf(" %[^\n]s", password);

        if (checkStudentData(name, password))
        {
            printf("Login Sucessful");
            system("pause");
            class();
            return;
        }
        else {
            printf("Error, try again");
            system("pause");
        }
    }
    printf("Too many failed attempts");
    system("Pause");
    
}

int checkStudentData(char *name, char *password) {
    FILE *fileData = fopen(fileStudentData, "rb");
    if (fileData == NULL)
    {
        printf("Error");
        return 0;
    }

    struct Data_File lrVar;
    while (fread(&lrVar, sizeof(struct Data_File), 1, fileData))
    {
        if (strcmp(lrVar.name, name) == 0 && strcmp(lrVar.password, password) == 0)
        {
            fclose(fileData);
            return 1;
        }
        
    }
    fclose(fileData);
    return 0;
}

void classRequest () {
    FILE *file = fopen(fileRequest, "ab");
    if (file == NULL)
    {
        printf("Error");
        return;
    }
    
    char className[100], date[100], description[100];
    printf("\n=== Request a New Class ===\n");
    printf("Class Name \t: ");
    scanf(" %[^\n]s", className);
    printf("Date \t\t: ");
    scanf(" %[^\n]s", date);
    printf("Description \t: ");
    scanf(" %[^\n]s", description);

    fprintf(file, "%s;%s;%s\n", className, date, description);
    fclose(file);
    printf("Class requested");
    system("pause");
}

void class () {
    int classID, choice;

    while (1)
    {
        system("cls");
        printf("\n=== Available Classes ===\n");
        professorClass();

        printf("\nEnter class ID to join, or 0 to return to the menu: ");
        scanf("%d", &classID);

        if (classID == 0)
        {
            return;
        }

        if (status(classID))
        {
            printf("\nYou are enrolled.\n");
            printf("Leave Class\n");
        }
        else {
            printf("You are not enrolled");
            printf("Your choice: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                if (status(classID))
                {
                    exitClass(classID);
                }
                else {
                    joinClass(classID);
                }
            }
            else if (choice == 2)
                {
                    continue;
                }
            else {
                printf("invalid\n");
            }
        }
    }
}

void professorClass () {
    FILE *file = fopen(fileClass, "rb");
    if (file == NULL)
    {
        printf("No Classes are available\n");
        return;
    }
    
    struct Data_File class;
    int index = 1;
    while (fread(&class, sizeof(class), 1, file))
    {
        printf("%d. Class: %s | Teacher: %s | Date: %s\n", index++, class.className, class.professor, class.date);
    }
    fclose(file);
}

int status(int classID) {
    FILE *file = fopen(filefile, "rb");
    if (file == NULL)
    {
        return 0;
    }
    
    int id;
    while (fread(&id, sizeof(int), 1, file))
    {
        if (id == classID)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void joinClass(int classID) {
    FILE *file = fopen(filefile, "ab");
    if (file == NULL)
    {
        printf("Error");
        return;
    }

    fwrite(&classID, sizeof(int), 1, file);
    fclose(file);
    printf("You joined a class %d\n", classID);
    printf("pause");
}

void exitClass (int classID) {
    FILE *file = fopen(filefile, "rb");
    if (file == NULL)
    {
        printf("Error\n");
        return;
    }
    
    int IDs[100], count = 0, temp;
    while (fread(&temp, sizeof(int), 1, file))
    {
        if (temp != classID)
        {
            IDs[count++] = temp;
        }
        
    }
    fclose(file);

    file = fopen(fileClass, "wb");
    if (file == NULL)
    {
        printf("Error\n");
        return;
    }
    int i = 0;
    while (i < count)
    {
        fwrite(&IDs[i], sizeof(int), 1, file);
        i++;
    }
    fclose(file);
    printf("Left the class");
    system("pause");
}

void teacherMenu() {
    printf("\n=== Professor Menu ===\n");
    printf("Feature coming soon...\n");
    system("pause");
}

void adminMenu() {
    printf("\n=== Admin Menu ===\n");
    printf("Feature coming soon...\n");
    system("pause");
}
