#include <stdio.h>
#include <unistd.h>

void print_help() {
    printf("Usage: facman-list [options]\n");
    printf("Flags:\n");
    printf("  -n : Show only not installed packages\n");
    printf("  -i : Show only installed packages\n");
    printf("  -h : Show this help message\n");
}

int main(int argc, char *argv[]) {
    int opt, not_installed = 0, installed_only = 0;
    while ((opt = getopt(argc, argv, "nih")) != -1) {
        switch (opt) {
            case 'n': not_installed = 1; break;
            case 'i': installed_only = 1; break;
            case 'h': print_help(); return 0;
            default: return 1;
        }
    }

    const char *packages[] = {"carikata", "urutkan", "facman-get", "facman-rm", "facman-list"};
    int num_packages = 5;

    printf("Facman Repository (NofianToro/tutam6-os):\n");
    for (int j = 0; j < num_packages; j++) {
        int is_installed = (access(packages[j], F_OK) != -1);

        if (not_installed && is_installed) continue;
        if (installed_only && !is_installed) continue;

        printf("[%s] %s\n", is_installed ? "x" : " ", packages[j]);
    }

    return 0;
}
