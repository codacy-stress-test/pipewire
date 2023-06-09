/* Spa */
/* SPDX-FileCopyrightText: Copyright © 2021 Wim Taymans */
/* SPDX-License-Identifier: MIT */

#include "volume-ops.h"

#include <xmmintrin.h>

void
volume_f32_sse(struct volume *vol, void * SPA_RESTRICT dst,
		const void * SPA_RESTRICT src, float volume, uint32_t n_samples)
{
	uint32_t n, unrolled;
	float *d = (float*)dst;
	const float *s = (const float*)src;

	if (volume == VOLUME_MIN) {
		memset(d, 0, n_samples * sizeof(float));
	}
	else if (volume == VOLUME_NORM) {
		spa_memcpy(d, s, n_samples * sizeof(float));
	}
	else {
		__m128 t[4];
		const __m128 vol = _mm_set1_ps(volume);

		if (SPA_IS_ALIGNED(d, 16) &&
		    SPA_IS_ALIGNED(s, 16))
			unrolled = n_samples & ~15;
		else
			unrolled = 0;

		for(n = 0; n < unrolled; n += 16) {
			t[0] = _mm_load_ps(&s[n]);
			t[1] = _mm_load_ps(&s[n+4]);
			t[2] = _mm_load_ps(&s[n+8]);
			t[3] = _mm_load_ps(&s[n+12]);
			_mm_store_ps(&d[n], _mm_mul_ps(t[0], vol));
			_mm_store_ps(&d[n+4], _mm_mul_ps(t[1], vol));
			_mm_store_ps(&d[n+8], _mm_mul_ps(t[2], vol));
			_mm_store_ps(&d[n+12], _mm_mul_ps(t[3], vol));
		}
		for(; n < n_samples; n++)
			_mm_store_ss(&d[n], _mm_mul_ss(_mm_load_ss(&s[n]), vol));
	}
}
