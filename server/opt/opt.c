#include <stddef.h>

#include "opt.h"
#include "cli.c"
#include "cfg.c"
#include "def.c"

/**
 * Получить опции
 */
struct opt opt (int argc, char ** argv)
{
	struct opt o_cli = opt_cli (argc, argv);
	struct opt o_cfg = opt_cfg ();
	struct opt o_def = opt_def ();

	struct opt o;

	return o;
}


