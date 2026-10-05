#ifndef SORT_H
#define SORT_H

#include "file_loader.h"
#include "parser.h"
#include "status.h"

Status weight_based_quicksort(const FileString *filestring,KeywordData *keyword_data, unsigned short *weight);

#endif
