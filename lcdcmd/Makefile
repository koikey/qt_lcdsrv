CC=					gcc
LD=					gcc
AR=					ar

TARGET=				lcdcmd
TEST_SRCS=			main.c
TEST_OBJS=			${TEST_SRCS:.c=.o}
TEST_INC_DIR=		./
TEST_LIB_DIR=		./
LIBS=				-static -L${TEST_LIB_DIR}

# オプション
CFLAGS=			-O0 -g3 -Wall -I${TEST_INC_DIR}
LFLAGS=			


all:				${TARGET}

## ライブラリの作成
#${LIBSTUB}:		${LIBSTUBOBJS}
#	${AR} rcs  $@ $<

# ターゲットの作成
${TARGET}:			${TEST_SRCS}
	${CC} ${CFLAGS} ${TEST_SRCS} -c -o ${TEST_OBJS}
	${LD} ${LFLAGS} ${TEST_OBJS} -o $@ ${LIBS}

#${LIBSTUBOBJS}:	${LIBSTUBSRCS}
#	${CC} -c ${CFLAGS} -o $@ $<

clean:
	-rm -rf *.o *.a ${TARGET}

#install:
#	install -m 0755 ./${TARGET} /usr/bin/${TARGET}

