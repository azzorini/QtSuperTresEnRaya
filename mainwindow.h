#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>

#include "tresenrayagui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel labelStatus;
    QLabel labelPlayer;
    QSuperTablero tab;

public slots:
    void slotPlayerChanged(unsigned short player);
    void slotChoosingTablero();
    void slotChosenTablero();
    void slotGameOver(unsigned short winner);

    void slotNormas();
    void slotSobre();

signals:
    void Reset();
};
#endif // MAINWINDOW_H
