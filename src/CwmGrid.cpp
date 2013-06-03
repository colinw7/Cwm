#include "CwmI.h"

struct CwmCell {
  int   x;
  int   y;
  bool  used;
  void *id;
};

CwmGrid::
CwmGrid(CwmScreen &screen1, int cell_width1, int cell_height1) :
 screen(screen1), cell_width(cell_width1), cell_height(cell_height1)
{
  num_cols = screen.getWidth() /cell_width;
  num_rows = screen.getHeight()/cell_height;

  cells = new CwmCell * [num_rows];

  for (int col = 0; col < num_rows; col++)
    cells[col] = new CwmCell [num_cols];

  int y = 0;

  for (int row = 0; row < num_rows; row++) {
    int x = 0;

    for (int col = 0; col < num_cols; col++) {
      CwmCell *cell = &cells[row][col];

      cell->x    = x;
      cell->y    = y;
      cell->used = false;
      cell->id   = NULL;

      x += cell_width;
    }

    y += cell_height;
  }
}

CwmGrid::
~CwmGrid()
{
  for (int row = 0; row < num_rows; row++)
    delete [] cells[row];

  delete [] cells;
}

void
CwmGrid::
add(void *id, int *x1, int *y1, int *x2, int *y2,
    CwmGridInsertType insert_major, CwmGridInsertType insert_minor)
{
  if (*x1 < 0) {
    *x2 += -(*x1);
    *x1  = 0;
  }

  if (*y1 < 0) {
    *y2 += -(*y1);
    *y1  = 0;
  }

  if (*x2 >= screen.getWidth()) {
    *x1 -= *x2 - (screen.getWidth() - 1);
    *x2  = screen.getWidth() - 1;
  }

  if (*y2 >= screen.getHeight()) {
    *y1 -= *y2 - (screen.getHeight() - 1);
    *y2  = screen.getHeight() - 1;
  }

  int width  = *x2 - *x1;
  int height = *y2 - *y1;

  int num_rows1 = height/cell_height;
  int num_cols1 = width /cell_width;

  if (height % cell_height != 0)
    num_rows1++;

  if (width  % cell_width  != 0)
    num_cols1++;

  int row, col;

  for (row = 0; row <= num_rows - num_rows1; row++) {
    CwmCell *cell = &cells[row][0];

    if (*y1 >= cell->y && *y1 < cell->y + cell_height)
      break;
  }

  for (col = 0; col <= num_cols - num_cols1; col++) {
    CwmCell *cell = &cells[0][col];

    if (*x1 >= cell->x && *x1 < cell->x + cell_width)
      break;
  }

  if (row <= num_rows - num_rows1 && col <= num_cols - num_cols1) {
    if (checkCells(row, col, num_rows1, num_cols1)) {
      placeInCells(id, row, col, num_rows1, num_cols1);

      setPosition(row, col, num_rows1, num_cols1, x1, y1, x2, y2);

      return;
    }
  }

  if (insert_major == CWM_INSERT_LEFT ||
      insert_major == CWM_INSERT_RIGHT) {
    int i1 = 0;
    int i2 = 0;
    int j1 = 0;
    int j2 = 0;
    int di = 1;
    int dj = 1;

    if (insert_major == CWM_INSERT_LEFT)
      j2 = num_cols - num_cols1 - 1;
    else {
      j1 = num_cols - num_cols1 - 1;
      dj = -1;
    }

    if (insert_minor == CWM_INSERT_TOP)
      i2 = num_rows - num_rows1 - 1;
    else {
      i1 = num_rows - num_rows1 - 1;
      di = -1;
    }

    for (row = i1; di > 0 ? row <= i2 : row >= i2; row += di) {
      int col;

      for (col = j1; dj > 0 ? col <= j2 : col >= j2; col += dj) {
        if (checkCells(row, col, num_rows1, num_cols1)) {
          placeInCells(id, row, col, num_rows1, num_cols1);
          setPosition(row, col, num_rows1, num_cols1, x1, y1, x2, y2);
          return;
        }
      }
    }
  }
  else {
    int i1 = 0;
    int i2 = 0;
    int j1 = 0;
    int j2 = 0;
    int di = 1;
    int dj = 1;

    if (insert_major == CWM_INSERT_TOP)
      i2 = num_rows - num_rows1 - 1;
    else {
      i1 = num_rows - num_rows1 - 1;
      di = -1;
    }

    if (insert_minor == CWM_INSERT_LEFT)
      j2 = num_cols - num_cols1 - 1;
    else {
      j1 = num_cols - num_cols1 - 1;
      dj = -1;
    }

    for (col = j1; dj > 0 ? col <= j2 : col >= j2; col += dj) {
      for (row = i1; di > 0 ? row <= i2 : row >= i2; row += di) {
        if (checkCells(row, col, num_rows1, num_cols1)) {
          placeInCells(id, row, col, num_rows1, num_cols1);
          setPosition(row, col, num_rows1, num_cols1, x1, y1, x2, y2);
          return;
        }
      }
    }
  }

  if (row < 0 || row >= num_rows - num_rows1)
    row = 0;

  if (col < 0 || col >= num_cols - num_cols1)
    col = 0;

  setPosition(row, col, num_rows1, num_cols1, x1, y1, x2, y2);
}

void
CwmGrid::
remove(void *id)
{
  bool found = false;

  for (int row = 0; row < num_rows; row++)
    for (int col = 0; col < num_cols; col++)
      if (cells[row][col].id == id) {
        cells[row][col].used = false;
        cells[row][col].id   = NULL;

        found = true;
      }
}

bool
CwmGrid::
checkCells(int row, int col, int num_rows1, int num_cols1)
{
  for (int i = 0; i < num_rows1; i++)
    for (int j = 0; j < num_cols1; j++)
      if (cells[row + i][col + j].used)
        return false;

  return true;
}

void
CwmGrid::
placeInCells(void *id, int row, int col, int num_rows1, int num_cols1)
{
  for (int i = 0; i < num_rows1; i++)
    for (int j = 0; j < num_cols1; j++) {
      cells[row + i][col + j].used = true;
      cells[row + i][col + j].id   = id;
    }
}

void
CwmGrid::
setPosition(int row, int col, int num_rows1, int num_cols1,
            int *x1, int *y1, int *x2, int *y2)
{
  CwmCell *cell = &cells[row][col];

  int width  = *x2 - *x1;
  int height = *y2 - *y1;

  int dx = (num_cols1*cell_width  - width )/2;
  int dy = (num_rows1*cell_height - height)/2;

  *x1 = cell->x + dx;
  *y1 = cell->y + dy;
  *x2 = *x1 + width;
  *y2 = *y1 + height;
}

void
CwmGrid::
printMap()
{
  for (int row = 0; row < num_rows; row++) {
    for (int col = 0; col < num_cols; col++)
      if (cells[row][col].used)
        fputc('X', stdout);
      else
        fputc('.', stdout);

    fputc('\n', stdout);
  }
}
