#include "AllegroClass.h"
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>

#define CONFIRMBOX_H (500)
#define SQUARE (100)

#define ERROR1 (-1)

#define ONCOLOR "blue"
#define OFFCOLOR "gray"
#define CONFIRMCOLOR "hotpink"

#define ROWS (12)
#define COLS (16)

#define MAXNUM (9)

int getClick(float pos, int ammount, int size);
void writeFile(const char * fileName, int matrix[ROWS][COLS]);
void readFile(const char * fileName, int matrix[ROWS][COLS]);

int main() {
	AllegroClass allegro(16 * SQUARE, 12 * SQUARE + CONFIRMBOX_H, 60.0);
	ALLEGRO_FONT * font = allegro.loadFont("mont.ttf", 50);

	int matrix[ROWS][COLS];

	for (int i = 0; i < ROWS; i++) {
		for (int a = 0; a < COLS; a++) {
			matrix[i][a] = 0;
		}
	}

	
	
	ALLEGRO_BITMAP * curr = al_get_target_bitmap();
	ALLEGRO_BITMAP * confirmBox = al_create_bitmap(16 * SQUARE, CONFIRMBOX_H);
	al_set_target_bitmap(confirmBox);
	al_draw_rectangle(0, 0, SQUARE * 16,CONFIRMBOX_H, al_color_name(CONFIRMCOLOR), 10);
	al_draw_text(font, al_color_name("white"), 16 *SQUARE /2.0, CONFIRMBOX_H /2.0, ALLEGRO_ALIGN_CENTRE, "Confirm");

	vector <ALLEGRO_BITMAP * > bitmaps;

	for (int i = 0; i < 10; ++i) {
		bitmaps.push_back(al_create_bitmap(SQUARE, SQUARE));
		al_set_target_bitmap(bitmaps[i]);
		al_draw_rectangle(0, 0, SQUARE, SQUARE, al_color_name("white"), 5);
		al_draw_text(font, al_color_name("white"), SQUARE / 2.0, SQUARE / 2.0 - al_get_font_line_height(font)/2.0, ALLEGRO_ALIGN_CENTRE, to_string(i).c_str());
	}

	al_set_target_bitmap(curr);

	readFile("file.cvs", matrix);

	bool quit = false;
	bool save = false;

	while (!quit) {

		ALLEGRO_EVENT ev;
		int col;
		int row;

		if (al_get_next_event(allegro.getEventQueue(), &ev) ){
			switch (ev.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				quit = true;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				col = getClick(ev.mouse.x, COLS, SQUARE);
				row = getClick(ev.mouse.y, ROWS, SQUARE);

				if (col != ERROR1 && row != ERROR1) {

					ALLEGRO_MOUSE_STATE state;

					al_get_mouse_state(&state);
					if ((state.buttons & 1 ) ) {			// Boton Izquierdo
						if (matrix[row][col] < MAXNUM)
							matrix[row][col]++;
						else
							matrix[row][col] = 0;
					}
					else if ((state.buttons & 2)) {			// Boton Derecho
						if (matrix[row][col] > 0)
							matrix[row][col]--;
						else
							matrix[row][col] = MAXNUM;

					}
				}
				else
				{
					quit = true;
					save = true;
				}

				
				break;
			case ALLEGRO_EVENT_TIMER:
				al_clear_to_color(al_color_name("black"));
				for (int i = 0; i < ROWS ; i++)
					for (int a = 0; a < COLS; a++) {	 
						al_draw_bitmap(bitmaps[matrix[i][a]], a * SQUARE, i * SQUARE, 0);
					}
				al_draw_bitmap(confirmBox,0,ROWS *SQUARE,0);
				al_flip_display();
				break;

			}
		}
	}
	if (save)
		writeFile("file.cvs",matrix);

	for (ALLEGRO_BITMAP * bitmap : bitmaps)
		al_destroy_bitmap(bitmap);

	al_destroy_bitmap(confirmBox);


	return 0;
}


int getClick(float pos, int ammount, int size) {
	int retValue = ERROR1;

	for (int i = 0; retValue == ERROR1 && i < ammount; i++) {
		if (i * size < pos && (i + 1) * size >pos)
			retValue = i;
	}
	return retValue;
}

void writeFile(const char * fileName, int matrix[ROWS][COLS]) {

	ofstream output;

	output.open(fileName, fstream::out);

	for (int i = 0; i < ROWS; i++) {
		for (int a = 0; a < COLS; a++) {
			output.put(matrix[i][a] + '0');
			output.put(';');
		}
		output.put('\n');
	}
	output.close();
}

void readFile(const char * fileName, int matrix[ROWS][COLS]) {
	ifstream input;
	input.open(fileName, ifstream::in);
	bool leave = false;
	int row = 0;
	int col = 0;
	
	while (!leave && !input.eof()) {
		int a = input.get();

		if (a == ';');
		else if (isdigit(a)) {
			matrix[row][col++] = a - '0';
		}
		else if (a == '\n') {
			row++;
			col = 0;
		}


	}

}

