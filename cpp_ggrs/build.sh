# build dll
cargo build
# install cxx builder if needed
cargo install cxxbridge-cmd
# build files
cxxbridge src/lib.rs --include rust/cxx.h --header > out/cpp_ggrs.h
cxxbridge src/lib.rs --include rust/cxx.h > out/cpp_ggrs.cc