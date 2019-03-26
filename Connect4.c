/* Bradley Bolton
 * Connect4.c
 * This program allows the user to play connect 4 against another user or the computer
 * Last worked on 2/20/2019
 */

#include <stdio.h>
#include <cs50.h>

int generateNewBoard();
int drawBoard();
int placePiece(int rowChoice, bool player);
int AiMove();
void checkBoardState();


int board[8][8];

//boardState represents the number of strings of 2, 3, and 4 pieces for each player, and blocked strings
//indexs 0 and 1 are two length lines, 2 and 3 are three length lines, 4 and 5 are four length lines, 6 and 7 are blocked 2 length lines, 8 and 9 are blocked 3 length lines
int boardState[10];

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
    bool player = true;
    int rowChoice;
    do
    {
        if (player)
        {
            printf("Player X, select the row to drop your piece.\n");
            rowChoice = get_int() - 1;
        }
        else if (playerCount == 2)
        {
            printf("Player O, select the row to drop your piece.\n");
            rowChoice = get_int() - 1;
        }
        else
        {
            rowChoice = AiMove();
        }
        if (board[0][rowChoice] == 0)
        {
            placePiece(rowChoice, player);
            player = !player;
            drawBoard();
        }
        else
        {
            printf("Invalid move.\n");
        }
    } while(true);

}

int generateNewBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            board[i][n] = 0;
        }
    }
    return 0;
}

int drawBoard()
{
    printf(" 1   2   3   4   5   6   7   8\n");
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
                printf("[X] ");
                break;
            case 2:
                printf("[O] ");
                break;
            }
        }
        printf("\n");
    }
    return 0;
}

int placePiece(int rowChoice, bool player)
{
    int i = 0;
    while (board[i + 1][rowChoice] == 0 && i < 7)
    {
        i++;
    }
    if (player)
    {
        board[i][rowChoice] = 1;
    }
    else
    {
        board[i][rowChoice] = 2;
    }
    return 0;
}

void checkBoardState()
{
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
                        if(board[i+j][n+k] == centralPiece && (j != 0 || k != 0)) //checks for 2nd in string
                        {
                            if(board[i+j*2][n+k*2] == centralPiece) //checks for 3rd in string
                            {
                                if(board[i+j*3][n+k*3] == centralPiece) //checks for 4th in string
                                {
                                   boardState[3+centralPiece]++; //records boardstate of 4 in string
                                }
                                else if(board[i+j*3][n+k*3] > 0) //checks for block at 4th point in string
                                {
                                    boardState[7+centralPiece]++; //records boardstate of blocked 3 string
                                }
                                else
                                {
                                    boardState[1+centralPiece]++; //records boardstate of unblocked 3 string
                                }
                            }
                            else if(board[i+j*2][n+k*2] > 0) //checks for block at 3rd point in string
                            {
                                boardState[5+centralPiece]++; //records boardstate of blocked 2 string
                            }
                            else
                            {
                                boardState[-1+centralPiece]++; //records boardstate of unblocked 2 string
                            }
                        }
                    }
                }
            }
        }
    }
}

int AiMove()
{
    //TODO: Algorithm that makes move to block opponent from winning, or 
    //maximizes beneficial impact on board state array according to 
    //weighted value of 2, 3, blocked 2, and blocked 3 lines
    return 1;
}
