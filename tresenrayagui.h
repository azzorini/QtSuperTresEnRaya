#ifndef QBUTTONTRESENRAYA_H
#define QBUTTONTRESENRAYA_H

// Qt libraries
#include <QPushButton>
#include <QGridLayout>
#include <QFrame>
#include <QMouseEvent>

// STD libraries
#include <vector>

// My own libraries
#include "TresEnRaya.hpp"

// Classes defined here
class QButtonTresEnRaya;
class QTableroTresEnRaya;
class QFrameTresEnRaya;
class QSuperTablero;

/** QButtonTresEnRaya **/

class QButtonTresEnRaya : public QPushButton {
    Q_OBJECT
private:
    unsigned pos;
    bool is_empty;
    QTableroTresEnRaya* tab;
public:
    QButtonTresEnRaya(unsigned position, QTableroTresEnRaya* tablero, QWidget* parent = nullptr);

    void set_pos(unsigned position) { pos = position; }
    unsigned get_pos() const { return pos; }

    void set_empty(bool empty) { is_empty = empty; }
    bool get_empty() const { return is_empty; }

    void reset();

    void changeIcon(unsigned short player);

    void enable();

protected:
    virtual bool event(QEvent* e);

public slots:
    void slotButtonClicked();
};

/** QTableroTresEnRaya **/

class QTableroTresEnRaya : public QGridLayout {
    Q_OBJECT
private:
    std::vector<QButtonTresEnRaya*> buttons;
    TresEnRaya game;
    unsigned pos;
    QSuperTablero* super;
public:
    QTableroTresEnRaya(unsigned position, QSuperTablero* super_tab, QWidget* parent = nullptr);

    void set_pos(unsigned position) { pos = position; }
    unsigned get_pos() const { return pos; }

    QSuperTablero* get_super() const { return super; }

    unsigned short get_winner() const { return game.get_winner(); }

    QButtonTresEnRaya* get_button(unsigned n) const {
        if (n < 9) return buttons[n];
        return nullptr;
    }
    void disable();
    void enable();

    void reset();

    void makeChange(QButtonTresEnRaya* but);
};

/** QFrameTresEnRaya **/

class QFrameTresEnRaya : public QFrame {
    Q_OBJECT
private:
    unsigned pos;
    QColor colorNoMouse;
    QColor colorMouse;
    QColor colorActive;
    bool changeColorMouse;
    bool is_active;
    bool is_mouse_on_top;
public:
    QFrameTresEnRaya(unsigned position, QWidget* parent = nullptr, const QColor& color1 = Qt::gray, const QColor& color2 = Qt::darkRed,
                     const QColor& color3 = Qt::gray, const QColor& color4 = Qt::darkGreen);
    void setChangeColorMouse(bool value) { changeColorMouse = value; }
    void setColorNoMouse(const QColor& color) { colorNoMouse = color; }
    void setColorMouse(const QColor& color) { colorMouse = color; }
    void setColor(const QColor& color);

    void setChoosable(bool choosable = true);

    void set_pos(unsigned position) { pos = position; }
    unsigned get_pos() const { return pos; }

    bool get_choosable() const { return changeColorMouse; }
    bool get_mouse_on_top() const { return is_mouse_on_top; }

    void activate();

    void deactivate();

protected:
    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
public slots:
    void slotGameOver(unsigned short winner);
signals:
    void frameActivated(unsigned pos);
};

/** QSuperTablero **/

class QSuperTablero : public QGridLayout {
    Q_OBJECT
private:
    std::vector<QTableroTresEnRaya*> tableros;
    std::vector<QFrameTresEnRaya*> frames;
    TresEnRaya game;
public:
    QSuperTablero(QWidget* parent = nullptr);

    unsigned short get_player() const { return game.get_player(); }

    void swap_player() { game.swap_player(); }

    void setChoosable(bool choosable = true);

    bool checkForSelectedFrames();

    void PlayedInTablero(unsigned pos_square, unsigned pos_tab);

    bool tableroFinished(unsigned pos, unsigned short winner);

public slots:
    void slotFrameActivated(unsigned pos);
    void slotReset();
signals:
    void changeTurn(unsigned short player);
    void choosingTablero();
    void chosenTablero();
    void gameOver(unsigned short winner);
};

#endif // QBUTTONTRESENRAYA_H
