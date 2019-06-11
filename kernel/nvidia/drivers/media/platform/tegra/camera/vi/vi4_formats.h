/*
 * NVIDIA Tegra Video Input Device Driver VI4 formats
 *
 * Copyright (c) 2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * Author: Bhanu Murthy V <bmurthyv@nvidia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __VI4_FORMATS_H_
#define __VI4_FORMATS_H_

#include <media/tegra_camera_core.h>

/*
 * These go into the VI_CHn_PIXFMT_FORMAT register bits 7:0
 * Output pixel memory format for the VI channel.
 */
enum tegra_image_format {
	TEGRA_IMAGE_FORMAT_T_L8 = 16,

	TEGRA_IMAGE_FORMAT_T_R16_I = 32,
	TEGRA_IMAGE_FORMAT_T_B5G6R5,
	TEGRA_IMAGE_FORMAT_T_R5G6B5,
	TEGRA_IMAGE_FORMAT_T_A1B5G5R5,
	TEGRA_IMAGE_FORMAT_T_A1R5G5B5,
	TEGRA_IMAGE_FORMAT_T_B5G5R5A1,
	TEGRA_IMAGE_FORMAT_T_R5G5B5A1,
	TEGRA_IMAGE_FORMAT_T_A4B4G4R4,
	TEGRA_IMAGE_FORMAT_T_A4R4G4B4,
	TEGRA_IMAGE_FORMAT_T_B4G4R4A4,
	TEGRA_IMAGE_FORMAT_T_R4G4B4A4,

	TEGRA_IMAGE_FORMAT_T_A8B8G8R8 = 64,
	TEGRA_IMAGE_FORMAT_T_A8R8G8B8,
	TEGRA_IMAGE_FORMAT_T_B8G8R8A8,
	TEGRA_IMAGE_FORMAT_T_R8G8B8A8,
	TEGRA_IMAGE_FORMAT_T_A2B10G10R10,
	TEGRA_IMAGE_FORMAT_T_A2R10G10B10,
	TEGRA_IMAGE_FORMAT_T_B10G10R10A2,
	TEGRA_IMAGE_FORMAT_T_R10G10B10A2,

	TEGRA_IMAGE_FORMAT_T_A8Y8U8V8 = 193,
	TEGRA_IMAGE_FORMAT_T_V8U8Y8A8,

	TEGRA_IMAGE_FORMAT_T_A2Y10U10V10 = 197,
	TEGRA_IMAGE_FORMAT_T_V10U10Y10A2,

	TEGRA_IMAGE_FORMAT_T_Y8_U8__Y8_V8 = 200,
	TEGRA_IMAGE_FORMAT_T_Y8_V8__Y8_U8,

	TEGRA_IMAGE_FORMAT_T_U8_Y8__V8_Y8 = 203,

	TEGRA_IMAGE_FORMAT_T_T_V8_Y8__U8_Y8 = 223,
	TEGRA_IMAGE_FORMAT_T_T_Y8__U8__V8_N444,
	TEGRA_IMAGE_FORMAT_T_Y8__U8V8_N444,
	TEGRA_IMAGE_FORMAT_T_Y8__V8U8_N444,
	TEGRA_IMAGE_FORMAT_T_Y8__U8__V8_N422,
	TEGRA_IMAGE_FORMAT_T_Y8__U8V8_N422,
	TEGRA_IMAGE_FORMAT_T_Y8__V8U8_N422,
	TEGRA_IMAGE_FORMAT_T_Y8__U8__V8_N420,
	TEGRA_IMAGE_FORMAT_T_Y8__U8V8_N420,
	TEGRA_IMAGE_FORMAT_T_Y8__V8U8_N420,

	TEGRA_IMAGE_FORMAT_T_Y10__U10__V10_N422 = 240,
	TEGRA_IMAGE_FORMAT_T_Y10__U10V10_N422,
	TEGRA_IMAGE_FORMAT_T_Y10__V10U10_N422,
	TEGRA_IMAGE_FORMAT_T_Y10__U10__V10_N420,
	TEGRA_IMAGE_FORMAT_T_Y10__U10V10_N420,
	TEGRA_IMAGE_FORMAT_T_Y10__V10U10_N420,

	TEGRA_IMAGE_FORMAT_T_R16 = 248,
	TEGRA_IMAGE_FORMAT_T_R32,
	TEGRA_IMAGE_FORMAT_T_L16_F,
	TEGRA_IMAGE_FORMAT_T_L32_F,

	TEGRA_IMAGE_FORMAT_T_DPCM_RAW10 = 254,
	TEGRA_IMAGE_FORMAT_T_DPCM_RAW12,
};

static const struct tegra_video_format vi4_video_formats[] = {
	/* RAW 6: TODO */

	/* RAW 7: TODO */

	/* RAW 8 */
	TEGRA_VIDEO_FORMAT(RAW8, 8, SRGGB8_1X8, 1, 1, T_L8,
				RAW8, SRGGB8, "RGRG.. GBGB.."),
	TEGRA_VIDEO_FORMAT(RAW8, 8, SGRBG8_1X8, 1, 1, T_L8,
				RAW8, SGRBG8, "GRGR.. BGBG.."),
	TEGRA_VIDEO_FORMAT(RAW8, 8, SGBRG8_1X8, 1, 1, T_L8,
				RAW8, SGBRG8, "GBGB.. RGRG.."),
	TEGRA_VIDEO_FORMAT(RAW8, 8, SBGGR8_1X8, 1, 1, T_L8,
				RAW8, SBGGR8, "BGBG.. GRGR.."),

	/* RAW 10 */
	TEGRA_VIDEO_FORMAT(RAW10, 10, SRGGB10_1X10, 2, 1, T_R16_I,
				RAW10, SRGGB10, "RGRG.. GBGB.."),
	TEGRA_VIDEO_FORMAT(RAW10, 10, SGRBG10_1X10, 2, 1, T_R16_I,
				RAW10, SGRBG10, "GRGR.. BGBG.."),
	TEGRA_VIDEO_FORMAT(RAW10, 10, SGBRG10_1X10, 2, 1, T_R16_I,
				RAW10, SGBRG10, "GBGB.. RGRG.."),
	TEGRA_VIDEO_FORMAT(RAW10, 10, SBGGR10_1X10, 2, 1, T_R16_I,
				RAW10, SBGGR10, "BGBG.. GRGR.."),

	/* RAW 12 */
	TEGRA_VIDEO_FORMAT(RAW12, 12, SRGGB12_1X12, 2, 1, T_R16_I,
				RAW12, SRGGB12, "RGRG.. GBGB.."),
	TEGRA_VIDEO_FORMAT(RAW12, 12, SGRBG12_1X12, 2, 1, T_R16_I,
				RAW12, SGRBG12, "GRGR.. BGBG.."),
	TEGRA_VIDEO_FORMAT(RAW12, 12, SGBRG12_1X12, 2, 1, T_R16_I,
				RAW12, SGBRG12, "GBGB.. RGRG.."),
	TEGRA_VIDEO_FORMAT(RAW12, 12, SBGGR12_1X12, 2, 1, T_R16_I,
				RAW12, SBGGR12, "BGBG.. GRGR.."),

	/* RGB888 */
	TEGRA_VIDEO_FORMAT(RGB888, 24, RGB888_1X24, 4, 1, T_A8R8G8B8,
				RGB888, ABGR32, "BGRA-8-8-8-8"),
	TEGRA_VIDEO_FORMAT(RGB888, 24, RGB888_1X32_PADHI, 4, 1, T_A8B8G8R8,
				RGB888, RGB32, "RGB-8-8-8-8"),

	/* YUV422 */
	TEGRA_VIDEO_FORMAT(YUV422, 16, UYVY8_1X16, 2, 1, T_U8_Y8__V8_Y8,
				YUV422_8, UYVY, "YUV 4:2:2"),
	TEGRA_VIDEO_FORMAT(YUV422, 16, UYVY8_1X16, 1, 1, T_Y8__V8U8_N422,
				YUV422_8, NV16, "NV16"),
	TEGRA_VIDEO_FORMAT(YUV422, 16, UYVY8_2X8, 2, 1, T_U8_Y8__V8_Y8,
				YUV422_8, UYVY, "YUV 4:2:2 UYVY"),
	TEGRA_VIDEO_FORMAT(YUV422, 16, VYUY8_2X8, 2, 1, T_T_V8_Y8__U8_Y8,
				YUV422_8, VYUY, "YUV 4:2:2 VYUY"),
	TEGRA_VIDEO_FORMAT(YUV422, 16, YUYV8_2X8, 2, 1, T_Y8_U8__Y8_V8,
				YUV422_8, YUYV, "YUV 4:2:2 YUYV"),
	TEGRA_VIDEO_FORMAT(YUV422, 16, YVYU8_2X8, 2, 1, T_Y8_V8__Y8_U8,
				YUV422_8, YVYU, "YUV 4:2:2 YVYU"),
};

#endif
