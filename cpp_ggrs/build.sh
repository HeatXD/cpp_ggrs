# build dll
cargo build
# install cxx builder if needed
cargo install cxxbridge-cmd
# build files
cxxbridge src/lib.rs --header > out/cpp_ggrs.h
cxxbridge src/lib.rs > out/cpp_ggrs.cc