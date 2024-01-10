/***************************************************
 * 2-D Board Game between computer and User.
 *
 * Code intended to run on Windows OS
****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>

// Constant data.
#define PLAYER_ONE 'X'
#define PLAYER_TWO 'O'

// The board is a 7x7 square.
// Number of squares that make up that side.
#define SIDE 7

/*Implementation of the getline() function that is available in Linux/Unix */
size_t getline(char** buffer, size_t* alloc, FILE* file)
{
    // To prevent frequent allocations, we first allocate 32 bytes.
    if (*alloc == 0 || (*buffer) == NULL)
    {
        *buffer = (char*)malloc(32 * sizeof(char));
        *alloc = 32;
    }
    int bytes = 0;
    while (1)
    {
        char c = fgetc(file);
        // The character is pushed into the buffer.
        if (bytes < (*alloc))
        {
            (*buffer)[bytes++] = c;
        }

        if(bytes == (*alloc))
        {
            *buffer = (char*)realloc(*buffer, (*alloc) + 32);
            (*alloc) += 32;
        }

        if (c == '\n')
        {
            // Newline character.
            break;
        }
    }
    // Including the null terminator.
    (*buffer)[bytes] = '\0';
    return (bytes - 1);
}


// Implementation of the function that initializes the board game.
void initializeBoard(char** board, int player_pieces)
{
    // The array of players.
    char players[2] = { PLAYER_ONE, PLAYER_TWO };
    // Count of players. (loop counter)
    int c = 0;
    // First we set every character to NULL byte.
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            board[i][j] = 0;
        }
    }
    while (c < 2)
    {
        // The number of player pieces placed for this player.
        int p = 0;
        while (p < player_pieces)
        {
            // This piece is placed in a random location.
            int i = rand() % SIDE;
            int j = rand() % SIDE;
            if (board[i][j] == 0)
            {
                // The slot is vacant.
                // We place the piece.
                board[i][j] = players[c];
                // Successfully placed a player.
                p++;
            }
        }
        c++;
    }
}

// Implementation of the function that prints the board to the terminal.
void printBoard(char** board)
{
    // Printing the column numbers.
    printf("  ");
    for (int j = 0; j < SIDE; j++)
    {
        printf("%2c ", '0' + j);
    }
    // The board rows follow next line.
    printf("\n");

    for (int i = 0; i < SIDE; i++)
    {
        // The row indicator.
        // Because of ASCII encoding we get the row indicating letters in order.
        printf("%-2c", 'a' + i);
        for (int j = 0; j < SIDE; j++)
        {
            if (board[i][j] != 0)
            {
                printf("%2c ", board[i][j]);
            }
            else {
                printf("%2c ", ' ');
            }
        }
        printf("\n");
    }
}

/* Implementation of the function to check if a chosen player position is valid. */
// The first arg is the board 2D array.
// The second arg is the player symbol.
// The third arg is the position entered in string form.
int isChosenPositionValid(char** board, char player_sym, char* pos)
{
    int i = tolower(pos[0]) - 'a';
    int j = tolower(pos[1]) - '0';
    if (i < 0 || i >= SIDE)
    {
        // Invalid row.
        return 0;
    }
    if (j < 0 || j >= SIDE)
    {
        // Invalid column.
        return 0;
    }
    if (board[i][j] != player_sym)
    {
        // This position is either empty or occupied by another player.
        // This is invalid.
        return 0;
    }
    // The chosen piece position is valid.
    return 1;
}

// Function that helps in free()-ing a NULL pointer terminated array of strings.
void freeArray(char** list)
{
    if (!list)
    {
        return;
    }
    // Traversing the list of strings.
    for (char** l = list; *l != NULL; l++)
    {
        free(*l);
    }
    free(list);
}

// Function that helps in counting the strings inside a null terminated array of strings.
int countPosStrings(char** list)
{
    int count = 0;
    // Traversing the list of strings.
    for (char** l = list; *l != NULL; l++)
    {
        count++;
    }
    return count;
}


// Function that returns an array of strings representing the positions of a player.
char** getPlayerPositions(char** board, char player_sym)
{
    int alloc = 1;
    char** list = (char**)malloc(sizeof(char*));
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (board[i][j] == player_sym)
            {
                // The position of the player is stored in a char*
                // and its pointer is stored in the list that will be returned.
                list[alloc - 1] = (char*)malloc(sizeof(char) * 3);
                // Row indicator character.
                list[alloc - 1][0] = 'a' + i;
                // Colum indicating numerical character.
                list[alloc - 1][1] = '0' + j;
                // The null terminator for this string.
                list[alloc - 1][2] = 0;

                // Next we grow list, so it can accomadate a future char*.
                list = (char**)realloc(list, sizeof(char*) * (alloc + 1));
                alloc++;
            }
        }
    }
    // The last string is set to NULL, so that the end can be marked.
    list[alloc - 1] = NULL;
    return list;
}


// Function that returns an array of strings representing the possible positions a player can move.
// The array of string is NULL terminated.
char** getPlayerValidMoves(char** board, char player_sym)
{
    int alloc = 1;
    char** list = (char**)malloc(sizeof(char*));
    // List of player positions.
    char** pos = getPlayerPositions(board, player_sym);
    // Iterating through the positions of our player.
    for (char** p = pos; *p != NULL; p++)
    {
        // The position string in <row indicator><col indicator> format.
        char* s = *p;
        // We check the neighbourhood of this position to find all possible moves.
        // The player position in integer coordinates.
        int i = tolower(s[0]) - 'a';
        int j = tolower(s[1]) - '0';

        // Since, the player can move only horizontally or vertically there is no need of nested loop here.
        // We check vertically possible positions.
        for (int r = (i - 1); r <= (i + 1); r++)
        {
            if (r < 0 || r >= SIDE)
            {
                // Invalid row.
                continue;
            }
            if (board[r][j] == 0)
            {
                // Vacant place for the piece.
                // The position we found is vacant and hence its a valid move for the player.
                list[alloc - 1] = (char*)malloc(sizeof(char) * 3);
                // Row indicator character.
                list[alloc - 1][0] = 'a' + r;
                // Colum indicating numerical character.
                list[alloc - 1][1] = '0' + j;
                // The null terminator for this string.
                list[alloc - 1][2] = 0;

                // Next we grow list, so it can accomadate a future char*.
                list = (char**)realloc(list, sizeof(char*) * (alloc + 1));
                alloc++;
            }
        }

        // Next we check horizontally possible directions.
        for (int c = (j - 1); c <= (j + 1); c++)
        {
            if (c < 0 || c >= SIDE)
            {
                // Invalid row.
                continue;
            }
            if (board[i][c] == 0)
            {
                // Vacant place for the piece.
                // The position we found is vacant and hence its a valid move for the player.
                list[alloc - 1] = (char*)malloc(sizeof(char) * 3);
                // Row indicator character.
                list[alloc - 1][0] = 'a' + i;
                // Colum indicating numerical character.
                list[alloc - 1][1] = '0' + c;
                // The null terminator for this string.
                list[alloc - 1][2] = 0;

                // Next we grow list, so it can accomadate a future char*.
                list = (char**)realloc(list, sizeof(char*) * (alloc + 1));
                alloc++;
            }
        }

    }
    // The last string pointer is set to NULL, so that the end can be marked.
    list[alloc - 1] = NULL;
    return list;
}


// Function that returns the number of valid moves
int countPlayerValidMoves(char** board, char player_sym)
{
    char** list = getPlayerValidMoves(board, player_sym);
    int ret = countPosStrings(list);
    freeArray(list);
    return ret;
}

/*
 * Implementation of the function to check if the player chose a valid move.
 * It needs to know the piece position.
*/
int isPlayerMoveValid(char** board, char player_sym, char* piece_pos, char* player_move)
{
    // The present position as integers.
    int i = tolower(piece_pos[0]) - 'a';
    int j = tolower(piece_pos[1]) - '0';

    // The player's move as integers.
    int row = tolower(player_move[0]) - 'a';
    int col = tolower(player_move[1]) - '0';

    // Identity moves are not allowed.
    if (row == i && col == j)
    {
        printf("ERROR: Move cannot be same as the chosen piece position. \n");
        return 0;
    }

    /* We check if the row and column of the move is within range. */
    if (row < 0 || row >= SIDE)
    {
        printf("ERROR: Row of move is out of bound. \n");
        return 0;
    }

    if (col < 0 || col >= SIDE)
    {
        printf("ERROR: Column of move is out of bound. \n");
        return 0;
    }

    // Since the player piece can move horizontally or vertically only,
    // Diagonal moves are invalid.
    if ((row == (i - 1) && col == (j - 1)) || (row == (i + 1) && col == (j + 1))
        || (row == (i - 1) && col == (j + 1)) || (row == (i + 1) && col == (j - 1)))
    {
        printf("ERROR: Diagonal moves are NOT allowed. \n");
        return 0;
    }

    // Next we check to see if the chosen position is vacant.
    if (board[row][col] != 0)
    {
        printf("ERROR: Chosen move position is already occupied! \n");
        return 0;
    }
    // It went through all challenges!
    // The move is valid.
    return 1;
}

// Function that returns a list of valid moves for a player piece at provided position string
char** getPlayerPieceValidMoves(char** board, char* piece_pos)
{
    // If there is no piece at the provided position we return NULL.
    if (!piece_pos)
    {
        return NULL;
    }
    // The position in integer form.
    int row = tolower(piece_pos[0]) - 'a';
    int col = tolower(piece_pos[1]) - '0';

    int alloc = 1;
    char** list = (char**)malloc(sizeof(char*));

    // Testing which all vertical moves are valid.
    for (int r = (row - 1); r <= (row + 1); r++)
    {
        if (r < 0 || r >= SIDE)
        {
            // Invalid row.
            continue;
        }
        if (board[r][col] == 0)
        {
            // The position is vacant.
            list[alloc - 1] = (char*)malloc(sizeof(char) * 3);
            // Row indicator character.
            list[alloc - 1][0] = 'a' + r;
            // Colum indicating numerical character.
            list[alloc - 1][1] = '0' + col;
            // The null terminator for this string.
            list[alloc - 1][2] = 0;
            // Next we grow list, so it can accomadate a future char*.
            list = (char**)realloc(list, sizeof(char*) * (alloc + 1));
            alloc++;
        }
    }

    // Testing which all horizontal moves are valid.
    for (int c = (col - 1); c <= (col + 1); c++)
    {
        if (c < 0 || c >= SIDE)
        {
            // Invalid row.
            continue;
        }
        if (board[row][c] == 0)
        {
            // The position is vacant.
            list[alloc - 1] = (char*)malloc(sizeof(char) * 3);
            // Row indicator character.
            list[alloc - 1][0] = 'a' + row;
            // Colum indicating numerical character.
            list[alloc - 1][1] = '0' + c;
            // The null terminator for this string.
            list[alloc - 1][2] = 0;
            // Next we grow list, so it can accomadate a future char*.
            list = (char**)realloc(list, sizeof(char*) * (alloc + 1));
            alloc++;
        }
    }

    // Placing NULL as marker for the end of the array.
    list[alloc - 1] = NULL;
    return list;
}

// Function to check if game is over.
int isGameOver(char** board, int turn_user, int computer_first)
{
    // The array of player symbols.
    char player_symbol[2] = { PLAYER_ONE, PLAYER_TWO };
    // The symbol of the player that is going to play the next turn.
    char player_sym = 0;
    if (turn_user)
    {
        // The user is the one who is going to play next turn.
        player_sym = player_symbol[computer_first];
    }
    else {
        player_sym = player_symbol[!computer_first];
    }

    // We find all the valid possible moves for the player.
    char** all_valid_moves = getPlayerValidMoves(board, player_sym);
    if (countPosStrings(all_valid_moves) == 0)
    {
        // There are no valid moves.
        // Game over.
        freeArray(all_valid_moves);
        return 1;
    }

    freeArray(all_valid_moves);
    // Game is not over, and the player can make a turn.
    return 0;
}


// Function that calculates and displays the heuristic score for the game board.
void calculateHeuristicScore(char** board)
{
    // Counting the valid moves possible for 'X'
    int count_x = countPlayerValidMoves(board, PLAYER_ONE);
    int count_o = countPlayerValidMoves(board, PLAYER_TWO);
    int score = count_x - count_o;
    printf("Heuristic score for the board state: %d\n", score);

    if (score < 0)
    {
        // The score is negative, implies the board state is favourable for player 'O'
        printf("NOTE: Score indicates board state is more favourable for player 'O' \n");
    }
    else if (score > 0)
    {
        // A positive score implies the board state is more favourable for player 'X'
        printf("NOTE: Score indicates board state is more favourable for player 'X' \n");
    }
    else {
        // Zero score implies that the board state is favourable for both players.
        printf("NOTE: Score indicates board state is favourable for both players! \n");
    }
}

// Main entry point of our application.
int main(int argc, char** argv)
{
    // Game title.
    printf("\t ******** 2D Board Game Between User & Computer ******** \n");

    // The buffer to store user input.
    char* input = NULL;
    size_t alloc = 0;
    // Flag to detect the turn of the user.
    int turn_user = 0;
    // Flag to detect if the computer is the first player.
    int computer_first = 1;
    // The number of pieces per player.
    int player_pieces = 0;
    // The number of turns in the game.
    int turns = 0;
    // The array of player symbols.
    char player_symbol[2] = { PLAYER_ONE, PLAYER_TWO };
    // The turn count. (Variable to keep track of the turn of the game)
    int turn_count = 0;
    // Flag to detect if game is over.
    int game_over = 0;

    // Seeding the random number generator.
    srand(time(NULL));


    // The 2D array representing the game board is allocated.
    char** board = (char**)malloc(SIDE * sizeof(char*));
    for (int i = 0; i < SIDE; i++)
    {
        board[i] = (char*)malloc(SIDE);
    }
    // Asking the user, whether they wanna be the first player.
    while (1)
    {
        printf("Welcome dear user, \nDo you want to be the first player 'X' (first player) or player 'O' (second player) ? (X/O)\n", player_symbol[0]);
        getline(&input, &alloc, stdin);
        if (strlen(input) == 1)
        {
            continue;
        }
        else {
          if (strcasecmp(input, "X\n") == 0 || strcasecmp(input, "x\n") == 0)
          {
            // The user wants to be the first player.
            turn_user = 1;
            // The computer is not the first player.
            computer_first = 0;
            break;
          }else{
            if(strcasecmp(input, "O\n") != 0 && strcasecmp(input, "o\n") != 0)
            {
                printf("ERROR: Please provide with 'X' or 'O' as answer! \n");
                continue;
            }else{
                break;
            }
          }
        }
    }

    // Now ask the user the number of pieces per player.
    while (1)
    {
        printf("Please, provide the number of pieces per player: \n");
        getline(&input, &alloc, stdin);
        if (strlen(input) == 1)
        {
            continue;
        }
        else {
            player_pieces = atoi(input);
            if (player_pieces == 0)
            {
                printf("Oops! The number of pieces cannot be zero! Please try again!\n");
                continue;
            }

            if (player_pieces * 2 > (SIDE * SIDE))
            {
                printf("Oops! Thats too many pieces! Please try again with any positive number less than: %d\n", (SIDE * SIDE) / 2);
                continue;
            }
            break;
        }
    }

    // The board is initailized randomly.
    initializeBoard(board, player_pieces);
    /* Next, we need to accept the number of terms from the user. */
    while (1)
    {
        printf("Please, provide the number of turns: \n");
        getline(&input, &alloc, stdin);
        if (strlen(input) == 1)
        {
            continue;
        }
        else {
            if (atoi(input) == 0)
            {
                printf("Number of turns cannot be zero. Please try again!\n");
                continue;
            }
            else {
                turns = atoi(input);
                break;
            }
            break;
        }
    }

    /* We have received the parameters from the user. */

    // We enter into the game loop.
    while (turn_count < turns)
    {
        // First we need to check if the game is over.
        game_over = isGameOver(board, turn_count, computer_first);
        if (game_over)
        {
            break;
        }
        // The heading of the present turn we are in.
        printf("********** TURN: %d ***********\n", turn_count + 1);
        // We print the board to the terminal.
        printBoard(board);
        printf("\n");
        // We calculate and display the heuristic score for the present board state.
        calculateHeuristicScore(board);
        printf("\n");
        if (turn_user)
        {
            printf("\n* PLAYER %c's turn *\n\n", player_symbol[computer_first]);
            // If it is the user's turn we ask the user to choose their piece, via providing the position.
            // The loop runs till the user provides a valid position.
            // Array to save the player's chosen piece position.
            char player_pos[3] = { 0, 0, 0 };
            while (1)
            {
                printf("Dear Player '%c', please enter a piece position you wish to move: ", player_symbol[computer_first]);
                getline(&input, &alloc, stdin);
                // getline() inserts the new line character.
                input[strlen(input) - 1] = 0;
                if (strlen(input) != 2)
                {
                    printf("Please enter the choice in <row symbol><column number> format, without angle brackets or spaces. \n");
                    continue;
                }

                // Now we check the validity of the chosen position.
                if (!isChosenPositionValid(board, player_symbol[computer_first], input))
                {
                    printf("Oops! Chosen position is unfortunately, invalid. Please try again!\n");
                }
                else {
                    memcpy(player_pos, input, 2);
                    break;
                }
            }

            // Next we ask the user for a valid move.
            while (1)
            {
                printf("Dear Player '%c', please enter your new move: ", player_symbol[computer_first]);
                getline(&input, &alloc, stdin);
                // getline() inserts the new line character.
                input[strlen(input) - 1] = 0;
                if (strlen(input) != 2)
                {
                    printf("Please enter the new move in <row symbol><column number> format, without angle brackets or spaces. \n");
                    continue;
                }

                // Now we check if the player chose a legal move.
                if (!isPlayerMoveValid(board, player_symbol[computer_first], player_pos, input))
                {
                    printf("Oops! That was an invalid move! Please try again!\n");
                }
                else {
                    // The player move is valid.
                    break;
                }

            }

            // Finally we perform the act of moving the player.
            int row = tolower(input[0]) - 'a';
            int col = tolower(input[1]) - '0';
            board[row][col] = player_symbol[computer_first];
            // Erasing the old position.
            board[tolower(player_pos[0]) - 'a'][tolower(player_pos[1]) - '0'] = 0;
            // We print a message to the terminal.
            printf("\nPlayer '%c' moves piece from '%s' to '%s'.\n", player_symbol[computer_first], player_pos, input);

        }
        else {
            // This is the computers turn.
            printf("\n* PLAYER %c's turn (computer's turn) *\n\n", player_symbol[!computer_first]);
            // First we get a list of positions of the computer's player.
            char** player_pos = getPlayerPositions(board, player_symbol[!computer_first]);

            // We print the positions of the comnputer's player on the terminal.
            printf("Player %c's positions: ", player_symbol[!computer_first]);
            for (char** p = player_pos; *p != NULL; p++)
            {
                printf("%s ", *p);
            }
            printf("\n");
            // We iterate through the positions and find the valid moves for that position.
            // We find the piece that has the maximum moves possible.

            // The index of the position description that has maximum moves possible!
            int max_idx = 0;
            int max = 0;
            for (char** p = player_pos; *p != NULL; p++)
            {
                char** valid_moves = getPlayerPieceValidMoves(board, *p);
                int count = countPosStrings(valid_moves);
                if(count > max)
                {
                    max = count;
                    max_idx = (p - player_pos);
                }
                if(valid_moves)
                {
                    freeArray(valid_moves);
                }
            }

            // We have found the piece that has maximum valid moves possible!
            // We enumerate all the valid moves for the piece at that position.
            char **validMovesToChoose = getPlayerPieceValidMoves(board, player_pos[max_idx]);
            int count = countPosStrings(validMovesToChoose);

            // The computer randomly chooses a move.
            int idx = rand() % count;
            printf("Computer (Player '%c') chooses piece at: '%s' \n", player_symbol[!computer_first], player_pos[max_idx]);
            // We perform the movement.
            char* move = validMovesToChoose[idx];
            board[tolower(move[0]) - 'a'][tolower(move[1]) - '0'] = player_symbol[!computer_first];
            // We erase the previous position to simulate the movement.
            board[tolower(player_pos[max_idx][0]) - 'a'][tolower(player_pos[max_idx][1]) - '0'] = 0;
            printf("\nComputer (player '%c') moves piece form: '%s' to '%s' \n", player_symbol[!computer_first], player_pos[max_idx],
                move);

            if(validMovesToChoose)
            {
                freeArray(validMovesToChoose);
            }
            freeArray(player_pos);
        }

        // This code toggles the turn of the user or computer every iteration.
        turn_user = !turn_user;
        // Incrementing the turn count.
        turn_count++;
        // To make output clear we add this new line.
        printf("\n");
    }

    // We print the final board state for verification.
    printf("******** FINAL STATE ********\n");
    printBoard(board);
    printf("\n");
    if (game_over)
    {
        // The game is over.
        printf("!!!!!!!! GAME OVER !!!!!!!!\n");
        if (turn_user)
        {
            // If it was the user's turn when the game got over,
            // then they lost the game.
            printf("\nPlayer: '%c' (Computer) won the game! \n\n", player_symbol[!computer_first]);
        }
        else {
            // If it was the computer's turn, then the user won the game.
            printf("\nPlayer: '%c' (you) won the game! \n\n", player_symbol[computer_first]);
        }
    }
    else {
        // The number of turns got exhausted.
        printf("!!!!!!!! NO MORE TURNS !!!!!!!!\n");

        // We compute the number of valid moves each player can make.
        printf("Computing all valid moves for: '%c'\n", player_symbol[0]);
        char** all_valid_moves = getPlayerValidMoves(board, player_symbol[0]);
        // Getting the count of such valid moves.
        int count_a = countPosStrings(all_valid_moves);
        printf("Player: '%c' has %d valid moves (for each movable piece): ", player_symbol[0], count_a);
        for (int i = 0; i < count_a; i++)
        {
            printf("%s ", all_valid_moves[i]);
        }
        freeArray(all_valid_moves);
        printf("\n\n");

        printf("Computing all valid moves for: '%c'\n", player_symbol[1]);
        all_valid_moves = getPlayerValidMoves(board, player_symbol[1]);
        // Getting the count of such valid moves.
        int count_b = countPosStrings(all_valid_moves);
        printf("Player: '%c' has %d valid moves (for each movable piece): ", player_symbol[1], count_b);
        for (int i = 0; i < count_b; i++)
        {
            printf("%s ", all_valid_moves[i]);
        }
        printf("\n\n");

        if (count_a == count_b)
        {
            printf("*** The game is a DRAW *** \n");
        }
        else {
            if (count_a > count_b)
            {
                printf("***** The WINNER is player: '%c' ***** \n\n", player_symbol[0]);
            }
            else {
                printf("***** The WINNER is player: '%c' ***** \n\n", player_symbol[1]);
            }
        }
    }


    /* Freeing the allocated board. */
    for (int i = 0; i < SIDE; i++)
    {
        free(board[i]);
    }
    free(board);

    if (!input)
    {
        free(input);
    }

    system("pause");
    return 0;
}
