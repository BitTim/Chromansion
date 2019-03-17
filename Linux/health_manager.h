#ifndef HEALTH_MANAGER_H
#define HEALTH_MANAGER_H

#include <vector>
#include "variables.h"

int amnt_dmg = 10;
int inv_max_duration = 25;
int inv_curr_duration = 0;

bool dmg_flag = false;

void check_damage(int head_tile, int foot_tile)
{
	dmg_flag = false;

	switch(head_tile)
	{
		case 9:
			dmg_flag = true;
			break;
		case 10:
			if(player_color == 1) dmg_flag = true;
			break;
		case 11:
			if(player_color == 2) dmg_flag = true;
			break;
		case 12:
			if(player_color == 3) dmg_flag = true;
			break;
	}
	
	switch(foot_tile)
	{
		case 9:
			dmg_flag = true;
			break;
		case 10:
			if(player_color == 1) dmg_flag = true;
			break;
		case 11:
			if(player_color == 2) dmg_flag = true;
			break;
		case 12:
			if(player_color == 3) dmg_flag = true;
			break;
	}

	if(dmg_flag && !player_invincible)
	{
		player_invincible = true;
		player_health -= amnt_dmg;
	}
}

#endif // HEALTH_MANAGER
