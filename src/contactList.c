/************************
Simrandeep Bajwa 1040216
sbajwa05@uoguelph.ca
Assignment #4
*************************/

#include "contactList.h"
#include <ctype.h>
#include "string.h"
#include <stdbool.h>
/*
  This function opens up an existing file called myContactList.db,
  and if it does not exist, then it will create a new file called
  myContactList.db. It will then return a FILE pointer.
*/
FILE* getFile()
{
  /*Declare varrables*/
  FILE* fptr;

  /*Open File for reading and writing*/
  fptr = fopen("myContactList.db", "a+");

  /*Return File Pointer*/
  return fptr;
}

/*
  This function takes the file pointer, and calls all the functions needed
  in order to run the program.
*/
void startMenu(FILE* fptr)
{
  /*Declare variables*/
  contactList *head;

  /*Initialize variables*/
	head = NULL;

  /*Create the linked list*/
  head = createLinkedList(head, fptr);

  /*Display the contact list and take actions*/
  fullContactList(head);

}

/*
  This function opens creates the linked list from the existing file,
  and then returns the head once it does so.
*/

contactList* createLinkedList(contactList * head, FILE* fptr)
{
  /*Declare variables*/
  contactStruct locationStruct;
  long lengthOfFile;
  contactList* ptr;


  /*Move the location to the end of the file and get the length
  of the file, and move it back to the start*/
  fseek(fptr, 0, SEEK_END);
  lengthOfFile = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  /*Keep running until the end of the file is reached. Allocate memory for new nodes
    in the linked list, and call the insert function to place them in the correct position.*/
  while(ftell(fptr) < lengthOfFile)
  {
    fread(&locationStruct, sizeof(contactStruct), 1, fptr);
    ptr = allocateMemory();
    readInfo(locationStruct, ptr, fptr);
    head = insertContact(head, ptr);
    fseek(fptr, locationStruct.next, SEEK_SET);
  }

  return head;
}

/*
  This function allocates memory for a new node in the linked list.
*/

contactList* allocateMemory()
{
  /*Declare variables*/
  contactList* ptr;

  /*Allocate*/
  ptr = malloc(sizeof(contactList));
  ptr->firstName = malloc(sizeof(char));
  ptr->lastName = malloc(sizeof(char));
  ptr->companyName = malloc(sizeof(char));
  ptr->email = malloc(sizeof(char));

  /*Initalize*/
  ptr->firstName[0] = '\n';
  ptr->lastName[0] = '\n';
  ptr->companyName[0] = '\n';
  ptr->email[0] = '\n';

  /*Return*/
  return ptr;
}

/*
  This function takes the head and frees the linked list.
*/

void freeLinkedList(contactList * head)
{
  /*Declare variables*/
  contactList* temp;

  /*Run through List and free all the nodes*/
  while(head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp->firstName);
    free(temp->lastName);
    free(temp->companyName);
    free(temp->email);
    free(temp);
  }
}

/*
  This function will take the a structure of type contactStruct with all the positions
  of the firstnames, company names etc. The positions will be used to put the data from
  the file into the node of the linked list if it exists.
*/

void readInfo(contactStruct locationStruct, contactList* ptr, FILE* fptr)
{
  /*Declare variables*/
  char a;
  int index;

  /*Initialize Variables*/
  a = 'a';
  index = 0;

  /*Get the First name and if it doesn't exist just set it to a newline character*/
  if(locationStruct.first_name_posn != 0)
  {
    fseek(fptr, locationStruct.first_name_posn, SEEK_SET);
    for(index = 0; a != '\0'; index++)
    {
      fread(&a, sizeof(char), 1, fptr);
      ptr->firstName = realloc(ptr->firstName, (sizeof(char) * (index+2)) );
      ptr->firstName[index] = a;
    }
  }
  else
  {
    ptr->firstName[0] = '\n';
  }

  /*Reinitalize char a*/
  a = ' ';

  /*Get the last name*/
  if(locationStruct.last_name_posn != 0)
  {
    fseek(fptr, locationStruct.last_name_posn, SEEK_SET);
    for(index = 0; a != '\0'; index++)
    {
      fread(&a, sizeof(char), 1, fptr);
      ptr->lastName = realloc(ptr->lastName, (sizeof(char) * (index+2)) );
      ptr->lastName[index] = a;
    }
  }
  else
  {
    ptr->lastName[0] = '\n';
  }

  /*Reinitalize char a*/
  a = ' ';

  /*Get the Company name*/
  if(locationStruct.company_name_posn != 0)
  {
    fseek(fptr, locationStruct.company_name_posn, SEEK_SET);
    for(index = 0; a != '\0'; index++)
    {
      fread(&a, sizeof(char), 1, fptr);
      ptr->companyName = realloc(ptr->companyName, (sizeof(char) * (index+2)) );
      ptr->companyName[index] = a;
    }
  }
  else
  {
    ptr->companyName[0] = '\n';
  }

  /*Reinitalize char a*/
  a = ' ';

  /*Get the Phone number*/
  ptr->phone_number = locationStruct.phone_number;

  /*Get the Email*/
  if(locationStruct.email_posn != 0)
  {
    fseek(fptr, locationStruct.email_posn, SEEK_SET);
    for(index = 0; a != '\0'; index++)
    {
      fread(&a, sizeof(char), 1, fptr);
      ptr->email = realloc(ptr->email, (sizeof(char) * (index+2)) );
      ptr->email[index] = a;
    }
  }
  else
  {
    ptr->email[0] = '\n';
  }
}

/*
  This function takes the head of the linked list and a node, and will place it
  into the correct position in the linked list.
*/

contactList* insertContact(contactList* head, contactList *insert)
{
  /*declare variables*/
  contactList *temp, *prev;

  /*If the linked list is empty then the node will just be places at the head*/
  if(head == NULL)
  {
    insert->next = head;
    head = insert;
  }
  else
  {
    /*Initalize values*/
    temp = head;
    prev = temp;

    /*A loop is used to run through the linked list until a po*/
    while((temp->next != NULL))
    {
      /*Keep track of the previous node, adn the node in front of it*/
      prev = temp;
      temp = temp->next;

      /*A total of 8 cases were used in order to sort. The 8 cases made sure that
        strcmp was not used on a string that did not exist. If the last name did not
        exist for a certain node then its company name would be used since one of the
        two was a requirement for each node*/
      if((insert->lastName[0] != '\n') && (temp->lastName[0] != '\n') && (prev->lastName[0] != '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->lastName, temp->lastName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->lastName, prev->lastName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] == '\n') && (temp->lastName[0] == '\n') && (prev->lastName[0] == '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->companyName, temp->companyName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->companyName, prev->companyName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] == '\n') && (temp->lastName[0] != '\n') && (prev->lastName[0] == '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->companyName, temp->lastName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->companyName, prev->companyName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] == '\n') && (temp->lastName[0] == '\n') && (prev->lastName[0] != '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->companyName, temp->companyName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->companyName, prev->lastName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] == '\n') && (temp->lastName[0] != '\n') && (prev->lastName[0] != '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->companyName, temp->lastName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->companyName, prev->lastName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] != '\n') && (temp->lastName[0] == '\n') && (prev->lastName[0] == '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->lastName, temp->companyName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->lastName, prev->companyName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] != '\n') && (temp->lastName[0] == '\n') && (prev->lastName[0] != '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->lastName, temp->companyName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->lastName, prev->lastName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }
      else if((insert->lastName[0] != '\n') && (temp->lastName[0] != '\n') && (prev->lastName[0] == '\n'))
      {
        /*check to see if the node comes before the temp*/
        if((strcmp(insert->lastName, temp->lastName) < 0))
        {
          /*Inserting at the head of the linked list, and the node at the head must also be checked.
            else the node is just entered somewhere in the body if not the head*/
          if(prev == head && (strcmp(insert->lastName, prev->companyName) < 0))
          {
            insert->next = prev;
            head = insert;
          }
          else
          {
            insert->next = temp;
            prev->next = insert;
          }
          return head;
        }
      }


    }

    /*If there is only one node in the linked list and you need to insert at the head. Test all possible cases.*/
    if(prev == head && prev->next == NULL && ((insert->lastName[0] != '\n') && (prev->lastName[0] != '\n')) && (strcmp(insert->lastName, prev->lastName) < 0))
    {
      insert->next = prev;
      head = insert;
    }
    else if(prev == head && prev->next == NULL && ((insert->lastName[0] == '\n') && (prev->lastName[0] != '\n')) && (strcmp(insert->companyName, prev->lastName) < 0))
    {
      insert->next = prev;
      head = insert;
    }
    else if(prev == head && prev->next == NULL && ((insert->lastName[0] != '\n') && (prev->lastName[0] == '\n')) && (strcmp(insert->lastName, prev->companyName) < 0))
    {
      insert->next = prev;
      head = insert;
    }
    else if(prev == head && prev->next == NULL && ((insert->lastName[0] == '\n') && (prev->lastName[0] == '\n')) && (strcmp(insert->companyName, prev->companyName) < 0))
    {
      insert->next = prev;
      head = insert;
    }
    /*Inserts at the end of the linked list if none of the other cases are met.*/
    else
    {
      /*End of the list*/
      insert->next = temp->next;
      temp->next = insert;
    }
  }

  /*Return Values*/
  return head;
}

/*
  This function prints out the entire linked list, and asks the user for an action,
  and calls functions based on the action.
*/

void fullContactList(contactList* head)
{
  /*declare variables*/
  contactList* ptr, *newContact;
  int index;
  int numOfContacts;
  char action[10];
  int num;
  FILE* fptr;

  /*Initalize Variables*/
  ptr = head;
  numOfContacts = 0;

  /*Run through the linked list to count the number of contacts*/
  while(ptr != NULL)
  {
    numOfContacts++;
    ptr = ptr->next;
  }

  /*Initalize Variables*/
  ptr = head;
  index = 1;

  /*Print number of Contacts*/
  printf("Number of Contacts = %d\n", numOfContacts );

  /*Run through list and print out the contacts checking to make sure the first name,
    last name, and company names exist, and printing according to that.*/
  while ( ptr!= NULL )
  {
    if(ptr->lastName[0] == '\n')
    {
      ptr->companyName[strcspn(ptr->companyName,"\n")] = 0;
      printf("%d. %s\n", index, ptr->companyName);
    }
    else if( (ptr->firstName[0] == '\n') && (ptr->companyName[0] != '\n') && (ptr->lastName[0] != '\n') )
    {
      ptr->lastName[strcspn(ptr->lastName,"\n")] = 0;
      printf("%d. %s\n", index, ptr->lastName);
    }
    else if(ptr->firstName[0] == '\n' && ptr->companyName[0] == '\n')
    {
      ptr->lastName[strcspn(ptr->lastName,"\n")] = 0;
      printf("%d. %s\n", index, ptr->lastName);
    }
    else
    {
      ptr->firstName[strcspn(ptr->firstName,"\n")] = 0;
      ptr->lastName[strcspn(ptr->lastName,"\n")] = 0;
      printf("%d. %s %s\n", index, ptr->firstName, ptr->lastName);
    }
    index++;
    ptr = ptr->next;
  }

  /*Ask for action*/
  printf("Action: ");
  fgets(action, 9, stdin);

  /*Keep prompting user until something valid is inputted*/
  while((!isANumber(action) && ((strcmp(action, "A\n") != 0)) && ((strcmp(action, "X\n") != 0))) ||  action[0] == '\n')
  {
    printf("Action: ");
    fgets(action, 9, stdin);
  }

  /*If the user enters a number*/
  if(isANumber(action))
  {
    /*Make sure the user doesnt try to selecr a contact on an empty list*/
    if(head == NULL)
    {
      printf("You need to enter a contact before you can choose a contact!\n");
      fullContactList(head);
    }
    else
    {
      sscanf(action,"%d", &num);

      /*Make sure that the user enters a number that is actually being displayed on the screen*/
      if(num > numOfContacts)
      {
        printf("Please enter a number between 1 and %d: ", numOfContacts);
        fgets(action, 9, stdin);
        sscanf(action,"%d", &num);

        /*keep asking the user for a number until a valid number is entered*/
        while(num > numOfContacts)
        {
            printf("Please enter a number between 1 and %d: ", numOfContacts);
            fgets(action, 9, stdin);

            /*keep asking the user for a number until a number is entered*/
            while(!isANumber(action))
            {
              printf("Please enter a number between 1 and %d: ", numOfContacts);
              fgets(action, 9, stdin);
            }
            sscanf(action,"%d", &num);
        }
      }

      /*Display selected Contact*/
      head = contactDisplay(head, num);

      /*Return to the full conatct list*/
      fullContactList(head);
    }
  }
  else if((strcmp(action, "A\n") == 0))
  {
    /*Create a new contact if the user prompts to do so.*/
    newContact = allocateMemory();
    newContact = addContact(newContact);

    /*Ask for action*/
    printf("Action: ");
    fgets(action, 9, stdin);

    /*Keep prompting user until something valid is inputted*/
    while( (((strcmp(action, "R\n") != 0)) && ((strcmp(action, "S\n") != 0))) ||  action[0] == '\n' )
    {
      printf("Action: ");
      fgets(action, 9, stdin);
    }

    /*If the user wants to return the fullContactList then free the newly created node and then
      proceed to return back to fullContactList*/
    if((strcmp(action, "R\n") == 0))
    {
      free(newContact->firstName);
      free(newContact->lastName);
      free(newContact->companyName);
      free(newContact->email);
      free(newContact);
      fullContactList(head);
    }
    /*Enter the contact into the linked list if the user wants to save it*/
    else
    {
      head = insertContact(head, newContact);
      fullContactList(head);
    }

  }
  /*if the user wants to exit*/
  else if((strcmp(action, "X\n") == 0))
  {
    /*if the contactlist is empty create a new empty file*/
    if(head == NULL)
    {
      fptr = fopen("myContactList.db", "w+");
      fclose(fptr);
    }
    /*else write the linked list ot the file, and free the list before existing*/
    else
    {
      writeLinkedList(head);
      freeLinkedList(head);
    }
  }

}

/*
  This function checks to see if a string only consists of numbers, and
  returns true or false depending if the string is a number or not.
*/

bool isANumber(char string[])
{
  /*Declare varibales*/
  int counter;
  int index;

  /*Initalize Variables*/
  counter = 0;

  /*If the string consists only of one number check for that*/
  if( (strlen(string) == 2) && isdigit(string[0]))
  {
    counter++;
  }
  else
  {
    /*Check every indice of the string to make sure that it is in fact a number*/
    for(index = 0; (index < strlen(string)) && (string[index] != '\n'); index++)
    {
      if(isdigit(string[index]))
      {
        counter++;
      }
    }
  }

  /*If the amount of numbers is the same as the length of the string then the whole
    string is a number, and return true if that is true.*/
  if(counter == (strlen(string)-1))
  {
    return true;
  }
  else
  {
    return false;
  }

}

/*
  This function checks to see if a string only consists of numbers, and
  returns true or false depending if the string is a number or not. This function
  is the exact same as the one above with a slight tweek. This function accounts
  for a lsight inconsistency in the strings entered.
*/

bool isANumber2(char string[])
{
  /*Declare varibales*/
  int counter;
  int index;

  /*Initalize Variables*/
  counter = 0;

  /*If the string consists only of one number check for that*/
  if( (strlen(string) == 1) && isdigit(string[0]))
  {
    counter++;
  }
  else
  {
    /*Check every indice of the string to make sure that it is in fact a number*/
    for(index = 0; (index < strlen(string)); index++)
    {
      if(isdigit(string[index]))
      {
        counter++;
      }
    }
  }

  /*If the amount of numbers is the same as the length of the string then the whole
    string is a number, and return true if that is true.*/
  if(counter == (strlen(string)))
  {
    return true;
  }
  else
  {
    return false;
  }

}

/*
  This function prints out an individual node, and asks the user for an action
  and proceeds accordingly.
*/

contactList* contactDisplay(contactList* head, int position)
{
  /*Declare variables*/
  contactList* ptr, *newContact;
  int index;
  char action[10];
  int editOption;

  /*Initalize varibales*/
  ptr = head;
  editOption = -1;

  /*Move to position asked*/
  for(index = 0;((index < position-1) && (ptr != NULL)); index++)
  {
    ptr = ptr->next;
  }

  /*Print out information, and make sure that it exists*/
  if(ptr->firstName[0] == '\n')
  {
    printf("First Name: \n");
  }
  else
  {
    printf("First Name: %s\n", ptr->firstName);
  }

  if(ptr->lastName[0] == '\n')
  {
    printf("Last Name: \n");
  }
  else
  {
    printf("Last Name: %s\n", ptr->lastName);
  }

  if(ptr->companyName[0] == '\n')
  {
    printf("Company Name: \n");
  }
  else
  {
    printf("Company Name: %s\n", ptr->companyName);
  }

  printf("Phone Number: %lu\n", ptr->phone_number);

  if(ptr->email[0] == '\n')
  {
    printf("Email: \n");
  }
  else
  {
    printf("Email: %s\n", ptr->email);
  }

  /*Ask for action*/
  printf("Action: ");
  fgets(action, 9, stdin);

  /*Keep prompting user until something valid is inputted*/
  while( (((strcmp(action, "R\n") != 0)) && ((strcmp(action, "E\n") != 0)) && ((strcmp(action, "D\n") != 0))) ||  action[0] == '\n')
  {
    printf("Action: ");
    fgets(action, 9, stdin);
  }

  /*Delete the contact if D is entered*/
  if((strcmp(action, "D\n") == 0))
  {
    head = deleteContact(head, position);
  }
  /*Edit the contact if D is entered*/
  else if((strcmp(action, "E\n") == 0))
  {
    editContact(ptr, &editOption);

    /*If the user entered D then editOption will be set to 1 and the contact will be deleted */
    if(editOption == 1)
    {
      head = deleteContact(head, position);
    }
    /*If the user entered S then editOption will be set to 2 and the contact will be saved */
    else if(editOption == 2)
    {
      /*The following creates a new contact that is set to all the information of the node that was put into the
        editContact function.*/
      newContact = allocateMemory();

      if(ptr->firstName[0] != '\n')
      {
        newContact->firstName = realloc(newContact->firstName, (sizeof(char)*(strlen(ptr->firstName) + 1)) );
        strcpy(newContact->firstName, ptr->firstName);
      }

      if(ptr->lastName[0] != '\n')
      {
        newContact->lastName = realloc(newContact->lastName, (sizeof(char)*(strlen(ptr->lastName) + 1)) );
        strcpy(newContact->lastName, ptr->lastName);
      }

      if(ptr->companyName[0] != '\n')
      {
        newContact->companyName = realloc(newContact->companyName, (sizeof(char)*(strlen(ptr->companyName) + 1)) );
        strcpy(newContact->companyName, ptr->companyName);
      }

      newContact->phone_number = ptr->phone_number;

      if(ptr->email[0] != '\n')
      {
        newContact->email = realloc(newContact->email, (sizeof(char)*(strlen(ptr->email) + 1)) );
        strcpy(newContact->email, ptr->email);
      }

      /*Then the old contact ptr will be deleted, and the new contact will be entered into the linked list.
        This is to make sure the editted contact is placed in the correct place after being eddited.*/
      head = deleteContact(head, position);
      head = insertContact(head, newContact);
    }

  }

  return head;
}

/*
  This function takes the head and a position, and will delete a contact at the specfied position.
*/

contactList* deleteContact(contactList* head, int position)
{
  /*Declare Variables*/
  contactList* temp1, *temp2;
  int index;

  /*Initalize Variables*/
  temp1 = head;

  /*If the first node is to be deleted*/
  if(position == 1)
  {
    head = temp1->next;
    free(temp1->firstName);
    free(temp1->lastName);
    free(temp1->companyName);
    free(temp1->email);
    free(temp1);
    return head;
  }

  /*If a node other than the first node is to be deleted*/

  /*Move to the correct position*/
  for(index = 0; index < position -2; index++)
  {
    temp1 = temp1->next;
  }

  /*Remove temp2 out of the list and then free it*/
  temp2 = temp1->next;
  temp1->next = temp2->next;

  free(temp2->firstName);
  free(temp2->lastName);
  free(temp2->companyName);
  free(temp2->email);
  free(temp2);

  return head;

}

/*
  This function takes a node, and an integer pointer. The function will allow the user
  to edit the company name, and phone number of the node. It will then change the option's
  value depending on what is entered in by the user.
*/

void editContact(contactList* ptr, int* option)
{
  /*Declare variables*/
  int index;
  char action[10];
  char* companyName;
  char* phoneNum;

  /*Initalize varibales*/
  companyName = malloc(sizeof(char));
  phoneNum = malloc(sizeof(char));

  /*Print out the contact if it exists and allow the company name and phone number to be edited*/
  if(ptr->firstName[0] != '\n')
  {
    printf("First Name: %s\n", ptr->firstName);
  }
  else
  {
    printf("First Name: \n");
  }

  if(ptr->lastName[0] != '\n')
  {
    printf("Last Name: %s\n", ptr->lastName);
  }
  else
  {
    printf("Last Name: \n");
  }

  printf("Company Name: ");
  for(index = 0; ((companyName[index] = fgetc(stdin)) != '\n'); index++)
  {
    companyName = realloc(companyName, sizeof(char)*(index+2));
  }
  if(companyName[0] != '\n')
  {
    companyName[index] = '\0';
  }

  printf("Phone Number (enter only numbers): ");
  for(index = 0; ((phoneNum[index] = fgetc(stdin)) != '\n'); index++)
  {
    phoneNum = realloc(phoneNum, sizeof(char)*(index+2));
  }
  if(phoneNum[0] != '\n')
  {
    phoneNum[index] = '\0';
  }

  /*Make sure that a valid phone number is entered*/
  if( (phoneNum[0] == '\n') || !isANumber2(phoneNum) || ((strlen(phoneNum) != 10) && (strlen(phoneNum) != 7)))
  {
    while((phoneNum[0] == '\n') || !isANumber2(phoneNum) || ((strlen(phoneNum) != 10) && (strlen(phoneNum) != 7)))
    {
      printf("Phone Number (enter only numbers and 7 or 10 digits): ");
      phoneNum = realloc(phoneNum, sizeof(char));
      for(index = 0; ((phoneNum[index] = fgetc(stdin)) != '\n'); index++)
      {
        phoneNum = realloc(phoneNum, sizeof(char)*(index+2));
      }
      if(phoneNum[0] != '\n')
      {
        phoneNum[index] = '\0';
      }

    }
  }
  if(ptr->email[0] != '\n')
  {
    printf("Email: %s\n", ptr->email);
  }
  else
  {
    printf("Email: \n");
  }

  /*Ask for action*/
  printf("Action: ");
  fgets(action, 9, stdin);

  /*Keep prompting user until something valid is inputted*/
  while( (((strcmp(action, "R\n") != 0)) && ((strcmp(action, "S\n") != 0)) && ((strcmp(action, "D\n") != 0))) ||  action[0] == '\n' )
  {
    printf("Action: ");
    fgets(action, 9, stdin);
  }

  if((strcmp(action, "D\n") == 0))
  {
    *option = 1;
  }
  else if((strcmp(action, "S\n") == 0))
  {
    /*Change the node's information only if the user wants to save it*/
    ptr->companyName = realloc(ptr->companyName, (strlen(companyName)+1));
    strcpy(ptr->companyName, companyName);
    sscanf(phoneNum, "%lu", &ptr->phone_number);
    *option = 2;
  }

  free(companyName);
  free(phoneNum);
}

/*
  This function takes a node, and will allow the user to enter all the necessary
  information. It will then return the node.
*/

contactList* addContact(contactList* newContact)
{
  /*Declare variables*/
  int index;
  char* phoneNum;

  /*Initalize Variables*/
  phoneNum = malloc(sizeof(char));

  /*Ask for Information*/
  printf("New Contact\n");

  printf("First Name: ");
  for(index = 0; ((newContact->firstName[index] = fgetc(stdin)) != '\n'); index++)
  {
    newContact->firstName = realloc(newContact->firstName, sizeof(char)*(index+2));
  }
  if(newContact->firstName[0] != '\n')
  {
    newContact->firstName[index] = '\0';
  }

  printf("Last Name: ");
  for(index = 0; ((newContact->lastName[index] = fgetc(stdin)) != '\n'); index++)
  {
    newContact->lastName = realloc(newContact->lastName, sizeof(char)*(index+2));
  }
  if(newContact->lastName[0] != '\n')
  {
    newContact->lastName[index] = '\0';
  }

  printf("Company Name: ");
  for(index = 0; ((newContact->companyName[index] = fgetc(stdin)) != '\n'); index++)
  {
    newContact->companyName = realloc(newContact->companyName, sizeof(char)*(index+2));
  }
  if(newContact->companyName[0] != '\n')
  {
    newContact->companyName[index] = '\0';
  }

  while( (newContact->lastName[0] == '\n') && (newContact->companyName[0] == '\n') )
  {
    printf("Company Name: ");
    for(index = 0; ((newContact->companyName[index] = fgetc(stdin)) != '\n'); index++)
    {
      newContact->companyName = realloc(newContact->companyName, sizeof(char)*(index+2));
    }
    if(newContact->companyName[0] != '\n')
    {
      newContact->companyName[index] = '\0';
    }
  }

  printf("Phone Number (enter only numbers): ");
  for(index = 0; ((phoneNum[index] = fgetc(stdin)) != '\n'); index++)
  {
    phoneNum = realloc(phoneNum, sizeof(char)*(index+2));
  }
  if(phoneNum[0] != '\n')
  {
    phoneNum[index] = '\0';
  }

  /*Check to make sure a vlid phone number is being entered*/
  if( (phoneNum[0] == '\n')|| !isANumber2(phoneNum) || ((strlen(phoneNum) != 10) && (strlen(phoneNum) != 7)))
  {
    while((phoneNum[0] == '\n') || !isANumber2(phoneNum) || ((strlen(phoneNum) != 10) && (strlen(phoneNum) != 7)))
    {
      printf("Phone Number (enter only numbers and 7 or 10 digits): ");
      phoneNum = realloc(phoneNum, sizeof(char));
      for(index = 0; ((phoneNum[index] = fgetc(stdin)) != '\n'); index++)
      {
        phoneNum = realloc(phoneNum, sizeof(char)*(index+2));
      }
      if(phoneNum[0] != '\n')
      {
        phoneNum[index] = '\0';
      }

    }
  }

  sscanf(phoneNum, "%lu", &newContact->phone_number);

  printf("Email: ");
  for(index = 0; ((newContact->email[index] = fgetc(stdin)) != '\n'); index++)
  {
    newContact->email = realloc(newContact->email, sizeof(char)*(index+2));
  }
  if(newContact->email[0] != '\n')
  {
    newContact->email[index] = '\0';
  }

  /*Check to make sure that the email has the correct format by calling the isValidEmail
    function.*/
  while( (newContact->email[0] == '\n') || !isValidEmail(newContact->email))
  {
    printf("Email: ");
    for(index = 0; ((newContact->email[index] = fgetc(stdin)) != '\n'); index++)
    {
      newContact->email = realloc(newContact->email, sizeof(char)*(index+2));
    }
    if(newContact->email[0] != '\n')
    {
      newContact->email[index] = '\0';
    }
  }

  /*Free variables*/
  free(phoneNum);

  /*Return Head*/
  return newContact;
}

/*
  This function takes a string, and checks to see if that string has the correct
  format for an email. It will return true or false accordingly.
*/

bool isValidEmail(char string[])
{
  /*Declare variables*/
  int atPosition, dotPosition, index, repeat;

  /*Initalize Variables*/
  atPosition = -1;
  dotPosition = -1;
  index = 0;
  repeat = 0;

  /*Find @ and . positionss*/
  for(index = 0; index < strlen(string); index++)
  {
    if(string[index] == '@')
    {
      atPosition = index;
      repeat++;
    }

    /*if the @ repeats more than once then we know that the email is not correctly formatted
      so false is returned.*/
    if(repeat > 1)
    {
      return false;
    }
  }

  /*Reset repeat to check for repeated periods*/
  repeat = 0;

  /*This runs to find the position of the period, and if a period is repeated
    false is returned.*/
  for(index = 0; index < strlen(string); index++)
  {
    if(string[index] == '.')
    {
      dotPosition = index;
      repeat++;
    }

    if(repeat > 1)
    {
      return false;
    }
  }

  /*Check if email is valid*/

  /*if string is not longer than 5 it can't be a valid email ex. a@g.c There needs to be a minimum of 5 characters*/
  if(strlen(string) < 5)
  {
    return false;
  }
  else
  {
    /*This makes sure that the period comes after the @, and that the period and @ are not right beside each other.*/
    if( ( (dotPosition != -1) && (atPosition != -1) ) && (dotPosition > atPosition) && ((atPosition + 1) != dotPosition) && (atPosition != 0) && (string[dotPosition + 1] != '\0'))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

/*
  This function will take the head and write the entire linked list to the file.
  The function does not require a file pointer, as it will create a new file
  with the same name to overwrite the file, and then the linked list will be
  written to that new file.
*/

void writeLinkedList(contactList* head)
{
  /*Declare variables*/
  contactStruct recordsHolder;
  contactList* ptr;
  FILE* fptr;

  /*Initalize Variables*/
  ptr = head;

  /*Write to a new file*/
  fptr = fopen("myContactList.db", "w+");

  while(ptr != NULL)
  {
    /*Assign/calculate Positions*/
    recordsHolder.first_name_posn = ftell(fptr) + sizeof(contactStruct);

    /*calculate the positions by checking if the first name, last name etc. exists or not*/
    if(ptr->firstName[0] == '\n')
    {
      recordsHolder.last_name_posn =  recordsHolder.first_name_posn;
    }
    else
    {
      recordsHolder.last_name_posn =  (recordsHolder.first_name_posn) + (strlen(ptr->firstName)+1);
    }

    if(ptr->lastName[0] == '\n')
    {
      recordsHolder.company_name_posn =  recordsHolder.last_name_posn;
    }
    else
    {
      recordsHolder.company_name_posn =  (recordsHolder.last_name_posn) + (strlen(ptr->lastName)+1);
    }

    if(ptr->companyName[0] == '\n')
    {
      recordsHolder.email_posn =  recordsHolder.company_name_posn;
    }
    else
    {
      recordsHolder.email_posn =  (recordsHolder.company_name_posn) + (strlen(ptr->companyName)+1);
    }

    if(ptr->email[0] == '\n')
    {
      recordsHolder.next =  (recordsHolder.email_posn);
    }
    else
    {
      recordsHolder.next =  (recordsHolder.email_posn) + (strlen(ptr->email)+1);
    }

    /*Set position to zero if need so, and set phone Number*/
    recordsHolder.phone_number = ptr->phone_number;
    setZero(&recordsHolder.first_name_posn, ptr->firstName);
    setZero(&recordsHolder.last_name_posn, ptr->lastName);
    setZero(&recordsHolder.company_name_posn, ptr->companyName);
    setZero(&recordsHolder.email_posn, ptr->email);

    /*Write Information to file*/
    fwrite(&recordsHolder, sizeof(contactStruct), 1, fptr);
    checkAndWrite(ptr->firstName, fptr);
    checkAndWrite(ptr->lastName, fptr);
    checkAndWrite(ptr->companyName, fptr);
    checkAndWrite(ptr->email, fptr);

    /*Move to next position*/
    ptr = ptr->next;
  }

  /*Close file*/
  fclose(fptr);
}

/*
  This function takes a string, and checks to see if that string actually exists or not,
  and will write it to the file by using the file pointer given in the parameters.
*/

void checkAndWrite(char* string, FILE* fptr)
{
  if(string[0] != '\n')
  {
    fwrite(string, sizeof(char), (strlen(string)+1), fptr);
  }
}

/*
  This function takes a string and a long pointer, it will set the long Pointer
  equal to zero if the string does not exist.
*/

void setZero(long * position, char* string)
{
  if(string[0] == '\n')
  {
    *position = 0;
  }
}
