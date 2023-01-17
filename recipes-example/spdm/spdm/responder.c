#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/mctp.h>

int main(void)
{
    struct sockaddr_mctp addr = { 0 };
    char buf[4096];
    int sd, rc;

    /* create the MCTP socket */
    sd = socket(AF_MCTP, SOCK_DGRAM, 0);
    if (sd < 0)
        err(EXIT_FAILURE, "socket() failed");

    /* populate the local address information for our bind(), which defines
     * properties of the messages that we will receive */
    addr.smctp_family = AF_MCTP;
    addr.smctp_addr.s_addr = MCTP_ADDR_ANY;   /* receive from any address */
    addr.smctp_type = 5;                      /* receive messages of type 5 */

    rc = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc)
        err(EXIT_FAILURE, "bind() failed");

    for (;;) {
        socklen_t addrlen;
        ssize_t len;

        addrlen = sizeof(addr);

        /* receive an incoming message, as well as the sender's address */
        len = recvfrom(sd, buf, sizeof(buf), MSG_TRUNC,
                      (struct sockaddr *)&addr,  &addrlen);

        if (len <= 0) {
            err(EXIT_FAILURE, "recvfrom failed()");

        } else if (len > (ssize_t)sizeof(buf)) {
            warnx("recvfrom: message too large for buffer");
            continue;

        }

        printf("message (%zd bytes) from remote EID 0x%02x\n",
                len, addr.smctp_addr);

        addr.smctp_addr.s_addr = 0x0b;
        for (size_t i=0; i<len; ++i) {
            printf("%c%02x", i%16==0?'\n':' ', buf[i]);
        }
        printf("\n");

        /* for the tag used in the reply, we clear the tag-owner bit, but
         * keep the tag value */
        addr.smctp_tag &= ~MCTP_TAG_OWNER;

        /* return message to sender */
        sendto(sd, buf, len, 0,
               (struct sockaddr *)&addr, sizeof(addr));
    }

    return EXIT_SUCCESS;
}
