#include <argtable3.h>
#include <assert.h>
#include <jsondiff.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
    struct arg_lit *help;
    struct arg_str *a_str, *b_str;
    struct arg_end *end;

    /* the global arg_xxx structs are initialised within the argtable */
    void *argtable[] = {
        help     = arg_litn(NULL, "help", 0, 1, "display this help and exit"),
        a_str    = arg_str1("a", "json_a", "JSON", "the json string to diff"),
        b_str    = arg_str1("b", "json_b", "JSON", "the json string to diff against"),
        end      = arg_end(20),
    };

    const char* progname = "jsondiff";
    int exitcode = 0;
    int nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf("Jsondiff Tool\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        exitcode = 0;
        goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        exitcode = 1;
        goto exit;
    }

    printf("A is %s\nB is %s\n", a_str->sval[0], b_str->sval[0]);

    json_t *a_json = json_loads(a_str->sval[0], JSON_DECODE_ANY, NULL);
    json_t *b_json = json_loads(b_str->sval[0], JSON_DECODE_ANY, NULL);
    if (a_json == NULL || b_json == NULL) {
        fprintf(stderr, "malformed JSON, got: %s and %s\n", a_str->sval[0], b_str->sval[0]);
        exitcode = 1;
        goto exit;
    }

    json_t *diff = jsondiff_compare(a_json, b_json, 0);
    printf("Diff is %s\n", json_dumps(diff, JSON_ENCODE_ANY));

exit:
    json_decref(a_json);
    json_decref(b_json);
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}