/* Hecho por:
- Javier Olivares
- Matias Meneses
- Santiago Silva
*/

#include <iostream>
using namespace std;
struct Cell {
    int row, col, value;
    Cell* nextRow;
    Cell* nextCol;
    Cell(int r, int c, int v) : row(r), col(c), value(v), nextRow(nullptr),
                                nextCol(nullptr) {}
};
struct Header {
    int index;
    Cell* next; // puntero al primer nodo
    Header* nextHeader;
    Header(int i) : index(i), next(nullptr), nextHeader(nullptr) {}
};
class SparseMatrix {
private:
    Header* rowHead;
    Header* colHead;
    int rows, cols;
// Buscar o crear cabecera para filas
    Header* getOrCreateRowHeader(int row) {
        Header* prev = nullptr;
        Header* curr = rowHead;
        while (curr && curr->index < row) {
            prev = curr;
            curr = curr->nextHeader;
        }
        if (!curr || curr->index != row) {
            Header* newHeader = new Header(row);
            if (prev) {
                prev->nextHeader = newHeader;
            } else {
                rowHead = newHeader;
            }
            newHeader->nextHeader = curr;
            return newHeader;
        }
        return curr;
    }
// Buscar o crear cabecera para columnas
    Header* getOrCreateColHeader(int col) {
        Header* prev = nullptr;
        Header* curr = colHead;
        while (curr && curr->index < col) {
            prev = curr;
            curr = curr->nextHeader;
        }
        if (!curr || curr->index != col) {
            Header* newHeader = new Header(col);
            if (prev) {
                prev->nextHeader = newHeader;
            } else {
                colHead = newHeader;
            }
            newHeader->nextHeader = curr;
            return newHeader;
        }
        return curr;
    }

    SparseMatrix<T> suma(SparseMatrix<T> &otra)
    {
        SparseMatrix<T> resultado(n_rows, n_cols);
        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current1 = rows[i];
            Node<T> *current2 = otra.rows[i];
            while (current1 || current2)
            {
                if (!current2 || (current1 && current1->pos_col < current2->pos_col))
                {
                    resultado.insertar(i, current1->pos_col, current1->data);
                    current1 = current1->next_col;
                }
                else if (!current1 || (current2 && current2->pos_col < current1->pos_col))
                {
                    resultado.insertar(i, current2->pos_col, current2->data);
                    current2 = current2->next_col;
                }
                else
                {
                    resultado.insertar(i, current1->pos_col, current1->data + current2->data);
                    current1 = current1->next_col;
                    current2 = current2->next_col;
                }
            }
        }
        return resultado;
    }
public:
    SparseMatrix(int r, int c) : rowHead(nullptr), colHead(nullptr), rows(r),
                                 cols(c) {}
    void insert(int row, int col, int value) {
        if (row < 0 || row >= rows || col < 0 || col >= cols || value == 0) return;
        Header* rowHeader = getOrCreateRowHeader(row);
        Header* colHeader = getOrCreateColHeader(col);
        Cell* newCell = new Cell(row, col, value);
// Insertar en la fila
        Cell* rowPrev = nullptr;
        Cell* rowCurr = rowHeader->next;
        while (rowCurr && rowCurr->col < col) {
            rowPrev = rowCurr;
            rowCurr = rowCurr->nextRow;
        }
        if (rowPrev) {
            rowPrev->nextRow = newCell;
        } else {
            rowHeader->next = newCell;
        }
        newCell->nextRow = rowCurr;
// Insertar en la columna
        Cell* colPrev = nullptr;
        Cell* colCurr = colHeader->next;
        while (colCurr && colCurr->row < row) {
            colPrev = colCurr;
            colCurr = colCurr->nextCol;
        }
        if (colPrev) {
            colPrev->nextCol = newCell;
        } else {
            colHeader->next = newCell;
        }
        newCell->nextCol = colCurr;
    }
// Mostrar la matriz
    void display() {
        for (int i = 0; i < rows; ++i) {
            Header* rowHeader = getOrCreateRowHeader(i);
            Cell* cell = rowHeader->next;
            for (int j = 0; j < cols; ++j) {
                if (cell && cell->col == j) {
                    cout << cell->value << " ";
                    cell = cell->nextRow;
                } else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }
    ~SparseMatrix() {
        Header* rowCurr = rowHead;
        while (rowCurr) {
            Cell* cellCurr = rowCurr->next;
            while (cellCurr) {
                Cell* toDelete = cellCurr;
                cellCurr = cellCurr->nextRow;
                delete toDelete;
            }
            Header* toDelete = rowCurr;
            rowCurr = rowCurr->nextHeader;
            delete toDelete;
        }
        Header* colCurr = colHead;
        while (colCurr) {
            Header* toDelete = colCurr;
            colCurr = colCurr->nextHeader;
            delete toDelete;
        }
    }
};
int main() {
    SparseMatrix M1(9, 7);
    M1.insert(2,3,3);
    M1.insert(5,1,3);
    M1.insert(5,4,1);
    M1.insert(7,4,5);
    M1.insert(8,1,1);
    M1.insert(8,6,4);
    M1.insert(9,3,2);
    M1.display();

    cout<<endl;



    SparseMatrix M2(9, 7);
    M2.insert(2,3,4);
    M2.insert(5,1,2);
    M2.insert(5,4,1);
    M2.insert(7,4,4);
    M2.insert(8,1,3);
    M2.insert(8,6,2);
    M2.insert(9,3,1);
    M2.display();

    return 0;
}
