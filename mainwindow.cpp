#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), labelStatus("Eligiendo tablero", this), labelPlayer(this), tab()
{
    ui->setupUi(this);

    QFont boldFont("Arial", 14, QFont::Bold);

    QPixmap pixmap(":/icons/BigCircle.png");

    labelStatus.setAlignment(Qt::AlignRight | Qt::AlignCenter);
    labelStatus.setFont(boldFont);
    labelStatus.setMaximumWidth(155);

    labelPlayer.setMaximumSize(24, 24);
    labelPlayer.setPixmap(pixmap.scaled(labelPlayer.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    labelPlayer.setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    QHBoxLayout* tableroLayout = new QHBoxLayout();
    QHBoxLayout* statusLayout = new QHBoxLayout();

    statusLayout->addStretch();
    statusLayout->addWidget(&labelStatus, Qt::AlignRight | Qt::AlignBottom);
    statusLayout->addSpacing(5);
    statusLayout->addWidget(&labelPlayer, Qt::AlignLeft | Qt::AlignBottom);
    statusLayout->addStretch();

    tableroLayout->addStretch();
    tableroLayout->addLayout(&tab);
    tableroLayout->addStretch();

    verticalLayout->addStretch();
    verticalLayout->addLayout(tableroLayout);
    verticalLayout->addStretch();
    verticalLayout->addLayout(statusLayout);

    tab.setChoosable();

    centralWidget()->setLayout(verticalLayout);
    show();

    QObject::connect(ui->actionCerrar, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(ui->actionResetear, SIGNAL(triggered()), &tab, SLOT(slotReset()));
    QObject::connect(ui->actionNormas, SIGNAL(triggered()), this, SLOT(slotNormas()));
    QObject::connect(ui->actionSobre, SIGNAL(triggered()), this, SLOT(slotSobre()));

    QObject::connect(&tab, SIGNAL(changeTurn(ushort)), this, SLOT(slotPlayerChanged(ushort)));
    QObject::connect(&tab, SIGNAL(choosingTablero()), this, SLOT(slotChoosingTablero()));
    QObject::connect(&tab, SIGNAL(chosenTablero()), this, SLOT(slotChosenTablero()));

    QObject::connect(&tab, SIGNAL(gameOver(ushort)), this, SLOT(slotGameOver(ushort)));
    QObject::connect(this, SIGNAL(Reset()), &tab, SLOT(slotReset()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotPlayerChanged(unsigned short player) {
    const char* imageRoute;

    switch (player) {
    case PLAYER1:
        imageRoute = ":/icons/BigCircle.png";
        break;
    case PLAYER2:
        imageRoute = ":/icons/BigCross.png";
        break;
    default:
        imageRoute = ":/icons/Empty.png";
    }

    QPixmap pixmap(imageRoute);

    labelPlayer.setPixmap(pixmap.scaled(labelPlayer.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::slotChoosingTablero() {
    labelStatus.setText("Eligiendo tablero");
    labelStatus.setMaximumWidth(155);
}

void MainWindow::slotChosenTablero() {
    labelStatus.setText("Jugando");
    labelStatus.setMaximumWidth(80);
}

void MainWindow::slotGameOver(unsigned short winner) {
    QMessageBox msg(this);

    slotPlayerChanged(winner);

    if (winner == PLAYER1) {
        msg.setText("Ganó el jugador de los círculos");
        msg.setIconPixmap(QPixmap(":/icons/Circle.png"));
        labelStatus.setMaximumWidth(50);
        labelStatus.setText("Ganó");
    } else if (winner == PLAYER2) {
        msg.setText("Ganó el jugador de las cruces");
        msg.setIconPixmap(QPixmap(":/icons/Cross.png"));
        labelStatus.setMaximumWidth(50);
        labelStatus.setText("Ganó");
    } else {
        msg.setText("El juego acabó en empate");
        labelStatus.setMaximumWidth(80);
        labelStatus.setText("Empate");
    }



    msg.setInformativeText("¿Quieres empezar otra partida?");
    msg.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Retry);

    int res = msg.exec();

    if (res == QMessageBox::Retry) emit Reset();
}

void MainWindow::slotNormas() {
    QMessageBox msg(this);

    msg.setText("Este juego consta de 9 tableros de tres en raya clásicos.\n"
                "\n"
                "El primer jugador elige el tablero en el que quiere empezar a jugar. Esta tablero será marcado como activo (color verde oscuro). "
                "Este mismo jugador colocará su marca en uno de los cuadros del tablero elegido.\n"
                "\n"
                "A continuación el tablero activo cambiará al tablero que corresponda con la posición que el anterior jugador jugó. Si, por ejemplo, "
                "el primer jugador eligió el tablero central y jugó en la esquina superior izquierda de este. El tablero activo cambiará al tablero de"
                "la esquina superior izquierda.\n"
                "\n"
                "Una vez que una de las partidas en uno de los tableros individuales acabe este tablero se marcará con el color del ganador "
                "o con gris oscuro si el juego hubiera acabado en empate. Este tablero ya estará marcado como terminado. En caso de que, siguiendo las "
                "normas expuestas, a un jugador le tocara jugar en un tablero terminado se le dará a este jugador la libertad de elegir en qué tablero desea jugar.\n"
                "\n"
                "El objetivo del juego es hacer un tres en raya en la cuadrícula formada por los tableros individuales de tres en raya.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);

    msg.exec();
}

void MainWindow::slotSobre() {
    QMessageBox msg(this);

    msg.setText("Este juego fue creado por David Ortiz del Campo usando Qt 6.6.2.\n"
                "\n"
                "Está publicado bajo una licencia GPL.");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);

    msg.exec();
}
