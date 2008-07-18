// Uwc3ng nvault functions

public NVAULT_Init()
{
	nvault_handle = nvault_open ( NVAULT_FILE );
	
	if ( nvault_handle == INVALID_HANDLE )
	{
		log_amx ( "Error while setting up NVAULT connection!" );
	}
}

public NVAULT_Save( id )
{
	if ( !NVAULT_Check() )
	{
		set_task( 0.3, "NVAULT_Init" );
		
		return PLUGIN_CONTINUE;
	}
	
	// Checks how to save the xps
	new szKeyDef[512];
	Save_GetKey( id, szKeyDef, charsmax( szKeyDef ) ); 
	
	// We add a prefix to the key
	new szKey[64];
	formatex( szKey, charsmax( szKey ), "%s-UWC3XP", szKeyDef );
	
	// We format the text for saving 
	static savetext[64];
	formatex( savetext, charsmax( savetext ), "%d", p_data[id][PLAYER_XP] );
	
	// kk. Save the data. Firs only the xps
	nvault_set ( nvault_handle, szKey, savetext );
	
	// We have to save the skills
	formatex( szKey, charsmax( szKey ), "%s-UWC3SKILLS", szKeyDef );
	
	// make the savetext for the skills ready
	static savetextskills[512], i;
	for ( i = 0; i < MAX_SKILLS; ++i )
	{
		// We need no clear skills to save
		if ( p_data_skill[id][i] > 0 )
		{
			// i = every skill, p_data_skill = the level from the skill
			formatex ( savetextskills, charsmax( savetextskills ), "%s%d %d ", savetextskills, i, p_data_skill[id][i] );
		}
	}
	
	// kk. Save the skills
	nvault_set ( nvault_handle, szKey, savetextskills );
	
	// We have to save the attribs
	formatex( szKey, charsmax( szKey ), "%s-UWC3ATTRIBS", szKeyDef );
	
	// make the savetext for the attribs ready
	static savetextattribs[512];
	for ( i = 0; i < MAX_ATTRIBS; ++i )
	{
		// We need no clear attribs
		if ( p_data_attrib[id][i] > MAX_ATTRIBS_STARTPOINTS )
		{
			// i = every attrib, p_data_attrib = the level from the attrib
			formatex ( savetextattribs, charsmax( savetextattribs ), "%s%d %d ", savetextattribs, i, p_data_attrib[id][i] );
		}
	}
	
	// kk. Save the skills
	nvault_set ( nvault_handle, szKey, savetextattribs );
	
	// We have to save the resists
	formatex( szKey, charsmax( szKey ), "%s-UWC3RESISTS", szKeyDef );
	
	// make the savetext for the resists ready
	static savetextresists[512];
	for ( i = 0; i < MAX_RESIST; ++i )
	{
		// We need no clear resist to save
		if ( p_data_resist[id][i] > 0 )
		{
			// i = every resist, p_data_resist = the level from the resist
			formatex ( savetextresists, charsmax( savetextresists ), "%s%d %d ", savetextresists, i, p_data_resist[id][i] );
		}
	}
	
	// kk. Save the skills
	nvault_set ( nvault_handle, szKey, savetextresists );
	
	return PLUGIN_CONTINUE;
}

public NVAULT_GetAllXP( id )
{
	// We get the right ID back
	if ( id >= TASK_LOADXP )
	{
		id -= TASK_LOADXP;
	}
	
	if ( !NVAULT_Check() )
	{
		// The connection is atm not available. We do it in few seconds again
		set_task( 0.3, "NVAULT_Init" );
		set_task( 0.5, "NVAULT_GetAllXP", ( TASK_LOADXP + id ) );
		
		return PLUGIN_CONTINUE;
	}
	
	// Checks how to get the xps
	new szKeyDef[64];
	Save_GetKey( id, szKeyDef, charsmax( szKeyDef ) );
	
	// We add a prefix to the key
	static szKey[64];
	formatex( szKey, charsmax( szKey ), "%s-UWC3XP", szKeyDef );
	
	// We get the data from the nvault file
	static gettext[512], time;
	nvault_lookup ( nvault_handle, szKey, gettext, charsmax( gettext ), time );
	
	// save the xps in the array
	p_data[id][PLAYER_XP] = str_to_num ( gettext );
	
	// Create the prefix for the skills
	static p_Prefix[64];
	formatex( p_Prefix, charsmax( p_Prefix ), "%s-UWC3SKILLS", szKeyDef );
	
	// Get the skills
	static iFoundLine;
	iFoundLine = nvault_lookup ( nvault_handle, p_Prefix, gettext, charsmax( gettext ), time );
	
	// Be sure that we have saved skills
	
	// Get the skill id's and the level's from the string
	static i, TempIDStr[64], TempLevelStr[64];
	for ( i = 0; i < MAX_SKILLS; ++i )
	{
		// We have nothing found
		if ( !iFoundLine )
		{
			break;
		}
		
		// We read every skill id and the level from the skill
		strbreak( gettext, TempIDStr, charsmax( TempIDStr ), gettext, charsmax( gettext ) );
		strbreak( gettext, TempLevelStr, charsmax( TempLevelStr ), gettext, charsmax( gettext ) );
		
		// Convert all and save it in the array
		p_data_skill[id][str_to_num( TempIDStr )] = str_to_num( TempLevelStr );
	}
	
	// Create the prefix for the attribs
	formatex( p_Prefix, charsmax( p_Prefix ), "%s-UWC3ATTRIBS", szKeyDef );
	
	// Get the attribs
	iFoundLine = nvault_lookup ( nvault_handle, p_Prefix, gettext, charsmax( gettext ), time );
	
	// Get the attribs id's and the level's from the string
	for ( i = 0; i < MAX_ATTRIBS; ++i )
	{
		// We have nothing found
		if ( !iFoundLine )
		{
			break;
		}
		
		// We read every attrib id and the level from the attrib
		strbreak( gettext, TempIDStr, charsmax( TempIDStr ), gettext, charsmax( gettext ) );
		strbreak( gettext, TempLevelStr, charsmax( TempLevelStr ), gettext, charsmax( gettext ) );
		
		// Convert all and save it in the array
		p_data_attrib[id][str_to_num( TempIDStr )] = str_to_num( TempLevelStr );
		
		// Sometimes we have here bugs...
		if ( p_data_attrib[id][str_to_num( TempIDStr )] == 0 )
		{
			p_data_attrib[id][str_to_num( TempIDStr )] = MAX_ATTRIBS_STARTPOINTS;
		}
	}
	
	// Create the prefix for the resists
	formatex( p_Prefix, charsmax( p_Prefix ), "%s-UWC3RESISTS", szKeyDef );
	
	// Get the resists
	iFoundLine = nvault_lookup ( nvault_handle, p_Prefix, gettext, charsmax( gettext ), time );
	
	// Get the resists id's and the level's from the string
	for ( i = 0; i < MAX_RESIST; ++i )
	{
		// We have nothing found
		if ( !iFoundLine )
		{
			break;
		}
		
		// We read every resist id and the level from the resist
		strbreak( gettext, TempIDStr, charsmax( TempIDStr ), gettext, charsmax( gettext ) );
		strbreak( gettext, TempLevelStr, charsmax( TempLevelStr ), gettext, charsmax( gettext ) );
		
		// Convert all and save it in the array
		p_data_resist[id][str_to_num( TempIDStr )] = str_to_num( TempLevelStr );
	}
	
	return PLUGIN_CONTINUE;
}

public NVAULT_CloseNow()
{
	if ( NVAULT_Check() )
	{
		nvault_close ( nvault_handle );
	}
}

bool:NVAULT_Check()
{
	if ( nvault_handle != INVALID_HANDLE )
	{
		return true;
	}
	
	return false;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
