
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


//Represents a node
typedef struct node {
    char *artist;
    char *songName;
    char *genre;
    struct node *next;
} Node;

/**
 * @brief Represents a linked list.
 */
typedef struct linkedList {
    Node *head;
} LinkedList;

// Declarations of linked list functions

void initList(LinkedList *list) {
    // The head initially points to nothing.
    list->head = NULL;
}

//Creates a new Node
Node* createNode (char* songNameInput, char* artistInput, char* genreInput) {
	Node * current = malloc (sizeof (Node));

	current -> songName = strdup (songNameInput);
	current -> artist = strdup (artistInput);
	current -> genre = strdup (genreInput);
	current -> next = NULL;
	
	return current;
	
}

//Checks if the list is empty
bool isEmpty(LinkedList *list) {
    if(list->head == NULL) {
        // The head does not point to anything.
        return true;
    }
    
    return false;
}

//Inserts the node in correct alpha order in the list
bool insertInAlpha (LinkedList *list, char* songNameInput, char* artistInput, char* genreInput) {
    if(isEmpty(list)) {
        // The list is empty, create the very first node.
        list->head = createNode(songNameInput, artistInput, genreInput); // Could return NULL if out-of-memory.
        
        return list->head != NULL;
    }
    
    // The list is not empty.
    // Create a new node.
    Node *temp = createNode(songNameInput, artistInput, genreInput);
    if(temp == NULL) {
        // We couldn't allocate enough memory for a new node.
        return false;
    }
	
	if (strcmp (temp -> songName, list -> head -> songName) < 0) {
		temp -> next = list -> head;
		list -> head = temp;
		return true;
	}
	
	Node *n = list -> head;
	
	//If the current song name is greater than the song name at the current element in the list
	while (n -> next != NULL && strcmp (temp-> songName, n-> next-> songName) > 0) {
		n = n -> next;
	}
	
	//Link the rest of the list with the temp node
	temp -> next = n -> next;
	//Overwrite next with the temp node
	n -> next = temp;
    
    return true;
}

//Prints entire linked list
void printList(LinkedList *list) {
    Node *n = list->head;
    
    while(n != NULL) {
        // Print out the data at this element.
		printf ("\n");
        printf("%s\n", n->songName);
		printf("%s\n", n->artist);
		printf("%s\n", n->genre);
        // Move to the next element in the list.
        n = n->next;
    }
}


// Declarations of support functions
// See below the main function for descriptions of what these functions do

void inputStringFromUser(char *prompt, char *s, int arraySize);
void songNameDuplicate(char *songName);
void songNameFound(char *songName);
void songNameNotFound(char *songName);
void songNameDeleted(char *songName);
void artistFound(char *artist);
void artistNotFound(char *artist);
void printMusicLibraryEmpty(void);
void printMusicLibraryTitle(void);
bool checkDuplicate (LinkedList *list, char* input);
Node* getDuplicateNode (LinkedList *list, char* input);
bool deleteFirstMatch(LinkedList *list, char* value);
bool deleteAllNodes(LinkedList * list);

const int MAX_LENGTH = 1024;

int main(void) {
	//Creates and initializes new linked list
	LinkedList myList;
	initList(&myList);
	
	
    // Announce the start of the program
    printf("Personal Music Library.\n\n");
    printf("%s", "Commands are I (insert), D (delete), S (search by song name),\n"
           "P (print), Q (quit).\n");

    char response;
	char input [MAX_LENGTH + 1];
    char songNameInput [MAX_LENGTH + 1];
	char artistInput [MAX_LENGTH + 1];
	char genreInput [MAX_LENGTH + 1];
	
    do {
        inputStringFromUser("\nCommand", input, MAX_LENGTH);

        // Response is the first character entered by user.
        // Convert to uppercase to simplify later comparisons.
        response = toupper(input[0]);

        if (response == 'I') {
            
            // USE THE FOLLOWING STRINGS WHEN PROMPTING FOR DATA:
			char *promptName = "Song name" ;
			char *promptArtist =  "Artist" ;
			char *promptGenre = "Genre" ;
			
			inputStringFromUser(promptName, songNameInput, MAX_LENGTH);
			inputStringFromUser (promptArtist, artistInput, MAX_LENGTH);
			inputStringFromUser(promptGenre, genreInput, MAX_LENGTH);
			
			if (!checkDuplicate(&myList, songNameInput)) {
				// Insert a song into the linked list.
				// Maintain the list in alphabetical order by song name.
				insertInAlpha(&myList, songNameInput, artistInput, genreInput);
			} else {
				songNameDuplicate(songNameInput);
			}
			
        }
        else if (response == 'D') {
            // Delete a song from the list.

            char *prompt = "\nEnter the name of the song to be deleted" ;
			
			inputStringFromUser(prompt, input, MAX_LENGTH);
			
			if (deleteFirstMatch(&myList, input)) {
				songNameDeleted(input);
			} else {
				songNameNotFound(input);
			}

        }
        else if (response == 'S') {
            // Search for a song by its name.
			
            char *prompt = "\nEnter the name of the song to search for" ;
			
			 //   ADD STATEMENT(S) HERE
			inputStringFromUser(prompt, input, MAX_LENGTH);
			
			Node* n = getDuplicateNode(&myList, input);
			
			if (n == NULL) {
				songNameNotFound(input);
			} else {
				songNameFound(input);
				printf ("\n");
				printf("%s\n", n->songName);
				printf("%s\n", n->artist);
				printf("%s\n", n->genre);
			}
           

        }
        else if (response == 'P') {
            // Print the music library.
			
			if (!isEmpty(&myList)) {
				printMusicLibraryTitle();
				printList(&myList);
			} else {
				printMusicLibraryEmpty();
			}
            //   ADD STATEMENT(S) HERE

        }
        else if (response == 'Q') {
            ; // do nothing, we'll catch this below
        }
        else {
            // do this if no command matched ...
            printf ("\nInvalid command.\n");
        }
    } while (response != 'Q');

    // Delete the entire linked list.
	deleteAllNodes(&myList);
	
    // Print the linked list to confirm deletion.
	if (isEmpty(&myList)) {
		printMusicLibraryEmpty();
	}

    return 0;
}

// Support Function Definitions

// Prompt the user for a string safely, without buffer overflow
void inputStringFromUser(char *prompt, char *s, int maxStrLength) {
    int i = 0;
    char c;

    printf("%s --> ", prompt);
    while (i < maxStrLength && (c = getchar()) != '\n') {
        s[i++] = c;
	}
    s[i] = '\0';
}

// Function to call when the user is trying to insert a song name
// that is already in the personal music library.
void songNameDuplicate(char *songName) {
    printf("\nA song with the name '%s' is already in the music library.\n"
           "No new song entered.\n", songName);
}

// Function to call when a song name was found in the personal music library.
void songNameFound(char *songName) {
    printf("\nThe song name '%s' was found in the music library.\n",
           songName);
}

// Function to call when a song name was not found in the personal music library.
void songNameNotFound(char *songName) {
    printf("\nThe song name '%s' was not found in the music library.\n",
           songName);
}

// Function to call when a song name that is to be deleted
// was found in the personal music library.
void songNameDeleted(char *songName) {
    printf("\nDeleting a song with name '%s' from the music library.\n",
           songName);
}

// Function to call when printing an empty music library.
void printMusicLibraryEmpty(void) {
    printf("\nThe music library is empty.\n");
}

// Function to call to print a title when the entire music library is printed.
void printMusicLibraryTitle(void) {
    printf("\nMy Personal Music Library: \n");
}

// Add your functions below this line.

bool checkDuplicate (LinkedList *list, char* input) {
	Node* n = list -> head;
	
	while (n != NULL) {
		if (strcmp (input, n -> songName) == 0) {
			return true;
		}
		n = n -> next;
	}
	
	return false;
}

Node* getDuplicateNode (LinkedList *list, char* input) {
	Node* n = list -> head;
	
	while (n != NULL) {
		if (strcmp (input, n -> songName) == 0) {
			return n;
		}
		n = n -> next;
	}
	
	return NULL;
}

bool deleteAllNodes(LinkedList * list) {
	if (isEmpty(list)) {
		return false;
	}
	
	Node * temp = NULL;
	
	while (!isEmpty (list)) {
		temp = list -> head -> next;
		
		songNameDeleted(list -> head -> songName);
		free (list->head->songName);
		free (list->head->artist);
		free (list->head->genre);
		free(list -> head); 
		list -> head = temp; 
	}
	
	list -> head = NULL;
	return true;
}

bool deleteFirstMatch(LinkedList *list, char* value) {
    if (isEmpty(list) == true) {
        // There is nothing to do in an empty list.
        return false;
    }
	
	// The first node matches the value.
    if (strcmp(list->head->songName, value) == 0) {

        // Save a pointer to the second element in the list. Could be NULL, that's ok.
        Node *temp = list->head->next;

		free (list->head->songName);
		free (list->head->artist);
		free (list->head->genre);
		free(list->head); // Delete the first element of the list.
		list->head = temp; // Update the list to point to a new head.

        return true;
    }

    // Search for a node that matches the value, but maintain a pointer to the node just before it.
    Node *current = list->head;
    while (current->next != NULL && strcmp(current->next->songName, value) != 0) {
        current = current->next;
    }

    // current now points to a node just before the node that matched, OR current points to the last node.
    if (current->next != NULL) {
        // current does not point to the last node.
        Node *temp = current->next; // temp is the node we must delete.
        current->next = temp->next; // Update n so that temp is no longer linked.
		free (temp->songName);
		free (temp->artist);
		free (temp->genre);
		free(temp);

        return true;
    }

    return false;
}
