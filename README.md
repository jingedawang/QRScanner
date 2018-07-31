# QRScanner

This is a QR scanner demo powered by [zbar](http://zbar.sourceforge.net/) library.

Image preprocessing procedure is contributed by [sky_rudy](https://www.cnblogs.com/skyrudy/p/4955641.html).

### Usage

Install zbar library.
```
sudo apt install libzbar-dev
```

Compile and run.
```
mkdir build
cd build
cmake ..
make
./QRScanner
```

You may need to adjust the CMakeLists.txt if cmake or make error occurs.