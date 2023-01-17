#include <err.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <linux/mctp.h>

int main(void)
{
    struct sockaddr_mctp addr = { 0 };
    char buf[] = "hello, world!";
    int sd, rc;

    /* create the MCTP socket */
    sd = socket(AF_MCTP, SOCK_DGRAM, 0);
    if (sd < 0)
        err(EXIT_FAILURE, "socket() failed");

    /* populate the remote address information */
    addr.smctp_family = AF_MCTP;  /* we're using the MCTP family */
    addr.smctp_addr.s_addr = 8;   /* send to remote endpoint ID 8 */
    addr.smctp_type = 5;          /* encapsulated protocol type (eg. PLDM = 1) */
    addr.smctp_tag = MCTP_TAG_OWNER; /* we own the tag, and so the kernel
                                        will allocate one for us */

    /* send the MCTP message */
    rc = sendto(sd, buf, sizeof(buf), 0,
                (struct sockaddr *)&addr, sizeof(addr));

    if (rc != sizeof(buf))
        err(EXIT_FAILURE, "sendto() failed");

    return EXIT_SUCCESS;
}
