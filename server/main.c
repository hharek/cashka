#include <stdio.h>

#include "inc/cashka.h"
#include "inc/err.h"
#include "opt/opt.h"

int main (int argc, char ** argv)
{
	struct opt o;

	int result = opt (argc, argv, &o);
	if (result != 0)
	{
		fprintf (stderr, err_get ()->message);
		return 1;
	}

    return 0;
}