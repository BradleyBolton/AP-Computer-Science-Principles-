/* Bradley Bolton
 * Connect4.c
 * This program allows the user to play connect 4 against another user or the computer.
 * The AI will evaluate one move in advance, and prioritize building multiple strings of pieces while blocking the opponents when an immeadiate victory is not possible.
 * Last worked on 4/4/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h> //The CS50 Library is used for its getInt function, which is used for the human player to indicate which move they will make.
//This library has been written and distributed by Harvard University's CS50 course

void generateNewBoard();
void drawBoard();
void placePiece(int collumnChoice, bool player);
void removePiece(int collumnChoice);
int AiMove();
void checkBoardState();
void printBoardState(); //debug function
void initializeAi();


int board[8][8];

//boardState represents the number of strings of 2, 3, and 4 pieces for each player, and blocked strings
//indexs 0 and 1 are two length lines, 2 and 3 are three length lines, 4 and 5 are four length lines, 6 and 7 are blocked 2 length lines, 8 and 9 are blocked 3 length lines
int boardState[10];
int moveWeight[5];
int previousAiMove;

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
    generateNewBoard();
    drawBoard();
    initializeAi();
    bool player = true; //used to track which player is making their move
    int collumnChoice;
    do
    {
        if (player)
        {
            printf("Player X, select the collumn to drop your piece.\n");
            collumnChoice = get_int() - 1;
        }
        else if (playerCount == 2)
        {
            printf("Player O, select the collumn to drop your piece.\n");
            collumnChoice = get_int() - 1;
        }
        else
        {
            collumnChoice = AiMove();
        }
        while (collumnChoice > 7 || collumnChoice < 0)
        {
            printf("Invalid collumn choice. Please retry\n");
            collumnChoice = get_int() - 1;
        }
        if (board[0][collumnChoice] == 0) //checks that collumn can have a piece dropped in it
        {
            placePiece(collumnChoice, player);
            player = !player; //swaps who's turn it is
            drawBoard();
            checkBoardState();
            if (boardState[4] > 0)
            {
                printf("Player X has won!\n");
                break;
            }
            if (boardState[5] > 0 || playerCount == 2)
            {
                printf("Player 0 has won!\n");
                break;
            }
            else if(boardState[5] > 0)
            {
                printf("The AI has won!\n");
                break;
            }
            //checkBoardState(); //debug tool
            //printBoardState(); //debug tool
        }
        else
        {
            if (player)
            {
                printf("Invalid move.\n");
            }
        }
    } while(true);
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
    printf(" 1   2   3   4   5   6   7   8\n"); //marks collumn choices
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

void placePiece(int collumnChoice, bool player)
{
    int i = 0;
    while (board[i + 1][collumnChoice] == 0 && i < 7) //determines proper row for piece to be placed in collumn
    {
        i++;
    }
    if (player)
    {
        board[i][collumnChoice] = 1; //sets player 1 piece
    }
    else
    {
        board[i][collumnChoice] = 2; //sets player 2 or AI piece
    }
}

void removePiece(int collumnChoice)
{
    int i = 0;
    while (board[i + 1][collumnChoice] == 0 && i < 7) //determines proper row for piece to be removed in collumn
    {
        i++;
    }
    board[i + 1][collumnChoice] = 0; //removes piece
}

void checkBoardState()
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
                                                boardState[3+centralPiece]++; //records boardstate of 4 in string
                                            }
                                            else
                                            {
                                                boardState[1+centralPiece]++; //records boardstate of 3 string
                                            }
                                            if(board[i+j*3][n+k*3] > 0) //checks for block at 4th point in string
                                            {
                                                boardState[7+centralPiece]++; //records boardstate of blocked 3 string
                                            }
                                        }
                                    }
                                    else
                                    {
                                        boardState[-1+centralPiece]++; //records boardstate of 2 string
                                    }
                                    if(board[i+j*2][n+k*2] > 0) //checks for block at 3rd point in string
                                    {
                                        boardState[5+centralPiece]++; //records boardstate of blocked 2 string
                                    }
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
    int baseState[10]; //Will serve as a comparison point
    int moveValue[8]; //Will be used to compare the value of each possible move
    int minValue = 1000; //Will represent the weighted value of the AI's move being considered
    for (int i = 0; i < 10; i++)
    {
    baseState[i] = boardState[i]; //Copies the board state before the Ai's move
    }
    for (int i = 0; i < 8; i++)
    {
        if (board[0][i] == 0)
        {
        //printf("Considering collumn %d.\n", i); //debug tool
        placePiece(i, false); //places theoretical piece to examine result on boardstate
        if (boardState[5] > 0)
        {
            //printf("One move win detected, playing it.\n"); //debug tool
            removePiece(i); //removes theoretical piece
            return i;
        }
        for (int n = 0; n < 8; n++)
        {
            if (board[0][n] == 0)
            {
                int exchangeValue = 0;
                placePiece(n, true);
                checkBoardState();
                removePiece(n); //This line and the 2 above it create a theoretical board state to examine
                for (int k = 0; k < 6; k += 2)
                {
                        exchangeValue += (boardState[k + 1] - baseState[k + 1]) * moveWeight[k / 2];
                        exchangeValue -= (boardState[k] - baseState[k]) * moveWeight[k / 2];
                }
                if (exchangeValue < minValue)
                {
                    minValue = exchangeValue;
                }
                //printf("%d-%d exchange has a value of %d.\n", i, n, exchangeValue); //debug tool
            }
        }
        moveValue[i] = minValue;
        //printf("minValue for collumn %d evaluated to %d.\n", i, minValue); //debug tool
        minValue = 1000;
        removePiece(i); //removes theoretical piece
        }
    }
    int bestMove = 0;
    for (int i = 0; i < 8; i++)
    {
        if ((moveValue[i] >= moveValue[bestMove] && i != previousAiMove) || moveValue[i] > moveValue[bestMove])
        {
            bestMove = i;
        }
    }
    previousAiMove = bestMove;
    return bestMove;
}

void initializeAi()
{
    moveWeight[0] = 1;
    moveWeight[1] = 12;
    moveWeight[2] = 10000;
    moveWeight[3] = 2;
    moveWeight[4] = 10000;
}
