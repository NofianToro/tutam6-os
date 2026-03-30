#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int reverse = 0;
int numeric = 0;

void print_help() {
    printf("Usage: urutkan [options] [filename]\n");
    printf("Flags:\n");
    printf("  -r : Reverse sorting\n");
    printf("  -n : Numeric sorting\n");
    printf("  -h : Show this help message\n");
}

int compare(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    if (numeric) {
        float num1 = atof(str1);
        float num2 = atof(str2);
        if (num1 < num2) return reverse ? 1 : -1;
        if (num1 > num2) return reverse ? -1 : 1;
        return 0;
    }

    int res = strcmp(str1, str2);
    return reverse ? -res : res;
}

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "rnh")) != -1) {
        switch (opt) {
            case 'r': reverse = 1; break;
            case 'n': numeric = 1; break;
            case 'h': print_help(); return 0;
            default: return 1;
        }
    }

    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (!fp) {
            printf("File not found!\n");
            return 1;
        }
    }

    char **lines = NULL;
    char buffer[1024];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        lines = realloc(lines, (count + 1) * sizeof(char *));
        lines[count] = strdup(buffer);
        count++;
    }

    if (fp != stdin) fclose(fp);

    qsort(lines, count, sizeof(char *), compare);

    for (int i = 0; i < count; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    free(lines);

    return 0;
}
