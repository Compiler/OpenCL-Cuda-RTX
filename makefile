
CXX = g++
C++_VERSION = c++17
CXXFLAGS = -std=$(C++_VERSION) -Wall -w -g -static-libgcc -static-libstdc++

OUT_DIR = bin
LAUNCHER_NAME = CudaOpenCLRTX
SRC_DIR = src
ENTRY_POINT = src/main.cpp

#Libs
OPENCL_INC = outsourced/OpenCL-SDK-main/include


INC=-I $(SRC_DIR) -I $(OPENCL_INC)
LIBS =-L $(GLFW_LIB)
LINKS = -lopencl
OBJS = 
ALL_SETTINGS = $(CXX) $(CXXFLAGS) $(LIBS) $(INC) 


main: $(ENTRY_POINT) $(OBJS)
	$(ALL_SETTINGS) -o $(OUT_DIR)/$(LAUNCHER_NAME) $^ $(LINKS)
	./$(OUT_DIR)/$(LAUNCHER_NAME).exe

$(MATHS_OBJS): $(OUT_DIR)/%.o: src/maths/%.cpp
	$(ALL_SETTINGS) -c $< -o $@  




run: $(OUT_DIR)/$(LAUNCHER_NAME).exe
	./$(OUT_DIR)/$(LAUNCHER_NAME).exe

launch:
	make
	make run