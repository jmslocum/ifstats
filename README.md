# libifstats
This is a simple linux library for providing C level programming access to the
/proc/net/dev file. All major Linux distributions use procfs and report system
details through various dynamically generated files. This library will read the
network stats reported by the kernel for all devices on the system and build
a linked list of data structures. 

## Installation
To install libifstats on your system just clone the git repo, and run the 
gnu autoreconf program on the project. This will generate the configure
scripts. Then you install it like any other program.

```bash
git clone git://github.com/jmslocum/ifstats.git
cd ifstats
autoreconf --install
./configure
make
sudo make install
make clean
```

Installing from a dist tarball is simpler and does not require the libtools suite to
be install. you can just unpack the tar.gz file and do the "usual" install.

```bash
tar -zxvf ifstats-1.0.0.tar.gz
cd ifstats-1.0.0
./configure
make 
sudo make install
make clean
```

The library is called libifstats.a, and will be installed to /usr/local/lib. The header file will be installed to /usr/local/include. and the man page will be installed to /usr/share/man/man3. 

## Documentation
To view the documentation you can run any of the following commands
```bash
man ifstats
man getIfStats
man releaseStats
```

## Testing
The library was tested on Ubuntu 12.04 and Fedora 17. It was also run through Valgrind
and no memory leaks or errors were reported. 

## License
This software is avilable under the MIT License.

