#pragma once
#include <CL/cl.hpp>
#include <Tools/Logging.h>
class OpenCLFactory{
    public:
        static std::pair<cl::Context, cl::Device> selectDeviceAndContext(bool print = false);
};