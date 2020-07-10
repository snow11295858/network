#include <stdio.h>       /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h>  /* socket()��connect()��sendto()��recvfrom()��ɬ�� */
#include <arpa/inet.h>   /* sockaddrin��inet_addr()��ɬ�� */
#include <stdlib.h>      /* atoi()��ɬ�� */
#include <string.h>      /* memset()��ɬ�� */
#include <unistd.h>      /* close()��ɬ�� */

#define MAXRECVSTRING 255 /* ��������ʸ����κ���Ĺ */

void DieWithError(char *errorMessage); /* �������顼�����ؿ� */

 int main(int argc, char *argv[])
 {
     int sock;                         /* �����å� */
     struct sockaddr_in broadcastAddr; /* �֥��ɥ��㥹�ȥ��ɥ쥹 */
     unsigned int broadcastPort;     /* �ݡ��� */
     char recvString[MAXRECVSTRING+1]; /* ʸ����μ����ѥХåե� */
     int recvStringLen;                /* ��������ʸ�����Ĺ�� */

     if (argc != 2)    /* �����ο�������������ǧ */
     {
         fprintf(stderr,"Usage: %s <Broadcast Port>\n", argv[0]);
         exit(1);
     }

     broadcastPort = atoi(argv[1]);  /* 1���ܤΰ������֥��ɥ��㥹�ȥݡ��� */

     /* UDP�ˤ��٥��ȥ��ե����ȼ��Υǡ�������ॽ���åȤ���� */
     if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

     /* �Х���ɤ��빽¤�Τκ��� */
     memset(&broadcastAddr, 0, sizeof(broadcastAddr));  /* ��¤�Τ򥼥������ */
     broadcastAddr.sin_family = AF_INET;                /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
     broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* �磻��ɥ����ɤ���� */
     broadcastAddr.sin_port = htons(broadcastPort);     /* �֥��ɥ��㥹���ѥݡ��� */

     /* �֥��ɥ��㥹�Ȥ���ݡ��ȤؤΥХ���� */
     if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
         DieWithError("bind() failed");

     /* �ǡ��������򥵡��Ф���1�ļ��� */
     if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
         DieWithError("recvfrom() failed");

     recvString[recvStringLen] = '\0';
     printf("Received: %s\n", recvString);   /* ��������ʸ�����ɽ�� */

     close(sock);
     exit(0);
 }
