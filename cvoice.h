#ifndef VOICE
#define VOICE

#define SAY(_regular, _phrase, ...)                 \
    printf((_regular), (_phrase), ## __VA_ARGS__);   \
    voiceSay(_phrase);

void voiceSay (const char* phrase);

void voiceStart ();

#endif // voice.h