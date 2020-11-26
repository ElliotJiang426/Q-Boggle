#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
#include "WordListWidget.h"
#include "lexicon.h"
#define index(x,y) ((x)*5+(y))

#include <QMainWindow>

class BoggleWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void human_turn(QString Line);
    void computer_turn();
public:
    BoggleWindow(QWidget *parent = 0);
    ~BoggleWindow();
private:
    WordListWidget *me;
    WordListWidget *computer;
    Board *board;
    Console *console;
    Lexicon *lex;
    Lexicon *human_lex;
    Lexicon *computer_lex;
    bool find_in_board(int beginx,int beginy,QString goal);
    void search_word(int beginx,int beginy);
    bool check_validity(QString word);


    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;
};

#endif // BOGGLEWINDOW_H
