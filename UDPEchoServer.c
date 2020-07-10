#include <stdio.h> /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��bind()��ɬ�� */
#include <arpa/inet.h> /* sockaddr_in��inet_ntoa()��ɬ�� */
#include <stdlib.h> /* atoi()��ɬ�� */
#include <string.h> /* memset()��ɬ�� */
#include <unistd.h> /* close()��ɬ�� */

#define ECHOMAX 255 /* ������ʸ����κ���Ĺ */

void DieWithError(char *errorMessage); /* �������顼�����ؿ� */

int main(int argc, char *argv[])
{
  int sock; /* �����å� */
  struct sockaddr_in echoServAddr; /* �����륢�ɥ쥹 */
  struct sockaddr_in echoClntAddr; /* ���饤����ȥ��ɥ쥹 */
  unsigned int cliAddrLen; /* �忮��å�������Ĺ�� */
  char echoBuffer[ECHOMAX]; /* ������ʸ�����ѥХåե� */
  unsigned short echoServPort; /* �����ФΥݡ����ֹ� */
  int recvMsgSize; /* ������å������Υ����� */

  if (argc != 2) /* �ѥ�᡼���ο�������������ǧ */
  {
    fprintf(stderr,"Usage: %s <UDP SERVER PORT>\n", argv[0]);
    exit(1);
  }

  echoServPort = atoi(argv[1]); /* 1���ܤΰ�����������ݡ����ֹ� */

  /* �ǡ����������������˻Ȥ������åȤ���� */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");

  /* ������Υ��ɥ쥹��¤�Τ���� */
  memset(&echoServAddr, 0, sizeof(echoServAddr)); /* ��¤�Τ˥�������� */
  echoServAddr.sin_family = AF_INET; /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* �磻��ɥ����ɤ���� */
  echoServAddr.sin_port = htons(echoServPort); /* ������ݡ��� */

  /* �����륢�ɥ쥹�إХ���� */
  if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    DieWithError("bind() failed");

  for (;;) /* �ץ���ब��λ����ޤǷ����֤��¹� */
  {
    /* �����ϥѥ�᡼���Υ������򥻥å� */
    cliAddrLen = sizeof(echoClntAddr);

    /* ���饤����Ȥ����å��������������ޤǥ֥�å�����  */
    if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,

      (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
      DieWithError("recvfrom() failed");

    printf("Handling client %s\n", inet_ntoa(echoClntAddr. sin_addr));

    /* ���������ǡ��������򥯥饤����Ȥ��ֿ����� */
    if (sendto(sock, echoBuffer, recvMsgSize, 0,
      (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
      DieWithError("sendto() sent a different number of bytes than expected");
  }
  /* ������ʬ�ˤ���ã���ʤ�*/
}
