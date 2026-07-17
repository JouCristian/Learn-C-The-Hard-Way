#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"dbg.h"

#define MAX_DATA 100

typedef enum EyeColor{
	BLUE_EYES = 0,
	GREEN_EYES,
	BROWN_EYES,
	BLACK_EYES,
	OTHER_EYES
}EyeColor;

static const char *const EYE_COLOR_NAME[] = {
	"Blue",
	"Green",
	"Brown",
	"Black",
	"Other"
};

typedef struct Person{
	int age;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
	EyeColor eyes;
	float income;
}Person;

static int read_line(char *buffer, size_t size){
	if(buffer == NULL || size < 2){
		return 0;
	}
	if(fgets(buffer,(int)size,stdin) == NULL){
		return 0;
	}
	char *newline = strchr(buffer,'\n');
	if(newline != NULL){
		*newline = '\0';
		return 1;
	}
	if(feof(stdin)){
		return 1;
	}
	int ch = getchar();

	if(ch == '\n' || ch == EOF){
		return 1;
	}
	while((ch = getchar()) != '\n' && ch != EOF){}
	return -1;
}

int main(void){
	Person you = {0};
	int line_result = 0;
	int rc = 0;
	int selected_eye = 0;

	printf("What's your first name? ");
	fflush(stdout);

	line_result = read_line(you.first_name,sizeof(you.first_name));
	check(line_result != 0,"Failed to read first name.");
	check(line_result != -1,"First name is too long.");
	check(you.first_name[0] != '\0',"First name cannot be empty.");

	printf("What's your last name? ");
	fflush(stdout);
	line_result = read_line(you.last_name,sizeof(you.last_name));
	check(line_result != 0, "Failed to read last name.");
	check(line_result != -1, "Last name is too long.");
	check(you.last_name[0] != '\0',"Last name cannot be empty.");

	printf("How old are you? ");
	fflush(stdout);
	errno = 0;
	rc = fscanf(stdin, "%d", &you.age);
	check(you.age >= 0 && you.age <= 150, "Age must be between 0 and 150.");

	printf("What color are your eyes?\n");
	int i=0;
	for(i = 0; i <= OTHER_EYES; i++){
		printf("%d) %s\n",
				i +1,
				EYE_COLOR_NAME[i]);
	}

	printf(">");
	fflush(stdout);

	errno = 0;

	rc = fscanf(stdin,"%d", &selected_eye);
	check(rc == 1, "Eye color must be a number.");
	check(selected_eye >= 1 && selected_eye <= OTHER_EYES + 1,"That is not a valid eye color.");
	you.eyes = (EyeColor)(selected_eye - 1);
	
	printf("How much do you make an hour?");
	fflush(stdout);
	errno = 0;
	
	rc = fscanf(stdin,"%f",&you.income);
	check(rc == 1, "Income cannot be negative.");

	printf("\n-----RESULT-----\n");
	printf("First Name: %s\n",you.first_name);
	printf("Last name: %s\n",you.last_name);
	printf("Age: %d\n",you.age);
	printf("Eyes: %s\n",EYE_COLOR_NAME[you.eyes]);
	printf("Income: %.2f\n",(double)you.income);
	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}
