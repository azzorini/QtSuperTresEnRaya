#pragma once

#include <ostream>
#include <string>
#include <vector>

enum Casilla: unsigned short {VACIO, PLAYER1, PLAYER2, NONE};

/*
   Numeración del tablero de tres en raya:
			1|2|3
			-----
			4|5|6
			-----
			7|8|9
*/

class TresEnRaya {
	unsigned short tablero[3][3];
	unsigned short playing;
	bool ended;
	unsigned casillas_libres;

	// Utilidades

	// Comprueba si el jugador que hizo la última jugada ganó
	bool ha_ganado() const;
public:
	// Constructor
	TresEnRaya();

    unsigned short get_tablero_square(unsigned i, unsigned j) const { return tablero[i][j];  }
    unsigned short get_tablero_square(unsigned pos) const {
        pos--;
        return tablero[pos/3][pos%3];
    }

	// Mira si el juego ya acabó
	bool game_ended() const { return ended; }

	// Devuelve el jugador actual
	unsigned short get_player() const { return playing; }

	// Devuelve el ganador del juego (si sigue en juego devuelve VACIO)
	unsigned short get_winner() const {
		if (ended) return playing;
		else return VACIO;
	}

	// Operadores
	friend std::ostream& operator<<(std::ostream&, const TresEnRaya&);

	// El jugador actual pone una marca en la casilla indicada
	bool play(unsigned);

	// El jugador indicado pone una marca en la casilla indicada
	bool player_plays(unsigned, unsigned short);

    // Cambia el jugador actual
    void swap_player() { playing = (playing == PLAYER2 ? PLAYER1 : PLAYER2); }

	// Add NONE to the indicated position if possible
	bool add_none(unsigned);

	// Devuelve el tablero como un vector de strings
	std::vector<std::string> get_tablero_vs() const;

	// Indica si ya solo falta una casilla por jugar
	bool ultima_casilla() const { return casillas_libres == 1; }

	// Devuelve la posición de la primera casilla que está vacía
	unsigned primero_vacio() const;
};
