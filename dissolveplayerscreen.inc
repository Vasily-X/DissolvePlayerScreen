#define CUSTOM_TAG_TYPES						F@_@i

#include <indirection>
#include <YSI\y_utils>
#include <YSI\y_timers>
#include <YSI\y_hooks>

#define DISSOLVE_STEPS							15
#define NO_TRANSITION_RESPONSE					Func:0<i>

#define DissolvePlayerScreen(%0,%1,%2,&%3)		DissolvePlayerScreen(%0,%1,%2,addressof(%3<i>))
#define DissolvePlayerScreenToBlack(%0,&%1)		DissolvePlayerScreenToBlack(%0,addressof(%1<i>))		
#define DissolvePlayerScreenToBlank(%0,&%1)		DissolvePlayerScreenToBlank(%0,addressof(%1<i>))		

static d_step[MAX_PLAYERS char];
static d_init_color[MAX_PLAYERS char];
static d_end_color[MAX_PLAYERS char];

static Timer:d_screen_timer[MAX_PLAYERS] 	= 	{Timer:-1, ...};
static PlayerText:d_screen_td[MAX_PLAYERS] 	= 	{PlayerText:INVALID_TEXT_DRAW, ...};

hook OnPlayerDisconnect(playerid, reason)
{
	ClearScreenDissolveData(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

ClearScreenDissolveData(playerid)
{
	d_step{playerid} = 0;
	d_init_color{playerid} = 0;
	d_end_color{playerid} = 0;

	if(d_screen_timer[playerid] != Timer:-1)
	{
		stop d_screen_timer[playerid];
		d_screen_timer[playerid] = Timer:-1;
	}

	if(d_screen_td[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, d_screen_td[playerid]);
		d_screen_td[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
}

stock DissolvePlayerScreen(playerid, init_color, end_color, Func:response_func<i> = NO_TRANSITION_RESPONSE)
{
	ClearScreenDissolveData(playerid);

	d_screen_td[playerid] = CreatePlayerTextDraw(playerid, 0.0, 0.0, "_");
	PlayerTextDrawTextSize(playerid, d_screen_td[playerid], 640.0, 480.0);
	PlayerTextDrawLetterSize(playerid, d_screen_td[playerid], 0.0, 50.0);
	PlayerTextDrawUseBox(playerid, d_screen_td[playerid], 1);
	PlayerTextDrawBoxColor(playerid, d_screen_td[playerid], init_color);
	PlayerTextDrawShow(playerid, d_screen_td[playerid]);

	d_init_color{playerid} = init_color;
	d_end_color{playerid} = end_color;

	if(response_func != NO_TRANSITION_RESPONSE)
	{
		Indirect_Claim(response_func);
	}

	d_screen_timer[playerid] = repeat update_d_screen_td(playerid, response_func);
}

stock DissolvePlayerScreenToBlank(playerid, Func:response_func<i> = NO_TRANSITION_RESPONSE)
{
	DissolvePlayerScreen(playerid, 0x000000FF, 0x00000000, response_func);
}

stock DissolvePlayerScreenToBlack(playerid, Func:response_func<i> = NO_TRANSITION_RESPONSE)
{
	DissolvePlayerScreen(playerid, 0x00000000, 0x000000FF, response_func);
}

stock IsPlayerScreenDissolving(playerid)
{
	if(d_step{playerid} > 0)
	{
		return 1;
	}
	return 0;
}

timer update_d_screen_td[70](playerid, Func:response_func<i>)
{
	d_step{playerid}++;

	PlayerTextDrawBoxColor(playerid, d_screen_td[playerid],
		InterpolateColour(d_init_color{playerid}, d_end_color{playerid}, d_step{playerid}, DISSOLVE_STEPS)
	);

	PlayerTextDrawShow(playerid, d_screen_td[playerid]);

	if(d_step{playerid} == DISSOLVE_STEPS)
	{
		d_step{playerid} = 0;

		stop d_screen_timer[playerid];
		d_screen_timer[playerid] = Timer:-1;

		if(response_func != NO_TRANSITION_RESPONSE)
		{
			@.response_func(playerid);
			Indirect_Release(response_func);
		}
	}
}
