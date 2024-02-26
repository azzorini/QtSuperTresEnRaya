#include "tresenrayagui.h"

/** QButtonTresEnRaya **/

QButtonTresEnRaya::QButtonTresEnRaya(unsigned position, QTableroTresEnRaya* tablero, QWidget* parent)
    : QPushButton(parent), pos(position), is_empty(true), tab(tablero) {

    QPixmap pixmap(":/icons/Empty.png");
    QIcon ButtonIcon = QIcon(pixmap);
    setIcon(ButtonIcon);
    setIconSize(pixmap.rect().size());
    adjustSize();
    setEnabled(false);

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
}

bool QButtonTresEnRaya::event(QEvent* e) {
    e->accept();

    if (e->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);

        if (mouseEvent->button() == Qt::LeftButton)
            if (tab->get_super()->checkForSelectedFrames()) {
                return true;
            }

    }

    return QPushButton::event(e);
}

void QButtonTresEnRaya::slotButtonClicked() {
    tab->makeChange(this);
}

void QButtonTresEnRaya::reset() {
    QPixmap pixmap(":/icons/Empty.png");
    QIcon ButtonIcon = QIcon(pixmap);
    setIcon(ButtonIcon);
    setIconSize(pixmap.rect().size());
    adjustSize();
    setEnabled(false);
    is_empty = true;
}

void QButtonTresEnRaya::changeIcon(unsigned short player) {
    const char* route;
    if (player == PLAYER1) {
        route = ":/icons/Circle.png";
    } else if (player == PLAYER2) {
        route = ":/icons/Cross.png";
    } else {
        return;
    }
    QPixmap pixmap(route);
    QIcon ButtonIcon = QIcon(pixmap);
    ButtonIcon.addPixmap(pixmap, QIcon::Disabled);
    setIcon(ButtonIcon);
    setIconSize(pixmap.rect().size());
    adjustSize();
}

void QButtonTresEnRaya::enable() {
    if (is_empty)
        setEnabled(true);
}

/** QTableroTresEnRaya **/

QTableroTresEnRaya::QTableroTresEnRaya(unsigned position, QSuperTablero* super_tab, QWidget* parent)
    : QGridLayout(parent), buttons(9, nullptr), game(), pos(position), super(super_tab) {
    unsigned i, j, pos_but;
    setContentsMargins(2.5, 2.5, 2.5, 2.5);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            pos_but = j + 3*i;
            // We set the parent of the buttons here to be the parent of the Tablero
            // but then when executing addWidget this will be changed
            buttons[pos_but] = new QButtonTresEnRaya(pos_but+1, this, parent);
            addWidget(buttons[pos_but], j, i, 1, 1, Qt::AlignCenter);
        }
    }
}

void QTableroTresEnRaya::disable() {
    for (unsigned i = 0; i < 9; i++)
        buttons[i]->setEnabled(false);
}

void QTableroTresEnRaya::enable() {
    for (unsigned i = 0; i < 9; i++)
        buttons[i]->enable();
}

void QTableroTresEnRaya::reset() {
    for (unsigned i = 0; i < 9; i++) {
        buttons[i]->reset();
    }
    game = TresEnRaya();
}

void QTableroTresEnRaya::makeChange(QButtonTresEnRaya* but) {
    unsigned short player = super->get_player();
    unsigned but_pos = but->get_pos();
    bool game_keeps_going = true;

    if (game.player_plays(but_pos, player)) {
        but->changeIcon(player);
        but->setEnabled(false);
        but->set_empty(false);
        if (game.game_ended()) {
            game_keeps_going = super->tableroFinished(pos, game.get_winner());
        } else super->swap_player();

        disable();

        if (game_keeps_going) super->PlayedInTablero(but_pos, pos);
    }
}

/** QFrameTresEnRaya **/

QFrameTresEnRaya::QFrameTresEnRaya(unsigned position, QWidget *parent, const QColor &color1, const QColor &color2, const QColor &color3, const QColor& color4)
: QFrame(parent), pos(position), colorNoMouse(color1), colorMouse(color2), colorActive(color4), changeColorMouse(false), is_active(false),
is_mouse_on_top(false) {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, color1);
    pal.setColor(QPalette::WindowText, color3);
    setAutoFillBackground(true);

    setContentsMargins(0, 0, 0, 0);
    setPalette(pal);
    setFrameStyle(QFrame::Box);
    setLineWidth(0);
}

void QFrameTresEnRaya::setColor(const QColor &color) {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, color);
    setPalette(pal);
}

void QFrameTresEnRaya::setChoosable(bool choosable) {
    if (choosable) {
        changeColorMouse = true;
        is_active = false;

        if (underMouse()) {
            setColor(colorMouse);
            is_mouse_on_top = true;
        } else setColor(colorNoMouse);
    } else {
        changeColorMouse = false;
        is_mouse_on_top = false;
        // SEE: Maybe I need to add this line to fix a possible bug with the colors
        //if (!is_active) setColor(colorNoMouse);
    }
}

void QFrameTresEnRaya::activate() {
    is_active = true;
    is_mouse_on_top = false;
    changeColorMouse = false;
    setColor(colorActive);
    emit frameActivated(pos);
}

void QFrameTresEnRaya::deactivate() {
    is_active = false;
    is_mouse_on_top = false;
    changeColorMouse = false;
    setColor(colorNoMouse);
}

void QFrameTresEnRaya::enterEvent(QEnterEvent *event) {
    event->accept();
    if (changeColorMouse) {
        setColor(colorMouse);
        is_mouse_on_top = true;
    }
    QWidget::enterEvent(event);
}

void QFrameTresEnRaya::leaveEvent(QEvent *event) {
    event->accept();
    if (changeColorMouse) {
        setColor(colorNoMouse);
        is_mouse_on_top = false;
    }
    QWidget::leaveEvent(event);
}

void QFrameTresEnRaya::mousePressEvent(QMouseEvent* event) {
    event->accept();

    if (event->button() == Qt::LeftButton && is_mouse_on_top)
        activate();

    QWidget::mousePressEvent(event);
}

void QFrameTresEnRaya::slotGameOver(unsigned short winner) {
    changeColorMouse = false;
    is_mouse_on_top = false;
    is_active = false;
    if (winner == PLAYER1) setColor(Qt::blue);
    else if (winner == PLAYER2) setColor(Qt::red);
    else setColor(colorNoMouse);
}

/** QSuperTablero **/

QSuperTablero::QSuperTablero(QWidget* parent)
: QGridLayout(parent), tableros(9, nullptr), frames(9, nullptr), game() {
    unsigned i, j, pos;

    setContentsMargins(7.5, 7.5, 7.5, 7.5);

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            pos = j + 3*i;
            frames[pos] = new QFrameTresEnRaya(pos+1, parent, Qt::gray, Qt::darkRed, Qt::gray, Qt::darkGreen);
            tableros[pos] = new QTableroTresEnRaya(pos+1, this, frames[pos]);
            addWidget(frames[pos], j, i, 1, 1, Qt::AlignCenter);

            QObject::connect(frames[pos], SIGNAL(frameActivated(uint)), this, SLOT(slotFrameActivated(uint)));
        }
    }
}

void QSuperTablero::setChoosable(bool choosable) {
    for (unsigned pos = 0; pos < 9; pos++) {
        if (game.get_tablero_square(tableros[pos]->get_pos()) == VACIO) {
            frames[pos]->setChoosable(choosable);
        }
    }
}

bool QSuperTablero::checkForSelectedFrames() {
    for (unsigned i = 0; i < 9; i++) {
        if (frames[i]->get_choosable() && frames[i]->get_mouse_on_top()) {
            frames[i]->activate();
            return true;
        }
    }

    return false;
}

void QSuperTablero::PlayedInTablero(unsigned pos_square, unsigned pos_tab) {
    QTableroTresEnRaya* tab_square = nullptr;
    QFrameTresEnRaya* frame_played = nullptr;
    QFrameTresEnRaya* frame_square = nullptr;
    unsigned pos_ultima_casilla;
    unsigned i = 0;

    while ((tab_square == nullptr || frame_square == nullptr || frame_played == nullptr) && i < 9) {
        if (tableros[i]->get_pos() == pos_square) tab_square = tableros[i];
        if (frames[i]->get_pos() == pos_square) frame_square = frames[i];
        if (frames[i]->get_pos() == pos_tab) frame_played = frames[i];
        i++;
    }

    if (game.get_tablero_square(pos_tab) == VACIO) frame_played->deactivate();

    emit changeTurn(game.get_player());

    if (game.get_tablero_square(pos_square) == VACIO) {
        tab_square->enable();
        frame_square->activate();
    } else if (game.ultima_casilla()) {
        pos_ultima_casilla = game.primero_vacio();
        for (i = 0; i < 9; i++) {
            if (tableros[i]->get_pos() == pos_ultima_casilla) tableros[i]->enable();
            if (frames[i]->get_pos() == pos_ultima_casilla) frames[i]->activate();
        }
    } else {
        setChoosable();
        emit choosingTablero();
    }

}

bool QSuperTablero::tableroFinished(unsigned pos, unsigned short winner) {
    unsigned i;

    if (winner != NONE) game.play(pos);
    else game.add_none(pos);

    for (i = 0; i < 9; i++)
        if (frames[i]->get_pos() == pos) {
            frames[i]->deactivate();
            switch (game.get_tablero_square(pos)) {
            case PLAYER1:
                frames[i]->setColor(Qt::blue);
                break;
            case PLAYER2:
                frames[i]->setColor(Qt::red);
                break;
            default:
                frames[i]->setColor(Qt::darkGray);
            }
            break;
        }


    if (game.game_ended()) {
        emit gameOver(game.get_winner());
        return false;
    }

    return true;
}

void QSuperTablero::slotFrameActivated(unsigned pos) {
    for (unsigned i = 0; i < 9; i++) {
        frames[i]->setChoosable(false);
        if (tableros[i]->get_pos() == pos)
            tableros[i]->enable();
    }

    emit chosenTablero();
}

void QSuperTablero::slotReset() {
    unsigned i;

    game = TresEnRaya();

    for (i = 0; i < 9; i++) {
        frames[i]->deactivate();
        frames[i]->setChoosable();
        tableros[i]->reset();
    }

    emit choosingTablero();
    emit changeTurn(PLAYER1);
}
