
CXX = g++
C++_VERSION = c++17
CXXFLAGS = -std=$(C++_VERSION) -Wall -w -g -static-libgcc -static-libstdc++

OUT_DIR = bin
LAUNCHER_NAME = OpenCLRTX
SRC_DIR = src
ENTRY_POINT = src/main.cpp

#Libs
OPENCL_INC = extern/OpenCL/
OPENCL_LIB = extern/OpenCL/lib/

GLM_INC = extern/glm/

INC=-I$(SRC_DIR) -I$(OPENCL_INC) -I$(GLM_INC)
LIBS =-L$(OPENCL_LIB)
LINKS = -lOpenCL2
#objects
OPENCL_HELPER_OBJS = $(OUT_DIR)/OpenCLFactory.o
OBJS = $(OPENCL_HELPER_OBJS)
ALL_SETTINGS = $(CXX) $(CXXFLAGS) $(INC) $(LIBS)  


main: $(ENTRY_POINT) $(OBJS)
	$(CXX) $(CXXFLAGS) $^  $(INC) $(LIBS) -o $(OUT_DIR)/$(LAUNCHER_NAME) $(LINKS)
	./$(OUT_DIR)/$(LAUNCHER_NAME).exe

$(OPENCL_HELPER_OBJS): $(OUT_DIR)/%.o: src/OpenCLHelpers/%.cpp
	$(ALL_SETTINGS) -c $< -o $@  


run: $(OUT_DIR)/$(LAUNCHER_NAME).exe
	./$(OUT_DIR)/$(LAUNCHER_NAME).exe

launch:
	make
	make run