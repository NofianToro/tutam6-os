#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void print_help() {
    printf("Usage: facman-get [options] {package}\n");
    printf("Flags:\n");
    printf("  -y : Assume yes to all prompts\n");
    printf("  -q : Quiet mode\n");
    printf("  -h : Show this help message\n");
}

int main(int argc, char *argv[]) {
    int opt, yes = 0, quiet = 0;
    while ((opt = getopt(argc, argv, "yqh")) != -1) {
        switch (opt) {
            case 'y': yes = 1; break;
            case 'q': quiet = 1; break;
            case 'h': print_help(); return 0;
            default: return 1;
        }
    }

    if (optind >= argc) {
        printf("Error: No package specified. Try 'facman-get -h'\n");
        return 1;
    }

    char *package = argv[optind];

    if (!yes) {
        printf("Do you want to install '%s'? [Y/n] ", package);
        char resp = getchar();
        if (resp != 'Y' && resp != 'y' && resp != '\n') {
            printf("Installation aborted.\n");
            return 0;
        }
    }

    char url[256];
    sprintf(url, "https://raw.githubusercontent.com/fauzan/facman-repo/main/%s", package);

    pid_t pid = fork();
    if (pid == 0) {
        if (quiet) {
            execlp("wget", "wget", "-q", "-O", package, url, NULL);
        } else {
            execlp("wget", "wget", "-O", package, url, NULL);
        }
        exit(1);
    } else {
        wait(NULL);
    }

    pid_t pid_chmod = fork();
    if (pid_chmod == 0) {
        execlp("chmod", "chmod", "+x", package, NULL);
        exit(1);
    } else {
        wait(NULL);
    }

    if (!quiet) printf("Package '%s' installed successfully.\n", package);
    return 0;
}
