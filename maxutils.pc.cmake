prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: maxutils
Description: urgen library 
Version: 
Requires: 
Cflags:  -I${includedir} 
Libs:  -L${libdir} -lmaxutils 
