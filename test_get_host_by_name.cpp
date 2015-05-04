#include <netdb.h>
#include <stdio.h>

int main()
{
    struct hostent *ent = gethostbyname("www.baidu.com");

    printf("hostent.h_name = %s\n", ent->h_name);

    for (char **p = ent->h_aliases; *p != NULL; ++p)
    {
        printf("hostent.h_aliases = %s\n", *p);
    }

    printf("hostent.h_type = %d\n", ent->h_addrtype);

    printf("hostent.h_length = %d\n", ent->h_length);

    for (char **p = ent->h_addr_list; *p != NULL; ++p)
    {
        printf("%x\n", *(int *)(*p));
    }

    return 0;
}
