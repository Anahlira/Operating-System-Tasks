#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct data{
	uint16_t off;
	uint8_t l;
	uint8_t non;
}__attribute__((packed));

int main(const int argc, const char * argv[]){
	if(argc != 4){
		errx(1,"wrong input");
	}

	//open first two files
	int fd1,fi1;
	fd1=open(argv[1],O_RDONLY);
	if(fd1 ==-1){
		err(2,"opening file");
	}
	fi1=open(argv[2],O_RDONLY);
        if(fi1 == -1){
                err(2,"opening file");
        }
	//open files 3 and 4
	int fd2,fi2;
        fd2=open(argv[3],O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
        if(fd2 ==-1){
                err(2,"opening file");
        }
        fi2=open(argv[4],O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
        if(fi2 == -1){
                err(2,"opening file");
        }

	int rf;
	struct data d;
	struct data nd;
	while((rf = read(fd1,&d,sizeof(d))) == sizeof(d)){
		if(lseek(fd1, d.off, SEEK_SET) == -1){
			err(4,"err lseek file");
		}

		uint8_t letter;
		if(read(fd1,&letter,sizeof(letter)) != sizeof(letter)){
			err(5,"err reading file");
		}
		if(letter < 0x41 || letter > 0x5A){
			continue;
		}

		if(lseek(fd1, d.off, SEEK_SET) == -1){
                        err(4,"err lseek file");
                }
		
		nd = d;
		int offset;
		if((offset = lseek(fi2,0,SEEK_END)) == -1){
			err(4, "err lseek file");
		}
		nd.off = offset;
		
		if(write(fi2,&nd,sizeof(nd)) != sizeof(nd)){
			err(6, "err in writing");
		}
		
		uint8_t * buf = malloc(d.l);
		
		for(uint8_t i=0; i<d.l;i++){
			if(read(fd1,&buf[i],sizeof(uint8_t)) != sizeof(uint8_t)){
				err(6,"err in reading");
			}
		}

		for(uint8_t i=0; i<d.l;i++){
                        if(write(fd2,&buf[i],sizeof(uint8_t)) != sizeof(uint8_t)){
                                err(6,"err in writing");
                        }
                }

		free(buf);
		
	}
	if(rf != 0){
		err(3,"reading file");
	}

	close(fi1);
	close(fd1);
	close(fi2);
        close(fd2);
	exit(0);
}
