#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATHLEN 200
#define DIRLEN 20
#define MAX 500

struct struct_arg {
    char srcpath[PATHLEN];
    int num;
    char type;
};

char pwd[PATHLEN];
int counter = 0;
char files[MAX][PATHLEN];
int failed_a[MAX];
int failed_b = 0;
struct struct_arg args[MAX];

int is_dir_exist(char *dirpath) {
    struct stat dirstat;
    return stat(dirpath, &dirstat) == 0 && S_ISDIR(dirstat.st_mode);
}

int is_file_exist(char *filepath) {
  struct stat filestat;   
  return stat(filepath, &filestat) == 0 && S_ISREG(filestat.st_mode);
}

int is_hidden(char *filename) {
    return filename[0] == '.' && strcmp(filename, ".") != 0 && 
        strcmp(filename, "..") != 0;
}

void *move(void *argument) {
    int flag = 0;
    struct struct_arg *arg = (struct struct_arg *) argument;
    char *srcpath = (char *) arg->srcpath;
    char *filename = basename(strdup(srcpath));

    if (!is_file_exist(srcpath)){
        flag = 1;
    } else {
        char dirname[DIRLEN];
        if (is_hidden(filename)) {
            strcpy(dirname, "Hidden");
        } else {
            char *dotptr;
            dotptr = strchr(filename, '.');
            if (dotptr == NULL){
                strcpy(dirname, "Unknown");
            } else {
                int i;
                for (i=0; i<strlen(dotptr)-1; i++){
                    dirname[i] = tolower(dotptr[i+1]);
                }
            }
        }

        if(!is_dir_exist(dirname)){
            if (mkdir(dirname, 0777) != 0){
                flag = 1;
            }
        }
        
        char destpath[PATHLEN];
        strcpy(destpath, pwd);
        strcat(destpath, "/");
        strcat(destpath, dirname);
        strcat(destpath, "/");
        strcat(destpath, filename);

        if (rename(srcpath, destpath) != 0){
            flag = 1;
        }
    }
    
    if (arg->type == 'a'){
        failed_a[arg->num] = flag;
    } else if (arg->type == 'b'){
        if (flag == 1){
            failed_b = 1;
        }
    }

}

void listfile(char *basepath) {
    struct dirent *dp;
    DIR *dir = opendir(basepath);

    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            char currpath[PATHLEN];
            strcpy(currpath, basepath);
            strcat(currpath, "/");
            strcat(currpath, dp->d_name);

            if (is_dir_exist(currpath)){
                listfile(currpath);
            } else if (is_file_exist(currpath)){
                strcpy(files[counter], currpath);
                counter++;
            } 
        }
    }
    
    closedir(dir);
}

int main(int argc, char **argv) {
    pthread_t tid[MAX];
    int i, err[MAX];

    getcwd(pwd, sizeof(pwd));
    
    if (strcmp(argv[1], "-f") == 0){
        for (i=0; i<argc-2; i++){
            err[i] = 1;
            strcpy(args[i].srcpath, argv[i+2]);
            args[i].num = i;
            args[i].type = 'a';
            err[i] = pthread_create(&(tid[i]), NULL, move, (void *) &(args[i]));
            if (err[i] != 0){
                failed_a[i] = 1;
            }
        }
        for (i=0; i<argc-2; i++){
            if (err[i] == 0){
                pthread_join(tid[i], NULL);
            }
        }
        for (i=0; i<argc-2; i++){
            if (failed_a[i] == 0){
                printf("File %d : Berhasil Dikategorikan\n", i+1);
            } else if (failed_a[i] == 1){
                printf("File %d : Sad, gagal :(\n", i+1);
            }
        }
    } else if (strcmp(argv[1], "-d") == 0 && argc == 3){
        if (!is_dir_exist(argv[2])){
            printf("Yah, gagal disimpan :(\n");
        } else {
            listfile(argv[2]);
            for (i=0; i<counter; i++){
                err[i] = 1;
                strcpy(args[i].srcpath, files[i]);
                args[i].type = 'b';
                err[i] = pthread_create(&(tid[i]), NULL, move, (void *) &(args[i]));
                if (err[i] != 0){
                    failed_b = 1;
                }
            }
            for (i=0; i<counter; i++){
                if (err[i] == 0){
                    pthread_join(tid[i], NULL);
                }
            }
            if (failed_b == 0){
                printf("Direktori sukses disimpan!\n");
            } else if (failed_b == 1){
                printf("Yah, gagal disimpan :(\n");
            }
        }  
    } else if (strcmp(argv[1], "*") == 0 && argc == 2) {
        listfile(pwd);
        for (i=0; i<counter; i++){
            err[i] = 1;
            strcpy(args[i].srcpath, files[i]);
            args[i].type = 'c';
            err[i] = pthread_create(&(tid[i]), NULL, move, (void *) &(args[i]));
        }
        for (i=0; i<counter; i++){
            if (err[i] == 0){
                pthread_join(tid[i], NULL);
            }
        }
    } 

    return 0;

}
