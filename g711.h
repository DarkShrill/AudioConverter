#ifndef G711_H
#define G711_H

unsigned char __af_linear2alaw(int pcm_val);
int __af_alaw2linear(unsigned char a_val);
unsigned char __af_linear2ulaw (int pcm_val);
int __af_ulaw2linear (unsigned char u_val);
unsigned char alaw2ulaw(unsigned char aval);
unsigned char ulaw2alaw(unsigned char uval);


#endif
