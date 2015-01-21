#include "dialog.h"
#include "ui_dialog.h"
#include <QtWidgets>
#include <QDebug>
#include <QApplication>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    newGame();
}

Dialog::~Dialog()
{
    delete ui;
}
QList<QString> Dialog::ReadWords()
{
    QList<QString> wordList;
    QString filename=":files/words.txt";
    QFile wordFile(filename);
    wordFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&wordFile);
    QString word;
    while(!wordFile.atEnd())
    {
        in>>word;
        wordList.append(word);
    }
    wordList.append(word);
    wordFile.close();
    return wordList;
}

QString Dialog::ChooseWord()
{
    srand (time(NULL));
    QList<QString> *wordList = new QList<QString>(ReadWords());

    QString word=wordList->value(rand() % wordList->length());
    return word;
}

void Dialog::GuessLetter(Button* button)
{
    QString letter = button->text().at(0);
    lettersGuessed->append(letter);
    if(!chosenWord.contains(letter, Qt::CaseInsensitive))
        guessesLeft-=1;
}
void Dialog::reset()
{
    QLayoutItem* item;
    if ( hangmanLayout != NULL )
    {

        while ( ( item = hangmanLayout->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete hangmanLayout;
    }
    if ( wordLayout != NULL )
    {

        while ( ( item = wordLayout->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete wordLayout;
    }
    if ( layout != NULL )
    {
        while ( ( item = layout->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete layout;
    }
    newGame();
}

void Dialog::createGameOver()
{


    exitDialog = new QDialog(this);
    exitLayout = new QHBoxLayout(exitDialog);
    exitLabel = new QLabel("Game Over", exitDialog);
    exitLabel->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    exitLabel->setFont(QFont("Chalkduster", 50));

    //exitLabel->setStyleSheet(" color:white ");

    exitLayout->addWidget(exitLabel);
    exitLayout->setAlignment(exitLabel, Qt::AlignCenter);
    qDebug()<<exitLabel->height()<<"*"<<exitLabel->width();
    gameOver = new QMessageBox(this);
    gameOver->setText("Sorry you ran out of guesses. The word was "+chosenWord);
    gameOver->addButton(QMessageBox::Close);
    //gameOver->addButton(QMessageBox::Reset);
    //gameOver->button(QMessageBox::Reset)->setText("New Game");
    gameOver->setIcon(QMessageBox::Warning);
    //connect(gameOver->button(QMessageBox::Close),SIGNAL(clicked()),QApplication::instance(),SLOT(quit()));
    //connect(gameOver->button(QMessageBox::Reset),SIGNAL(clicked()),this,SLOT(reset()));
}

void Dialog::CreateButtons()
{
    QFont f( "American Typewriter" , 10, QFont::Bold);

    for(int i=0; i<26; i++)
    {


        button[i] = new Button(this);
        button[i]->setFont(f);
        button[i]->setText(QString(i+65));
        button[i]->setMinimumHeight(75);
        button[i]->setMinimumWidth(50);
        button[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        if(i<9)
            layout->addWidget(button[i],2,i);
        else if(i>=9 && i<18)
            layout->addWidget(button[i],3,i-9);
        else
            layout->addWidget(button[i],4,i-18);
        connect(button[i],SIGNAL(pressed()),this,SLOT(slotButtonClicked()));





    }

}

void Dialog::UpdateLabels()
{

    for(int i=0; i<chosenWord.length(); i++)
    {

        if(lettersGuessed->contains(QString(chosenWord.at(i)).toUpper()))
        {



            letterLabel[i]->setText(chosenWord.at(i));
        }
        else
        {
            letterLabel[i]->setText("_");
        }




        letterLabel[i]->show();

    }
    guessesLeftBar->setValue(guessesLeft);
    hangman->load(hangmanPics[10-guessesLeft]);
    *hangman=hangman->scaled(hangmanLabel->width(),hangmanLabel->height());
    //*hangman=hangman->scaled(1000,1000,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    qDebug()<<hangmanLabel->width()<<" "<<hangmanLabel->height();
    hangmanLabel->setPixmap(*hangman);


}
void Dialog::createGameWon()
{
    gameWon = new QMessageBox(this);
    gameWon->setText("You won");
    gameWon->addButton(QMessageBox::Close);
    //gameWon->addButton(QMessageBox::Reset);
    //gameWon->button(QMessageBox::Reset)->setText("New Game");
    gameWon->setIcon(QMessageBox::Information);

    //connect(gameWon->button(QMessageBox::Close),SIGNAL(clicked()),this,SLOT(close()));
    //connect(gameWon->button(QMessageBox::Reset),SIGNAL(clicked()),this,SLOT(newGame()));
}

void Dialog::slotButtonClicked()
{

    Button* temp = qobject_cast<Button* >(QObject::sender());
    GuessLetter(temp);
    temp->setDisabled(true);
    UpdateLabels();
    if(guessesLeft==0)
    {
        gameOver->exec();
        exitDialog->showMaximized();
        this->hide();
    }
    else if(isGameWon())
    {
        gameWon->exec();
    }

}

bool Dialog::isGameWon()
{
    int flag=1;
    for(int i=0; i<chosenWord.length(); i++)
    {
        if(lettersGuessed->contains(QString(chosenWord.at(i)).toUpper())==false)
            flag=0;

    }
    if(flag==1)
        return true;
    return false;
}


void Dialog::newGame()
{

    QFontDatabase::addApplicationFont(":/fonts/Chalkduster.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Herculanum.ttf");
    QFontDatabase::addApplicationFont(":/fonts/AmericanTypewriter.ttc");

    ui->setupUi(this);
    this->setStyleSheet("QDialog {background-image: url(:/pics/chalkboard.jpg)}");
    //QHBoxLayout hangmanLayout;
    lettersGuessed = new QList<QString>;
    hangmanLabel = new QLabel(this);
    //hangmanLabel->setScaledContents(true);

    for(int i=1; i<=11; i++)
        hangmanPics.append(":/pics/hangman"+QString::number(i)+".png");
    hangman = new QPixmap();
    hangmanLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //hangmanLabel->setMinimumHeight(500);
    //hangmanLabel->setMinimumWidth(500);
    //hangmanLabel->setMaximumSize(QSize(2000,2000));
    guessesLeft=10;
    guessesLeftBar = new QProgressBar(this);
    guessesLeftBar->setRange(0,10);
    guessesLeftBar->setOrientation(Qt::Vertical);
    guessesLeftBar->setValue(guessesLeft);
    hangman->load(hangmanPics[2]);
    hangmanLabel->setPixmap(*hangman);
    layout = new QGridLayout(this);
    wordLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->addLayout(wordLayout,5,0,1,9);
    CreateButtons();
    chosenWord=ChooseWord();
    qDebug()<<chosenWord;
    titleLabel = new QLabel("Hangman");
    titleLabel->setFont(QFont("Herculanum",40));
    //titleLabel->setStyleSheet("color : white;");
    layout->addWidget(titleLabel,0,0,1,9,Qt::AlignCenter);
    hangmanLayout = new QHBoxLayout();
    hangmanLayout->addStretch(1);
    hangmanLayout->addWidget(hangmanLabel,2);
    hangmanLayout->addStretch(1);
    //layout->addWidget(hangmanLabel,1,0,1,10);
    layout->addLayout(hangmanLayout,1,0,1,9);

    layout->addWidget(guessesLeftBar,0,9,6,1);
    //hangmanLayout->addWidget(hangmanLabel);
    //layout->addLayout(hangmanLayout,39,39);
    QFont f( "Chalkduster" , 20);
    wordLayout->addStretch();
    for(int i=0; i<chosenWord.length(); i++)
    {
        letterLabel[i] =  new QLabel();
        letterLabel[i]->setFont(f);
        letterLabel[i]->setStyleSheet("border: 2px solid");
        letterLabel[i]->setAlignment(Qt::AlignCenter);
        //letterLabel[i]->setStyleSheet("color : white;");
        wordLayout->addWidget(letterLabel[i]);
        letterLabel[i]->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
        letterLabel[i]->setMinimumSize(QSize(70,100));
    }
    wordLayout->addStretch();
    layout->setRowStretch(1,1);

    UpdateLabels();
    createGameOver();
    createGameWon();
}

void Dialog::resizeEvent(QResizeEvent* event)
{
   QDialog::resizeEvent(event);
   UpdateLabels();

}
