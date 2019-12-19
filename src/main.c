/************************
Simrandeep Bajwa 1040216
sbajwa05@uoguelph.ca
Assignment #4
*************************/

#include "contactList.h"

int main()
{
	/*Declare variables*/
	FILE* fptr;

	/*Get the file*/
	fptr = getFile();

	/*Check to make sure the file was read correctly*/
	if(fptr == NULL)
	{
		printf("There was an error!\n");
	}
	else
	{
		startMenu(fptr);
	}

	/*Close the file and free variables*/
	fclose(fptr);

	return 1;
}
