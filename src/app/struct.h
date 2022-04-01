//
// Created by Zbysa on 31.03.2022.
//

#ifndef ZPC_UKOL1_STRUCT_H
#define ZPC_UKOL1_STRUCT_H
#include "stdlib.h"

struct Value {
    unsigned int row;
    unsigned int column;
    double value;
    struct Value *nextValueRow;
    struct Value *nextValueColumn;
} typedef Value;

struct RowOrColumn{
    unsigned int index;
    struct RowOrColumn *next;
    struct Value *firstValue;
} typedef RowOrColumn;

struct Matrix {
    size_t rowCount;
    size_t columnCount;
    RowOrColumn *row;
    RowOrColumn *column;
} typedef Matrix;

#endif //ZPC_UKOL1_STRUCT_H
