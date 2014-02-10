#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/Time.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>

namespace MultiLibrary
{

class Thread;

namespace CurrentThread
{

MULTILIBRARY_COMMON_API void Sleep( const Time &duration );
MULTILIBRARY_COMMON_API Thread Get( );

} // namespace CurrentThread

class MULTILIBRARY_COMMON_API Thread : public NonCopyable
{
private:
	struct FunctorBase;
	struct Functor;
	template<typename T> struct FunctorWithArg;
	template<typename C> struct FunctorMember;
	template<typename C, typename T> struct FunctorMemberWithArg;

public:
	struct Data;

	Thread( void *( *function ) ( ) );
	template<typename T> Thread( void *( *function ) ( T ), T argument );
	template<typename C> Thread( void *( C::*function ) ( ), C *object );
	template<typename C, typename T> Thread( void *( C::*function ) ( T ), C *object, T argument );

	~Thread( );

	bool Launch( );

	void Detach( );
	void Terminate( );
	void Join( );

	void *GetReturnValue( ) const;

private:
	friend Thread CurrentThread::Get( );
	Thread( Data *data );

	void Init( FunctorBase *func );

	Data *thread_data;
};

#include <MultiLibrary/Common/Thread.inl>

} // namespace MultiLibrary