#include <ncurses.h>
#include <list>
#include <vector>
#include <tuple>

enum { UP, DOWN, LEFT, RIGHT };

struct Cell {
  int row = 0;
  int column = 0;

  Cell(int row, int column): row(row), column(column) {};
};

void changeDir(char &dir, int input) {
  switch (input) {
    case KEY_UP:
      if (dir != DOWN) {
        dir = UP;
      }
      break;
    case KEY_DOWN:
      if (dir != UP) {
        dir = DOWN;
      }
      break;
    case KEY_LEFT:
      if (dir != RIGHT) {
        dir = LEFT;
      }
      break;
    case KEY_RIGHT:
      if (dir != LEFT) {
        dir = RIGHT;
      }
      break;
  }
}

void moveSnake(std::list<Cell> &snake, int rows, int cols, char dir, Cell *&apple, bool &isOver) {
  Cell& head = snake.front();
  Cell& tail = snake.back();
  
  int head_row = head.row;
  int head_col = head.column;

  switch(dir) {
    case UP:
      head_row--;

      if (head_row < 0) {
        head_row = rows - 1;
      }
      break;
    case DOWN:
      head_row++;

      if (head_row == rows) {
        head_row = 0;
      }
      break;
    case LEFT:
      head_col--;

      if (head_col < 0) {
        head_col = cols - 1;
      }
      break;
    case RIGHT:
      head_col++;

      if (head_col == cols) {
        head_col = 0;
      }
      break;
  }

  
  if (mvinch(head_row, head_col) == '@') {
    isOver = true;
    return;
  }
  
  snake.push_front(Cell{head_row, head_col});

  if (apple != nullptr && head_row == apple->row && head_col == apple->column) {
    mvdelch(apple->row, apple->column);
    apple = nullptr;
    return;
  }
  
  snake.pop_back();
}

void drawSnake(std::list<Cell> &snake) {
  for (std::list<Cell>::iterator it = snake.begin(); it != snake.end(); it++) {
    mvaddch(it->row, it->column, '@');
  }
}

std::tuple<int, int> getAppleCell(int rows, int cols) {
  std::vector<Cell> freeCells = {};

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (mvinch(i, j) != '@') {
        freeCells.push_back(Cell{i, j});
      }
    }
  }

  int index = rand() % freeCells.size();
  Cell &cell = freeCells.at(index);
  
  return std::tuple<int, int>{cell.row, cell.column};
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
  bool isOver = false;
  Cell *apple = nullptr;

  getmaxyx(stdscr, rows, columns);

  // Spawn snake
  std::list<Cell> snake = {
    Cell{rows/2, columns/2},
    Cell{rows/2, columns/2 + 1},
    Cell{rows/2, columns/2 + 2}
  };

  drawSnake(snake);

  // Game cycle
  while(!isOver) {
    // Spawn apple
    if (apple == nullptr) {
      int r, c;
      std::tie(r, c) = getAppleCell(rows, columns);
      
      apple = new Cell{r, c};
    }

    if (apple != nullptr) {
      mvaddch(apple->row, apple->column, '0');
    }

    // Snake moving
    int input = getch();
    changeDir(dir, input);

    
    moveSnake(snake, rows, columns, dir, apple, isOver);
    clear();
    drawSnake(snake);


    refresh();
  }

  printw("OVER!!!");
  refresh();
  getch();

  endwin();
}