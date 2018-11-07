CXX = g++
CPPFLAGS = -std=c++11 -I../include
LDFLAGS = -lpthread -lrt

BIN = systimer_test delegate_test msgqueue_test

all:${BIN}

systimer_test:
	${CXX} ${CPPFLAGS} ${LDFLAGS} ../src/timer.cpp systimer_test.cpp -o $@

delegate_test:
	${CXX} ${CPPFLAGS} ${LDFLAGS} delegate_test.cpp -o $@

msgqueue_test:
	${CXX} ${CPPFLAGS} ${LDFLAGS} ../src/msgqueue.cpp msgqueue_test.cpp -o $@

clean:
	rm -f ${BIN}
