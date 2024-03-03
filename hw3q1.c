#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

#define MAX 9
#define MIN 3
#define ERR -5
#define SEC_ERR -1
#define THIRD_ERR -10
#define RED ('R')
#define YELLOW ('Y')
#define EMPTY (' ')
#define T 2
#define M 81
#define D_M 6

//----------------------- Message Identifiers --------------------------//
#define MSG_GET_BOARD_ROWS   0
#define MSG_GET_BOARD_COLS   1
#define MSG_GET_NUMBER_TOKENS 2


//--------------------------- Board Edges ------------------------------//

#define BOARD_VERT_SEP  '|'
#define BOARD_LEFT_ANG '\\'
#define BOARD_RIGHT_ANG '/'
#define BOARD_BOTTOM  '-'
#define BOARD_BOTTOM_SEP '-'



//----------------------------------------------------------------------//


void print_welcome_message();
void print_read_game_params_message(int param);
void print_chose_color_message();
void print_chose_move_message(int player);
void print_enter_column_message();
void print_full_column_message();
void print_unavailable_undo_redo_message();
void scan_params(int game_params[]);
void board_boot(char board[MAX][MAX], int moves[M]);
void board_print(char board[][MAX], int game_params[MIN]);
void play_turn(char colors[T], int game_params[MIN], char board[][MAX], int player, int* last_move_ptr, int data[T], int moves[M], int* k_ptr);
void undo_move(char board[][MAX], int game_params[MIN], int last_move_ptr, int data[MSG_GET_NUMBER_TOKENS]);
bool check_tie(char board[][MAX], int game_params[MIN]);
bool check_winner(char board[][MAX], int game_params[MIN], int player_id);
bool check_winner_column(char board[][MAX], int game_params[MIN], int player_id);
bool check_winner_diagonal(char board[][MAX], int game_params[MIN], int player_id);
bool check_winner_diagonal_MSG_GET_NUMBER_TOKENS(char board[][MAX], int game_params[MIN], int player_id);
bool check_legal_tokens(int game_params[MIN], bool legal);
void play_not_undo_redo(int col, int* last_move_ptr, int data[MIN], int game_params[MIN], int player, char board[][MAX], char colors[T]);
int min(int x, int y);
void game_engine(int game_params[MIN], char board[][MAX], int player_id, int* last_move_ptr, char colors[MSG_GET_NUMBER_TOKENS], int data[MIN]);
int play_if_undo_redo(int col, char board[][MAX], int game_params[MIN], int moves[M], int data[MIN], int again, int* k);
void send_to_start_functions(int game_params[MIN], char board[][MAX], int moves[M]);
int play_redo_move(int data[MIN],int moves[M],int* k_ptr,int again,int* last_move,int game_params[MIN],int player,char board[][MAX],char colors[T]);
void normal_col(int col,int data[MIN],int moves[M],int* k_ptr,int* last_move_ptr,int game_params[MIN],int player,char board[][MAX],char colors[T]);
bool check_status(char board[][MAX], int game_params[MIN], int player_id);
int swap_players(int data[MIN], int player_id, int i);

 /*
  * Outputs winner or tie message.
  *
  * player_id - the id to be output. Pass <0 for
 .
  */
void print_winner(int player_id);

//--------------------------- Main Program -----------------------------//

int main()
{
    // TODO: Write code here
    char colors[MSG_GET_NUMBER_TOKENS] = { 0 }, board[MAX][MAX] = { {0} };
    int player_id = 1, data[MIN] = { 0 }, game_params[MIN] = { 0 }, last_move = 0;
    int* last_move_ptr;
    last_move_ptr = &last_move;

    print_welcome_message(colors);

    game_engine(game_params, board, player_id, last_move_ptr, colors, data);
    return 0;
}

//this function is the game engine and calls the right functions
void game_engine(int game_params[MIN], char board[][MAX], int player_id, int* last_move_ptr, char colors[MSG_GET_NUMBER_TOKENS], int data[MIN])
{
    int k = 0, moves[M] = { SEC_ERR };
    int* k_ptr = &k;
    send_to_start_functions(game_params, board, moves);
    bool tie = false, winner_found = false;
    for (int i = 0; winner_found == false && tie == false; i++)
    {
        play_turn(colors, game_params, board, player_id, last_move_ptr, data, moves, k_ptr);
        board_print(board, game_params);
        winner_found = check_status(board, game_params, player_id);
        tie = check_tie(board, game_params);
        player_id = swap_players(data, player_id, i);
    }
}
//this function checks the winning status
bool check_status(char board[][MAX], int game_params[MIN], int player_id)
{
    int flag = 0;
    flag += check_winner(board, game_params, player_id);
    flag += check_winner_column(board, game_params, player_id);
    flag += check_winner_diagonal(board, game_params, player_id);
    flag += check_winner_diagonal_MSG_GET_NUMBER_TOKENS(board, game_params, player_id);
    if (flag > 0)
        return true;
    else
        return false;
}
//start the game function
void send_to_start_functions(int game_params[MIN], char board[][MAX], int moves[M])
{
    scan_params(game_params);
    board_boot(board, moves);
    board_print(board, game_params);
}
//this function swap the turns
int swap_players(int data[MIN], int player_id, int i)
{
    if (data[MSG_GET_NUMBER_TOKENS] != ERR)
    {
        if (i % MSG_GET_NUMBER_TOKENS == 0)
            player_id = MSG_GET_NUMBER_TOKENS;
        else
            player_id = 1;
    }
    return player_id;
}
//---------------------- Printing Functions ----------------------------//

// message number 0 = "Welcome to 4-in-a-row game! \n"
void print_welcome_message(char colors[MSG_GET_NUMBER_TOKENS])
{
    printf("Welcome to 4-in-a-row game! \n");
    do
    {
        print_chose_color_message();
        scanf(" %c", &colors[0]);
    } while (colors[0] != RED && colors[0] != YELLOW);
    if (colors[0] == RED)
        colors[1] = YELLOW;
    else
        colors[1] = RED;
}


// message number 1 = "Please enter number of rows:"
// message number MSG_GET_NUMBER_TOKENS = "Please enter number of columns:"
// message number MIN = "Please enter number of tokens:"

void print_read_game_params_message(int param)
{
    char const* const possible_params[] = { "rows", "columns", "tokens" };

    printf("Please enter number of %s: ", possible_params[param]);
}

// message number 4 = "Please choose starting color (Y)ellow or (R)ed: "
void print_chose_color_message()
{
    printf("Please choose starting color (Y)ellow or (R)ed: ");
}

// message number 5 = "Your move, player <player>. "
void print_chose_move_message(int player)
{
    printf("Your move, player %d. Please enter column: ", player);
}//1 line

// message number 6
void print_enter_column_message()
{
    printf("Please enter column: ");
}//1 line

void print_full_column_message()
{
    printf("Column full. \n");
}//1 line

void print_unavailable_undo_redo_message()
{
    printf("No moves to undo/redo. ");
}// 1 line
//this function scans the game parameters from the user
void scan_params(int game_params[])
{
    for (int i = 0; i < MIN; i++)
    {
        bool legal = false;
        while (legal == false)
        {
            print_read_game_params_message(i);
            scanf("%d", &game_params[i]);
            if (i == 0 || i == 1)
            {
                if (game_params[i] < MIN || game_params[i] > MAX)
                {
                    legal = false;
                }
                else
                {
                    legal = true;
                }
            }
            else if (i == MSG_GET_NUMBER_TOKENS)
            {
                legal = check_legal_tokens(game_params, legal);
            }
        }
    }
}
//this function checkks if the entered token is legal
bool check_legal_tokens(int game_params[MIN], bool legal)
{
    if (game_params[MSG_GET_NUMBER_TOKENS] < MIN || game_params[MSG_GET_NUMBER_TOKENS] > min(game_params[0], game_params[1]))
    {
        legal = false;
    }
    else
        legal = true;
    return legal;
}
//this function returns the minimum between 2 numbers
int min(int x, int y)
{
    int min = 0;
    if (y < x)
        min = y;
    else
        min = x;
    return min;
}
//this function boots the bored and the moves array (for the undo)
void board_boot(char board[MAX][MAX], int moves[M])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            board[i][j] = EMPTY;
    }
    for (int i = 0; i < M; i++)
    {
        moves[i] = SEC_ERR;
    }
}
//this function prints the board
void board_print(char board[][MAX], int game_params[MIN])
{
    printf("\n");
    for (int i = 0; i < game_params[0]; i++)
    {
        for (int j = 0; j < game_params[1] + 1; j++)
        {
            printf("|%c", board[i][j]);
        }
        printf("\n");
    }
    printf("\\");
    for (int i = 0; i < game_params[0] * MSG_GET_NUMBER_TOKENS - 1; i++)
    {
        printf("-");
    }
    printf("/\n");
}

//this function is the one that undo a move
void undo_move(char board[][MAX], int game_params[MIN], int last_move_ptr, int data[MSG_GET_NUMBER_TOKENS])
{
    for (int i = 0; i < game_params[1]; i++)
    {
        if (board[i][last_move_ptr] == RED || board[i][last_move_ptr] == YELLOW)
        {
            data[0] = i;
            data[1] = last_move_ptr;
            data[MSG_GET_NUMBER_TOKENS] = (int)board[i][last_move_ptr];
            board[i][last_move_ptr] = EMPTY;
            break;
        }
    }
}


//this function cheks if there is a winner in the rows
bool check_winner(char board[][MAX], int game_params[MIN], int player_id)
{
    int cnt = 0;
    for (int i = 0; i < game_params[0]; i++)
    {
        for (int j = 0; j < game_params[1] - 1; j++)
        {
            if (board[i][j] == board[i][j + 1] && (board[i][j] == RED || board[i][j] == YELLOW))
                cnt++;
            else
                cnt = 0;
            if (cnt == game_params[MSG_GET_NUMBER_TOKENS] - 1)
            {
                print_winner(player_id);
                i = game_params[0]; //to stop the first for
                return true;
            }

        }
        cnt = 0;
    }
    return false;
}
//this function cheks if there is a winner in the columns
bool check_winner_column(char board[][MAX], int game_params[MIN], int player_id)
{
    int cnt = 0;
    for (int i = 0; i < game_params[0]; i++)
    {
        for (int j = 0; j < game_params[1] - 1; j++)
        {
            if (board[j][i] == board[j + 1][i] && (board[j][i] == RED || board[j][i] == YELLOW))
                cnt++;
            else
                cnt = 0;
            if (cnt == game_params[MSG_GET_NUMBER_TOKENS] - 1)
            {
                print_winner(player_id);
                i = game_params[0]; //to stop the first for
                return true;
            }
        }
        cnt = 0;
    }
    return false;
}
//this function cheks if there is a winner in the right diagonal
bool check_winner_diagonal(char board[][MAX], int game_params[MIN], int player_id)
{
    int cnt = 0;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            for (int k = 1; k < D_M; k++)
            {
                if (board[i][j] == board[i + k][j + k] && (board[i][j] == RED || board[i][j] == YELLOW))
                    cnt++;
                else { break; }
                (!(board[i][j] == board[i + k][j + k] && (board[i][j] == RED || board[i][j] == YELLOW))) ? cnt = 0 : 1;
            }
            if (cnt >= game_params[MSG_GET_NUMBER_TOKENS] - 1)
            {
                print_winner(player_id);
                return true;
            }
            cnt = 0;
        }
    }
    return false;
}
//this function cheks if there is a winner in the LEFT DIAGONAL
bool check_winner_diagonal_MSG_GET_NUMBER_TOKENS(char board[][MAX], int game_params[MIN], int player_id)
{
    int cnt = 0;
    for (int i = 1; i < MAX; i++)
    {
        for (int j = 1; j < MAX; j++)
        {
            for (int k = 1; k < D_M; k++)
            {
                if (board[i][j] == board[i - k][j + k] && (board[i][j] == RED || board[i][j] == YELLOW))
                    cnt++;
                else { break; }
                (!(board[i][j] == board[i - k][j + k] && (board[i][j] == RED || board[i][j] == YELLOW))) ? cnt = 0 : 1;
            }
            if (cnt >= game_params[MSG_GET_NUMBER_TOKENS] - 1)
            {
                print_winner(player_id);
                return true;
            }
            cnt = 0;
        }
    }
    return false;
}
//this function send to the right function to play the move
void play_turn(char colors[T], int game_params[MIN], char board[][MAX], int player, int* last_move_ptr, int data[T], int moves[M], int* k_ptr)
{
    int again = 0, col = 0, cnt = 0;
    print_chose_move_message(player);
    do {
        col = 0, again = 0;
        (cnt == 0) ? scanf("%d", &col) : 1;
        while ((col < -MSG_GET_NUMBER_TOKENS || col > game_params[1] || col == 0) || (again == MSG_GET_NUMBER_TOKENS || again == 1))
        {
            printf("Please enter column: ");
            scanf("%d", &col);
        }
        if (col == SEC_ERR) { again = play_if_undo_redo(col, board, game_params, moves, data, again, k_ptr); }
        else if (col == -MSG_GET_NUMBER_TOKENS) { again = play_redo_move(data, moves, k_ptr,again,last_move_ptr,game_params,player,board,colors);}
        else { normal_col(col, data, moves, k_ptr, last_move_ptr, game_params, player, board, colors); }    
        cnt++;
    } while (again == 1 || again == MSG_GET_NUMBER_TOKENS);
}
//this function deals with the undo or redo
int play_if_undo_redo(int col, char board[][MAX], int game_params[MIN], int moves[M], int data[MIN], int again, int* k)
{
    if (*k <= 0)
    {
        print_unavailable_undo_redo_message();
        again = 1;
    }
    else if (col == SEC_ERR)
    {
        undo_move(board, game_params, moves[*k - 1], data);
        (*k)--;
        data[1] = THIRD_ERR;

    }
    return again;
}
//this function make a move if its not undo or redo
void play_not_undo_redo(int col, int* last_move_ptr, int data[MIN], int game_params[MIN], int player, char board[][MAX], char colors[T])
{
    col -= 1, * last_move_ptr = col, data[0] = SEC_ERR;
    for (int i = game_params[0] - 1; i >= 0; i--)
    {
        if (player == 1)
        {
            if (board[i][col] == EMPTY)
            {
                board[i][col] = colors[0];
                break;
            }
        }
        else
        {
            if (board[i][col] == EMPTY)
            {
                board[i][col] = colors[1];
                check_winner(board, game_params, player);
                break;
            }
        }
        if (i == 0 && board[i][col] != EMPTY)
            print_full_column_message();

    }
}
//this function redo a move
int play_redo_move(int data[MIN],int moves[M],int* k_ptr,int again,int* last_move,int game_params[MIN],int player,char board[][MAX],char colors[T])
{
    if (data[1] != THIRD_ERR || moves[*k_ptr] == SEC_ERR)
    {
        print_unavailable_undo_redo_message();
        again = 1;
    }
    else
    {
        play_not_undo_redo(moves[*k_ptr] + 1, last_move, data, game_params, player, board, colors);
        (*k_ptr)++;
        again = 0;
    }
    return again;
}

void normal_col(int col,int data[MIN],int moves[M],int* k_ptr,int* last_move_ptr,int game_params[MIN],int player,char board[][MAX],char colors[T])
{
    data[1] = 0;
    play_not_undo_redo(col, last_move_ptr, data, game_params, player, board, colors);
    moves[*k_ptr] = col - 1;
    (*k_ptr)++;
}
//print the winner or the tie
void print_winner(int player_id)
{
    if (player_id > 0)
        printf("Player %d won! \n", player_id);
    else if (player_id == 0)
        printf("That's a tie. \n");
}//4 lines
//check if there is a tie
bool check_tie(char board[][MAX], int game_params[MIN])
{
    for (int i = 0; i < game_params[1]; i++)
    {
        if (board[0][i] == EMPTY)
            return false;
    }
    print_winner(0);
    return true;
}//5 lines