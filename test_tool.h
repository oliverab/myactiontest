
#ifdef __XC8
#include <xc.h>
void putch(char data) {
    while( ! PIR1bits.TXIF)          // wait until the transmitter is ready
        continue;
    TXREG = data;                     // send one character
}

// Call init_test() to set up UART
void init_test(void) {
    TXSTAbits.TXEN = 1;               // enable transmitter
    RCSTAbits.SPEN = 1;               // enable serial port
}
// Call end_test() to wait for "UART" then break
void end_test(void) {
    _delay(1000);
    while(1); // BREAKPOINT GOES HERE!!!
}
#define report_compiler() printf("XC8 compiler version %.2f\n", __XC8_VERSION / 1000.0f)
#else
#define init_test(X)
#define end_test(X)
#define report_compiler() printf("Non-XC8 compiler\n")
#endif
