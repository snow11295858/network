#include <stdio.h>      /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��connect()��sendto()��recvfrom()��ɬ�� */
#include <arpa/inet.h>  /* sockaddr_in��inet_addr()��ɬ�� */
#include <stdlib.h>     /* atoi()��ɬ�� */
#include <string.h>     /* memset()��ɬ�� */
#include <unistd.h>     /* close()��ɬ�� */
#include <errno.h>      /* errno��EINTR��ɬ�� */
#include <signal.h>     /* sigaction()��ɬ�� */

#define ECHOMAX        255    /* ������ʸ����κ���Ĺ */
#define TIMEOUT_SECS   2      /* �������ޤǤ��ÿ� */
#define MAXTRIES       5      /* �����Բ�� */

int tries=0;   /* ��������Υ����󥿡ʥ����ʥ�ϥ�ɥ餫��Υ��������ѡ������Х�� */

void DieWithfrror(char *errorMessage);   /* ���顼�����ؿ� */
void CatchAlarm(int ignored);            /* SIGALRM�Υϥ�ɥ� */

int main(int argc, char *argv[])
{
    int sock;                       /* �����åȥǥ�������ץ� */
    struct sockaddr_in echoServAddr; /* �����������ФΥ��ɥ쥹 */
    struct sockaddr_in fromAddr;     /* �������������Υ��ɥ쥹 */
    unsigned short echoServPort;    /* �����������ФΥݡ����ֹ� */
    unsigned int fromSize;        /* recvfrom()�Υ��ɥ쥹�������ϥ����� */
    struct sigaction myAction;      /* �����ʥ�ϥ�ɥ�������� */
    char *servIP;                   /* �����Ф�IP���ɥ쥹 */
    char *echoString;               /* �����������Ф���������ʸ���� */
    char echoBuffer[ECHOMAX+1];     /* ������ʸ����μ����Хåե� */
    int echoStringLen;              /* ������ʸ�����Ĺ�� */
    int respStringLen;              /* �����ǡ���������Ĺ�� */
   
    if ((argc < 3) || (argc > 4))   /* �����ο�������������ǧ */
    {
        fprintf(stderr,"Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
        exit(1);
    }
   
    servIP = argv[1];           /* 1���ܤΰ����� �����Ф�IP���ɥ쥹�ʥɥåȶ��ڤ�10��ɽ���� */
    echoString = argv[2];       /* 2���ܤΰ����� ������ʸ���� */
   
    if ((echoStringLen = strlen(echoString)) > ECHOMAX)

    DieWithError("Echo word too long");
   
    if (argc == 4)
        echoServPort = atoi(argv[3]); /* ����Υݡ����ֹ椬����л��� */
    else
        echoServPort = 7; /* �����������ӥ���well-known�ֹ� */
   
    /* �٥��ȥ��ե����ȷ�UDP�ǡ�������ॽ���åȤ���� */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
   
    /* ���顼�ॷ���ʥ��ѥ����ʥ�ϥ�ɥ�Υ��å� */
    myAction.sa_handler = CatchAlarm;
    if (sigfillset(&myAction.sa_mask) < 0) /* �ϥ�ɥ���ǤϤ��٤Ƥ�֥�å� */
    DieWithError("sigfillset() failed");
    myAction.sa_flags = 0;
   
    if (sigaction(SIGALRM, &myAction, 0) < 0)
        DieWithError("sigaction() failed for SIGALRM");
   
    /* �����ФΥ��ɥ쥹��¤�Τ���� */
    memset(&echoServAddr, 0, sizeof(echoServAddr));  /* ��¤�Τ򥼥������ */

    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* �����Ф�IP���ɥ쥹 */
    echoServAddr.sin_port = htons(echoServPort);       /* �����ФΥݡ����ֹ� */
   
    /* ʸ����򥵡��Ф����� */
    if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *)
               &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
        DieWithError("sendto() sent a different number of bytes than expected");
   
    /* ��������� */
   
    fromSize = sizeof(fromAddr);
    alarm(TIMEOUT_SECS);        /* �����ॢ���Ȼ��֤����� */
    while ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0,
           (struct sockaddr *) &fromAddr, &fromSize)) < 0)
        if (errno == EINTR)     /* ���顼��ν�λ */
        {
            if (tries < MAXTRIES) /* tries�ϥ����ʥ�ϥ�ɥ���ǥ��󥯥���� */
            {
                printf("timed out, %d more tries...\n", MAXTRIES-tries);
                if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *)
                            &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
                    DieWithError("sendto() failed");
                alarm(TIMEOUT_SECS);
             }
             else
                 DieWithError("No Response");
         }
         else
             DieWithError("recvfrom() failed");
   
    /* recvfrom()����������������顢�����ॢ���Ȥ򥭥�󥻥� */
    alarm(0);
   
    /* �����ǡ�����NULLʸ���ǽ�ü������ */
    echoBuffer[respStringLen] = '\0';
    printf("Received: %s\n", echoBuffer);  /* �����ǡ�����ɽ�� */
    
     close(sock);
     exit(0);
}

void CatchAlarm(int ignored)    /* SIGALRM�Υϥ�ɥ� */
{
    tries += 1;
}
