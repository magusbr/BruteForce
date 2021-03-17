#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "sha256.h"
#include "md5.h"

static const char VCHARS[] = "0123456789ABCDEFGHIJLMNOPQRSTUVZ";
char try[1024];
char try_pos[1023] = {-1};
int try_size = 0;
const char* known = "VL7A";
BYTE objective[SHA256_BLOCK_SIZE] = {0x6f,0xc7,0x28,0x86,0x4b,0x65,0x07,0x2b,0xdc,0xf0,0xc0,0x9d,0x89,0xeb,0x3f,0xa8,0xe9,0x4a,0x82,0x96,0x4f,0x28,0x18,0xdb,0xd8,0x73,0x43,0xa9,0x52,0xfe,0x91,0x61};
const int MAX_TRY_SIZE = 6;
#define DEBUG 1


char conv_char1(BYTE val)
{
	BYTE first = ((val & 0xF0) >> 4);
	if (first < 10)
		return first + '0';
	else
		return first - 10 + 'a';
}

char conv_char2(BYTE val)
{
	BYTE sec = (val & 0x0F);
	if (sec < 10)
		return sec + '0';
	else
		return sec - 10 + 'a';
}

void sha256_to_char(char* out, char* in)
{
	int i, j = 0;
	
	for(i = 0; i < SHA256_BLOCK_SIZE; i++)
	{
		out[j++] = conv_char1(in[i]);
		out[j++] = conv_char2(in[i]);
	}
	
	out[j] = '\0';
}

void md5_to_char(char* out, char* in)
{
	int i, j = 0;
	
	for(i = 0; i < MD5_BLOCK_SIZE; i++)
	{
		out[j++] = conv_char1(in[i]);
		out[j++] = conv_char2(in[i]);
	}
	
	out[j] = '\0';
}

void sha256_hash(char* sha_hash, char* text)
{
	BYTE md5_hash[MD5_BLOCK_SIZE];
	BYTE md5_hash_ascii[MD5_BLOCK_SIZE*2 + 1];
	SHA256_CTX ctx_sha;
	MD5_CTX ctx_md5;
	int pass = 1;
	
	md5_init(&ctx_md5);
	md5_update(&ctx_md5, text, strlen(text));
	md5_final(&ctx_md5, md5_hash);
	
	md5_to_char(md5_hash_ascii, md5_hash);

	sha256_init(&ctx_sha);
	sha256_update(&ctx_sha, md5_hash_ascii, MD5_BLOCK_SIZE*2);
	sha256_final(&ctx_sha, sha_hash);
}

void increase_pos(int pos) {
	if (pos < 0) return;
	
	if (try_pos[pos] == (strlen(VCHARS)-1)) {
		if (pos == 0) { // if it is the last position, increase the try size and reset all
			try_size++;
			for (pos = 0; pos < try_size + 1; pos++) {
				try_pos[pos] = 0;
			}
		} else { // zero current pos and increase the position before
			try_pos[pos] = 0;
			increase_pos(pos - 1);
		}
	} else {
		try_pos[pos]++;
	}
}

int generate_string()
{
	int pos;
	
	increase_pos(try_size);
	if (try_size == MAX_TRY_SIZE) {
		printf("FIM\n");
		exit(0);
	}
	
	for (pos = 0; pos < try_size + 1; pos++) {
		try[pos] = VCHARS[try_pos[pos]];
	}
	try[pos+1] = '\0';
}

int sha256_test(char* text)
{
	BYTE text1[1024];
	BYTE sha_hash_res[SHA256_BLOCK_SIZE];
	BYTE sha256_hash_ascii[SHA256_BLOCK_SIZE*2 + 1];
	int pass = 1;
	
	//sprintf(text1, "%s%s", text, known);
	sprintf(text1, "%s", text);
#ifdef DEBUG
	printf("%s ", text1);
#endif
	
	sha256_hash(sha_hash_res, text1);
	
	sha256_to_char(sha256_hash_ascii, sha_hash_res);
#ifdef DEBUG
	printf("%s\n", sha256_hash_ascii);
#endif

	pass = pass && !memcmp(objective, sha_hash_res, SHA256_BLOCK_SIZE);

	return(pass);
}

int main()
{
	int i;
	
	/*while(1) {
		generate_string();
		if (i++ % 100000 == 0) {
			printf("...\n%s\n", try);
		}
		if (sha256_test(try)) {
			printf("ACHOU: %s\n", try);
			exit(0);
		}
	}*/
	
	printf("SHA-256 tests: %s\n", sha256_test("5B574TVL7A") ? "SUCCEEDED" : "FAILED");

	return(0);
}