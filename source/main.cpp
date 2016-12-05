#include <cstring>
#include <string>

int main(int argc, char**argv)
{
    if (argc != 2) {
        printf("Usage: meow [server|client]\n");
        return 1;
    }
    else if (!strcmp(argv[1], "server")) {
        printf("Starting server\n");
        return 0;
    }
    else if (!strcmp(argv[1], "client"))  {
        printf("Starting client\n");
        return 0;
    }
    else {
        printf("Unrecognized command-line option: %s\n", argv[1]);
        return 1;
    }
}

