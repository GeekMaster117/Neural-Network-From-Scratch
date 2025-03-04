CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

INCLUDE_PATH = ./include
MATRIX_HPP = ${INCLUDE_PATH}/matrix.hpp
LAYER_HPP = ${INCLUDE_PATH}/layer.hpp
LAYER_UTILS_H = ${INCLUDE_PATH}/layer_utils.h
CSV_UTILS_H = ${INCLUDE_PATH}/csv_utils.h
ACTIVATION_H = ${INCLUDE_PATH}/activation.h
CONFIG_H = ${INCLUDE_PATH}/config.h
ERROR_H = ${INCLUDE_PATH}/error.h

SRC_PATH = ./src
TRAIN_CPP = ${SRC_PATH}/train.cpp
MATRIX_CPP = ${SRC_PATH}/matrix.cpp
LAYER_CPP = ${SRC_PATH}/layer.cpp
LAYER_UTILS_CPP = ${SRC_PATH}/layer_utils.cpp
CSV_UTILS_CPP = ${SRC_PATH}/csv_utils.cpp
ACTIVATION_CPP = ${SRC_PATH}/activation.cpp
CONFIG_CPP = ${SRC_PATH}/config.cpp
ERROR_CPP = ${SRC_PATH}/error.cpp

OBJ_PATH = ./bin/obj
TRAIN_O = ${OBJ_PATH}/train.o
MATRIX_O = ${OBJ_PATH}/matrix.o
LAYER_O = ${OBJ_PATH}/layer.o
LAYER_UTILS_O = ${OBJ_PATH}/layer_utils.o
CSV_UTILS_O = ${OBJ_PATH}/csv_utils.o
ACTIVATION_O = ${OBJ_PATH}/activation.o
CONFIG_O = ${OBJ_PATH}/config.o
ERROR_O = ${OBJ_PATH}/error.o

OUT_PATH = ./bin/out
TRAIN = ${OUT_PATH}/train

DATA_PATH = ./data
LAYER1_WEIGHTS = $(DATA_PATH)/layer1_weights.csv
LAYER1_BIASES = $(DATA_PATH)/layer1_biases.csv
LAYER2_WEIGHTS = $(DATA_PATH)/layer2_weights.csv
LAYER2_BIASES = $(DATA_PATH)/layer2_biases.csv
LAYER3_WEIGHTS = $(DATA_PATH)/layer3_weights.csv
LAYER3_BIASES = $(DATA_PATH)/layer3_biases.csv

PARAMS = $(LAYER1_WEIGHTS) $(LAYER1_BIASES) $(LAYER2_WEIGHTS) $(LAYER2_BIASES) $(LAYER3_WEIGHTS) $(LAYER3_BIASES)

OBJS = ${TRAIN_O} ${MATRIX_O} ${LAYER_O} ${LAYER_UTILS_O} ${CSV_UTILS_O} ${ACTIVATION_O} ${CONFIG_O} ${ERROR_O}

$(TRAIN): $(OBJ_PATH) $(OUT_PATH) ${DATA_PATH} $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TRAIN) ${OBJS}

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(OUT_PATH):
	mkdir -p $(OUT_PATH)

$(DATA_PATH):
	mkdir -p $(DATA_PATH)

${TRAIN_O}: ${TRAIN_CPP} ${LAYER_HPP} ${LAYER_UTILS_HPP} ${CSV_UTILS_H} ${ACTIVATION_HPP} ${CONFIG_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${TRAIN_CPP} -o ${TRAIN_O}

${MATRIX_O}: ${MATRIX_CPP} ${MATRIX_HPP} ${ERROR_H} ${CONFIG_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${MATRIX_CPP} -o ${MATRIX_O}

${LAYER_O}: ${LAYER_CPP} ${LAYER_HPP} ${LAYER_UTILS_H} ${MATRIX_HPP}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${LAYER_CPP} -o ${LAYER_O}

${LAYER_UTILS_O}: ${LAYER_UTILS_CPP} ${LAYER_UTILS_H} ${CONFIG_H} ${ERROR_H}
	${CXX} ${CXXFLAGS} -I ${INCLUDE_PATH} -c ${LAYER_UTILS_CPP} -o ${LAYER_UTILS_O}

${CSV_UTILS_O}: ${CSV_UTILS_CPP} ${CSV_UTILS_H} ${MATRIX_HPP} ${CONFIG_H} ${ERROR_H}
	${CXX} ${CXXFLAGS} -I ${INCLUDE_PATH} -c ${CSV_UTILS_CPP} -o ${CSV_UTILS_O}

${ACTIVATION_O}: ${ACTIVATION_CPP} ${ACTIVATION_H}
	${CXX} ${CXXFLAGS} -I ${INCLUDE_PATH} -c ${ACTIVATION_CPP} -o ${ACTIVATION_O}

${CONFIG_O}: ${CONFIG_CPP} ${CONFIG_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${CONFIG_CPP} -o ${CONFIG_O}

${ERROR_O}: ${ERROR_CPP} ${ERROR_H}
	$(CXX) $(CXXFLAGS) -I $(INCLUDE_PATH) -c ${ERROR_CPP} -o ${ERROR_O}

train:
	${TRAIN}

clean-bin:
	rm -f $(TRAIN) $(OBJS)

clean-params:
	rm -f $(PARAMS)

debug-train:
	gdb -ex run --args ${TRAIN}