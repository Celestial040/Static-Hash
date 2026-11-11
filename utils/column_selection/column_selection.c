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

Keyword_Data__Best_Columns column_selection(Keyword_Data data) {
    Keyword_Data__Best_Columns result = {
        {NULL, NULL, NULL, 0, 0, 0}, {NULL, 0, 0}, NO_ERROR
    };

    Keyword_Data__Grouping grouping_result = {
        {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0, 0}, NO_ERROR
    };

    Unsigned_Char_Vector_Status best_columns = {
        NO_ERROR, {NULL, 0, 0}
    };

    unsigned char selected_column = 0;
    Pair_Check_Result pair_check_result =  {0, 0};
    Status insert_status = NO_ERROR;
    unsigned char duplicate_group_start = 0;
    unsigned char duplicate_group_count = 0;

    grouping_result = row_grouping_by_length(move_keyword_data(&data));

    if (grouping_result.status != NO_ERROR) {
        result.status = grouping_result.status;
        return result;
    }

    selected_column = column_pick_based_on_length(
        grouping_result.data,
        grouping_result.grouping
    );

    best_columns = allocate_unsigned_char_vector(grouping_result.data.longest_keyword_len);

    if (best_columns.status != NO_ERROR) {
        result.status = best_columns.status;
        return result;
    }

    grouping_result = row_grouping_based_on_selected_column(
        grouping_result.data,
        move_grouping_buffer(&grouping_result.grouping),
        selected_column,
        grouping_result.data.shortest_keyword_len,
        grouping_result.data.longest_keyword_len - grouping_result.data.shortest_keyword_len + 1
    );

    if (grouping_result.status != NO_ERROR) {
        result.status = grouping_result.status;
        return result;
    }

    pair_check_result = no_pair_left(grouping_result.grouping);

    insert_status = insert_unsigned_char_vector(&best_columns.vector, selected_column);
    if (insert_status != NO_ERROR) {
        result.status = insert_status;
        return result;
    }

    while (pair_check_result.no_more_pair_exist == FALSE) {
        if (best_columns.vector.size == grouping_result.data.longest_keyword_len) {

            duplicate_group_start =
                grouping_result.grouping.start[pair_check_result.pair_index_location];

            duplicate_group_count =
                grouping_result.grouping.count[pair_check_result.pair_index_location];

            printf("Error duplicate keyword\n");

            printf(
                "Duplicate on '%.*s' keyword \n",
                grouping_result.data.keywords_length[duplicate_group_start],
                grouping_result.data.keywords + grouping_result.data.longest_keyword_len * grouping_result.grouping.start[pair_check_result.pair_index_location]
            );

            printf("There are %d duplicates of it\n", duplicate_group_count - 1);

            result.status = KEYWORD_DUPLICATE_ERROR;
            return result;
        }

        selected_column = column_pick_based_on_previous_selected_column(
            grouping_result.data,
            grouping_result.grouping,
            best_columns.vector
        );

        grouping_result = row_grouping_based_on_selected_column(
            grouping_result.data,
            move_grouping_buffer(&grouping_result.grouping),
            selected_column,
            0,
            grouping_result.data.keyword_count
        );

        if (grouping_result.status != NO_ERROR) {
            result.status = grouping_result.status;
            return result;
        }

        pair_check_result = no_pair_left(grouping_result.grouping);

        insert_status = insert_unsigned_char_vector(&best_columns.vector, selected_column);
        if (insert_status != NO_ERROR) {
            result.status = insert_status;
            return result;
        }
    }

    print_keyword_data_details(grouping_result.data);


    result.data = grouping_result.data;
    result.best_columns = best_columns.vector;

    return result;
}
