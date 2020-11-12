#define CL_HPP_MINIMUM_OPENCL_VERSION 210
#define CL_HPP_TARGET_OPENCL_VERSION    

#include <CL/cl.hpp>
#include <CL/cl_platform.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>



void getDeviceType(cl_device_type type, std::string& str){
    if(type == CL_DEVICE_TYPE_CPU) str = "CPU";
    else if(type == CL_DEVICE_TYPE_GPU) str = "GPU";
    else if(type == CL_DEVICE_TYPE_ACCELERATOR) str = "ACCELERATOR";
}
int main(){
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    printf("There are %d platforms supported\n", platforms.size());
    for(cl::Platform platform : platforms){
        cl::STRING_CLASS platformName = platform.getInfo<CL_PLATFORM_NAME>();
        cl::STRING_CLASS platformVendor = platform.getInfo<CL_PLATFORM_VENDOR>();
        cl::STRING_CLASS platformVersion = platform.getInfo<CL_PLATFORM_VERSION>();
        printf("Platform '%s' with vendor '%s' and version '%s'\n", platformName.c_str(), platformVendor.c_str(), platformVersion.c_str());

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
        printf("\tThere are %d devices on this platform\n", devices.size());
        for(cl::Device d : devices){
            auto vendor = d.getInfo<CL_DEVICE_VENDOR>();
            auto version = d.getInfo<CL_DEVICE_VERSION>();
            cl_device_type type = d.getInfo<CL_DEVICE_TYPE>();
            auto id = d.getInfo<CL_DEVICE_VENDOR_ID>();
            auto workUnits = d.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
            std::string typeStr = "Unspecified custom type";
            getDeviceType(type, typeStr);
            printf("\tVendor: %s\n\tVersion: %s\n\tType: %s\n\tWork Units: %d\n\tID: %d\n"
            , vendor.c_str(), version.c_str(), typeStr.c_str(), workUnits, id);
        }
    }
    return 0;
}