#include <stdio.h> /* printf()��fprintf()��ɬ�� */
#include <netinet/in.h> /* htons()��ɬ�� */
#include <netdb.h> /* getservbyname()��ɬ�� */
#include <stdlib.h> /* atoi()��ɬ�� */

unsigned short ResolveService(char service[], char protocol[])
{
  struct servent *serv; /* �����ӥ�������Ǽ���빽¤�� */
  unsigned short port; /* ����ͤȤʤ�ݡ��� */

  if ((port = atoi(service)) == 0) /* �ݡ��ȤϿ��ͤ��� */
  {
    /* ���ͤǤϤʤ���̾���Ȳ�ᤷ�Ƹ������Ƥߤ� */
    if ((serv = getservbyname(service, protocol)) == NULL)
    {
      fprintf(stderr, "getservbyname() failed");
      exit(1);
    }
    else
      port = serv->s_port; /* ̾������ݡ��ȡʥͥåȥ���Х��Ƚ�ˤ����Ĥ��� */
  }
  else
    port = htons(port); /* port��ͥåȥ���Х��Ƚ���Ѵ����� */

  return port;
}
