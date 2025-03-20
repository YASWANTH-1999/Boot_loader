#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>

#define UART_PORT "COM5"    // Change this to match your port
#define BAUD_RATE 115200

#define CMD_ERASE   0x01
#define CMD_WRITE   0x02
#define CMD_RUN     0x04

#define PACKET_SIZE  256

typedef struct {
    uint8_t command;
    uint32_t address;
    uint8_t data[PACKET_SIZE];
    uint16_t length;
} FlashPacket;

HANDLE hSerial;

// Open UART connection
int uart_open() {
    hSerial = CreateFile(UART_PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to open COM port.\n");
        return -1;
    }
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = BAUD_RATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);
    return 0;
}

// Send data via UART
void uart_send(FlashPacket *packet) {
    DWORD bytesWritten;
    WriteFile(hSerial, packet, sizeof(FlashPacket), &bytesWritten, NULL);
}

// Send firmware to STM32
void send_firmware(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open firmware file.\n");
        return;
    }
    
    FlashPacket packet;
    packet.command = CMD_ERASE;
    uart_send(&packet);
    Sleep(100);
    
    size_t bytesRead;
    uint32_t address = 0x08004000;  // Application start address
    while ((bytesRead = fread(packet.data, 1, PACKET_SIZE, file)) > 0) {
        packet.command = CMD_WRITE;
        packet.address = address;
        packet.length = bytesRead;
        uart_send(&packet);
        Sleep(10);
        address += bytesRead;
    }
    
    packet.command = CMD_RUN;
    uart_send(&packet);
    printf("Firmware successfully updated!\n");
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s firmware.bin\n", argv[0]);
        return 1;
    }
    if (uart_open() != 0) return 1;
    send_firmware(argv[1]);
    CloseHandle(hSerial);
    return 0;
}
