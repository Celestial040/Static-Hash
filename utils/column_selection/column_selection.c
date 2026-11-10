#include "column_selection/column_selection.h"
#include "column_selection/by_length.h"
#include "column_selection/column_pick.h"
#include "column_selection/by_selected_column.h"
#include "char_manip.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Pair_Check_Result {
    unsigned char no_more_pair_exist;
    unsigned char pair_index_location;
}Pair_Check_Result;


Pair_Check_Result no_pair_left(const Grouping_Buffer grouping) {
    Pair_Check_Result result = {0,0};
    unsigned char i = 0;
    for (i = 0; i < grouping.size; i++) {
        if (grouping.count[i] > 1) {
            result.no_more_pair_exist = FALSE;
            result.pair_index_location = i;
            return result;
        }
    }
    result.no_more_pair_exist = TRUE;
    return result;
}

Column_Selection column_selection_second_stage(Column_Selection_Internal_Result *first_stage_result) {
    Column_Selection result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0, 0},
        {NULL, 0, 0},
    };

    Grouping_By_Selected_Column selected_column_grouping_result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0},
        {NULL, NULL, 0, 0}
    };

    unsigned_char_Vector_Status best_columns = {
        NO_ERROR,
        {NULL, 0, 0}
    };

    Column_Selection_Internal_Result iteration_data = {
        {NULL, NULL, NULL, 0, 0 ,0},
        {NULL, NULL, 0, 0},
        NO_ERROR,
        0
    };

    Pair_Check_Result pair_check_result =  {0,0};
    unsigned char next_selected_column = 0;
    Status insert_status = NO_ERROR;

    best_columns = allocate_unsigned_char_vector(first_stage_result->data.longest_keyword_len);
    if (best_columns.status != NO_ERROR) {
        result.status = best_columns.status;
        return result;
    }

    selected_column_grouping_result = row_grouping_based_on_selected_column(
        first_stage_result,
        first_stage_result->data.shortest_keyword_len,
        first_stage_result->data.longest_keyword_len - first_stage_result->data.shortest_keyword_len + 1
    );

    pair_check_result = no_pair_left(selected_column_grouping_result.grouping);

    insert_status = insert_unsigned_char_vector(&best_columns.vector, first_stage_result->selected_column);
    if (insert_status != NO_ERROR) {
        result.status = insert_status;
        return result;
    }

    while (pair_check_result.no_more_pair_exist == FALSE) {
        if (best_columns.vector.size == first_stage_result->data.longest_keyword_len) {
            printf("Error duplicate keyword");
            result.status = MISMATCH_EXPECTATION;
            return result;
        }

        next_selected_column = column_pick_based_on_previous_selected_column(
            selected_column_grouping_result.data,
            selected_column_grouping_result.grouping,
            best_columns.vector
        );

        iteration_data.data = selected_column_grouping_result.data;
        iteration_data.grouping = selected_column_grouping_result.grouping;
        iteration_data.selected_column = next_selected_column;

        selected_column_grouping_result = row_grouping_based_on_selected_column(
            &iteration_data,
            0,
            selected_column_grouping_result.data.keyword_count
        );

        pair_check_result = no_pair_left(selected_column_grouping_result.grouping);
    }

    print_keyword_data_details(selected_column_grouping_result.data);

    return result;
}

Column_Selection_Internal_Result column_selection_first_stage(Keyword_Data *data) {
    Column_Selection_Internal_Result result = {
        {NULL, NULL, NULL, 0, 0, 0},
        {NULL, NULL, 0, 0},
        NO_ERROR,
        0
    };

    Grouping_By_Length length_grouping_result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0 ,0},
        {NULL, NULL, 0, 0}
    };

    length_grouping_result = row_grouping_by_length(data);

    if (length_grouping_result.status != NO_ERROR) {
        result.status = length_grouping_result.status;
        return result;
    }

    result.selected_column = column_pick_based_on_length(length_grouping_result.data,length_grouping_result.grouping);
    result.grouping = length_grouping_result.grouping;
    result.data = length_grouping_result.data;

    return result;
}

Column_Selection column_selection_start(Keyword_Data *data) {
    Column_Selection result = {
        NO_ERROR,
        {NULL, NULL, NULL, 0, 0, 0},
        {NULL, 0, 0}
    };

    Column_Selection_Internal_Result first_stage_result = {
        {NULL, NULL, NULL, 0, 0, 0},
        {NULL, NULL, 0, 0},
        NO_ERROR,
        0
    };

    first_stage_result = column_selection_first_stage(data);
    result = column_selection_second_stage(&first_stage_result);

    return result;
}
