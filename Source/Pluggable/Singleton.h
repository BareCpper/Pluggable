/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2011 Labyrinth Entertainment Ltd.
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRX_SINGLETON_H
#define	KISMOFRX_SINGLETON_H

#include "KismoFRX/Common.h"

/** Defines the singleton functions in a derived class so they work across DLL boundaries
*/
#define SINGLETON( _className )	\
	static _className& getSingleton( void );\
	static _className* getSingletonPtr( void );

/** macros for implimenting the singleton functions so that they work across DLL boundaries
*/
#define IMPL_SINGLETON_INSTANCE( _className ) \
	template<> _className* kismo::Singleton< _className, false >::ms_singleton = 0;
#define IMPL_SINGLETON( _className ) IMPL_SINGLETON_EX( _className, false )
#define IMPL_SINGLETON_EX( _className, _multiInstance )/* \
	template<> _className* kismo::Singleton< _className, _multiInstance >::ms_singleton = 0; \
	_className& _className::getSingleton( void ) \
	{ Assert( ms_singleton != 0 ); return ( *ms_singleton ); } \
	_className* _className::getSingletonPtr( void ) \
	{ return ms_singleton;  }*/

namespace kismo
{		
	/*
		Example to create a singleton from this simply do:
			class CLASSNAME : public Singleton< CLASSNAME >
			{...

		NOTE derived class should have this in their cpp:
			template<> CLASSNAME* Singleton< CLASSNAME >::ms_Singleton = 0;
	*/

	/** Manual instanciated Singleton class
		\remarks By manual instanciation the lifetime of the instance can be managed like any normal variable
	*/
	template < class T, bool multiInstance = false > 
	class Singleton;

	/**
		\brief Single instantiated Singleton. Asserts if a second instance of the class is created
	*/
	template < class T >
	class Singleton< T, false >
	{
	public:
		Singleton()
		{
			Assert( !ms_singleton );  //Make sure the singleton is not created twice anywhere!          
			ms_singleton = static_cast< T* >( this );
		}

		virtual ~Singleton( void )
		{  
			Assert( ms_singleton ); //Make sure the singletone was created.... wierd if not
			ms_singleton = 0; 
		}

			
		static T& getSingleton( void ) 
		{ 
			Assert( ms_singleton != 0 ); 
			return ( *ms_singleton ); 
		}
		
		static T* getSingletonPtr( void ) 
		{ 
			Assert( ms_singleton != 0 ); 
			return ms_singleton;  
		}

	protected:
		static T* ms_singleton; //Static instance pointer
	};

	/**
		\brief Multiple instancece singleton where only one instance si ever current at a time
		\remarks The newest is the current by default. Call 'setAsSingletonInstance()' to set as current
	*/
	template < class T > 
	class Singleton < T, true >
	{
	public:
		Singleton()
		{
			ms_singleton = static_cast< T* >( this );
		}

		virtual ~Singleton( void )
		{  
			if ( ms_singleton == static_cast< T* >( this ) ) //Only set to 0 if the instance is current
				ms_singleton = 0; 
		}

		/**
			\brief Make this the current globally accessible instance
		*/
		void setAsSingletonInstance()
		{
			ms_singleton = static_cast< T* >( this ); 
		}

			
		static T& getSingleton( void ) 
		{ 
			Assert( ms_singleton != 0 ); 
			return ( *ms_singleton ); 
		}
		
		static T* getSingletonPtr( void ) 
		{ 
			Assert( ms_singleton != 0 ); 
			return ms_singleton;  
		}

	protected:
		static T* ms_singleton; //Static instance pointer
	};
	
	template<class T>
	T* Singleton < T, false >::ms_singleton = 0;
	
	template<class T>
	T* Singleton < T, true >::ms_singleton = 0;
	
} // END: frx

#endif
