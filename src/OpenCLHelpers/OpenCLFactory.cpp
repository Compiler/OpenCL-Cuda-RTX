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
            cl_device_type type = d.getInfo<CL_DEVICE_TYPE>();
            auto id = d.getInfo<CL_DEVICE_VENDOR_ID>();
            auto workUnits = d.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
            std::string typeStr = "Unspecified custom type";
            getDeviceType(type, typeStr);
            if(print) printf("\tName: %s\n\tVendor: %s\n\tVersion: %s\n\tType: %s\n\tWork Units: %d\n\tID: %d\n"
            , name.c_str(), vendor.c_str(), version.c_str(), typeStr.c_str(), workUnits, id);
            if(vendor.find("NVIDIA") != std::string::npos){
                return std::pair<cl::Context, cl::Device>(cl::Context(d), d);
            }

        }
    }

    if(found) printf("Context created!\n");
    else printf("Failed to create context\n");  

}