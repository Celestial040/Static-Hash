#include "file_loader.h"
#include "parser.h"
#include "status.h"

int main(void) {
    OptionalFileString input_file = {NO_ERROR, {NULL, 0}};
    OptionalParser parser_result = {NO_ERROR};

    const char *filename = "test_text/input.txt";

    input_file = load_file_to_memory(filename);
    if (input_file.status != NO_ERROR) {
        status_print(input_file.status);
        return 1;
    }

    parser_result = parser_start(input_file.filestring, filename);
    if (parser_result.status != NO_ERROR) {
        status_print(parser_result.status);
        return 1;
    }

    return 0;
}
