#include "aes_xts.h"

void error_handler(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
	  EVP_CIPHER_CTX *ctx;

	  int len;

	  int ciphertext_len;

	  /* Create and initialise the context */
	  if(!(ctx = EVP_CIPHER_CTX_new()))
		 error_handler();

	  if(EVP_EncryptInit_ex(ctx, EVP_aes_256_xts(), NULL, key, iv) != 1)
	  	  error_handler();

	  if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len) != 1)
	    	error_handler();

	  ciphertext_len = len;

	  if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
		error_handler();

	  ciphertext_len += len;

	  EVP_CIPHER_CTX_free(ctx);

	  return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
	  EVP_CIPHER_CTX *ctx;

	  int len;

	  int plaintext_len;

	  if(!(ctx = EVP_CIPHER_CTX_new()))
		error_handler();

	  if(EVP_DecryptInit_ex(ctx, EVP_aes_256_xts(), NULL, key, iv) != 1)
	        error_handler();

	  if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1)
	    	error_handler();

	  plaintext_len = len;

	  if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
		 error_handler();

	  plaintext_len += len;

	  EVP_CIPHER_CTX_free(ctx);

	  return plaintext_len;
}
