#ifndef __GPU_H__
#define __GPU_H__

#include <stdbool.h>
#include <stdio.h>
#include <CL/cl.h>

#define SUCCESS 0
#define FAILURE 1
#define DEBUG true

typedef struct {
	uint32_t threadIndex;
	cl_device_id device;
	cl_context context;
	cl_command_queue commandQueue;
	cl_kernel kernelHashes;
	cl_kernel kernelSearch;
	cl_program program;
	uint32_t *output;
	cl_mem outputBuffer;
	cl_mem inputBuffer;
	bool supported;
	int output_size;

//	platform_type platformType;
}GPU;

extern int opt_depth;
extern int opt_buffer;
extern int opt_worksize;

#endif /* __MINER_H__ */
