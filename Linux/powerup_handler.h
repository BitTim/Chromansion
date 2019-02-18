#ifndef POWERUP_HANDLER_H
#define POWERUP_HANDLER_H

#include <vector>
#include "variables.h"

struct pup
{
	int x;
	int y;
	int type;
	bool visible;
};

int check_collect(float player_pos_raster[2], std::vector<pup> &pup_list)
{
	for(pup &pow : pup_list)
	{
		if(((int)player_pos_raster[0] == pow.x) && ((int)player_pos_raster[1] == pow.y || (int)player_pos_raster[1] + 1 == pow.y) && pow.visible)
		{
			pow.visible = false;
			return pow.type;
		}
	}

	return 0;
}

#endif // POWERUP_HANDLER_H
