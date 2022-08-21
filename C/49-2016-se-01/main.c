#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

static int comp(const void *p1, const void *p2){
	return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int main (const int argc, const char * argv[]){
	if(argc != 2){
		errx(1,"invalid input");
	}
	
	int fd=open(argv[1],O_RDWR);
	if (fd == -1){
		err(2, "err opening file");
	}
	
	struct stat statfd;
	if(fstat(fd,&statfd) == -1){
		err(3,"err getting file stat");
	}
	int size = statfd.st_size;

	char* buf = malloc(size);

	if(buf == NULL){
		err(4,"err alocating memmory");
	}

	if(read(fd,buf,size) != size){
		err(5,"err reading file");
	}

	qsort(&buf[0],size,1,comp);
	if (lseek(fd,0,SEEK_SET) == -1){
		err(6,"changing the pointer");
	}

	if(write(fd,&buf,size) != size){
		err(7,"writing in file");
	}

	free(buf);

	close(fd);
	exit(0);
}
