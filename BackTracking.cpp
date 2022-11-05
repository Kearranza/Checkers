#include <QRandomGenerator64>
#include "BackTracking.h"
#include "Game.h"
#include <QtGlobal>
#include <fstream>

std::pair<int, std::pair<char, char>> findBestJumpMoveAI(const std::map<std::pair<char, char>, char> &gameBoard,
                                                         const std::pair<char, char> &from){

    std::set<std::pair<char, char>> possibilities {}; //squares that need to be searched
    std::set<std::pair<char, char>> searchedPossibilities{}; //squares that were already searched through

    int playerPiece = gameBoard.at(from);

    //Structure that contains a position, the possibilities for the next step and if the enemy has been there
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

    Node current (from);//Create a new node
    possibilities.insert(from);//Insert a square in possibilities

    int jumpedPieceAmount = 0;
    std::map<int, std::vector<std::pair<char, char>>> valueOfSquare{}; //Stores squares based on how many enemies they have jumped over.
    valueOfSquare[0].push_back(from);

    while (possibilities != searchedPossibilities) {
        auto temp = findJumpSquares(playerPiece, current.square, gameBoard);

        if (!current.searched) {
            for (unsigned int i = 0; i < temp.first.size(); i++) {
                auto el = temp.first.at(i);
                current.jumpPoints.push(el);
                possibilities.insert(el);
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
            if (current.fromPtr == nullptr) {//Avoid end
                break;
            }
            else {
                valueOfSquare[jumpedPieceAmount].push_back(current.square);
                current = *(current.fromPtr); //return, at end of the line
                jumpedPieceAmount--;
            }
        }
        else {
            auto top = current.jumpPoints.top();
            current.jumpPoints.pop();
            searchedPossibilities.insert(current.square);
            current = Node(top, std::make_shared<Node>(current), temp.second.at(current.jumpPoints.size()/*-1 todo */)); //search down the branch
            jumpedPieceAmount++;
        }

    }

    std::pair<char, char> to = from;
    int val = 0;
    for (auto it = valueOfSquare.begin(); it != valueOfSquare.end(); ++it){
        if (it->first > val){
            val = it->first;
            int randomIndex = rand() % it->second.size();
            to = it->second.at(randomIndex);
        }
    }

    ofstream text;


    if(val == 0){
        return {0, to};
    }
    else
        return { val, to };
}

std::pair<char, char> findSingleSquareMoveAI(const std::map<std::pair<char, char>, char> & gameBoard,
                                             const std::pair<char, char> & from) {

    std::vector<std::pair<char, char>> possibleMoves {};
    char playerPiece = gameBoard.find(from)->second;
    auto it = gameBoard.find(std::make_pair(from.first + 1, from.second + 1));

    if (playerPiece == pieces[Black] || playerPiece == pieces[BlackKing]) { //Pieces that can move up
        //top right square
        it = gameBoard.find(std::make_pair(from.first + 1, from.second + 1));
        if(it != gameBoard.end()){
            if(it->second == pieces[Empty]){ //if the square is empty
                possibleMoves.push_back(it->first);
            }
        }

        //top left square
        it = gameBoard.find(std::make_pair(from.first - 1, from.second + 1));
        if(it != gameBoard.end()){
            if(it->second == pieces[Empty]){ //if the square is empty
                possibleMoves.push_back(it->first);
            }
        }

    }
    if (playerPiece == pieces[BlackKing]) { // Pieces that can move up
        //bottom left square
        it = gameBoard.find(std::make_pair(from.first - 1, from.second - 1));
        if(it != gameBoard.end()){
            if(it->second == pieces[Empty]){ //if the square is empty
                possibleMoves.push_back(it->first);
            }
        }
        //bottom right square
        it = gameBoard.find(std::make_pair(from.first + 1, from.second - 1));
        if(it != gameBoard.end()){
            if(it->second == pieces[Empty]){ //if the square is empty
                possibleMoves.push_back(it->first);
            }
        }
    }
    if(possibleMoves.size() == 0){
        return {'z', 'z'};
    }else{
        int randomIndex = rand() % possibleMoves.size();
        return possibleMoves.at(randomIndex);
    }
}

std::pair<std::pair<char, char>, std::pair<char, char>> getMoveAI(std::map<std::pair<char, char>, char> & gameBoard,
               int & playerTurn)
{
    //find all the pieces available to move
    std::vector<std::pair<char, char>> pieceVec;
    for (auto it = gameBoard.begin(); it != gameBoard.end(); ++it){
        if (it->second == pieces[playerTurn] || it->second==pieces[playerTurn+1]){
            pieceVec.push_back(it->first);
        }
    }
    if (pieceVec.size() == 0){ // sanity check
        std::cout<<"Programmer error: AI has no pieces to move."<<std::endl;
        throw "Programmer error: AI has no pieces to move.";
    }

    //Generates a vector Best Move, that contains the movement to make
    std::pair<std::pair<char, char>, std::pair<char, char>> bestMove = std::make_pair(std::make_pair('z', 'z'), std::make_pair('z', 'z'));
    int val = 0;
    std::vector<std::pair<std::pair<char, char>, std::pair<char, char>>> possibleMoves {};
    for(auto it = pieceVec.begin(); it != pieceVec.end(); ++it){//Search in all the available pieces to move
        auto temp = findBestJumpMoveAI(gameBoard, *it);//Calls findBestJumpMoveAI
        if(temp.first > val){ //Sees if the movement is better than the past one
            std::vector<std::pair<std::pair<char, char>, std::pair<char, char>>> newPossibleMoves;
            newPossibleMoves.push_back(std::make_pair(*it, temp.second));
            val = temp.first;
            possibleMoves = newPossibleMoves;
        }else if (temp.first == val){
            possibleMoves.push_back(std::make_pair(*it, temp.second));
        }
    }

    if(val>0){//Assign a possible move from the best possibleMoves vector list
        int randomIndex = rand() % possibleMoves.size();
        bestMove = possibleMoves.at(randomIndex);
    }

    else { //Don't bother looking for a single square if a jump square was found
        std::vector<std::pair<std::pair<char, char>, std::pair<char, char>>> possibleSingleMoves {};
        for(auto it = pieceVec.begin(); it != pieceVec.end(); ++it){
            std::pair<char, char> temp = findSingleSquareMoveAI(gameBoard, *it);
            if (temp != std::make_pair('z', 'z')){
                possibleSingleMoves.push_back(std::make_pair(*it, temp));
            }
        }

        int randomIndex = rand() % possibleSingleMoves.size();
        bestMove = possibleSingleMoves.at(randomIndex);

    }
    if (bestMove.second == std::make_pair('z', 'z')){ // Ensure a move has been found
        std::cout<<"Programmer error: AI has no valid moves."<<std::endl;
        throw "Programmer error: AI has no valid moves.";
    }

    return bestMove;
}