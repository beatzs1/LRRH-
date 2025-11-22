/*
    Description: A Simple Game about "Red Riding Hood" traversing through a board trying to avoid obstacles and completing the task of getting into granny
    Programmed By: Zion Aemar S. Tagle
    Last Modified: 11/9/2025
    Version: v0.6.7
    Acknowledgements:
        -  https://www.w3schools.com/
        -  stdbool library (for providing booleans into C)
        -  windows.h to put sleep function
        -  math.h 
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>
#include <conio.h>

typedef enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
}direction;

typedef struct LRRH {
    int xPos;
    int yPos;
    char dirChar;
    direction playerDirection;
    bool hasWoodsman;
    bool hasFlower;
    bool hasBread;
}LRRH;

typedef struct Granny {
    int xPos;
    int yPos;
    int stepsAway;
}Granny;


/*
    This function prints the current state of the game board to the console.
    It formats the 1D gameArray into a 2D n x n grid display.
    Precondition: gameArray points to a valid character array of at least nTableSize * nTableSize elements.
    @param nTableSize The dimension (n) of the n x n square game board.
    @param gameArray A pointer to the 1D character array representing the game board.
    @return void
*/
void printGameBoard(int nTableSize, char *gameArray) {
    for (int i = 0;  i<nTableSize; i++) {
        for(int j=0; j<nTableSize; j++ ) {
            printf("%c ",gameArray[i * nTableSize + j]);
        }
        printf("\n");
    }
}

/*
    This function places a specified object character onto the game board at user-defined coordinates.
    It prompts the user for (X, Y) coordinates, adjusts for 1-based indexing,
    and checks if the target square is empty ('*') before placing the object.
    It will re-prompt if the chosen location is already occupied.
    Precondition: gameArray is a valid pointer to the game board. nTableSize is the board's dimension.
                  User inputs coordinates in 1-based indexing (1,1).
    @param nTableSize The dimension (n) of the n x n game board.
    @param gameArray A pointer to the 1D character array representing the game board.
    @param object The character (e.g., 'W', 'P', '_') to be placed on the board.
    @return void
*/
void generateObject(int nTableSize, char *gameArray, char object) {
    bool generated = false;
    // srand(time(NULL));
    while(generated == false) {
        /*
        int x = rand() % nTableSize + 1;
        int y = rand() % nTableSize + 1;
        */
        int x, y;
        printf("Please input X and Y Coordinate for Respecting Object Type(Format of X Y): ");
        scanf("%d%d", &x, &y);

        // Offset User Input By One As 'Starting Position' Starts in (1,1)
        x-=1;
        y-=1;

        if(gameArray[y * nTableSize + x] == '*'){
            gameArray[y * nTableSize + x] = object;
            generated = true;
        }else {
            printf("Position already has Object, Please try again...\n\n");
        }
    }
}

/*
    This function manages the generation of one or more game objects of a specific type.
    If 'multiple' is true, it asks the user how many objects of that type to create.
    It then calls generateObject() the specified number of times.
    Precondition: gameArray is a valid pointer. nGameSize is the board's dimension.
    @param type A string describing the object (e.g., "Pit") for user prompts.
    @param object The character representing the object (e.g., '_').
    @param nGameSize The dimension (n) of the n x n game board.
    @param gameArray A pointer to the 1D character array representing the game board.
    @param multiple A boolean; if true, prompts user for the amount to generate.
    @return void
*/
void askToGenerate(char *type, char object, int nGameSize, char *gameArray, bool multiple) {
    int amount;
    int invalid=0;
    int limit = nGameSize/2;
    
    
    
    if(multiple == true) {
        do{
        	invalid=0;
        	amount =0;
        printf("\nHow Many %ss Do You Want in The Game? (1-%d): ", type, limit);
        scanf("%d", &amount);
        if(amount<=0||amount>limit){
        	printf("Invalid Amount!");
        	invalid=1;
        }
        }while(invalid==1);
    }else {
        printf("\nCurrently Placing %s.\n", type);
    }
    

    for(int i = 0; i<amount;i++) {
        generateObject(nGameSize, (char*)gameArray, object);
    }
}

/*
    This function moves the player one square forward based on their current direction.
    It updates the player's xPos or yPos, stores the object at the new location
    in savedObject, and performs boundary checks to prevent moving off the grid.
    Precondition: player and savedObject are valid pointers. gameArray is the valid game board.
    @param player A pointer to the LRRH struct holding the player's current state.
    @param savedObject A pointer to a char to store the object at the destination square.
    @param gameArray A pointer to the 1D character array representing the game board.
    @param nTableSize The dimension (n) of the n x n game board.
    @return void
*/
void moveForward(LRRH *player, char *savedObject, char *gameArray, int nTableSize) {
    switch(player->playerDirection) {
         case RIGHT:
            player->xPos += 1;

            // Set Saved Object
            *savedObject = gameArray[player->yPos * nTableSize + player->xPos];
            break;
        case DOWN:
            player->yPos += 1;

            // Set Saved Object
            *savedObject = gameArray[player->yPos * nTableSize + player->xPos];
            break;
        case LEFT:
            player->xPos -= 1;

            // Set Saved Object
            *savedObject = gameArray[player->yPos * nTableSize + player->xPos];   
            break;
        case UP:
            player->yPos -= 1;

            // Set Saved Object
            *savedObject = gameArray[player->yPos * nTableSize + player->xPos];    
            break;
    }

    /* Code Was Used to Debug Certain Bug
    printf("\n\n\nDEBUG: '%c'\n\n\n", *savedObject);
    */
    
    // Boundary Checks
    if(player->xPos < 0) {player->xPos = 0;}
    if(player->yPos < 0) {player->yPos = 0;}

    if(player->xPos > nTableSize-1) {player->xPos -= 1;}
    if(player->yPos > nTableSize-1) {player->yPos -= 1;}
}

/*
    This function rotates the player's facing direction 90 degrees clockwise.
    It updates the playerDirection enum in the LRRH struct.
    (RIGHT -> DOWN -> LEFT -> UP -> RIGHT)
    Precondition: player is a valid pointer to the LRRH struct.
    @param player A pointer to the LRRH struct whose direction will be updated.
    @return void
*/
void rotate(LRRH *player) {
   switch(player->playerDirection) {
        case RIGHT:
            player->playerDirection = DOWN;
            player->dirChar = 'v';
            break;
        case DOWN:
            player->playerDirection = LEFT;
            player->dirChar = '<';
            break;
        case LEFT:
            player->playerDirection = UP;
            player->dirChar = '^';
            break;
        case UP:
            player->playerDirection = RIGHT;
            player->dirChar = '>';
            break;
   }
}

/*
    This function senses the square immediately in front of the player.
    It checks the object in the adjacent square based on playerDirection
    and prints a corresponding sensory message to the console.
    Precondition: player is a valid pointer. gameArray is the valid game board.
                  Player is not at an edge facing outwards (to prevent out-of-bounds access).
    @param nTableSize The dimension (n) of the n x n game board.
    @param gameArray A pointer to the 1D character array representing the game board.
    @param player A pointer to the LRRH struct containing the player's position and direction.
    @return void
*/
void sense(int nTableSize, char *gameArray, LRRH *player) {
    int changedPosition;
    char sensedObject; 

    // Direction & Object Checking
    switch(player->playerDirection) {
        case RIGHT:
            changedPosition = player->xPos + 1;
            sensedObject = gameArray[player->yPos * nTableSize + changedPosition];
            break;
        case DOWN:
            changedPosition = player->yPos+1;
            sensedObject = gameArray[changedPosition * nTableSize + player->xPos];
            break;
        case LEFT:
            changedPosition = player->xPos-1;
            sensedObject = gameArray[player->yPos * nTableSize + changedPosition];
            break;
        case UP:
            changedPosition = player->yPos-1;
            sensedObject = gameArray[changedPosition * nTableSize + player->xPos];
            break;
    }

    // Return Sensed Object
    switch(sensedObject) {
        case '*':   
            printf("\nYou sense nothing...\n");
            break;
        case '_':
            printf("\nYou sense a cold breeze...\n");
            break;
        case '&':
            printf("\nYou sense a fragrant smell...\n");
            break;
        case 'B':
            printf("\nYou sense heat...\n");
            break;
        case 'W':
            printf("\nYou sense a howl in the distance...\n");
            break;
        case 'P':
            printf("\nYou sense a sound in the distance...\n");
            break;
        case 'G':
            printf("\nYou sense a faint light in the distance...\n");
            break;

    }

}



int main() {
   
    // Game State

    int nGameSize;
    bool decisionState = false;

    // Ask for Game Size
    while(decisionState == false) {
        printf("Please Input Game Size (8-15 Only), game will be nxn big: ");
        scanf("%d", &nGameSize);

        if(nGameSize <= 15 && nGameSize >= 8) {
            decisionState = true;
        } else {
            printf("Invalid Game Size, Please Input Again...\n\n");
        }
    }
    
    // Make Game Board
    
    char gameArray[nGameSize][nGameSize];
    for (int i = 0;  i<nGameSize; i++) {
        for(int j=0; j<nGameSize; j++ ) {
            gameArray[i][j] = '*';
        }
    }

    // Populate Game Board

    // Generate character
    LRRH player;
    player.xPos = 0;
    player.yPos = 0;
    player.dirChar = '>';
    player.playerDirection = RIGHT;
    player.hasWoodsman = false;
    player.hasFlower = false;
    player.hasBread = false;

    // Place character before asking for object to prevent users placing objects in (1,1)
    gameArray[player.yPos][player.xPos] = player.dirChar;
    

    // Ask & Generate Objects
    // Multiple Objects
    askToGenerate("Pit", '_', nGameSize, (char*)gameArray, true);
    askToGenerate("Flower", 'F', nGameSize, (char*)gameArray, true);
    askToGenerate("Bakeshop", 'B', nGameSize, (char*)gameArray, true);

    // Single Objects
    askToGenerate("Wolf", 'W', nGameSize, (char*)gameArray, false);
    askToGenerate("Woodsman", 'P', nGameSize, (char*)gameArray, false);
    askToGenerate("Granny", 'G', nGameSize, (char*)gameArray, false);

    // Look for Granny & Save Her Position
    Granny granny;
    for (int i = 0;  i<nGameSize; i++) {
        for(int j=0; j<nGameSize; j++ ) {
            if(gameArray[i][j] == 'G') {
                granny.xPos = j;
                granny.yPos = i;
            }
        }
    }
    // Make Counter Variables
    int numberOfRotates = 0;
    int numberOfMoves = 0;
    int numberOfSense = 0;

    // Make Variable to Save Removed Character/Object
    char savedObject = '*';
    
    /*Game loop will mainly consist of 
        ACTION PHASE (Where all the logic is executed)
        DECISION (Where the player will input what they want to do, actions will happen after the loop goes back to Action Phase)
    */

    // Main Game Loop
    bool running = true;
    while(running == true) {
        char actionDecision;

        // Game Logic, In this Phase Game will Update
        system("cls");
        switch(savedObject) {
            case 'F':
                player.hasFlower = true;
                printf("----------------------------------\n\n\nYou Picked up Some Flowers!\n\n\n----------------------------------\n");
                Sleep(5000);
                break; 
            case '_':
                savedObject = '*';
                running = false;
                printf("----------------------------------\n\n\nYOU HAVE DIED\nYou have Fallen into a Pit!\n\n\n----------------------------------\n");
                break;
            case 'W':
                if(player.hasBread == true) {
                    printf("----------------------------------\n\n\nYou encountered the wolf\nthough since you have bread,\nyou fed it to the wolf and he let you pass\n\n\n----------------------------------\n");
                    player.hasBread = false;
                    Sleep(5000);
                }else {
                    printf("----------------------------------\n\n\nYOU HAVE DIED\nYou have been eaten by the Wolf!!\n\n\n----------------------------------\n");
                    running = false;
                }
                break;
            case 'B':
                player.hasBread = true;
                printf("----------------------------------\n\n\nYou Bought a Piece of Bread!\n\n\n----------------------------------\n");
                Sleep(5000);
                break;
            case 'G':
                if(player.hasBread && player.hasFlower && player.hasWoodsman) {
                    printf("----------------------------------\n\n\nYOU WIN!\nYou have safely reached Granny with a flower, a piece of bread, and the protection of the woodsman.\nWith these you have succesfully healed your Granny while the woodsman protected you!\n\n\n----------------------------------\n");
                    running = false;
                }else if(player.hasBread == false || player.hasFlower == false) {
                    printf("----------------------------------\n\n\nYOU HAVE DIED\nEven with the protection of the woodsman,\ngranny still fell weak and you, the woodsman, and granny was eaten by the wolf\n\n\n----------------------------------\n");
                    running = false;
                }else if(player.hasWoodsman == false) {
                    printf("----------------------------------\n\n\nYOU HAVE DIED\nWith all your efforts, it wasn't enough, without protection,\nyou and granny got eaten by the wolf\n\n\n----------------------------------\n");
                    running = false;
                }
                break;
            case 'P':
                savedObject = '*';
                player.hasWoodsman = true;
                granny.stepsAway = abs((player.xPos - granny.xPos) + (player.yPos - granny.yPos));
                printf("----------------------------------\n\n\nYou encountered the woodsman\nyou asked him for help and agreed, \nhe says that he recalls that granny is %d steps away\n\n\n----------------------------------\n", granny.stepsAway);
                Sleep(5000);
                break;

        }
        
        if(running == true) {
            system("cls");
            // Place Character
            gameArray[player.yPos][player.xPos] = player.dirChar;

            // Print Game Board
            printf("----------------------------------\n");
            printGameBoard(nGameSize, (char*)gameArray);

            // Counter Dashboard
            printf("----------------------------------\nCounter Dashboard\n");
            printf("Movements: %d\n", numberOfMoves);
            printf("Rotates: %d\n", numberOfRotates);
            printf("Senses: %d\n", numberOfSense);
            printf("----------------------------------\n");

            // Display Rotation
            switch(player.playerDirection) {  
                case RIGHT:
                    printf("You are Currently Facing Right.\n");
                    break;
                case DOWN:
                    printf("You are Currently Facing Down.\n");
                    break;
                case LEFT:
                    printf("You are Currently Facing Left.\n");
                    break;
                case UP:
                    printf("You are Currently Facing Up.\n");
                    break;
            }

            // Display Coordinates
            printf("You are currently at (%d,%d)\n",player.xPos + 1, player.yPos + 1);
            printf("----------------------------------\n");
            printf("INVENTORY:\nHasBread: %s\nHasFlower: %s\nWithWoodsman: %s\n", player.hasBread?"true":"false", player.hasFlower?"true":"false",player.hasWoodsman?"true":"false");
            printf("----------------------------------\n");
            printf("ACTIONS:\nw - move forward\nr - rotate\ns - sense\nq - quit\n");
            printf("----------------------------------\n");
            printf("What Action Do You Want To Take?: ");
            actionDecision = getch();

            switch (actionDecision) {
            case 'w':
                // Set Previous Position to saved Object
                gameArray[player.yPos][player.xPos] = savedObject;

                // Move Forward
                moveForward(&player, &savedObject, (char*) gameArray, nGameSize);
                numberOfMoves++;
                break;
            case 'r':
                rotate(&player);
                numberOfRotates++;
                break;
            case 's':
                sense(nGameSize, (char*) gameArray, &player);
                numberOfSense++;
                break;
            case 'q':
                running=false;
                break;
            default:
                printf("Invalid Response\n");
                break;
            }
        }
        
    }

    // Win/Lose Screen

    return 0;
}

