/* 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have recvbufd a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/*
  client structures - written by Stephane Vaxelaire <svax@free.fr>
*/

#ifndef __CLIENTSTRUCTS_H
#define __CLIENTSTRUCTS_H

#include <iostream>
#include <string.h>
#include "gfx/quaternion.h"
#include "const.h"
#include "cubicsplines.h"
#include "configxml.h"
#include "vsnet_clientstate.h"
#include "vsnet_address.h"
#include "vsnet_socket.h"
#include "cmd/unit_generic.h"
#include "savegame.h"

extern VegaConfig *vs_config;

struct Client;

// Description of a client's ship (ship type, weapons, shields status...)
struct ClientDescription
{
};

// Network part of a client description
struct	Client
{
	/* Network and identification properties */
	Unit	*		game_unit;
	SaveGame		save;
	CubicSpline		spline;
	AddressIP		cltadr;
	bool            is_tcp;
	SOCKETALT		sock;
	ObjSerial		serial;
	// 2 timeout vals to check a timeout for client connections
	// those vals are server times
	double		old_timeout;
	double		latest_timeout;
	// 2 timestamps vals from client time to check receiving old packet after newer ones
	unsigned int	old_timestamp;
	unsigned int	latest_timestamp;
	unsigned int	deltatime;
	int				sdl_channel;
	char			name[NAMELEN];
	char			passwd[NAMELEN];
	ClientState		old_state;
	ClientState		current_state;
	ClientDescription	current_desc;
	/* In-game parameters */
	int				zone;
	char			ingame;
	//char			localplayer;

	Client()
	{
		memset( &old_state, 0, sizeof( ClientState));
		memset( &current_state, 0, sizeof( ClientState));
		latest_timestamp=0;
		old_timestamp=0;
		latest_timeout=0;
		old_timeout=0;
		deltatime=0;
		ingame=0;
	}

	Client( SOCKETALT& s, bool tcp )
	    : is_tcp(tcp)
	    , sock(s)
	{
		memset( &old_state, 0, sizeof( ClientState));
		memset( &current_state, 0, sizeof( ClientState));
		latest_timestamp=0;
		old_timestamp=0;
		latest_timeout=0;
		old_timeout=0;
		deltatime=0;
		ingame=0;
	}

	inline bool isTcp( ) const {
	    return is_tcp;
	}

	inline bool isUdp( ) const {
	    return !is_tcp;
	}

	friend std::ostream& operator<<( std::ostream& ostr, const Client& c );

private:
	Client( const Client& );
	Client& operator=( const Client& );
};

void	LoadXMLUnit( Unit * unit, const char * filename, char * buf);
void	WriteXMLUnit( string filename, char * xmlbuf, int tsize);

#endif
