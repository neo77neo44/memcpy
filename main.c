#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

typedef unsigned long vc_uint32;

typedef struct memcpy_data_size{
	int a[16];
}DATA_SIZE, *P_DATA_SIZE;
void *mymemcpy(void *to, const void *from, size_t size)
{
	P_DATA_SIZE dst = (P_DATA_SIZE)to;
	P_DATA_SIZE src = (P_DATA_SIZE)from;
	int new_len  = size/sizeof(DATA_SIZE)-1;
	int remain  = size%sizeof(DATA_SIZE)-1;
	while (new_len >= 1){
		*dst++ = *src++;
		new_len--;
	}
	while (remain >= 0){
		*((char *)dst + remain) = *((char *)src + remain);
		remain--;
	}
	return to;
}

void cpy(unsigned long len)
{
    struct timeval start, end;
	unsigned long diff;
    char *dst;
    char *src;
    vc_uint32 i;
    gettimeofday(&start, NULL);
    dst = malloc(sizeof(char)*len);
    src = malloc(sizeof(char)*len);
    memset(src,0xA3,sizeof(char)*len);
    memcpy(dst,src,sizeof(char)*len);
    gettimeofday(&end, NULL);
	diff = 1000000*(end.tv_sec - start.tv_sec)+ end.tv_usec - start.tv_usec;
    if(diff >= 1000){
        printf("%d, %ld\n",len,diff);
	}else{
        printf("%d, %ld\r\n",len,diff);
    }
    free(dst);
	free(src);
	return 0;
}
void mycpy(unsigned long len)
{
    struct timeval start, end;
	unsigned long diff;
    char *dst;
    char *src;
    vc_uint32 i;

    gettimeofday(&start, NULL);
    dst = malloc(sizeof(char)*len);
    src = malloc(sizeof(char)*len);
    memset(src,0xA0,sizeof(char)*len);
    mymemcpy(dst,src,sizeof(char)*len);
    gettimeofday(&end, NULL);
	diff = 1000000*(end.tv_sec - start.tv_sec)+ end.tv_usec - start.tv_usec;
    printf("%d, %ld\r\n",len,diff);
    free(dst);
	free(src);
	return 0;
}
void ncpy(unsigned long len)
{
    struct timeval start, end;
	unsigned long diff;
    char *dst;
    char *src;
    vc_uint32 i;

    gettimeofday(&start, NULL);
    dst = malloc(sizeof(char)*len);
    src = malloc(sizeof(char)*len);
    memset(src,0xA0,sizeof(char)*len);
    for(i=0;i<len;i++){
        dst[i]=src[i];
    }
    gettimeofday(&end, NULL);
	diff = 1000000*(end.tv_sec - start.tv_sec)+ end.tv_usec - start.tv_usec;
    printf("%d, %ld\r\n",len,diff);
    free(dst);
	free(src);
	return 0;
}
int main()
{
    vc_uint32 wLen = 0;
    vc_uint32 i;
    for(i=0;i<=28;i++){
        cpy(2<<i);
    }
    for(i=0;i<=28;i++){
        mycpy(2<<i);
    }
    for(i=0;i<=28;i++){
        ncpy(2<<i);
    }
    return 0;
}
