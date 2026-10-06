#ifndef SIGNIFICANT_COLUMN_SORT_H
#define SIGNIFICANT_COLUMN_SORT_H

#include <stddef.h>
#include "status.h"

Status significant_column_sort(unsigned char **column_uniqueness, unsigned char **column_index , size_t array_len);

#endif
