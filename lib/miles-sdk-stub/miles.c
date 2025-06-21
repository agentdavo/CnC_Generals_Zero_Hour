/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Stub library containing subset of functions from mss32.dll as used by the W3D engine.
    free(ptr);
 *
 * @copyright This is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            3 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "mss/mss.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "miniaudio.h"

static ma_engine g_engine;
static ma_mutex  g_mutex;

#define USER_DATA_SLOTS 8

typedef struct _SAMPLE {
    ma_sound sound;
    float    volume;
    int      pan;
    int      loop_count;
    int      user_data[USER_DATA_SLOTS];
    float    position[3];
    float    velocity[3];
    float    orientation[6];
    float    max_dist;
    float    min_dist;
    float    effects_level;
} SAMPLE;

typedef struct _STREAM {
    ma_sound sound;
    float    volume;
    int      pan;
    int      loop_count;
    int      user_data[USER_DATA_SLOTS];
} STREAM;

typedef struct _AUDIO { ma_sound sound; float volume; } AUDIO;
static DIG_DRIVER g_driver;
static const char *g_provider_name = "miniaudio";
static const HPROVIDER g_provider_id = 1;

long __stdcall AIL_3D_sample_volume(H3DSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return 0;
    return (long)(s->volume * 127);
}

void __stdcall AIL_set_3D_sample_volume(H3DSAMPLE sample, float volume)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->volume = volume;
    ma_sound_set_volume(&s->sound, volume);
}

void __stdcall AIL_end_3D_sample(H3DSAMPLE sample)
{
    AIL_end_sample((HSAMPLE)sample);
}

void __stdcall AIL_resume_3D_sample(H3DSAMPLE sample)
{
    AIL_resume_sample((HSAMPLE)sample);
}

void __stdcall AIL_stop_3D_sample(H3DSAMPLE sample)
{
    AIL_stop_sample((HSAMPLE)sample);
}

void __stdcall AIL_start_3D_sample(H3DSAMPLE sample)
{
    AIL_start_sample((HSAMPLE)sample);
}

unsigned int __stdcall AIL_3D_sample_loop_count(H3DSAMPLE sample)
{
    return (unsigned int)AIL_sample_loop_count((HSAMPLE)sample);
}

void __stdcall AIL_set_3D_sample_offset(H3DSAMPLE sample, unsigned int offset)
{
    AIL_set_sample_ms_position((HSAMPLE)sample, offset);
}

int __stdcall AIL_3D_sample_length(H3DSAMPLE sample)
{
    S32 len=0;
    AIL_sample_ms_position((HSAMPLE)sample, &len, NULL);
    return len;
}

unsigned int __stdcall AIL_3D_sample_offset(H3DSAMPLE sample)
{
    S32 cur=0;
    AIL_sample_ms_position((HSAMPLE)sample, NULL, &cur);
    return cur;
}

int __stdcall AIL_3D_sample_playback_rate(H3DSAMPLE sample)
{
    return AIL_sample_playback_rate((HSAMPLE)sample);
}

void __stdcall AIL_set_3D_sample_playback_rate(H3DSAMPLE sample, int playback_rate)
{
    AIL_set_sample_playback_rate((HSAMPLE)sample, playback_rate);
}

int __stdcall AIL_set_3D_sample_file(H3DSAMPLE sample, const void* file_image)
{
    return AIL_set_sample_file((HSAMPLE)sample, file_image, 0);
}

HPROVIDER __stdcall AIL_set_sample_processor(HSAMPLE sample, SAMPLESTAGE pipeline_stage, HPROVIDER provider)
{
    return 0;
}

void __stdcall AIL_set_filter_sample_preference(HSAMPLE sample, const char* name, const void* val)
{
}

void __stdcall AIL_release_sample_handle(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_sound_uninit(&s->sound);
    free(s);
}

void __stdcall AIL_close_3D_provider(HPROVIDER lib)
{
    (void)lib;
}

int __stdcall AIL_set_preference(unsigned int number, int value)
{
    if(number == DIG_USE_WAVEOUT){
        g_driver.emulated_ds = value;
        return AIL_NO_ERROR;
    }
    return AIL_NO_ERROR;
}

int __stdcall AIL_waveOutOpen(HDIGDRIVER* driver, LPHWAVEOUT* waveout, int id, LPWAVEFORMAT format)
{
    (void)waveout; (void)id; (void)format;
    if(driver) *driver = &g_driver;
    return AIL_NO_ERROR;
}

void __stdcall AIL_waveOutClose(HDIGDRIVER driver)
{
    (void)driver;
}

void __stdcall AIL_set_3D_sample_loop_count(H3DSAMPLE sample, unsigned int count)
{
    AIL_set_sample_loop_count((HSAMPLE)sample, count);
}

void __stdcall AIL_set_stream_playback_rate(HSTREAM stream, int rate)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    float pitch = rate / (float)ma_engine_get_sample_rate(&g_engine);
    ma_sound_set_pitch(&s->sound, pitch);
}

int __stdcall AIL_stream_playback_rate(HSTREAM stream)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return 0;
    float pitch = ma_sound_get_pitch(&s->sound);
    return (int)(pitch * ma_engine_get_sample_rate(&g_engine));
}

void __stdcall AIL_stream_ms_position(HSTREAM sample, S32* total_milliseconds, S32* current_milliseconds)
{
    STREAM *s = (STREAM*)sample;
    if(!s) return;
    ma_uint64 len = 0, cur = 0;
    ma_sound_get_length_in_pcm_frames(&s->sound, &len);
    ma_sound_get_cursor_in_pcm_frames(&s->sound, &cur);
    ma_uint32 rate = ma_engine_get_sample_rate(&g_engine);
    if(total_milliseconds) *total_milliseconds = (S32)((len * 1000) / rate);
    if(current_milliseconds) *current_milliseconds = (S32)((cur * 1000) / rate);
}

void __stdcall AIL_set_stream_ms_position(HSTREAM stream, int pos)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    ma_uint64 frame = ((ma_uint64)pos * ma_engine_get_sample_rate(&g_engine)) / 1000;
    ma_sound_seek_to_pcm_frame(&s->sound, frame);
}

int __stdcall AIL_stream_loop_count(HSTREAM stream)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return 0;
    return s->loop_count;
}

void __stdcall AIL_set_stream_loop_block(HSTREAM stream, int loop_start, int loop_end)
{
}

void __stdcall AIL_set_stream_loop_count(HSTREAM stream, int count)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    s->loop_count = count;
    ma_sound_set_looping(&s->sound, count != 0);
}

int __stdcall AIL_stream_volume(HSTREAM stream)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return 0;
    return (int)(s->volume * 127);
}

void __stdcall AIL_set_stream_volume(HSTREAM stream, int volume)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    s->volume = volume / 127.0f;
    ma_sound_set_volume(&s->sound, s->volume);
}

int __stdcall AIL_stream_pan(HSTREAM stream)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return 0;
    return s->pan;
}

void __stdcall AIL_set_stream_pan(HSTREAM stream, int pan)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    s->pan = pan;
    ma_sound_set_pan(&s->sound, (pan - 64) / 64.0f);
}

void __stdcall AIL_close_stream(HSTREAM stream)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    ma_sound_uninit(&s->sound);
    free(s);
}

void __stdcall AIL_pause_stream(HSTREAM stream, int onoff)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    if(onoff) ma_sound_stop(&s->sound);
    else ma_sound_start(&s->sound);
}

AIL_stream_callback __stdcall AIL_register_stream_callback(HSTREAM stream, AIL_stream_callback callback)
{
    return NULL;
}

AIL_3dsample_callback __stdcall AIL_register_3D_EOS_callback(H3DSAMPLE sample, AIL_3dsample_callback EOS)
{
    return NULL;
}

AIL_sample_callback __stdcall AIL_register_EOS_callback(HSAMPLE sample, AIL_sample_callback EOS)
{
    return NULL;
}

void __stdcall AIL_start_stream(HSTREAM stream)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    ma_sound_start(&s->sound);
}

HSTREAM __stdcall AIL_open_stream_by_sample(HDIGDRIVER driver, HSAMPLE sample, const char* file_name, int mem)
{
    return 0;
}

void __stdcall AIL_set_sample_playback_rate(HSAMPLE sample, int playback_rate)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    float pitch = playback_rate / (float)ma_engine_get_sample_rate(&g_engine);
    ma_sound_set_pitch(&s->sound, pitch);
}

int __stdcall AIL_sample_playback_rate(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return 0;
    float pitch = ma_sound_get_pitch(&s->sound);
    return (int)(pitch * ma_engine_get_sample_rate(&g_engine));
}

void __stdcall AIL_sample_ms_position(HSAMPLE sample, S32* total_ms, S32* current_ms)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_uint64 len=0, cur=0;
    ma_sound_get_length_in_pcm_frames(&s->sound, &len);
    ma_sound_get_cursor_in_pcm_frames(&s->sound, &cur);
    ma_uint32 rate = ma_engine_get_sample_rate(&g_engine);
    if(total_ms) *total_ms = (S32)((len * 1000) / rate);
    if(current_ms) *current_ms = (S32)((cur * 1000) / rate);
}

void __stdcall AIL_set_sample_ms_position(HSAMPLE sample, int pos)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_uint64 frame = ((ma_uint64)pos * ma_engine_get_sample_rate(&g_engine)) / 1000;
    ma_sound_seek_to_pcm_frame(&s->sound, frame);
}

int __stdcall AIL_sample_loop_count(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return 0;
    return s->loop_count;
}

void __stdcall AIL_set_sample_loop_count(HSAMPLE sample, int count)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->loop_count = count;
    ma_sound_set_looping(&s->sound, count != 0);
}

int __stdcall AIL_sample_volume(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return 0;
    return (int)(s->volume * 127);
}

void __stdcall AIL_set_sample_volume(HSAMPLE sample, int volume)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->volume = volume / 127.0f;
    ma_sound_set_volume(&s->sound, s->volume);
}

int __stdcall AIL_sample_pan(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return 0;
    return s->pan;
}

void __stdcall AIL_set_sample_pan(HSAMPLE sample, int pan)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->pan = pan;
    ma_sound_set_pan(&s->sound, (pan - 64) / 64.0f);
}

void __stdcall AIL_end_sample(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_sound_stop(&s->sound);
}

void __stdcall AIL_resume_sample(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_sound_start(&s->sound);
}

void __stdcall AIL_stop_sample(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_sound_stop(&s->sound);
}

void __stdcall AIL_start_sample(HSAMPLE sample)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    ma_sound_start(&s->sound);
}

void __stdcall AIL_init_sample(HSAMPLE sample)
{
    (void)sample;
}

int __stdcall AIL_set_named_sample_file(
    HSAMPLE sample, const char* file_name, const void* file_image, int file_size, int block)
{
    (void)file_image;
    (void)file_size;
    (void)block;
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return 0;
    if(ma_sound_init_from_file(&g_engine, file_name, 0, NULL, NULL, &s->sound) != MA_SUCCESS)
        return 0;
    return 1;
}

void __stdcall AIL_set_3D_sample_effects_level(H3DSAMPLE sample, float effect_level)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->effects_level = effect_level;
}

void __stdcall AIL_set_3D_sample_distances(H3DSAMPLE sample, float max_dist, float min_dist)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->max_dist = max_dist;
    s->min_dist = min_dist;
}

void __stdcall AIL_set_3D_velocity_vector(H3DSAMPLE sample, float x, float y, float z)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->velocity[0] = x; s->velocity[1] = y; s->velocity[2] = z;
}

void __stdcall AIL_set_3D_position(H3DPOBJECT obj, float X, float Y, float Z)
{
    SAMPLE *s = (SAMPLE*)obj;
    if(!s) return;
    s->position[0] = X; s->position[1] = Y; s->position[2] = Z;
}

void __stdcall AIL_set_3D_orientation(
    H3DPOBJECT obj, float X_face, float Y_face, float Z_face, float X_up, float Y_up, float Z_up)
{
    SAMPLE *s = (SAMPLE*)obj;
    if(!s) return;
    s->orientation[0] = X_face; s->orientation[1] = Y_face; s->orientation[2] = Z_face;
    s->orientation[3] = X_up;   s->orientation[4] = Y_up;   s->orientation[5] = Z_up;
}

int __stdcall AIL_WAV_info(const void* data, AILSOUNDINFO* info)
{
    if(!data || !info) return 0;
    const unsigned char* p = (const unsigned char*)data;
    if(memcmp(p, "RIFF", 4) != 0 || memcmp(p+8, "WAVE", 4) != 0) return 0;
    size_t offset = 12;
    uint16_t fmt=0, channels=0, bits=0, blockAlign=0; uint32_t rate=0; const unsigned char* dataPtr=NULL; uint32_t dataLen=0;
    while(offset < 0xFFFF){
        const char* id = (const char*)(p + offset); offset += 4;
        uint32_t size = p[offset] | (p[offset+1]<<8) | (p[offset+2]<<16) | (p[offset+3]<<24); offset += 4;
        if(memcmp(id, "fmt ",4)==0){
            fmt = p[offset] | (p[offset+1]<<8);
            channels = p[offset+2] | (p[offset+3]<<8);
            rate = p[offset+4] | (p[offset+5]<<8) | (p[offset+6]<<16) | (p[offset+7]<<24);
            blockAlign = p[offset+12] | (p[offset+13]<<8);
            bits = p[offset+14] | (p[offset+15]<<8);
        } else if(memcmp(id,"data",4)==0){
            dataPtr = p + offset;
            dataLen = size;
            break;
        }
        offset += size;
    }
    info->format = fmt;
    info->data_ptr = dataPtr;
    info->data_len = dataLen;
    info->rate = rate;
    info->bits = bits;
    info->channels = channels;
    info->samples = blockAlign ? dataLen / blockAlign : 0;
    info->block_size = blockAlign;
    info->initial_ptr = data;
    return 1;
}

void __stdcall AIL_stop_timer(HTIMER timer)
{
}

void __stdcall AIL_release_timer_handle(HTIMER timer)
{
}

void __stdcall AIL_shutdown(void)
{
    ma_engine_uninit(&g_engine);
    ma_mutex_uninit(&g_mutex);
}

int __stdcall AIL_enumerate_filters(HPROENUM* next, HPROVIDER* dest, char** name)
{
    return 0;
}

void __stdcall AIL_set_file_callbacks(AIL_file_open_callback opencb, AIL_file_close_callback closecb,
    AIL_file_seek_callback seekcb, AIL_file_read_callback readcb)
{
}

void __stdcall AIL_release_3D_sample_handle(H3DSAMPLE sample)
{
    AIL_release_sample_handle((HSAMPLE)sample);
}

H3DSAMPLE __stdcall AIL_allocate_3D_sample_handle(HPROVIDER lib)
{
    (void)lib;
    return (H3DSAMPLE)AIL_allocate_sample_handle(NULL);
}

void __stdcall AIL_set_3D_user_data(H3DPOBJECT obj, unsigned int index, int value)
{
    AIL_set_sample_user_data((HSAMPLE)obj, index, value);
}

void __stdcall AIL_unlock(void)
{
    ma_mutex_unlock(&g_mutex);
}

void __stdcall AIL_lock(void)
{
    ma_mutex_lock(&g_mutex);
}

void __stdcall AIL_set_3D_speaker_type(HPROVIDER lib, int speaker_type)
{
    (void)lib; (void)speaker_type;
}

void __stdcall AIL_close_3D_listener(H3DPOBJECT listener)
{
    AIL_release_sample_handle((HSAMPLE)listener);
}

int __stdcall AIL_enumerate_3D_providers(HPROENUM* next, HPROVIDER* dest, char** name)
{
    if(!next || !dest || !name) return 0;
    if(*next == HPROENUM_FIRST){
        *dest = g_provider_id;
        *name = (char*)g_provider_name;
        *next = 0;
        return 1;
    }
    return 0;
}

M3DRESULT __stdcall AIL_open_3D_provider(HPROVIDER lib)
{
    (void)lib;
    return M3D_NOERR;
}

char* __stdcall AIL_last_error(void)
{
    return NULL;
}

H3DPOBJECT __stdcall AIL_open_3D_listener(HPROVIDER lib)
{
    (void)lib;
    return (H3DPOBJECT)AIL_allocate_sample_handle(NULL);
}

int __stdcall AIL_3D_user_data(H3DSAMPLE sample, unsigned int index)
{
    return AIL_sample_user_data((HSAMPLE)sample, index);
}

int __stdcall AIL_sample_user_data(HSAMPLE sample, unsigned int index)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s || index >= USER_DATA_SLOTS) return 0;
    return s->user_data[index];
}

HSAMPLE __stdcall AIL_allocate_sample_handle(HDIGDRIVER dig)
{
    (void)dig;
    SAMPLE *s = calloc(1, sizeof(SAMPLE));
    if(!s) return NULL;
    s->volume = 1.0f;
    s->pan = 64;
    s->loop_count = 0;
    return s;
}

void __stdcall AIL_set_sample_user_data(HSAMPLE sample, unsigned int index, int value)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s || index >= USER_DATA_SLOTS) return;
    s->user_data[index] = value;
}

int __stdcall AIL_decompress_ADPCM(const AILSOUNDINFO *info, void **outdata, unsigned long *outsize)
{
    if(!info || !outdata || !outsize) return 0;
    ma_decoder_config cfg = ma_decoder_config_init(ma_format_s16, info->channels, info->rate);
    ma_decoder dec;
    if(ma_decoder_init_memory(info->data_ptr, info->data_len, &cfg, &dec) != MA_SUCCESS)
        return 0;
    ma_uint64 frames = 0;
    ma_decoder_get_length_in_pcm_frames(&dec, &frames);
    *outsize = (unsigned long)(frames * info->channels * sizeof(short));
    *outdata = malloc(*outsize);
    if(!*outdata){
        ma_decoder_uninit(&dec);
        return 0;
    }
    ma_decoder_read_pcm_frames(&dec, *outdata, frames, NULL);
    ma_decoder_uninit(&dec);
    return 1;
}

void __stdcall AIL_get_DirectSound_info(HSAMPLE sample, AILLPDIRECTSOUND *lplpDS, AILLPDIRECTSOUNDBUFFER *lplpDSB)
{
}

void __stdcall AIL_mem_free_lock(void *ptr)
{
    free(ptr);
}

HSTREAM __stdcall AIL_open_stream(HDIGDRIVER dig, const char *filename, int stream_mem)
{
    (void)dig; (void)stream_mem;
    STREAM *s = (STREAM*)malloc(sizeof(STREAM));
    if(!s) return NULL;
    if(ma_sound_init_from_file(&g_engine, filename, MA_SOUND_FLAG_STREAM, NULL, NULL, &s->sound) != MA_SUCCESS){
        free(s);
        return NULL;
    }
    s->volume = 1.0f;
    s->pan = 64;
    s->loop_count = 0;
    return s;
}

void __stdcall AIL_quick_unload(HAUDIO audio)
{
    AUDIO *a = (AUDIO*)audio;
    if(!a) return;
    ma_sound_uninit(&a->sound);
    free(a);
}

HAUDIO __stdcall AIL_quick_load_and_play(const char *filename, unsigned int loop_count, int wait_request)
{
    (void)wait_request;
    AUDIO *a = (AUDIO*)malloc(sizeof(AUDIO));
    if(!a) return NULL;
    if(ma_sound_init_from_file(&g_engine, filename, 0, NULL, NULL, &a->sound) != MA_SUCCESS){
        free(a);
        return NULL;
    }
    ma_sound_set_looping(&a->sound, loop_count > 1);
    ma_sound_start(&a->sound);
    a->volume = 1.0f;
    return a;
}

void __stdcall AIL_quick_set_volume(HAUDIO audio, float volume, float extravol)
{
    (void)extravol;
    AUDIO *a = (AUDIO*)audio;
    if(!a) return;
    a->volume = volume;
    ma_sound_set_volume(&a->sound, volume);
}

int __stdcall AIL_quick_startup(int use_digital, int use_MIDI, unsigned int output_rate, int output_bits, int output_channels)
{
    (void)use_digital; (void)use_MIDI; (void)output_rate; (void)output_bits; (void)output_channels;
    return AIL_startup();
}

void __stdcall AIL_quick_handles(HDIGDRIVER *pdig, HMDIDRIVER *pmdi, HDLSDEVICE *pdls)
{
    if(pdig) *pdig = &g_driver;
    if(pmdi) *pmdi = NULL;
    if(pdls) *pdls = NULL;
}

void __stdcall AIL_sample_volume_pan(HSAMPLE sample, float *volume, float *pan)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    if(volume) *volume = s->volume;
    if(pan) *pan = s->pan / 127.0f;
}

void __stdcall AIL_set_3D_sample_occlusion(H3DSAMPLE sample, float occlusion)
{
}

char *__stdcall AIL_set_redist_directory(const char *dir)
{
    return 0;
}

int __stdcall AIL_set_sample_file(HSAMPLE sample, const void *file_image, int block)
{
    return 0;
}

void __stdcall AIL_set_sample_volume_pan(HSAMPLE sample, float volume, float pan)
{
    SAMPLE *s = (SAMPLE*)sample;
    if(!s) return;
    s->volume = volume;
    s->pan = (int)(pan * 127);
    ma_sound_set_volume(&s->sound, s->volume);
    ma_sound_set_pan(&s->sound, pan * 2.0f - 1.0f);
}

void __stdcall AIL_set_stream_volume_pan(HSTREAM stream, float volume, float pan)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    s->volume = volume;
    s->pan = (int)(pan * 127);
    ma_sound_set_volume(&s->sound, s->volume);
    ma_sound_set_pan(&s->sound, pan * 2.0f - 1.0f);
}

void __stdcall AIL_stream_volume_pan(HSTREAM stream, float *volume, float *pan)
{
    STREAM *s = (STREAM*)stream;
    if(!s) return;
    if(volume) *volume = s->volume;
    if(pan) *pan = s->pan / 127.0f;
}

int __stdcall AIL_startup(void)
{
    if (ma_engine_init(NULL, &g_engine) != MA_SUCCESS)
        return -1;
    ma_mutex_init(&g_mutex);
    return 0;
}

unsigned long __stdcall AIL_get_timer_highest_delay(void)
{
    return 0;
}
