#include "TresEnRaya.hpp"

bool TresEnRaya::ha_ganado() const {
	unsigned i, j;

	// Filas
	for (i = 0; i < 3; i++) {
		j = 0;
		while (j < 3 && tablero[i][j] == playing) j++;
		if (j == 3) return true;
	}
	// Columnas
	for (j = 0; j < 3; j++) {
		i = 0;
		while (i < 3 && tablero[i][j] == playing) i++;
		if (i == 3) return true;
	}
	// Diagonal 1
	i = 0;
	while (i < 3 && tablero[i][i] == playing) i++;
	if (i == 3) return true;

	// Diagonal 2
	i = 0;
	while (i < 3 && tablero[i][2-i] == playing) i++;
	if (i == 3) return true;

	return false;
}

TresEnRaya::TresEnRaya() {
	unsigned i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			tablero[i][j] = VACIO;
	playing = PLAYER1;
	ended = false;
	casillas_libres = 9;
}

std::ostream& operator<<(std::ostream& os, const TresEnRaya& p) {
	unsigned i, j, n = 1;
	std::string sep(5, '-');

	for (i = 0; i < 3; i++) {
		if (i > 0) os << sep << '\n';
		for (j = 0; j < 3; j++) {
			if (p.tablero[i][j] == VACIO) os << n;
			else if (p.tablero[i][j] == PLAYER1) os << 'X';
			else os << 'O';

			n++;

			if (j < 2) os << '|';
		}
		os << '\n';
	}

	return os;
}

bool TresEnRaya::play(unsigned pos) {
	unsigned i, j;

	if (ended) return false;
	if (pos < 1 || pos > 9) return false;

	// Get indeces
	pos--;
	i = pos / 3;
    j = pos % 3;

	if (tablero[i][j] != VACIO) return false;

	tablero[i][j] = playing;

	casillas_libres--;

	if (ha_ganado()) ended = true;
	else if (casillas_libres == 0) {
		ended = true;
		playing = NONE;
	} else playing = (playing == PLAYER1 ? PLAYER2 : PLAYER1);

	return true;
}

bool TresEnRaya::player_plays(unsigned pos, unsigned short player) {
	playing = player;
	return play(pos);
}

bool TresEnRaya::add_none(unsigned pos) {
	unsigned i, j;

	if (ended) return false;
	if (pos < 1 || pos > 9) return false;

	// Get indeces
	pos--;
	i = pos / 3;
	j = pos - i*3;

	if (tablero[i][j] != VACIO) return false;

	tablero[i][j] = NONE;

	casillas_libres--;

	if (casillas_libres == 0) {
		ended = true;
		playing = NONE;
	} else playing = (playing == PLAYER1 ? PLAYER2 : PLAYER1);

	return true;
}

std::vector<std::string> TresEnRaya::get_tablero_vs() const {
	unsigned i, j, n = 1;
	std::vector<std::string> r(3, "");

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (tablero[i][j] == VACIO) r[i] += std::to_string(n);
			else if (tablero[i][j] == PLAYER1) r[i] += "X";
			else r[i] += "O";

			n++;

			if (j < 2) r[i] += "|";
		}
	}

	return r;
}

unsigned TresEnRaya::primero_vacio() const {
	unsigned i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (tablero[i][j] == VACIO)
				return 3*i + j + 1;

	return 10;
}
