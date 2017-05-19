#include "opencv_gpu_cpu.h"

COpencvGC::COpencvGC() :m_opencvGC(EGPUCPU_CPU)
{

}
bool COpencvGC::init_gpu()
{
	int num_devices = cv::gpu::getCudaEnabledDeviceCount();
	if (num_devices <= 0)
	{
		std::cerr << "There is no device." << std::endl;
		return false;
	}

	int enable_device_id = -1;


	for (int i = 0; i < num_devices; i++)
	{
		cv::gpu::DeviceInfo dev_info(i);
		if (dev_info.isCompatible())
		{
			enable_device_id = i;
		}
	}
	if (enable_device_id < 0)
	{
		std::cerr << "GPU module isn't built for GPU" << std::endl;
		return false;
	}

	gpu::setDevice(enable_device_id);

	m_opencvGC = EGPUCPU_GPU;

	return true;
}