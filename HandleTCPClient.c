#include <stdio.h>  /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* recv()��send()��ɬ�� */
#include <unistd.h> /* close()��ɬ�� */

#define RCVBUFSIZE 32 /* �����Хåե��Υ����� */

void DieWithError(char *errorMessage);  /* ���顼�����ؿ� */

void HandleTCPClient(int clntSocket)
{
  char echoBuffer[RCVBUFSIZE];  /* ������ʸ����ΥХåե� */
  int recvMsgSize;  /* ������å������Υ����� */

  /* ���饤����Ȥ���μ�����å����� */
  if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
    DieWithError("recv() failed");

  /* ��������ʸ�������������ž������λ���Ƥ��ʤ���м���������� */
  while (recvMsgSize > 0) /* �����ž���ν�λ���̣���� */
  {
    /* ��å������򥯥饤����Ȥ˥������Хå����� */
    if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
      DieWithError("send() failed");

    /* ��������ǡ������ĤäƤ��ʤ�����ǧ���� */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
      DieWithError("recv() failed");
  }

  close(clntSocket);  /* ���饤����ȥ����åȤ򥯥������� */
}
