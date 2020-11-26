#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"
#include <QTime>
#include <QWidget>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = 5, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    void shake();
    QString *letters;

    bool *map;
    struct node{
        int x,y,d;
    };
    node *path;
    int size;

signals:

public slots:

private:
    Cube **cubes;

    inline int index(int i, int j) { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];   


};

#endif // BOARD_H
