#include "aes_xts.h"

void error_handler(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

int _encrypt(EVP_CIPHER_CTX *ctx,unsigned char *ciphertext, unsigned char *plaintext, int plaintext_len)
{
	  int len = 0;
	  int ciphertext_len = 0;
	  if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
	    	error_handler();

	  ciphertext_len = len;

	  if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
		error_handler();

	  return ciphertext_len += len;


}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
	  EVP_CIPHER_CTX *ctx;

	printf ("encryption: len %d, input %p, output %p\n",plaintext_len,ciphertext, plaintext);
	  int len;

	  int offset = 0;

	  /* Create and initialise the context */
	  if(!(ctx = EVP_CIPHER_CTX_new()))
		 error_handler();

	  if(EVP_EncryptInit_ex(ctx, EVP_aes_256_xts(), NULL, key, iv) != 1)
	  	  error_handler();

	  while (offset < plaintext_len) { 
	  	len = _encrypt(ctx, ciphertext + offset , plaintext + offset, 512);
		printf ("encryption: offset %d, input %p, output %p, len %d\n",offset, ciphertext + offset, plaintext + offset, len);
		offset += len;
	  }
 
	  EVP_CIPHER_CTX_free(ctx);

	  return offset;
}

int _decrypt(EVP_CIPHER_CTX *ctx, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext)
{
	int len = 0;
	int plaintext_len = 0;

	if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
	    	error_handler();

	  plaintext_len = len;

	  if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
		 error_handler();

	  return plaintext_len += len;


}
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
	printf ("decryption: len %d, input %p, output %p\n",ciphertext_len,ciphertext, plaintext);
	  EVP_CIPHER_CTX *ctx;

	  int len = 0;

	  int offset = 0;

	  if(!(ctx = EVP_CIPHER_CTX_new()))
		error_handler();

	  if(EVP_DecryptInit_ex(ctx, EVP_aes_256_xts(), NULL, key, iv) != 1)
	        error_handler();

	while (offset < ciphertext_len) {
		len = _decrypt(ctx, ciphertext + offset, 512, plaintext + offset );
		printf ("decryption: offset %d, input %p, output %p, len %d\n",offset, ciphertext + offset, plaintext + offset, len);
		offset += len;
	}
	EVP_CIPHER_CTX_free(ctx);

	  return offset;
}
