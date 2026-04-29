add_library(f-hpp INTERFACE)
configure_target(f-hpp)
message(WARNING "f.hpp is just a fallback for gcc & clang, and will stop maintenance as soon as they support modules.")