/*************************************************************************
 * MultiLibrary - https://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2022, Daniel Almeida
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
 *************************************************************************/

#include <MultiLibrary/Common/Stopwatch.hpp>
#include <windows.h>

static int64_t update_frequency = 0;

namespace MultiLibrary
{

Stopwatch::Stopwatch( ) :
	last_update( 0.0 ),
	elapsed_time( 0.0 ),
	running( false )
{
	if( update_frequency == 0 )
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		update_frequency = freq.QuadPart;
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

	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	last_update = time.QuadPart * 1000.0 / update_frequency;
	running = true;
}

void Stopwatch::Pause( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	if( !running )
		return;

	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	elapsed_time += time.QuadPart * 1000.0 / update_frequency - last_update;
	running = false;
}

void Stopwatch::Reset( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	elapsed_time = 0.0;
	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	last_update = time.QuadPart * 1000.0 / update_frequency;
}

double Stopwatch::GetElapsedTime( )
{
	std::lock_guard<std::mutex> auto_lock( update_lock );

	if( running )
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter( &time );
		double increment = time.QuadPart * 1000.0 / update_frequency;
		elapsed_time += increment - last_update;
		last_update = increment;
	}

	return elapsed_time;
}

} // namespace MultiLibrary
