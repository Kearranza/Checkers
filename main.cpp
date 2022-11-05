#include <QTimer>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>

#include "main.h"
#include "Game.h"
#include "MovePiece.h"
#include "Square.h"
#include "Check.h"
#include "BackTracking.h"
#include <map>

namespace CV{
    std::map<std::pair<char, char>, char> gameBoard;
    std::map<std::pair<char, char>, char> userCreatedBoard;

    int playerTurn = White; //White goes first
    int boardLayout = Standard;
    int gameStatus = ValidMove; //Displays in top right hand corner

    QString movesListString = QString("White\tBlack\n"); //keeps a list of the moves taken
    QString movesListString2 = QString("White\tBlack\n"); //for the second column if the first fills up
}
namespace CF{
    bool resetFlag = false; //Reset the game
    bool refreshFlag = false; //Refresh the scene

    bool whiteAIFlag = false; //Is white AI-controlled?
    bool blackAIFlag = false; //Is black AI-controlled?
    bool playerMovingFlag = false; //Avoids interrupting
}

void drawSceneBoard( QGraphicsScene & scene){
    int xOffset = 10;
    int yOffset = 85;


    QGraphicsTextItem * playerTurnText = scene.addText( (CV::playerTurn == White) ? QString("White to move") : QString("Black to move") );
    playerTurnText->setFont(QFont("Times New Roman", 16));
    playerTurnText->setPos(0, 30);

    //Displays if the move was valid or if a colour has won
    QGraphicsTextItem * displayBar = scene.addText(QString(CV::gameStateVector.at(CV::gameStatus).c_str()));
    displayBar->setFont(QFont("Times New Roman", 22));
    displayBar->setPos(620+75, 30);

    //Displays the moves that have been made this game
    QGraphicsTextItem * movesList = scene.addText(CV::movesListString);
    movesList->setFont(QFont("Times", 12));
    movesList->setPos(620+75, 150);
    //movesList->setTextWidth(100);
    movesList->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    if(CV::movesListString.size() >= 694){ //if the text gets too long, off the window, start a new column
        QGraphicsTextItem * movesList2 = scene.addText(CV::movesListString2);
        movesList2->setFont(QFont("Times", 12));
        movesList2->setPos(800+75, 150);
        movesList2->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    }

    //reset button
    QPushButton *resetButton = new QPushButton;
    QObject::connect(resetButton, &QPushButton::clicked, [](){CF::resetFlag = true;});
    resetButton->setFont(QFont("Times New Roman", 14));
    resetButton->setGeometry(QRect(620 + 75, 75, 120, 30));
    resetButton->setText("Reset Game");
    scene.addWidget(resetButton);

    QGraphicsItem *BackdropItem = new Backdrop(); //can accept drops and return an error if the user misses dropping on a valid square
    scene.addItem(BackdropItem);

    //Prints the black squares of the board
    for (char y = '1'; y <= '8'; y++){
        for (char x = 'a'; x <= 'h'; x++){
            if ((((y - 49) % 2) == 0) == (((x - 97) % 2) == 0)){  //XNOR to help with the diagonalness of the board
                QGraphicsItem *boardSquareItem = new BoardSquare((x-97)*75 + 75, 525-(y-49)*75 + yOffset, std::make_pair(x, y));
                //visualBoard[std::make_pair(x, y)] = boardSquareItem;
                scene.addItem(boardSquareItem);
            //else -> is a white square, so doesn't matter
            }
        }
    }

    //Border rectangle
    scene.addRect(75, yOffset, 600, 600);

    //Draws the row numbers
    for(int i = 0; i < 8; i++){
        QGraphicsSimpleTextItem * number = new QGraphicsSimpleTextItem();
        number->setFont(QFont("Times",55));
        number->setPos(0, (75 * i)+yOffset-10);
        std::string s{};
        s += '8' - i;
        number->setText(QString(s[0]));
        scene.addItem(number);
    }
    //Draws the column letters

    for(int i = 0; i < 8; i++){
        QGraphicsSimpleTextItem * letter = new QGraphicsSimpleTextItem();
        letter->setFont(QFont("Times",55));
        letter->setPos(xOffset + 75 + (75 * i), 600 + yOffset);
        std::string s{};
        s += 'A' + i;
        letter->setText(QString(s[0]));
        scene.addItem(letter);
    }
}
void drawScenePieces(QGraphicsScene & scene, std::map<std::pair<char, char>, char> & gameBoard){
    int yOffset = 85;
    QColor color = Qt::white;
    for (char y = '1'; y <= '8'; y++) {
        for (char x = 'a'; x <= 'h'; x++) {
            if ((((y-49) % 2) == 0) == (((x - 97) % 2) == 0)) { //Helps with the diagonalness of the board
                char piece = gameBoard[std::make_pair(x,y)];
                bool king = false;
                //std::cout<<"Found ["<<piece<<"] at "<<x<<","<<y<<std::endl;
                if (piece == pieces[Black]){
                    color = QColor::fromRgb(101,70,50);
                }else if(piece == pieces[BlackKing]){
                    color = QColor::fromRgb(101,70,50);
                    king = true;
                }else if (piece == pieces[White]){
                    color = QColor::fromRgb(251,228,122);
                }else if (piece == pieces[WhiteKing]){
                    color = QColor::fromRgb(251,228,122);
                    king = true;
                }else{
                    continue;
                }
                QGraphicsItem *gamePieceItem = new GamePiece((x-97)*75 + 75, 525 - (y-49)*75+yOffset, color, std::make_pair(x, y), king);
                scene.addItem(gamePieceItem);
            }
        }
    }
}
void redrawBoard(std::pair<char, char> from, std::pair<char, char> to, QGraphicsScene * scene){
    CF::playerMovingFlag = true;
    std::cout<<"Move: "<<char(from.first)<<char(from.second)<<"->"<<char(to.first)<<char(to.second)<<std::endl;
    scene->clear();
    if(CV::gameStatus != WhiteWin && CV::gameStatus != BlackWin && CV::gameStatus != Draw){ //if the game is running

        CV::gameStatus = changeTurn(CV::gameBoard, std::make_pair(from, to), CV::playerTurn);

        if (CV::gameStatus != ValidMove)
            std::cout<< CV::gameStateVector.at(CV::gameStatus)<<std::endl;

        if(CV::gameStatus != InvalidMove){
            std::stringstream ss {};
            ss << char(toupper(from.first)) << from.second << " -> " << char(toupper(to.first)) << to.second << " ";
            if(CV::gameStatus == WhiteWin || CV::gameStatus == BlackWin || CV::gameStatus == Draw)
                ss<<"#";
            else if (CV::playerTurn == White) //If the next player's turn is white
                ss << "\n";
            else
                ss <<"| ";

            if(CV::movesListString.size() < 700)
                CV::movesListString += QString(ss.str().c_str());
            else
                CV::movesListString2 += QString(ss.str().c_str());
        }
    }
    drawSceneBoard(*scene);
    drawScenePieces(*scene, CV::gameBoard);
    CF::playerMovingFlag = false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int width = 1920;
    int height = 1080;
    QGraphicsScene scene(0,0, width, height);
    boardReset(CV::gameBoard);
    drawSceneBoard(scene);
    drawScenePieces(scene, CV::gameBoard);

    GraphicsView view(&scene);
    QRectF bounds = scene.itemsBoundingRect();
    bounds.setWidth(bounds.width()*0.9);
    bounds.setHeight(bounds.height()*0.9);

    view.fitInView(bounds, Qt::KeepAspectRatio);

    view.setRenderHint(QPainter::Antialiasing);
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setBackgroundBrush(QColor(255,255,255));
    view.setWindowTitle("Checkers");
    view.showMaximized();

    QTimer *timer = new QTimer;
    QObject::connect(timer, &QTimer::timeout, [&scene](){
        if(CF::resetFlag){
            switch(CV::boardLayout){
            case Standard:
                boardReset(CV::gameBoard);
                break;
            }
            checkCrown(CV::gameBoard);
            CV::gameStatus = win(CV::gameBoard, CV::playerTurn);
            CV::playerTurn = White;
            scene.clear();
            CV::movesListString = QString("White\tBlack\n");
            CV::movesListString2 = QString("White\tBlack\n");
            drawSceneBoard(scene);
            drawScenePieces(scene, CV::gameBoard);
            CF::resetFlag = false;
        }
        else if(!CF::playerMovingFlag && CV::gameStatus != WhiteWin && CV::gameStatus != BlackWin && CV::gameStatus != Draw){      
            if(true && CV::playerTurn == Black){ //If it's Blacks's turn and an AI is controlling it
                auto move = getMoveAI(CV::gameBoard, CV::playerTurn);
                redrawBoard(move.first, move.second, &scene);
            }
        }

    });
    timer->start(100);
    view.show();
    return a.exec();
}
