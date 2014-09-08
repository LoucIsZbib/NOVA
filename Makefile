all: nova

CTUNING=-mtune=native -pipe
OS=unix
#OS=windows

##################
# RESSOURCES
##################

LIBRAW_FOLDER=res/libraw
ifeq ($(OS),unix)
	LIBRAW_CLIBS=-L./${LIBRAW_FOLDER}/libUnix  -lraw_r -lm
else
	LIBRAW_CLIBS=-L./${LIBRAW_FOLDER}/libWin  -lraw -lws2_32 -lm
endif
LIBRAW_CFLAGS=-I./${LIBRAW_FOLDER}/include -DLIBRAW_NODLL

OPENCV_FOLDER=res/opencv
ifeq ($(OS),unix)
	OPENCV_CLIBS=-L./${OPENCV_FOLDER}/libUnix -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_reg
else
	OPENCV_CLIBS=-L./${OPENCV_FOLDER}/libWin -lopencv_core300.dll -lopencv_highgui300.dll -lopencv_imgproc300.dll -lopencv_reg300.dll
endif
OPENCV_CFLAGS=-I./${OPENCV_FOLDER}/include

NOVA_CFLAGS=-I./include
NOVA_OBJECTS=bin/objets/rawimage.o bin/objets/ui.o

################################################################################
# TESTS
################################################################################

tests: test_libraw test_opencv_highgui test_opencv_reg test_librawETopencv


test_libraw: src/tests/test_libraw.cpp
	g++ ${CTUNING} ${LIBRAW_CFLAGS} -o bin/test_libraw src/tests/test_libraw.cpp ${LIBRAW_CLIBS}

test_opencv_highgui: src/tests/test_opencv_highgui.cpp
	g++ ${CTUNING} ${OPENCV_CFLAGS} -o bin/test_opencv_highgui src/tests/test_opencv_highgui.cpp ${OPENCV_CLIBS}

test_opencv_reg: src/tests/test_opencv_reg.cpp
	g++ ${CTUNING} ${OPENCV_CFLAGS} -o bin/test_opencv_reg src/tests/test_opencv_reg.cpp ${OPENCV_CLIBS}
    
test_librawETopencv: src/tests/test_librawETopencv.cpp
	g++ ${CTUNING} ${OPENCV_CFLAGS} ${LIBRAW_CFLAGS} -o bin/test_librawETopencv src/tests/test_librawETopencv.cpp ${OPENCV_CLIBS} ${LIBRAW_CLIBS}

################################################################################
# NOVA
################################################################################
nova: src/nova.cpp rawimage.o ui.o
	g++ ${CTUNING} ${OPENCV_CFLAGS} ${LIBRAW_CFLAGS} ${NOVA_CFLAGS} -o bin/nova ${NOVA_OBJECTS} src/nova.cpp ${OPENCV_CLIBS} ${LIBRAW_CLIBS}

rawimage.o: src/rawimage.cpp
	g++ -c ${CTUNING} ${OPENCV_CFLAGS} ${LIBRAW_CFLAGS} ${NOVA_CFLAGS} -o bin/objets/rawimage.o src/rawimage.cpp ${OPENCV_CLIBS} ${LIBRAW_CLIBS}
    
ui.o: src/ui.cpp
	g++ -c ${CTUNING} ${OPENCV_CFLAGS} ${LIBRAW_CFLAGS} ${NOVA_CFLAGS} -o bin/objets/ui.o src/ui.cpp ${OPENCV_CLIBS} ${LIBRAW_CLIBS}

################################################################################
# Clean	
################################################################################

# 18/04/2014 : seulement LIBRAW et TESTS
clean:
	rm -f bin/*.o

purge: clean
	rm -f bin/*.exe
