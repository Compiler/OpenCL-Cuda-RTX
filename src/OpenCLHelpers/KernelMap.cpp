#include "KernelMap.h"


void KernelMap::add(cl::Kernel kernel){
    std::string kernelName = kernel.getInfo<CL_KERNEL_FUNCTION_NAME>().c_str();
    bool found = false;
    if(_keys.size() > 0)
        for(auto name : _keys){
            DEBUG("Checking '%s'", name.c_str());
            if(name.c_str() == kernelName){
                ERROR("function: '%s' == %s w/ %d args and %d references -- already defined in map", kernelName.c_str(), name.c_str(), kernel.getInfo<CL_KERNEL_NUM_ARGS>(), kernel.getInfo<CL_KERNEL_REFERENCE_COUNT>() );
                found = true;
            }
        }
    if(!found){
        _kernels[kernelName] = kernel;
        _keys.emplace_back(std::string(kernelName));
        DEBUG("function: '%s' w/ %d args and %d references -- added to map", kernelName.c_str(), kernel.getInfo<CL_KERNEL_NUM_ARGS>(), kernel.getInfo<CL_KERNEL_REFERENCE_COUNT>() );
    }

}

void KernelMap::executeKernels(cl::CommandQueue& queue){
    LOG("Executing all kernels in map:");
    for(auto& name : _keys){
        cl_int error = queue.enqueueTask(_kernels[name]);
        DEBUG("%d", error);
    }
}
