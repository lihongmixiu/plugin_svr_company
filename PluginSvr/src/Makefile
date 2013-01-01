########################################
#  desc:makefile for plugin server
#  author:wili
#  date:2012-08-13
#  target:plugin_server
########################################

CC  :=  gcc
CXX :=  g++
LD  :=  g++
RM  :=  @rm -rf

PLUGIN_LIB   = -L./../lib
PLUGIN_SRC   = $(wildcard *.cpp)
PLUGIN_OBJ   = $(addprefix ./,$(subst .cpp,.o, ${PLUGIN_SRC}))
TARGET       = ./plugin_server
INC_LIB_PATH = -I./../lib
INC_DLL_LIB_PATH = -I ./../include
LDFLAGS      = ${PLUGIN_LIB} -levent
CPPFLAGS     = -g -Wall ${INC_DLL_LIB_PATH} ${INC_LIB_PATH}

all: ${TARGET}
${TARGET}:${PLUGIN_OBJ}
	$(CXX) -o $@ ${PLUGIN_OBJ} ${CPPFLAGS} ${LDFLAGS}
./%.o : %.cpp
	$(CXX) -o $@ -c $< ${CPPFLAGS}

.PHONY:all clean FORCE
clean:
	$(RM) *.o ${TARGET}
FORCE:
	

