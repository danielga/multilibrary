/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014, Daniel Almeida
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#include <MultiLibrary/Common/Stopwatch.hpp>
#include <mach/mach_time.h>
double update_frequency = 0.0;

namespace MultiLibrary
{

Stopwatch::Stopwatch( ) :
	last_update( 0.0 ),
	elapsed_time( 0.0 ),
	running( false )
{
	if( frequency == 0.0 )
	{
		mach_timebase_info_data_t timebaseInfo;
		mach_timebase_info( &timebaseInfo );
		update_frequency = timebaseInfo.numer / timebaseInfo.denom;
	}
}

Stopwatch::~Stopwatch( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );
	running = false;
}

void Stopwatch::Resume( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	if( running )
		return;

	last_update = mach_absolute_time( ) * update_frequency;
	running = true;
}

void Stopwatch::Pause( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	if( !running )
		return;

	elapsed_time += mach_absolute_time( ) * update_frequency - last_update;
	running = false;
}

void Stopwatch::Reset( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	elapsed_time = 0.0;
	last_update = mach_absolute_time( ) * update_frequency;
}

double Stopwatch::GetElapsedTime( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	if( running )
	{
		double increment = mach_absolute_time( ) * update_frequency;
		elapsed_time += increment - last_update;
		last_update = increment;
	}

	return elapsed_time;
}

} // namespace MultiLibrary
