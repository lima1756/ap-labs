
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include "logger.h"

#define INFO_COLOR 37
#define WARNING_COLOR 33
#define ERROR_COLOR 31
#define PANIC_FONT_COLOR 30
#define PANIC_BACK_COLOR 41
#define DEFAULT_BACK_COLOR 48
#define DEFAULT_FONT_COLOR 37

char syslogOn = 0;
int syslogStatus = 0;

int initLogger(char *logType) {
    if(strcmp(logType,"syslog") == 0){
        syslogOn=1;
    }
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

void setColor(int font, int back) {
	printf("%c[0;%d;%dm",0x1B, font, back);
}

void resetColor() {
    setColor(DEFAULT_FONT_COLOR, DEFAULT_BACK_COLOR);
}

void print(const char *format, va_list args) {
    if(syslogOn){
        openlog("logger", LOG_PID | LOG_CONS, LOG_SYSLOG);
        vsyslog(syslogStatus, format, args);
        closelog();
    }
    else{
        vprintf(format, args);
    }
    
}

int infof(const char *format, ...){
    setColor(INFO_COLOR, DEFAULT_BACK_COLOR);
    syslogStatus = LOG_INFO;
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
    return 0;
}

int warnf(const char *format, ...) {
    setColor(WARNING_COLOR, DEFAULT_BACK_COLOR);
    syslogStatus = LOG_WARNING;
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
    return 0;
}

int errorf(const char *format, ...) {
    setColor(ERROR_COLOR, DEFAULT_BACK_COLOR);
    syslogStatus = LOG_ERR;
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
    return 0;
}

int panicf(const char *format, ...) {
    setColor(PANIC_FONT_COLOR, PANIC_BACK_COLOR);
    syslogStatus = LOG_CRIT;
    va_list args;
    va_start(args, format);
    print(format, args);
    va_end(args);
    resetColor();
    return 0;
}