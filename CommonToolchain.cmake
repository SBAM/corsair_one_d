string(CONCAT CMAKE_CXX_FLAGS
  "-Wall -Wextra -Werror "
  "-Wconversion "
  "-Wdisabled-optimization "
  "-Winline "
  "-Wshadow ")

string(CONCAT CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")

string(CONCAT CMAKE_CXX_FLAGS_RELEASE "-ggdb1 -O3 -DNDEBUG")
