#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>

namespace MultiLibrary
{

// Written by Jack Handy - jakkhandy@hotmail.com
bool WildcardCompare( const char *wild, const char *string )
{
	const char *cp = NULL, *mp = NULL;

	while( *string && *wild != '*' )
	{
		if( *wild != *string && *wild != '?' )
			return false;

		++wild;
		++string;
	}

	while( *string )
	{
		if( *wild == '*' )
		{
			if( *++wild == '\0' )
				return true;

			mp = wild;
			cp = string + 1;
		}
		else if( *wild == *string || *wild == '?' )
		{
			++wild;
			++string;
		}
		else
		{
			wild = mp;
			string = cp++;
		}
	}

	while( *wild == '*' )
		++wild;

	return *wild == '\0';
}

} // namespace MultiLibrary