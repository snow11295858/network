#include <stdio.h>      /* fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��connect()��send()��recv()��ɬ�� */
#include <arpa/inet.h>  /* sockaddr_in��inet_addr()��ɬ�� */
#include <stdlib.h>     /* atoi()��ɬ�� */
#include <string.h>     /* memset()��ɬ�� */
#include <unistd.h>     /* sleep()��ɬ�� */

void DieWithError(char *errorMessage);  /* �������顼�����ؿ� */


int main(int argc, char *argv[])
 {
     int sock;                        /* �����å� */
     struct sockaddr_in multicastAddr; /* �ޥ�����㥹�ȥ��ɥ쥹 */
     char *multicastIP;               /* IP�ޥ�����㥹�ȥ��ɥ쥹 */
     unsigned short multicastPort;     /* �����ФΥݡ��� */
     char *sendString;                /* �ޥ�����㥹�Ȥ���ʸ���� */
     unsigned char multicastTTL;      /* �ޥ�����㥹�ȥѥ��åȤ�TTL */
     unsigned int sendStringLen;      /* �ޥ�����㥹�Ȥ���ʸ�����Ĺ�� */

     if ((argc < 4) || (argc > 5))         /* �ѥ�᡼���ο�������������ǧ */
     {
         fprintf(stderr, "Usage: %s <Multicast Address> <Port> <Send String> [<TTL>]\n",
                  argv[0]);
         exit(1);
     }

     multicastIP = argv[1];              /* 1���ܤΰ������ޥ�����㥹�Ȥ���IP���ɥ쥹 */
     multicastPort = atoi(argv[2]);      /* 2���ܤΰ������ޥ�����㥹���ѥݡ��� */
     sendString = argv[3];             /* 2���ܤΰ������ޥ�����㥹�Ȥ���ʸ���� */

     if (argc == 5)                    /* TTL�����ޥ�ɥ饤�󤫤���ꤵ��Ƥ��뤫 */
         multicastTTL = atoi(argv[4]); /* ���ޥ�ɥ饤�󤫤���ꤵ�줿TTL */
     else
     multicastTTL = 1;                 /* �ǥե���Ȥ�TTL = 1 */

     /* �ǡ���������������ѥ����åȤκ��� */
     if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
     DieWithError("socket() failed");

     /* �ޥ�����㥹�ȥѥ��åȤ�TTL������ */
     if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &multicastTTL,
           sizeof(multicastTTL)) < 0)
         DieWithError("setsockopt() failed");

     /* ������Υ��ɥ쥹��¤�Τ���� */
     memset(&multicastAddr, 0, sizeof(multicastAddr)); /* ��¤�Τ򥼥������ */
     multicastAddr.sin_family = AF_INET;               /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
     multicastAddr.sin_addr.s_addr = inet_addr(multicastIP);/* �ޥ�����㥹��IP���ɥ쥹 */
     multicastAddr.sin_port = htons(multicastPort);       /* �ޥ�����㥹�Ȥ���ݡ��� */

     sendStringLen = strlen(sendString); /* sendString��Ĺ����Ĵ�٤� */
     for (;;) /* �ץ���ब��λ����ޤǷ����֤��¹� */
     {
         /* sendString�Υǡ���������3�ä��Ȥ˥��饤����Ȥإޥ�����㥹�Ȥ��� */
         if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *)
               &multicastAddr, sizeof(multicastAddr)) != sendStringLen)
             DieWithError("sendto() sent a different number of bytes than expected");
         sleep(3);
     }
     /* ������ʬ�ˤ���ã���ʤ� */
 }

