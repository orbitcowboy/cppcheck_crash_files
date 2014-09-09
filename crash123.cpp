/*
/home/martin/wxWidgets/wxWidgets-svn-trunk/src/jpeg/jpegtran.c
 */

int
main (int argc, char **argv)
{

    if ((input_file = fopen(argv[file_index], READ_BINARY)) == NULL)
    {
        fprintf(stderr, "%s: can't open %s\n", progname, argv[file_index]);
        exit(EXIT_FAILURE);
    }
    return 0;
}
