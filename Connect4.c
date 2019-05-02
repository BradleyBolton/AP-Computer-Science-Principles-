/* Connect4.c
 * This program allows the user to play connect 4 against another user or the computer.
 * The AI will evaluate one move in advance, and prioritize building multiple strings of pieces while blocking the opponents when an immeadiate victory is not possible.
 * Last worked on 5/1/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h> //The CS50 Library is used for its getInt function, which is used for the human player to indicate which move they will make.
//This library has been written and distributed by Harvard University's CS50 course

void generateNewBoard();
void drawBoard();
void placePiece(int columnChoice, bool player);
void removePiece(int columnChoice);
int AiMove();
void checkBoardState(); //allows AI to evaluate board
void printBoardState(); //debug function
void initializeAi(); //generates a set of weighted values for AI to evaluate board

int board[8][8];

//boardState represents the number of strings of 2, 3, and 4 pieces for each player, and blocked strings
//indexes 0, 2, 4, 6, and 8 are, in order, player 1's lines of 2 pieces, lines of 3 pieces, lines of 4 pieces, blocked lines of opponents 2 pieces, and blocked lines of opponents 3 pieces
//indexes 1, 3, 5, 7, and 9 are, in order, player 2's lines of 2 pieces, lines of 3 pieces, lines of 4 pieces, blocked lines of opponents 2 pieces, and blocked lines of opponents 3 pieces
int boardState[10];
int moveWeight[5]; //allows for the value of a specific board state to be adjusted for varied AI decisions across games
int previousAiMove; //tracking the ai's previous move allows for it to player less vertically, which is generally a better strategy but not always reflected perfectly in the algorithm
int roundCounter; //tracks the number of moves the player has made
int baseState[10]; //serves as a comparison point for AI decision making
int moveValue[8]; //used to compare the value of each possible move for the AI
int minValue = 1000; //Will represent the weighted value of the AI's move being considered
int exchangeValue = 0; //Represents the value of a specific play and counter-play the AI may decide to engage in
int bestMove = 0; //Represents which column is the optimal play for the AI to make


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Proper Usage: ./Connect4 [# of human players]\n");
        return 1;
    }
    int playerCount = atoi(argv[1]);
    if (playerCount != 1 && playerCount != 2)
    {
        printf("Error: Only 1 or 2 players are supported in Connect4\n");
        return 1;
    }
    generateNewBoard(); //properly fills the array to describe the game board
    drawBoard(); //prints the game board
    initializeAi(); //creates the weighted values the AI uses to determine the value of a move
    roundCounter = 0; //counts how many rounds of turns the game has gone through
    bool player = true; //used to track which player is making their move
    int columnChoice; //represents the column a piece is being dropped in
    while (true)
    {
        if (player)
        {
            printf("Player X, select the column to drop your piece.\n");
            columnChoice = get_int() - 1;
            roundCounter++;
        }
        else if (playerCount == 2)
        {
            printf("Player O, select the column to drop your piece.\n");
            columnChoice = get_int() - 1;
        }
        else if (roundCounter == 1)
        {
            if (board[7][3] == 1)
            {
                columnChoice = 4;
            }
            else
            {
                columnChoice = 3;
            }
        }
        else
        {
            columnChoice = AiMove();
        }
        while ((columnChoice > 7 || columnChoice < 0) && board[0][columnChoice] != 0) //ensures valid input
        {
            printf("Invalid column choice. Please retry\n");
            columnChoice = get_int() - 1;
        }
        placePiece(columnChoice, player);
        player = !player; //swaps who's turn it is
        drawBoard();
        checkBoardState(); //used to determine if a player has won
        //printBoardState(); //debug tool to show how the AI sees the board
        if (boardState[4] > 0)
        {
            printf("Player X has won! It took %d rounds.\n", roundCounter);
            break;
        }
        if (boardState[5] > 0 && playerCount == 2)
        {
            printf("Player 0 has won! It took %d rounds.\n", roundCounter);
            break;
        }
        else if(boardState[5] > 0)
        {
            printf("The AI has won! It took %d rounds.\n", roundCounter);
            break;
        }
    }
    return 0;
}

void generateNewBoard() //sets all board locations to empty
{
    for (int i = 0; i < 8; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            board[i][n] = 0;
        }
    }
}

void drawBoard()
{
    printf(" 1   2   3   4   5   6   7   8\n"); //marks column choices
    for (int i = 0; i < 8; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            switch (board[i][n])
            {
            case 0:
                printf("[ ] ");
                break;
            case 1:
                printf("[X] "); //player one's piece
                break;
            case 2:
                printf("[O] "); //player two's piece
                break;
            }
        }
        printf("\n");
    }
}

void placePiece(int columnChoice, bool player)
{
    int i = 0;
    while (board[i + 1][columnChoice] == 0 && i < 7) //determines proper row for piece to be placed in column
    {
        i++;
        if (i == 7)
        {
            break;
        }
    }
    if (board[i][columnChoice] == 0) //redundant check that the spot on the board is unoccupied
    {
        if (player)
        {
            board[i][columnChoice] = 1; //sets player 1 piece
        }
        else
        {
            board[i][columnChoice] = 2; //sets player 2 or AI piece
        }
    }
}

void removePiece(int columnChoice) //used by AI to temporarily make theoretical moves to examine the result of a considered play
{
    int i = 0;
    while (board[i + 1][columnChoice] == 0 && i < 7) //determines proper row for piece to be removed in column
    {
        i++;
    }
    board[i + 1][columnChoice] = 0; //removes piece
}

void checkBoardState() //Fills out the array boardState[] to allow the AI to properly analyze potential moves
{
    for (int i = 0; i < 10; i++) //clears boardState
    {
        boardState[i] = 0;
    }
    int centralPiece;
    for (int i = 0; i < 8; i++) //iterates through board
    {
        for (int n = 0; n < 8; n++)
        {
            centralPiece = board[i][n];
            if(centralPiece == 1 || centralPiece == 2)
            {
                for (int j = -1; j < 2; j++) //iterates around piece
                {
                    for (int k = -1; k < 2; k++)
                    {
                        if (i + j > -1 && i + j < 8 && n + k > - 1 && n + k < 8) //ensures valid indexing
                        {
                            if(board[i+j][n+k] == centralPiece && (j != 0 || k != 0)) //checks for 2nd in string
                            {
                                if (i + j * 2 > -1 && i + j * 2 < 8 && n + k * 2 > - 1 && n + k * 2 < 8) //ensures valid indexing
                                {
                                    if(board[i+j*2][n+k*2] == centralPiece) //checks for 3rd in string
                                    {
                                        if (i + j * 3 > -1 && i + j * 3 < 8 && n + k * 3 > - 1 && n + k * 3 < 8) //ensures valid indexing
                                        {
                                            if(board[i+j*3][n+k*3] == centralPiece) //checks for 4th in string
                                            {
                                                boardState[3+centralPiece]++; //records boardstate of 4 string
                                            }
                                            else if (board[i+j*3][n+k*3] == 0) //checks for unblocked 3 string
                                            {
                                                boardState[1+centralPiece]++; //records boardstate of unblocked 3 string
                                            }
                                            else
                                            {
                                                boardState[8+centralPiece]++; //records boardstate of blocked 3 string
                                            }
                                        }
                                        else
                                        {
                                            boardState[8+centralPiece]++; //records boardstate of blocked 3 string (block from edge of game board)
                                        }
                                    }
                                    else if (board[i+j*2][n+k*2] == 0) //checks for unblocked 2 string
                                    {
                                        boardState[-1+centralPiece]++; //records boardstate of unblocked 2 string
                                    }
                                    else
                                    {
                                        boardState[6+centralPiece]++; //records boardstate of blocked 2 string
                                    }
                                }
                                else
                                {
                                    boardState[6+centralPiece]++; //records boardstate of blocked 2 string (block from edge of game board)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//debug tool
void printBoardState()
{
    for (int i = 0; i < 10; i++)
    {
        printf("%d", boardState[i]);
    }
    printf("\n");
}

int AiMove() //Algorithm to decide what move the AI will make
{
    checkBoardState();
    minValue = 32767; //Will represent the weighted value of the AI's move being considered
    for (int i = 0; i < 10; i++)
    {
    baseState[i] = boardState[i]; //Copies the board state before the Ai's move
    }
    for (int i = 0; i < 8; i++)
    {
        if (board[0][i] == 0)
        {
            //printf("Considering column %d.\n", i); //debug tool
            placePiece(i, false); //places theoretical piece to examine result on boardstate
            checkBoardState();
            if (boardState[5] > 0)
            {
                //printf("One move win detected, playing it.\n"); //debug tool to check if the AI knows it can win with a certain move
                removePiece(i); //removes theoretical piece
                printf("AI placed piece in column %d.\n", i + 1);
                return i;
            }
            for (int n = 0; n < 8; n++)
            {
                if (board[0][n] == 0)
                {
                    exchangeValue = 0;
                    placePiece(n, true);
                    checkBoardState();
                    removePiece(n); //This line and the 2 above it create a theoretical board state to examine
                    for (int k = 0; k < 8; k += 2)
                    {
                        exchangeValue += (boardState[k + 1] - baseState[k + 1]) * moveWeight[k / 2]; //adds the value of the changes in board state for the AI's pieces
                        exchangeValue -= (boardState[k] - baseState[k]) * moveWeight[k / 2]; //subtracts the value of the changes in board state for the opponent's pieces
                    }
                    if (exchangeValue < minValue)
                    {
                        minValue = exchangeValue;
                    }
                    //printf("%d-%d exchange has a value of %d.\n", i, n, exchangeValue); //debug tool
                }
            }
            moveValue[i] = minValue;
            //printf("minValue for column %d evaluated to %d.\n", i, minValue); //debug tool
            minValue = 1000;
            removePiece(i); //removes theoretical piece
        }
    }
    bestMove = 0;
    for (int i = 0; i < 8; i++) //determines which column has the best worst case value for the AI to play in
    {
        if ((moveValue[i] >= moveValue[bestMove] && i != previousAiMove) || moveValue[i] > moveValue[bestMove])
        {
            bestMove = i;
        }
    }
    previousAiMove = bestMove;
    printf("AI placed piece in column %d.\n", bestMove + 1);
    return bestMove;
}

void initializeAi() //Initializes an AI that values certain board states in varied ways
{
    moveWeight[0] = 1 + (rand() % 3); //Value of having 2 pieces in a line
    moveWeight[1] = 7 + (rand() % 5); //Value of having 3 pieces in a line
    moveWeight[2] = 10000; //Value of having 4 pieces in a line (Extremely high as this is the game's win condition)
    moveWeight[3] = 5 + (rand() % 4); //Value of blocking an opponent's 2 piece line
    moveWeight[4] = 100; //Value of blocking an opponent's 3 piece line
}
