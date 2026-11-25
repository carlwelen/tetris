#ifndef VGA_H
#define VGA_H

#include <stdint.h>

/* DTEK-V mmap */
#define VGA_DMA_BASE  0x04000100u   /* VGA DMA registers: 0x4000100-0x400010f */
#define VGA_FB_FIRST  0x08000000u   /* VGA buffer:        0x8000000-0x80257ff */
#define VGA_FB_LAST   0x080257FFu   /* last valid byte address (inclusive)    */

#define VGA_BUFFER_BASE 0x8000000
#define VGA_DMA_BASE 0x4000100
#define VGA_WIDTH 320
#define VGA_HEIGHT 240

typedef uint16_t color_t;

/* Initiera VGA, peka DMAn på bufferten*/
void vga_init(void);

/* Skriv en pixel till buffer*/
void vga_put_pixel(int x, int y, color_t c);

#endif