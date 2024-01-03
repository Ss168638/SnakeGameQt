#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRandomGenerator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int SCALE = 20;
    QPoint head;
    QVector<QPoint> pathrec;
    bool up, down, right, left;
    bool eaten;
    int tail;
    uint8_t foodPosition;
    bool gameOver;
    QVector<QRect> grid;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateposition();
    void generatefood();
    void checkcollidepoint();
    void displayGrid();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *key);
};

#endif // MAINWINDOW_H
