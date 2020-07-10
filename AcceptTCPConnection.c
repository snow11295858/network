#include <stdio.h>      /* printf()��ɬ�� */
#include <sys/socket.h> /* accept()��ɬ�� */
#include <arpa/inet.h>  /* sockaddr_in��inet_ntoa()��ɬ�� */

void DieWithError(char *errorMessage); /* ���顼�����ؿ� */

int AcceptTCPConnection(int servSock)
{
    int clntSock;                   /* ���饤����ȤΥ����åȥǥ�������ץ� */
    struct sockaddr_in echoClntAddr; /* ���饤����ȤΥ��ɥ쥹 */
    unsigned int clntLen;           /* ���饤����ȤΥ��ɥ쥹��¤�Τ�Ĺ�� */

    /* �����ϥѥ�᡼���Υ������򥻥å� */
    clntLen = sizeof(echoClntAddr);

    /* ���饤����Ȥ������³�׵���Ԥ� */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
           &clntLen)) < 0)
        DieWithError("accept() failed");

     /* clntSock�ϥ��饤����Ȥ���³�Ѥ� */

     printf ("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

     return clntSock;
}
