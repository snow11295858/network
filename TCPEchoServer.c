#include <stdio.h>  /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��bind()��connect()��ɬ�� */
#include <arpa/inet.h> /* sockaddr_in��inet_ntoa()��ɬ�� */
#include <stdlib.h> /* atoi()��ɬ�� */
#include <string.h> /* memset()��ɬ�� */
#include <unistd.h> /* close()��ɬ�� */

#define MAXPENDING 5 /* Ʊ���˥��塼��ǽ����³�׵�κ���� */
#define RCVBUFSIZE 32 /* �����Хåե��Υ����� */

void DieWithError(char *errorMessage); /* ���顼�����ؿ� */
void HandleTCPClient(int clntSocket); /* TCP���饤����Ƚ����ؿ� */

int main(int argc, char *argv[])
{
  int servSock; /* �����ФΥ����åȥǥ�������ץ� */
  int clntSock; /* ���饤����ȤΥ����åȥǥ�������ץ� */
  struct sockaddr_in echoServAddr;  /* �����륢�ɥ쥹 */
  struct sockaddr_in echoClntAddr;  /* ���饤����ȥ��ɥ쥹 */
  unsigned short echoServPort;  /* �����Хݡ��� */
  unsigned int clntLen; /* ���饤����ȤΥ��ɥ쥹��¤�Τ�Ĺ�� */

  if (argc != 2)  /* �����ο�������������ǧ */
  {
    fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
    exit(1);
  }

  echoServPort = atoi(argv[1]); /* 1���ܤΰ�����������ݡ��� */

  /* �忮��³�ѤΥ����åȤ���� */
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  /* ������Υ��ɥ쥹��¤�Τ���� */
  memset(&echoServAddr, 0, sizeof(echoServAddr)); /* ��¤�Τ򥼥������ */
  echoServAddr.sin_family = AF_INET;  /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* �磻��ɥ����ɤ���� */
  echoServAddr.sin_port = htons(echoServPort);  /* ������ݡ��� */


  /* �����륢�ɥ쥹�إХ���� */
  if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    DieWithError("bind() failed");

  /* �忮��³�׵�Υꥹ����֤Ȥ����ޡ����򥽥��åȤ��դ��� */
  if (listen(servSock, MAXPENDING) < 0)
    DieWithError("listen() failed");

  for (;;)  /* �ʱ�˼¹� */
  {
    /* �����ϥѥ�᡼���Υ������򥻥å� */
    clntLen = sizeof(echoClntAddr);
    printf("%d", 1);
    /* ���饤����Ȥ������³�׵���Ե� */
    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
                 &clntLen)) < 0)
      DieWithError("accept() failed");

    /* clntSock�ϥ��饤����Ȥ���³�Ѥ� */

    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    HandleTCPClient(clntSock);
  }
  /* ������ʬ�ˤ���ã���ʤ� */
}
