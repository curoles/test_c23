# How To Build  GCC13

Untar downloaded file into directory `srcdir-gcc-13.1.0/`

```
mkdir objdir && cd objdir
cd ../srcdir-gcc-13.1.0/
./contrib/download_prerequisites 
cd ../objdir/
mkdir ../bindir
../srcdir-gcc-13.1.0/configure --prefix=`pwd`/../bindir --disable-multilib
make -j32
make install
```

gcc binary is located at `/home/igor/tool/gcc-13.1.0/bindir/bin/gcc`
