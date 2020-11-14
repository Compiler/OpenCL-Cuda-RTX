#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include <OpenCLHelpers/OpenCLFactory.h>
#include <CL/cl.hpp>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>
#include <fstream>


void checkForError(cl_int error){
    static int count = 0;
    if(error) printf("ERROR::%d::\tERROR NUM %d\n", count, error);    
    count++;
}

void loadTextFromFile(const char fileName[], std::string& dst){
    std::ifstream t(fileName);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    dst = str;
    if(dst.empty()) printf("Couldn't open : '%s'\n", fileName);
}


int main(){
    
    auto [context, chosenDevice] = OpenCLFactory::selectDeviceAndContext();

    std::string fileContents;
    loadTextFromFile("src/test.cl", fileContents);
    cl_int error;
    
    cl::Program program(context, fileContents.c_str(), CL_TRUE, &error);
    checkForError(error);

    error = program.build("-cl-std=CL1.2");
    checkForError(error);

    cl::Kernel kernel(program, "test", &error);
    checkForError(error);

    printf("Executing kernel:\n");
    cl::CommandQueue queue(context, chosenDevice);
    queue.enqueueTask(kernel);


    
    return 0;
}