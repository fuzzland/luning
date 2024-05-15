## Setup LLVM Environment

```
./env.sh
```

## Build Pass

First time build:
```
mkdir build
cd build
cmake ..
make
cd ..
```

Rebuild:
```
cd build
make
cd ..
```

## Compile with Pass

Simple way:
```
$CC -fpass-plugin=`build/instrumentation/InstrPass.so` test.c hooks.c -g3
```

CMake Project:
```
# Compile the hook to so
$CC -shared -fPIC hooks.c -o libhooks.so

export ICWD=/root/call-tracer
CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD cmake ..
CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD make
```

PHP:
```
$CC -shared -fPIC hooks.c -o libhooks.so
cd php-src

./buildconf

export ICWD=/root/call-tracer
CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD ./configure
CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD make -j100
```

### Run

PHP:
```
# dump calls
LD_LIBRARY_PATH=/root/call-tracer ./sapi/cli/php -r 'echo "Hello World\n";' > res.txt 

# generate callgraph
python3 ../parser_1.py  > callgraph.txt
```