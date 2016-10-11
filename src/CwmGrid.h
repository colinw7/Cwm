enum CwmGridInsertType {
  CWM_INSERT_LEFT,
  CWM_INSERT_RIGHT,
  CWM_INSERT_TOP,
  CWM_INSERT_BOTTOM,
};

struct CwmCell;

class CwmGrid {
 private:
  CwmScreen  &screen;
  CwmCell   **cells;
  int         num_cols;
  int         num_rows;
  int         cell_width;
  int         cell_height;

 public:
  CwmGrid(CwmScreen &screen, int cell_width, int cell_height);
 ~CwmGrid();

  void add(CwmData id, int *x1, int *y1, int *x2, int *y2,
           CwmGridInsertType insert_major, CwmGridInsertType insert_minor);

  bool remove(CwmData id);

  bool checkCells(int row, int col, int num_rows1, int num_cols1);

  void placeInCells(void *id, int row, int col, int num_rows1, int num_cols1);

  void setPosition(int row, int col, int num_rows1, int num_cols1,
                   int *x1, int *y1, int *x2, int *y2);

  void printMap();
};
