#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CL/cl.h>

inline const char *clGetErrorString(cl_int error)
{
	switch (error)
	{
// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

	// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

	// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}

#define CHECK_EC if (ec != CL_SUCCESS) printf("Line %i: %s\n", __LINE__, clGetErrorString(ec))
#define Exit { (void)system("pause"); exit(0); }

int main(void)
{
	char *src;
	size_t srcSz;
	{
		FILE *fs = fopen("Main.cl", "rb");
		fseek(fs, 0, SEEK_END);
		srcSz = ftell(fs);
		src = malloc(srcSz + 1);
		rewind(fs);
		fread(src, 1, srcSz, fs);
		fclose(fs);
		src[srcSz] = 0;
	}

	cl_uint nPlatforms;
	cl_platform_id platform;
	cl_device_id device;
	cl_uint nDevices;
	cl_int ec;

	ec = clGetPlatformIDs(1, &platform, &nPlatforms); CHECK_EC;
	ec = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &nDevices); CHECK_EC;
	if (ec == CL_DEVICE_NOT_FOUND || ec == CL_DEVICE_NOT_AVAILABLE)
		Exit;

	char deviceName[128];
	ec = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(deviceName), &deviceName, NULL); CHECK_EC;
	printf("Device Name: '%s'\n", deviceName);

	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &ec); CHECK_EC;
	cl_command_queue q = clCreateCommandQueueWithProperties(context, device, NULL, &ec); CHECK_EC;

	enum { nObjs = 7};
	float in[nObjs] = { 6, 9, 4, 20, 123, 9090909, 120312.321f };
	float out[nObjs];
	cl_mem buff = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof in, &in, &ec); CHECK_EC;

	cl_program prog = clCreateProgramWithSource(context, 1, &src, &srcSz, &ec); CHECK_EC;
	ec = clBuildProgram(prog, 1, &device, NULL, NULL, NULL); CHECK_EC;
	if (ec == CL_BUILD_PROGRAM_FAILURE)
	{
		size_t sz;
		clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &sz);

		char *log = malloc(sz);
		clGetProgramBuildInfo(prog, device, CL_PROGRAM_BUILD_LOG, sz, log, NULL);
		puts(log);
	}

	cl_kernel kernel = clCreateKernel(prog, "FastISqrt", &ec); CHECK_EC;
	ec = clSetKernelArg(kernel, 0, sizeof buff, &buff); CHECK_EC;
	size_t nThrds = nObjs;
	ec = clEnqueueNDRangeKernel(q, kernel, 1, NULL, &nThrds, NULL, 0, NULL, NULL); CHECK_EC;
	ec = clEnqueueReadBuffer(q, buff, CL_TRUE, 0, sizeof out, out, 0, NULL, NULL); CHECK_EC;
	for (int i = 0; i < nObjs; i++)
		printf("In: %f, Out: %f\n", in[i], out[i]);

	ec = clFlush(q); CHECK_EC;
	ec = clFinish(q); CHECK_EC;
	ec = clReleaseKernel(kernel); CHECK_EC;
	ec = clReleaseProgram(prog); CHECK_EC;
	ec = clReleaseMemObject(buff); CHECK_EC;
	ec = clReleaseCommandQueue(q); CHECK_EC;
	ec = clReleaseContext(context); CHECK_EC;

	Exit;
}