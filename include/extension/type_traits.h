#pragma once

#if __cplusplus >= 201103L

#include "functional/unwrap_ref.h"
#include "type_traits/bounded_array_traits.h"
#include "type_traits/copy_cv.h"
#include "type_traits/is_final.h"
#include "type_traits/is_invocable.h"
#include "type_traits/is_nothrow_convertible.h"
#include "type_traits/is_trivially_copyable.h"
#include "type_traits/logical_traits.h"
#include "type_traits/remove_cvref.h"
#include "type_traits/type_identity.h"
#include "type_traits/void_t.h"

#elif __cplusplus

#include "type_traits/add_cv.h"
#include "type_traits/add_lvalue_reference.h"
#include "type_traits/add_pointer.h"
#include "type_traits/add_rvalue_reference.h"
#include "type_traits/bounded_array_traits.h"
#include "type_traits/conditional.h"
#include "type_traits/copy_cv.h"
#include "type_traits/decay.h"
#include "type_traits/enable_if.h"
#include "type_traits/integral_constant.h"
#include "type_traits/is_array.h"
#include "type_traits/is_class.h"
#include "type_traits/is_const.h"
#include "type_traits/is_final.h"
#include "type_traits/is_function.h"
#include "type_traits/is_lvalue_reference.h"
#include "type_traits/is_pointer.h"
#include "type_traits/is_reference.h"
#include "type_traits/is_rvalue_reference.h"
#include "type_traits/is_same.h"
#include "type_traits/is_union.h"
#include "type_traits/is_void.h"
#include "type_traits/is_volatile.h"
#include "type_traits/remove_all_extents.h"
#include "type_traits/remove_cv.h"
#include "type_traits/remove_cvref.h"
#include "type_traits/remove_extent.h"
#include "type_traits/remove_pointer.h"
#include "type_traits/remove_reference.h"
#include "type_traits/sizeable-03.h"
#include "type_traits/type_identity.h"
#include "utility/declval-03.h"

#endif
