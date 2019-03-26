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

//boardState represents the number of strings of 2, 3, and 4 pieces for each player
//and the number of 2 and 3 strings that were blocked by the opposing player
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
    for (int i = 0; i < 8; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            centralPiece = board[i][n];
            if(centralPiece == 1 || centralPiece == 2)
            {
                //TODO: Add to board state array according to pieces around centralPiece
            }
        }
    }
}

int AiMove()
{
    //TODO: Algorithm that makes move to block opponent from winning, or 
    //maximizes beneficial impact on board state array according to 
    //weighted value of 2, 3, blocked 2, and blocked 3 lines
}
