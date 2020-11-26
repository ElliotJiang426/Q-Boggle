#include "BoggleWindow.h"

#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>

static int top;
BoggleWindow::BoggleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    board = new Board(this);
    console = new Console(this);

    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }

    lex=new Lexicon(qFile);
    human_lex=new Lexicon();
    computer_lex=new Lexicon();

    me->setGeometry(20, 20, 230, 300);
    board->setGeometry(230, 0, 300, 300);
    computer->setGeometry(800 - 50 - 200, 20, 230, 300);
    console->setGeometry(30, 320, 740, 260);

    /*
    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
    Lexicon lex(qFile);
    */

    /*
    for (std::string s: lex) {
        std::cout << s << std::endl;
    }
    */
    console->write("Welcome to the game Boggle!\n");
    connect(console,SIGNAL(newLineWritten(QString)),this,SLOT(human_turn(QString)));
}

BoggleWindow::~BoggleWindow()
{

}
void BoggleWindow::human_turn(QString Line){
    //先转换为小写并去除前后空白
    Line=Line.trimmed().toLower();

    //为空，结束human_turn，开始computer_turn
    if(Line.isEmpty()){
        this->computer_turn();
        return;
    }

    //长度是否满足条件
    if(Line.length()<4){
        this->console->write("The length of the word is too short(<4)!");
        return;
    }
    //是否在词典中
    const std::string testLine=Line.toStdString();
    if(!this->lex->contains(testLine)){
        this->console->write("The word is not contained in the lexicon!");
        return;
    }
    //是否已经在wordlist中
    if(this->human_lex->contains(testLine)){
        this->console->write("The word is repeated!");
        return ;
    }
    //是否能在board中生成

    top=-1;
    for(int i=0;i<this->board->size;i++){
        for(int j=0;j<this->board->size;j++){
            if(this->board->letters[index(i,j)].toLower()==Line[0]){
                //地图初始化
                //console->write(QString::number(i)+ " "+QString::number(j));
                for(int i1=0;i1<this->board->size;i1++)
                    for(int j1=0;j1<this->board->size;j1++)
                        this->board->map[index(i1,j1)]=true;
                //利用回溯法进行判断
                if(find_in_board(i,j,Line)){
                    this->console->write("Congratulations!");
                    this->me->addScore(Line.length()-3);
                    this->me->addWord(Line);
                    this->human_lex->add(testLine);
                    return;
                }
            }
        }
    }

    this->console->write("The word can't be formed on the board!");

}
bool BoggleWindow::find_in_board(int beginx,int beginy,QString goal){
    int curx,cury,curd;
    QString temp;
    top++;
    //初始方块入栈
    this->board->path[top].x=beginx;
    this->board->path[top].y=beginy;
    this->board->path[top].d=-1;
    this->board->map[index(beginx,beginy)]=false;

    while(top>-1){
        curx=this->board->path[top].x;
        cury=this->board->path[top].y;
        curd=this->board->path[top].d;
        temp.append(this->board->letters[index(curx,cury)].toLower());
        //console->write(QString::number(curx)+ " "+QString::number(cury));
        if(temp==goal){
            return true;
        }
        int flag=0;
        while(curd<8 && flag==0){
            curd++;
            switch (curd) {
            case 0://左
                cury=this->board->path[top].y-1;curx=this->board->path[top].x;break;
            case 1://右
                cury=this->board->path[top].y+1;curx=this->board->path[top].x;break;
            case 2://上
                curx=this->board->path[top].x-1;cury=this->board->path[top].y;break;
            case 3://下
                curx=this->board->path[top].x+1;cury=this->board->path[top].y;break;
            case 4://左上
                cury=this->board->path[top].y-1;curx=this->board->path[top].x-1;break;
            case 5://右上
                cury=this->board->path[top].y+1;curx=this->board->path[top].x-1;break;
            case 6://左下
                cury=this->board->path[top].y-1;curx=this->board->path[top].x+1;break;
            case 7://右下
                cury=this->board->path[top].y+1;curx=this->board->path[top].x+1;break;
            }
            if(curx>=0&&curx<this->board->size&&cury>=0&&cury<this->board->size&&this->board->letters[index(curx,cury)].toLower()==goal[top+1]&&this->board->map[index(curx,cury)])
                flag=1;
        }
        if(flag){
            this->board->path[top].d=curd;
            top++;
            this->board->path[top].x=curx;
            this->board->path[top].y=cury;
            this->board->path[top].d=-1;
            this->board->map[index(curx,cury)]=false;
        }else{
            this->board->map[index(this->board->path[top].x,this->board->path[top].y)]=true;
            temp=temp.left(temp.length()-1);
            top--;
        }
    }
    return false;
}

void BoggleWindow::computer_turn(){
    console->write("Witness the superiority of silicon!");
    for(int i=0;i<this->board->size;i++){
        for(int j=0;j<this->board->size;j++){
            //地图初始化
            for(int i1=0;i1<this->board->size;i1++)
                for(int j1=0;j1<this->board->size;j1++)
                    this->board->map[index(i1,j1)]=true;
            //回溯法搜索所有单词
            //console->write(QString::number(i)+ " "+QString::number(j));
            top=-1;
            this->search_word(i,j);
        }
    }
}

void BoggleWindow::search_word(int beginx, int beginy){
    int curx,cury,curd;
    QString formed_word;
    //初始方块入栈
    top++;
    this->board->path[top].x=beginx;
    this->board->path[top].y=beginy;
    this->board->path[top].d=-1;
    this->board->map[index(beginx,beginy)]=false;

    while(top>-1){
        curx=this->board->path[top].x;
        cury=this->board->path[top].y;
        curd=this->board->path[top].d;

        formed_word.append(this->board->letters[index(curx,cury)].toLower());

       // console->write(QString::number(curx)+ " "+QString::number(cury)+formed_word);
        if(this->check_validity(formed_word)){
            this->computer->addScore(formed_word.length()-3);
            this->computer->addWord(formed_word);
            this->computer_lex->add(formed_word.toStdString());
        }
        int flag=0;
        while(curd<8 && flag==0){
            curd++;
            switch (curd) {
            case 0://左
                cury=this->board->path[top].y-1;curx=this->board->path[top].x;break;
            case 1://右
                cury=this->board->path[top].y+1;curx=this->board->path[top].x;break;
            case 2://上
                curx=this->board->path[top].x-1;cury=this->board->path[top].y;break;
            case 3://下
                curx=this->board->path[top].x+1;cury=this->board->path[top].y;break;
            case 4://左上
                cury=this->board->path[top].y-1;curx=this->board->path[top].x-1;break;
            case 5://右上
                cury=this->board->path[top].y+1;curx=this->board->path[top].x-1;break;
            case 6://左下
                cury=this->board->path[top].y-1;curx=this->board->path[top].x+1;break;
            case 7://右下
                cury=this->board->path[top].y+1;curx=this->board->path[top].x+1;break;
            }
            formed_word.append(this->board->letters[index(curx,cury)].toLower());
            //利用prune进行剪枝
            if(curx>=0&&curx<this->board->size&&cury>=0&&cury<this->board->size&&this->board->map[index(curx,cury)]&&this->lex->containsPrefix(formed_word.toStdString()))
                flag=1;

            formed_word=formed_word.left(formed_word.length()-1);
        }

        if(!flag){
            this->board->path[top].d=curd;
            top++;
            this->board->path[top].x=curx;
            this->board->path[top].y=cury;
            this->board->path[top].d=-1;
            this->board->map[index(curx,cury)]=false;
        }else{
            this->board->map[index(this->board->path[top].x,this->board->path[top].y)]=true;
            formed_word=formed_word.left(formed_word.length()-1);
            top--;
        }
   }
}
bool BoggleWindow::check_validity(QString word){
    if(word.length()<4||!this->lex->contains(word.toStdString())||this->human_lex->contains(word.toStdString())||this->computer_lex->contains(word.toStdString())){
        return false;
    }
    return true;
}
