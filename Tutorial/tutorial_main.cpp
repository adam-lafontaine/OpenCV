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

	//sobel_video();

	//laplacian();

	//laplacian_video();

	//canny();

	//canny_video();

	//hough_line();

	//hough_circle();

	//remap();

	//affine_transform();

	//equalize_hist();

	//calc_hist();

	//compare_hist();

	//back_projection();

	//template_matching();

	//contours();

	//contour_video();

	//convex_hull();

	//bounding();

	//bounding_rotated();

	//moments();

	//point_polygon();

	//distance_transform();

	//deblur_filter();

	//motion_deblur();

	//anisotropic();

	//periodic_noise_filter();

	//trackbar();

	//gdal();

	video();
}