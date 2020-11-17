#include "OpenCLFactory.h"


void getDeviceType(cl_device_type type, std::string& str){
    if(type == CL_DEVICE_TYPE_CPU) str = "CPU";
    else if(type == CL_DEVICE_TYPE_GPU) str = "GPU";
    else if(type == CL_DEVICE_TYPE_ACCELERATOR) str = "ACCELERATOR";
}




std::pair<cl::Context, cl::Device> OpenCLFactory::selectDeviceAndContext(bool print){
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    bool found = false;
    std::pair<cl::Context, cl::Device> pair;
    if(print) printf("There are %d platforms supported\n", platforms.size());
    for(cl::Platform platform : platforms){
        auto extens = platform.getInfo<CL_PLATFORM_EXTENSIONS>();
        cl::STRING_CLASS platformName = platform.getInfo<CL_PLATFORM_NAME>();
        cl::STRING_CLASS platformVendor = platform.getInfo<CL_PLATFORM_VENDOR>();
        cl::STRING_CLASS platformVersion = platform.getInfo<CL_PLATFORM_VERSION>();
        if(print) printf("Platform '%s' with vendor '%s' and version '%s'\nExtensions: %s\n", platformName.c_str(), platformVendor.c_str(), platformVersion.c_str(), extens.c_str());

        std::vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        if(print) printf("\tThere are %d devices on this platform\n", devices.size());
        for(cl::Device d : devices){
            auto name = d.getInfo<CL_DEVICE_NAME>();
            auto vendor = d.getInfo<CL_DEVICE_VENDOR>();
            auto version = d.getInfo<CL_DEVICE_VERSION>();
            auto width = d.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR>();
            cl_device_type type = d.getInfo<CL_DEVICE_TYPE>();
            auto id = d.getInfo<CL_DEVICE_VENDOR_ID>();
            auto workUnits = d.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
            auto globalSize = d.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
            auto globalCacheSize = d.getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE>();
            auto constantBufferSize = d.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();
            auto localSize = d.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
            std::string typeStr = "Unspecified custom type";
            getDeviceType(type, typeStr);
            if(print){
                LOG("\tName: %s\n\tVendor: %s\n\tVersion: %s\n\tType: %s\n\tWork Units: %d\n\tID: %d\n\tPreferred vector width: %d bytes\n\tGlobal memory size: %lu\n\tGlobal cache size: %lu\n\tMAX constant buffer size: %lu\n\tLocal memory size: %lu\n"
                , name.c_str(), vendor.c_str(), version.c_str(), typeStr.c_str(), workUnits, id, width,globalSize, globalCacheSize, constantBufferSize, localSize);
            }

            if(vendor.find("NVIDIA") != std::string::npos){
                pair = std::pair(cl::Context(d), d);
                found = true;
            }

        }
    }

    if(found){
        DEBUG("Context created!\n");
    }else{
        ERROR("Failed to create context\n");  
    }

    return pair;

}