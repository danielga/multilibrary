struct Thread::FunctorBase
{
	virtual ~FunctorBase( ) { }
	virtual void *Run( ) = 0;
};

struct Thread::Functor : public Thread::FunctorBase
{
	Functor( void *( *function ) ( ) ) :
		m_function( function )
	{ }

	virtual void *Run( )
	{
		return m_function( );
	}

	void *( *m_function ) ( );
};

template<typename T> struct Thread::FunctorWithArg : public Thread::FunctorBase
{
	FunctorWithArg( void *( *function ) ( T ), T arg ) :
		m_function( function ),
		m_arg( arg )
	{ }

	virtual void *Run( )
	{
		return m_function( m_arg );
	}

	void *( *m_function ) ( T );
	T m_arg;
};

template<typename C> struct Thread::FunctorMember : public Thread::FunctorBase
{
	FunctorMember( void *( C::*function ) ( ), C *object ) :
		m_function( function ),
		m_object( object )
	{ }

	virtual void *Run( )
	{
		return ( m_object->*m_function ) ( );
	}

	void *( C::*m_function ) ( );
	C *m_object;
};

template<typename C, typename T> struct Thread::FunctorMemberWithArg : public Thread::FunctorBase
{
	FunctorMemberWithArg( void *( C::*function ) ( T ), C *object, T argument ) :
		m_function( function ),
		m_object( object ),
		m_arg( argument )
	{ }

	virtual void *Run( )
	{
		return ( m_object->*m_function ) ( m_arg );
	}

	void *( C::*m_function ) ( T );
	C *m_object;
	T m_arg;
};

inline Thread::Thread( void *( *function ) ( ) ) :
	thread_data( NULL )
{
	Init( new Functor( function ) );
}

template<typename T> Thread::Thread( void *( *function ) ( T ), T argument ) :
	thread_data( NULL )
{
	Init( new FunctorWithArg<T>( function, argument ) );
}

template<typename C> Thread::Thread( void *( C::*function ) ( ), C *object ) :
	thread_data( NULL )
{
	Init( new FunctorMember<C>( function, object ) );
}

template<typename C, typename T> Thread::Thread( void *( C::*function ) ( T ), C *object, T argument ) :
	thread_data( NULL )
{
	Init( new FunctorMemberWithArg<C, T>( function, object, argument ) );
}