#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

struct header{
	uint32_t a; //4B
	uint16_t count; //2B
	uint16_t b; //2B
	uint64_t c; //8B
}__attribute__((packed));

struct affixPair{
	uint16_t start; 
	uint16_t num; 
}__attribute__((packed));


int main(const int argc,const char * argv[]){
	if(argc != 7){
		errx(1, "invalid input");
	}

	//open files ----------------------------------------
	int fd[6];
	for(int i = 0; i < 5; i++){
		if((fd[i] = open(argv[i+1],O_RDONLY)) == -1){
			err(2,"err opening %s",argv[i+1]);
		}
	}
	if((fd[5] = open(argv[6],O_CREAT | O_TRUNC | O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(2,"err opening %s",argv[5]);
	}
	
	//read affix and check size-----------------------	
	struct header h;
	if(read(fd[0],&h,sizeof(h)) != sizeof(h)){
		err(3,"err reading %s",argv[1]);
	}

	struct stat st;
	if(fstat(fd[0],&st)){
		err(4,"err geting stat of file");
	}
	if((st.st_size - sizeof(h)) % sizeof(struct affixPair) != 0 || (st.st_size - sizeof(h)) - sizeof(struct affixPair)*h.count != 0){
		errx(5,"file not right size");
	}
	

	//start writing last file------------------------
	struct affixPair p;
	uint64_t c;
	int size[4]={4,1,2,4} //post,pre,in,suff
	int rd;
	int i=0;
	while((rd = read(fd[0],&p,sizeof(p))) == sizeof(p)){
		if(i==4) i = 0;

		if(lseek(fd[i+1],p.start*size[i],SEEK_SET) == -1){ //lseek по елемент
			err(6,"err changing offset");
		}
		for(int j = 0; j < p.num;j++){
			if(read(fd[i+1],&c,size[i]) != size[i]){
				err(3,"err reading %s",argv[i+2]);
			}
			if(write(fd[5],&c,size[i]) != size[i]){ 
				err(4,"err writing");
			}
		}
	}
	if(rd != 0){
		err(3,"err reading %s",argv[1]);
	}

	//close files------------------------------------
	for(int i = 0; i < 6; i++){
		close(fd[i]);
	}
	exit(0);
}
