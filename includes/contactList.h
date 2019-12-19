/************************
Simrandeep Bajwa 1040216
sbajwa05@uoguelph.ca
Assignment #4
*************************/

#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct contact
{
	unsigned long phone_number;
	long first_name_posn;
	long last_name_posn;
	long company_name_posn;
	long email_posn;
	long next;
};
typedef struct contact contactStruct;

/*Linked list structure*/
struct allContacts
{
	unsigned long phone_number;
	char* firstName;
	char* lastName;
	char* companyName;
	char* email;
	struct allContacts* next;

};
typedef struct allContacts contactList;

/*Function Definitions*/
FILE* getFile();
void startMenu(FILE* fptr);
contactList* createLinkedList(contactList * head, FILE* fptr);
contactList* allocateMemory();
void readInfo(contactStruct locationStruct, contactList* ptr, FILE* fptr);
contactList* insertContact(contactList* head, contactList *insert);
void fullContactList(contactList* head);
bool isANumber(char string[]);
bool isANumber2(char string[]);
contactList* contactDisplay(contactList* head, int position);
contactList* deleteContact(contactList* head, int position);
void editContact(contactList* ptr, int* option);
void freeLinkedList(contactList * head);
contactList* addContact(contactList* head);
bool isValidEmail(char string[]);
void writeLinkedList(contactList* head);
void checkAndWrite(char* string, FILE* fptr);
void setZero(long * position, char* string);

#endif
