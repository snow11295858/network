#include "TCPEchoServer.h" /* TCP�����������ФΥإå��ե�����򥤥󥯥롼�� */
#include <pthread.h>      /* POSIX����åɤ�ɬ�� */

void *ThreadMain(void *arg);          /* ����åɤΥᥤ��ץ���� */

/* ���饤����ȥ���åɤ��Ϥ������ι�¤�� */
struct ThreadArgs
{
int clntSock;                          /* ���饤����ȤΥ����åȥǥ�������ץ� */
};

int main(int argc, char *argv[])
{
    int servSock;                    /* �����ФΥ����åȥǥ�������ץ� */
    int clntSock;                    /* ���饤����ȤΥ����åȥǥ�������ץ� */
    unsigned short echoServPort;     /* �����ФΥݡ��� */
    pthread_t threadID;              /* pthread_create()���֤�����å�ID */
    struct ThreadArgs *threadArgs;   /* ����åɤΰ�����¤�ΤؤΥݥ��� */

    if (argc != 2)    /* �����ο�������������ǧ */
    {
        fprintf(stderr,"Usage: %s <SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]); /* 1���ܤΰ����� ������ݡ��� */

    servSock = CreateTCPServerSocket(echoServPort);

    for (;;) /* �ץ���ब��λ����ޤǷ����֤��¹� */
    {
        clntSock = AcceptTCPConnection(servSock);

        /* ���饤����Ȱ����Ѥ˥���򿷤������� */
        if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs)))
               == NULL)
             DieWithError("malloc() failed");
        threadArgs -> clntSock = clntSock;

        /* ���饤����ȥ���åɤ����� */
        if (pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
            DieWithError("pthread_create() failed");
        printf("with thread %ld\n", (long int) threadID);
    }
    /* ������ʬ�ˤ���ã���ʤ� */
}

void *ThreadMain(void *threadArgs)
{
    int clntSock; /* ���饤�������³�ѥ����åȥǥ�������ץ� */

    /* �����ˡ�����åɤΥ꥽�����������Ʋ�� */
    pthread_detach(pthread_self());

    /* �����åȤΥե�����ǥ�������ץ������������Ф� */
    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    free(threadArgs);              /* �����˳�����Ƥ��Ƥ����������� */

    HandleTCPClient(clntSock);

    return (NULL);
}
