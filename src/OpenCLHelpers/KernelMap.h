#pragma once
#include <unordered_map>
#include <CL/cl.hpp>
#include <Tools/Logging.h>
class KernelMap{

    private:
        std::unordered_map<std::string, cl::Kernel> _kernels;
        std::vector<std::string> _keys;
    
    public:


        void add(cl::Kernel kernel);
        void executeKernels(cl::CommandQueue& queue);

        inline cl::Kernel& getKernel(const char* name){return _kernels[name];}

};