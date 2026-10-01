#include "file_loader.h"
#include "parser.h"
#include "status.h"


int main(void) {
    Status status = NO_ERROR;
    FileString test_file;


    status = load_file_to_memory("test_text/input.txt", &test_file);
    if (status != NO_ERROR) {
        status_print(status);
        return 1;
    }

    status = parser_start(&test_file);
    if (status != NO_ERROR) {
        status_print(status);
        return 1;
    }


    return 0;
}
