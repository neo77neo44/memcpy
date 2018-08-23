#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#define max_function_rec 100
typedef unsigned long vc_uint32;

typedef struct memcpy_data_size{
	int a[16];
}DATA_SIZE, *P_DATA_SIZE;

typedef struct function_rec{
	void *function;
	char type;	
}function_rec;
function_rec gFrec[max_function_rec];
vc_uint32 rec_idx=0;
#define DUMP(type, func, call) \
	printf("%s: func = %p, called by = %p\n", type, func, call)

void __attribute__((__no_instrument_function__))
__cyg_profile_func_enter(void *this_func, void *call_site)
{
    DUMP("entry",this_func, call_site);
}
void __attribute__((__no_instrument_function__))
__cyg_profile_func_exit(void *this_func, void *call_site)
{
    DUMP("exit",this_func, call_site);
}

void function_entry(void*function){
	if(rec_idx==max_function_rec){
		rec_idx = 0;
	}
	gFrec[rec_idx].function = function;
	gFrec[rec_idx].type = 0;
	rec_idx++;
}
void function_exit(void*function){
	if(rec_idx==max_function_rec){
		rec_idx = 0;
	}
	gFrec[rec_idx].function = function;
	gFrec[rec_idx].type = 1;
	rec_idx++;
}
void *libc_memcpy (void *dest, const void *src, size_t len)
{
	function_entry(libc_memcpy);
	char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	function_exit(libc_memcpy);
	return dest;
}
void *mymemcpy(void *to, const void *from, size_t size)
{
	function_entry(mymemcpy);
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
	function_exit(mymemcpy);
	return to;
}
void libc(unsigned long len)
{	
    struct timeval start, end;
	unsigned long diff;
    char *dst;
    char *src;
	function_entry(libc);	
    vc_uint32 i;
    gettimeofday(&start, NULL);
    dst = malloc(sizeof(char)*len);
    src = malloc(sizeof(char)*len);
    memset(src,0xA3,sizeof(char)*len);
    libc_memcpy(dst,src,sizeof(char)*len);
    gettimeofday(&end, NULL);
	diff = 1000000*(end.tv_sec - start.tv_sec)+ end.tv_usec - start.tv_usec;
    printf("%-10d, %ld\r\n",len,diff);
    free(dst);
	free(src);
	function_exit(libc);
	return 0;
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
    printf("%-10d, %ld\r\n",len,diff);
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
    printf("%-10d, %ld\r\n",len,diff);
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
    printf("%-10d, %ld\r\n",len,diff);
    free(dst);
	free(src);
	return 0;
}
#define START_NUM 1
#define STOP_NUM 28

int main()
{
    vc_uint32 wLen = 0;
    vc_uint32 i;
	
    for(i=START_NUM;i<=STOP_NUM;i++){
        libc(2<<i);
    }
	printf("------------------------------------------\r\n");
	for(i=START_NUM;i<=STOP_NUM;i++){
        cpy(2<<i);
    }
	printf("------------------------------------------\r\n");
    for(i=START_NUM;i<=STOP_NUM;i++){
        mycpy(2<<i);
    }
	printf("------------------------------------------\r\n");
    for(i=START_NUM;i<=STOP_NUM;i++){
        ncpy(2<<i);
    }
	printf("------------------------------------------\r\n");
	for(i=0;i<max_function_rec;i++){
		printf("0x%X,%d\r\n",gFrec[i].function,gFrec[i].type);
		
	}
	
    return 0;
}
