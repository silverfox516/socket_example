#ifndef _LINUX_TTY_COLOR_H
#define _LINUX_TTY_COLOR_H

#include <stdarg.h>

#define tty_color(color)   "\e[38;5;" #color "m"
#define tty_clear()        "\e[0m"

#define LOG_TAG
#define log(color, fmt, arg...) \
	printf(tty_color(color) LOG_TAG " : " fmt tty_clear() "\n", ##arg);

#define logd(fmt, arg...) log(39, fmt, ##arg);
#define logi(fmt, arg...) log(190, fmt, ##arg);
#define logw(fmt, arg...) log(208, fmt, ##arg);
#define loge(fmt, arg...) log(135, fmt, ##arg);

void tty_color_print()
{
    int i;
    for (i = 0; i < 256; i++) {
        printf("\e[38;5;%dm%d \e[0m" , i, i);
    }
    printf("\n");
}

#endif  /* _LINUX_TTY_COLOR_H */
