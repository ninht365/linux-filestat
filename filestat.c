#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>

/**
 * @brief Determine file object type based on st_mode field of struct stat.
 * @param mode st_mode value retrieved from struct stat.
 * @return String describing file type (Regular File, Directory, Symbolic Link, ...).
 */
const char *get_file_type(mode_t mode) {
    if (S_ISREG(mode)) {
        return "Regular File";
    } else if (S_ISDIR(mode)) {
        return "Directory";
    } else if (S_ISLNK(mode)) {
        return "Symbolic Link";
    } else if (S_ISCHR(mode)) {
        return "Character Device";
    } else if (S_ISBLK(mode)) {
        return "Block Device";
    } else if (S_ISFIFO(mode)) {
        return "FIFO / Pipe";
    } else if (S_ISSOCK(mode)) {
        return "Socket";
    } else {
        return "Unknown Type";
    }
}

/**
 * @brief Convert timestamp (time_t) to human-readable datetime string: YYYY-MM-DD HH:MM:SS.
 * @param mtime Modification time retrieved from st_mtime.
 * @param buffer Buffer containing formatted datetime string.
 * @param max_size Maximum size of the buffer.
 */
void format_time(time_t mtime, char *buffer, size_t max_size) {
    struct tm *time_info = localtime(&mtime);
    if (time_info == NULL) {
        snprintf(buffer, max_size, "Unknown Time");
        return;
    }
    /* Format: Year-Month-Day Hour:Minute:Second */
    strftime(buffer, max_size, "%Y-%m-%d %H:%M:%S", time_info);
}

int main(int argc, char *argv[]) {
    /* 1. Check exactly 1 path argument from command line */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_path = argv[1];
    struct stat file_stat;

    /* 2. Use lstat() system call to get object metadata */
    if (lstat(file_path, &file_stat) < 0) {
        fprintf(stderr, "Error accessing '%s': %s\n", file_path, strerror(errno));
        return EXIT_FAILURE;
    }

    /* 3. Format last modified time (Last Modified) */
    char time_str[64];
    format_time(file_stat.st_mtime, time_str, sizeof(time_str));

    /* 4. Print detailed object information per assignment requirements */
    printf("--------------------------------------------------\n");
    printf("                 FILE INFORMATION                 \n");
    printf("--------------------------------------------------\n");
    printf("File Path     : %s\n", file_path);
    printf("File Type     : %s\n", get_file_type(file_stat.st_mode));
    printf("Size          : %ld bytes\n", (long)file_stat.st_size);
    printf("Last Modified : %s\n", time_str);
    printf("--------------------------------------------------\n");

    return EXIT_SUCCESS;
}