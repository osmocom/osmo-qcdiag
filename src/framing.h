#ifndef framing_h
#define framing_h

#include <stdint.h>
#include <sys/types.h>

#define MAX_PACKET 2048

int frame_pack(const uint8_t *in, const size_t in_len, uint8_t *out, const size_t out_len);
int frame_unpack(uint8_t *pDataIn, int nDataIn, uint8_t *pPacket);

#endif
