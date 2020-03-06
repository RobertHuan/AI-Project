#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <utility>
using namespace std;
//回傳一個pair(方向,座標),方向:8個方向  回傳的座標為x*8+y
vector< pair<int, int> > available_move(int board[][10], int x, int y, int side) {
    vector< pair<int, int> > next;
    if(board[x][y]) {
        return next;
    }
    //左上
    if((y < 8) && (x > 1) && (board[x-1][y+1] == -side)) {
        for(int i=0; (x-i-2 >= 0) && (y+i+2 < 10); i++) {
            if(!board[x-i-2][y+i+2]) {
                break;
            } else if(board[x-i-2][y+i+2] == side) {
                next.push_back(make_pair<int, int>(0, (x-i-2)*16+(y+i+2)));
                break;
            }
        }
    }
    //左
    if((x > 1) && (board[x-1][y] == -side)) {
        for(int i=x-2; i >= 0; i--) {
            if(!board[i][y]) {
                break;
            } else if(board[i][y] == side) {
                next.push_back(make_pair<int, int>(1, i*16+y));
                break;
            }
        }
    }
    //左下
    if((y > 1) && (x > 1) && (board[x-1][y-1] == -side)) {
        for(int i=0; (x-i-2 >= 0) && (y-i-2 >= 0); i++) {
            if(!board[x-i-2][y-i-2]) {
                break;
            } else if(board[x-i-2][y-i-2] == side) {
                next.push_back(make_pair<int, int>(2, (x-i-2)*16+(y-i-2)));
                break;
            }
        }
    }
    //下
    if((y > 1) && (board[x][y-1] == -side)) {
        for(int i=y-2; i >= 0; i--) {
            if(!board[x][i]) {
                break;
            } else if(board[x][i] == side) {
                next.push_back(make_pair<int, int>(3, x*16+i));
                break;
            }
        }
    }
    //右下
    if((y > 1) && (x < 8) && (board[x+1][y-1] == -side)) {
        for(int i=0; (x+i+2 < 10) && (y-i-2 >= 0); i++) {
            if(!board[x+i+2][y-i-2]) {
                break;
            } else if(board[x+i+2][y-i-2] == side) {
                next.push_back(make_pair<int, int>(4, (x+i+2)*16+(y-i-2)));
                break;
            }
        }
    }
     //右
    if((x < 8) && (board[x+1][y] == -side)) {
        for(int i=x+2; i < 10; i++) {
            if(!board[i][y]) {
                break;
            } else if(board[i][y] == side) {
                next.push_back(make_pair<int, int>(5, i*16+y));
                break;
            }
        }
    }
    //右上
    if((y < 8) && (x < 8) && (board[x+1][y+1] == -side)) {
        for(int i=0; (x+i+2 < 10) && (y+i+2 < 10); i++) {
            if(!board[x+i+2][y+i+2]) {
                break;
            } else if(board[x+i+2][y+i+2] == side) {
                next.push_back(make_pair<int, int>(6, (x+i+2)*16+(y+i+2)));
                break;
            }
        }
    }
    //上
    if((y < 8) && (board[x][y+1] == -side)) {
        for(int i=y+2; i < 10; i++) {
            if(!board[x][i]) {
                break;
            } else if(board[x][i] == side) {
                next.push_back(make_pair<int, int>(7, x*16+i));
                break;
            }
        }
    }
    return next;
}

//回傳所以可以移動的座標
vector< pair<int, vector< pair<int, int> > > > get_moves(int board[][10], int side) {
    vector< pair<int, vector< pair<int, int> > > > temp_move;
    for(int i=0; i < 10; i++) {
        for(int j=0; j < 10; j++) {
            temp_move.push_back(make_pair<int, vector< pair<int, int> > >(i*16+j, available_move(board, i, j, side)));
            if(!temp_move.back().second.size()) {
                temp_move.pop_back();
            }
        }
    }
    return temp_move;
}

//下一步棋並將其他需要變色的棋換成該顏色
void action(int board[][10], int x, int y, int side, vector< pair<int, int> > next) {
    for(auto it=next.begin(); it != next.end(); it++) {
        int t_x = x;
        int t_y = y;
        while((t_x != ((*it).second/16)) || (t_y != ((*it).second&15))) {
            board[t_x][t_y] = side;
            if(t_x < ((*it).second/16)) {
                t_x++;
            } else if((t_x > (*it).second/16)) {
                t_x--;
            }
            if(t_y < ((*it).second&15)) {
                t_y++;
            } else if(t_y > ((*it).second&15)) {
                t_y--;
            }
        }
    }
}

//negamax演算法中的取消這步棋
void undo_move(int board[][10], int x, int y, int side, vector< pair<int, int> > next) {
    for(auto it=next.begin(); it != next.end(); it++) {
        int t_x = x;
        int t_y = y;
        while((t_x != ((*it).second/16)) || (t_y != ((*it).second&15))) {
            board[t_x][t_y] = -side;
            if(t_x < ((*it).second/16)) {
                t_x++;
            } else if((t_x > (*it).second/16)) {
                t_x--;
            }
            if(t_y < ((*it).second&15)) {
                t_y++;
            } else if(t_y > ((*it).second&15)) {
                t_y--;
            }
        }
        board[t_x][t_y] = side;
    }
    board[x][y] = 0;
}

//印出盤面
void print(int board[][10]) {
    for(int i=9; i >= 0; i--) {
        printf("%d ", i);
        for(int j=0; j < 10; j++) {
            if(!board[j][i]) {
                printf(". ");
            } else if(board[j][i] == 1) {
                printf("O ");
            } else {
                printf("X ");
            }
        }
        printf("\n");
    }
    printf("  ");
    for(int i=0; i < 10; i++) {
        printf("%d ", i);
    }
    printf("\n\n");
}

//計算得分
int score(int board[][10], int side) {
    int sum = 0;
    for(int i=0; i < 10; i++) {
        for(int j=0; j < 10; j++) {
            sum += board[i][j];
        }
    }
    return sum * side;
}

//基層的negamax演算法
int negamax_all(int board[][10], int side, int depth, int alpha, int beta) {
    if(depth == 0) {
        return score(board, side);
    }
    vector< pair<int, vector< pair<int, int> > > > moves = get_moves(board, side);
    if(moves.size() == 0) {
        if(get_moves(board, -side).size() == 0) {
            return score(board, side);
        }
        int val = -negamax_all(board, -side, depth-1, -beta, -alpha);
        if(val >= beta) {
            return val;
        }
        if(val > alpha) {
            alpha = val;
        }
    } else {
        for(auto it=moves.begin(); it != moves.end(); it++) {
            action(board, (*it).first/16, (*it).first&15, side, (*it).second);
            int val = -negamax_all(board, -side, depth-1, -beta, -alpha);
            undo_move(board, (*it).first/16, (*it).first&15, side, (*it).second);
            if(val >= beta) {
                return val;
            }
            if(val > alpha) {
                alpha = val;
            }
        }
    }
    return alpha;
}
//negamax演算法
int negamax(int board[][10], int side, int depth) {
    int alpha = -80;
    int beta = 80;
    vector< pair<int, vector< pair<int, int> > > > moves = get_moves(board, side);
    int move_next = moves[0].first;
    for(auto it=moves.begin(); it != moves.end(); it++) {
        action(board, (*it).first/16, (*it).first&15, side, (*it).second);
        int val = -negamax_all(board, -side, depth-1, -beta, -alpha);
        undo_move(board, (*it).first/16, (*it).first&15, side, (*it).second);
        if(val >= beta) {
            return (*it).first;
        }
        if(val > alpha) {
            alpha = val;
            move_next = (*it).first;
        }
    }
    return move_next;
}

//main.
int main() {

    //盤面初始化
    int depth = 3*2;
    int board[10][10];
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            board[i][j]=0;
        }
    }
    board[4][4] = board[5][5] = -1;
    board[4][5] = board[5][4] = 1;
    int side = -1;
    printf("You are the team X.....\n");
    printf("This is a game of othello with 10X10\n");
    system("pause");
    system("cls");

    //開始遊戲
    while(true) {
        print(board);
        vector< pair<int, vector< pair<int, int> > > > moves= get_moves(board, side);
        printf("You can move: ");
        for(auto it=moves.begin(); it != moves.end(); it++) {
            printf("(%d, %d)  ", (*it).first/16, (*it).first%16);
        }
        printf("\n");
        if(moves.size() == 0) {
            side = -side;
            moves = get_moves(board, side);
            if(moves.size() == 0) {
                printf("final score: %d\n", score(board, -1));
                if(score(board,-1)>0)
                {
                    printf("You Win!!\n");
                }
                else
                    printf("You Lose!!\n");
                return 0;
            }
        } else {
            int x, y;
            if(side == -1) {
                scanf("%d %d", &x, &y);
                for(auto it=moves.begin(); it != moves.end(); it++) {
                    if(x*16+y == ((*it).first)) {
                        printf("You moved: %d %d\n", x, y);
                        action(board, x, y, side, (*it).second);
                        side = -side;
                        break;
                    }
                }
            } else {
                x = negamax(board, side, depth);
                for(auto it=moves.begin(); it != moves.end(); it++) {
                    if(x == ((*it).first)) {
                        printf("THE AI moved: %d %d\n", x/16, x%16);
                        action(board, x/16, x%16, side, (*it).second);
                        side = -side;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
