#include <stdio.h>
#include <unistd.h>

void print_help() {
    printf("Usage: facman-rm [options] {package}\n");
    printf("Flags:\n");
    printf("  -f : Force removal\n");
    printf("  -v : Verbose output\n");
    printf("  -h : Show this help message\n");
}

int main(int argc, char *argv[]) {
    int opt, force = 0, verbose = 0;
    while ((opt = getopt(argc, argv, "fvh")) != -1) {
        switch (opt) {
            case 'f': force = 1; break;
            case 'v': verbose = 1; break;
            case 'h': print_help(); return 0;
            default: return 1;
        }
    }

    if (optind >= argc) {
        printf("Error: No package specified. Try 'facman-rm -h'\n");
        return 1;
    }

    char *package = argv[optind];

    if (!force) {
        if (access(package, F_OK) == -1) {
            printf("Package '%s' is not installed.\n", package);
            return 1;
        }
    }

    if (unlink(package) == 0) {
        if (verbose) printf("Removed package: %s\n", package);
    } else {
        if (!force) perror("Failed to remove package");
    }

    return 0;
}
