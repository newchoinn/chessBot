#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#define MAX 100000
#define MIN -100000

using namespace std;

enum class Color { Black, White }; // 定義顏色類型

// 定義棋盤
bool run = true;          //程式是否正在運行
char board[8][8];         //棋盤
string boardnum[8][8];    //棋盤編號
int boardPressure[8][8];  //棋盤上防守和攻擊的狀態
int boardWeight[8][8];    //棋盤格子的重要程度
int boardPoint[8][8];     //棋盤格子的分數
int allPoint;             //分數
int maxDepth = 4;         //機器人思考的最大深度
int nowDepth;             //現在的深度
int castleMove;

class Piece //棋子
{
public:
  Piece(char type, Color color, int x, int y)
      : type_(type), color_(color), coor_x(x), coor_y(y) {} // 建構函數

  char getType() const { return type_; }    // 獲取棋子類型
  Color getColor() const { return color_; } // 獲取棋子顏色
  int getCoorX() const { return coor_x; }   // 獲取棋子X座標
  int getCoorY() const { return coor_y; }   // 獲取棋子Y座標
  void coorXIn(int x) {coor_x=x;}
  void coorYIn(int y) {coor_y=y;}
  void typeIn(char t) {type_ =t;}

  vector<pair<int, int>> eat_chess(int x, int y, char board[8][8]) 
  {
    Color c = color_;
    vector<pair<int, int>> moves;

    if (x >= 0 && x <= 7 && y >= 0 && y <= 7) {
        if (c == Color::Black) {
            if (isupper(board[x][y]) || board[x][y] == ' ')
                moves.emplace_back(x, y);
        } else if (c == Color::White) {
            if (islower(board[x][y]) || board[x][y] == ' ')
                moves.emplace_back(x, y);
        } else {
            cout << "err";
        }
    }

    return moves;
}

bool canEatChess(int x, int y, char board[8][8]) {
    Color c = getColor();
    bool suc = false;
  
    if (x >= 0 && x <= 7 && y >= 0 && y <= 7) {
        if (board[x][y] == ' ') {
            suc = true;
        } else if (c == Color::Black && isupper(board[x][y])) {
            suc = true;
        } else if (c == Color::White && islower(board[x][y])) {
            suc = true;
        }
    }

    return suc;
}

vector<pair<int, int>> eat_chess(int x1, int y1, int x2, int y2, char board[8][8]) {
    vector<pair<int, int>> moves;

    if (x1 >= 0 && x1 <= 7 && y1 >= 0 && y1 <= 7 && x2 >= 0 && x2 <= 7 && y2 >= 0 && y2 <= 7) {
        Color c = getColor();
        if (c == Color::Black) {
            if (isupper(board[x1][y1]))
                moves.emplace_back(x1, y1);
            if (isupper(board[x2][y2]))
                moves.emplace_back(x2, y2);
        } else {
            if (islower(board[x1][y1]))
                moves.emplace_back(x1, y1);
            if (islower(board[x2][y2]))
                moves.emplace_back(x2, y2);
        }
    }

    return moves;
}


vector<pair<int, int>> getPossibleMoves (char board[8][8]) {
    int x = getCoorX(),y=getCoorY();
    vector<pair<int, int>> moves; // 儲存所有可能的移動位置
    vector<pair<int, int>> in;
    if (type_ == 'P' || type_ == 'p') // Pown
    {
      if (getColor() == Color::White) {
        in = eat_chess(x - 1, y - 1, x + 1, y - 1,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x][y - 1] == ' ')moves.emplace_back(x, y - 1);
        if (y == 6 && board[x][y - 2] == ' '
          && board[x][y - 1] == ' ') 
        {
          moves.emplace_back(x, y - 2);
        }
      } else // if(getColor()==Color::Black)
      {
        in = eat_chess(x - 1, y + 1, x + 1, y + 1,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x][y + 1] == ' ')moves.emplace_back(x, y + 1);
        if (y == 1 && board[x][y + 2] == ' ' 
          && board[x][y + 1] == ' ') 
        {
          moves.emplace_back(x, y + 2);
        }
      }
    }

    else if (type_ == 'N' || type_ == 'n') // Knight
    {
      in = eat_chess(x - 1, y - 2,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x - 2, y - 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x - 2, y + 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x - 1, y + 2,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x + 1, y + 2,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x + 2, y + 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x + 2, y - 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x + 1, y - 2,board);
      moves.insert(moves.begin(), in.begin(), in.end());

    }

    else if (type_ == 'R' || type_ == 'r') // Rook
    {
        // Check moves in upward direction
        for (int i = 1; x + i < 8; i++) {
            if (board[x + i][y] != ' ') {
                if (isupper(type_) != isupper(board[x + i][y])) {
                    moves.emplace_back(x + i, y);
                }
                break;
            } else {
                moves.emplace_back(x + i, y);
            }
        }

        // Check moves in downward direction
        for (int i = 1; x - i >= 0; i++) {
            if (board[x - i][y] != ' ') {
                if (isupper(type_) != isupper(board[x - i][y])) {
                    moves.emplace_back(x - i, y);
                }
                break;
            } else {
                moves.emplace_back(x - i, y);
            }
        }

        // Check moves in right direction
        for (int i = 1; y + i < 8; i++) {
            if (board[x][y + i] != ' ') {
                if (isupper(type_) != isupper(board[x][y + i])) {
                    moves.emplace_back(x, y + i);
                }
                break;
            } else {
                moves.emplace_back(x, y + i);
            }
        }

        // Check moves in left direction
        for (int i = 1; y - i >= 0; i++) {
            if (board[x][y - i] != ' ') {
                if (isupper(type_) != isupper(board[x][y - i])) {
                    moves.emplace_back(x, y - i);
                }
                break;
            } else {
                moves.emplace_back(x, y - i);
            }
        }

    }

    else if (type_ == 'B' || type_ == 'b') // Bishop
    {
    // top-right
    int i = 1;
    while (x + i < 8 && y + i < 8)
    {
        if (!canEatChess(x + i, y + i,board)) 
        {
            break;
        }
        auto in = eat_chess(x + i, y + i,board);
        moves.insert(moves.end(), in.begin(), in.end());
        if (board[x + i][y + i] != ' ')
        {
            break;
        }
        i++;
    }

    // top-left
    i = 1;
    while (x + i < 8 && y - i >= 0)
    {
        if(!canEatChess(x+i, y-i, board))
        {
          break;
        }
        auto in = eat_chess(x + i, y - i,board);
        moves.insert(moves.end(), in.begin(), in.end());
        if (board[x + i][y - i] != ' ')
        {
            break;
        }
        i++;
    }

    // bottom-left
    i = 1;
    while (x - i >= 0 && y - i >= 0)
    {
        if (!canEatChess(x - i, y - i,board)) 
        {
            break;
        }
        auto in = eat_chess(x - i, y - i,board);
        moves.insert(moves.end(), in.begin(), in.end());
        if (board[x - i][y - i] != ' ')
        {
            break;
        }
        i++;
    }

    // bottom-right
    i = 1;
    while (x - i >= 0 && y + i < 8)
    {
        if (!canEatChess(x - i, y + i,board)) 
        {
            break;
        }
        auto in = eat_chess(x - i, y + i,board);
        moves.insert(moves.end(), in.begin(), in.end());
        if (board[x - i][y + i] != ' ')
        {
            break;
        }
        ++i;
    }

  }


    else if (type_ == 'K' || type_ == 'k') // King
    {
      in = eat_chess(x + 1, y,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x - 1, y,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x, y + 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x, y - 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x + 1, y + 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x + 1, y - 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x - 1, y + 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());
      in = eat_chess(x - 1, y - 1,board);
      moves.insert(moves.begin(), in.begin(), in.end());

      if (isupper(board[x][y])) 
      {
        // King-side castle
        if (board[x + 1][y] == ' ' && board[x + 2][y] == ' ' 
          && board[4][7]=='K'&&board[x + 3][y]=='R') 
        {
          moves.emplace_back(x + 2, y);
        }
        // Queen-side castle
        if (board[x - 1][y] == ' ' && board[x - 2][y] == ' ' 
          && board[x - 3][y] == ' ' && board[4][7]=='K'
          &&board[x - 4][y]=='R') 
        {
          moves.emplace_back(x - 2, y);
        }
      }
      else//black castle
      {
        // King-side castle
        if (board[x + 1][y] == ' ' && board[x + 2][y] == ' ' 
          && board[4][0]=='k' && board[x + 3][y]=='r') 
        {
          moves.emplace_back(x + 2, y);
        }
        // Queen-side castle
        if (board[x - 1][y] == ' ' && board[x - 2][y] == ' ' 
          && board[x - 3][y] == ' ' && board[4][0]=='k'
          &&board[x - 4][y]=='r') 
        {
          moves.emplace_back(x - 2, y);
        }
      }
    }

    else // if(type_=='Q'||type_=='q') //Queen
    {
      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x + i, y,board)) break;
        in = eat_chess(x + i, y,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x+i][y]!=' ')break;
      }
      
      for (int i = 1; i < 8; i++)
      {
        if (!canEatChess(x - i, y,board)) break;
        in = eat_chess(x - i, y,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x-i][y]!=' ')break;
      }
      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x, y + i,board)) break;
        in = eat_chess(x, y + i,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x][y+i]!=' ')break;
      }
      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x, y - i,board)) break;
        in = eat_chess(x, y - i,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x][y-i]!=' ')break;
      }
      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x + i, y + i,board)) break;
        in = eat_chess(x + i, y + i,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x+i][y+i]!=' ')break;
      }
      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x - i, y - i,board)) break;
        in = eat_chess(x - i, y - i,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x-i][y-i]!=' ')break;
      }

      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x + i, y - i,board)) break;
        in = eat_chess(x + i, y - i,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x+i][y-i]!=' ')break;
      }
      for (int i = 1; i < 8; i++) 
      {
        if (!canEatChess(x - i, y + i,board)) break;
        in = eat_chess(x - i, y + i,board);
        moves.insert(moves.begin(), in.begin(), in.end());
        if(board[x-i][y+i]!=' ')break;
      }
    }
    for(int i=0;i<moves.size();i++)
      {
        if(moves[i].first>7||moves[i].first<0
          ||moves[i].second>7||moves[i].second<0)
        {
          moves.erase(moves.begin()+i-1);
          i--;
        }
      }
    return moves;
  }

private:
  Color color_; // 棋子顏色
  int coor_x; //棋子X座標
  int coor_y; //棋子Y座標
  char type_;   // 棋子類型
};

 //函數
void initBoard();
Color detectChessColor(int x, int y); 
void printboard();
void printnum();
bool move(int x1, int y1, int x2, int y2, vector<Piece> &pieces);
void playerMove();
void botMove();
void countBoardPressure(vector<Piece> pieces);
void countBoardWeight(vector<Piece> pieces);
void countBoardPoint(vector<Piece> pieces);
int  botTest(vector<Piece> last);
int  humanTest(vector<Piece> last);
void setBoard(char board[8][8],vector<Piece> pieces);
void printmoves(int y1, int x1);
void botMove();

vector<Piece> pieces;   //所有棋子

int main() {
  initBoard();//初始化棋盤
  setBoard(board,pieces);//將棋子放上棋盤
  printboard();//印出棋盤
  while (run) {
    playerMove();//玩家移動
    botMove();//機器人移動
  }
  cout<<"Quit";
}

// 函數程式碼
void initBoard() {
  pieces.emplace_back(Piece('r', Color::Black, 0, 0));
  pieces.emplace_back(Piece('n', Color::Black, 1, 0));
  pieces.emplace_back(Piece('b', Color::Black, 2, 0));
  pieces.emplace_back(Piece('q', Color::Black, 3, 0));
  pieces.emplace_back(Piece('k', Color::Black, 4, 0));
  pieces.emplace_back(Piece('b', Color::Black, 5, 0));
  pieces.emplace_back(Piece('n', Color::Black, 6, 0));
  pieces.emplace_back(Piece('r', Color::Black, 7, 0));
  for (int i = 0; i < 8; i++) {
    pieces.emplace_back(Piece('p', Color::Black, i, 1));
  }
  for (int i = 0; i < 8; i++) {
    pieces.emplace_back(Piece('P', Color::White, i, 6));
  }
  pieces.emplace_back(Piece('R', Color::White, 0, 7));
  pieces.emplace_back(Piece('N', Color::White, 1, 7));
  pieces.emplace_back(Piece('B', Color::White, 2, 7));
  pieces.emplace_back(Piece('Q', Color::White, 3, 7));
  pieces.emplace_back(Piece('K', Color::White, 4, 7));
  pieces.emplace_back(Piece('B', Color::White, 5, 7));
  pieces.emplace_back(Piece('N', Color::White, 6, 7));
  pieces.emplace_back(Piece('R', Color::White, 7, 7));
}
//初始化棋盤
Color detectChessColor(int x, int y) {
  int a = -1;
  for (int i = 0; i < 32; i++) {
    if (pieces[i].getCoorX() == x && pieces[i].getCoorY() == y) {
      a = i;
      break;
    }
  }
  return pieces[a].getColor();
}
//輸出棋子顏色
void setBoard(char board[8][8],vector<Piece> pieces){
  // 將棋子放到棋盤上
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = ' ';
    }
  }
  for (int i = 0; i < pieces.size(); i++) 
  {
    board[pieces[i].getCoorX()][pieces[i].getCoorY()]=
      pieces[i].getColor() == Color::White?
      toupper(pieces[i].getType()):pieces[i].getType();
  }
}
//將棋子擺上棋盤
void printboard(){
  for (int i = 0; i < 8; i++) {
    cout << 8-i << ' ';
    for (int j = 0; j < 8; j++)
      cout << board[j][i] << ' ';
    cout << endl;
  }
  cout << "/ ";
  for (int i = 0; i < 8; i++) {
    char c = 'a'+i;
    cout << c << ' ';
  }
  cout << endl;
  cout << endl;
}
//印出棋盤
void printnum() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++)
      boardnum[i][j] = ' ';
  }
  for (int i = 0; i < 32; i++) {
    boardnum[pieces[i].getCoorX()][pieces[i].getCoorY()] = to_string(i);
  }
  cout << "  ";
  for (int i = 0; i < 8; i++) {
    cout << i << ' ';
  }
  cout << endl;
  for (int i = 0; i < 8; i++) {
    cout << i << ' ';
    for (int j = 0; j < 8; j++)
      cout << boardnum[j][i] << ' ';
    cout << endl;
  }
  cout << endl;
}
//印出棋盤
void printmoves(int x1, int y1) {
  char canmove[8][8];

  setBoard(board,pieces);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      canmove[i][j] = ' ';
    }
  }

  for (int i = 0; i < pieces.size(); i++) {
    if (pieces[i].getCoorX() == x1 && pieces[i].getCoorY() == y1) {
      char type = pieces[i].getType();
      vector<pair<int, int>> moves = pieces[i].getPossibleMoves(board);
      for (int j = 0; j < moves.size(); j++) {
        pair<int, int> move = moves[j];
        int movex = move.first;
        int movey = move.second;
        canmove[movex][movey] = type;
      }
    }
  }

  cout << "  ";
  for (int i = 0; i < 8; i++) {
    cout << i << ' ';
  }
  cout << endl;
  for (int i = 0; i < 8; i++) {
    cout << i << ' ';
    for (int j = 0; j < 8; j++)
      cout << canmove[j][i] << ' ';
    cout << endl;
  }
  cout << endl;
  printboard();
}
//印出輸入的棋子可移動的位置
bool move(int x1, int y1, int x2, int y2, vector<Piece> &pieces) {
  setBoard(board,pieces);
  bool found;
  for (int i = 0; i < pieces.size(); i++) 
  {
    if (pieces[i].getCoorX() == x1 && pieces[i].getCoorY() == y1) 
    {
      vector<pair<int, int>> moves=
        pieces[i].getPossibleMoves(board);
      found = any_of(moves.begin(),moves.end(),
                          [&](const pair<int, int> &p) {
                            return p.first == x2 && p.second == y2;
                          }); //棋子是否可以移動
      if (found) 
      {
        for(int j=0;j<pieces.size();j++)
          {
            if(pieces[j].getCoorX()==x2 && pieces[j].getCoorY()==y2)
            {
              swap(*(pieces.begin() + j),*(pieces.end() - 1));
              pieces.pop_back();
              break;
            }
          }
        pieces[i].coorXIn(x2);
        pieces[i].coorYIn(y2);
        if(y2==(Color::Black==pieces[i].getColor()?7:0))
        {
          pieces[i].typeIn
          (Color::Black==pieces[i].getColor()?'q':'Q');
        }
        }
        if((pieces[i].getType()=='K'||pieces[i].getType()=='k')
          &&abs(x2-x1)>1)
        {
          if(y1==7&&x2==6)
          {
            for(int j=0;j<pieces.size();j++)
              {
                if(pieces[j].getCoorX()==7&&pieces[j].getCoorY()==7)
                {
                  pieces[j].coorXIn(5);
                  allPoint-=10;
                  break;
                }
              }
          }
          else if(y1==7&&x2==2)
          {
            for(int j=0;j<pieces.size();j++)
              {
                if(pieces[j].getCoorX()==0&&pieces[j].getCoorY()==7)
                {
                  pieces[j].coorXIn(3);
                  allPoint-=10;
                  break;
                }
              }
          }
          else if(y1==0&&x2==6)
          {
            for(int j=0;j<pieces.size();j++)
              {
                if(pieces[j].getCoorX()==7&&pieces[j].getCoorY()==0)
                {
                  pieces[j].coorXIn(5);
                  allPoint+=10;
                  break;
                }
              }
          }
          else
          {
            for(int j=0;j<pieces.size();j++)
              {
                if(pieces[j].getCoorX()==0&&pieces[j].getCoorY()==0)
                {
                  pieces[j].coorXIn(3);
                  allPoint+=10;
                  break;
                }
              }
          }
        }
        break;
    }
  }
  return found;
}
//將數入的棋子移動到輸入的位置
void playerMove() {
  string a;bool next=false;
  while(!next)
  {
  cin>>a;
  if((a[0]=='q'||a[0]=='s') && a.length()==1)
  {
    run=false;
  }
  else if(a[0]=='m' && a.length()==1)
  {
    int x,y;
    string s;
    cin>>s;
    x=s[0]-'a';
    y='8' - s[1];
    printmoves(x,y);
  }
  else //if(a[0]<='h'&&a[1]>='a'&&a[1]<='8'&&a[1]>='1')
  {
    string s;
    int x1,x2,y1,y2;
    cin>>s;
    x1=a[0] - 'a';
    y1='8' - a[1];
    x2=s[0] - 'a';
    y2='8' - s[1];
    if(x1!=' '&&y1!=' ')
    {
      next = move(x1,y1,x2,y2,pieces);
      setBoard(board,pieces);
      printboard();
    }
  }
  }
}
//讓玩家在棋盤上移動
void countBoardPressure(vector<Piece> pieces){
  vector<pair<int,int>> in,blackMoves,whiteMoves;
  for(int i=0;i<8;i++)
    {
      for(int j=0;j<8;j++)
        {
          boardPressure[i][j]=0;
        }
    }
  for(int i=0;i<pieces.size();i++)
    {
      if(pieces[i].getColor()==Color::Black)
      {
        in=pieces[i].getPossibleMoves(board);
        if(in.size()!=0)
        {
          blackMoves.insert(blackMoves.end(),in.begin(),in.end());
        }
      }
    }
  for(int i=0;i<blackMoves.size();i++)
    {
      boardPressure[blackMoves[i].first][blackMoves[i].second]++;
    }
  for(Piece p:pieces)
    {
      if(p.getType()=='k')
      {
        boardPressure[p.getCoorX()][p.getCoorY()] = 0;
        break;
      }
    }
  for(int i=0;i<pieces.size();i++)
    {
      if(pieces[i].getColor()==Color::White)
      {
        in=pieces[i].getPossibleMoves(board);
        if(in.size()!=0)
        {
          whiteMoves.insert(whiteMoves.end(),in.begin(),in.end());
        }
      }
    }
  for(int i=0;i<whiteMoves.size();i++)
    {
      boardPressure[whiteMoves[i].first][whiteMoves[i].second]--;
    }
}
//計算每個棋盤的壓力（被多少個棋子攻擊或防守）
void countBoardWeight(vector<Piece> pieces){
  char piece[12] = {'p','n','b','r','k','q','P','N','B','R','K','Q'};
  int  point[12] = {1,3,3,5,100,9,-1,-3,-3,-5,-100,-9};
  for(int i=0;i<8;i++)
    {
      for(int j=0;j<8;j++)
        {
          boardWeight[i][j]=0;
        }
    }
  for(int i = 0;i<pieces.size();i++)
    {
      for(int j=0;j<12;j++)
        {
          if(pieces[i].getType()==piece[j])
          {
            boardWeight[pieces[i].getCoorX()][pieces[i].getCoorY()]=point[j];
            break;
          }
        }
    }
  for(Piece p:pieces)
    {
      int x=p.getCoorX();
      int y=p.getCoorY();
      if(y==0&&p.getType()!='k')
      {
        boardWeight[x][y]++;
      }
      if((x==4||x==3)&&(y=3||y==4))
      {
        if(p.getType()=='p')board[x][y]--;
        else if(p.getType()=='P')board[x][y]++;
      }
    }
}
//計算每格棋盤的棋子分數
void countBoardPoint(vector<Piece> pieces){
  countBoardPressure(pieces);
  countBoardWeight(pieces);
  allPoint = 0;
  for(int i =0;i<8;i++)
    {
      for(int j=0;j<8;j++)
        {
            boardPoint[i][j]=boardPressure[i][j]*boardWeight[i][j];
            allPoint+=boardPoint[i][j];
        }
    }
}
//計算輸入棋盤的分數
int botTest(vector<Piece> last) {
  nowDepth++;
  if(nowDepth>=maxDepth)
  {
    countBoardPoint(last);
    return allPoint;
  }
  else
  {
    vector<Piece> test;
    char nowBoard[8][8];
    vector<pair<int,int>> testMoves;
    vector<int> points;
    int bestPoint=-10000;
    setBoard(nowBoard,last);
    
    for(int i = 0;i<last.size(); i++)//算出每一步的分數
    {
      testMoves =  last[i].getPossibleMoves(nowBoard);
      for(int j=0;j<testMoves.size();j++)
        {
          test = last;
          if(test[i].getColor()==Color::Black)
          {
            move(test[i].getCoorX(),test[i].getCoorY(),
              testMoves[j].first,testMoves[j].second,test); 
            points.push_back(humanTest(test));
          }
        }
    }
    
    for(int i:points)//找出最好的分數
    {
      if(i>bestPoint)
      {
        bestPoint=i;
      }
    }
    return bestPoint;  
  }
}
//計算出每一步最高的分數
int humanTest(vector<Piece> last){
  nowDepth++;
  if(nowDepth>=maxDepth)
  {
    countBoardPoint(last);
    return allPoint;
  }
  else
  {
    char nowBoard[8][8];
    vector<pair<int,int>> testMoves;
    vector<Piece> bestBoard;
    vector<Piece> test;
    int bestPoint=1000000;
    setBoard(nowBoard,last);
    for(int i=0;i<last.size();i++)
    {
      testMoves = last[i].getPossibleMoves(nowBoard);
      for(int j=0;j<testMoves.size();j++)
        {
          test = last;
          if(test[i].getColor()==Color::White)
          {
            move(test[i].getCoorX(),test[i].getCoorY(),
            testMoves[j].first,testMoves[j].second,test);
            countBoardPoint(test);
            if(allPoint<bestPoint)
            {
              bestPoint=allPoint;
              bestBoard=test;
            }
          }
        }
    }
    int a=botTest(bestBoard);
    return a;
  }
}
//計算出傳入的棋盤黑棋最好的步數
void botMove(){
  char nowBoard[8][8];
  vector<pair<int,int>> testMoves;
  vector<int> firstMoves[4];
  vector<int> points;
  vector<Piece> test;
  int bestMove,bestPoint = -10000;
  countBoardPoint(pieces);
  int lastPoint = allPoint;
  
  setBoard(nowBoard,pieces);
  
  for(int i=0;i<pieces.size();i++)
    {
      testMoves = pieces[i].getPossibleMoves(nowBoard);
      for(int j=0;j<testMoves.size();j++)
        {
          test = pieces;
          if(test[i].getColor()==Color::Black)
          {
            firstMoves[0].push_back(test[i].getCoorX());
            firstMoves[1].push_back(test[i].getCoorY());
            firstMoves[2].push_back(testMoves[j].first);
            firstMoves[3].push_back(testMoves[j].second);
            move(test[i].getCoorX(),test[i].getCoorY(),
              testMoves[j].first,testMoves[j].second,test);
            nowDepth=0;
            points.push_back(humanTest(test));
          }
        }
    }
  
  int a=0;
  for(int i=0;i<points.size();i++)
    {
      if(points[i]>bestPoint)
      {
        bestPoint=points[i];
        bestMove = i;
        a=1;
      }
    }
  cout<<bestPoint<<endl;
  move(firstMoves[0][bestMove],firstMoves[1][bestMove],
        firstMoves[2][bestMove],firstMoves[3][bestMove],pieces);
  setBoard(board,pieces);
  printboard();
}
//將所有可能的步數的列出
void countBoardPoint(vector<Piece> piece)
{
  char piece[12] = {'p','n','b','r','k','q','P','N','B','R','K','Q'};
  int  point[12] = {1,3,3,5,100,9,1,3,3,5,100,9},out=-1;
  int countingBoard[8][8];
  char board[8][8];
  setBoard(board,piece);
  for(int i=0;i<pieces.size();i++)
    {
      if(isupper(pieces[i].getType()))
      {
        vector<pair<int, int>> moves =
          piece[i].getPossibleMoves(board);
        for(pair<int,int> p:moves)
          {
            for(int j=0;j<12;j++)
              {
                if(board[p.first][p.second]==piece[j])
                {
                  out=j;
                  break;
                }
                
              }
          }
      }
      else
      {
        
      }
    }
}
