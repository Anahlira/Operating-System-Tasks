#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

struct elements{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

int main(const int argc, const char * argv[]){
	if(argc != 3){
		errx(1,"wrong input");
	}

	//file1,file2,patch
	int fd1,fd2,fd3;
	//Open, create files
	if((fd1 = open(argv[1],O_RDONLY)) == -1){
		err(2,"err opening file");
	}
	if((fd2 = open(argv[2],O_RDONLY)) == -1){
                err(2,"err opening file");
        }
	if((fd3 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
                err(2,"err opening file");
        }
	
	struct stat st1;
	struct stat st2;
	if(fstat(fd1,&st1) == -1){
		err(3,"err getting size");
	}
	if(fstat(fd2,&st2) == -1){
		err(3,"err getting size");
	}

	if(st1.st_size != st2.st_size){
		errx(4,"files are not the same size");
	}

	uint8_t A,B;
	int rf;
	while((rf=read(fd1,&A,sizeof(A)) == sizeof(A))){
		if(read(fd2,&B,sizeof(B) != sizeof(B))){
			err(5,"err while reading");
		}
		if (A == B){
			continue;
		}
		
		struct elements e;
		int off;
		if((off = lseek(fd1, 0, SEEK_CUR)) == -1){
			err(6,"err lseek");
		}
		e.offset = off;
		e.original = A;
		e.new = B;

		if(write(fd3,&e,sizeof(e)) != sizeof(e)){
			err(7,"err writing");
		}

	}
	if(rf != 0){
		err(5,"err while reading");
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
