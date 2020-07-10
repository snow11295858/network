#include <stdio.h>  /* printf()��ɬ�� */ 
#include <signal.h> /* sigaction()��ɬ�� */
#include <unistd.h> /* pause()��ɬ�� */

void DieWithError(char *errorMessage);       /* ���顼�����ؿ� */
void InterruptSignalHandler(int signalType); /* �����ߥ����ʥ�����ؿ� */

int main(int argc, char *argv[])
{
    struct sigaction handler;        /* �����ʥ�ϥ�ɥ����ꤹ�빽¤�� */

    /* InterruptSignalHandler()��ϥ�ɥ�ؿ��Ȥ������� */
    handler.sa_handler = InterruptSignalHandler;
    /* �������ʥ��ޥ�������ޥ�������� */
    if (sigfillset(&handler.sa_mask) < 0)
        DieWithError("sigfillset() failed");
    /* �ե饰�ʤ� */
    handler.sa_flags = 0;

    /* �����ߥ����ʥ���Ф������������ */
    if (sigaction(SIGINT, &handler, 0) < 0)
        DieWithError("sigaction() failed");

    for(;;)
        pause(); /* �����ʥ��������ޤǥץ���������� */

     exit(0);
}

void InterruptSignalHandler(int signalType)
{
      printf("Interrupt Received.  Exiting program.\n");
      exit(1);
}
