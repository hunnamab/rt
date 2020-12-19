extern "C"{
    #include "rt_cuda.h"
    }
void	print_gpu_info(void)
{
	int val;
	cudaDeviceProp *prop;

	prop = (cudaDeviceProp *)malloc(sizeof(cudaDeviceProp));
	cudaGetDeviceCount(&val);
	printf("device count %d\n", val);
	cudaGetDevice(&val);
	printf("current dev %d\n", val);
	cudaGetDeviceProperties(prop, 0);
	printf("device name %s\n", prop->name);
	printf("max threads per block %d\n", prop->maxThreadsPerBlock);
	printf("multiProcessorCount %d\n", prop->multiProcessorCount);
	printf("regsPerBlock %d\n", prop->regsPerBlock);
	printf("warp size %d\n", prop->warpSize);
	printf("canMapHostMemory %d\n", prop->canMapHostMemory);
	printf("maxGridSize[3] (%d,%d,%d)\n", prop->maxGridSize[0],prop->maxGridSize[1],prop->maxGridSize[2]);
}