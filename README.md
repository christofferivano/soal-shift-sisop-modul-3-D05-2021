# soal-shift-sisop-modul-3-D05-2021

## 2a.
Membuat program perkalian matrix 4x3 dengan matrix 3x6 dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20. Pertama-tama inisialisasi key dan array `*mat` agar dapat digunakan dalam shared memory pada program kedua.
```
key_t key = 1234;
int shmid = shmget(key, sizeof(int) * 4 * 6, IPC_CREAT | 0666);
int *mat = (int *)shmat(shmid, NULL, 0);
```
membuat 2 matrix dengan input dari user untuk perkalian biasa menggunakan for loop. Matrix pertama memiliki besar 4x3, matrix kedua memiliki besar 3x6, dan hasil perkalian matrix memiliki besar 4x6.
```
printf("Matrix 1 : [4][3]\n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			scanf("%d", &matrix1[i][j]);
		}
	}
	printf("Matrix 2 : [3][6]\n");
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			scanf("%d", &matrix2[i][j]);
		}
	}
	int hasil = 0;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				hasil += matrix1[i][k] * matrix2[k][j];
			}
			matrix[i][j] = hasil;
			hasil = 0;
		}
	}
```
Selanjutnya memasukkan value dari matrix yang telah dikali ke array yang akan digunakan dalam shared memory yaitu matrix `mat`.
```
for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			//mat[i * 6 + j] = matrix[i][j];
			matrix[i][j] = mat[i * 6 + j];
		}
	}
```
Lalu, jalankan program 2b.
```
printf("Run Program 2b.\n");
for(int i = 30; i > 0; i--)
{
	printf("%d...\n", i);
	sleep(1);
}
```
![image](https://user-images.githubusercontent.com/73484021/119261173-70988e00-bc00-11eb-8709-3707285d1997.png)

## 2b
Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya.

Pertama-tama menginisialisasi key dan arr agar dapat digunakan shared memorynya dari program pertama, lalu assign valuenya pada matrix lokal pada program ke 2.
```
key_t key = 1234;
int shmid = shmget(key, sizeof(int) * 4 * 6, IPC_CREAT | 0666);
int *mat = (int *)shmat(shmid, NULL, 0);
int matrix[4][6];
for(int i = 0; i < 4; i++)
{
	for(int j = 0; j < 6; j++)
	{
		matrix[i][j] = mat[i * 6 + j];
	}
}
```
Selanjutnya menginputkan matrix untuk melakukan perhitungan angka faktorial sesuai besar matrix hasil perkalian.
```
int matBaru[4][6];
printf("Masukkan nilai matrix[4][6] ; \n");
for(int i = 0; i < 4; i++)
{
	for(int j = 0; j < 6; j++)
	{
		scanf("%d", &matBaru[i][j]);
	}
}
```
Lalu untuk setiap kotak matrix inputan, dilakukan perbandingan terhadap matrix hasil perkalian dari program 1 dan dilakukan perhitungan angka faktorialnya sesuai permintaan soal.
```
printf("Output Matrix : \n");
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			if(matrix[i][j] >= matBaru[i][j])
			{
				x = matrix[i][j];
				y = matBaru[i][j];
				stemps.temp1 = x;
				stemps.temp2 = y;
			}
			else if(matBaru[i][j] > matrix[i][j])
			{
				x = matrix[i][j];
				stemps.temp1 = x;
				stemps.temp2 = x;
			}
			else if(matrix[i][j] == 0)
			{
				stemps.temp1 = 0;
				stemps.temp2 = 0;
			}
			pthread_create(&tid[flag], NULL, &func, (void *)&stemps);
			pthread_join(tid[flag], NULL);
			flag++;
		}
		printf("n");
	}
```
Untuk fungsi yang dipakai yaitu pertama `void printFact(int x, int y)`, yaitu untuk melakukan print hasil faktorial, x sebagai batas atasnya dan x-y sebagai batas bawahnya.
```
void printFact(int x, int y)
{
	int bawah = x - y;
	int hasil = 1;
	printf("[");
	if(x == 0 || y == 0)
	{
		printf("0");
	}
	else
	{
		for(int i = x; i > 0; i--, x--)
		{
			if(i == bawah)
			{
				break;
			}
			else
			{
				hasil *= x;
			}
		}
		printf("%d", hasil);
	}
	printf("] ");
}
```
Lalu untuk fungsi yang memanggil fungsi `void printFact(int, int)` tadi dalam bentuk thread merupakan fungsi `void *func(void temps)`, yaitu:
```
void *func(void *temps)
{
	struct stemp *stemps = (struct stemp *)temps;
	printFact(stemps->temp1, stemps->temp2);
}
```
Didalam fungsi ini menggunakan struct sebagai argumen karena pada parameter fungsi menggunakan lebih dari 1 variabel, yaitu x dan y. Struct didefinisikan di awal program, yaitu:
```
struct stemp
{
	int temp1;
	int temp2;
};
```
![image](https://user-images.githubusercontent.com/73484021/119262031-f8cc6280-bc03-11eb-94dc-160274cb0170.png)
Kesulitan yang dialami pada soal ini adalah saat ingin memindahkan isi dari array di program pertama ke program kedua. Hal ini terjadi karena saya salah menginputkan parameter pada salah satu fungsi shared memorynya.

## 3. Mengategorikan File
Pada soal ini, kita diminta untuk mengategorikan file berdasarkan nama eksistensi file dengan memanfaatkan `multithreading`. File-file ini akan dipindahkan ke dalam `folder` yang berada di dalam `current working directory` dengan nama `eksistensi file` tersebut. Nama folder (eksistensi file) diambil dari `titik terdepan` nama file tersebut dan `tidak case sensitive`, sehingga abc.tar.gz akan berada di dalam folder yg sama dengan bcd.TAR.GZ. Kemudian, jika file tersebut tidak memiliki eksistensi, maka akan dimasukkan ke dalam folder `Unknown` dan jika file tersebut merupakan file hidden, maka file tersebut akan dimasukkan ke dalan folder `Hidden`.

Perintah yang diberikan untuk menjalankan thread tersebut adalah sebagai berikut.
```
./nama_executable_file -f path/to/file1 path/to/file2 path/to/file3
```

Adapun format output yang diinginkan adalah sebagai berikut.
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```

Untuk dapat menjalankan perintah yang diinginkan, pertama, kita harus mendeklarasikan array yang digunakan untuk menyimpan thread id (`tid`) sebanyak MAX dan array yang digunakan untuk menyimpan nilai error dari pembuatan thread (`err`) sebanyak MAX. Selain itu, kita juga mencari path dari current working directory dan menyimpannya ke dalam variable `pwd`.
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

...

char pwd[PATHLEN];

...

int main(int argc, char **argv) {
    pthread_t tid[MAX];
    int i, err[MAX];

    getcwd(pwd, sizeof(pwd));
    
    ...

    return 0;

}
```

Adapun MAX dan PATHLEN didefinisikan sebagai berikut.
```
#define PATHLEN 200
#define DIRLEN 20
#define MAX 500
```

Kemudian, kita dapat meng-create thread untuk menjalankan perintah yang diminta.
```

...

char pwd[PATHLEN];
int failed_a[MAX];
struct struct_arg args[MAX];

...

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
    } 
    
    ...

    return 0;

}
```

Untuk meng-create 1 thread untuk 1 file, kita dapat melakukan `looping` sebanyak jumlah file, yaitu `argc-2`. Kemudian, untuk mencegah program menunggu thread yang gagal di-create, kita dapat menginisialisasi nilai `err[i]` dengan `1`. Jika selama pembuatan thread, nilai err[i] berubah menjadi `0` (sukses), maka thread akan di-`join`-kan. Jika tidak, maka `failed_a[i]` akan bernilai `1`. Array failed_a ini digunakan untuk menyimpan informasi kegagalan dari proses pengategorian suatu file, dimana nilai 1 menandakan kegagalan dan mengoutputkan `File i+1 : Sad, gagal :(` sedangkan nilai 0 (nilai awal) menandakan keberhasilan dan mengoutputkan `File i+1 : Berhasil Dikategorikan`.

Dalam pembuatan thread, kita dapat menggunakan `&(tid[i])` sebagai alamat memori dari thread id, `NULL` sebagai attribut thread (default), `move` sebagai fungsi thread, dan `(void *) &(args[i]))` sebagai alamat dari struct yang menyimpan argumen fungsi thread. Adapun isi dari struct struct_arg args tersebut adalah sebagai berikut.
```
struct struct_arg {
    char srcpath[PATHLEN];
    int num;
    char type;
};
```
Dimana `srcpath` berisi path dari file, `num` berisi nomor file yang dimulai dari angka 0, dan `type` berisi tipe dari soal yang diminta, dapat berupa `a`, `b`, atau `c`. Untuk mengategorikan file-file, tipe yang digunakan adalah `a` dan srcpath didapat dari `argv[i+2]`.

Adapun fungsi thread move adalah sebagai berikut.
```
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
    } 
    
    ...

}
```

Variabel `flag` digunakan untuk menandakan error yang terjadi selama proses pemindahan file dan diinisialisasi dengan angka 0 untuk menandakan belum ada error yang terjadi. Variabel `srcpath` digunakan untuk menyimpan srcpath dari struct argument dan variabel `filename` digunakan untuk menyimpan nama file yang diperoleh dari srcpath. Kemudian, kita dapat mengecek apakah file pada srcpath ada dengan memanggil fungsi `is_file_exist` dengan parameter srcpath.
```
int is_file_exist(char *filepath) {
  struct stat filestat;   
  return stat(filepath, &filestat) == 0 && S_ISREG(filestat.st_mode);
}
```

Jika file tidak ditemukan, maka flag akan di-set menjadi 1. Namun, jika file ditemukan, maka akan mengecek apakah file tersebut merupakan file hidden dengan memanggil fungsi `is_hidden` dengan parameter filename.
```
int is_hidden(char *filename) {
    return filename[0] == '.' && strcmp(filename, ".") != 0 && 
        strcmp(filename, "..") != 0;
}
```

Jika file merupakan file hidden, maka `dirname` akan di-set menjadi `Hidden`. Jika tidak, maka akan mencari lokasi dot (`dorptr`) untuk mencari tau apakah file memiliki eksistensi atau tidak, dimana lokasi dot yang dicari adalah lokasi dot pertama yang ditemukan. Jika tidak ada dot yang ditemukan, maka dirname akan di-set menjadi `Unknown`. Jika dot ditemukan, maka dirname di-set menjadi string setelah lokasi dot (`nama eksistensi`), dimana string diubah menjadi huruf kecil untuk menyamakan case sensitive.

Selanjutnya, membuat direktori sesuai dirname yang diperoleh, jika direktori tersebut tidak ada pada current working directory. Untuk mengecek apakah direktori sudah ada atau belum, kita dapat memanggil fungsi `is_dir_exist` dengan parameter dirname. Jika direktori gagal dibuat, maka flag akan di-set menjadi 1.
```
int is_dir_exist(char *dirpath) {
    struct stat dirstat;
    return stat(dirpath, &dirstat) == 0 && S_ISDIR(dirstat.st_mode);
}
```

Selanjutnya, kita dapat membuat `destpath` dengan format `pwd/dirname/filename` dan me-`rename` path yang dimiliki file dari srcpath menjadi destpath yang baru saja dibuat. Jika rename gagal dilakukan, maka flag akan di-set dengan 1. Kemudian, kita dapat mengeset `failed_a[arg->num]` dengan nilai dari flag untuk mengoutputkan output yang diinginkan nanti.

Selain memindahkan file-file yang dituliskan path nya, soal ini juga meminta memindahkan file-file dalam sebuah `direktori` yang diketahui path nya secara `rekursif`. Output yang diinginkan adalah `Direktori sukses disimpan!` jika berhasil dipindahkan dan `Yah, gagal disimpan :(` apabila gagal. Adapun perintah yang diberikan adalah sebagai berikut.
```
./nama_executable_file -d path/to/direktori
```

Untuk memindahkan file-file dalam sebuah direktori yang diketahui path nya secara rekursif, kita dapat melakukan hal yang sama seperti memindahkan file-file yang dituliskan path nya. Yang membedakan adalah variabel yang menyatakan kegagalan untuk proses pemindahan file-file tersebut. Apabila pada permintaan sebelumnya kita diminta untuk mengecek kegagalan setiap filenya, pada permintaan kali ini kita diminta untuk mengecek dan mengoutputkan kegagalan secara keseluhan sehingga kegagalan dalam pemindahan 1 file dianggap sebagai kegagalan dalam pemindahan 1 direktori. Untuk mewakili kegagalan tersebut, kita dapat menggunakan variabel `failed_b` yang diinisialisasi dengan nilai 0 untuk menyatakan bahwa belum ada kegagalan yang terjadi.  

```

...

char pwd[PATHLEN];
int counter = 0;
char files[MAX][PATHLEN];
int failed_b = 0;
struct struct_arg args[MAX];

...

int main(int argc, char **argv) {
    pthread_t tid[MAX];
    int i, err[MAX];

    getcwd(pwd, sizeof(pwd));
    
    ...
    
    else if (strcmp(argv[1], "-d") == 0 && argc == 3){
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
    } 
    
    ...

    return 0;

}
```

Kemudian, untuk mendapatkan list path file dalam direktori, kita dapat memanggil fungsi `listfile` dengan parameter path direktori yang didapat yaitu pada `argv[2]`. Hasil list path file ini nanti nya akan disimpan dalam array `files`.

```
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
```

Selanjutnya, karena `args[i].type` di-set menjadi `b` pada main, maka apabila terjadi kegagalan dalam pemindahan salah satu file, nilai `failed_b` akan diubah menjadi 1 dalam fungsi thread `move` jika `arg->type` adalah b.

```
void *move(void *argument) {
    
    ...
    
    else if (arg->type == 'b'){
        if (flag == 1){
            failed_b = 1;
        }
    }

}
```

Selain kedua tipe pemindahan file tersebut, pada soal ini juga diminta untuk memindahkan seluruh file pada `current working directory` secara `rekursif`. Adapun format perintah yang diberikan adalah sebagai berikut.
```
./nama_executable_file \*
```

Untuk menyelesaikan perintah ini, kita dapat melakukan cara yang sama seperti memindahkan direktori yang diketahui pathnya. Yang membedakan adalah kita akan memanggil fungsi `listfile` dengan parameter `pwd` (current working directory), `args[i].type` di-set menjadi c, dan tidak ada output yang dikeluarkan.

```
int main(int argc, char **argv) {
    pthread_t tid[MAX];
    int i, err[MAX];

    getcwd(pwd, sizeof(pwd));
    
    ...
    
    else if (strcmp(argv[1], "*") == 0 && argc == 2) {
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
```

Sehingga dengan menjalankan ketiga pertintah tersebut, dapat diperoleh hasil sebagai berikut.
![image](https://user-images.githubusercontent.com/73484021/119262171-78f2c800-bc04-11eb-95f2-aa65250fd326.png)
![image](https://user-images.githubusercontent.com/73484021/119262189-8740e400-bc04-11eb-9de7-81dee93fbffb.png)

Adapun kendala yang dialami selama pengerjaan soal ini adalah sebagai berikut.
1. Apabila dipindahkan secara manual menjadi binary file, maka ada beberapa tipe file yang tidak dapat dibuka.

