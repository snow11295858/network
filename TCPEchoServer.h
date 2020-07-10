#include <stdio.h>      /* printf()��fprintf()��ɬ�� */
#include <sys/socket.h> /* socket()��bind()��connect()��ɬ�� */
#include <arpa/inet.h>  /* sockaddr_in��inet_ntoa()��ɬ�� */
#include <stdlib.h>     /* atoi()��ɬ�� */
#include <string.h>      /* memset()��ɬ�� */
#include <unistd.h>     /* close()��ɬ�� */

void DieWithError(char *errorMessage);  /* ���顼�����ؿ� */
void HandleTCPClient(int clntSocket);   /* TCP���饤����Ȥν����ؿ� */
int CreateTCPServerSocket(unsigned short port); /* TCP�����Х����åȤ���� */
int AcceptTCPConnection(int servSock);  /* TCP��³�׵������դ��� */
