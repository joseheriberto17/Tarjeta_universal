#ifndef ENCODER_H_
#define ENCODER_H_

#include <asf.h>

/* Pines ── fase A PB25 (D4) y fase B PB24 (D5) ------------- */
#define PIN_ENC_A_IDX   PIO_PC26_IDX       /* D4  */
#define PIN_ENC_A_MASK  PIO_PC26
#define PIN_ENC_A_PORT  PIOC

#define PIN_ENC_B_IDX   PIO_PC25_IDX       /* D5  */
#define PIN_ENC_B_MASK  PIO_PC25
#define PIN_ENC_B_PORT  PIOC

/* API ------------------------------------------------------- */
void  encoder_init(void);
int32_t encoder_get_position(void);
bool   encoder_get_direction(void);

#endif /* ENCODER_H_ */
