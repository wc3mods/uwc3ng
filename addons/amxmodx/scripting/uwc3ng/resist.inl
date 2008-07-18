// Uwc3ng Resistances functions

// Display the select resist menu
public RESIST_ShowSelectMenu( id )
{
	// The header
	new MenuText[512] = "", KEYS = (1<<9) | (1<<8);
	formatex( MenuText, charsmax( MenuText ), "\ySelect a Resistance to enhance [%d pts remaining]: \w^n", p_data[id][AVAILABLE_RESISTS] );
	add( MenuText, charsmax( MenuText ), "\y(say /resistinfo for info about resistances) \w^n^n" );
	
	// Loop through all resistens
	for ( new i = 0; i < MAX_RESIST; ++i )
	{
		// Add every resist to the menutext
		formatex( MenuText, charsmax( MenuText ), "%s\w%d. %s Resistance     [ %d/%d ]^n", MenuText, i+1, Resistance_Names[i], p_data_resist[id][i], MAX_RESIST_POINTS ); 
		
		// Add every keypoint
		KEYS |= (1<<i);
	}
	
	// Add the attrib menupoint
	add( MenuText, charsmax( MenuText ), "^n\w9. Attributes Menu^n" );
	
	// Add the exit menupoint
	add( MenuText, charsmax( MenuText ), "\w0. Exit^n" );
	
	// Show the player the menu
	show_menu ( id, KEYS, MenuText, 25, "ResistMenu" );
	
	return;
}

// Handle the pressed key from the select resist menu
public RESIST_HandleMenu( id, key )
{
	if ( !is_user_connected( id ) )
	{
		return;
	}
	
	// The player exit the menu
	if ( key == 9 )
	{
		return;
	}
	
	// The player want to the attributes menu
	else if ( key == 8 )
	{
		ATTRIB_ShowSelectMenu( id );
		
		return;
	}
	
	// The player is already on the last attrib level
	if ( p_data_resist[id][key] == MAX_RESIST_POINTS )
	{
		client_print( id, print_chat, "%s You have already reached the maximum resistance level", MOD_NAME );
		
		return;
	}
	
	// The player has no available attribute points
	else if ( p_data[id][AVAILABLE_RESISTS] <= 0 )
	{
		client_print( id, print_chat, "%s You do not have any Resistance points to spend, earn more E-XP", MOD_NAME );
		
		return;
	}
	
	// Alright. Add one point
	p_data_resist[id][key]++;
	
	// Remove one from the available points
	p_data[id][AVAILABLE_RESISTS]--;
	
	// Print him a message
	client_print( id, print_chat, "%s You spend 1 Resistance point to %s", MOD_NAME, Resistance_Names[key] );
	
	// He has more available resist points re-open the menu
	if ( p_data[id][AVAILABLE_RESISTS] > 0 )
	{
		RESIST_ShowSelectMenu( id );
	}
	
	return;
}

// Check available resists points
public RESIST_Check_Available_points( id )
{
	// Get the selected attrib points
	new iSelectedResists = RESIST_SelectedResists( id );
	
	// Calculate the default available resists for this player
	new iAvailablePoints = ( p_data[id][PLAYER_LVL] * ( MAX_RESIST_AVAILABLE / MAX_LEVEL ) );
		
	// Save the available attrib points
	p_data[id][AVAILABLE_RESISTS] = ( iAvailablePoints -  iSelectedResists );
	
	return;
}
	
	
// Check selected resists points
RESIST_SelectedResists( id )
{
	// Loop through all resists and count the selected resists points
	static i, iResistCounter; iResistCounter = 0;
	for ( i = 0; i < MAX_RESIST; ++i )
	{
		iResistCounter += p_data_resist[id][i];
	}
	
	return iResistCounter;
}

// Reset all resists
public RESIST_ResetAll( id )
{
	// print him a message that all resists will be reseted
	client_print( id, print_center, "Your resistances will be reset next round" );
	
	// We have to save it because on the next round we should reset all resists
	p_data_b[id][RESET_RESISTS] = true;
}

// Reset now all resists (called on new round or spawn if csdm enabled)
public RESIST_ResetAllNow( id )
{
	if ( p_data_b[id][RESET_RESISTS] )
	{
		// We loop through all resists and set it to start points
		for ( new j = 0; j < MAX_RESIST; ++j )
		{
			p_data_resist[id][j] = 0;
		}
			
		// Detect new available resist points
		RESIST_Check_Available_points( id )
			
		// Next round the resists should no more reseted
		p_data_b[id][RESET_RESISTS] = false;
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang1031\\ f0\\ fs16 \n\\ par }
*/
