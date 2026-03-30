#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void print_help() {
    printf("Usage: carikata [options] {pattern} [filename]\n");
    printf("Flags:\n");
    printf("  -i : Ignore case\n");
    printf("  -v : Invert match\n");
    printf("  -h : Show this help message\n");
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int ignore_case = 0, invert = 0;

    while ((opt = getopt(argc, argv, "ivh")) != -1) {
        switch (opt) {
            case 'i': ignore_case = 1; break;
            case 'v': invert = 1; break;
            case 'h': print_help(); return 0;
            default: return 1;
        }
    }

    if (optind >= argc) {
        printf("Usage error. Try 'carikata -h'\n");
        return 1;
    }

    char *pattern = argv[optind];
    char pattern_lower[256];
    strncpy(pattern_lower, pattern, 255);
    pattern_lower[255] = '\0';
    if (ignore_case) to_lowercase(pattern_lower);

    FILE *fp = stdin;
    if (optind + 1 < argc) {
        fp = fopen(argv[optind + 1], "r");
        if (!fp) {
            printf("File not found!\n");
            return 1;
        }
    }

    char line[1024];
    char line_lower[1024];

    while (fgets(line, sizeof(line), fp)) {
        int match = 0;
        if (ignore_case) {
            strncpy(line_lower, line, sizeof(line_lower));
            to_lowercase(line_lower);
            if (strstr(line_lower, pattern_lower) != NULL) match = 1;
        } else {
            if (strstr(line, pattern) != NULL) match = 1;
        }

        if ((match && !invert) || (!match && invert)) {
            printf("%s", line);
        }
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
