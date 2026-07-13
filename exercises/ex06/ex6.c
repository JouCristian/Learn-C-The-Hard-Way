#include<stdio.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	int distance = 100;
	float power = 2.345f;
	double super_power = 56789.4532;
	char initial = 'A';
	char first_name[] = "Zed";
	char last_name[] = "Shaw";

	printf("You are %d miles away.\n",distance);
	printf("You have %f levels of power.\n",power);
	printf("You have %f awesome super powers.\n",super_power);
	printf("I have an initial %c.\n",initial);
	printf("I have a first name %s.\n",first_name);
	printf("I have a last name %s.\n",last_name);
	printf("My whole name is %s %c. %s.\n",first_name,initial,last_name);

	printf("Distance in decimal: %d\n", distance);
	printf("Distance in octal: %o\n", distance);
	printf("Distance in hexadecimal: %x\n", distance);
	printf("Distance in uppercase hexadecimal: %X\n", distance);

	printf("With prefixes: %#o, %#x, %#X\n",
       		distance, distance, distance);

	printf("Power with 2 decimal places: %.2f\n", power);
	printf("Power in scientific notation: %e\n", power);
	printf("Power in compact notation: %g\n", power);

	printf("Distance padded with spaces: %8d\n", distance);
	printf("Distance padded with zeros: %08d\n", distance);

	printf("Empty string: '%s'\n", "");

	return 0;
}
