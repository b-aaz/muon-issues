#if ! defined(LEGOLAS_H)
# define LEGOLAS_H

# if defined(LEGOLAS_HEADER)
void legolas(char *);
# elif defined(LEGOLAS_IMPLEMENTATION)
#  include<stdio.h>
void legolas(char * to)
{
	printf("They're taking the hobbits to %s\n", to);
}
# else
#  error Please specify the implementation for this header.
# endif

#endif 
