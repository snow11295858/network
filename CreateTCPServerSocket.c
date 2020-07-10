#include <sys/socket.h> /* socket()��bind()��connect()��ɬ�� */
#include <arpa/inet.h>  /* sockaddr_in and inet_ntoa()��ɬ�� */
#include <string.h>     /* memset()��ɬ�� */

#define MAXPENDING 5    /* ̤��������³�׵�κ����� */

void DieWithError(char *errorMessage); /* ���顼�����ؿ� */

int CreateTCPServerSocket(unsigned short port)
{
    int sock;                        /* �������륽���å� */
    struct sockaddr_in echoServAddr; /* �����륢�ɥ쥹 */

    /* �忮��³�׵���Ф��륽���åȤ���� */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* ������Υ��ɥ쥹��¤�Τ���� */
    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* ��¤�Τ򥼥������ */
    echoServAddr.sin_family = AF_INET;                /* ���󥿡��ͥåȥ��ɥ쥹�ե��ߥ� */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* �磻��ɥ����ɤ���� */
    echoServAddr.sin_port = htons(port);              /* ������ݡ��� */

    /* �����륢�ɥ쥹�إХ���� */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    /* �忮��³�׵�Υꥹ����Ȥ����ޡ����򥽥��åȤ��դ��� */
    if (listen(sock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    return sock;
}
