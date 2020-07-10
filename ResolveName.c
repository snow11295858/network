#include <stdio.h> /* fprintf()��ɬ�� */
#include <netdb.h> /* gethostbyname()��ɬ�� */

unsigned long ResolveName(char name[])
{
  struct hostent *host; /* �ۥ��Ⱦ�����Ǽ���빽¤�� */

  if ((host = gethostbyname(name)) == NULL)
  {
    fprintf(stderr, "gethostbyname() failed");
    exit(1);
  }

  /* �ͥåȥ���Х��Ƚ��ɽ�����Х��ʥ��IP���ɥ쥹���֤� */
  return *((unsigned long *)host->h_addr_list[0]);
}
