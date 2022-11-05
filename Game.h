#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <iostream>
#include <sstream> 
#include <iomanip>

#include <stack>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <exception> 
#include <stack>
#include <memory>

typedef enum Move_State{
    InvalidMove = 0,
    ValidMove,
    WhiteWin,
    BlackWin,
    Draw
}Move_State;

typedef enum Pieces_List{
    Empty = 0,
    Black,
    BlackKing,
    White,
    WhiteKing
}Pieces_List;

static const std::vector<char> pieces = { '.', 'x', 'X', 'o', 'O' };

void emptyBoard(std::map<std::pair<char, char>, char> & gameBoard);
void boardReset(std::map<std::pair<char, char>, char> & gameBoard);

void showBoard(const std::map<std::pair<char, char>, char> & gameBoard);

std::vector<std::pair<char, char>> findPiecesRemaining(const char& piece, const std::map<std::pair<char, char>, char> & gameBoard);

bool checkStalemate(const int & playerTurn, const std::map<std::pair<char, char>, char> & gameBoard);

void checkCrown(std::map<std::pair<char, char>, char> & gameBoard);

std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> jumpPathSearch(const std::pair<char, char> & from,
                                                                                                                  const std::pair<char, char> & to,
                                                                                                                  const std::map<std::pair<char, char>, char> & gameBoard);

std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> checkMove(const int & player,
                                                                                                             const std::pair<char, char> & from,
                                                                                                             const std::pair<char, char> & to,
                                                                                                             const std::map<std::pair<char, char>, char> & gameBoard);

std::pair< std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>> findJumpSquares(const char & playerPiece,
                                                                                                   const std::pair<char, char> & square,
                                                                                                   const std::map<std::pair<char, char>, char> & gameBoard);

bool singleSquareMove(const std::pair<char, char> & from,
                      const std::pair<char, char> & to,
                      const std::map<std::pair<char, char>, char> & gameBoard,
                      const bool & findAllSquares = false);

void printError();

void movePiece(std::pair<char, char> &to,
               std::pair<char, char> &from,
               std::map<std::pair<char, char>, char> & gameBoard);

std::pair<std::pair<char, char>, std::pair<char, char>> getUserMove(std::string & prompt);

int win(std::map<std::pair<char, char>, char> & gameBoard, int & playerTurn);

int changeTurn(std::map<std::pair<char, char>, char> & gameBoard,
             std::pair<std::pair<char, char>, std::pair<char, char>> playerMove,
             int & playerTurn);
#endif
