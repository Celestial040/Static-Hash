#include "column_selection/column_selection.h"
#include "column_selection/by_length.h"
#include "column_selection/column_pick.h"
#include "column_selection/by_selected_column.h"
#include "char_manip.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char no_pair_left(const Grouping_Buffer grouping) {
    unsigned char i = 0;
    for (i = 0; i < grouping.size; i++) {
        if (grouping.count[i] > 1) {
            return FALSE;
        }
    }
    return TRUE;
}

Column_Selection column_selection_second_stage(
    Column_Selection src_data,
    const Grouping_Buffer grouping) {
    Column_Selection result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, 0, 0}
    };

    Keyword_Data data = {
        NULL, NULL, NULL, 0, 0 ,0
    };

    unsigned_char_Vector best_columns = {
        NULL, 0, 0
    };

/*     Grouping_Buffer new_grouping = {
        NULL, NULL, 0
    };

    Grouping_By_Selected_Column selected_column_grouping_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0}
    }; */

    data = move_keyword_data(&src_data.data);
    best_columns = move_unsigned_char_vector(&src_data.best_column);

    row_grouping_based_on_selected_column(&data, grouping, best_columns.data[best_columns.size-1]);

    return result;
}

Column_Selection column_selection_first_stage(Keyword_Data src_data) {
    Column_Selection result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, 0, 0}
    };

    Keyword_Data data = {
        NULL, NULL, NULL, 0, 0 ,0
    };

    unsigned_char_Vector_Status best_columns = {
        NO_ERROR, {NULL, 0, 0}
    };

    Grouping_By_Length length_grouping_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0, 0}
    };

    unsigned char selected_column = 0;

    data = move_keyword_data(&src_data);

    best_columns = allocate_unsigned_char_vector(data.longest_keyword_len*2);
    if (best_columns.status != NO_ERROR) {
        result.status = best_columns.status;
        return result;
    }

    length_grouping_result = row_grouping_by_length(data);

    if (length_grouping_result.status != NO_ERROR) {
        result.status = length_grouping_result.status;
        return result;
    }

    selected_column = column_pick_based_on_length(length_grouping_result.data,length_grouping_result.grouping);

    insert_unsigned_char_vector(&best_columns.vector, selected_column);

    result.data = length_grouping_result.data;
    result.best_column = best_columns.vector;

    result = column_selection_second_stage(result, length_grouping_result.grouping);

    return result;
}

Column_Selection column_selection_start(Keyword_Data data) {
    Column_Selection result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, 0, 0}
    };

    result = column_selection_first_stage(data);

    return result;
}
