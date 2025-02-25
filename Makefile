CXX = g++
CXXFLAGS = -Wall -g

INCLUDE_PATH = ./include
MATRIX_HPP = ${INCLUDE_PATH}/matrix.hpp
LAYER_HPP = ${INCLUDE_PATH}/layer.hpp
LAYER_UTILS_H = ${INCLUDE_PATH}/layer_utils.h
CONFIG_H = ${INCLUDE_PATH}/config.h
ERROR_H = ${INCLUDE_PATH}/error.h

SRC_PATH = ./src
MAIN_CPP = ${SRC_PATH}/main.cpp
MATRIX_CPP = ${SRC_PATH}/matrix.cpp
LAYER_CPP = ${SRC_PATH}/layer.cpp
LAYER_UTILS_CPP = ${SRC_PATH}/layer_utils.cpp
CONFIG_CPP = ${SRC_PATH}/config.cpp
ERROR_CPP = ${SRC_PATH}/error.cpp

OBJ_PATH = ./bin/obj
MAIN_O = ${OBJ_PATH}/main.o
MATRIX_O = ${OBJ_PATH}/matrix.o
LAYER_O = ${OBJ_PATH}/layer.o
LAYER_UTILS_O = ${OBJ_PATH}/layer_utils.o
CONFIG_O = ${OBJ_PATH}/config.o
ERROR_O = ${OBJ_PATH}/error.o

OBJS = ${MAIN_O} ${MATRIX_O} ${LAYER_O} ${LAYER_UTILS_O} ${CONFIG_O} ${ERROR_O}

OUT_PATH = ./bin/out
TARGET = ${OUT_PATH}/nnfs

$(TARGET): $(OBJ_PATH) $(OUT_PATH) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) ${OBJS}

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(OUT_PATH):
	mkdir -p $(OUT_PATH)

${MAIN_O}: ${MAIN_CPP} ${MATRIX_HPP}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${MAIN_CPP} -o ${MAIN_O}

${MATRIX_O}: ${MATRIX_CPP} ${MATRIX_HPP} ${ERROR_H} ${CONFIG_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${MATRIX_CPP} -o ${MATRIX_O}

${LAYER_O}: ${LAYER_CPP} ${LAYER_HPP} ${LAYER_UTILS_H} ${MATRIX_HPP}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${LAYER_CPP} -o ${LAYER_O}

${LAYER_UTILS_O}: ${LAYER_UTILS_CPP} ${LAYER_UTILS_H}
	${CXX} ${CXXFLAGS} -I ${INCLUDE_PATH} -c ${LAYER_UTILS_CPP} -o ${LAYER_UTILS_O}

${CONFIG_O}: ${CONFIG_CPP} ${CONFIG_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${CONFIG_CPP} -o ${CONFIG_O}

${ERROR_O}: ${ERROR_CPP} ${ERROR_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${ERROR_CPP} -o ${ERROR_O}

run:
	${TARGET}

clean:
	rm -f $(TARGET) $(OBJS)

debug-run:
	gdb -ex run --args ${TARGET}