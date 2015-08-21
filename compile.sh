

mkdir build 
cd build
mkdir debug release

echo "build to build/debug directory.."
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make

cd ..

echo "build to build/release directory.."
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make

