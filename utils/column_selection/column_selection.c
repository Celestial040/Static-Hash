#include "column_selection/column_selection.h"
#include "column_selection/by_length.h"
#include <stdlib.h>


Column_Selection column_selection_start(const Keyword_Data data) {
    Column_Selection result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}
    };

    Grouping_By_Length length_grouping_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0}
    };

    length_grouping_result = row_grouping_by_length(data);

    if (length_grouping_result.status != NO_ERROR) {
        result.status = length_grouping_result.status;
        return result;
    }

    return result;
}
