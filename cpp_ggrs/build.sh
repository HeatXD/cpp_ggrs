# build 
cargo build --release
# copy dll to example
cp -fr target/release/cpp_ggrs.dll example/lib
cp -fr target/release/cpp_ggrs.dll example/build
# install cxx builder if needed
cargo install cxxbridge-cmd
# build files
cxxbridge src/lib.rs --include rust/cxx.h --header > out/cpp_ggrs.h
cxxbridge src/lib.rs --include rust/cxx.h > out/cpp_ggrs.cc

