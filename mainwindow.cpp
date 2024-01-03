#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Initial setup
    qsrand(time(NULL));
    gameOver = false;
    head.setX(20);
    head.setY(20);
    up = down = right = left = false;
    eaten = false;
    tail = 0;

    //display grid
    displayGrid();

    //Generate Food Position
    generatefood();

    //timer initialization
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(update()));
    timer->start(300);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateposition()
{
    QPoint Tail(head);
    if (down == true)
    {
        int y = head.y();
        if (head.y() < height())
        {
            y += SCALE;
            head.setY(y);
        }
        else
        {
            head.setY(0);
        }
    }
    else if (up == true)
    {
        int y = head.y();
        if (head.y() > 0)
        {
            y -= SCALE;
            head.setY(y);
        }
        else
        {
            head.setY(height());
        }
    }
    else if (right == true)
    {
        int x = head.x();
        if (head.x() < width())
        {
             x += SCALE;
            head.setX(x);
        }
        else
        {
            head.setX(0);
        }
    }
    else if (left == true)
    {
        int x = head.x();
        if (head.x() > 0)
        {
            x -= SCALE;
            head.setX(x);
        }
        else
        {
            head.setX(width());
        }
    }
    else
    {
        //dummy
    }

    if (eaten == true)
    {
        eaten = false;
        tail += 1;
        pathrec.append(Tail);
        generatefood();
    }

    if (tail > 0)
    {
        pathrec.append(Tail);
        for (int i=0; i < pathrec.size()-1; i++)
        {
            pathrec[i] = pathrec[i+1];
        }
        pathrec.resize(tail);
    }


}

void MainWindow::generatefood()
{
    foodPosition = QRandomGenerator::global()->bounded(grid.length());
}

void MainWindow::checkcollidepoint()
{
    QRect center1(head.x(), head.y(), SCALE, SCALE);
    QRect center2 = grid.at(foodPosition);

    if (center1.intersects(center2)) { eaten = true; }

    for (QPoint point : qAsConst(pathrec))
    {
        QRect rect(point.x(), point.y(), SCALE, SCALE);
        if(center1.intersects(rect)) { gameOver = true; }
    }
}

void MainWindow::displayGrid()
{
    for(uint16_t row=0; row < this->width()/SCALE; row++)
    {
        for(uint16_t col = 0; col < this->height()/SCALE; col++)
        {
            grid.append(QRect(row*SCALE, col*SCALE, SCALE, SCALE));
        }
    }
}



void MainWindow::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);
    QPainter painter(this);


    //get display grid positions
    displayGrid();

    //Update snake position
    updateposition();

    //Check for collision
    checkcollidepoint();

    if(!gameOver)
    {
        //Draw Grid
        for(QRect rect : qAsConst(grid))
        {
            painter.drawRect(rect);
        }

        //draw snake head
        painter.setPen(QPen(Qt::blue));
        painter.setBrush(QBrush(Qt::blue));
        painter.drawRect(head.x(), head.y(), SCALE, SCALE);

        //draw snake body
        painter.setPen(QPen(Qt::green));
        painter.setBrush(QBrush(Qt::green));
        for (QPoint point : qAsConst(pathrec))
        {
            painter.drawRect(point.x(), point.y(), SCALE, SCALE);
        }

        //draw food
        painter.setPen(QPen(Qt::red));
        painter.setBrush(QBrush(Qt::red));
        painter.drawRect(grid.at(foodPosition));

        //Update Score
        QFont font("Helvetica", 18);
        painter.setFont(font);
        painter.drawText(this->width()/2-SCALE, SCALE, "Score: " + QString::number(pathrec.length()));
    }
    else {
        //Update Score
        QFont font("Helvetica", 18);
        painter.setPen(QPen(Qt::red));
        painter.setBrush(QBrush(Qt::red));
        painter.setFont(font);
        painter.drawText(this->width()/2-SCALE, this->height()/2-SCALE, "Score: " + QString::number(pathrec.length()));

        painter.setPen(QPen(Qt::black));
        painter.setBrush(QBrush(Qt::black));
        QString gameOverString("Game Over!");
        painter.drawText(this->width()/2-(2*SCALE), this->height()/2, gameOverString);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *key)
{

    if ((key->key() == Qt::Key_Down) && (up == false))
    {
        if (down == true)
            return;
        down = true;
        up = right = left = false;
    }
    else if ((key->key() == Qt::Key_Up) && (down == false))
    {
        if (up == true)
            return;
        up = true;
        down = right = left = false;
    }
    else if ((key->key() == Qt::Key_Right) && (left == false))
    {
        if (right == true)
            return;
        right = true;
        down = up = left = false;
    }
    else if ((key->key() == Qt::Key_Left) && (right == false))
    {
        if (left == true)
            return;
        left = true;
        down = up = right = false;
    }
    else { /*dummy*/ }
}
