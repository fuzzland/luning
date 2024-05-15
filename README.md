# Luning
Generates Foundry-style call graph for LLVM targets (C/C++).  

<img width="633" alt="Screenshot 2024-05-15 at 3 08 06 PM" src="https://github.com/fuzzland/luning/assets/10573715/0a410d72-e8fb-45fb-984c-156eeff187f6">

## Setup LLVM Environment

```
export ICWD=`pwd`
# Change paths based on your LLVM installation locations
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

CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD cmake ..
CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD make
```

PHP:
```
$CC -shared -fPIC hooks.c -o libhooks.so

git clone https://github.com/php/php-src
cd php-src

./buildconf

CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD ./configure
CXXFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" CFLAGS="-fpass-plugin=$ICWD/build/instrumentation/InstrPass.so" LDFLAGS="-L$ICWD -lhooks" LD_LIBRARY_PATH=$ICWD make -j100
```

### Run

PHP:
```
# dump calls
LD_LIBRARY_PATH=$ICWD ./sapi/cli/php -r 'echo "Hello World\n";' | python3 ../parser_1.py  > callgraph.txt
```
