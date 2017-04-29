
# cmixer
A lightweight, portable ANSI C audio mixer for games.

#### Features
* Easy to use
* Tiny: two files, around 600sloc
* No dependencies (`stb_vorbis` is optional)
* Support for `wav` and `ogg` files if `stb_vorbis` is used
* Simple API for adding custom formats
* Resampling and seamless looping
* Per-source gain, pan and pitch control
* Fast, fixed-point internal processing


## Getting Started
The library's .c and .h files can be dropped into a project and compiled along
with it. If ogg vorbis support is desired `stb_vorbis.c` should also be added to
the project and the `-DCM_USE_STB_VORBIS` flag passed to the compiler.

Before use, the library should be initialized with the desired samplerate:
```c
cm_init(44100);
```

A sound can be played by first creating a source, then calling `cm_play()` on
that source.
```c
cm_Source *src = cm_new_source_from_file("sound.wav");
cm_play(src);
```

When the `cm_process()` function is called, all the currently playing streams
will be processed and mixed, the master output will be written to the provided
16bit buffer. This output is always stereo interlaced, thus the buffer length
must always be an even number:
```c
cm_Int16 buffer[512];
cm_process(buffer, 512);
```

`cm_process()` can safely be called from a separate thread; if more than one
thread is used a lock event handler must be set:
```c
static void lock_handler(cm_Event *e) {
  if (e->type == CM_EVENT_LOCK) {
    printf("acquired lock\n");
  }
  if (e->type == CM_EVENT_UNLOCK) {
    printf("released lock\n");
  }
}

cm_set_lock(lock_handler);
```

See [doc/api.md](doc/api.md) for an overview of the API or [demo](demo) for
usage demos.

## License
This library is free software; you can redistribute it and/or modify it under
the terms of the MIT license. See [LICENSE](LICENSE) for details.
