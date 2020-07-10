#include <stdio.h> /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��connect()��sendto()��recvfrom()��ɬ�� */
#include <arpa/inet.h> /* sockaddr_in��inet_addr()��ɬ�� */
#include <stdlib.h> /* atoi()��ɬ�� */
#include <string.h> /* memset()��ɬ�� */
#include <unistd.h> /* for close()��ɬ�� */

#define ECHOMAX 255 /* ������ʸ����κ���Ĺ */

void DieWithError(char *errorMessage); /* �������顼�����ؿ� */

int main(int argc, char *argv[])
{
  int sock; /* �����åȥǥ�������ץ� */
  struct sockaddr_in echoServAddr; /* �����������ФΥ��ɥ쥹 */
  struct sockaddr_in fromAddr; /* �������������Υ��ɥ쥹 */
  unsigned short echoServPort; /* �����������ФΥݡ����ֹ� */
  unsigned int fromSize; /* recvfrom()�Υ��ɥ쥹�������ϥ����� */
  char *servIP; /* �����Ф�IP���ɥ쥹 */
  char *echoString; /* �����������Ф���������ʸ���� */
  char echoBuffer[ECHOMAX+1]; /* ������ʸ����μ����ѥХåե� */
  int echoStringLen; /* ������ʸ�����Ĺ�� */
  int respStringLen; /* ��������������Ĺ�� */

  if ((argc < 3) || (argc > 4)) /* �����ο�������������ǧ */
  {
    fprintf(stderr,"Usage: %s <Server IP> <Echo Word> [<Echo Fort>]\n", argv[0]);
    exit(1);
  }

  servIP = argv[1]; /* 1���ܤΰ����������Ф�IP���ɥ쥹��dotted quad��*/
  echoString = argv[2]; /* 2���ܤΰ�����������ʸ���� */

  if ((echoStringLen = strlen(echoString)) > ECHOMAX) /* ���Ϥ���ʸ�����Ĺ��������å� */
    DieWithError("Echo word too long");

  if (argc == 4)
    echoServPort = atoi(argv[3]); /* ����Υݡ����ֹ椬����л��� */
  else
    echoServPort = 7; /* 7�ϥ����������ӥ���well-known�ݡ����ֹ� */

  /* UDP�ǡ�������ॽ���åȤκ��� */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");

  /* �����ФΥ��ɥ쥹��¤�Τκ��� */
  memset(&echoServAddr, 0, sizeof(echoServAddr)); /* ��¤�Τ˥�������� */
  echoServAddr.sin_family = AF_INET; /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
  echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* �����Ф�IP���ɥ쥹 */
  echoServAddr.sin_port = htons(echoServPort); /* �����ФΥݡ����ֹ� */

  /* ʸ����򥵡��Ф����� */
  if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *)
         &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
    DieWithError("sendto() sent a different   number of bytes than expected");

  /* ��������� */
  fromSize = sizeof(fromAddr);
  if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0,
    (struct sockaddr *) &fromAddr, &fromSize)) != echoStringLen)
    DieWithError("recvfrom() failed");

  if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
  {
    fprintf(stderr,"Error: received a packet from unknown source.\n");
    exit(1);
  }

  /* �����ǡ�����NULLʸ���ǽ�ü������ */
  echoBuffer[respStringLen] = '\0';
  printf("Received: %s\n", echoBuffer); /* �����Υ�����ʸ�����ɽ�� */

  close(sock);
  exit(0);
}
