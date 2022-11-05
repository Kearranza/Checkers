#ifndef MAIN_H
#define MAIN_H
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsSceneDragDropEvent>

namespace CV{static const std::vector<std::string> gameStateVector = {"Invalid Move", "" /*Valid Move*/, "White Wins", "Black Wins", "Draw", };}

typedef enum BoardLayout{
    Standard = 1,
    Kings,
    Jumpalicious,
    TwoRows,
    CustomBoardCreate,
    CustomBoardPlay
}BoardLayout;

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene){
        setAcceptDrops(true);
    }

private:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event){
        Q_UNUSED(event);
    }
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event){
        Q_UNUSED(event);
        update();
    }
    void dropEvent(QGraphicsSceneDragDropEvent *event){
        event->setAccepted(true);
        update();
    }
};

void drawSceneBoard( QGraphicsScene & scene);
void drawScenePieces(QGraphicsScene & scene, std::map<std::pair<char, char>, char> & gameBoard);
void redrawBoard(std::pair<char, char> from, std::pair<char, char> to, QGraphicsScene * scene);
int main(int argc, char *argv[]);

#endif
