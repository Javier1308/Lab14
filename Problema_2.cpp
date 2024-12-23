#include <iostream>
#include <vector>
using namespace std;

// Nodo de la matriz dispersa
template <class T>
class Node
{
public:
    T data;
    int pos_row, pos_col;
    Node<T> *next_row;
    Node<T> *next_col;

    Node(T value, int row, int col)
            : data(value), pos_row(row), pos_col(col), next_row(nullptr), next_col(nullptr) {}
};

// Matriz dispersa
template <class T>
class SparseMatrix
{
private:
    vector<Node<T> *> rows;
    vector<Node<T> *> cols;
    int n_rows, n_cols;

public:
    // Constructor
    SparseMatrix(int n_rows, int n_cols)
            : n_rows(n_rows), n_cols(n_cols)
    {
        rows.resize(n_rows, nullptr);
        cols.resize(n_cols, nullptr);
    }

    // Destructor
    ~SparseMatrix()
    {
        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current = rows[i];
            while (current)
            {
                Node<T> *temp = current;
                current = current->next_col;
                delete temp;
            }
        }
    }

    // Insertar un elemento
    void insertar(int row, int col, T value)
    {
        if (value == 0)
            return;

        Node<T> *nuevoNodo = new Node<T>(value, row, col);

        // Inserción en la fila
        if (!rows[row])
        {
            rows[row] = nuevoNodo;
        }
        else
        {
            Node<T> *current = rows[row];
            Node<T> *prev = nullptr;
            while (current && current->pos_col < col)
            {
                prev = current;
                current = current->next_col;
            }
            if (current && current->pos_col == col)
            {
                current->data = value;
                delete nuevoNodo;
                return;
            }
            nuevoNodo->next_col = current;
            if (prev)
                prev->next_col = nuevoNodo;
            else
                rows[row] = nuevoNodo;
        }

        // Inserción en la columna
        if (!cols[col])
        {
            cols[col] = nuevoNodo;
        }
        else
        {
            Node<T> *current = cols[col];
            Node<T> *prev = nullptr;
            while (current && current->pos_row < row)
            {
                prev = current;
                current = current->next_row;
            }
            nuevoNodo->next_row = current;
            if (prev)
                prev->next_row = nuevoNodo;
            else
                cols[col] = nuevoNodo;
        }
    }

    // Eliminar un elemento
    void eliminar(int row, int col)
    {
        if (!rows[row] || !cols[col])
            return;

        // Eliminar de la fila
        Node<T> *current = rows[row];
        Node<T> *prev = nullptr;
        while (current && current->pos_col < col)
        {
            prev = current;
            current = current->next_col;
        }
        if (current && current->pos_col == col)
        {
            if (prev)
                prev->next_col = current->next_col;
            else
                rows[row] = current->next_col;
        }

        // Eliminar de la columna
        current = cols[col];
        prev = nullptr;
        while (current && current->pos_row < row)
        {
            prev = current;
            current = current->next_row;
        }
        if (current && current->pos_row == row)
        {
            if (prev)
                prev->next_row = current->next_row;
            else
                cols[col] = current->next_row;
        }

        delete current;
    }

    // Buscar un elemento
    T buscar(int row, int col)
    {
        Node<T> *current = rows[row];
        while (current && current->pos_col < col)
        {
            current = current->next_col;
        }
        return (current && current->pos_col == col) ? current->data : 0;
    }

    // Sobrecarga del operador () para obtener un valor
    T operator()(int row, int col)
    {
        return buscar(row, col);
    }

    // Comparar dos matrices
    bool comparar(SparseMatrix<T> &otra)
    {
        if (n_rows != otra.n_rows || n_cols != otra.n_cols)
            return false;

        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current1 = rows[i];
            Node<T> *current2 = otra.rows[i];
            while (current1 || current2)
            {
                if (!current1 || !current2 || current1->pos_col != current2->pos_col || current1->data != current2->data)
                {
                    return false;
                }
                current1 = current1->next_col;
                current2 = current2->next_col;
            }
        }
        return true;
    }

    // Suma de matrices
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

    SparseMatrix<T> resta(SparseMatrix<T> &otra)
    {
        if (n_rows != otra.n_rows || n_cols != otra.n_cols)
        {
            throw invalid_argument("Las matrices deben tener las mismas dimensiones para restar");
        }

        SparseMatrix<T> resultado(n_rows, n_cols);

        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current1 = rows[i];
            Node<T> *current2 = otra.rows[i];

            while (current1 || current2)
            {
                if (!current2 || (current1 && current1->pos_col < current2->pos_col))
                {
                    // Solo hay un nodo en la primera matriz
                    resultado.insertar(i, current1->pos_col, current1->data);
                    current1 = current1->next_col;
                }
                else if (!current1 || (current2 && current2->pos_col < current1->pos_col))
                {
                    // Solo hay un nodo en la segunda matriz (se resta como negativo)
                    resultado.insertar(i, current2->pos_col, -current2->data);
                    current2 = current2->next_col;
                }
                else
                {
                    // Ambos nodos existen, restar valores
                    T valor = current1->data - current2->data;
                    if (valor != 0)
                    {
                        resultado.insertar(i, current1->pos_col, valor);
                    }
                    current1 = current1->next_col;
                    current2 = current2->next_col;
                }
            }
        }

        return resultado;
    }

    // Transpuesta de la matriz
    SparseMatrix<T> transpuesta()
    {
        SparseMatrix<T> resultado(n_cols, n_rows);
        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current = rows[i];
            while (current)
            {
                resultado.insertar(current->pos_col, current->pos_row, current->data);
                current = current->next_col;
            }
        }
        return resultado;
    }

    // Multiplicación de matrices
    SparseMatrix<T> multiplicar(SparseMatrix<T> &otra)
    {
        if (n_cols != otra.n_rows)
            throw invalid_argument("Dimensiones incompatibles para multiplicación");

        SparseMatrix<T> resultado(n_rows, otra.n_cols);
        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current1 = rows[i];
            while (current1)
            {
                Node<T> *current2 = otra.cols[current1->pos_col];
                while (current2)
                {
                    T valor = resultado.buscar(i, current2->pos_row) + current1->data * current2->data;
                    resultado.insertar(i, current2->pos_row, valor);
                    current2 = current2->next_row;
                }
                current1 = current1->next_col;
            }
        }
        return resultado;
    }

    void imprimir()
    {
        cout << "Representacion enlazada por filas:" << endl;
        for (int i = 0; i < n_rows; ++i)
        {
            Node<T> *current = rows[i];
            cout << "Fila " << i + 1 << ": ";
            while (current)
            {
                cout << "(" << current->pos_row + 1 << ", " << current->pos_col + 1 << ", " << current->data << ") ";
                current = current->next_col;
            }
            cout << endl;
        }

        cout << "Representacion enlazada por columnas:" << endl;
        for (int j = 0; j < n_cols; ++j)
        {
            Node<T> *current = cols[j];
            cout << "Columna " << j + 1 << ": ";
            while (current)
            {
                cout << "(" << current->pos_row + 1 << ", " << current->pos_col + 1 << ", " << current->data << ") ";
                current = current->next_row;
            }
            cout << endl;
        }
    }
    
    void display()
    {
        
    }


};

// Ejemplo de uso
int main()
{
    SparseMatrix<int> mat1(10, 7);//Hemos asignado un tamaño de 10x7 debido a que simo se pierden valores por ello se asigna un tamaño mayor y empieza en 1 y no en 0
    mat1.insertar(2, 3, 3);
    mat1.insertar(5,1,3);
    mat1.insertar(5,4,1);
    mat1.insertar(7,4,5);
    mat1.insertar(8,1,1);
    mat1.insertar(8,6,4);
    mat1.insertar(9,3,2);
    mat1.imprimir();

    SparseMatrix<int> mat2(10, 7);//Hemos asignado un tamaño de 10x7 debido a que simo se pierden valores por ello se asigna un tamaño mayor y empieza en 1 y no en 0
    mat1.insertar(2,3,4);
    mat1.insertar(5,1,2);
    mat1.insertar(5,4,1);
    mat1.insertar(7,4,4);
    mat1.insertar(8,1,3);
    mat1.insertar(8,6,2);
    mat1.insertar(9,3,1);
    mat1.imprimir();

    SparseMatrix<int> mat3 = mat1.suma(mat2);
    mat3.imprimir();

    return 0;
}
