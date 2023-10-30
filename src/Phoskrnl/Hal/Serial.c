#include "Hal.h"

//
// UART code will be changed later to use interrupts
//

BOOLEAN
PHOSAPI
HalInitSerial(
	IN UINT16 Port 
) {
	CONST UINT8 TestByte = 0x88;
	 
	__outbyte(Port + UART_IER, 0);          // Disable interrupts (for now)
	__outbyte(Port + UART_LCR, 1 << 7);     // Enable DLAB
	
	__outbyte(Port,            1);          // Low baud rate divisor byte
	__outbyte(Port + UART_IER, 0);          // High baud rate divisor byte
	__outbyte(Port + UART_FCR, 0b11000111); // Enable FIFO registers, clear receive & transmit FIFO, DMA mode 0, 14 bytes
	__outbyte(Port + UART_LCR, 0b11);       // 8/N/1 framing

	__outbyte(Port + UART_MCR, 0b11111);    // Data terminal ready, request to send, use both outputs, set in loopback mode (test port)

	__outbyte(Port, TestByte);

	if (__inbyte(Port) != TestByte)
		return FALSE;
	
	__outbyte(Port + UART_MCR, 0b1111);     // DTR, RTS, use both outputs
	
	return TRUE;
}

UINT8
PHOSAPI
HalPollSerial(
	IN UINT16 Port 
) {
	return __inbyte(Port + UART_LSR);
}

UINT8
PHOSAPI
HalReadSerial(
	IN UINT16 Port 
) {
	while (!(HalPollSerial(Port) & 1));

	return __inbyte(Port);
}

VOID
PHOSAPI
HalWriteSerial(
	IN UINT16 Port,
	IN UINT8  Value
) {
	while (!(HalPollSerial(Port) & 1 << 5));

	__outbyte(Port, Value);
}

VOID
PHOSAPI
HalWriteSerialEx(
	IN UINT16       Port,
	IN CONST UINT8 *Data,
	IN UINTN        Size
) {
	for (UINTN i = 0; i < Size; i++) {
		while (!(HalPollSerial(Port) & 1 << 5));

		__outbyte(Port, Data[i]);
	}
}