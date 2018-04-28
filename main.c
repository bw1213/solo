#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "cbor.h"
#include "usbhid.h"
#include "util.h"


void check_ret(CborError ret)
{
    if (ret != CborNoError)
    {
        printf("CborError: %d\n", ret);
        exit(1);
    }
}

int main(int argc, char * argv[])
{
    CborError ret;
    uint8_t buf[16];
    memset(buf,0,sizeof(buf));
    CborEncoder encoder;
    cbor_encoder_init(&encoder, buf, sizeof(buf), 0);
    ret = cbor_encode_int(&encoder, 55);
    check_ret(ret);

    dump_hex(buf,sizeof(buf));

    printf("init usbhid\n");
    usbhid_init();

    uint8_t hidmsg[64];
    memset(hidmsg,0,sizeof(hidmsg));

    printf("recv'ing hid msg \n");

    while(1)
    {
        usbhid_recv(hidmsg);
        printf(">> "); dump_hex(hidmsg,sizeof(hidmsg));
        printf("<< "); dump_hex(hidmsg,sizeof(hidmsg));
        usbhid_send(hidmsg);
    }


    usbhid_close();
    printf("done\n");
    return 0;
}