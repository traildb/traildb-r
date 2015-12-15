#ifndef SHARED_POINTER_TYPEDEFS_H_INCLUDED
#define SHARED_POINTER_TYPEDEFS_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

/*!
 * @def SHARED_POINTER_TYPEDEFS
 * @brief macro to define shared pointer typedefs
 */
#define SHARED_POINTER_TYPEDEFS(C) \
  typedef boost::shared_ptr<C> C##Ptr; \
  typedef boost::shared_ptr<const C> C##ConstPtr;

#endif // SHARED_POINTER_TYPEDEFS_H_INCLUDED

