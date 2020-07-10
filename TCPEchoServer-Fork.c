#include "TCPEchoServer.h"  /* TCP�����������ФΥإå��ե�����򥤥󥯥롼�� */
#include <sys/wait.h>       /* waitpid()��ɬ�� */

int main(int argc, char *argv[])
{
    int servSock;                     /* �����ФΥ����åȥǥ�������ץ� */
    int clntSock;                     /* ���饤����ȤΥ����åȥǥ�������ץ� */
    unsigned short echoServPort;      /* �����ФΥݡ����ֹ� */
    pid_t processID;                  /* fork()���֤��ץ���ID */
  unsigned int childProcCount = 0;   /* �ҥץ����ο� */

  if (argc != 2)    /* �����ο�������������ǧ */
  {
      fprintf(stderr, "Usage: %s <Server Port>", argv[0]);
      exit(1);
  }

  echoServPort = atoi(argv[1]); /* 1���ܤΰ����� ������ݡ��� */

  servSock = CreateTCPServerSocket(echoServPort);

  for (;;) /* �����򷫤��֤��¹� */
  {
      clntSock = AcceptTCPConnection(servSock);
      /* �ҥץ����Υե������ȥ��顼����� */
     if ((processID = fork()) < 0)
         DieWithError("fork() failed");
     else if (processID == 0) /* �ҥץ����ξ�� */
    {
         close(servSock);   /* �Ԥ�������Υ����åȤ�ҥץ����������� */
         HandleTCPClient(clntSock);

         exit(0);           /* �ҥץ�����λ */
    }

    printf("with child process: %d\n", (int) processID);
    close(clntSock);       /* �ҤΥ����åȥǥ�������ץ���Ƥ������� */
    childProcCount++;      /* ̤����λҥץ����ο��򥤥󥯥���� */

    while (childProcCount) /* ������Ӥ򥯥꡼�󥢥å� */
    {
        processID = waitpid((pid_t) -1, NULL, WNOHANG); /* �Υ�֥�å��󥰤Ǽ¹� */
        if (processID < 0) /* waitpid()�Υ��顼���ǧ */
            DieWithError("waitpid() failed");
        else if (processID == 0)  /* ����Ӥ�¸�ߤ��ʤ� */
            break;
        else
            childProcCount--;  /* �ҥץ������� */
    }
  }
  /* ������ʬ�ˤ���ã���ʤ� */
}
