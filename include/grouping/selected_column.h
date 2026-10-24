#ifndef GROUPING_SELECTED_COLUMN_H
#define GROUPING_SELECTED_COLUMN_H

#include "status.h"
#include "parser.h"

Status grouping_by_selected_column(KeywordData *data, unsigned char selected_column, unsigned char *uppercase_start, unsigned char *lowercase_start,
                                   unsigned char start, unsigned char length);

#endif
