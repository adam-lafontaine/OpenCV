#if defined(_WIN32)
#include "../utils/win32_leak_check.h"
#endif

#include "src/tutorial.hpp"

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;   // check block integrity
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF; // don't recycle memory
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF;     // leak report on exit
	_CrtSetDbgFlag(dbgFlags);
#endif



	//display_image();

	//lookup_table();

	//mask_operations();

	//mat_operations();

	//blending();

	//linear_transform();

	//discrete_fourier_transform();

	//read_write();

	//parallel_for();

	//basic_draw();

	//random_draw();

	//smoothing();

	//erode_dilate();

	//morphology();

	//hit_or_miss();

	//extract_h_v_lines();

	//zoom_in_out();

	//threashold();

	//threshold_inrange();

	//filter2d();

	//border();

	//sobel_scharr();

	sobel_video();
}