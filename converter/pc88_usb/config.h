/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONFIG_H
#define CONFIG_H

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x8801
#define DEVICE_VER      0x0101
#define MANUFACTURER    t.m.k.
#define PRODUCT         PC88 keyboard converter
#define DESCRIPTION     converts PC88 keyboard protocol into USB


/* matrix size */
#define MATRIX_ROWS     16
#define MATRIX_COLS     8

/* key combination for command */
#define IS_COMMAND()    ( \
    keyboard_report->keys[0] == KC_STOP || \
    keyboard_report->mods == (MOD_BIT(KC_LALT) | MOD_BIT(KC_RALT)) \
)


/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE


/*
 * PC88 Serial(USART) configuration
 *     asynchronous, positive logic, 20800baud, bit order: LSB first
 *     1-start bit, 8-data bit, non parity, 1-stop bit
 */

/*
 * Software Serial
 *   Add protocol/serial_soft.c to SRC in Makefile
 */
#define SERIAL_SOFT_BAUD                20800
#define SERIAL_SOFT_PARITY_NONE
#define SERIAL_SOFT_BIT_ORDER_LSB
#define SERIAL_SOFT_LOGIC_POSITIVE
/* RXD Port */
#define SERIAL_SOFT_RXD_DDR             DDRD
#define SERIAL_SOFT_RXD_PORT            PORTD
#define SERIAL_SOFT_RXD_PIN             PIND
#define SERIAL_SOFT_RXD_BIT             2
#define SERIAL_SOFT_RXD_READ()          (SERIAL_SOFT_RXD_PIN&(1<<SERIAL_SOFT_RXD_BIT))
/* RXD Interupt */
#define SERIAL_SOFT_RXD_VECT            INT2_vect
#define SERIAL_SOFT_RXD_INIT()          do { \
    /* pin configuration: input with pull-up */ \
    SERIAL_SOFT_RXD_DDR &= ~(1<<SERIAL_SOFT_RXD_BIT); \
    SERIAL_SOFT_RXD_PORT |= (1<<SERIAL_SOFT_RXD_BIT); \
    /* enable interrupt: INT2(falling edge) */ \
    EICRA |= ((1<<ISC21)|(0<<ISC20)); \
    EIMSK |= (1<<INT2); \
    sei(); \
} while (0)
#define SERIAL_SOFT_RXD_INT_ENTER()
#define SERIAL_SOFT_RXD_INT_EXIT()      do { \
    /* clear interrupt  flag */ \
    EIFR = (1<<INTF2); \
} while (0)
/* TXD Port */
#define SERIAL_SOFT_TXD_DDR             DDRD
#define SERIAL_SOFT_TXD_PORT            PORTD
#define SERIAL_SOFT_TXD_PIN             PIND
#define SERIAL_SOFT_TXD_BIT             3
#define SERIAL_SOFT_TXD_HI()            do { SERIAL_SOFT_TXD_PORT |=  (1<<SERIAL_SOFT_TXD_BIT); } while (0)
#define SERIAL_SOFT_TXD_LO()            do { SERIAL_SOFT_TXD_PORT &= ~(1<<SERIAL_SOFT_TXD_BIT); } while (0)
#define SERIAL_SOFT_TXD_INIT()          do { \
    /* pin configuration: output */ \
    SERIAL_SOFT_TXD_DDR |= (1<<SERIAL_SOFT_TXD_BIT); \
    /* idle */ \
    SERIAL_SOFT_TXD_ON(); \
} while (0)


/*
 * Hardware Serial(UART)
 *   Add protocol/serial_uart.c to SRC in Makefile
 */
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32U2__)
    #define SERIAL_UART_BAUD       20800
    #define SERIAL_UART_DATA       UDR1
    #define SERIAL_UART_UBRR       ((F_CPU/(16UL*SERIAL_UART_BAUD))-1)
    #define SERIAL_UART_RXD_VECT   USART1_RX_vect
    #define SERIAL_UART_TXD_READY  (UCSR1A&(1<<UDRE1))
    #define SERIAL_UART_INIT()     do { \
        UBRR1L = (uint8_t) SERIAL_UART_UBRR;       /* baud rate */ \
        UBRR1H = (uint8_t) (SERIAL_UART_UBRR>>8);  /* baud rate */ \
        UCSR1B |= (1<<RXCIE1) | (1<<RXEN1); /* RX interrupt, RX: enable */ \
        UCSR1B |= (0<<TXCIE1) | (1<<TXEN1); /* TX interrupt, TX: enable */ \
        UCSR1C |= (0<<UPM11) | (0<<UPM10);  /* parity: none(00), even(01), odd(11) */ \
        DDRD  &= ~(1<<2); PORTD |=  (1<<2); /* Pull-up RXD pin */ \
        sei(); \
    } while(0)
#else
    #error "USART configuration is needed."
#endif


#endif
