#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "cmixer.h"


static SDL_mutex* audio_mutex;

static void lock_handler(cm_Event *e) {
  if (e->type == CM_EVENT_LOCK) {
    SDL_LockMutex(audio_mutex);
  }
  if (e->type == CM_EVENT_UNLOCK) {
    SDL_UnlockMutex(audio_mutex);
  }
}


static void audio_callback(void *udata, Uint8 *stream, int size) {
  cm_process((void*) stream, size / 2);
}


int main(int argc, char **argv) {
  SDL_AudioDeviceID dev;
  SDL_AudioSpec fmt, got;
  cm_Source *src;

  /* Init SDL */
  SDL_Init(SDL_INIT_AUDIO);
  audio_mutex = SDL_CreateMutex();

  /* Init SDL audio */
  memset(&fmt, 0, sizeof(fmt));
  fmt.freq      = 44100;
  fmt.format    = AUDIO_S16;
  fmt.channels  = 2;
  fmt.samples   = 1024;
  fmt.callback  = audio_callback;

  dev = SDL_OpenAudioDevice(NULL, 0, &fmt, &got, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
  if (dev == 0) {
    fprintf(stderr, "Error: failed to open audio device '%s'\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  /* Init library */
  cm_init(got.freq);
  cm_set_lock(lock_handler);
  cm_set_master_gain(0.5);

  /* Start audio */
  SDL_PauseAudioDevice(dev, 0);

  /* Create source and play */
  src = cm_new_source_from_file("loop.wav");
  if (!src) {
    fprintf(stderr, "Error: failed to create source '%s'\n", cm_get_error());
    exit(EXIT_FAILURE);
  }
  cm_set_loop(src, 1);
  cm_play(src);

  /* Wait for [return] */
  printf("Press [return] to exit\n");
  getchar();

  /* Clean up */
  cm_destroy_source(src);
  SDL_CloseAudioDevice(dev);
  SDL_Quit();

  return EXIT_SUCCESS;
}
