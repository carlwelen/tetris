#ifndef VGA_H
#define VGA_H

#include <stdint.h>

/* DTEK-V mmap */
#define VGA_DMA_BASE  0x04000100   /* VGA DMA registers: 0x4000100-0x400010f */
#define VGA_FB_FIRST  0x080000000   /* VGA buffer:        0x8000000-0x80257ff */
#define VGA_FB_LAST   0x080257FFu   /* last valid byte address (inclusive)    */

#define VGA_WIDTH 320
#define VGA_HEIGHT 240

typedef uint16_t color_t;

/* Initiera VGA, peka DMAn på bufferten*/
void vga_init(void)
{
    volatile uint32_t *dma_buffer_addr = 
        (volatile uint32_t *)(uintptr_t)VGA_DMA_BASE;
    
    *dma_buffer_addr = (uint32_t)(uintptr_t)VGA_FB_FIRST;

}

/* Skriv en pixel till buffer*/
void vga_put_pixel(int x, int y, color_t c)
{
    if (x < 0 || x >= VGA_WIDTH || y < 0 || y >= VGA_HEIGHT) {
        return; // Error out of bounds (too beeg)
    }

    int change = (y * VGA_WIDTH + x);
    volatile color_t *buffer = (volatile color_t *)(uintptr_t)VGA_FB_FIRST;
    buffer[change] = c;
    
}

#endif