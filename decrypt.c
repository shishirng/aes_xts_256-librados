#include <rbd/librbd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include "aes_xts.h"

int main()
{
     	int ret,len = 0;
	rados_t cluster;
    	char *buf, *out_buf = NULL;
	char *pool = "rbd";
    	char *rbd_name = "vol1";
	int fd = -1;
	int i = 0;
    	rados_completion_t read_comp, write_comp;
    	rbd_image_t rbd_image;
	time_t start_t, end_t, total_t = 0;
        unsigned char *cipher_key = (unsigned char *) "7190c8bc27ac4a1bbe1ab1cf55cf3b097190c8bc27ac4a1bbe1ab1cf55cf3b09";
        unsigned char *iv = (unsigned char *)         "dcbfdd41e40f74a2";

	ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();

	ret = rados_create(&cluster, NULL);
	if (ret) {
		printf("error creating rados_t object\n");
		return -1;
	}

	ret = rados_conf_read_file(cluster,"/home/shishir/repos/ceph/src/ceph.conf");
	if (ret) {
		printf("Error reading conf files \n");
		return -1;
	}

	ret = rados_connect(cluster);
	if (ret) {
		printf("Could not connect to cluster\n");
		return -1;
	}

	rados_ioctx_t io;

	ret = rados_ioctx_create(cluster, pool, &io);
	if (ret) {
		printf("Could not open connection to cluster\n");
		return -1;
	}

	ret = rbd_open(io, rbd_name, &rbd_image, NULL);
	if (ret) {
		printf("Failed to open image %s\n",rbd_name);
		return -1;
	}

	posix_memalign((void**)&buf, 4096, 65536);
	if (!buf) {
		printf("Failed to malloc\n");
		return -1;
	}
	posix_memalign((void**)&out_buf, 4096, 65536);
	if (!out_buf) {
		printf("Failed to malloc\n");
		return -1;
	}
	ret = rbd_aio_create_completion(NULL, NULL, &read_comp);
	if (ret) {
		printf ("Failed to create aio completion\n");
	return -1;
	}

	memset(buf, 0, 65536);
	memset(out_buf, 0, 65536);
	ret = rbd_aio_read(rbd_image, 0, 65536, buf, read_comp);
	if (ret < 0) {
		printf ("Failed to read aio completion\n");
		return -1;
	}


	rbd_aio_wait_for_complete(read_comp);
	rbd_aio_release(read_comp);
	len = 65536;
	len = decrypt(buf, len, cipher_key, iv, out_buf);

	fd = open("/tmp/decrypted_file_1", O_CREAT|O_RDWR);
	if (fd > 0) {
		write(fd, out_buf, 65536);
		close(fd);
	}

	rados_ioctx_destroy(io);
	rados_shutdown(cluster);
	free(buf);
	free(out_buf);
	return 0;
	}
