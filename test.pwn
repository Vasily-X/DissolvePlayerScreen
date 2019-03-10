#include <a_samp>

#include <dissolveplayerscreen>

#include <YSI\y_colours>
#include <YSI\y_commands>
#include <YSI\y_hooks>

main()
{
}

hook OnGameModeInit()
{
	AddPlayerClass(0, 0.0, 0.0, 6.0, 0.0, 0, 0, 0, 0, 0, 0);
	return 1;
}

hook OnPlayerConnect(playerid)
{
	SendClientMessage(playerid, -1, "Transition | Blank -> Black");
    DissolvePlayerScreenToBlack(playerid, &Fade_OnPlayerConnect);
    return Y_HOOKS_CONTINUE_RETURN_1;
}

Fade_OnPlayerConnect(playerid)
{
    SendClientMessage(playerid, -1, "Transition | Black - Blank");
    DissolvePlayerScreenToBlank(playerid, &ReturnToBlank);
}

ReturnToBlank(playerid)
{
    SendClientMessage(playerid, -1, "Transition complete.");
}

YCMD:fade1(playerid, params[], help)
{
	DissolvePlayerScreen(playerid, Y_RED | 0xFF, Y_ORANGE | 0xFF);
	return 1;
}

YCMD:fade2(playerid, params[], help)
{
	DissolvePlayerScreen(playerid, Y_ORANGE | 0xFF, Y_SKYBLUE | 0xFF);
	return 1;
}

YCMD:fade3(playerid, params[], help)
{
	DissolvePlayerScreen(playerid, Y_SKYBLUE | 0xFF, Y_BLACK | 0x00);
	return 1;
}