#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

#define INFO_COLOR 37
#define WARNING_COLOR 33
#define ERROR_COLOR 31
#define PANIC_FONT_COLOR 30
#define PANIC_BACK_COLOR 41
#define DEFAULT_BACK_COLOR 48
#define DEFAULT_FONT_COLOR 37

void setColor(int font, int back) {
	printf("%c[0;%d;%dm",0x1B, font, back);
}

void resetColor() {
    setColor(DEFAULT_FONT_COLOR, DEFAULT_BACK_COLOR);
    fflush(stdout);
}

void print(const char *format, va_list args) {
    vprintf(format, args);
}

int infof(const char *format, ...){
    setColor(INFO_COLOR, DEFAULT_BACK_COLOR);
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
}

int warnf(const char *format, ...) {
    setColor(WARNING_COLOR, DEFAULT_BACK_COLOR);
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
}

int errorf(const char *format, ...) {
    setColor(ERROR_COLOR, DEFAULT_BACK_COLOR);
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
}

int panicf(const char *format, ...) {
    setColor(PANIC_FONT_COLOR, PANIC_BACK_COLOR);
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
    raise(SIGABRT);
}

