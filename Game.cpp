#ifndef GAME
#define GAME

#include "Game.h"

void emptyBoard(std::map<std::pair<char, char>, char> & gameBoard){
    std::map<std::pair<char, char>, char> temp;
    for (char y = '1'; y <= '8'; y++) {
        for (char x = 'a'; x <= 'h'; x++) {
            if ((((y - 1) % 2) == 0) == (((x - 97) % 2) == 0)) { //XNOR to help with the diagonalness of the board
                temp[std::make_pair(x, y)] = pieces[Empty];
            }
            //else -> not on board, so doesn't matter
        }
    }
    gameBoard = temp;
}
void customBoardEightPiecesEach(std::map<std::pair<char, char>, char> & gameBoard) {
    std::map<std::pair<char, char>, char> temp;
    char currentPiece = '0';
    for (char y = '1'; y <= '8'; y++) {
        if (y <= '2')
            currentPiece = pieces[Black];
        else if (y >= '7')
            currentPiece = pieces[White];
        else
            currentPiece = pieces[Empty];

        for (char x = 'a'; x <= 'h'; x++) {
            if ((((y - 1) % 2) == 0) == (((x - 97) % 2) == 0)) { //XNOR to help with the diagonalness of the board
                temp[std::make_pair(x, y)] = currentPiece;
            }
            //else -> not on board, so doesn't matter
        }

    }
    gameBoard = temp;
}

void boardReset(std::map<std::pair<char, char>, char> & gameBoard) {
    std::map<std::pair<char, char>, char> temp;
    char currentPiece = '0';
    for (char y = '1'; y <= '8'; y++) {
        if (y <= '3')
            currentPiece = pieces[Black];
        else if (y >= '6')
            currentPiece = pieces[White];
        else
            currentPiece = pieces[Empty];

        for (char x = 'a'; x <= 'h'; x++) {
            if ((((y - 1) % 2) == 0) == (((x - 97) % 2) == 0)) { //XNOR to help with the diagonalness of the board
                temp[std::make_pair(x, y)] = currentPiece;
            }
            //else -> not on board, so doesn't matter
        }

    }
    gameBoard = temp;
}
//Depreciated: Prints board into console output
void showBoard(const std::map<std::pair<char, char>, char> & gameBoard) {
    std::ostringstream buf{};
    std::cout << std::setw(14) << "Checkers" << std::endl;
    buf << "  -------------------" << std::endl;
    for (char y = '8'; y >= '1'; y--) {
        buf << y << " |";
        for (char x = 'a'; x <= 'h'; x++) {
            //std::cout << x << " " << y << std::endl;
            if ((((y - 1) % 2) == 0) == (((x - 97) % 2) == 0)) //XNOR to help with the diagonalness of the board
                buf << " " << gameBoard.at({ x, y });
            else {
                buf << "  ";
            }
            //else -> not on board, so doesn't matter
        }
        buf << " |" << std::endl;
    }
    buf << "Y -------------------" << std::endl;
    buf << "  X";
    for (char x = 'a'; x <= 'h'; x++)
        buf << ' ' << x;
    buf << std::endl;
    std::cout << buf.str();
}
std::vector<std::pair<char, char>> findPiecesRemaining(const int & player,
                                                       const std::map<std::pair<char, char>, char> & gameBoard) {
    std::vector<std::pair<char, char>> tokens;
    for (auto el : gameBoard) {
        if (el.second == pieces[player]) //regular pieces
            tokens.push_back(el.first);
        if (el.second == pieces[player + 1]) //kings
            tokens.push_back(el.first);
    }
    return tokens;
}
//Checks if there are any valid moves for the player specified. If not, there is a stalemate
bool checkStalemate(const int & playerTurn, const std::map<std::pair<char, char>, char> & gameBoard) {
    auto playerPieces = findPiecesRemaining(playerTurn, gameBoard);
    bool moveFound = false;
    for(auto el : playerPieces){
        if( singleSquareMove(el, std::make_pair('z', 'z') /*invalid TO position, as it won't be used*/, gameBoard, true)){
            moveFound = true;
            break;
        }else if (findJumpSquares(playerTurn, el, gameBoard).first.size() > 0){
            moveFound = true;
            break;
        }
    }
    return !moveFound; //If a move was found, there is no stalemate
}
//Promotes tokens to kings if on the last rank of enemy lines
void checkCrown(std::map<std::pair<char, char>, char> & gameBoard) {
    for (auto el : gameBoard) {
        if ((el.first).second == '8')
            if (el.second == pieces[Black])
                gameBoard[el.first] = pieces[BlackKing];
        if ((el.first).second == '1')
            if (el.second == pieces[White])
                gameBoard[el.first] = pieces[WhiteKing];
    }
}

//Makes move if legal, returns errors if it is not
std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> checkMove(const int & player,
                                                                                                             const std::pair<char, char> & from,
                                                                                                             const std::pair<char, char> & to,
                                                                                                             const std::map<std::pair<char, char>, char> & gameBoard)
{
    bool errors = false;
    std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>> returnResult;

    auto fromIt = gameBoard.find(from);
    if (gameBoard.find(from) == gameBoard.end()) { //Check if tiles selected are on the board or not
        std::cout << "Invalid FROM location." << std::endl;
        errors = true;
    }
    else {
        //Ensure player is trying to move their own piece
        if ((fromIt->second != pieces[player]) && (fromIt->second != pieces[player + 1/*king*/])) {
            std::cout << "Invalid FROM location." << std::endl;
            errors = true;
        }
    }

    auto toIt = gameBoard.find(to);
    if (gameBoard.find(to) == gameBoard.end()) { //Check if tiles selected are on the board or not
        std::cout << "Invalid TO location." << std::endl;
        errors = true;
    }
    else {
        //Check if space player is trying to move to is free
        if (toIt->second != pieces[Empty]) {
            std::cout << "Invalid TO location" << std::endl;
            errors = true;
        }
    }

    //Is there a valid jumping path
    if (!errors) {
        if (singleSquareMove(from, to, gameBoard)) {
            errors = false;
            returnResult.first.push_back(to);
        }
        else {
            auto result = jumpPathSearch(from, to, gameBoard);
            if (!result.first) {
                std::cout << "No legal path found." << std::endl;
                errors = true;
            }
            else {
                errors = false;
                returnResult = result.second;
            }
        }
    }
    return { /*Was the move error-free?*/ !errors, returnResult };
}
std::pair<bool, std::pair<std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>>> jumpPathSearch(const std::pair<char, char> & from,
                                                                                                                  const std::pair<char, char> & to,
                                                                                                                  const std::map<std::pair<char, char>, char> & gameBoard){

    std::set<std::pair<char, char>> possibilities {}; //squares that need to be searched
    std::set<std::pair<char, char>> searchedPossibilities{}; //squares that were already searched through

    std::vector<std::pair<char, char>> path{}; //squares jumped to while on way to the "TO" square
    std::vector<std::pair<char, char>> jumpedActual{}; //enemy tokens that were jumped over in the process
    bool pathFound = false;
    int playerPiece = gameBoard.at(from);

    struct Node {
    public:
        std::pair<char, char> square; //Which square this is
        std::stack<std::pair<char, char>> jumpPoints; //Where this node can jump to
        std::pair<char, char> jumpedOver = { 0,0 }; //What enemy token this node has jumped over
        bool searched = false;
        std::shared_ptr<Node> fromPtr = nullptr;
        Node(std::pair<char, char> square, std::shared_ptr<Node> fromPtr, std::pair<char, char> jumpedOver) {
            this->square = square;
            this->fromPtr = fromPtr;
            this->jumpedOver = jumpedOver;
        }
        Node(std::pair<char, char> x) {
            square = x;
        }
        Node() {

        }
    };
    //typedef std::shared_ptr<Node> NodePtr;

    Node current (from);
    possibilities.insert(from);

    //auto temp = findJumpSquares(player, current.square, gameBoard);
    while (possibilities != searchedPossibilities) {
        auto temp = findJumpSquares(playerPiece, current.square, gameBoard);
        if (pathFound) {
            path.push_back(current.square);
            if (current.fromPtr == nullptr) {
                break;
            }
            else {
                jumpedActual.push_back(current.jumpedOver);
                current = *(current.fromPtr);
            }

        }
        else {
            if (!current.searched) {
                //assert(temp.first.size() == temp.second.size());
                for (unsigned int i = 0; i < temp.first.size(); i++) {
                    auto el = temp.first.at(i);
                    current.jumpPoints.push(el);
                    possibilities.insert(el);
                    if (el == to) {
                        path.push_back(el);
                        jumpedActual.push_back(temp.second.at(i));
                        pathFound = true;
                        break;
                    }
                }
                current.searched = true;
            }
            if (!current.jumpPoints.empty()) {
                auto top = current.jumpPoints.top();

                //Cycles through 'tops' that have already been searched
                while (searchedPossibilities.find(top) != searchedPossibilities.end()) {
                    if (!current.jumpPoints.empty()) //avoid crash
                        current.jumpPoints.pop();

                    if (current.jumpPoints.empty()) //avoid crash
                        break;
                    else
                        top = current.jumpPoints.top();
                }
            }
            if (current.jumpPoints.empty()) { //Check if end of the line
                if (current.fromPtr == nullptr) {
                    break;
                }
                else {
                    //std::cout << "Tried: " << current.square.first << current.square.second << std::endl;
                    current = *(current.fromPtr); //return, at end of the line
                }
            }
            else {
                auto top = current.jumpPoints.top();
                current.jumpPoints.pop();
                searchedPossibilities.insert(current.square);
                current = Node(top, std::make_shared<Node>(current), temp.second.at(current.jumpPoints.size()/*-1 todo */)); //search down the branch
            }
        }
    }
    //returns:
    //  boolean-> was a path found?
    //  pair   -> vector of points -> squares that the piece jumped to on the way to the finish
    //  pair   -> vector of points -> enemy pieces squares that were jumped over along the path
    return { pathFound,{ path, jumpedActual } };
}

std::pair< std::vector<std::pair<char, char>>, std::vector<std::pair<char, char>>> findJumpSquares(const char & playerPiece,
                                                                                                   const std::pair<char, char> & square,
                                                                                                   const std::map<std::pair<char, char>, char> & gameBoard) {

    //Return vectors
    std::vector<std::pair<char, char>> output;
    std::vector<std::pair<char, char>> jumped;

    int enemy; //keeps track of which pieces it can jump over legally
    if (playerPiece == pieces[Black] || playerPiece == pieces[BlackKing])
        enemy = White; // 'o'
    else
        enemy = Black; // 'x'

    char possibleJumpedPiece;

    if(playerPiece != pieces[White]){ //pieces that can go forward
        if ((square.first < 'g') && (square.second < '7')) {
            //Top right square
            possibleJumpedPiece = gameBoard.at({ square.first + 1, square.second + 1 });
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1] /*king*/)) {
                if (gameBoard.find({square.first+2, square.second+2})->second == pieces[Empty]){
                    output.push_back({ square.first + 2, square.second + 2 });
                    jumped.push_back({ square.first + 1, square.second + 1 });
                }
            }
        }
        if ((square.first > 'b') && (square.second < '7')) { //Make sure we are jumping over an enemy piece
            //Top left square
            possibleJumpedPiece = gameBoard.at({ square.first - 1, square.second + 1 });
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1])) {
                if (gameBoard.find({square.first-2, square.second+2})->second == pieces[Empty]){
                    output.push_back({ square.first - 2, square.second + 2 });
                    jumped.push_back({ square.first - 1, square.second + 1 });
                }
            }
        }
    }

    if(playerPiece != pieces[Black]){ //pieces that can go backward
        if ((square.first < 'g') && (square.second > '2')) {
            //Bottom right square
            possibleJumpedPiece = gameBoard.at({ square.first + 1, square.second - 1 });
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1])) {
                if (gameBoard.find({square.first+2, square.second-2})->second == pieces[Empty]){
                    output.push_back({ square.first + 2, square.second - 2 });
                    jumped.push_back({ square.first + 1, square.second - 1 });
                }
            }
        }

        if ((square.first > 'b') && (square.second > '2')) {
            //Bottom left piece
            possibleJumpedPiece = gameBoard.at({ square.first - 1, square.second - 1 });
            if ((possibleJumpedPiece == pieces[enemy]) || (possibleJumpedPiece == pieces[enemy + 1])) {
                if (gameBoard.find({square.first-2, square.second-2})->second == pieces[Empty]){
                    output.push_back({ square.first - 2, square.second - 2 });
                    jumped.push_back({square.first - 1, square.second - 1});
                }
            }
        }
    }

    return { output, jumped };
}
//Checks to see if the move is a single square away and if it is valid.
//Alternatively, if findAllSquares is true, returns if there are any valid single square moves.
bool singleSquareMove(const std::pair<char, char> & from,
                      const std::pair<char, char> & to,
                      const std::map<std::pair<char, char>, char> & gameBoard,
                      const bool & findAllSquares /* = false */) {

    char playerPiece = gameBoard.find(from)->second;
    int amtSquares = 0;
    if (playerPiece == pieces[Black] || playerPiece == pieces[BlackKing] || playerPiece == pieces[WhiteKing]) { //Pieces that can move up
        //top right square
        if(findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first + 1, from.second + 1));
            if(it != gameBoard.end()){
                if(it->second == pieces[Empty]){ //if the square is empty
                    amtSquares++;
                }
            }
        }
        else if ((from.first + 1) == (to.first)){
            if ((from.second + 1) == (to.second)){
                return true;
            }
        }

        //top left square
        if(findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first - 1, from.second + 1));
            if(it != gameBoard.end()){
                if(it->second == pieces[Empty]){ //if the square is empty
                    amtSquares++;
                }
            }
        }
        else if ((from.first - 1) == (to.first)){
            if ((from.second + 1) == (to.second)){
                return true;
            }
        }
    }
    if (playerPiece == pieces[White] || playerPiece == pieces[BlackKing] || playerPiece == pieces[WhiteKing]) { // Pieces that can move up
        //bottom left square
        if(findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first - 1, from.second - 1));
            if(it != gameBoard.end()){
                if(it->second == pieces[Empty]){ //if the square is empty
                    amtSquares++;
                }
            }
        }
        else if ((from.first - 1) == (to.first)){
            if ((from.second - 1) == (to.second)){
                return true;
            }
        }

        //bottom right square
        if(findAllSquares) {
            auto it = gameBoard.find(std::make_pair(from.first + 1, from.second - 1));
            if(it != gameBoard.end()){
                if(it->second == pieces[Empty]){ //if the square is empty
                    amtSquares++;
                }
            }
        }
        else if ((from.first + 1) == (to.first)){
            if ((from.second - 1) == (to.second)){
                return true;
            }
        }

    }

    if (!findAllSquares) //If we were searching for a specific move
        return false; //If the move was not found
    else
        return (amtSquares > 0); //If we were searching for any available move
}
//Overwrites any tokens on the specified square. Used for deleting "jumped over" tokens.
void removeSquare(const std::pair<char, char> & square, std::map<std::pair<char, char>, char> & gameBoard) {
    gameBoard.at(square) = pieces[Empty];
}
//Moves the piece without checking if the move is legal.
void movePiece(std::pair<char, char> &from,
               std::pair<char, char> &to,
               std::map<std::pair<char, char>, char> & gameBoard) {

    gameBoard.at(to) = gameBoard.at(from);
    gameBoard.at(from) = pieces[Empty];
}
//Checks if players have pieces remaining to play
int win(std::map<std::pair<char, char>, char> & gameBoard, int & playerTurn){
    if ((findPiecesRemaining(White, gameBoard)).size() == 0) {
        if ((findPiecesRemaining(Black, gameBoard)).size() == 0){
            return Draw;
        }else{
            return BlackWin;
        }
    }
    else if ((findPiecesRemaining(Black, gameBoard)).size() == 0) {
        return WhiteWin;
    }
    else if (checkStalemate(playerTurn, gameBoard)) {
        if(checkStalemate( ((playerTurn == Black) ? White : Black), gameBoard ))
            return Draw; //Both are at stalemate
        return ((playerTurn == Black) ? WhiteWin : BlackWin);
    }
    else{
        return ValidMove;
    }
}
//Handles the player taking their turn
int changeTurn(std::map<std::pair<char, char>, char> & gameBoard,
             std::pair<std::pair<char, char>, std::pair<char, char>> playerMove,
             int & playerTurn)
{
    auto result = checkMove(playerTurn, playerMove.first, playerMove.second, gameBoard);

    if (result.first) { //if the move was legal
        movePiece(playerMove.first, playerMove.second, gameBoard);
        for (auto el: result.second.second) { //delete any "jumped" tokens
            removeSquare(el, gameBoard);
        }
    }
    else {
        return InvalidMove;
    }

    //It is now the other player's turn
    if (playerTurn == Black)
        playerTurn = White;
    else
        playerTurn = Black;

    checkCrown(gameBoard);

    return win(gameBoard, playerTurn);
}

#endif
