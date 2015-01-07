#include "miner.h"

enum platform_type {
	AMD,
	NVIDIA,
	OTHER
};


void CHECK_OPENCL_ERROR(cl_int err, uint32_t id)
{
	if (err != CL_SUCCESS)
	{
		switch (err)
		{
		case CL_DEVICE_NOT_FOUND:
			applog(LOG_ERR, "[GPU%u] error:  CL_DEVICE_NOT_FOUND", id); exit(1);
		case CL_DEVICE_NOT_AVAILABLE:
			applog(LOG_ERR, "[GPU%u] error:  CL_DEVICE_NOT_AVAILABLE", id); exit(1);
		case CL_COMPILER_NOT_AVAILABLE:
			applog(LOG_ERR, "[GPU%u] error:  CL_COMPILER_NOT_AVAILABLE", id); exit(1);
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			applog(LOG_ERR, "[GPU%u] error:  CL_MEM_OBJECT_ALLOCATION_FAILURE", id); exit(1);
		case CL_OUT_OF_RESOURCES:
			applog(LOG_ERR, "[GPU%u] error:  CL_OUT_OF_RESOURCES", id); exit(1);
		case CL_OUT_OF_HOST_MEMORY:
			applog(LOG_ERR, "[GPU%u] error:  CL_OUT_OF_HOST_MEMORY", id); exit(1);
		case CL_PROFILING_INFO_NOT_AVAILABLE:
			applog(LOG_ERR, "[GPU%u] error:  CL_PROFILING_INFO_NOT_AVAILABLE", id); exit(1);
		case CL_MEM_COPY_OVERLAP:
			applog(LOG_ERR, "[GPU%u] error:  CL_MEM_COPY_OVERLAP", id); exit(1);
		case CL_IMAGE_FORMAT_MISMATCH:
			applog(LOG_ERR, "[GPU%u] error:  CL_IMAGE_FORMAT_MISMATCH", id); exit(1);
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			applog(LOG_ERR, "[GPU%u] error:  CL_IMAGE_FORMAT_NOT_SUPPORTED", id); exit(1);
		case CL_BUILD_PROGRAM_FAILURE:
			applog(LOG_ERR, "[GPU%u] error:  CL_BUILD_PROGRAM_FAILURE", id); exit(1);
		case CL_MAP_FAILURE:
			applog(LOG_ERR, "[GPU%u] error:  CL_MAP_FAILURE", id); exit(1);

		case CL_INVALID_VALUE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_VALUE", id); exit(1);
		case CL_INVALID_DEVICE_TYPE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_DEVICE_TYPE", id); exit(1);
		case CL_INVALID_PLATFORM:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_PLATFORM", id); exit(1);
		case CL_INVALID_DEVICE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_DEVICE", id); exit(1);
		case CL_INVALID_CONTEXT:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_CONTEXT", id); exit(1);
		case CL_INVALID_QUEUE_PROPERTIES:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_QUEUE_PROPERTIES", id); exit(1);
		case CL_INVALID_COMMAND_QUEUE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_COMMAND_QUEUE", id); exit(1);
		case CL_INVALID_HOST_PTR:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_HOST_PTR", id); exit(1);
		case CL_INVALID_MEM_OBJECT:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_MEM_OBJECT", id); exit(1);
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_IMAGE_FORMAT_DESCRIPTOR", id); exit(1);
		case CL_INVALID_IMAGE_SIZE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_IMAGE_SIZE", id); exit(1);
		case CL_INVALID_SAMPLER:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_SAMPLER", id); exit(1);
		case CL_INVALID_BINARY:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_BINARY", id); exit(1);
		case CL_INVALID_BUILD_OPTIONS:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_BUILD_OPTIONS", id); exit(1);
		case CL_INVALID_PROGRAM:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_PROGRAM", id); exit(1);
		case CL_INVALID_PROGRAM_EXECUTABLE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_PROGRAM_EXECUTABLE", id); exit(1);
		case CL_INVALID_KERNEL_NAME:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_KERNEL_NAME", id); exit(1);
		case CL_INVALID_KERNEL_DEFINITION:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_KERNEL_DEFINITION", id); exit(1);
		case CL_INVALID_KERNEL:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_KERNEL", id); exit(1);
		case CL_INVALID_ARG_INDEX:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_ARG_INDEX", id); exit(1);
		case CL_INVALID_ARG_VALUE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_ARG_VALUE", id); exit(1);
		case CL_INVALID_ARG_SIZE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_ARG_SIZE", id); exit(1);
		case CL_INVALID_KERNEL_ARGS:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_KERNEL_ARGS", id); exit(1);
		case CL_INVALID_WORK_DIMENSION:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_WORK_DIMENSION", id); exit(1);
		case CL_INVALID_WORK_GROUP_SIZE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_WORK_GROUP_SIZE", id); exit(1);
		case CL_INVALID_WORK_ITEM_SIZE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_WORK_ITEM_SIZE", id); exit(1);
		case CL_INVALID_GLOBAL_OFFSET:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_GLOBAL_OFFSET", id); exit(1);
		case CL_INVALID_EVENT_WAIT_LIST:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_EVENT_WAIT_LIST", id); exit(1);
		case CL_INVALID_EVENT:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_EVENT", id); exit(1);
		case CL_INVALID_OPERATION:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_OPERATION", id); exit(1);
		case CL_INVALID_GL_OBJECT:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_GL_OBJECT", id); exit(1);
		case CL_INVALID_BUFFER_SIZE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_BUFFER_SIZE", id); exit(1);
		case CL_INVALID_MIP_LEVEL:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_MIP_LEVEL", id); exit(1);
		case CL_INVALID_GLOBAL_WORK_SIZE:
			applog(LOG_ERR, "[GPU%u] error:  CL_INVALID_GLOBAL_WORK_SIZE", id); exit(1);
		}
	}
}

/* convert the kernel file into a string */
char* convertToString(const char *filename)
{
	FILE *fp;
	long lSize;
	char *buffer;

	fp = fopen ( filename , "rb" );
	if( !fp )
		perror(filename),exit(1);

	fseek( fp , 0L , SEEK_END);
	lSize = ftell( fp );
	rewind( fp );

	/* allocate memory for entire content */
	buffer = calloc( 1, lSize+1 );
	if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

	/* copy the file into the buffer */
	if( 1!=fread( buffer , lSize, 1 , fp) )
	  fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

	fclose(fp);

	return buffer;
}

enum platform_type PrintPlatformInfo(unsigned int id, cl_platform_id platformId)
{
	enum platform_type result;
	// Get Required Size
	size_t length;
	clGetPlatformInfo(platformId, CL_PLATFORM_NAME, 0, NULL, &length);
	char* sInfo = malloc(length);
 	clGetPlatformInfo(platformId, CL_PLATFORM_NAME, length, sInfo, NULL);
	applog(LOG_INFO, "[GPU%u] Platform: %s", id, sInfo);
	if (strstr(sInfo, "NVIDIA") != NULL)
		result = NVIDIA;
	else if (strstr(sInfo, "AMD") != NULL)
		result = AMD;
	else
		result = OTHER;
 	free(sInfo);

	return result;
}

void PrintDeviceInfo(unsigned int id, cl_device_id deviceId, cl_ulong *maxMem, cl_ulong *maxBuffer)
{
	// Get Required Size
	size_t length;
	clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &length);
	// Get actual device name
	char* sInfo = malloc(length);
 	clGetDeviceInfo(deviceId, CL_DEVICE_NAME, length, sInfo, NULL);
	applog(LOG_INFO, "[GPU%u] Device: %s", id, sInfo);

	clGetDeviceInfo(deviceId, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), maxMem, NULL);
	applog(LOG_INFO, "[GPU%u] Total device memory: %d MB", id, *maxMem >> 20);

	clGetDeviceInfo(deviceId, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), maxBuffer, NULL);
	applog(LOG_INFO, "[GPU%u] Maximum buffer size: %d MB", id, *maxBuffer >> 20);

 	free(sInfo);
}

cl_kernel GetKernel (cl_program program, const char* sKernelName)
{
	cl_int err;
	cl_kernel kernel = clCreateKernel(program, sKernelName, &err);
	CHECK_OPENCL_ERROR(err, 0);

	return kernel;
}

cl_mem DeviceMalloc(cl_context m_context, size_t size)
{
	cl_int err;
	cl_mem mem = clCreateBuffer(m_context, CL_MEM_READ_WRITE, size, NULL, &err);
	CHECK_OPENCL_ERROR(err, 0);

	return mem;
}

void CopyBufferToDevice(cl_command_queue queue, cl_mem buffer, void* h_Buffer, size_t size)
{

	cl_int err = clEnqueueWriteBuffer (queue, buffer, CL_TRUE, 0, size, h_Buffer, 0, NULL, NULL);
	CHECK_OPENCL_ERROR(err, 0);
}

void CopyBufferToHost  (cl_command_queue queue, cl_mem buffer, void* h_Buffer, size_t size)
{
	cl_int err = clEnqueueReadBuffer (queue, buffer, CL_TRUE, 0, size, h_Buffer, 0, NULL, NULL);
	CHECK_OPENCL_ERROR(err, 0);
}

GPU* initGPU(uint32_t id) {
	GPU* gpu = (GPU*)calloc(1, sizeof(GPU));
	gpu->threadIndex = id + 1;
	applog(LOG_DEBUG, "[GPU%u] Init", gpu->threadIndex);

	/*Step1: Getting platforms and choose an available one.*/
	cl_uint numPlatforms;	//the NO. of platforms
	cl_platform_id platform = NULL;	//the chosen platform
	cl_int	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
	{
		applog(LOG_ERR, "[GPU%u] Error %d getting platform list!", gpu->threadIndex, status);
		return NULL;
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	cl_uint				numDevices = 0;
	cl_device_id        *devices;
	cl_platform_id* platforms = (cl_platform_id*)malloc(numPlatforms* sizeof(cl_platform_id));
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);
	unsigned int gpuCount = 0;
	for (int i = 0; i < numPlatforms; i++) {
		platform = platforms[i];
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
		if (numDevices > 0) {
			if (id - gpuCount < numDevices) {
				devices = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
				status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
				gpu->device = devices[id - gpuCount];
				devices = &gpu->device;
				numDevices = 1;
				break;
			}
			else
				gpuCount += numDevices;
		}
	}
	free(platforms);

	char* value;
	size_t valueSize;

	if (gpu->device == NULL) {
		applog(LOG_INFO, "[GPU%u] Device not found", gpu->threadIndex);
		return NULL;
	}

//	gpu->platformType = PrintPlatformInfo(gpu->threadIndex, platform);
	PrintPlatformInfo(gpu->threadIndex, platform);
	cl_ulong maxMem, maxBuffer;
	PrintDeviceInfo(gpu->threadIndex, gpu->device, &maxMem, &maxBuffer);

	gpu->supported = true;

	gpu->context = clCreateContext(NULL, numDevices, devices, NULL, NULL, NULL);
	gpu->commandQueue = clCreateCommandQueue(gpu->context, gpu->device, 0, NULL);

	gpu->output_size = (1 << opt_buffer);
	gpu->output = (uint32_t*)malloc(gpu->output_size);
	gpu->outputBuffer = clCreateBuffer(gpu->context, 0, gpu->output_size, NULL, &status);
	applog(LOG_DEBUG, "[GPU%u] Allocating output buffer: %d bytes", gpu->threadIndex, gpu->output_size);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[GPU%u] Error %d allocating output buffer!", gpu->threadIndex, status);
		return NULL;
	}

	gpu->inputBuffer = clCreateBuffer(gpu->context, 0, 80, NULL, &status);
	applog(LOG_DEBUG, "[GPU%u] Allocating input buffer: %d bytes", gpu->threadIndex, 80);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[GPU%u] Error %d allocating input buffer!", gpu->threadIndex, status);
		return NULL;
	}

	const char *filename = "dcrypt.cl";

	char *source = convertToString(filename);
	size_t sourceSize[] = { strlen(source) };

	applog(LOG_DEBUG, "Creating the program: %s, size %d", filename, *sourceSize);
	gpu->program = clCreateProgramWithSource(gpu->context, 1, (const char**)&source, sourceSize, &status);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "Error %d creating the program: %s", status, filename);
		return NULL;
	}
	free(source);

	/*Step 6: Build program. */
	char *CompilerOptions = (char *)calloc(1, 256);
	sprintf(CompilerOptions, "-D OUTPUT_SIZE=%d -D STEPS=%d ", gpu->output_size, opt_depth);
/*
	if (gpu->platformType == NVIDIA)
		strcat(CompilerOptions, "-D NVIDIA ");
	if (gpu->platformType == AMD)
		strcat(CompilerOptions, "-D AMD ");
*/

	if (strlen(CompilerOptions) > 0)
		applog(LOG_DEBUG, "[GPU%u] Compiler options: %s", gpu->threadIndex, CompilerOptions);

	status = clBuildProgram(gpu->program, numDevices, devices, CompilerOptions, NULL, NULL);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[GPU%u] Error %d building the program: %s", gpu->threadIndex, status, filename);

		if (DEBUG) {
			char *programBuffer; char *programLog;
			size_t programSize; size_t logSize;
			cl_int error;
			cl_build_status buildStatus;

			// check build error and build status first
			clGetProgramBuildInfo(gpu->program, gpu->device, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &buildStatus, NULL);
			// check build log
			clGetProgramBuildInfo(gpu->program, gpu->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
			programLog = (char*) calloc (logSize+1, sizeof(char));
			clGetProgramBuildInfo(gpu->program, gpu->device, CL_PROGRAM_BUILD_LOG, logSize+1, programLog, NULL);
			applog(LOG_ERR, "[GPU%u] Build failed; error=%d, status=%d, programLog:\n\n%s", gpu->threadIndex, error, buildStatus, programLog);
			free(programLog);
		}
		return NULL;
	}

	gpu->kernelHashes = clCreateKernel(gpu->program, "hashes", NULL);
	gpu->kernelSearch = clCreateKernel(gpu->program, "search", NULL);

	applog(LOG_INFO, "[GPU%u] initialized successfully", gpu->threadIndex);
	return gpu;
}

int scanhash_dcrypt_opencl(int thr_id, uint32_t *pdata, uint32_t *target, uint32_t first_nonce, uint32_t max_nonce, GPU* gpu) {
	cl_int status;
	memset(gpu->output, 0x00, gpu->output_size);
	status = clEnqueueWriteBuffer(gpu->commandQueue, gpu->outputBuffer, CL_TRUE, 0, gpu->output_size, gpu->output, 0, NULL, NULL);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[GPU%u] Error clearing output buffer: %d!", gpu->threadIndex, status);
		return FAILURE;
	}

	status = clEnqueueWriteBuffer(gpu->commandQueue, gpu->inputBuffer, CL_TRUE, 0, 80, pdata, 0, NULL, NULL);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[GPU%u] Error writing input buffer: %d!", gpu->threadIndex, status);
		return FAILURE;
	}

	status = clSetKernelArg(gpu->kernelHashes, 0, sizeof(cl_mem), &gpu->inputBuffer);
	status = clSetKernelArg(gpu->kernelHashes, 1, sizeof(cl_mem), &gpu->outputBuffer);

	size_t global_work_size[1] = { max_nonce - first_nonce + 1 };
	size_t local_work_size[1] = { opt_worksize  };
	size_t global_offset[1] = { first_nonce  };
	applog(LOG_DEBUG, "[%u] Search: %u - %u", thr_id, global_work_size[0], global_offset[0]);
	status = clEnqueueNDRangeKernel(gpu->commandQueue, gpu->kernelHashes, 1, global_offset, global_work_size, local_work_size, 0, NULL, NULL);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[%u] GPU error %d: search", thr_id, status);
		return FAILURE;
	}

	status = clEnqueueReadBuffer(gpu->commandQueue, gpu->outputBuffer, CL_TRUE, 0, gpu->output_size, gpu->output, 0, NULL, NULL);
	if (status != CL_SUCCESS) {
		applog(LOG_ERR, "[GPU%u] Error reading search results: %d!", gpu->threadIndex, status);
		return FAILURE;
	}

	uint32_t results = gpu->output[(gpu->output_size >> 2) - 1];
//	applog(LOG_INFO, "[GPU%u] Got %u hashes", gpu->threadIndex, results);

	if (results > 0) {
		global_work_size[0] = results;
		gpu->output[(gpu->output_size >> 2) - 1] = 0;
		status = clEnqueueWriteBuffer(gpu->commandQueue, gpu->outputBuffer, CL_TRUE, 0, gpu->output_size, gpu->output, 0, NULL, NULL);
		if (status != CL_SUCCESS) {
			applog(LOG_ERR, "[GPU%u] Error clearing result buffer: %d!", gpu->threadIndex, status);
			return -1;
		}

		status = clSetKernelArg(gpu->kernelSearch, 0, sizeof(cl_mem), &gpu->inputBuffer);
		status = clSetKernelArg(gpu->kernelSearch, 1, sizeof(cl_mem), &gpu->outputBuffer);
		status = clSetKernelArg(gpu->kernelSearch, 2, sizeof(uint32_t), target);
		status = clEnqueueNDRangeKernel(gpu->commandQueue, gpu->kernelSearch, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		if (status != CL_SUCCESS) {
			applog(LOG_ERR, "[%u] GPU error %d: search", thr_id, status);
			return -1;
		}

		status = clEnqueueReadBuffer(gpu->commandQueue, gpu->outputBuffer, CL_TRUE, 0, gpu->output_size, gpu->output, 0, NULL, NULL);
		if (status != CL_SUCCESS) {
			applog(LOG_ERR, "[GPU%u] Error reading search results: %d!", gpu->threadIndex, status);
			return -1;
		}
	}

	return results;
}
