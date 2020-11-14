#pragma once
#include <CL/cl.hpp>
class OpenCLFactory{
    public:
        static std::pair<cl::Context, cl::Device> selectDeviceAndContext(bool print = false);

};