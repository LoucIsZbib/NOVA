all: libraries NOVA



################################################################################
# LIBRARIES
################################################################################

libraries: libraw


##################
# LIBRAW 
##################

libraw: lib/libraw.a

LIBRAW_FOLDER=lib/libraw

# OpenMP support
#LIBRAW_CFLAGS_OPENMP=-fopenmp

# Demosaic Pack GPL2:
#LIBRAW_CFLAGS_DP2=-I../LibRaw-demosaic-pack-GPL2
#LIBRAW_CFLAGS2=-DLIBRAW_DEMOSAIC_PACK_GPL2

# Demosaic Pack GPL3:
#LIBRAW_CFLAGS_DP3=-I../LibRaw-demosaic-pack-GPL3
#LIBRAW_CFLAGS3=-DLIBRAW_DEMOSAIC_PACK_GPL3

# Jasper support for RedCine
#LIBRAW_CFLAGS+=-DUSE_JASPER -I/usr/local/include
#LDADD+=-L/usr/local/lib -ljasper

# JPEG support for DNG
#LIBRAW_CFLAGS+=-DUSE_JPEG -I/usr/local/include
#LDADD+=-L/usr/local/lib -ljpeg

# LCMS support
#LCMS_DEF=-DUSE_LCMS -I/usr/local/include
#LCMS_LIB=-L/usr/local/lib -llcms

# LCMS2.x support
#LCMS_DEF=-DUSE_LCMS2 -I/usr/local/include
#LCMS_LIB=-L/usr/local/lib -llcms2

# Non thread-safe:
#LIBRAW_CFLAGS_THREADS=-DLIBRAW_NOTHREADS

# Thread Safe:
LIBRAW_CFLAGS_THREADS=

# Common flags
# WARNING: library order matters
LIBRAW_COMMON_LIBS=-lws2_32 -lm ${LCMS_LIB}
LIBRAW_CLIBS=-L./${LIBRAW_FOLDER}/lib  -lraw ${LIBRAW_COMMON_LIBS}
LIBRAW_CFLAGS=-O4 -I./${LIBRAW_FOLDER} -w -DLIBRAW_NODLL ${LIBRAW_CFLAGS_OPENMP} ${LIBRAW_CFLAGS_THREADS} ${LIBRAW_CFLAGS2} ${LIBRAW_CFLAGS3}

DCRAW_LIB_OBJECTS=${LIBRAW_FOLDER}/object/libraw_cxx.o ${LIBRAW_FOLDER}/object/libraw_datastream.o ${LIBRAW_FOLDER}/object/libraw_c_api.o ${LIBRAW_FOLDER}/object/dcraw_common.o ${LIBRAW_FOLDER}/object/dcraw_fileio.o ${LIBRAW_FOLDER}/object/demosaic_packs.o

# Samples

#bin/4channels: lib/libraw.a samples/4channels.cpp
#	g++ ${LCMS_DEF} ${LIBRAW_CFLAGS} -o bin/4channels samples/4channels.cpp ${LIBRAW_CLIBS}

#Libraries

${LIBRAW_FOLDER}/object/dcraw_common.o: ${LIBRAW_FOLDER}/internal/dcraw_common.cpp
	g++ -c ${LCMS_DEF} ${LIBRAW_CFLAGS} -o ${LIBRAW_FOLDER}/object/dcraw_common.o ${LIBRAW_FOLDER}/internal/dcraw_common.cpp

${LIBRAW_FOLDER}/object/demosaic_packs.o: ${LIBRAW_FOLDER}/internal/demosaic_packs.cpp
	g++ -c ${LCMS_DEF} ${LIBRAW_CFLAGS} ${LIBRAW_CFLAGS_DP2} ${LIBRAW_CFLAGS_DP3} -o ${LIBRAW_FOLDER}/object/demosaic_packs.o ${LIBRAW_FOLDER}/internal/demosaic_packs.cpp

${LIBRAW_FOLDER}/object/dcraw_fileio.o: ${LIBRAW_FOLDER}/internal/dcraw_fileio.cpp
	g++ -c ${LCMS_DEF} ${LIBRAW_CFLAGS} -o ${LIBRAW_FOLDER}/object/dcraw_fileio.o ${LIBRAW_FOLDER}/internal/dcraw_fileio.cpp

${LIBRAW_FOLDER}/object/libraw_cxx.o: ${LIBRAW_FOLDER}/src/libraw_cxx.cpp
	g++ -c ${LCMS_DEF} ${LIBRAW_CFLAGS} -o ${LIBRAW_FOLDER}/object/libraw_cxx.o ${LIBRAW_FOLDER}/src/libraw_cxx.cpp

${LIBRAW_FOLDER}/object/libraw_datastream.o: ${LIBRAW_FOLDER}/src/libraw_datastream.cpp
	g++ -c ${LCMS_DEF} ${LIBRAW_CFLAGS} -o ${LIBRAW_FOLDER}/object/libraw_datastream.o ${LIBRAW_FOLDER}/src/libraw_datastream.cpp

${LIBRAW_FOLDER}/object/libraw_c_api.o: ${LIBRAW_FOLDER}/src/libraw_c_api.cpp
	g++ -c ${LCMS_DEF} ${LIBRAW_CFLAGS} -o ${LIBRAW_FOLDER}/object/libraw_c_api.o ${LIBRAW_FOLDER}/src/libraw_c_api.cpp

#object/dcraw_half.o: ${LIBRAW_FOLDER}/samples/dcraw_half.c
#	gcc -c ${LCMS_DEF} ${LIBRAW_CFLAGS} -o ${LIBRAW_FOLDER}/object/dcraw_half.o ${LIBRAW_FOLDER}/samples/dcraw_half.c

lib/libraw.a: ${DCRAW_LIB_OBJECTS}
#	-del lib\\libraw.a
	rm -f ${LIBRAW_FOLDER}/lib/libraw.a
	ar crv ${LIBRAW_FOLDER}/lib/libraw.a ${DCRAW_LIB_OBJECTS}
	ranlib ${LIBRAW_FOLDER}/lib/libraw.a	


################################################################################
# NOVA
################################################################################

NOVA: ${LIBRAW_FOLDER}/lib/libraw.a src/4channels.cpp
	g++ ${LCMS_DEF} ${LIBRAW_CFLAGS} -o bin/4channels src/4channels.cpp ${LIBRAW_CLIBS}





################################################################################
# Clean	
################################################################################

# 18/04/2014 : seulement libraw
clean:
#-del bin\\*.dSYM
#-del lib\\lib*.a bin\\*.exe object\\*o 
	rm -f ${LIBRAW_FOLDER}/lib/lib*.a ${LIBRAW_FOLDER}/bin/*.exe ${LIBRAW_FOLDER}/object/*o 
	rm -f bin/*.exe
