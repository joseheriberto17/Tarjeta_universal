#include "pwm.h"

void pwm_init12()
{
    /* 1. Activar reloj del periférico PWM */
    pmc_enable_periph_clk(ID_PWM);

    pio_configure(PIOC, PIO_PERIPH_B, PIO_PC23 | PIO_PC24 , PIO_DEFAULT);

    // pwm_channel_disable(PWM, );


}