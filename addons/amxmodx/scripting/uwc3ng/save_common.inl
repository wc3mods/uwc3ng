// Uwc3ng Common save functions

public DetermineType()
{

	new iSaveType = get_pcvar_num( CVAR_uwc3ng_savexp );

	// See if we can save with MySQL
	if ( iSaveType == DB_MYSQLX )
	{
	
		// Then we can't save using mysql
		if ( !module_exists( "sqlx" ) )
		{
			log_amx( "[WARNING] Unable to saving using MySQL, please enable the mysql module" );
			log_amx( "[WARNING] Saving (possibly) will be done by using SQLite" );
		}

		// OK we can save!
		else
		{
			// We save and load now only with mysqlx
			g_SaveType = DB_MYSQLX;

			// Make sure affinity is set correctly!
			SQL_SetAffinity( "mysql" );
		}
	}

	// See if we can save with SQLite if MySQLX didn't work
	if ( iSaveType == DB_SQLITE )
	{
		
		// Then we can't save w/this!!!
		if ( !module_exists( "sqlx" ) )
		{
			log_amx( "[WARNING] Unable to saving using SQLite, please enable the sqlite module" );
			log_amx( "[WARNING] Saving (possibly) will be done by using Nvault" );
		}
		
		// OK we can save
		else
		{
			// We're using SQLite
			g_SaveType = DB_SQLITE;

			// Make sure affinity is set correctly!
			SQL_SetAffinity( "sqlite" );
		}
	}
	
	// We can't save with MySQLX and SQLite. So we try it with Nvault 
	if ( g_SaveType == -1 )
	{
		// We can't save with nvault
		if ( !module_exists( "nvault" ) )
		{
			log_amx( "[WARNING] Unable to saving using Nvault, please enable the nvault module" );
		}
		
		// Ok we can
		else
		{
			// We're using nvault
			g_SaveType = NVAULT;
		}
	}
	
	// Well. Something is really wrong. We can't save xp
	if ( g_SaveType == -1 )
	{
		set_pcvar_num( CVAR_uwc3ng_savexp, 0 );

		log_amx( "[ERROR] Unable to save XP, please enable SQLite, MySQL X  or Nvault (mysql requires additional configuration)" );
	}
	
	return;
}

// Here we start the connections to the dbs or the nvault file
public Save_Init()
{
	// Then we don't want to save XP
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		return;
	}
	
	// We should only need to call this once
	if ( g_SaveType == -1 )
	{
		DetermineType();
	}	

	// Initiate our database or nvault file
	switch( g_SaveType )
	{
		case DB_MYSQLX:	MYSQLX_Init();
		case DB_SQLITE:	SQLITE_Init();
		case NVAULT: NVAULT_Init();
	}

	return;
}

// Close the database connection(s) or nvault connection
public Save_Close()
{
	// Close our connections
	switch( g_SaveType )
	{
		case DB_MYSQLX:	MYSQLX_Close();
		case DB_SQLITE:	SQLITE_Close();
		case NVAULT: NVAULT_CloseNow();
	}

	return;
}

// Save the user's XP
public Player_SaveXP( id, bool:threaded )
{
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		return;
	}

	if ( !UWC3NG_Check() )
	{
		return;
	}

	// Allow bot's XP to be saved if we're saving by name
	if ( is_user_bot( id ) && get_pcvar_num( CVAR_uwc3ng_save_by ) != SAVEBY_NAME )
	{
		return;
	}
	
	// Save the xps from the player
	if ( threaded )
	{
		switch( g_SaveType )
		{
			case DB_MYSQLX:	MYSQLX_Save_T( id );
			case DB_SQLITE:	SQLITE_Save( id );
			case NVAULT: NVAULT_Save( id );
		}
	}
	
	// Should not threaded
	else
	{
		switch( g_SaveType )
		{
			case DB_MYSQLX: MYSQLX_Save( id );
			case DB_SQLITE:	SQLITE_Save( id );
			case NVAULT: NVAULT_Save( id );
		}
	}
	
	// print the player a message 
	client_print( id, print_chat, "%s Your data is saved now!", MOD_NAME );
	
	return;
}

// This function will save the XP for all players
public Player_SaveAll()
{
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) )
	{
		return;
	}

	new players[32], numofplayers, i;
	get_players( players, numofplayers );

	for ( i = 0; i < numofplayers; ++i )
	{
		Player_SaveXP( players[i], false );
	}

	return;
}

// Function will return the appropriate key for a user
public Save_GetKey( id, szKey[], len )
{
	switch( get_pcvar_num( CVAR_uwc3ng_save_by ) )
	{
		case SAVEBY_NAME:
		{
			get_user_name( id, szKey, len );

			Save_FormatString( szKey, len );
		}

		case SAVEBY_IP:		get_user_ip( id, szKey, len, 1 );
		case SAVEBY_STEAMID:	get_user_authid( id, szKey, len );
	}
}

// Function will return the keyname
public Save_GetKeyName( szKeyName[], len )
{
	switch( get_pcvar_num( CVAR_uwc3ng_save_by ) )
	{
		case SAVEBY_NAME:	copy( szKeyName, len, "player_name" );
		case SAVEBY_IP:		copy( szKeyName, len, "player_ip" );
		case SAVEBY_STEAMID:	copy( szKeyName, len, "player_steamid" );
	}
}

// Function will get the user's XP
public Player_GetAllXP( id )
{
	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) || !id )
	{
		return;
	}
	
	if ( id >= TASK_GETXP )
	{
		id -= TASK_GETXP;
	}
	
	if ( !is_user_connected( id ) )
	{
		return;
	}
		
	log_amx( "Bot ID: %d", id );
	
	// Get the XP	
	switch( g_SaveType )
	{
		case DB_MYSQLX:	MYSQLX_GetAllXP( id );
		case DB_SQLITE:	SQLITE_GetAllXP( id );
		case NVAULT: NVAULT_GetAllXP( id );
	}
	
	// Detect and set the startlevel
	PLAYER_set_startlevel( id )
	
	// Calculate the Level
	XP_CalcLevel( id );
	
	// Look if the player has available skillpoints ready
	SKILL_available_points( id );
	
	// Look if the player has available attribute points ready
	ATTRIB_Check_Available_points( id );
	
	// Look if the player has available resistance points ready
	RESIST_Check_Available_points( id );
	
	return;
}

// Prepares text for database or nvault insertion
public Save_FormatString( text[], len )
{
	replace_all( text, len, "'", "\'" );
}

public Save_FetchUniqueID( id )
{
	// If we're not saving XP, why do this?
	if ( !get_pcvar_num( CVAR_uwc3ng_save_xp ) || !id )
	{
		return;
	}
	
	if ( is_user_bot( id ) )
	{
		return;
	}

	// we read or create the UniqueID
	switch( g_SaveType )
	{
		case DB_MYSQLX:	MYSQLX_FetchUniqueID( id );
		case DB_SQLITE:	SQLITE_FetchUniqueID( id );
		case NVAULT: return;
	}
	
	// Nothing was found - try again in a bit
	if ( p_data[id][PLAYER_UNIQUEID] == 0 )
	{
		if ( !g_DBConn )
		{
			// Close the connection
			Save_Close();
		
			// Re-open the connection
			Save_Init();
		}

		set_task( 1.0, "Save_FetchUniqueID", id );
	}

	return;
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
