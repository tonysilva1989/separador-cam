#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
   void handleButton();
   void handleOutput();
   void handleButtonGenerate();
   void mostrarPath(QString);
   void mostrarPathOut(QString);
private:
   QPushButton *m_button;
   QTextEdit   *lineEdit = new QTextEdit();
private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
