/* PipeWire */
/* SPDX-FileCopyrightText: Copyright © 2021 Axis Communications AB */
/* SPDX-License-Identifier: MIT */

/*
 [title]
 Stress test using pipewire-alsa.
 [title]
 */

#include <alsa/asoundlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define DEFAULT_PCM		"pipewire"
#define DEFAULT_RATE		44100
#define DEFAULT_CHANNELS	2
#define N_THREADS		20

static void *
thread_func(void *data)
{
	snd_pcm_t *pcm = NULL;
	snd_pcm_hw_params_t *params;
	int res;
	long n = (long)data;
	unsigned int sample_rate = DEFAULT_RATE;

	res = snd_pcm_open(&pcm, DEFAULT_PCM, SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK);
	if (res < 0) {
		fprintf(stderr, "open failed: %s\n", snd_strerror(res));
		pcm = NULL;
		goto fail;
	}
	printf("opened %ld\n", n);

	snd_pcm_hw_params_alloca(&params);
	res = snd_pcm_hw_params_any(pcm, params);
	if (res < 0) {
		fprintf(stderr, "params_any failed: %s\n", snd_strerror(res));
		goto fail;
	}

	res = snd_pcm_hw_params_set_access(pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (res < 0) {
		fprintf(stderr, "set_access failed: %s\n", snd_strerror(res));
		goto fail;
	}

	res = snd_pcm_hw_params_set_format(pcm, params, SND_PCM_FORMAT_S32_LE);
	if (res < 0) {
		fprintf(stderr, "set_format failed: %s\n", snd_strerror(res));
		goto fail;
	}

	res = snd_pcm_hw_params_set_rate_near(pcm, params, &sample_rate, 0);
	if (res < 0) {
		fprintf(stderr, "set_rate_near failed: %s\n", snd_strerror(res));
		goto fail;
	}

	res = snd_pcm_hw_params_set_channels(pcm, params, DEFAULT_CHANNELS);
	if (res < 0) {
		fprintf(stderr, "set_channels failed: %s\n", snd_strerror(res));
		goto fail;
	}

	res = snd_pcm_hw_params(pcm, params);
	if (res < 0) {
		fprintf(stderr, "params failed: %s\n", snd_strerror(res));
		goto fail;
	}

	res = snd_pcm_prepare(pcm);
	if (res < 0) {
		fprintf(stderr, "prepare failed: %s (%d)\n", snd_strerror(res), res);
		goto fail;
	}
	printf("prepared %ld\n", n);

	res = snd_pcm_close(pcm);
	if (res < 0) {
		fprintf(stderr, "close failed: %s\n", snd_strerror(res));
		pcm = NULL;
		goto fail;
	}
	printf("closed %ld\n", n);

	return NULL;

fail:
	if (pcm != NULL) {
		res = snd_pcm_close(pcm);
		if (res < 0) {
			fprintf(stderr, "close failed: %s\n", snd_strerror(res));
		}
	}
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	pthread_t t[N_THREADS] = { 0 };
	long n;
	int s;

	/* avoid rtkit in this test */
	setenv("PIPEWIRE_CONFIG_NAME", "client.conf", false);

	while (true) {
		for (n=0; n < N_THREADS; n++) {
			if ((s = pthread_create(&(t[n]), NULL, thread_func, (void *)n)) != 0) {
				fprintf(stderr, "pthread_create: %s\n", strerror(s));
				exit(EXIT_FAILURE);
			}
			printf("created %ld\n", n);
		}
		for (n=0; n < N_THREADS; n++) {
			if (t[n] != 0 && (s = pthread_join(t[n], NULL)) != 0) {
				fprintf(stderr, "pthread_join: %s\n", strerror(s));
				exit(EXIT_FAILURE);
			}
			printf("joined %ld\n", n);
			t[n] = 0;
		}
	}

	return EXIT_SUCCESS;
}
