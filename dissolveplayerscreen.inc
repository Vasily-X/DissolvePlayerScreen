#define CUSTOM_TAG_TYPES						F@_@i

#include <indirection>
#include <YSI\y_utils>
#include <YSI\y_timers>
#include <YSI\y_hooks>

#define FADE_STEPS								15
#define NO_FADE_RESPONSE						Func:0<i>
#define FadePlayerScreen(%0,%1,%2,&%3)			FadePlayerScreen(%0,%1,%2,addressof(%3<i>))

static fade_step[MAX_PLAYERS];
static fade_init_color[MAX_PLAYERS];
static fade_end_color[MAX_PLAYERS];

static Timer:fade_screen_timer[MAX_PLAYERS]   =	{Timer:-1, ...};
static PlayerText:fade_screen_td[MAX_PLAYERS] =	{PlayerText:INVALID_TEXT_DRAW, ...};

hook OnPlayerDisconnect(playerid, reason)
{
	ClearFadeScreenData(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

ClearFadeScreenData(playerid)
{
	fade_step[playerid] = 0;
	fade_init_color[playerid] = 0;
	fade_end_color[playerid] = 0;

	if(fade_screen_timer[playerid] != Timer:-1)
	{
		stop fade_screen_timer[playerid];
		fade_screen_timer[playerid] = Timer:-1;
	}

	if(fade_screen_td[playerid] != PlayerText:INVALID_TEXT_DRAW)
	{
		PlayerTextDrawDestroy(playerid, fade_screen_td[playerid]);
		fade_screen_td[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}
}

stock FadePlayerScreen(playerid, init_color, end_color, Func:response_func<i> = NO_FADE_RESPONSE)
{
	ClearFadeScreenData(playerid);

	fade_screen_td[playerid] = CreatePlayerTextDraw(playerid, 0.0, 0.0, "_");
	PlayerTextDrawTextSize(playerid, fade_screen_td[playerid], 640.0, 480.0);
	PlayerTextDrawLetterSize(playerid, fade_screen_td[playerid], 0.0, 50.0);
	PlayerTextDrawUseBox(playerid, fade_screen_td[playerid], 1);
	PlayerTextDrawBoxColor(playerid, fade_screen_td[playerid], init_color);
	PlayerTextDrawShow(playerid, fade_screen_td[playerid]);

	fade_init_color[playerid] = init_color;
	fade_end_color[playerid] = end_color;

	if(response_func != NO_FADE_RESPONSE)
	{
		Indirect_Claim(response_func);
	}

	fade_screen_timer[playerid] = repeat update_fade_screen_td(playerid, response_func);
}

stock FadePlayerScreenIn(playerid, Func:response_func<i> = NO_FADE_RESPONSE)
{
	FadePlayerScreen(playerid, 0x000000FF, 0x00000000, response_func);
}

stock FadePlayerScreenOut(playerid, Func:response_func<i> = NO_FADE_RESPONSE)
{
	FadePlayerScreen(playerid, 0x00000000, 0x000000FF, response_func);
}

timer update_fade_screen_td[70](playerid, Func:response_func<i>)
{
	fade_step[playerid]++;

	PlayerTextDrawBoxColor(playerid, fade_screen_td[playerid],
		InterpolateColour(fade_init_color[playerid], fade_end_color[playerid], fade_step[playerid], FADE_STEPS)
	);

	PlayerTextDrawShow(playerid, fade_screen_td[playerid]);

	if(fade_step[playerid] == FADE_STEPS)
	{
		stop fade_screen_timer[playerid];
		fade_screen_timer[playerid] = Timer:-1;

		if(response_func != NO_FADE_RESPONSE)
		{
			@.response_func(playerid);
			Indirect_Release(response_func);
		}
	}
}
