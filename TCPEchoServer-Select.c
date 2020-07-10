#include "TCPEchoServer.h"  /* TCP�����������ФΥ��󥯥롼�ɥե����� */
#include <sys/time.h>        /* timeval{}��ɬ�� */
#include <fcntl.h>          /* fcntl()��ɬ�� */

int main(int argc, char *argv[])
{
     int *servSock;                   /* �����ФΥ����åȥǥ�������ץ� */
     int maxDescriptor;               /* �����åȥǥ�������ץ��κ����� */
     fd_set sockSet;                  /* select()���оݤȤʤ륽���åȥǥ�������ץ��Υ��å� */
     long timeout;                    /* ���ޥ�ɥ饤�󤫤���ꤵ�줿�����ॢ���Ȥ��� */
     struct timeval selTimeout;       /* select()�Υ����ॢ���� */
     int running = 1;                 /* �����Ф�ư����Ǥ����1�������Ǥʤ����0 */
     int noPorts;                     /* ���ޥ�ɥ饤�󤫤���ꤷ���ݡ��Ȥο� */
     int port;                        /* �ݡ��Ȥ��������롼�פ��ѿ� */
     unsigned short portNo;           /* �ºݤΥݡ����ֹ� */

     if (argc < 3)      /* �����ο�����������Ĵ�٤� */
     {
         fprintf (stderr, "Usage:  %s <Timeout (secs.)> <Port 1> ... \n", argv[0]);
         exit(1);
     }

     timeout = atol(argv[1]);        /* 1���ܤΰ����������ॢ���ȡ��á� */
     noPorts = argc - 2;             /* �ݡ��Ȥο��ϡ������Υ�����ȡڴ����� argc���ͤǤ��ۤ���2����� */

     /* �忮��³�׵������륽���åȥꥹ�Ȥγ������ */
     servSock = (int *) malloc(noPorts * sizeof(int));

     /* select()�ǻ��Ѥ���maxDescriptor�ν���� */
     maxDescriptor = -1;

     /* �ݡ��ȡ�����ӥݡ��Ȥ�������륽���åȤΥꥹ�Ȥ���� */
     for (port = 0; port < noPorts; port++)
     {
         /* �ݡ��Ȥ�ꥹ�Ȥ��ɲ� */
         portNo = atoi(argv[port + 2]); /*�ǽ��2�Ĥΰ����򥹥��å� */

         /* �ݡ��ȤΥ����åȤ���� */
         servSock[port] = CreateTCPServerSocket(portNo);

         /* �������ǥ�������ץ����ͤ����礫Ĵ�٤� */
         if (servSock[port] > maxDescriptor)
            maxDescriptor = servSock[port];
     }

     printf("Starting server: Hit return to shutdown\n");
     while (running)
     {
         /* �����åȥǥ�������ץ��Υ٥����򥼥��������ƥ����ФΥ����åȤ򥻥åȤ��� */
         /* select()��¹Ԥ��뤿�Ӥ˥ꥻ�åȤ�ɬ�� */
         FD_ZERO(&sockSet);
         /* �ǥ�������ץ��Υ٥����˥����ܡ��ɤ��ɲ� */
         FD_SET(STDIN_FILENO, &sockSet);
         for (port = 0; port < noPorts; port++)
             FD_SET(servSock[port], &sockSet);

         /* �����ॢ���Ȥλ��� */
         /* select()��¹Ԥ��뤿�Ӥ˥ꥻ�åȤ�ɬ�� */
         selTimeout.tv_sec = timeout;       /* �����ॢ���ȡ��á� */
         selTimeout.tv_usec = 0;            /* 0�ޥ�������*/

         /* �ǥ�������ץ������Ѳ�ǽ���ޤ��ϥ����ॢ���Ȥˤʤ�ޤǥץ����������� */
         if (select(maxDescriptor + 1, &sockSet, NULL, NULL, &selTimeout) == 0)
             printf("No echo requests for %ld secs... Server still alive\n", timeout);
         else
         {
             if (FD_ISSET(STDIN_FILENO, &sockSet)) /* �����ܡ��ɤΥ����å� */
             {
                 printf("Shutting down server\n");
                 getchar();
                 running = 0;
             }

         for (port = 0; port < noPorts; port++)
             if (FD_ISSET(servSock[port], &sockSet))
             {
                 printf("Request on port %d: ", port);
                 HandleTCPClient(AcceptTCPConnection(servSock[port]));
             }
       }
     }

     /* �����åȤΥ����� */
     for (port = 0; port < noPorts; port++)
         close(servSock[port]);

     /* �����åȤΥꥹ�Ȥ���꤫����� */
     free(servSock);

     exit(0);
 }

