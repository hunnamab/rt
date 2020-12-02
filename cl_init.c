#include "rt.h"

const char *KernelSource = "\n" \
"__kernel void square(                                                       \n" \
"   __global float* input,                                              \n" \
"   __global float* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   if(i < count)                                                       \n" \
"       output[i] = input[i] * input[i];                                \n" \
"}                                                                      \n" \
"\n";

void    cl_init(t_cl_data cl_data)
{
    int err;
    float data[1024];              // original data set given to device
    float results[1024];           // results returned from device
    unsigned int correct;               // number of correct results returned
    unsigned int count = 1024;
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    int i;
    err = 0;
    err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &cl_data.device_id, NULL);
    cl_data.programs = malloc(sizeof(cl_program) * 1);
    cl_data.kernels = malloc(sizeof(cl_kernel) * 1);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
    cl_data.context = clCreateContext(0, 1, &cl_data.device_id, NULL, NULL, &err);
    if (!cl_data.context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }
    cl_data.commands = clCreateCommandQueue(cl_data.context, cl_data.device_id, 0, &err);
    if (!cl_data.commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }
    cl_data.programs[0] = clCreateProgramWithSource(cl_data.context, 1, (const char **)&KernelSource, NULL, &err);
    if (!cl_data.programs[0])
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }
    err = clBuildProgram(cl_data.programs[0], 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(cl_data.programs[0], cl_data.device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
    cl_data.kernels[0] = clCreateKernel(cl_data.programs[0], "square", &err);
    if (!cl_data.kernels[0] || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
    cl_data.input = clCreateBuffer(cl_data.context,  CL_MEM_READ_ONLY,  sizeof(float) * 1024, NULL, NULL);
    cl_data.output = clCreateBuffer(cl_data.context, CL_MEM_WRITE_ONLY, sizeof(float) * 1024, NULL, NULL);
    if (!cl_data.input || ! cl_data.output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }
    err = clEnqueueWriteBuffer(cl_data.commands, cl_data.input, CL_TRUE, 0, sizeof(float) * count, data, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }
    err = 0;
    err  = clSetKernelArg(cl_data.kernels[0], 0, sizeof(cl_mem), &cl_data.input);
    err |= clSetKernelArg(cl_data.kernels[0], 1, sizeof(cl_mem), &cl_data.output);
    err |= clSetKernelArg(cl_data.kernels[0], 2, sizeof(unsigned int), &count);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(cl_data.kernels[0], cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = count;
    err = clEnqueueNDRangeKernel(cl_data.commands, cl_data.kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(cl_data.commands);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer(cl_data.commands, cl_data.output, CL_TRUE, 0, sizeof(float) * count, results, 0, NULL, NULL );  
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    // Validate our results
    //
    correct = 0;
    for(i = 0; i < count; i++)
    {
        if(results[i] == data[i] * data[i])
            correct++;
    }
    
    // Print a brief summary detailing the results
    //
    printf("Computed '%d/%d' correct values!\n", correct, count);
    
    // Shutdown and cleanup
    //
    clReleaseMemObject(cl_data.input);
    clReleaseMemObject(cl_data.output);
    clReleaseProgram(cl_data.programs[0]);
    clReleaseKernel(cl_data.kernels[0]);
    clReleaseCommandQueue(cl_data.commands);
    clReleaseContext(cl_data.context);
}