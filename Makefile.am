OPENCL_INCLUDES= -I"$(AMDAPPSDKROOT)//include"

if WANT_JANSSON
JANSSON_INCLUDES= -I$(top_srcdir)/compat/jansson
else
JANSSON_INCLUDES=
endif

EXTRA_DIST	= dcrypt.cl example-cfg.json

SUBDIRS		= compat

INCLUDES	= $(PTHREAD_FLAGS) -fno-strict-aliasing $(JANSSON_INCLUDES) $(OPENCL_INCLUDES) 

bin_PROGRAMS	= minerd

minerd_SOURCES	= elist.h \
		  miner.h \
		  compat.h \
		  gpu.h \
                  dcrypt.h \
                  dcrypt_sha256.h \
		  gpu.c \
		  cpu-miner.c \
		  util.c \
                  dcrypt.c \
                  dcrypt_sha256.c \
                  sha2.c

minerd_LDFLAGS	= $(PTHREAD_FLAGS) 
minerd_LDADD	= @LIBCURL@ @JANSSON_LIBS@ @PTHREAD_LIBS@ @WS2_LIBS@ @OPENSSL_LIBS@ @MATH_LIBS@ -lOpenCL 
minerd_CPPFLAGS = @LIBCURL_CPPFLAGS@
minerd_CFLAGS   = -std=gnu11 -O3
#-march=native 
