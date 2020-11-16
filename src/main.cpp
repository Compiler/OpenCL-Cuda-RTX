#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#ifndef __FILE__ 
#define __FILE__ -1
#endif
#include <OpenCLHelpers/OpenCLFactory.h>
#include <OpenCLHelpers/KernelMap.h>
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
    if(error != 0){
        auto log = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(chosenDevice, &error);
        ERROR("%d", error);
        ERROR("%s", log.c_str());

    }

    error = program.build("-cl-std=CL1.2 -DTESTER=64 -Werror");
    DEBUG("%d", error);

    int twoInts[2];
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(twoInts));

    glm::vec4 a(1,2,3,4);
    glm::vec4 b(5,6,7,8);
    glm::vec4 allData[3] = {a,b, glm::vec4(0)};
    cl::Buffer mainBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * 3, &allData[0]);
    cl_buffer_region region;
    region.origin = 0;
    region.size = sizeof(glm::vec4);
    cl::Buffer subBuffer1 = mainBuffer.createSubBuffer(CL_MEM_READ_WRITE, CL_BUFFER_CREATE_TYPE_REGION, (const void*)&region, &error);
    cl_buffer_region region2;
    region2.origin = sizeof(glm::vec4);
    region2.size = sizeof(glm::vec4);
    cl::Buffer subBuffer2 = mainBuffer.createSubBuffer(CL_MEM_READ_WRITE, CL_BUFFER_CREATE_TYPE_REGION, (const void*)&region2, &error);
    cl_buffer_region region3;
    region3.origin = sizeof(glm::vec4);
    region3.size = sizeof(glm::vec4);
    cl::Buffer subBuffer3 = mainBuffer.createSubBuffer(CL_MEM_READ_WRITE, CL_BUFFER_CREATE_TYPE_REGION, (const void*)&region3, &error);

    KernelMap map;

    cl::Kernel kern(program, "multiply", &error);DEBUG("%d", error);
    kern.setArg(0, subBuffer1);
    kern.setArg(1, subBuffer2);
    kern.setArg(2, subBuffer3);

    cl::CommandQueue queue(context, chosenDevice);
    auto workItems = 4;
    queue.enqueueNDRangeKernel(kern, 1, workItems);
    queue.enqueueReadBuffer(subBuffer3, CL_TRUE, 0, sizeof(glm::vec4), &allData[2]);

    LOG("Returned Vec:\t[%f %f %f %f]", allData[2].x, allData[2].y, allData[2].z, allData[2].w);


    
    return 0;
}