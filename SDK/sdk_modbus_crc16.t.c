#include <sdk_modbus_crc16.h>
#include <stdio.h>
#include <stdlib.h>
#include <sdk_hex.h>


int main(int argc, char** argv){

    uint8_t buf[]={0x01, 0x03, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00};

    uint16_t crc = sdk_modbus_crc16(buf, sizeof(buf)-2);

    SDK_HEX_SET_UINT16_LE(buf, 6, crc);

    sdk_hex_dump("bytes", buf, sizeof(buf));

    printf("Modbus CRC16:%x\n", crc);

    return 0;
}

