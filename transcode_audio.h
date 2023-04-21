#ifndef TRANSCODE_AUDIO_H
#define TRANSCODE_AUDIO_H


int convert_audio(const char *in_file_name,
                  unsigned long out_sampl_rate,
                  unsigned char *out_buff,
                  unsigned long *out_buff_len);


int convert_audio_and_save_wav(const char *in_file_name,
                               const char *in_file_name_wav,
                               unsigned long out_sampl_rate,
                               unsigned char *out_buff,
                               unsigned long *out_buff_len);

#endif

