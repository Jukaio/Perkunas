// perkunas_handle.h

#ifndef INCLUDED_PERKUNAS_HANDLE
#define INCLUDED_PERKUNAS_HANDLE

#include <cinttypes>
#include "perkunas_template_concepts.h"

//
//namespace perkunas
//{
//	// Forward declare used systems
//	// Adds strong dependency to this Library, but 
//	// the user cannot see this class anyway 
//	class VideoSubSystem;
//	class AudioSubSystem;
//	class EventSubSystem;
//	
//	/* 
//		Change the possible amount of unique handles by changing 
//		the handle type. I.e. 
//		if you need many many handles, use uint32
//		if you just need a handful, because the resource is very big, use uint8
//* 	*/
//	template <_Signed_integer_concept _Signed_int_type>
//	class _Handle
//	{
//	private:	
//		// Systems which are allowed to create Handles
//		friend VideoSubSystem;
//		friend AudioSubSystem;
//		friend EventSubSystem;
//
//	public:
//		// Default construction to invalid handle (~0)
//		_Handle() = default;
//		virtual ~_Handle() = default;
//		// Copying of Handles is allowed, just passes the value 
//		_Handle(const _Handle& rhs) noexcept
//			: m_handle(rhs.m_handle)
//		{
//			m_handle = rhs.m_handle;
//		}
//		_Handle& operator=(const _Handle& rhs) noexcept
//		{
//			m_handle = rhs.m_handle;
//			return *this;
//		}
//
//		// Treat handle like an int, by converting it directly to any arithmethic number
//		// TODO: Change that to ints only - Floats, doubles, etc are redundant
//		template<_Arithmethic_concept _Type>
//		operator _Type() noexcept
//		{
//			return static_cast<_Type>(m_handle);
//		}
//
//
//	private:
//		// Only friends can access the overloaded constructor
//		// thus this is the only way to create a valid handle 
//		template<_Arithmethic_concept _Type>
//		_Handle(_Type p_handle) noexcept
//			: m_handle(static_cast<_Signed_int_type>(p_handle))
//		{
//		}
//
//		// Cannot get created unless a friend system is creating it
//		// At the end of the day a handle is only the position in a vector
//		_Signed_int_type m_handle = static_cast<_Signed_int_type>(~0); // Max value = invalidation
//	};
//
//	class Handle::_HandleImpl
//	{
//	public:
//		_HandleImpl() = default;
//
//		// get the handle through simple assignment 
//		template<_Arithmethic_concept _Type>
//		operator _Type() noexcept
//		{
//			return static_cast<_Type>(m_handle);
//		}
//		_Handle<uint8_t> m_handle;
//	};
//}

#endif // !INCLUDED_PERKUNAS_HANDLE
