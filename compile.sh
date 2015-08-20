

mkdir -p build 
cd build
mkdir -p debug release

echo '----------------------------------------------'
echo '-----debug------------------------------------'
echo '----------------------------------------------'
echo "build to build/debug directory.."
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
make test

cd ..

echo ''
echo ''

echo '----------------------------------------------'
echo '-----release----------------------------------'
echo '----------------------------------------------'
echo "build to build/release directory.."
cd release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
make test
