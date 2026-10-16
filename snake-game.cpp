#include <ncurses.h>
#include <list>
#include <vector>
#include <tuple>
#include <iterator>


int ROWS, COLUMNS;

class Snake {
  enum DIRECTION {UP, DOWN, LEFT, RIGHT};  
  struct Segment {
    int row;
    int col;
    Segment(int row, int col): row(row), col(col) {}
  };

  std::list<Segment> segments;
  DIRECTION dir = LEFT;
  bool isGrowing = false;
  
  void changeDir(int input);
  
  public:
  Snake(int row, int col);
  void move(int input);
  void draw();
  bool isOverlapping();
  bool isOccupies(int row, int col);
  const Segment& getHead();
  void grow();
};

Snake::Snake(int row, int col) {
  segments = {
    Segment{row, col},
    Segment{row, col+1},
    Segment{row, col+2}
  };
}
void Snake::move(int input) {
  Segment& head = segments.front();
  int r = head.row;
  int c = head.col;

  changeDir(input);

  // Changing dir and setting new head position
  switch (dir) {
    case UP:
      r--;

      if (r < 0) {
        r = ROWS - 1;
      }
      break;
    case DOWN:
      r++;

      if (r == ROWS) {
        r = 0;
      }
      break;
    case LEFT:
      c--;

      if (c < 0) {
        c = COLUMNS - 1;
      }
      break;
    case RIGHT:
      c++;

      if (c == COLUMNS) {
        c = 0;
      }
      break;
  }
    
  segments.push_front(Segment{r, c});
  if (isGrowing) {
    isGrowing = false;
    return;
  }
  segments.pop_back();
}
void Snake::draw() {
  for (std::list<Segment>::iterator it = segments.begin(); it != segments.end(); it++) {
    mvaddch(it->row, it->col, '@');
  }
}
void Snake::changeDir(int input) {
  switch (input) {
      case KEY_UP:
        if (dir != DOWN) dir = UP;
        break;
      case KEY_DOWN:
        if (dir != UP) dir = DOWN;
        break;
      case KEY_LEFT:
        if (dir != RIGHT) dir = LEFT;
        break;
      case KEY_RIGHT:
        if (dir != LEFT) dir = RIGHT;
        break;
    }
}
bool Snake::isOverlapping() {
  Segment& head = segments.front();
  for (std::list<Segment>::iterator it = std::next(segments.begin()); it != segments.end(); it++) {
    if (head.row == it->row && head.col == it->col) {
      return true;
    }
  }
  return false;
}
bool Snake::isOccupies(int row, int col) {
  for (auto &s: segments) {
    if (s.row == row && s.col == col) return true;
  }
  return false;
}
const Snake::Segment& Snake::getHead() {
  return segments.front();
}
void Snake::grow() {
  isGrowing = true;
}

struct Apple {
  int row;
  int col;

  Apple(Snake& snake);
  void spawn(Snake& snake);
  void draw();
};

Apple::Apple(Snake& snake) {
  spawn(snake);
}
void Apple::spawn(Snake& snake) {
  std::vector<std::tuple<int, int>> freeCells;

  // Getting ALL free cells
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      if (!snake.isOccupies(i, j)) freeCells.push_back(std::tuple<int, int>{i, j});
    }
  }

  // Choosing one and setting row and col
  int index = rand() % freeCells.size();
  std::tie<int, int>(row, col) = freeCells.at(index);
}
void Apple::draw() {
  mvaddch(row, col, 'O');
}

void initCurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, true);
  curs_set(0);

  timeout(250);
  getmaxyx(stdscr, ROWS, COLUMNS);
}

void checkCollision(Snake& snake, Apple& apple, bool& running) {
  // Checking if snake is bitting its body
  if (snake.isOverlapping()) {
    running = false;
    return;
  }

  // Checking snake and apple collision
  auto snakeHead = snake.getHead();
  if (apple.row == snakeHead.row && apple.col == snakeHead.col) {
    snake.grow();
    apple.spawn(snake);
  }
}

int main() {
  initCurses();

  bool running = true;
  Snake snake{ROWS/2, COLUMNS/2};
  Apple apple{snake};

  // Game cycle
  while(running) {
    // Checking collisions and reacting
    checkCollision(snake, apple, running);
    if (!running) break;

    // Getting user input
    int input = getch();

    // Moving snake
    snake.move(input);

    // Drawing things on screen
    clear();
    snake.draw();
    apple.draw();
    refresh();
  }

  endwin();
}