#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

struct header{
	uint32_t magic;
	uint32_t count;
}__attribute__((packed));

struct headerC{
	uint32_t magic1;
	uint16_t magic2;
	uint16_t reserved;
	uint64_t count;
}__attribute__((packed));

struct data{
	uint16_t type;
	uint16_t r[3];
	uint32_t offset1;
	uint32_t offset2;
}__attribute__((packed));

int main(const int argc,const char * argv[]){
	if(argc != 3){
		errx(1,"invalid input");
	}
	
	//open files--------------------------------------------
	int fdd,fdc; // data - compator
	if((fdc = open(argv[2],O_RDONLY)) == -1){
		err(2,"err opening file");
	}
	if((fdd = open(argv[1],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(2,"err opening file");
	}

	//read header------------------------------------------
	struct header h;
	struct headerC hc;

	if(read(fdc,&h,sizeof(h)) != sizeof(h)) err(3,"err reading file");
	if(read(fdd,&hc,sizeof(hc)) != sizeof(hc)) err(3,"err reading file");
	
	if(h.magic != 0x21796F4A) errx(4,"wrong format in %s", argv[1]);
	if(hc.magic1 != 0xAFBC7A37 || hc.magic2 != 0x1C27) errx(4,"wrong format in %s", argv[2]);

	//check file sizes-------------------------------------
	
	//read-------------------------------------------------
	struct data d;
	uint64_t el,A,B; // elements of data size
	int rd;
	uint16_t less = 1;
	while((rd=read(fdc,&d,sizeof(d))) == sizeof(d)){
		if(d.type == 0) less = 1;
		else if(d.type == 1) less = 0;
		else errx(5,"wrong format of %s",argv[2]);
		
		//check and if all d.r = 0	

		//in elements = without header
		if(lseek(fdd,(d.offset1)*sizeof(el) + sizeof(h),SEEK_SET) == -1) err(6,"err lseek");
		if(read(fdd,&A,sizeof(A)) != sizeof(A)) err(3,"err reading file");
		if(lseek(fdd,(d.offset2)*sizeof(el) + sizeof(h),SEEK_SET) == -1) err(6,"err lseek");
		if(read(fdd,&B,sizeof(B)) != sizeof(B)) err(3,"err reading file");
	
		if(A == B) continue;
		else if( (A < B) != less){
			if(lseek(fdd,(d.offset1)*sizeof(el) + sizeof(h),SEEK_SET) == -1) err(6,"err lseek");
			if(write(fdd,&B,sizeof(B)) != sizeof(B)) err(3,"err reading file");
			if(lseek(fdd,(d.offset2)*sizeof(el) + sizeof(h),SEEK_SET) == -1) err(6,"err lseek");
			if(read(fdd,&A,sizeof(A)) != sizeof(A)) err(3,"err reading file");	
		}
		//2 < 3 1 ; type = 0 - нищо не прави
		//3 < 2 1 ; type = 0 - прави
		//2 < 3 0 -> 0 1 - прави
	}
	if(rd != 0) err(3,"err reading file");

	//close files------------------------------------------
	close(fdc);
	close(fdd);
	exit(0);
}
