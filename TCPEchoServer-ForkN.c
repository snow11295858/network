#include "TCPEchoServer.h"


void ProcessMain(int servSock);     /* �ץ����Υᥤ��ץ���� */

int main(int argc, char *argv[])
{
    int servSock;                  /* �����ФΥ����åȥǥ�������ץ� */
    unsigned short echoServPort;   /* �����ФΥݡ��� */
    pid_t processID;               /* �ץ���ID */
    unsigned int processLimit;      /* ��������ҥץ����ο� */
    unsigned int processCt;         /* �ץ����Υ����� */

    if (argc != 3)       /* �����ο�������������ǧ */
    {
        fprintf (stderr, "Ussage: %s <SERVER PORT> <FORK LIMIT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]); /* 1���ܤΰ�������������ݡ��� */
    processLimit = atoi(argv[2]); /* 2���ܤΰ��������ҥץ����ο� */

    servSock = CreateTCPServerSocket(echoServPort);

    for (processCt=0; processCt < processLimit; processCt++)
        /* �ҥץ����Υե������ȥ��顼����� */
        if ((processID == fork()) < 0)
            DieWithError("fork() failed");
        else if (processID == 0) /* �ҥץ����ξ�� */
            ProcessMain(servSock);

    exit(0);  /* �ҥץ����ν�����³�� */
}

void ProcessMain(int servSock)
{
    int clntSock;                  /* ���饤����Ȥ���³���륽���åȥǥ�������ץ� */

    for (;;) /* �����򷫤��֤� */
    {
        clntSock = AcceptTCPConnection(servSock);
        printf("with child process: %d\n", (unsigned int) getpid());
        HandleTCPClient(clntSock);
    }
}
