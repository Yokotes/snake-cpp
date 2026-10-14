#include <ncurses.h>
#include <list>

enum { UP, DOWN, LEFT, RIGHT };

void changeDir(char &dir, int input) {
  switch (input) {
    case KEY_UP:
      dir = UP;
      break;
    case KEY_DOWN:
      dir = DOWN;
      break;
    case KEY_LEFT:
      dir = LEFT;
      break;
    case KEY_RIGHT:
      dir = RIGHT;
      break;
  }
}

void moveSnake(int &row, int &col, int rows, int cols, char dir) {
  switch(dir) {
    case UP:
      row--;

      if (row < 0) {
        row = rows - 1;
      }
      break;
    case DOWN:
      row++;

      if (row == rows) {
        row = 0;
      }
      break;
    case LEFT:
      col--;

      if (col < 0) {
        col = cols - 1;
      }
      break;
    case RIGHT:
      col++;

      if (col == cols) {
        col = 0;
      }
      break;
  }
    
    mvaddch(row, col, '@');
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  curs_set(0);

  timeout(250);

  int rows, columns;
  char dir = LEFT;

  getmaxyx(stdscr, rows, columns);

  int cur_row = rows/2;
  int cur_column = columns/2;

  // Spawn snake
  mvaddch(cur_row, cur_column, '@');

  // Game cycle
  while(true) {
    int input = getch();
    changeDir(dir, input);

    clear();

    moveSnake(cur_row, cur_column, rows, columns, dir);
    refresh();
  }

  endwin();
}