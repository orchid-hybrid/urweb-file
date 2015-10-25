# urweb-file
save a file to the disk

To compile, set the file path in upload.ur first so that you don't clobber an existing file. Then:

```
gcc -I/usr/local/include/urweb -c file.c
urweb upload -dbms sqlite
./upload -p 8080
```
