#ifndef SQUARE_H
#define SQUARE_H
#include <QGraphicsItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "main.h"

class BoardSquare : public QGraphicsItem
{
private:
    int x = 0;
    int y = 0;
    std::pair<char, char> square;
    QColor colour = Qt::black;
    bool dragOver = false;


public:
    BoardSquare(int x, int y, std::pair<char, char> square){
        this->x = x;
        this->y = y;
        this->square = square;
        setAcceptDrops(true);
    }
    void setPos(int x, int y){
        this->x = x;
        this->y = y;
    }
    std::pair<int, int> get(){
        return std::make_pair(x,y);
    }

private:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event){
        if (event->mimeData()->hasText()) {
            event->setAccepted(true);
            dragOver = true;
            update();
        } else {
            event->setAccepted(false);
        }
    }
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event){
        Q_UNUSED(event);
        dragOver = false;
        update();
    }
    void dropEvent(QGraphicsSceneDragDropEvent *event){
        dragOver = false;
        event->setAccepted(true);
        if (event->mimeData()->hasText()){
            std::string s = event->mimeData()->text().toStdString();
            redrawBoard(std::make_pair(char(s.at(0)), char(s.at(1))), square, scene());
        }else{
            update();
        }
    }

    QRectF boundingRect() const
    {
        return QRectF(x, y, 75, 75);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0){
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setBrush(dragOver ? Qt::lightGray: colour);
        painter->drawRect(boundingRect());
    }
};


#endif
