

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>   /* for printf().  man 3 printf */
#include <stdlib.h>  /* for exit().    man 3 exit   */
#include <string.h>  /* for strcat().  man strcat   */

#include <errno.h>
#include "lab_png.h"
#include "crc.h"
#include "zutil.h"


int set_ptr_file_type( char* fname, char* ptr)
{
    int i;
    char* ptr;
    struct stat buf;

   // for (i = 1; i < argc; i++) {
       // printf("%s: ", fname);
        if (lstat(fname, &buf) < 0) {
            perror("lstat error");
            continue;
        }

        if (S_ISREG(buf.st_mode))  ptr = "regular"; // if regular check_if_png(File *fp)
        else if (S_ISDIR(buf.st_mode))  ptr = "directory";
        else if (S_ISCHR(buf.st_mode))  ptr = "character special";
        else if (S_ISBLK(buf.st_mode))  ptr = "block special";
        else if (S_ISFIFO(buf.st_mode)) ptr = "fifo";
#ifdef S_ISLNK
        else if (S_ISLNK(buf.st_mode))  ptr = "symbolic link";
#endif
#ifdef S_ISSOCK
        else if (S_ISSOCK(buf.st_mode)) ptr = "socket";
#endif
        else                            ptr = "**unknown mode**";
        //printf("%s\n", ptr);
    //}
    return 0;
}


/**
 * @file: findpng.c
 * @brief: search directory tree and list all png files
  */

int main(int argc, char* argv[]) {

    if (argc == 1) {
        fprintf(stderr, "Usage: %s <directory name>\n", argv[0]);
        exit(1);
    }

    list_dir_files(argv[0]);


    return 0;
}



int list_dir_files( char* dir)
{
    DIR* p_dir;
    struct dirent* p_dirent;
    char str[64];

 /*   if (argc == 1) {
        fprintf(stderr, "Usage: %s <directory name>\n", dir);
        exit(1);
    }*/

    if ((p_dir = opendir(dir)) == NULL) {
        sprintf(str, "opendir(%s)", dir);
        perror(str);
        exit(2);
    }

    while ((p_dirent = readdir(p_dir)) != NULL) {
        char* str_path = p_dirent->d_name;  /* relative path name! */

        if (str_path == NULL) {
            fprintf(stderr, "Null pointer found!");
            exit(3);
        }
        else {
            //printf("%s\n", str_path);
            char* ftype = "\0";
            set_ptr_file_type(str_path, ftype);

            if (*ftype == "regular") {
                //png check
                check_if_png(fopen(str_path, "rb"))
            }
            if (*ftype == "directory") {
                list_dir_files(str_path);
            }
            //get file type, if dir call list_dir_files
        }
    }

    if (closedir(p_dir) != 0) {
        perror("closedir");
        exit(3);
    }

    return 0;
}
