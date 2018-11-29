


#include "AllegroClass.h"
#include "CursesClass.h"
#include "UserHandler.h"
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

#define MAXNUM (6)


#include <map>

int getClick(float pos, int ammount, int size);
void writeFile(const char * fileName, int matrix[ROWS][COLS]);
void readFile(const char * fileName, int matrix[ROWS][COLS]);


std::map<int,char> Translator;






int main() {

	Translator[0] = 'E';
	Translator[1] = 'F';
	Translator[2] = 'T';
	Translator[3] = 'P';
	Translator[4] = 'G';
	Translator[5] = 'C';

	int matrix[ROWS][COLS];

	for (int i = 0; i < ROWS; i++) {
		for (int a = 0; a < COLS; a++) {
			matrix[i][a] = 0;
		}
	}
	string fileName;
	bool leave = false;
	bool valid = false;

	do {
		CursesClass curses;
		switch (selectMode(curses)) {
		case READ:
			fileName = "stages/" + askForFile(curses);
			if (fileExists(fileName.c_str())) {
				readFile(fileName.c_str(), matrix);
				leave = true;
				valid = true;
			}
			else fileDoesntExist(curses, fileName);
				break;
		case WRITE:
			fileName = "stages/" +askForFile(curses);
			if (fileExists(fileName.c_str()))
				if (askToOverwrite(curses)) {
					valid = true;
					leave = true;
				}
				else {
					leave = true;
					valid = false;
				}
			else
				valid = true;
				leave = true;
			break;
		}
	}while (!leave);



	if (valid) {
		const char * color[] = { "BackgroundSprite.png","GroundSprite.png", "PS1.png","PGS1.png","GFS1.png", "CGS1.png" };

		AllegroClass allegro(16 * SQUARE, 12 * SQUARE + CONFIRMBOX_H, 60.0);
		
		ALLEGRO_FONT * font = allegro.loadFont("mont.ttf", 50);

		ALLEGRO_BITMAP * curr = al_get_target_bitmap();
		ALLEGRO_BITMAP * confirmBox = al_create_bitmap(16 * SQUARE, CONFIRMBOX_H);
		al_set_target_bitmap(confirmBox);
		al_draw_rectangle(0, 0, SQUARE * 16, CONFIRMBOX_H, al_color_name(CONFIRMCOLOR), 10);
		al_draw_text(font, al_color_name("white"), 16 * SQUARE / 2.0, CONFIRMBOX_H / 2.0, ALLEGRO_ALIGN_CENTRE, "Confirm");

		vector <ALLEGRO_BITMAP * > bitmaps;

		for (int i = 0; i < 6; ++i) {
			bitmaps.push_back(al_load_bitmap(color[i]));

			//Si en algun momento le agrego imagenes, tengo que borrar esta parte y cambiar el create bitmap por un load bitmap, y darle un vector de
			// strings. Se podria hacer todos los tama;os variables y que los ponga el usuario pero deberia hacerlo todo dinamico y paja
			//al_set_target_bitmap(bitmaps[i]);
			//al_draw_filled_rectangle(0, 0, SQUARE, SQUARE, al_color_name(color[i]));
			//al_draw_text(font, al_color_name("white"), SQUARE / 2.0, SQUARE / 2.0 - al_get_font_line_height(font) / 2.0, ALLEGRO_ALIGN_CENTRE, to_string(i).c_str());
		}

		al_set_target_bitmap(curr);

		bool quit = false;
		bool save = false;

		while (!quit) {

			ALLEGRO_EVENT ev;
			int col;
			int row;

			if (al_get_next_event(allegro.getEventQueue(), &ev)) {
				switch (ev.type) {
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					quit = true;
				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
					col = getClick(ev.mouse.x, COLS, SQUARE);
					row = getClick(ev.mouse.y, ROWS, SQUARE);

					if (col != ERROR1 && row != ERROR1) {

						ALLEGRO_MOUSE_STATE state;

						al_get_mouse_state(&state);
						if ((state.buttons & 1)) {			// Boton Izquierdo
							if (matrix[row][col] < MAXNUM-1)
								matrix[row][col]++;
							else
								matrix[row][col] = 0;
						}
						else if ((state.buttons & 2)) {			// Boton Derecho
							if (matrix[row][col] > 0)
								matrix[row][col]--;
							else
								matrix[row][col] = MAXNUM-1;

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
					for (int i = 0; i < ROWS; i++)
						for (int a = 0; a < COLS; a++) 
							al_draw_scaled_bitmap(bitmaps[matrix[i][a]], 0, 0, al_get_bitmap_width(bitmaps[matrix[i][a]]), al_get_bitmap_height(bitmaps[matrix[i][a]]), a * SQUARE, i *SQUARE,SQUARE,SQUARE,0);
					al_draw_bitmap(confirmBox, 0, ROWS *SQUARE, 0);
					al_flip_display();
					break;

				}
			}
		}
		if (save)
			writeFile(fileName.c_str(), matrix);

		for (ALLEGRO_BITMAP * bitmap : bitmaps)
			al_destroy_bitmap(bitmap);

		al_destroy_bitmap(confirmBox);
	}


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
			output.put(Translator[matrix[i][a]]);
			output.put(',');
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

		if (a == ',');
		else if (isalpha(a)) {
			matrix[row][col++] = a;
		}
		else if (a == '\n') {
			row++;
			col = 0;
		}


	}

}

