Prototype to test librbd async io behavior with aes_xts_256 encryption algo from openssl lib
librdb async io is what qemu block rbd layer uses

to run:

1.cp input_file /tmp/

2. make

3. ./encrypt

4. sudo md5sum /tmp/input_file /tmp/decrypted_file /tmp/encrypted_file

1st and 2nd files checksum should match, while 3rd file should be different
