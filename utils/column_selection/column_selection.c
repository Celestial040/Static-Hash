#include "column_selection/column_selection.h"
#include "column_selection/by_length.h"
#include "column_selection/column_pick.h"
#include <stdio.h>
#include <stdlib.h>


Column_Selection column_selection_start(Keyword_Data src_data) {
    Column_Selection result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}
    };

    Keyword_Data data = move_keyword_data(src_data);

    Grouping_By_Length length_grouping_result = {
        NO_ERROR, {NULL, NULL, NULL, 0, 0 ,0}, {NULL, NULL, 0}
    };

    unsigned char selected_column = 0;

    length_grouping_result = row_grouping_by_length(data);

    if (length_grouping_result.status != NO_ERROR) {
        result.status = length_grouping_result.status;
        return result;
    }

/*     for (i = 0; i < buffer.length; i++) {
        printf(
            "%d , %d \n",
            buffer.grouping_start_index[i],
            buffer.grouping_count[i]
        );
    } */


    print_keyword_data_details(length_grouping_result.data);

    selected_column = column_pick_based_on_length(length_grouping_result.data,length_grouping_result.grouping_buffer);

    printf("%d \n", selected_column);

    return result;
}
