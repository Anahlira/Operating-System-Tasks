#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

//питай мони за проверките за размера на файла, че е правил други работи от мен

struct header{
	uint32_t magic;
	uint8_t hv;
	uint8_t dv;
	uint16_t count;
	uint32_t r1;
	uint32_t r2;
}__attribute__((packed));

struct data1{
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

struct data2{
	uint32_t offset;
	uint16_t original;
	uint16_t new;
}__attribute__((packed));

int main(const int argc, const char * argv[]){
	if(argc != 4){
		errx(1, "invalid number of arguments");
	}

	int fdp,fd1,fd2;
	if((fdp = open(argv[1],O_RDONLY)) == -1){
		err(2, "err opening file %s", argv[1]);
	}
	if((fd1 = open(argv[2],O_RDONLY)) == -1){
		err(2, "err opening file %s", argv[2]);
	}
	if((fd2 = open(argv[3],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(2, "err opening file %s", argv[3]);
	}

	struct header h;
	struct data1 d1;
	struct data2 d2;
	uint16_t num;
	int dv = 0;
	
	//read header----------------------------------------------------------
	struct stat st;
	if(fstat(fdp, &st) == -1){
		err(3, "can't get stat of %s", argv[1]);
	}
	
	if(read(fdp, &h, sizeof(h)) != sizeof(h)) {
		err(5, "err reading %s", argv[1]);
	}

	if(h.magic != 0xEFBEADDE || h.hv != 0x01){
		errx(4, "file %s with wrong header format" , argv[1]);
	}
	if(h.dv != 0x00 && h.dv != 0x01){
		errx(4, "file %s with wrong data version", argv[1]);
	}
	num=h.count;
	if(h.dv == 0x01) dv=1;
	
	int dataSize = st.st_size - sizeof(h);

	//check file size -----------------------------------------------------
	if(fstat(fd1,&st) == -1){
		err(3, "can't get stat of %s", argv[2]);
	}
	if (dv == 0){
		if(st.st_size % sizeof(d1) != 0 || (st.st_size / sizeof(d1) != num)){
			errx(4, "wrong size of %s", argv[2]);
		}
		if(dataSize % sizeof(d1) != 0 || dataSize / sizeof(d1)*num != 1){
			errx(4, "wrong data size of %s", argv[1]);
		}
	}
	else{
		if(st.st_size % sizeof(d2) != 0 || (st.st_size / sizeof(d2) != num)){
			errx(4, "wrong size of %s", argv[2]);
		}
		if(dataSize % sizeof(d2) != 0 || dataSize / sizeof(d2)*num != 1){
			errx(4, "wrong data size of %s", argv[1]);
		}

	}

	//copy file1 to file2 -----------------------------------------------
	
	int rf;
	char c;
	while((rf = read(fd1,&c,sizeof(c))) == sizeof(c)){
		if(write(fd2,&c,sizeof(c)) != sizeof(c)){
			err(6, "err writing in %s", argv[3]);
		}
	}
	if(rf != 0) err(5, "err reading %s", argv[2]);
	

	//change file --------------------------------------------------------
		
	if(dv == 0){
		uint8_t buf;
		while((rf=read(fdp,&d1,sizeof(d1))) == sizeof(d1)){
			if(lseek(fd2,d1.offset,SEEK_SET) == -1){
				err(7,"err changing pointer in %s", argv[3]);
			}

			if(read(fd2,&buf,sizeof(buf)) != sizeof(buf)){
				err(5, "err reading %s", argv[3]);
			}
			if(buf != d1.original){
				errx(8, "Difrence between file and original");
			}
			
			if(lseek(fd2,d1.offset,SEEK_SET) == -1){
				err(7,"err changing pointer in %s", argv[3]);
			}
			if(write(fd2,&buf,sizeof(buf)) != sizeof(buf)){
				err(6,"err writing in %s",argv[3]);
			}
		}
		if(rf != 0) err(5, "err reading %s", argv[1]);
	}
	else{ //copy of the other
		uint16_t buf;
		while((rf=read(fdp,&d2,sizeof(d2))) == sizeof(d2)){
			if(lseek(fd2,d2.offset*(sizeof(uint16_t)),SEEK_SET) == -1){
				err(7,"err changing pointer in %s", argv[3]);
			}

			if(read(fd2,&buf,sizeof(buf)) != sizeof(buf)){
				err(5, "err reading %s", argv[3]);
			}
			if(buf != d2.original){
				errx(8, "Difrence between file and original");
			}
			
			if(lseek(fd2,d2.offset*(sizeof(uint16_t)),SEEK_SET) == -1){
				err(7,"err changing pointer in %s", argv[3]);
			}
			if(write(fd2,&buf,sizeof(buf)) != sizeof(buf)){
				err(6,"err writing in %s",argv[3]);
			}
		}
		if(rf != 0) err(5, "err reading %s", argv[1]);
	}

	
	close(fdp);
	close(fd1);
	close(fd2);
	exit(0);
}
