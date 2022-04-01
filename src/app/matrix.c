//
// Created by Zbysa on 31.03.2022.

#include "struct.h"
#include "stdlib.h"
#include "stdio.h"

Matrix *new_matrix() {
    Matrix* matrix = (Matrix*) malloc(sizeof(Matrix));
    matrix->column = NULL;
    matrix->row = NULL;
    matrix->columnCount = 0;
    matrix->rowCount = 0;
    return matrix;
}

RowOrColumn *new_row_or_column(unsigned int index) {
    RowOrColumn *rowOrColumn = (RowOrColumn*) malloc(sizeof(RowOrColumn));
    rowOrColumn->next = NULL;
    rowOrColumn->firstValue = NULL;
    rowOrColumn->index = index;
    return rowOrColumn;
}

Value *new_value(double val, unsigned int row, unsigned int column) {
    Value *value = (Value*) malloc(sizeof(Value));
    value->value = val;
    value->column = column;
    value->row = row;
    value->nextValueColumn = NULL;
    value->nextValueRow = NULL;
    return value;

}

RowOrColumn *newLine(size_t rows) {

    RowOrColumn *rowOrColumn_prev = new_row_or_column(0);
    RowOrColumn *start = rowOrColumn_prev;
    RowOrColumn* rowOrColumn;
    for(unsigned int i = 1; i < rows; i++) {
        rowOrColumn = new_row_or_column(i);
        rowOrColumn_prev->next = rowOrColumn;
        rowOrColumn_prev = rowOrColumn;
    }

    return start;
}

RowOrColumn* getRowAtIndex(Matrix *matrix, unsigned int index) {
    RowOrColumn* r = matrix->row;
    while (r->index != index && r->index <= index) {
        r = r->next;
    }
    return r;
}

RowOrColumn* getColumnAtIndex(Matrix *matrix, unsigned int index) {
    RowOrColumn* c = matrix->column;
    while (c->index != index && c->index <= index) {
        c = c->next;
    }
    return c;
}

Value* getPrevValueRow(RowOrColumn* rowOrColumn) {
    if(rowOrColumn == NULL || rowOrColumn->firstValue == NULL) {return NULL;}
    Value *v = rowOrColumn->firstValue;
    while (v->nextValueRow != NULL) {
        v = v->nextValueRow;
    }
    return v;
}

Value* getPrevValueColumn(RowOrColumn* rowOrColumn) {
    if(rowOrColumn == NULL || rowOrColumn->firstValue == NULL) {return NULL;}
    Value *v = rowOrColumn->firstValue;
    while (v->nextValueColumn != NULL) {
        v = v->nextValueColumn;
    }
    return v;
}


Matrix *create_matrix(double *values, size_t rows, size_t cols) {
    Matrix* matrix = new_matrix();
    matrix->rowCount = rows;
    matrix->columnCount = cols;
    matrix->row = newLine(rows);
    matrix->column = newLine(cols);

    int index = 0;
    for(unsigned int r = 0; r < rows; r++) {
        for (unsigned int c = 0; c < cols; c++) {
            double val = values[index++];
            if (val == 0) {continue;}
            Value *value = new_value(val, r, c);
            RowOrColumn *row = getRowAtIndex(matrix, r);
            RowOrColumn *column = getColumnAtIndex(matrix, c);
            Value *row_prev_val = getPrevValueRow(row);
            Value *column_prev_val = getPrevValueColumn(column);

            if (row_prev_val == NULL) {
                row->firstValue = value;
            } else {
                row_prev_val->nextValueRow = value;
            }

            if (column_prev_val == NULL) {
                column->firstValue = value;
            } else {
                column_prev_val->nextValueColumn = value;
            }
        }
    }

    return matrix;
}

Value *valueAt(Matrix *matrix, unsigned int row, unsigned int col) {
    RowOrColumn *rowOrColumn_row = getRowAtIndex(matrix, (int)row);
    Value *v = rowOrColumn_row->firstValue;
    while (v != NULL && v->column <= col) {
        if(v->column == col) {
            return v;
        }
        v = v->nextValueRow;
    }

    return NULL;
}

double element_at(Matrix *matrix, unsigned int row, unsigned int col) {
    Value *v = valueAt(matrix, row, col);
    if(v == NULL) {
        return 0;
    }
    return v->value;
}

//radek je stejny, sloupec se posouva doleva
Value* getPrevRowValue(Matrix *matrix, int row, int col) {
    while (col > 0) {
        Value *value = valueAt(matrix, row, col - 1);
        if(value != NULL) {
            return value;
        }
        col--;

    }
    return NULL;
}


//radek se posouva nahoru, sloupec je stejny
Value* getPrevColumnValue(Matrix *matrix, int row, int col) {
    while (row > 0) {
        Value *value = valueAt(matrix, row - 1, col);
        if(value != NULL) {
            return value;
        }
        row--;
    }
    return NULL;
}


// Nastaví hodnotu na danám řádku a sloupci
double set_element_at(Matrix *matrix, unsigned int row, unsigned int col, double value) {
    int r = (int)row;
    int c = (int)col;
    Value *v = valueAt(matrix, row, col);
    RowOrColumn *buff_row_or_column;
    if(v != NULL) {
        v->value = value;
    } else {
        v = new_value(value, r, c);
        //prev row
        Value *prev_row = getPrevRowValue(matrix, r, c);
        Value *prev_column = getPrevColumnValue(matrix, r, c);

        if (prev_row) {
            v->nextValueRow = prev_row->nextValueRow;
            prev_row->nextValueRow = v;
        } else {
            buff_row_or_column = getRowAtIndex(matrix, r);
            v->nextValueRow = buff_row_or_column->firstValue;
            buff_row_or_column->firstValue = v;
        }

        if (prev_column) {
            v->nextValueColumn = prev_column->nextValueColumn;
            prev_column->nextValueColumn = v;
        } else {
            buff_row_or_column = getColumnAtIndex(matrix, r);
            v->nextValueColumn = buff_row_or_column->firstValue;
            buff_row_or_column->firstValue = v;
        }
    }

    return value;
}

Matrix *add_subtract_helper(Matrix *left, Matrix *right, int plus_or_minus_one) {
    double* values = (double*)malloc(left->rowCount * left->columnCount * sizeof(double));

    int index = 0;
    for(unsigned int r = 0; r < left->rowCount; r++) {
        for (unsigned int c = 0; c < left->columnCount; c++) {
            values[index] = element_at(left, r, c) + (element_at(right, r, c) * plus_or_minus_one);
            index++;
        }
    }
    Matrix *m = create_matrix(values, left->rowCount, left->columnCount);
    free(values);
    return m;
}

Matrix *add(Matrix *left, Matrix *right) {
    return add_subtract_helper(left, right, 1);
}
Matrix *subtract(Matrix *left, Matrix *right) {
    return add_subtract_helper(left, right, -1);
}


//pochopeno jako nevetsi prvek z matice, snad spravne.
//nepochopil jsem ale, proč vracime *double, misto double
double* matrix_max(Matrix *matrix) {
    double max = element_at(matrix, 0, 0);
    double num;
    for(unsigned int r = 0; r < matrix->rowCount; r++) {
        for (unsigned int c = 0; c < matrix->columnCount; c++) {
            num = element_at(matrix, r, c);
            if(num > max) {max = num;}
        }
    }


    double* iter = &max;
    return iter;
}


void print_matrix(Matrix* m) {
    for(unsigned int r = 0; r < m->rowCount; r++) {
        printf("(");
        for(unsigned int c = 0; c < m->columnCount; c++) {
            printf("%f, ", element_at(m, r, c));
        }
        printf(")\n");
    }
}

void delete_values(Value *v) {
    if(v != NULL) {
        delete_values(v->nextValueColumn);
        delete_values(v->nextValueRow);
        free(v);
    }

}

void delete_r(RowOrColumn* r) {
    if(r != NULL) {
        delete_values(r->firstValue);
        delete_r(r->next);
        free(r);
    }
}

void delete_c(RowOrColumn* c) {
    if(c != NULL) {
        delete_c(c->next);
        free(c);
    }
}


void delete(Matrix *matrix) {
    delete_r(matrix->row);
    delete_c(matrix->column);
    free(matrix);
}







