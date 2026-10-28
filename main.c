#include "file_loader.h"
#include "parser.h"
#include "status.h"

int main(void) {
    FileString input_file = {NO_ERROR, NULL, 0};
    Parser parser_result = {NO_ERROR};

    const char *filename = "test_text/input.txt";

    input_file = load_file_to_memory(filename);
    if (input_file.status != NO_ERROR) {
        status_print(input_file.status);
        return 1;
    }

    parser_result = parser_start(input_file, filename);
    if (parser_result.status != NO_ERROR) {
        status_print(parser_result.status);
        return 1;
    }

    return 0;
}
