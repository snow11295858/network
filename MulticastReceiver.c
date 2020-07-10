#include <stdio.h>      /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��connect()��sendto()��recvfrom()��ɬ�� */
#include <arpa/inet.h>  /* sockaddr_in��inet_addr()��ɬ�� */
#include <stdlib.h>     /* atoi()��ɬ�� */
#include <string.h>     /* memset()��ɬ�� */
#include <unistd.h>     /* close()��ɬ�� */

#define MAXRECVSTRING 255  /* ��������ʸ����κ���Ĺ */

void DieWithError(char *errorMessage); /* �������顼�����ؿ� */


 int main(int argc, char *argv[])
 {
     int sock;                         /* �����å� */
     struct sockaddr_in multicastAddr; /* �ޥ�����㥹�ȥ��ɥ쥹 */
     char *multicastIP;                /* �ޥ�����㥹��IP���ɥ쥹 */
     unsigned int multicastPort;       /* �ݡ��� */
     char recvString[MAXRECVSTRING+1]; /* ʸ��������ѥХåե� */
     int recvStringLen;                /* ��������ʸ�����Ĺ�� */
     struct ip_mreq multicastRequest;  /* ���祤�󤹤�ޥ�����㥹�ȥ��ɥ쥹�ι�¤�� */

     if (argc != 3)       /* �����ο�������������ǧ */
     {
         fprintf(stderr, "Usage: %s <Multicast IP> <Multicast Port>\n", argv[0]);
         exit(1);
     }

     multicastIP = argv[1];        /* 1���ܤΰ������ޥ�����㥹��IP���ɥ쥹�ʥɥå�10��ɽ���� */
     multicastPort = atoi(argv[2]);/* 2���ܤΰ������ޥ�����㥹�ȥݡ��� */

     /* UDP�ˤ��٥��ȥ��ե����ȼ��Υǡ������������ */
     if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
         DieWithError("socket() failed");

     /* �Х���ɤ��빽¤�Τκ��� */
     memset(&multicastAddr, 0, sizeof(multicastAddr));   /* ��¤�Τ򥼥������ */

     multicastAddr.sin_family = AF_INET;                 /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
     multicastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* �磻��ɥ����ɤ���� */
     multicastAddr.sin_port = htons(multicastPort);      /* �ޥ�����㥹�ȥݡ��� */

     /* �ޥ�����㥹�ȥݡ��ȤإХ���� */
     if (bind(sock, (struct sockaddr *) &multicastAddr, sizeof(multicastAddr))< 0)
         DieWithError("bind() failed");

     /* �ޥ�����㥹�ȥ��롼�פλ��� */
     multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastIP);
     /* Ǥ�դΥ��󥿥ե���������ޥ�����㥹�Ȥ�����դ��� */
     multicastRequest.imr_interface. s_addr = htonl(INADDR_ANY);
     /* �ޥ�����㥹�ȥ��롼�פؤΥ��祤�� */
     if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest,
           sizeof(multicastRequest)) < 0)
         DieWithError("setsockopt() failed");

     /* �����Ф���ǡ���������1�ļ��� */
     if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
         DieWithError("recvfrom() failed");

     recvString[recvStringLen] =
     printf("Received: %s\n", recvString);     /* ��������ʸ�����ɽ�� */

     close(sock);
     exit(0);
 }


