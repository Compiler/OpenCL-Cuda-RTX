#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#ifndef __FILE__ 
#define __FILE__ -1
#endif
#include <OpenCLHelpers/OpenCLFactory.h>
#include <Tools/Logging.h>
#include <CL/cl.hpp>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>
#include <fstream>


void loadTextFromFile(const char fileName[], std::string& dst){
    std::ifstream t(fileName);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    dst = str;
    if(dst.empty()) printf("Couldn't open : '%s'\n", fileName);
}


int main(){
    

    auto [context, chosenDevice] = OpenCLFactory::selectDeviceAndContext();
    auto refCount = context.getInfo<CL_CONTEXT_REFERENCE_COUNT>();
    printf("The context has %d references\n", refCount);

    std::string fileContents;
    loadTextFromFile("src/test.cl", fileContents);
    cl_int error;
    cl::Program program(context, fileContents.c_str(), CL_TRUE, &error);
    DEBUG("%d", error);

    error = program.build("-cl-std=CL1.2 -TESTER=4");
    DEBUG("%d", error);
    
    int twoInts[2];
    cl::Buffer buffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(twoInts));

    cl::Kernel kernel(program, "test", &error);
    DEBUG("%d", error);
    error = kernel.setArg(0, buffer);
    DEBUG("%d", error);

    printf("Executing kernel:\n");
    cl::CommandQueue queue(context, chosenDevice);
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(buffer, 1, 0, sizeof(twoInts), twoInts);
    printf("Returned: (%d, %d)", twoInts[0], twoInts[1]);

    
    return 0;
}