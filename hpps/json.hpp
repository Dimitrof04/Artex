//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

/****************************************************************************\
 * Note on documentation: The source files contain links to the online      *
 * documentation of the public API at https://json.nlohmann.me. This URL    *
 * contains the most recent documentation and should also be applicable to  *
 * previous versions; documentation for deprecated functions is not         *
 * removed, but marked deprecated. See "Generate documentation" section in  *
 * file docs/README.md.                                                     *
\****************************************************************************/

#ifndef  INCLUDE_NLOHMANN_JSON_HPP_
#define INCLUDE_NLOHMANN_JSON_HPP_

#include <algorithm> // all_of, find, for_each
#include <cstddef> // nullptr_t, ptrdiff_t, size_t
#include <functional> // hash, less
#include <initializer_list> // initializer_list
#ifndef JSON_NO_IO
    #include <iosfwd> // istream, ostream
#endif  // JSON_NO_IO
#include <iterator> // random_access_iterator_tag
#include <memory> // unique_ptr
#include <string> // string, stoi, to_string
#include <utility> // declval, forward, move, pair, swap
#include <vector> // vector

// #include <nlohmann/adl_serializer.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <utility>

// #include <nlohmann/detail/abi_macros.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



// This file contains all macro definitions affecting or depending on the ABI

#ifndef JSON_SKIP_LIBRARY_VERSION_CHECK
    #if defined(NLOHMANN_JSON_VERSION_MAJOR) && defined(NLOHMANN_JSON_VERSION_MINOR) && defined(NLOHMANN_JSON_VERSION_PATCH)
        #if NLOHMANN_JSON_VERSION_MAJOR != 3 || NLOHMANN_JSON_VERSION_MINOR != 11 || NLOHMANN_JSON_VERSION_PATCH != 3
            #warning "Already included a different version of the library!"
        #endif
    #endif
#endif

#define NLOHMANN_JSON_VERSION_MAJOR 3   // NOLINT(modernize-macro-to-enum)
#define NLOHMANN_JSON_VERSION_MINOR 11  // NOLINT(modernize-macro-to-enum)
#define NLOHMANN_JSON_VERSION_PATCH 3   // NOLINT(modernize-macro-to-enum)

#ifndef JSON_DIAGNOSTICS
    #define JSON_DIAGNOSTICS 0
#endif

#ifndef JSON_USE_LEGACY_DISCARDED_VALUE_COMPARISON
    #define JSON_USE_LEGACY_DISCARDED_VALUE_COMPARISON 0
#endif

#if JSON_DIAGNOSTICS
    #define NLOHMANN_JSON_ABI_TAG_DIAGNOSTICS _diag
#else
    #define NLOHMANN_JSON_ABI_TAG_DIAGNOSTICS
#endif

#if JSON_USE_LEGACY_DISCARDED_VALUE_COMPARISON
    #define NLOHMANN_JSON_ABI_TAG_LEGACY_DISCARDED_VALUE_COMPARISON _ldvcmp
#else
    #define NLOHMANN_JSON_ABI_TAG_LEGACY_DISCARDED_VALUE_COMPARISON
#endif

#ifndef NLOHMANN_JSON_NAMESPACE_NO_VERSION
    #define NLOHMANN_JSON_NAMESPACE_NO_VERSION 0
#endif

// Construct the namespace ABI tags component
#define NLOHMANN_JSON_ABI_TAGS_CONCAT_EX(a, b) json_abi ## a ## b
#define NLOHMANN_JSON_ABI_TAGS_CONCAT(a, b) \
    NLOHMANN_JSON_ABI_TAGS_CONCAT_EX(a, b)

#define NLOHMANN_JSON_ABI_TAGS                                       \
    NLOHMANN_JSON_ABI_TAGS_CONCAT(                                   \
            NLOHMANN_JSON_ABI_TAG_DIAGNOSTICS,                       \
            NLOHMANN_JSON_ABI_TAG_LEGACY_DISCARDED_VALUE_COMPARISON)

// Construct the namespace version component
#define NLOHMANN_JSON_NAMESPACE_VERSION_CONCAT_EX(major, minor, patch) \
    _v ## major ## _ ## minor ## _ ## patch
#define NLOHMANN_JSON_NAMESPACE_VERSION_CONCAT(major, minor, patch) \
    NLOHMANN_JSON_NAMESPACE_VERSION_CONCAT_EX(major, minor, patch)

#if NLOHMANN_JSON_NAMESPACE_NO_VERSION
#define NLOHMANN_JSON_NAMESPACE_VERSION
#else
#define NLOHMANN_JSON_NAMESPACE_VERSION                                 \
    NLOHMANN_JSON_NAMESPACE_VERSION_CONCAT(NLOHMANN_JSON_VERSION_MAJOR, \
                                           NLOHMANN_JSON_VERSION_MINOR, \
                                           NLOHMANN_JSON_VERSION_PATCH)
#endif

// Combine namespace components
#define NLOHMANN_JSON_NAMESPACE_CONCAT_EX(a, b) a ## b
#define NLOHMANN_JSON_NAMESPACE_CONCAT(a, b) \
    NLOHMANN_JSON_NAMESPACE_CONCAT_EX(a, b)

#ifndef NLOHMANN_JSON_NAMESPACE
#define NLOHMANN_JSON_NAMESPACE               \
    nlohmann::NLOHMANN_JSON_NAMESPACE_CONCAT( \
            NLOHMANN_JSON_ABI_TAGS,           \
            NLOHMANN_JSON_NAMESPACE_VERSION)
#endif

#ifndef NLOHMANN_JSON_NAMESPACE_BEGIN
#define NLOHMANN_JSON_NAMESPACE_BEGIN                \
    namespace nlohmann                               \
    {                                                \
    inline namespace NLOHMANN_JSON_NAMESPACE_CONCAT( \
                NLOHMANN_JSON_ABI_TAGS,              \
                NLOHMANN_JSON_NAMESPACE_VERSION)     \
    {
#endif

#ifndef NLOHMANN_JSON_NAMESPACE_END
#define NLOHMANN_JSON_NAMESPACE_END                                     \
    }  /* namespace (inline namespace) NOLINT(readability/namespace) */ \
    }  // namespace nlohmann
#endif

// #include <nlohmann/detail/conversions/from_json.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <algorithm> // transform
#include <array> // array
#include <forward_list> // forward_list
#include <iterator> // inserter, front_inserter, end
#include <map> // map
#include <string> // string
#include <tuple> // tuple, make_tuple
#include <type_traits> // is_arithmetic, is_same, is_enum, underlying_type, is_convertible
#include <unordered_map> // unordered_map
#include <utility> // pair, declval
#include <valarray> // valarray

// #include <nlohmann/detail/exceptions.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <cstddef> // nullptr_t
#include <exception> // exception
#if JSON_DIAGNOSTICS
    #include <numeric> // accumulate
#endif
#include <stdexcept> // runtime_error
#include <string> // to_string
#include <vector> // vector

// #include <nlohmann/detail/value_t.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <array> // array
#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <string> // string

// #include <nlohmann/detail/macro_scope.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <utility> // declval, pair
// #include <nlohmann/detail/meta/detected.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <type_traits>

// #include <nlohmann/detail/meta/void_t.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



// #include <nlohmann/detail/abi_macros.hpp>


NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

template<typename ...Ts> struct make_void
{
    using type = void;
};
template<typename ...Ts> using void_t = typename make_void<Ts...>::type;

}  // namespace detail
NLOHMANN_JSON_NAMESPACE_END


NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

// https://en.cppreference.com/w/cpp/experimental/is_detected
struct nonesuch
{
    nonesuch() = delete;
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    nonesuch(nonesuch const&&) = delete;
    void operator=(nonesuch const&) = delete;
    void operator=(nonesuch&&) = delete;
};

template<class Default,
         class AlwaysVoid,
         template<class...> class Op,
         class... Args>
struct detector
{
    using value_t = std::false_type;
    using type = Default;
};

template<class Default, template<class...> class Op, class... Args>
struct detector<Default, void_t<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
    using type = Op<Args...>;
};

template<template<class...> class Op, class... Args>
using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

template<template<class...> class Op, class... Args>
struct is_detected_lazy : is_detected<Op, Args...> { };

template<template<class...> class Op, class... Args>
using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

template<class Default, template<class...> class Op, class... Args>
using detected_or = detector<Default, void, Op, Args...>;

template<class Default, template<class...> class Op, class... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

template<class Expected, template<class...> class Op, class... Args>
using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

template<class To, template<class...> class Op, class... Args>
using is_detected_convertible =
    std::is_convertible<detected_t<Op, Args...>, To>;

}  // namespace detail
NLOHMANN_JSON_NAMESPACE_END

// #include <nlohmann/thirdparty/hedley/hedley.hpp>


//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-FileCopyrightText: 2016-2021 Evan Nemerson <evan@nemerson.com>
// SPDX-License-Identifier: MIT

/* Hedley - https://nemequ.github.io/hedley
 * Created by Evan Nemerson <evan@nemerson.com>
 */

#if !defined(JSON_HEDLEY_VERSION) || (JSON_HEDLEY_VERSION < 15)
#if defined(JSON_HEDLEY_VERSION)
    #undef JSON_HEDLEY_VERSION
#endif
#define JSON_HEDLEY_VERSION 15

#if defined(JSON_HEDLEY_STRINGIFY_EX)
    #undef JSON_HEDLEY_STRINGIFY_EX
#endif
#define JSON_HEDLEY_STRINGIFY_EX(x) #x

#if defined(JSON_HEDLEY_STRINGIFY)
    #undef JSON_HEDLEY_STRINGIFY
#endif
#define JSON_HEDLEY_STRINGIFY(x) JSON_HEDLEY_STRINGIFY_EX(x)

#if defined(JSON_HEDLEY_CONCAT_EX)
    #undef JSON_HEDLEY_CONCAT_EX
#endif
#define JSON_HEDLEY_CONCAT_EX(a,b) a##b

#if defined(JSON_HEDLEY_CONCAT)
    #undef JSON_HEDLEY_CONCAT
#endif
#define JSON_HEDLEY_CONCAT(a,b) JSON_HEDLEY_CONCAT_EX(a,b)

#if defined(JSON_HEDLEY_CONCAT3_EX)
    #undef JSON_HEDLEY_CONCAT3_EX
#endif
#define JSON_HEDLEY_CONCAT3_EX(a,b,c) a##b##c

#if defined(JSON_HEDLEY_CONCAT3)
    #undef JSON_HEDLEY_CONCAT3
#endif
#define JSON_HEDLEY_CONCAT3(a,b,c) JSON_HEDLEY_CONCAT3_EX(a,b,c)

#if defined(JSON_HEDLEY_VERSION_ENCODE)
    #undef JSON_HEDLEY_VERSION_ENCODE
#endif
#define JSON_HEDLEY_VERSION_ENCODE(major,minor,revision) (((major) * 1000000) + ((minor) * 1000) + (revision))

#if defined(JSON_HEDLEY_VERSION_DECODE_MAJOR)
    #undef JSON_HEDLEY_VERSION_DECODE_MAJOR
#endif
#define JSON_HEDLEY_VERSION_DECODE_MAJOR(version) ((version) / 1000000)

#if defined(JSON_HEDLEY_VERSION_DECODE_MINOR)
    #undef JSON_HEDLEY_VERSION_DECODE_MINOR
#endif
#define JSON_HEDLEY_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)

#if defined(JSON_HEDLEY_VERSION_DECODE_REVISION)
    #undef JSON_HEDLEY_VERSION_DECODE_REVISION
#endif
#define JSON_HEDLEY_VERSION_DECODE_REVISION(version) ((version) % 1000)

#if defined(JSON_HEDLEY_GNUC_VERSION)
    #undef JSON_HEDLEY_GNUC_VERSION
#endif
#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
    #define JSON_HEDLEY_GNUC_VERSION JSON_HEDLEY_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
    #define JSON_HEDLEY_GNUC_VERSION JSON_HEDLEY_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(JSON_HEDLEY_GNUC_VERSION_CHECK)
    #undef JSON_HEDLEY_GNUC_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_GNUC_VERSION)
    #define JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_GNUC_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_MSVC_VERSION)
    #undef JSON_HEDLEY_MSVC_VERSION
#endif
#if defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 140000000) && !defined(__ICL)
    #define JSON_HEDLEY_MSVC_VERSION JSON_HEDLEY_VERSION_ENCODE(_MSC_FULL_VER / 10000000, (_MSC_FULL_VER % 10000000) / 100000, (_MSC_FULL_VER % 100000) / 100)
#elif defined(_MSC_FULL_VER) && !defined(__ICL)
    #define JSON_HEDLEY_MSVC_VERSION JSON_HEDLEY_VERSION_ENCODE(_MSC_FULL_VER / 1000000, (_MSC_FULL_VER % 1000000) / 10000, (_MSC_FULL_VER % 10000) / 10)
#elif defined(_MSC_VER) && !defined(__ICL)
    #define JSON_HEDLEY_MSVC_VERSION JSON_HEDLEY_VERSION_ENCODE(_MSC_VER / 100, _MSC_VER % 100, 0)
#endif

#if defined(JSON_HEDLEY_MSVC_VERSION_CHECK)
    #undef JSON_HEDLEY_MSVC_VERSION_CHECK
#endif
#if !defined(JSON_HEDLEY_MSVC_VERSION)
    #define JSON_HEDLEY_MSVC_VERSION_CHECK(major,minor,patch) (0)
#elif defined(_MSC_VER) && (_MSC_VER >= 1400)
    #define JSON_HEDLEY_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_FULL_VER >= ((major * 10000000) + (minor * 100000) + (patch)))
#elif defined(_MSC_VER) && (_MSC_VER >= 1200)
    #define JSON_HEDLEY_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_FULL_VER >= ((major * 1000000) + (minor * 10000) + (patch)))
#else
    #define JSON_HEDLEY_MSVC_VERSION_CHECK(major,minor,patch) (_MSC_VER >= ((major * 100) + (minor)))
#endif

#if defined(JSON_HEDLEY_INTEL_VERSION)
    #undef JSON_HEDLEY_INTEL_VERSION
#endif
#if defined(__INTEL_COMPILER) && defined(__INTEL_COMPILER_UPDATE) && !defined(__ICL)
    #define JSON_HEDLEY_INTEL_VERSION JSON_HEDLEY_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, __INTEL_COMPILER_UPDATE)
#elif defined(__INTEL_COMPILER) && !defined(__ICL)
    #define JSON_HEDLEY_INTEL_VERSION JSON_HEDLEY_VERSION_ENCODE(__INTEL_COMPILER / 100, __INTEL_COMPILER % 100, 0)
#endif

#if defined(JSON_HEDLEY_INTEL_VERSION_CHECK)
    #undef JSON_HEDLEY_INTEL_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_INTEL_VERSION)
    #define JSON_HEDLEY_INTEL_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_INTEL_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_INTEL_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_INTEL_CL_VERSION)
    #undef JSON_HEDLEY_INTEL_CL_VERSION
#endif
#if defined(__INTEL_COMPILER) && defined(__INTEL_COMPILER_UPDATE) && defined(__ICL)
    #define JSON_HEDLEY_INTEL_CL_VERSION JSON_HEDLEY_VERSION_ENCODE(__INTEL_COMPILER, __INTEL_COMPILER_UPDATE, 0)
#endif

#if defined(JSON_HEDLEY_INTEL_CL_VERSION_CHECK)
    #undef JSON_HEDLEY_INTEL_CL_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_INTEL_CL_VERSION)
    #define JSON_HEDLEY_INTEL_CL_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_INTEL_CL_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_INTEL_CL_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_PGI_VERSION)
    #undef JSON_HEDLEY_PGI_VERSION
#endif
#if defined(__PGI) && defined(__PGIC__) && defined(__PGIC_MINOR__) && defined(__PGIC_PATCHLEVEL__)
    #define JSON_HEDLEY_PGI_VERSION JSON_HEDLEY_VERSION_ENCODE(__PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__)
#endif

#if defined(JSON_HEDLEY_PGI_VERSION_CHECK)
    #undef JSON_HEDLEY_PGI_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_PGI_VERSION)
    #define JSON_HEDLEY_PGI_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_PGI_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_PGI_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_SUNPRO_VERSION)
    #undef JSON_HEDLEY_SUNPRO_VERSION
#endif
#if defined(__SUNPRO_C) && (__SUNPRO_C > 0x1000)
    #define JSON_HEDLEY_SUNPRO_VERSION JSON_HEDLEY_VERSION_ENCODE((((__SUNPRO_C >> 16) & 0xf) * 10) + ((__SUNPRO_C >> 12) & 0xf), (((__SUNPRO_C >> 8) & 0xf) * 10) + ((__SUNPRO_C >> 4) & 0xf), (__SUNPRO_C & 0xf) * 10)
#elif defined(__SUNPRO_C)
    #define JSON_HEDLEY_SUNPRO_VERSION JSON_HEDLEY_VERSION_ENCODE((__SUNPRO_C >> 8) & 0xf, (__SUNPRO_C >> 4) & 0xf, (__SUNPRO_C) & 0xf)
#elif defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x1000)
    #define JSON_HEDLEY_SUNPRO_VERSION JSON_HEDLEY_VERSION_ENCODE((((__SUNPRO_CC >> 16) & 0xf) * 10) + ((__SUNPRO_CC >> 12) & 0xf), (((__SUNPRO_CC >> 8) & 0xf) * 10) + ((__SUNPRO_CC >> 4) & 0xf), (__SUNPRO_CC & 0xf) * 10)
#elif defined(__SUNPRO_CC)
    #define JSON_HEDLEY_SUNPRO_VERSION JSON_HEDLEY_VERSION_ENCODE((__SUNPRO_CC >> 8) & 0xf, (__SUNPRO_CC >> 4) & 0xf, (__SUNPRO_CC) & 0xf)
#endif

#if defined(JSON_HEDLEY_SUNPRO_VERSION_CHECK)
    #undef JSON_HEDLEY_SUNPRO_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_SUNPRO_VERSION)
    #define JSON_HEDLEY_SUNPRO_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_SUNPRO_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_SUNPRO_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_EMSCRIPTEN_VERSION)
    #undef JSON_HEDLEY_EMSCRIPTEN_VERSION
#endif
#if defined(__EMSCRIPTEN__)
    #define JSON_HEDLEY_EMSCRIPTEN_VERSION JSON_HEDLEY_VERSION_ENCODE(__EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__)
#endif

#if defined(JSON_HEDLEY_EMSCRIPTEN_VERSION_CHECK)
    #undef JSON_HEDLEY_EMSCRIPTEN_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_EMSCRIPTEN_VERSION)
    #define JSON_HEDLEY_EMSCRIPTEN_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_EMSCRIPTEN_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_EMSCRIPTEN_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_ARM_VERSION)
    #undef JSON_HEDLEY_ARM_VERSION
#endif
#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
    #define JSON_HEDLEY_ARM_VERSION JSON_HEDLEY_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000, (__ARMCOMPILER_VERSION % 1000000) / 10000, (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
    #define JSON_HEDLEY_ARM_VERSION JSON_HEDLEY_VERSION_ENCODE(__ARMCC_VERSION / 1000000, (__ARMCC_VERSION % 1000000) / 10000, (__ARMCC_VERSION % 10000) / 100)
#endif

#if defined(JSON_HEDLEY_ARM_VERSION_CHECK)
    #undef JSON_HEDLEY_ARM_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_ARM_VERSION)
    #define JSON_HEDLEY_ARM_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_ARM_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_ARM_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_IBM_VERSION)
    #undef JSON_HEDLEY_IBM_VERSION
#endif
#if defined(__ibmxl__)
    #define JSON_HEDLEY_IBM_VERSION JSON_HEDLEY_VERSION_ENCODE(__ibmxl_version__, __ibmxl_release__, __ibmxl_modification__)
#elif defined(__xlC__) && defined(__xlC_ver__)
    #define JSON_HEDLEY_IBM_VERSION JSON_HEDLEY_VERSION_ENCODE(__xlC__ >> 8, __xlC__ & 0xff, (__xlC_ver__ >> 8) & 0xff)
#elif defined(__xlC__)
    #define JSON_HEDLEY_IBM_VERSION JSON_HEDLEY_VERSION_ENCODE(__xlC__ >> 8, __xlC__ & 0xff, 0)
#endif

#if defined(JSON_HEDLEY_IBM_VERSION_CHECK)
    #undef JSON_HEDLEY_IBM_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_IBM_VERSION)
    #define JSON_HEDLEY_IBM_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_IBM_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_IBM_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_VERSION)
    #undef JSON_HEDLEY_TI_VERSION
#endif
#if \
    defined(__TI_COMPILER_VERSION__) && \
    ( \
      defined(__TMS470__) || defined(__TI_ARM__) || \
      defined(__MSP430__) || \
      defined(__TMS320C2000__) \
    )
#if (__TI_COMPILER_VERSION__ >= 16000000)
    #define JSON_HEDLEY_TI_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif
#endif

#if defined(JSON_HEDLEY_TI_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_VERSION)
    #define JSON_HEDLEY_TI_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_CL2000_VERSION)
    #undef JSON_HEDLEY_TI_CL2000_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C2000__)
    #define JSON_HEDLEY_TI_CL2000_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(JSON_HEDLEY_TI_CL2000_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_CL2000_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_CL2000_VERSION)
    #define JSON_HEDLEY_TI_CL2000_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_CL2000_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_CL2000_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_CL430_VERSION)
    #undef JSON_HEDLEY_TI_CL430_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__MSP430__)
    #define JSON_HEDLEY_TI_CL430_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(JSON_HEDLEY_TI_CL430_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_CL430_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_CL430_VERSION)
    #define JSON_HEDLEY_TI_CL430_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_CL430_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_CL430_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_ARMCL_VERSION)
    #undef JSON_HEDLEY_TI_ARMCL_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && (defined(__TMS470__) || defined(__TI_ARM__))
    #define JSON_HEDLEY_TI_ARMCL_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(JSON_HEDLEY_TI_ARMCL_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_ARMCL_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_ARMCL_VERSION)
    #define JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_ARMCL_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_CL6X_VERSION)
    #undef JSON_HEDLEY_TI_CL6X_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C6X__)
    #define JSON_HEDLEY_TI_CL6X_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(JSON_HEDLEY_TI_CL6X_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_CL6X_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_CL6X_VERSION)
    #define JSON_HEDLEY_TI_CL6X_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_CL6X_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_CL6X_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_CL7X_VERSION)
    #undef JSON_HEDLEY_TI_CL7X_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__C7000__)
    #define JSON_HEDLEY_TI_CL7X_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(JSON_HEDLEY_TI_CL7X_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_CL7X_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_CL7X_VERSION)
    #define JSON_HEDLEY_TI_CL7X_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_CL7X_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_CL7X_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TI_CLPRU_VERSION)
    #undef JSON_HEDLEY_TI_CLPRU_VERSION
#endif
#if defined(__TI_COMPILER_VERSION__) && defined(__PRU__)
    #define JSON_HEDLEY_TI_CLPRU_VERSION JSON_HEDLEY_VERSION_ENCODE(__TI_COMPILER_VERSION__ / 1000000, (__TI_COMPILER_VERSION__ % 1000000) / 1000, (__TI_COMPILER_VERSION__ % 1000))
#endif

#if defined(JSON_HEDLEY_TI_CLPRU_VERSION_CHECK)
    #undef JSON_HEDLEY_TI_CLPRU_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TI_CLPRU_VERSION)
    #define JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TI_CLPRU_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_CRAY_VERSION)
    #undef JSON_HEDLEY_CRAY_VERSION
#endif
#if defined(_CRAYC)
    #if defined(_RELEASE_PATCHLEVEL)
        #define JSON_HEDLEY_CRAY_VERSION JSON_HEDLEY_VERSION_ENCODE(_RELEASE_MAJOR, _RELEASE_MINOR, _RELEASE_PATCHLEVEL)
    #else
        #define JSON_HEDLEY_CRAY_VERSION JSON_HEDLEY_VERSION_ENCODE(_RELEASE_MAJOR, _RELEASE_MINOR, 0)
    #endif
#endif

#if defined(JSON_HEDLEY_CRAY_VERSION_CHECK)
    #undef JSON_HEDLEY_CRAY_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_CRAY_VERSION)
    #define JSON_HEDLEY_CRAY_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_CRAY_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_CRAY_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_IAR_VERSION)
    #undef JSON_HEDLEY_IAR_VERSION
#endif
#if defined(__IAR_SYSTEMS_ICC__)
    #if __VER__ > 1000
        #define JSON_HEDLEY_IAR_VERSION JSON_HEDLEY_VERSION_ENCODE((__VER__ / 1000000), ((__VER__ / 1000) % 1000), (__VER__ % 1000))
    #else
        #define JSON_HEDLEY_IAR_VERSION JSON_HEDLEY_VERSION_ENCODE(__VER__ / 100, __VER__ % 100, 0)
    #endif
#endif

#if defined(JSON_HEDLEY_IAR_VERSION_CHECK)
    #undef JSON_HEDLEY_IAR_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_IAR_VERSION)
    #define JSON_HEDLEY_IAR_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_IAR_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_IAR_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_TINYC_VERSION)
    #undef JSON_HEDLEY_TINYC_VERSION
#endif
#if defined(__TINYC__)
    #define JSON_HEDLEY_TINYC_VERSION JSON_HEDLEY_VERSION_ENCODE(__TINYC__ / 1000, (__TINYC__ / 100) % 10, __TINYC__ % 100)
#endif

#if defined(JSON_HEDLEY_TINYC_VERSION_CHECK)
    #undef JSON_HEDLEY_TINYC_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_TINYC_VERSION)
    #define JSON_HEDLEY_TINYC_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_TINYC_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_TINYC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_DMC_VERSION)
    #undef JSON_HEDLEY_DMC_VERSION
#endif
#if defined(__DMC__)
    #define JSON_HEDLEY_DMC_VERSION JSON_HEDLEY_VERSION_ENCODE(__DMC__ >> 8, (__DMC__ >> 4) & 0xf, __DMC__ & 0xf)
#endif

#if defined(JSON_HEDLEY_DMC_VERSION_CHECK)
    #undef JSON_HEDLEY_DMC_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_DMC_VERSION)
    #define JSON_HEDLEY_DMC_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_DMC_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_DMC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_COMPCERT_VERSION)
    #undef JSON_HEDLEY_COMPCERT_VERSION
#endif
#if defined(__COMPCERT_VERSION__)
    #define JSON_HEDLEY_COMPCERT_VERSION JSON_HEDLEY_VERSION_ENCODE(__COMPCERT_VERSION__ / 10000, (__COMPCERT_VERSION__ / 100) % 100, __COMPCERT_VERSION__ % 100)
#endif

#if defined(JSON_HEDLEY_COMPCERT_VERSION_CHECK)
    #undef JSON_HEDLEY_COMPCERT_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_COMPCERT_VERSION)
    #define JSON_HEDLEY_COMPCERT_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_COMPCERT_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_COMPCERT_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_PELLES_VERSION)
    #undef JSON_HEDLEY_PELLES_VERSION
#endif
#if defined(__POCC__)
    #define JSON_HEDLEY_PELLES_VERSION JSON_HEDLEY_VERSION_ENCODE(__POCC__ / 100, __POCC__ % 100, 0)
#endif

#if defined(JSON_HEDLEY_PELLES_VERSION_CHECK)
    #undef JSON_HEDLEY_PELLES_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_PELLES_VERSION)
    #define JSON_HEDLEY_PELLES_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_PELLES_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_PELLES_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_MCST_LCC_VERSION)
    #undef JSON_HEDLEY_MCST_LCC_VERSION
#endif
#if defined(__LCC__) && defined(__LCC_MINOR__)
    #define JSON_HEDLEY_MCST_LCC_VERSION JSON_HEDLEY_VERSION_ENCODE(__LCC__ / 100, __LCC__ % 100, __LCC_MINOR__)
#endif

#if defined(JSON_HEDLEY_MCST_LCC_VERSION_CHECK)
    #undef JSON_HEDLEY_MCST_LCC_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_MCST_LCC_VERSION)
    #define JSON_HEDLEY_MCST_LCC_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_MCST_LCC_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_MCST_LCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_GCC_VERSION)
    #undef JSON_HEDLEY_GCC_VERSION
#endif
#if \
    defined(JSON_HEDLEY_GNUC_VERSION) && \
    !defined(__clang__) && \
    !defined(JSON_HEDLEY_INTEL_VERSION) && \
    !defined(JSON_HEDLEY_PGI_VERSION) && \
    !defined(JSON_HEDLEY_ARM_VERSION) && \
    !defined(JSON_HEDLEY_CRAY_VERSION) && \
    !defined(JSON_HEDLEY_TI_VERSION) && \
    !defined(JSON_HEDLEY_TI_ARMCL_VERSION) && \
    !defined(JSON_HEDLEY_TI_CL430_VERSION) && \
    !defined(JSON_HEDLEY_TI_CL2000_VERSION) && \
    !defined(JSON_HEDLEY_TI_CL6X_VERSION) && \
    !defined(JSON_HEDLEY_TI_CL7X_VERSION) && \
    !defined(JSON_HEDLEY_TI_CLPRU_VERSION) && \
    !defined(__COMPCERT__) && \
    !defined(JSON_HEDLEY_MCST_LCC_VERSION)
    #define JSON_HEDLEY_GCC_VERSION JSON_HEDLEY_GNUC_VERSION
#endif

#if defined(JSON_HEDLEY_GCC_VERSION_CHECK)
    #undef JSON_HEDLEY_GCC_VERSION_CHECK
#endif
#if defined(JSON_HEDLEY_GCC_VERSION)
    #define JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch) (JSON_HEDLEY_GCC_VERSION >= JSON_HEDLEY_VERSION_ENCODE(major, minor, patch))
#else
    #define JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch) (0)
#endif

#if defined(JSON_HEDLEY_HAS_ATTRIBUTE)
    #undef JSON_HEDLEY_HAS_ATTRIBUTE
#endif
#if \
  defined(__has_attribute) && \
  ( \
    (!defined(JSON_HEDLEY_IAR_VERSION) || JSON_HEDLEY_IAR_VERSION_CHECK(8,5,9)) \
  )
#  define JSON_HEDLEY_HAS_ATTRIBUTE(attribute) __has_attribute(attribute)
#else
#  define JSON_HEDLEY_HAS_ATTRIBUTE(attribute) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_ATTRIBUTE)
    #undef JSON_HEDLEY_GNUC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
    #define JSON_HEDLEY_GNUC_HAS_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_HAS_ATTRIBUTE(attribute)
#else
    #define JSON_HEDLEY_GNUC_HAS_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_ATTRIBUTE)
    #undef JSON_HEDLEY_GCC_HAS_ATTRIBUTE
#endif
#if defined(__has_attribute)
    #define JSON_HEDLEY_GCC_HAS_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_HAS_ATTRIBUTE(attribute)
#else
    #define JSON_HEDLEY_GCC_HAS_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_HAS_CPP_ATTRIBUTE)
    #undef JSON_HEDLEY_HAS_CPP_ATTRIBUTE
#endif
#if \
    defined(__has_cpp_attribute) && \
    defined(__cplusplus) && \
    (!defined(JSON_HEDLEY_SUNPRO_VERSION) || JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,15,0))
    #define JSON_HEDLEY_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
    #define JSON_HEDLEY_HAS_CPP_ATTRIBUTE(attribute) (0)
#endif

#if defined(JSON_HEDLEY_HAS_CPP_ATTRIBUTE_NS)
    #undef JSON_HEDLEY_HAS_CPP_ATTRIBUTE_NS
#endif
#if !defined(__cplusplus) || !defined(__has_cpp_attribute)
    #define JSON_HEDLEY_HAS_CPP_ATTRIBUTE_NS(ns,attribute) (0)
#elif \
    !defined(JSON_HEDLEY_PGI_VERSION) && \
    !defined(JSON_HEDLEY_IAR_VERSION) && \
    (!defined(JSON_HEDLEY_SUNPRO_VERSION) || JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,15,0)) && \
    (!defined(JSON_HEDLEY_MSVC_VERSION) || JSON_HEDLEY_MSVC_VERSION_CHECK(19,20,0))
    #define JSON_HEDLEY_HAS_CPP_ATTRIBUTE_NS(ns,attribute) JSON_HEDLEY_HAS_CPP_ATTRIBUTE(ns::attribute)
#else
    #define JSON_HEDLEY_HAS_CPP_ATTRIBUTE_NS(ns,attribute) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_CPP_ATTRIBUTE)
    #undef JSON_HEDLEY_GNUC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
    #define JSON_HEDLEY_GNUC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) __has_cpp_attribute(attribute)
#else
    #define JSON_HEDLEY_GNUC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_CPP_ATTRIBUTE)
    #undef JSON_HEDLEY_GCC_HAS_CPP_ATTRIBUTE
#endif
#if defined(__has_cpp_attribute) && defined(__cplusplus)
    #define JSON_HEDLEY_GCC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) __has_cpp_attribute(attribute)
#else
    #define JSON_HEDLEY_GCC_HAS_CPP_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_HAS_BUILTIN)
    #undef JSON_HEDLEY_HAS_BUILTIN
#endif
#if defined(__has_builtin)
    #define JSON_HEDLEY_HAS_BUILTIN(builtin) __has_builtin(builtin)
#else
    #define JSON_HEDLEY_HAS_BUILTIN(builtin) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_BUILTIN)
    #undef JSON_HEDLEY_GNUC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
    #define JSON_HEDLEY_GNUC_HAS_BUILTIN(builtin,major,minor,patch) __has_builtin(builtin)
#else
    #define JSON_HEDLEY_GNUC_HAS_BUILTIN(builtin,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_BUILTIN)
    #undef JSON_HEDLEY_GCC_HAS_BUILTIN
#endif
#if defined(__has_builtin)
    #define JSON_HEDLEY_GCC_HAS_BUILTIN(builtin,major,minor,patch) __has_builtin(builtin)
#else
    #define JSON_HEDLEY_GCC_HAS_BUILTIN(builtin,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_HAS_FEATURE)
    #undef JSON_HEDLEY_HAS_FEATURE
#endif
#if defined(__has_feature)
    #define JSON_HEDLEY_HAS_FEATURE(feature) __has_feature(feature)
#else
    #define JSON_HEDLEY_HAS_FEATURE(feature) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_FEATURE)
    #undef JSON_HEDLEY_GNUC_HAS_FEATURE
#endif
#if defined(__has_feature)
    #define JSON_HEDLEY_GNUC_HAS_FEATURE(feature,major,minor,patch) __has_feature(feature)
#else
    #define JSON_HEDLEY_GNUC_HAS_FEATURE(feature,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_FEATURE)
    #undef JSON_HEDLEY_GCC_HAS_FEATURE
#endif
#if defined(__has_feature)
    #define JSON_HEDLEY_GCC_HAS_FEATURE(feature,major,minor,patch) __has_feature(feature)
#else
    #define JSON_HEDLEY_GCC_HAS_FEATURE(feature,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_HAS_EXTENSION)
    #undef JSON_HEDLEY_HAS_EXTENSION
#endif
#if defined(__has_extension)
    #define JSON_HEDLEY_HAS_EXTENSION(extension) __has_extension(extension)
#else
    #define JSON_HEDLEY_HAS_EXTENSION(extension) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_EXTENSION)
    #undef JSON_HEDLEY_GNUC_HAS_EXTENSION
#endif
#if defined(__has_extension)
    #define JSON_HEDLEY_GNUC_HAS_EXTENSION(extension,major,minor,patch) __has_extension(extension)
#else
    #define JSON_HEDLEY_GNUC_HAS_EXTENSION(extension,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_EXTENSION)
    #undef JSON_HEDLEY_GCC_HAS_EXTENSION
#endif
#if defined(__has_extension)
    #define JSON_HEDLEY_GCC_HAS_EXTENSION(extension,major,minor,patch) __has_extension(extension)
#else
    #define JSON_HEDLEY_GCC_HAS_EXTENSION(extension,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_HAS_DECLSPEC_ATTRIBUTE)
    #undef JSON_HEDLEY_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
    #define JSON_HEDLEY_HAS_DECLSPEC_ATTRIBUTE(attribute) __has_declspec_attribute(attribute)
#else
    #define JSON_HEDLEY_HAS_DECLSPEC_ATTRIBUTE(attribute) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_DECLSPEC_ATTRIBUTE)
    #undef JSON_HEDLEY_GNUC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
    #define JSON_HEDLEY_GNUC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) __has_declspec_attribute(attribute)
#else
    #define JSON_HEDLEY_GNUC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_DECLSPEC_ATTRIBUTE)
    #undef JSON_HEDLEY_GCC_HAS_DECLSPEC_ATTRIBUTE
#endif
#if defined(__has_declspec_attribute)
    #define JSON_HEDLEY_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) __has_declspec_attribute(attribute)
#else
    #define JSON_HEDLEY_GCC_HAS_DECLSPEC_ATTRIBUTE(attribute,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_HAS_WARNING)
    #undef JSON_HEDLEY_HAS_WARNING
#endif
#if defined(__has_warning)
    #define JSON_HEDLEY_HAS_WARNING(warning) __has_warning(warning)
#else
    #define JSON_HEDLEY_HAS_WARNING(warning) (0)
#endif

#if defined(JSON_HEDLEY_GNUC_HAS_WARNING)
    #undef JSON_HEDLEY_GNUC_HAS_WARNING
#endif
#if defined(__has_warning)
    #define JSON_HEDLEY_GNUC_HAS_WARNING(warning,major,minor,patch) __has_warning(warning)
#else
    #define JSON_HEDLEY_GNUC_HAS_WARNING(warning,major,minor,patch) JSON_HEDLEY_GNUC_VERSION_CHECK(major,minor,patch)
#endif

#if defined(JSON_HEDLEY_GCC_HAS_WARNING)
    #undef JSON_HEDLEY_GCC_HAS_WARNING
#endif
#if defined(__has_warning)
    #define JSON_HEDLEY_GCC_HAS_WARNING(warning,major,minor,patch) __has_warning(warning)
#else
    #define JSON_HEDLEY_GCC_HAS_WARNING(warning,major,minor,patch) JSON_HEDLEY_GCC_VERSION_CHECK(major,minor,patch)
#endif

#if \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
    defined(__clang__) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,0,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0) || \
    JSON_HEDLEY_PGI_VERSION_CHECK(18,4,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,7,0) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(2,0,1) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,1,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,0,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_CRAY_VERSION_CHECK(5,0,0) || \
    JSON_HEDLEY_TINYC_VERSION_CHECK(0,9,17) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(8,0,0) || \
    (JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) && defined(__C99_PRAGMA_OPERATOR))
    #define JSON_HEDLEY_PRAGMA(value) _Pragma(#value)
#elif JSON_HEDLEY_MSVC_VERSION_CHECK(15,0,0)
    #define JSON_HEDLEY_PRAGMA(value) __pragma(value)
#else
    #define JSON_HEDLEY_PRAGMA(value)
#endif

#if defined(JSON_HEDLEY_DIAGNOSTIC_PUSH)
    #undef JSON_HEDLEY_DIAGNOSTIC_PUSH
#endif
#if defined(JSON_HEDLEY_DIAGNOSTIC_POP)
    #undef JSON_HEDLEY_DIAGNOSTIC_POP
#endif
#if defined(__clang__)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
    #define JSON_HEDLEY_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#elif JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH _Pragma("warning(push)")
    #define JSON_HEDLEY_DIAGNOSTIC_POP _Pragma("warning(pop)")
#elif JSON_HEDLEY_GCC_VERSION_CHECK(4,6,0)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
    #define JSON_HEDLEY_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(15,0,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH __pragma(warning(push))
    #define JSON_HEDLEY_DIAGNOSTIC_POP __pragma(warning(pop))
#elif JSON_HEDLEY_ARM_VERSION_CHECK(5,6,0)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH _Pragma("push")
    #define JSON_HEDLEY_DIAGNOSTIC_POP _Pragma("pop")
#elif \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,4,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,1,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH _Pragma("diag_push")
    #define JSON_HEDLEY_DIAGNOSTIC_POP _Pragma("diag_pop")
#elif JSON_HEDLEY_PELLES_VERSION_CHECK(2,90,0)
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH _Pragma("warning(push)")
    #define JSON_HEDLEY_DIAGNOSTIC_POP _Pragma("warning(pop)")
#else
    #define JSON_HEDLEY_DIAGNOSTIC_PUSH
    #define JSON_HEDLEY_DIAGNOSTIC_POP
#endif

/* JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_ is for
   HEDLEY INTERNAL USE ONLY.  API subject to change without notice. */
#if defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_)
    #undef JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_
#endif
#if defined(__cplusplus)
#  if JSON_HEDLEY_HAS_WARNING("-Wc++98-compat")
#    if JSON_HEDLEY_HAS_WARNING("-Wc++17-extensions")
#      if JSON_HEDLEY_HAS_WARNING("-Wc++1z-extensions")
#        define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
    JSON_HEDLEY_DIAGNOSTIC_PUSH \
    _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
    _Pragma("clang diagnostic ignored \"-Wc++17-extensions\"") \
    _Pragma("clang diagnostic ignored \"-Wc++1z-extensions\"") \
    xpr \
    JSON_HEDLEY_DIAGNOSTIC_POP
#      else
#        define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
    JSON_HEDLEY_DIAGNOSTIC_PUSH \
    _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
    _Pragma("clang diagnostic ignored \"-Wc++17-extensions\"") \
    xpr \
    JSON_HEDLEY_DIAGNOSTIC_POP
#      endif
#    else
#      define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(xpr) \
    JSON_HEDLEY_DIAGNOSTIC_PUSH \
    _Pragma("clang diagnostic ignored \"-Wc++98-compat\"") \
    xpr \
    JSON_HEDLEY_DIAGNOSTIC_POP
#    endif
#  endif
#endif
#if !defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(x) x
#endif

#if defined(JSON_HEDLEY_CONST_CAST)
    #undef JSON_HEDLEY_CONST_CAST
#endif
#if defined(__cplusplus)
#  define JSON_HEDLEY_CONST_CAST(T, expr) (const_cast<T>(expr))
#elif \
  JSON_HEDLEY_HAS_WARNING("-Wcast-qual") || \
  JSON_HEDLEY_GCC_VERSION_CHECK(4,6,0) || \
  JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0)
#  define JSON_HEDLEY_CONST_CAST(T, expr) (__extension__ ({ \
        JSON_HEDLEY_DIAGNOSTIC_PUSH \
        JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL \
        ((T) (expr)); \
        JSON_HEDLEY_DIAGNOSTIC_POP \
    }))
#else
#  define JSON_HEDLEY_CONST_CAST(T, expr) ((T) (expr))
#endif

#if defined(JSON_HEDLEY_REINTERPRET_CAST)
    #undef JSON_HEDLEY_REINTERPRET_CAST
#endif
#if defined(__cplusplus)
    #define JSON_HEDLEY_REINTERPRET_CAST(T, expr) (reinterpret_cast<T>(expr))
#else
    #define JSON_HEDLEY_REINTERPRET_CAST(T, expr) ((T) (expr))
#endif

#if defined(JSON_HEDLEY_STATIC_CAST)
    #undef JSON_HEDLEY_STATIC_CAST
#endif
#if defined(__cplusplus)
    #define JSON_HEDLEY_STATIC_CAST(T, expr) (static_cast<T>(expr))
#else
    #define JSON_HEDLEY_STATIC_CAST(T, expr) ((T) (expr))
#endif

#if defined(JSON_HEDLEY_CPP_CAST)
    #undef JSON_HEDLEY_CPP_CAST
#endif
#if defined(__cplusplus)
#  if JSON_HEDLEY_HAS_WARNING("-Wold-style-cast")
#    define JSON_HEDLEY_CPP_CAST(T, expr) \
    JSON_HEDLEY_DIAGNOSTIC_PUSH \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    ((T) (expr)) \
    JSON_HEDLEY_DIAGNOSTIC_POP
#  elif JSON_HEDLEY_IAR_VERSION_CHECK(8,3,0)
#    define JSON_HEDLEY_CPP_CAST(T, expr) \
    JSON_HEDLEY_DIAGNOSTIC_PUSH \
    _Pragma("diag_suppress=Pe137") \
    JSON_HEDLEY_DIAGNOSTIC_POP
#  else
#    define JSON_HEDLEY_CPP_CAST(T, expr) ((T) (expr))
#  endif
#else
#  define JSON_HEDLEY_CPP_CAST(T, expr) (expr)
#endif

#if defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED)
    #undef JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
#endif
#if JSON_HEDLEY_HAS_WARNING("-Wdeprecated-declarations")
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#elif JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("warning(disable:1478 1786)")
#elif JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED __pragma(warning(disable:1478 1786))
#elif JSON_HEDLEY_PGI_VERSION_CHECK(20,7,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1215,1216,1444,1445")
#elif JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1215,1444")
#elif JSON_HEDLEY_GCC_VERSION_CHECK(4,3,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif JSON_HEDLEY_MSVC_VERSION_CHECK(15,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED __pragma(warning(disable:4996))
#elif JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1215,1444")
#elif \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress 1291,1718")
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,13,0) && !defined(__cplusplus)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("error_messages(off,E_DEPRECATED_ATT,E_DEPRECATED_ATT_MESS)")
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,13,0) && defined(__cplusplus)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("error_messages(off,symdeprecated,symdeprecated2)")
#elif JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("diag_suppress=Pe1444,Pe1215")
#elif JSON_HEDLEY_PELLES_VERSION_CHECK(2,90,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED _Pragma("warn(disable:2241)")
#else
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_DEPRECATED
#endif

#if defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS)
    #undef JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif
#if JSON_HEDLEY_HAS_WARNING("-Wunknown-pragmas")
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("clang diagnostic ignored \"-Wunknown-pragmas\"")
#elif JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("warning(disable:161)")
#elif JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS __pragma(warning(disable:161))
#elif JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 1675")
#elif JSON_HEDLEY_GCC_VERSION_CHECK(4,3,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("GCC diagnostic ignored \"-Wunknown-pragmas\"")
#elif JSON_HEDLEY_MSVC_VERSION_CHECK(15,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS __pragma(warning(disable:4068))
#elif \
    JSON_HEDLEY_TI_VERSION_CHECK(16,9,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,0,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,3,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 163")
#elif JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 163")
#elif JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress=Pe161")
#elif JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS _Pragma("diag_suppress 161")
#else
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_PRAGMAS
#endif

#if defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES)
    #undef JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES
#endif
#if JSON_HEDLEY_HAS_WARNING("-Wunknown-attributes")
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("clang diagnostic ignored \"-Wunknown-attributes\"")
#elif JSON_HEDLEY_GCC_VERSION_CHECK(4,6,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif JSON_HEDLEY_INTEL_VERSION_CHECK(17,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("warning(disable:1292)")
#elif JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES __pragma(warning(disable:1292))
#elif JSON_HEDLEY_MSVC_VERSION_CHECK(19,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES __pragma(warning(disable:5030))
#elif JSON_HEDLEY_PGI_VERSION_CHECK(20,7,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1097,1098")
#elif JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1097")
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,14,0) && defined(__cplusplus)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("error_messages(off,attrskipunsup)")
#elif \
    JSON_HEDLEY_TI_VERSION_CHECK(18,1,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,3,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1173")
#elif JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress=Pe1097")
#elif JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES _Pragma("diag_suppress 1097")
#else
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNKNOWN_CPP_ATTRIBUTES
#endif

#if defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL)
    #undef JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL
#endif
#if JSON_HEDLEY_HAS_WARNING("-Wcast-qual")
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL _Pragma("clang diagnostic ignored \"-Wcast-qual\"")
#elif JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL _Pragma("warning(disable:2203 2331)")
#elif JSON_HEDLEY_GCC_VERSION_CHECK(3,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL _Pragma("GCC diagnostic ignored \"-Wcast-qual\"")
#else
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_CAST_QUAL
#endif

#if defined(JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION)
    #undef JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION
#endif
#if JSON_HEDLEY_HAS_WARNING("-Wunused-function")
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION _Pragma("clang diagnostic ignored \"-Wunused-function\"")
#elif JSON_HEDLEY_GCC_VERSION_CHECK(3,4,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#elif JSON_HEDLEY_MSVC_VERSION_CHECK(1,0,0)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION __pragma(warning(disable:4505))
#elif JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION _Pragma("diag_suppress 3142")
#else
    #define JSON_HEDLEY_DIAGNOSTIC_DISABLE_UNUSED_FUNCTION
#endif

#if defined(JSON_HEDLEY_DEPRECATED)
    #undef JSON_HEDLEY_DEPRECATED
#endif
#if defined(JSON_HEDLEY_DEPRECATED_FOR)
    #undef JSON_HEDLEY_DEPRECATED_FOR
#endif
#if \
    JSON_HEDLEY_MSVC_VERSION_CHECK(14,0,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_DEPRECATED(since) __declspec(deprecated("Since " # since))
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement) __declspec(deprecated("Since " #since "; use " #replacement))
#elif \
    (JSON_HEDLEY_HAS_EXTENSION(attribute_deprecated_with_message) && !defined(JSON_HEDLEY_IAR_VERSION)) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(4,5,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(5,6,0) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,13,0) || \
    JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(18,1,0) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(18,1,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,3,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,3,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__("Since " #since "; use " #replacement)))
#elif defined(__cplusplus) && (__cplusplus >= 201402L)
    #define JSON_HEDLEY_DEPRECATED(since) JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[deprecated("Since " #since)]])
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement) JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[deprecated("Since " #since "; use " #replacement)]])
#elif \
    JSON_HEDLEY_HAS_ATTRIBUTE(deprecated) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,1,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10) || \
    JSON_HEDLEY_IAR_VERSION_CHECK(8,10,0)
    #define JSON_HEDLEY_DEPRECATED(since) __attribute__((__deprecated__))
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement) __attribute__((__deprecated__))
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(13,10,0) || \
    JSON_HEDLEY_PELLES_VERSION_CHECK(6,50,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_DEPRECATED(since) __declspec(deprecated)
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement) __declspec(deprecated)
#elif JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_DEPRECATED(since) _Pragma("deprecated")
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement) _Pragma("deprecated")
#else
    #define JSON_HEDLEY_DEPRECATED(since)
    #define JSON_HEDLEY_DEPRECATED_FOR(since, replacement)
#endif

#if defined(JSON_HEDLEY_UNAVAILABLE)
    #undef JSON_HEDLEY_UNAVAILABLE
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(warning) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(4,3,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_UNAVAILABLE(available_since) __attribute__((__warning__("Not available until " #available_since)))
#else
    #define JSON_HEDLEY_UNAVAILABLE(available_since)
#endif

#if defined(JSON_HEDLEY_WARN_UNUSED_RESULT)
    #undef JSON_HEDLEY_WARN_UNUSED_RESULT
#endif
#if defined(JSON_HEDLEY_WARN_UNUSED_RESULT_MSG)
    #undef JSON_HEDLEY_WARN_UNUSED_RESULT_MSG
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(warn_unused_result) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,4,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    (JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,15,0) && defined(__cplusplus)) || \
    JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
    #define JSON_HEDLEY_WARN_UNUSED_RESULT_MSG(msg) __attribute__((__warn_unused_result__))
#elif (JSON_HEDLEY_HAS_CPP_ATTRIBUTE(nodiscard) >= 201907L)
    #define JSON_HEDLEY_WARN_UNUSED_RESULT JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
    #define JSON_HEDLEY_WARN_UNUSED_RESULT_MSG(msg) JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard(msg)]])
#elif JSON_HEDLEY_HAS_CPP_ATTRIBUTE(nodiscard)
    #define JSON_HEDLEY_WARN_UNUSED_RESULT JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
    #define JSON_HEDLEY_WARN_UNUSED_RESULT_MSG(msg) JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[nodiscard]])
#elif defined(_Check_return_) /* SAL */
    #define JSON_HEDLEY_WARN_UNUSED_RESULT _Check_return_
    #define JSON_HEDLEY_WARN_UNUSED_RESULT_MSG(msg) _Check_return_
#else
    #define JSON_HEDLEY_WARN_UNUSED_RESULT
    #define JSON_HEDLEY_WARN_UNUSED_RESULT_MSG(msg)
#endif

#if defined(JSON_HEDLEY_SENTINEL)
    #undef JSON_HEDLEY_SENTINEL
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(sentinel) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(4,0,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(5,4,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_SENTINEL(position) __attribute__((__sentinel__(position)))
#else
    #define JSON_HEDLEY_SENTINEL(position)
#endif

#if defined(JSON_HEDLEY_NO_RETURN)
    #undef JSON_HEDLEY_NO_RETURN
#endif
#if JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_NO_RETURN __noreturn
#elif \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_NO_RETURN __attribute__((__noreturn__))
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define JSON_HEDLEY_NO_RETURN _Noreturn
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
    #define JSON_HEDLEY_NO_RETURN JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[noreturn]])
#elif \
    JSON_HEDLEY_HAS_ATTRIBUTE(noreturn) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,2,0) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_IAR_VERSION_CHECK(8,10,0)
    #define JSON_HEDLEY_NO_RETURN __attribute__((__noreturn__))
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,10,0)
    #define JSON_HEDLEY_NO_RETURN _Pragma("does_not_return")
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(13,10,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_NO_RETURN __declspec(noreturn)
#elif JSON_HEDLEY_TI_CL6X_VERSION_CHECK(6,0,0) && defined(__cplusplus)
    #define JSON_HEDLEY_NO_RETURN _Pragma("FUNC_NEVER_RETURNS;")
#elif JSON_HEDLEY_COMPCERT_VERSION_CHECK(3,2,0)
    #define JSON_HEDLEY_NO_RETURN __attribute((noreturn))
#elif JSON_HEDLEY_PELLES_VERSION_CHECK(9,0,0)
    #define JSON_HEDLEY_NO_RETURN __declspec(noreturn)
#else
    #define JSON_HEDLEY_NO_RETURN
#endif

#if defined(JSON_HEDLEY_NO_ESCAPE)
    #undef JSON_HEDLEY_NO_ESCAPE
#endif
#if JSON_HEDLEY_HAS_ATTRIBUTE(noescape)
    #define JSON_HEDLEY_NO_ESCAPE __attribute__((__noescape__))
#else
    #define JSON_HEDLEY_NO_ESCAPE
#endif

#if defined(JSON_HEDLEY_UNREACHABLE)
    #undef JSON_HEDLEY_UNREACHABLE
#endif
#if defined(JSON_HEDLEY_UNREACHABLE_RETURN)
    #undef JSON_HEDLEY_UNREACHABLE_RETURN
#endif
#if defined(JSON_HEDLEY_ASSUME)
    #undef JSON_HEDLEY_ASSUME
#endif
#if \
    JSON_HEDLEY_MSVC_VERSION_CHECK(13,10,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_ASSUME(expr) __assume(expr)
#elif JSON_HEDLEY_HAS_BUILTIN(__builtin_assume)
    #define JSON_HEDLEY_ASSUME(expr) __builtin_assume(expr)
#elif \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,2,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(4,0,0)
    #if defined(__cplusplus)
        #define JSON_HEDLEY_ASSUME(expr) std::_nassert(expr)
    #else
        #define JSON_HEDLEY_ASSUME(expr) _nassert(expr)
    #endif
#endif
#if \
    (JSON_HEDLEY_HAS_BUILTIN(__builtin_unreachable) && (!defined(JSON_HEDLEY_ARM_VERSION))) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(4,5,0) || \
    JSON_HEDLEY_PGI_VERSION_CHECK(18,10,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(13,1,5) || \
    JSON_HEDLEY_CRAY_VERSION_CHECK(10,0,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_UNREACHABLE() __builtin_unreachable()
#elif defined(JSON_HEDLEY_ASSUME)
    #define JSON_HEDLEY_UNREACHABLE() JSON_HEDLEY_ASSUME(0)
#endif
#if !defined(JSON_HEDLEY_ASSUME)
    #if defined(JSON_HEDLEY_UNREACHABLE)
        #define JSON_HEDLEY_ASSUME(expr) JSON_HEDLEY_STATIC_CAST(void, ((expr) ? 1 : (JSON_HEDLEY_UNREACHABLE(), 1)))
    #else
        #define JSON_HEDLEY_ASSUME(expr) JSON_HEDLEY_STATIC_CAST(void, expr)
    #endif
#endif
#if defined(JSON_HEDLEY_UNREACHABLE)
    #if  \
        JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,2,0) || \
        JSON_HEDLEY_TI_CL6X_VERSION_CHECK(4,0,0)
        #define JSON_HEDLEY_UNREACHABLE_RETURN(value) return (JSON_HEDLEY_STATIC_CAST(void, JSON_HEDLEY_ASSUME(0)), (value))
    #else
        #define JSON_HEDLEY_UNREACHABLE_RETURN(value) JSON_HEDLEY_UNREACHABLE()
    #endif
#else
    #define JSON_HEDLEY_UNREACHABLE_RETURN(value) return (value)
#endif
#if !defined(JSON_HEDLEY_UNREACHABLE)
    #define JSON_HEDLEY_UNREACHABLE() JSON_HEDLEY_ASSUME(0)
#endif

JSON_HEDLEY_DIAGNOSTIC_PUSH
#if JSON_HEDLEY_HAS_WARNING("-Wpedantic")
    #pragma clang diagnostic ignored "-Wpedantic"
#endif
#if JSON_HEDLEY_HAS_WARNING("-Wc++98-compat-pedantic") && defined(__cplusplus)
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#if JSON_HEDLEY_GCC_HAS_WARNING("-Wvariadic-macros",4,0,0)
    #if defined(__clang__)
        #pragma clang diagnostic ignored "-Wvariadic-macros"
    #elif defined(JSON_HEDLEY_GCC_VERSION)
        #pragma GCC diagnostic ignored "-Wvariadic-macros"
    #endif
#endif
#if defined(JSON_HEDLEY_NON_NULL)
    #undef JSON_HEDLEY_NON_NULL
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(nonnull) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,3,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0)
    #define JSON_HEDLEY_NON_NULL(...) __attribute__((__nonnull__(__VA_ARGS__)))
#else
    #define JSON_HEDLEY_NON_NULL(...)
#endif
JSON_HEDLEY_DIAGNOSTIC_POP

#if defined(JSON_HEDLEY_PRINTF_FORMAT)
    #undef JSON_HEDLEY_PRINTF_FORMAT
#endif
#if defined(__MINGW32__) && JSON_HEDLEY_GCC_HAS_ATTRIBUTE(format,4,4,0) && !defined(__USE_MINGW_ANSI_STDIO)
    #define JSON_HEDLEY_PRINTF_FORMAT(string_idx,first_to_check) __attribute__((__format__(ms_printf, string_idx, first_to_check)))
#elif defined(__MINGW32__) && JSON_HEDLEY_GCC_HAS_ATTRIBUTE(format,4,4,0) && defined(__USE_MINGW_ANSI_STDIO)
    #define JSON_HEDLEY_PRINTF_FORMAT(string_idx,first_to_check) __attribute__((__format__(gnu_printf, string_idx, first_to_check)))
#elif \
    JSON_HEDLEY_HAS_ATTRIBUTE(format) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,1,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(5,6,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_PRINTF_FORMAT(string_idx,first_to_check) __attribute__((__format__(__printf__, string_idx, first_to_check)))
#elif JSON_HEDLEY_PELLES_VERSION_CHECK(6,0,0)
    #define JSON_HEDLEY_PRINTF_FORMAT(string_idx,first_to_check) __declspec(vaformat(printf,string_idx,first_to_check))
#else
    #define JSON_HEDLEY_PRINTF_FORMAT(string_idx,first_to_check)
#endif

#if defined(JSON_HEDLEY_CONSTEXPR)
    #undef JSON_HEDLEY_CONSTEXPR
#endif
#if defined(__cplusplus)
    #if __cplusplus >= 201103L
        #define JSON_HEDLEY_CONSTEXPR JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_(constexpr)
    #endif
#endif
#if !defined(JSON_HEDLEY_CONSTEXPR)
    #define JSON_HEDLEY_CONSTEXPR
#endif

#if defined(JSON_HEDLEY_PREDICT)
    #undef JSON_HEDLEY_PREDICT
#endif
#if defined(JSON_HEDLEY_LIKELY)
    #undef JSON_HEDLEY_LIKELY
#endif
#if defined(JSON_HEDLEY_UNLIKELY)
    #undef JSON_HEDLEY_UNLIKELY
#endif
#if defined(JSON_HEDLEY_UNPREDICTABLE)
    #undef JSON_HEDLEY_UNPREDICTABLE
#endif
#if JSON_HEDLEY_HAS_BUILTIN(__builtin_unpredictable)
    #define JSON_HEDLEY_UNPREDICTABLE(expr) __builtin_unpredictable((expr))
#endif
#if \
  (JSON_HEDLEY_HAS_BUILTIN(__builtin_expect_with_probability) && !defined(JSON_HEDLEY_PGI_VERSION)) || \
  JSON_HEDLEY_GCC_VERSION_CHECK(9,0,0) || \
  JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
#  define JSON_HEDLEY_PREDICT(expr, value, probability) __builtin_expect_with_probability(  (expr), (value), (probability))
#  define JSON_HEDLEY_PREDICT_TRUE(expr, probability)   __builtin_expect_with_probability(!!(expr),    1   , (probability))
#  define JSON_HEDLEY_PREDICT_FALSE(expr, probability)  __builtin_expect_with_probability(!!(expr),    0   , (probability))
#  define JSON_HEDLEY_LIKELY(expr)                      __builtin_expect                 (!!(expr),    1                  )
#  define JSON_HEDLEY_UNLIKELY(expr)                    __builtin_expect                 (!!(expr),    0                  )
#elif \
  (JSON_HEDLEY_HAS_BUILTIN(__builtin_expect) && !defined(JSON_HEDLEY_INTEL_CL_VERSION)) || \
  JSON_HEDLEY_GCC_VERSION_CHECK(3,0,0) || \
  JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
  (JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,15,0) && defined(__cplusplus)) || \
  JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
  JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
  JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
  JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,7,0) || \
  JSON_HEDLEY_TI_CL430_VERSION_CHECK(3,1,0) || \
  JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,1,0) || \
  JSON_HEDLEY_TI_CL6X_VERSION_CHECK(6,1,0) || \
  JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
  JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  JSON_HEDLEY_TINYC_VERSION_CHECK(0,9,27) || \
  JSON_HEDLEY_CRAY_VERSION_CHECK(8,1,0) || \
  JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
#  define JSON_HEDLEY_PREDICT(expr, expected, probability) \
    (((probability) >= 0.9) ? __builtin_expect((expr), (expected)) : (JSON_HEDLEY_STATIC_CAST(void, expected), (expr)))
#  define JSON_HEDLEY_PREDICT_TRUE(expr, probability) \
    (__extension__ ({ \
        double hedley_probability_ = (probability); \
        ((hedley_probability_ >= 0.9) ? __builtin_expect(!!(expr), 1) : ((hedley_probability_ <= 0.1) ? __builtin_expect(!!(expr), 0) : !!(expr))); \
    }))
#  define JSON_HEDLEY_PREDICT_FALSE(expr, probability) \
    (__extension__ ({ \
        double hedley_probability_ = (probability); \
        ((hedley_probability_ >= 0.9) ? __builtin_expect(!!(expr), 0) : ((hedley_probability_ <= 0.1) ? __builtin_expect(!!(expr), 1) : !!(expr))); \
    }))
#  define JSON_HEDLEY_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#  define JSON_HEDLEY_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#  define JSON_HEDLEY_PREDICT(expr, expected, probability) (JSON_HEDLEY_STATIC_CAST(void, expected), (expr))
#  define JSON_HEDLEY_PREDICT_TRUE(expr, probability) (!!(expr))
#  define JSON_HEDLEY_PREDICT_FALSE(expr, probability) (!!(expr))
#  define JSON_HEDLEY_LIKELY(expr) (!!(expr))
#  define JSON_HEDLEY_UNLIKELY(expr) (!!(expr))
#endif
#if !defined(JSON_HEDLEY_UNPREDICTABLE)
    #define JSON_HEDLEY_UNPREDICTABLE(expr) JSON_HEDLEY_PREDICT(expr, 1, 0.5)
#endif

#if defined(JSON_HEDLEY_MALLOC)
    #undef JSON_HEDLEY_MALLOC
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(malloc) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,1,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(12,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_MALLOC __attribute__((__malloc__))
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,10,0)
    #define JSON_HEDLEY_MALLOC _Pragma("returns_new_memory")
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(14,0,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_MALLOC __declspec(restrict)
#else
    #define JSON_HEDLEY_MALLOC
#endif

#if defined(JSON_HEDLEY_PURE)
    #undef JSON_HEDLEY_PURE
#endif
#if \
  JSON_HEDLEY_HAS_ATTRIBUTE(pure) || \
  JSON_HEDLEY_GCC_VERSION_CHECK(2,96,0) || \
  JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
  JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
  JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
  JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
  JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
  (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
  (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
  JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
  JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0) || \
  JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
#  define JSON_HEDLEY_PURE __attribute__((__pure__))
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,10,0)
#  define JSON_HEDLEY_PURE _Pragma("does_not_write_global_data")
#elif defined(__cplusplus) && \
    ( \
      JSON_HEDLEY_TI_CL430_VERSION_CHECK(2,0,1) || \
      JSON_HEDLEY_TI_CL6X_VERSION_CHECK(4,0,0) || \
      JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) \
    )
#  define JSON_HEDLEY_PURE _Pragma("FUNC_IS_PURE;")
#else
#  define JSON_HEDLEY_PURE
#endif

#if defined(JSON_HEDLEY_CONST)
    #undef JSON_HEDLEY_CONST
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(const) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(2,5,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_CONST __attribute__((__const__))
#elif \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,10,0)
    #define JSON_HEDLEY_CONST _Pragma("no_side_effect")
#else
    #define JSON_HEDLEY_CONST JSON_HEDLEY_PURE
#endif

#if defined(JSON_HEDLEY_RESTRICT)
    #undef JSON_HEDLEY_RESTRICT
#endif
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && !defined(__cplusplus)
    #define JSON_HEDLEY_RESTRICT restrict
#elif \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,1,0) || \
    JSON_HEDLEY_MSVC_VERSION_CHECK(14,0,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
    JSON_HEDLEY_PGI_VERSION_CHECK(17,10,0) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,2,4) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,1,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    (JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,14,0) && defined(__cplusplus)) || \
    JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0) || \
    defined(__clang__) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_RESTRICT __restrict
#elif JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,3,0) && !defined(__cplusplus)
    #define JSON_HEDLEY_RESTRICT _Restrict
#else
    #define JSON_HEDLEY_RESTRICT
#endif

#if defined(JSON_HEDLEY_INLINE)
    #undef JSON_HEDLEY_INLINE
#endif
#if \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || \
    (defined(__cplusplus) && (__cplusplus >= 199711L))
    #define JSON_HEDLEY_INLINE inline
#elif \
    defined(JSON_HEDLEY_GCC_VERSION) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(6,2,0)
    #define JSON_HEDLEY_INLINE __inline__
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(12,0,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,1,0) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(3,1,0) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,2,0) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(8,0,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_INLINE __inline
#else
    #define JSON_HEDLEY_INLINE
#endif

#if defined(JSON_HEDLEY_ALWAYS_INLINE)
    #undef JSON_HEDLEY_ALWAYS_INLINE
#endif
#if \
  JSON_HEDLEY_HAS_ATTRIBUTE(always_inline) || \
  JSON_HEDLEY_GCC_VERSION_CHECK(4,0,0) || \
  JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
  JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
  JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
  JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
  JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
  (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
  (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
  (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
  (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
  JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
  JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
  JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
  JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10) || \
  JSON_HEDLEY_IAR_VERSION_CHECK(8,10,0)
#  define JSON_HEDLEY_ALWAYS_INLINE __attribute__((__always_inline__)) JSON_HEDLEY_INLINE
#elif \
  JSON_HEDLEY_MSVC_VERSION_CHECK(12,0,0) || \
  JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
#  define JSON_HEDLEY_ALWAYS_INLINE __forceinline
#elif defined(__cplusplus) && \
    ( \
      JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
      JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
      JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
      JSON_HEDLEY_TI_CL6X_VERSION_CHECK(6,1,0) || \
      JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
      JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) \
    )
#  define JSON_HEDLEY_ALWAYS_INLINE _Pragma("FUNC_ALWAYS_INLINE;")
#elif JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
#  define JSON_HEDLEY_ALWAYS_INLINE _Pragma("inline=forced")
#else
#  define JSON_HEDLEY_ALWAYS_INLINE JSON_HEDLEY_INLINE
#endif

#if defined(JSON_HEDLEY_NEVER_INLINE)
    #undef JSON_HEDLEY_NEVER_INLINE
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(noinline) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(4,0,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(10,1,0) || \
    JSON_HEDLEY_TI_VERSION_CHECK(15,12,0) || \
    (JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(4,8,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_ARMCL_VERSION_CHECK(5,2,0) || \
    (JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL2000_VERSION_CHECK(6,4,0) || \
    (JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,0,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL430_VERSION_CHECK(4,3,0) || \
    (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
    JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) || \
    JSON_HEDLEY_TI_CL7X_VERSION_CHECK(1,2,0) || \
    JSON_HEDLEY_TI_CLPRU_VERSION_CHECK(2,1,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10) || \
    JSON_HEDLEY_IAR_VERSION_CHECK(8,10,0)
    #define JSON_HEDLEY_NEVER_INLINE __attribute__((__noinline__))
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(13,10,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0)
    #define JSON_HEDLEY_NEVER_INLINE __declspec(noinline)
#elif JSON_HEDLEY_PGI_VERSION_CHECK(10,2,0)
    #define JSON_HEDLEY_NEVER_INLINE _Pragma("noinline")
#elif JSON_HEDLEY_TI_CL6X_VERSION_CHECK(6,0,0) && defined(__cplusplus)
    #define JSON_HEDLEY_NEVER_INLINE _Pragma("FUNC_CANNOT_INLINE;")
#elif JSON_HEDLEY_IAR_VERSION_CHECK(8,0,0)
    #define JSON_HEDLEY_NEVER_INLINE _Pragma("inline=never")
#elif JSON_HEDLEY_COMPCERT_VERSION_CHECK(3,2,0)
    #define JSON_HEDLEY_NEVER_INLINE __attribute((noinline))
#elif JSON_HEDLEY_PELLES_VERSION_CHECK(9,0,0)
    #define JSON_HEDLEY_NEVER_INLINE __declspec(noinline)
#else
    #define JSON_HEDLEY_NEVER_INLINE
#endif

#if defined(JSON_HEDLEY_PRIVATE)
    #undef JSON_HEDLEY_PRIVATE
#endif
#if defined(JSON_HEDLEY_PUBLIC)
    #undef JSON_HEDLEY_PUBLIC
#endif
#if defined(JSON_HEDLEY_IMPORT)
    #undef JSON_HEDLEY_IMPORT
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
#  define JSON_HEDLEY_PRIVATE
#  define JSON_HEDLEY_PUBLIC   __declspec(dllexport)
#  define JSON_HEDLEY_IMPORT   __declspec(dllimport)
#else
#  if \
    JSON_HEDLEY_HAS_ATTRIBUTE(visibility) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,3,0) || \
    JSON_HEDLEY_SUNPRO_VERSION_CHECK(5,11,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0) || \
    JSON_HEDLEY_IBM_VERSION_CHECK(13,1,0) || \
    ( \
      defined(__TI_EABI__) && \
      ( \
        (JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,2,0) && defined(__TI_GNU_ATTRIBUTE_SUPPORT__)) || \
        JSON_HEDLEY_TI_CL6X_VERSION_CHECK(7,5,0) \
      ) \
    ) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
#    define JSON_HEDLEY_PRIVATE __attribute__((__visibility__("hidden")))
#    define JSON_HEDLEY_PUBLIC  __attribute__((__visibility__("default")))
#  else
#    define JSON_HEDLEY_PRIVATE
#    define JSON_HEDLEY_PUBLIC
#  endif
#  define JSON_HEDLEY_IMPORT    extern
#endif

#if defined(JSON_HEDLEY_NO_THROW)
    #undef JSON_HEDLEY_NO_THROW
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(nothrow) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(3,3,0) || \
    JSON_HEDLEY_INTEL_VERSION_CHECK(13,0,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_NO_THROW __attribute__((__nothrow__))
#elif \
    JSON_HEDLEY_MSVC_VERSION_CHECK(13,1,0) || \
    JSON_HEDLEY_INTEL_CL_VERSION_CHECK(2021,1,0) || \
    JSON_HEDLEY_ARM_VERSION_CHECK(4,1,0)
    #define JSON_HEDLEY_NO_THROW __declspec(nothrow)
#else
    #define JSON_HEDLEY_NO_THROW
#endif

#if defined(JSON_HEDLEY_FALL_THROUGH)
    #undef JSON_HEDLEY_FALL_THROUGH
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(fallthrough) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(7,0,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_FALL_THROUGH __attribute__((__fallthrough__))
#elif JSON_HEDLEY_HAS_CPP_ATTRIBUTE_NS(clang,fallthrough)
    #define JSON_HEDLEY_FALL_THROUGH JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[clang::fallthrough]])
#elif JSON_HEDLEY_HAS_CPP_ATTRIBUTE(fallthrough)
    #define JSON_HEDLEY_FALL_THROUGH JSON_HEDLEY_DIAGNOSTIC_DISABLE_CPP98_COMPAT_WRAP_([[fallthrough]])
#elif defined(__fallthrough) /* SAL */
    #define JSON_HEDLEY_FALL_THROUGH __fallthrough
#else
    #define JSON_HEDLEY_FALL_THROUGH
#endif

#if defined(JSON_HEDLEY_RETURNS_NON_NULL)
    #undef JSON_HEDLEY_RETURNS_NON_NULL
#endif
#if \
    JSON_HEDLEY_HAS_ATTRIBUTE(returns_nonnull) || \
    JSON_HEDLEY_GCC_VERSION_CHECK(4,9,0) || \
    JSON_HEDLEY_MCST_LCC_VERSION_CHECK(1,25,10)
    #define JSON_HEDLEY_RETURNS_NON_NULL __attribute__((__returns_nonnull__))
#elif defined(_Ret_notnull_) /* SAL */
    #define JSON_HEDLEY_RETURNS_NON_NULL _Ret_notnull_
#else
    #define JSON_HEDLEY_RETURNS_NON_NULL
#endif

#if defined(JSON_HEDLEY_ARRAY_PARAM)
    #undef JSON_HEDLEY_ARRAY_PARAM
#endif
#if \
    defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && \
    !defined(__STDC_NO_VLA__) && \
    !defined(__cplusplus) && \
    !defined(JSON_HEDLEY_PGI_VERSION) && \
    !defined(JSON_HEDLEY_TINYC_VERSION)
    #define JSON_HEDLEY_ARRAY_PARAM(name) (name)
#else
    #define JSON_HEDLEY_ARRAY_PARAM(name)
#endif

#if defined(JSON_HEDLEY_IS_CONSTANT)
    #undef JSON_HEDLEY_IS_CONSTANT
#endif
#if defined(JSON_HEDLEY_REQUIRE_CONSTEXPR)
    #undef JSON_HEDLEY_REQUIRE_CONSTEXPR
#endif
/* JSON_HEDLEY_IS_CONSTEXPR_ is for
   HEDLEY INTERNAL USE ONLY.  API subject to change without notice. */
#if defined(JSON_HEDLEY_IS_CONSTEXPR_)
    #undef JSON_HEDLEY_IS_CONSTEXPR_
#endif
#if \
    JSON_HEDLEY_HAS_BUILTIN(__buil   }
            else if (wc <= 0x7FF)
            {
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(0xC0u | ((static_cast<unsigned int>(wc) >> 6u) & 0x1Fu));
                utf8_bytes[1] = static_cast<std::char_traits<char>::int_type>(0x80u | (static_cast<unsigned int>(wc) & 0x3Fu));
                utf8_bytes_filled = 2;
            }
            else if (wc <= 0xFFFF)
            {
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(0xE0u | ((static_cast<unsigned int>(wc) >> 12u) & 0x0Fu));
                utf8_bytes[1] = static_cast<std::char_traits<char>::int_type>(0x80u | ((static_cast<unsigned int>(wc) >> 6u) & 0x3Fu));
                utf8_bytes[2] = static_cast<std::char_traits<char>::int_type>(0x80u | (static_cast<unsigned int>(wc) & 0x3Fu));
                utf8_bytes_filled = 3;
            }
            else if (wc <= 0x10FFFF)
            {
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(0xF0u | ((static_cast<unsigned int>(wc) >> 18u) & 0x07u));
                utf8_bytes[1] = static_cast<std::char_traits<char>::int_type>(0x80u | ((static_cast<unsigned int>(wc) >> 12u) & 0x3Fu));
                utf8_bytes[2] = static_cast<std::char_traits<char>::int_type>(0x80u | ((static_cast<unsigned int>(wc) >> 6u) & 0x3Fu));
                utf8_bytes[3] = static_cast<std::char_traits<char>::int_type>(0x80u | (static_cast<unsigned int>(wc) & 0x3Fu));
                utf8_bytes_filled = 4;
            }
            else
            {
                // unknown character
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(wc);
                utf8_bytes_filled = 1;
            }
        }
    }
};

template<typename BaseInputAdapter>
struct wide_string_input_helper<BaseInputAdapter, 2>
{
    // UTF-16
    static void fill_buffer(BaseInputAdapter& input,
                            std::array<std::char_traits<char>::int_type, 4>& utf8_bytes,
                            size_t& utf8_bytes_index,
                            size_t& utf8_bytes_filled)
    {
        utf8_bytes_index = 0;

        if (JSON_HEDLEY_UNLIKELY(input.empty()))
        {
            utf8_bytes[0] = std::char_traits<char>::eof();
            utf8_bytes_filled = 1;
        }
        else
        {
            // get the current character
            const auto wc = input.get_character();

            // UTF-16 to UTF-8 encoding
            if (wc < 0x80)
            {
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(wc);
                utf8_bytes_filled = 1;
            }
            else if (wc <= 0x7FF)
            {
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(0xC0u | ((static_cast<unsigned int>(wc) >> 6u)));
                utf8_bytes[1] = static_cast<std::char_traits<char>::int_type>(0x80u | (static_cast<unsigned int>(wc) & 0x3Fu));
                utf8_bytes_filled = 2;
            }
            else if (0xD800 > wc || wc >= 0xE000)
            {
                utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(0xE0u | ((static_cast<unsigned int>(wc) >> 12u)));
                utf8_bytes[1] = static_cast<std::char_traits<char>::int_type>(0x80u | ((static_cast<unsigned int>(wc) >> 6u) & 0x3Fu));
                utf8_bytes[2] = static_cast<std::char_traits<char>::int_type>(0x80u | (static_cast<unsigned int>(wc) & 0x3Fu));
                utf8_bytes_filled = 3;
            }
            else
            {
                if (JSON_HEDLEY_UNLIKELY(!input.empty()))
                {
                    const auto wc2 = static_cast<unsigned int>(input.get_character());
                    const auto charcode = 0x10000u + (((static_cast<unsigned int>(wc) & 0x3FFu) << 10u) | (wc2 & 0x3FFu));
                    utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(0xF0u | (charcode >> 18u));
                    utf8_bytes[1] = static_cast<std::char_traits<char>::int_type>(0x80u | ((charcode >> 12u) & 0x3Fu));
                    utf8_bytes[2] = static_cast<std::char_traits<char>::int_type>(0x80u | ((charcode >> 6u) & 0x3Fu));
                    utf8_bytes[3] = static_cast<std::char_traits<char>::int_type>(0x80u | (charcode & 0x3Fu));
                    utf8_bytes_filled = 4;
                }
                else
                {
                    utf8_bytes[0] = static_cast<std::char_traits<char>::int_type>(wc);
                    utf8_bytes_filled = 1;
                }
            }
        }
    }
};

// Wraps another input adapter to convert wide character types into individual bytes.
template<typename BaseInputAdapter, typename WideCharType>
class wide_string_input_adapter
{
  public:
    using char_type = char;

    wide_string_input_adapter(BaseInputAdapter base)
        : base_adapter(base) {}

    typename std::char_traits<char>::int_type get_character() noexcept
    {
        // check if buffer needs to be filled
        if (utf8_bytes_index == utf8_bytes_filled)
        {
            fill_buffer<sizeof(WideCharType)>();

            JSON_ASSERT(utf8_bytes_filled > 0);
            JSON_ASSERT(utf8_bytes_index == 0);
        }

        // use buffer
        JSON_ASSERT(utf8_bytes_filled > 0);
        JSON_ASSERT(utf8_bytes_index < utf8_bytes_filled);
        return utf8_bytes[utf8_bytes_index++];
    }

  private:
    BaseInputAdapter base_adapter;

    template<size_t T>
    void fill_buffer()
    {
        wide_string_input_helper<BaseInputAdapter, T>::fill_buffer(base_adapter, utf8_bytes, utf8_bytes_index, utf8_bytes_filled);
    }

    /// a buffer for UTF-8 bytes
    std::array<std::char_traits<char>::int_type, 4> utf8_bytes = {{0, 0, 0, 0}};

    /// index to the utf8_codes array for the next valid byte
    std::size_t utf8_bytes_index = 0;
    /// number of valid bytes in the utf8_codes array
    std::size_t utf8_bytes_filled = 0;
};

template<typename IteratorType, typename Enable = void>
struct iterator_input_adapter_factory
{
    using iterator_type = IteratorType;
    using char_type = typename std::iterator_traits<iterator_type>::value_type;
    using adapter_type = iterator_input_adapter<iterator_type>;

    static adapter_type create(IteratorType first, IteratorType last)
    {
        return adapter_type(std::move(first), std::move(last));
    }
};

template<typename T>
struct is_iterator_of_multibyte
{
    using value_type = typename std::iterator_traits<T>::value_type;
    enum
    {
        value = sizeof(value_type) > 1
    };
};

template<typename IteratorType>
struct iterator_input_adapter_factory<IteratorType, enable_if_t<is_iterator_of_multibyte<IteratorType>::value>>
{
    using iterator_type = IteratorType;
    using char_type = typename std::iterator_traits<iterator_type>::value_type;
    using base_adapter_type = iterator_input_adapter<iterator_type>;
    using adapter_type = wide_string_input_adapter<base_adapter_type, char_type>;

    static adapter_type create(IteratorType first, IteratorType last)
    {
        return adapter_type(base_adapter_type(std::move(first), std::move(last)));
    }
};

// General purpose iterator-based input
template<typename IteratorType>
typename iterator_input_adapter_factory<IteratorType>::adapter_type input_adapter(IteratorType first, IteratorType last)
{
    using factory_type = iterator_input_adapter_factory<IteratorType>;
    return factory_type::create(first, last);
}

// Convenience shorthand from container to iterator
// Enables ADL on begin(container) and end(container)
// Encloses the using declarations in namespace for not to leak them to outside scope

namespace container_input_adapter_factory_impl
{

using std::begin;
using std::end;

template<typename ContainerType, typename Enable = void>
struct container_input_adapter_factory {};

template<typename ContainerType>
struct container_input_adapter_factory< ContainerType,
       void_t<decltype(begin(std::declval<ContainerType>()), end(std::declval<ContainerType>()))>>
       {
           using adapter_type = decltype(input_adapter(begin(std::declval<ContainerType>()), end(std::declval<ContainerType>())));

           static adapter_type create(const ContainerType& container)
{
    return input_adapter(begin(container), end(container));
}
       };

}  // namespace container_input_adapter_factory_impl

template<typename ContainerType>
typename container_input_adapter_factory_impl::container_input_adapter_factory<ContainerType>::adapter_type input_adapter(const ContainerType& container)
{
    return container_input_adapter_factory_impl::container_input_adapter_factory<ContainerType>::create(container);
}

#ifndef JSON_NO_IO
// Special cases with fast paths
inline file_input_adapter input_adapter(std::FILE* file)
{
    return file_input_adapter(file);
}

inline input_stream_adapter input_adapter(std::istream& stream)
{
    return input_stream_adapter(stream);
}

inline input_stream_adapter input_adapter(std::istream&& stream)
{
    return input_stream_adapter(stream);
}
#endif  // JSON_NO_IO

using contiguous_bytes_input_adapter = decltype(input_adapter(std::declval<const char*>(), std::declval<const char*>()));

// Null-delimited strings, and the like.
template < typename CharT,
           typename std::enable_if <
               std::is_pointer<CharT>::value&&
               !std::is_array<CharT>::value&&
               std::is_integral<typename std::remove_pointer<CharT>::type>::value&&
               sizeof(typename std::remove_pointer<CharT>::type) == 1,
               int >::type = 0 >
contiguous_bytes_input_adapter input_adapter(CharT b)
{
    auto length = std::strlen(reinterpret_cast<const char*>(b));
    const auto* ptr = reinterpret_cast<const char*>(b);
    return input_adapter(ptr, ptr + length);
}

template<typename T, std::size_t N>
auto input_adapter(T (&array)[N]) -> decltype(input_adapter(array, array + N)) // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
{
    return input_adapter(array, array + N);
}

// This class only handles inputs of input_buffer_adapter type.
// It's required so that expressions like {ptr, len} can be implicitly cast
// to the correct adapter.
class span_input_adapter
{
  public:
    template < typename CharT,
               typename std::enable_if <
                   std::is_pointer<CharT>::value&&
                   std::is_integral<typename std::remove_pointer<CharT>::type>::value&&
                   sizeof(typename std::remove_pointer<CharT>::type) == 1,
                   int >::type = 0 >
    span_input_adapter(CharT b, std::size_t l)
        : ia(reinterpret_cast<const char*>(b), reinterpret_cast<const char*>(b) + l) {}

    template<class IteratorType,
             typename std::enable_if<
                 std::is_same<typename iterator_traits<IteratorType>::iterator_category, std::random_access_iterator_tag>::value,
                 int>::type = 0>
    span_input_adapter(IteratorType first, IteratorType last)
        : ia(input_adapter(first, last)) {}

    contiguous_bytes_input_adapter&& get()
    {
        return std::move(ia); // NOLINT(hicpp-move-const-arg,performance-move-const-arg)
    }

  private:
    contiguous_bytes_input_adapter ia;
};

}  // namespace detail
NLOHMANN_JSON_NAMESPACE_END

// #include <nlohmann/detail/input/json_sax.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <cstddef>
#include <string> // string
#include <utility> // move
#include <vector> // vector

// #include <nlohmann/detail/exceptions.hpp>

// #include <nlohmann/detail/macro_scope.hpp>

// #include <nlohmann/detail/string_concat.hpp>


NLOHMANN_JSON_NAMESPACE_BEGIN

/*!
@brief SAX interface

This class describes the SAX interface used by @ref nlohmann::json::sax_parse.
Each function is called in different situations while the input is parsed. The
boolean return value informs the parser whether to continue processing the
input.
*/
template<typename BasicJsonType>
struct json_sax
{
    using number_integer_t = typename BasicJsonType::number_integer_t;
    using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
    using number_float_t = typename BasicJsonType::number_float_t;
    using string_t = typename BasicJsonType::string_t;
    using binary_t = typename BasicJsonType::binary_t;

    /*!
    @brief a null value was read
    @return whether parsing should proceed
    */
    virtual bool null() = 0;

    /*!
    @brief a boolean value was read
    @param[in] val  boolean value
    @return whether parsing should proceed
    */
    virtual bool boolean(bool val) = 0;

    /*!
    @brief an integer number was read
    @param[in] val  integer value
    @return whether parsing should proceed
    */
    virtual bool number_integer(number_integer_t val) = 0;

    /*!
    @brief an unsigned integer number was read
    @param[in] val  unsigned integer value
    @return whether parsing should proceed
    */
    virtual bool number_unsigned(number_unsigned_t val) = 0;

    /*!
    @brief a floating-point number was read
    @param[in] val  floating-point value
    @param[in] s    raw token value
    @return whether parsing should proceed
    */
    virtual bool number_float(number_float_t val, const string_t& s) = 0;

    /*!
    @brief a string value was read
    @param[in] val  string value
    @return whether parsing should proceed
    @note It is safe to move the passed string value.
    */
    virtual bool string(string_t& val) = 0;

    /*!
    @brief a binary value was read
    @param[in] val  binary value
    @return whether parsing should proceed
    @note It is safe to move the passed binary value.
    */
    virtual bool binary(binary_t& val) = 0;

    /*!
    @brief the beginning of an object was read
    @param[in] elements  number of object elements or -1 if unknown
    @return whether parsing should proceed
    @note binary formats may report the number of elements
    */
    virtual bool start_object(std::size_t elements) = 0;

    /*!
    @brief an object key was read
    @param[in] val  object key
    @return whether parsing should proceed
    @note It is safe to move the passed string.
    */
    virtual bool key(string_t& val) = 0;

    /*!
    @brief the end of an object was read
    @return whether parsing should proceed
    */
    virtual bool end_object() = 0;

    /*!
    @brief the beginning of an array was read
    @param[in] elements  number of array elements or -1 if unknown
    @return whether parsing should proceed
    @note binary formats may report the number of elements
    */
    virtual bool start_array(std::size_t elements) = 0;

    /*!
    @brief the end of an array was read
    @return whether parsing should proceed
    */
    virtual bool end_array() = 0;

    /*!
    @brief a parse error occurred
    @param[in] position    the position in the input where the error occurs
    @param[in] last_token  the last read token
    @param[in] ex          an exception object describing the error
    @return whether parsing should proceed (must return false)
    */
    virtual bool parse_error(std::size_t position,
                             const std::string& last_token,
                             const detail::exception& ex) = 0;

    json_sax() = default;
    json_sax(const json_sax&) = default;
    json_sax(json_sax&&) noexcept = default;
    json_sax& operator=(const json_sax&) = default;
    json_sax& operator=(json_sax&&) noexcept = default;
    virtual ~json_sax() = default;
};

namespace detail
{
/*!
@brief SAX implementation to create a JSON value from SAX events

This class implements the @ref json_sax interface and processes the SAX events
to create a JSON value which makes it basically a DOM parser. The structure or
hierarchy of the JSON value is managed by the stack `ref_stack` which contains
a pointer to the respective array or object for each recursion depth.

After successful parsing, the value that is passed by reference to the
constructor contains the parsed value.

@tparam BasicJsonType  the JSON type
*/
template<typename BasicJsonType>
class json_sax_dom_parser
{
  public:
    using number_integer_t = typename BasicJsonType::number_integer_t;
    using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
    using number_float_t = typename BasicJsonType::number_float_t;
    using string_t = typename BasicJsonType::string_t;
    using binary_t = typename BasicJsonType::binary_t;

    /*!
    @param[in,out] r  reference to a JSON value that is manipulated while
                       parsing
    @param[in] allow_exceptions_  whether parse errors yield exceptions
    */
    explicit json_sax_dom_parser(BasicJsonType& r, const bool allow_exceptions_ = true)
        : root(r), allow_exceptions(allow_exceptions_)
    {}

    // make class move-only
    json_sax_dom_parser(const json_sax_dom_parser&) = delete;
    json_sax_dom_parser(json_sax_dom_parser&&) = default; // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
    json_sax_dom_parser& operator=(const json_sax_dom_parser&) = delete;
    json_sax_dom_parser& operator=(json_sax_dom_parser&&) = default; // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
    ~json_sax_dom_parser() = default;

    bool null()
    {
        handle_value(nullptr);
        return true;
    }

    bool boolean(bool val)
    {
        handle_value(val);
        return true;
    }

    bool number_integer(number_integer_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_unsigned(number_unsigned_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_float(number_float_t val, const string_t& /*unused*/)
    {
        handle_value(val);
        return true;
    }

    bool string(string_t& val)
    {
        handle_value(val);
        return true;
    }

    bool binary(binary_t& val)
    {
        handle_value(std::move(val));
        return true;
    }

    bool start_object(std::size_t len)
    {
        ref_stack.push_back(handle_value(BasicJsonType::value_t::object));

        if (JSON_HEDLEY_UNLIKELY(len != static_cast<std::size_t>(-1) && len > ref_stack.back()->max_size()))
        {
            JSON_THROW(out_of_range::create(408, concat("excessive object size: ", std::to_string(len)), ref_stack.back()));
        }

        return true;
    }

    bool key(string_t& val)
    {
        JSON_ASSERT(!ref_stack.empty());
        JSON_ASSERT(ref_stack.back()->is_object());

        // add null at given key and store the reference for later
        object_element = &(ref_stack.back()->m_data.m_value.object->operator[](val));
        return true;
    }

    bool end_object()
    {
        JSON_ASSERT(!ref_stack.empty());
        JSON_ASSERT(ref_stack.back()->is_object());

        ref_stack.back()->set_parents();
        ref_stack.pop_back();
        return true;
    }

    bool start_array(std::size_t len)
    {
        ref_stack.push_back(handle_value(BasicJsonType::value_t::array));

        if (JSON_HEDLEY_UNLIKELY(len != static_cast<std::size_t>(-1) && len > ref_stack.back()->max_size()))
        {
            JSON_THROW(out_of_range::create(408, concat("excessive array size: ", std::to_string(len)), ref_stack.back()));
        }

        return true;
    }

    bool end_array()
    {
        JSON_ASSERT(!ref_stack.empty());
        JSON_ASSERT(ref_stack.back()->is_array());

        ref_stack.back()->set_parents();
        ref_stack.pop_back();
        return true;
    }

    template<class Exception>
    bool parse_error(std::size_t /*unused*/, const std::string& /*unused*/,
                     const Exception& ex)
    {
        errored = true;
        static_cast<void>(ex);
        if (allow_exceptions)
        {
            JSON_THROW(ex);
        }
        return false;
    }

    constexpr bool is_errored() const
    {
        return errored;
    }

  private:
    /*!
    @invariant If the ref stack is empty, then the passed value will be the new
               root.
    @invariant If the ref stack contains a value, then it is an array or an
               object to which we can add elements
    */
    template<typename Value>
    JSON_HEDLEY_RETURNS_NON_NULL
    BasicJsonType* handle_value(Value&& v)
    {
        if (ref_stack.empty())
        {
            root = BasicJsonType(std::forward<Value>(v));
            return &root;
        }

        JSON_ASSERT(ref_stack.back()->is_array() || ref_stack.back()->is_object());

        if (ref_stack.back()->is_array())
        {
            ref_stack.back()->m_data.m_value.array->emplace_back(std::forward<Value>(v));
            return &(ref_stack.back()->m_data.m_value.array->back());
        }

        JSON_ASSERT(ref_stack.back()->is_object());
        JSON_ASSERT(object_element);
        *object_element = BasicJsonType(std::forward<Value>(v));
        return object_element;
    }

    /// the parsed JSON value
    BasicJsonType& root;
    /// stack to model hierarchy of values
    std::vector<BasicJsonType*> ref_stack {};
    /// helper to hold the reference for the next object element
    BasicJsonType* object_element = nullptr;
    /// whether a syntax error occurred
    bool errored = false;
    /// whether to throw exceptions in case of errors
    const bool allow_exceptions = true;
};

template<typename BasicJsonType>
class json_sax_dom_callback_parser
{
  public:
    using number_integer_t = typename BasicJsonType::number_integer_t;
    using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
    using number_float_t = typename BasicJsonType::number_float_t;
    using string_t = typename BasicJsonType::string_t;
    using binary_t = typename BasicJsonType::binary_t;
    using parser_callback_t = typename BasicJsonType::parser_callback_t;
    using parse_event_t = typename BasicJsonType::parse_event_t;

    json_sax_dom_callback_parser(BasicJsonType& r,
                                 const parser_callback_t cb,
                                 const bool allow_exceptions_ = true)
        : root(r), callback(cb), allow_exceptions(allow_exceptions_)
    {
        keep_stack.push_back(true);
    }

    // make class move-only
    json_sax_dom_callback_parser(const json_sax_dom_callback_parser&) = delete;
    json_sax_dom_callback_parser(json_sax_dom_callback_parser&&) = default; // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
    json_sax_dom_callback_parser& operator=(const json_sax_dom_callback_parser&) = delete;
    json_sax_dom_callback_parser& operator=(json_sax_dom_callback_parser&&) = default; // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
    ~json_sax_dom_callback_parser() = default;

    bool null()
    {
        handle_value(nullptr);
        return true;
    }

    bool boolean(bool val)
    {
        handle_value(val);
        return true;
    }

    bool number_integer(number_integer_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_unsigned(number_unsigned_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_float(number_float_t val, const string_t& /*unused*/)
    {
        handle_value(val);
        return true;
    }

    bool string(string_t& val)
    {
        handle_value(val);
        return true;
    }

    bool binary(binary_t& val)
    {
        handle_value(std::move(val));
        return true;
    }

    bool start_object(std::size_t len)
    {
        // check callback for object start
        const bool keep = callback(static_cast<int>(ref_stack.size()), parse_event_t::object_start, discarded);
        keep_stack.push_back(keep);

        auto val = handle_value(BasicJsonType::value_t::object, true);
        ref_stack.push_back(val.second);

        // check object limit
        if (ref_stack.back() && JSON_HEDLEY_UNLIKELY(len != static_cast<std::size_t>(-1) && len > ref_stack.back()->max_size()))
        {
            JSON_THROW(out_of_range::create(408, concat("excessive object size: ", std::to_string(len)), ref_stack.back()));
        }

        return true;
    }

    bool key(string_t& val)
    {
        BasicJsonType k = BasicJsonType(val);

        // check callback for key
        const bool keep = callback(static_cast<int>(ref_stack.size()), parse_event_t::key, k);
        key_keep_stack.push_back(keep);

        // add discarded value at given key and store the reference for later
        if (keep && ref_stack.back())
        {
            object_element = &(ref_stack.back()->m_data.m_value.object->operator[](val) = discarded);
        }

        return true;
    }

    bool end_object()
    {
        if (ref_stack.back())
        {
            if (!callback(static_cast<int>(ref_stack.size()) - 1, parse_event_t::object_end, *ref_stack.back()))
            {
                // discard object
                *ref_stack.back() = discarded;
            }
            else
            {
                ref_stack.back()->set_parents();
            }
        }

        JSON_ASSERT(!ref_stack.empty());
        JSON_ASSERT(!keep_stack.empty());
        ref_stack.pop_back();
        keep_stack.pop_back();

        if (!ref_stack.empty() && ref_stack.back() && ref_stack.back()->is_structured())
        {
            // remove discarded value
            for (auto it = ref_stack.back()->begin(); it != ref_stack.back()->end(); ++it)
            {
                if (it->is_discarded())
                {
                    ref_stack.back()->erase(it);
                    break;
                }
            }
        }

        return true;
    }

    bool start_array(std::size_t len)
    {
        const bool keep = callback(static_cast<int>(ref_stack.size()), parse_event_t::array_start, discarded);
        keep_stack.push_back(keep);

        auto val = handle_value(BasicJsonType::value_t::array, true);
        ref_stack.push_back(val.second);

        // check array limit
        if (ref_stack.back() && JSON_HEDLEY_UNLIKELY(len != static_cast<std::size_t>(-1) && len > ref_stack.back()->max_size()))
        {
            JSON_THROW(out_of_range::create(408, concat("excessive array size: ", std::to_string(len)), ref_stack.back()));
        }

        return true;
    }

    bool end_array()
    {
        bool keep = true;

        if (ref_stack.back())
        {
            keep = callback(static_cast<int>(ref_stack.size()) - 1, parse_event_t::array_end, *ref_stack.back());
            if (keep)
            {
                ref_stack.back()->set_parents();
            }
            else
            {
                // discard array
                *ref_stack.back() = discarded;
            }
        }

        JSON_ASSERT(!ref_stack.empty());
        JSON_ASSERT(!keep_stack.empty());
        ref_stack.pop_back();
        keep_stack.pop_back();

        // remove discarded value
        if (!keep && !ref_stack.empty() && ref_stack.back()->is_array())
        {
            ref_stack.back()->m_data.m_value.array->pop_back();
        }

        return true;
    }

    template<class Exception>
    bool parse_error(std::size_t /*unused*/, const std::string& /*unused*/,
                     const Exception& ex)
    {
        errored = true;
        static_cast<void>(ex);
        if (allow_exceptions)
        {
            JSON_THROW(ex);
        }
        return false;
    }

    constexpr bool is_errored() const
    {
        return errored;
    }

  private:
    /*!
    @param[in] v  value to add to the JSON value we build during parsing
    @param[in] skip_callback  whether we should skip calling the callback
               function; this is required after start_array() and
               start_object() SAX events, because otherwise we would call the
               callback function with an empty array or object, respectively.

    @invariant If the ref stack is empty, then the passed value will be the new
               root.
    @invariant If the ref stack contains a value, then it is an array or an
               object to which we can add elements

    @return pair of boolean (whether value should be kept) and pointer (to the
            passed value in the ref_stack hierarchy; nullptr if not kept)
    */
    template<typename Value>
    std::pair<bool, BasicJsonType*> handle_value(Value&& v, const bool skip_callback = false)
    {
        JSON_ASSERT(!keep_stack.empty());

        // do not handle this value if we know it would be added to a discarded
        // container
        if (!keep_stack.back())
        {
            return {false, nullptr};
        }

        // create value
        auto value = BasicJsonType(std::forward<Value>(v));

        // check callback
        const bool keep = skip_callback || callback(static_cast<int>(ref_stack.size()), parse_event_t::value, value);

        // do not handle this value if we just learnt it shall be discarded
        if (!keep)
        {
            return {false, nullptr};
        }

        if (ref_stack.empty())
        {
            root = std::move(value);
            return {true, & root};
        }

        // skip this value if we already decided to skip the parent
        // (https://github.com/nlohmann/json/issues/971#issuecomment-413678360)
        if (!ref_stack.back())
        {
            return {false, nullptr};
        }

        // we now only expect arrays and objects
        JSON_ASSERT(ref_stack.back()->is_array() || ref_stack.back()->is_object());

        // array
        if (ref_stack.back()->is_array())
        {
            ref_stack.back()->m_data.m_value.array->emplace_back(std::move(value));
            return {true, & (ref_stack.back()->m_data.m_value.array->back())};
        }

        // object
        JSON_ASSERT(ref_stack.back()->is_object());
        // check if we should store an element for the current key
        JSON_ASSERT(!key_keep_stack.empty());
        const bool store_element = key_keep_stack.back();
        key_keep_stack.pop_back();

        if (!store_element)
        {
            return {false, nullptr};
        }

        JSON_ASSERT(object_element);
        *object_element = std::move(value);
        return {true, object_element};
    }

    /// the parsed JSON value
    BasicJsonType& root;
    /// stack to model hierarchy of values
    std::vector<BasicJsonType*> ref_stack {};
    /// stack to manage which values to keep
    std::vector<bool> keep_stack {};
    /// stack to manage which object keys to keep
    std::vector<bool> key_keep_stack {};
    /// helper to hold the reference for the next object element
    BasicJsonType* object_element = nullptr;
    /// whether a syntax error occurred
    bool errored = false;
    /// callback function
    const parser_callback_t callback = nullptr;
    /// whether to throw exceptions in case of errors
    const bool allow_exceptions = true;
    /// a discarded value for the callback
    BasicJsonType discarded = BasicJsonType::value_t::discarded;
};

template<typename BasicJsonType>
class json_sax_acceptor
{
  public:
    using number_integer_t = typename BasicJsonType::number_integer_t;
    using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
    using number_float_t = typename BasicJsonType::number_float_t;
    using string_t = typename BasicJsonType::string_t;
    using binary_t = typename BasicJsonType::binary_t;

    bool null()
    {
        return true;
    }

    bool boolean(bool /*unused*/)
    {
        return true;
    }

    bool number_integer(number_integer_t /*unused*/)
    {
        return true;
    }

    bool number_unsigned(number_unsigned_t /*unused*/)
    {
        return true;
    }

    bool number_float(number_float_t /*unused*/, const string_t& /*unused*/)
    {
        return true;
    }

    bool string(string_t& /*unused*/)
    {
        return true;
    }

    bool binary(binary_t& /*unused*/)
    {
        return true;
    }

    bool start_object(std::size_t /*unused*/ = static_cast<std::size_t>(-1))
    {
        return true;
    }

    bool key(string_t& /*unused*/)
    {
        return true;
    }

    bool end_object()
    {
        return true;
    }

    bool start_array(std::size_t /*unused*/ = static_cast<std::size_t>(-1))
    {
        return true;
    }

    bool end_array()
    {
        return true;
    }

    bool parse_error(std::size_t /*unused*/, const std::string& /*unused*/, const detail::exception& /*unused*/)
    {
        return false;
    }
};

}  // namespace detail
NLOHMANN_JSON_NAMESPACE_END

// #include <nlohmann/detail/input/lexer.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <array> // array
#include <clocale> // localeconv
#include <cstddef> // size_t
#include <cstdio> // snprintf
#include <cstdlib> // strtof, strtod, strtold, strtoll, strtoull
#include <initializer_list> // initializer_list
#include <string> // char_traits, string
#include <utility> // move
#include <vector> // vector

// #include <nlohmann/detail/input/input_adapters.hpp>

// #include <nlohmann/detail/input/position_t.hpp>

// #include <nlohmann/detail/macro_scope.hpp>

// #include <nlohmann/detail/meta/type_traits.hpp>


NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

///////////
// lexer //
///////////

template<typename BasicJsonType>
class lexer_base
{
  public:
    /// token types for the parser
    enum class token_type
    {
        uninitialized,    ///< indicating the scanner is uninitialized
        literal_true,     ///< the `true` literal
        literal_false,    ///< the `false` literal
        literal_null,     ///< the `null` literal
        value_string,     ///< a string -- use get_string() for actual value
        value_unsigned,   ///< an unsigned integer -- use get_number_unsigned() for actual value
        value_integer,    ///< a signed integer -- use get_number_integer() for actual value
        value_float,      ///< an floating point number -- use get_number_float() for actual value
        begin_array,      ///< the character for array begin `[`
        begin_object,     ///< the character for object begin `{`
        end_array,        ///< the character for array end `]`
        end_object,       ///< the character for object end `}`
        name_separator,   ///< the name separator `:`
        value_separator,  ///< the value separator `,`
        parse_error,      ///< indicating a parse error
        end_of_input,     ///< indicating the end of the input buffer
        literal_or_value  ///< a literal or the begin of a value (only for diagnostics)
    };

    /// return name of values of type token_type (only used for errors)
    JSON_HEDLEY_RETURNS_NON_NULL
    JSON_HEDLEY_CONST
    static const char* token_type_name(const token_type t) noexcept
    {
        switch (t)
        {
            case token_type::uninitialized:
                return "<uninitialized>";
            case token_type::literal_true:
                return "true literal";
            case token_type::literal_false:
                return "false literal";
            case token_type::literal_null:
                return "null literal";
            case token_type::value_string:
                return "string literal";
            case token_type::value_unsigned:
            case token_type::value_integer:
            case token_type::value_float:
                return "number literal";
            case token_type::begin_array:
                return "'['";
            case token_type::begin_object:
                return "'{'";
            case token_type::end_array:
                return "']'";
            case token_type::end_object:
                return "'}'";
            case token_type::name_separator:
                return "':'";
            case token_type::value_separator:
                return "','";
            case token_type::parse_error:
                return "<parse error>";
            case token_type::end_of_input:
                return "end of input";
            case token_type::literal_or_value:
                return "'[', '{', or a literal";
            // LCOV_EXCL_START
            default: // catch non-enum values
                return "unknown token";
                // LCOV_EXCL_STOP
        }
    }
};
/*!
@brief lexical analysis

This class organizes the lexical analysis during JSON deserialization.
*/
template<typename BasicJsonType, typename InputAdapterType>
class lexer : public lexer_base<BasicJsonType>
{
    using number_integer_t = typename BasicJsonType::number_integer_t;
    using number_unsigned_t = typename BasicJsonType::number_unsigned_t;
    using number_float_t = typename BasicJsonType::number_float_t;
    using string_t = typename BasicJsonType::string_t;
    using char_type = typename InputAdapterType::char_type;
    using char_int_type = typename char_traits<char_type>::int_type;

  public:
    using token_type = typename lexer_base<BasicJsonType>::token_type;

    explicit lexer(InputAdapterType&& adapter, bool ignore_comments_ = false) noexcept
        : ia(std::move(adapter))
        , ignore_comments(ignore_comments_)
        , decimal_point_char(static_cast<char_int_type>(get_decimal_point()))
    {}

    // delete because of pointer members
    lexer(const lexer&) = delete;
    lexer(lexer&&) = default; // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
    lexer& operator=(lexer&) = delete;
    lexer& operator=(lexer&&) = default; // NOLINT(hicpp-noexcept-move,performance-noexcept-move-constructor)
    ~lexer() = default;

  private:
    /////////////////////
    // locales
    /////////////////////

    /// return the locale-dependent decimal point
    JSON_HEDLEY_PURE
    static char get_decimal_point() noexcept
    {
        const auto* loc = localeconv();
        JSON_ASSERT(loc != nullptr);
        return (loc->decimal_point == nullptr) ? '.' : *(loc->decimal_point);
    }

    /////////////////////
    // scan functions
    /////////////////////

    /*!
    @brief get codepoint from 4 hex characters following `\u`

    For input "\u c1 c2 c3 c4" the codepoint is:
      (c1 * 0x1000) + (c2 * 0x0100) + (c3 * 0x0010) + c4
    = (c1 << 12) + (c2 << 8) + (c3 << 4) + (c4 << 0)

    Furthermore, the possible characters '0'..'9', 'A'..'F', and 'a'..'f'
    must be converted to the integers 0x0..0x9, 0xA..0xF, 0xA..0xF, resp. The
    conversion is done by subtracting the offset (0x30, 0x37, and 0x57)
    between the ASCII value of the character and the desired integer value.

    @return codepoint (0x0000..0xFFFF) or -1 in case of an error (e.g. EOF or
            non-hex character)
    */
    int get_codepoint()
    {
        // this function only makes sense after reading `\u`
        JSON_ASSERT(current == 'u');
        int codepoint = 0;

        const auto factors = { 12u, 8u, 4u, 0u };
        for (const auto factor : factors)
        {
            get();

            if (current >= '0' && current <= '9')
            {
                codepoint += static_cast<int>((static_cast<unsigned int>(current) - 0x30u) << factor);
            }
            else if (current >= 'A' && current <= 'F')
            {
                codepoint += static_cast<int>((static_cast<unsigned int>(current) - 0x37u) << factor);
            }
            else if (current >= 'a' && current <= 'f')
            {
                codepoint += static_cast<int>((static_cast<unsigned int>(current) - 0x57u) << factor);
            }
            else
            {
                return -1;
            }
        }

        JSON_ASSERT(0x0000 <= codepoint && codepoint <= 0xFFFF);
        return codepoint;
    }

    /*!
    @brief check if the next byte(s) are inside a given range

    Adds the current byte and, for each passed range, reads a new byte and
    checks if it is inside the range. If a violation was detected, set up an
    error message and return false. Otherwise, return true.

    @param[in] ranges  list of integers; interpreted as list of pairs of
                       inclusive lower and upper bound, respectively

    @pre The passed list @a ranges must have 2, 4, or 6 elements; that is,
         1, 2, or 3 pairs. This precondition is enforced by an assertion.

    @return true if and only if no range violation was detected
    */
    bool next_byte_in_range(std::initializer_list<char_int_type> ranges)
    {
        JSON_ASSERT(ranges.size() == 2 || ranges.size() == 4 || ranges.size() == 6);
        add(current);

        for (auto range = ranges.begin(); range != ranges.end(); ++range)
        {
            get();
            if (JSON_HEDLEY_LIKELY(*range <= current && current <= *(++range))) // NOLINT(bugprone-inc-dec-in-conditions)
            {
                add(current);
            }
            else
            {
                error_message = "invalid string: ill-formed UTF-8 byte";
                return false;
            }
        }

        return true;
    }

    /*!
    @brief scan a string literal

    This function scans a string according to Sect. 7 of RFC 8259. While
    scanning, bytes are escaped and copied into buffer token_buffer. Then the
    function returns successfully, token_buffer is *not* null-terminated (as it
    may contain \0 bytes), and token_buffer.size() is the number of bytes in the
    string.

    @return token_type::value_string if string could be successfully scanned,
            token_type::parse_error otherwise

    @note In case of errors, variable error_message contains a textual
          description.
    */
    token_type scan_string()
    {
        // reset token_buffer (ignore opening quote)
        reset();

        // we entered the function by reading an open quote
        JSON_ASSERT(current == '\"');

        while (true)
        {
            // get next character
            switch (get())
            {
                // end of file while parsing string
                case char_traits<char_type>::eof():
                {
                    error_message = "invalid string: missing closing quote";
                    return token_type::parse_error;
                }

                // closing quote
                case '\"':
                {
                    return token_type::value_string;
                }

                // escapes
                case '\\':
                {
                    switch (get())
                    {
                        // quotation mark
                        case '\"':
                            add('\"');
                            break;
                        // reverse solidus
                        case '\\':
                            add('\\');
                            break;
                        // solidus
                        case '/':
                            add('/');
                            break;
                        // backspace
                        case 'b':
                            add('\b');
                            break;
                        // form feed
                        case 'f':
                            add('\f');
                            break;
                        // line feed
                        case 'n':
                            add('\n');
                            break;
                        // carriage return
                        case 'r':
                            add('\r');
                            break;
                        // tab
                        case 't':
                            add('\t');
                            break;

                        // unicode escapes
                        case 'u':
                        {
                            const int codepoint1 = get_codepoint();
                            int codepoint = codepoint1; // start with codepoint1

                            if (JSON_HEDLEY_UNLIKELY(codepoint1 == -1))
                            {
                                error_message = "invalid string: '\\u' must be followed by 4 hex digits";
                                return token_type::parse_error;
                            }

                            // check if code point is a high surrogate
                            if (0xD800 <= codepoint1 && codepoint1 <= 0xDBFF)
                            {
                                // expect next \uxxxx entry
                                if (JSON_HEDLEY_LIKELY(get() == '\\' && get() == 'u'))
                                {
                                    const int codepoint2 = get_codepoint();

                                    if (JSON_HEDLEY_UNLIKELY(codepoint2 == -1))
                                    {
                                        error_message = "invalid string: '\\u' must be followed by 4 hex digits";
                                        return token_type::parse_error;
                                    }

                                    // check if codepoint2 is a low surrogate
                                    if (JSON_HEDLEY_LIKELY(0xDC00 <= codepoint2 && codepoint2 <= 0xDFFF))
                                    {
                                        // overwrite codepoint
                                        codepoint = static_cast<int>(
                                                        // high surrogate occupies the most significant 22 bits
                                                        (static_cast<unsigned int>(codepoint1) << 10u)
                                                        // low surrogate occupies the least significant 15 bits
                                                        + static_cast<unsigned int>(codepoint2)
                                                        // there is still the 0xD800, 0xDC00 and 0x10000 noise
                                                        // in the result, so we have to subtract with:
                                                        // (0xD800 << 10) + DC00 - 0x10000 = 0x35FDC00
                                                        - 0x35FDC00u);
                                    }
                                    else
                                    {
                                        error_message = "invalid string: surrogate U+D800..U+DBFF must be followed by U+DC00..U+DFFF";
                                        return token_type::parse_error;
                                    }
                                }
                                else
                                {
                                    error_message = "invalid string: surrogate U+D800..U+DBFF must be followed by U+DC00..U+DFFF";
                                    return token_type::parse_error;
                                }
                            }
                            else
                            {
                                if (JSON_HEDLEY_UNLIKELY(0xDC00 <= codepoint1 && codepoint1 <= 0xDFFF))
                                {
                                    error_message = "invalid string: surrogate U+DC00..U+DFFF must follow U+D800..U+DBFF";
                                    return token_type::parse_error;
                                }
                            }

                            // result of the above calculation yields a proper codepoint
                            JSON_ASSERT(0x00 <= codepoint && codepoint <= 0x10FFFF);

                            // translate codepoint into bytes
                            if (codepoint < 0x80)
                            {
                                // 1-byte characters: 0xxxxxxx (ASCII)
                                add(static_cast<char_int_type>(codepoint));
                            }
                            else if (codepoint <= 0x7FF)
                            {
                                // 2-byte characters: 110xxxxx 10xxxxxx
                                add(static_cast<char_int_type>(0xC0u | (static_cast<unsigned int>(codepoint) >> 6u)));
                                add(static_cast<char_int_type>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu)));
                            }
                            else if (codepoint <= 0xFFFF)
                            {
                                // 3-byte characters: 1110xxxx 10xxxxxx 10xxxxxx
                                add(static_cast<char_int_type>(0xE0u | (static_cast<unsigned int>(codepoint) >> 12u)));
                                add(static_cast<char_int_type>(0x80u | ((static_cast<unsigned int>(codepoint) >> 6u) & 0x3Fu)));
                                add(static_cast<char_int_type>(0x80u | (staticlues instead. For instance, calling this function
          with pointer `"/this/that"` on a null value is equivalent to calling
          `operator[]("this").operator[]("that")` on that value, effectively
          changing the null value to an object.

    @param[in] ptr  a JSON value

    @return reference to the JSON value pointed to by the JSON pointer

    @complexity Linear in the length of the JSON pointer.

    @throw parse_error.106   if an array index begins with '0'
    @throw parse_error.109   if an array index was not a number
    @throw out_of_range.404  if the JSON pointer can not be resolved
    */
    template<typename BasicJsonType>
    BasicJsonType& get_unchecked(BasicJsonType* ptr) const
    {
        for (const auto& reference_token : reference_tokens)
        {
            // convert null values to arrays or objects before continuing
            if (ptr->is_null())
            {
                // check if reference token is a number
                const bool nums =
                    std::all_of(reference_token.begin(), reference_token.end(),
                                [](const unsigned char x)
                {
                    return std::isdigit(x);
                });

                // change value to array for numbers or "-" or to object otherwise
                *ptr = (nums || reference_token == "-")
                       ? detail::value_t::array
                       : detail::value_t::object;
            }

            switch (ptr->type())
            {
                case detail::value_t::object:
                {
                    // use unchecked object access
                    ptr = &ptr->operator[](reference_token);
                    break;
                }

                case detail::value_t::array:
                {
                    if (reference_token == "-")
                    {
                        // explicitly treat "-" as index beyond the end
                        ptr = &ptr->operator[](ptr->m_data.m_value.array->size());
                    }
                    else
                    {
                        // convert array index to number; unchecked access
                        ptr = &ptr->operator[](array_index<BasicJsonType>(reference_token));
                    }
                    break;
                }

                case detail::value_t::null:
                case detail::value_t::string:
                case detail::value_t::boolean:
                case detail::value_t::number_integer:
                case detail::value_t::number_unsigned:
                case detail::value_t::number_float:
                case detail::value_t::binary:
                case detail::value_t::discarded:
                default:
                    JSON_THROW(detail::out_of_range::create(404, detail::concat("unresolved reference token '", reference_token, "'"), ptr));
            }
        }

        return *ptr;
    }

    /*!
    @throw parse_error.106   if an array index begins with '0'
    @throw parse_error.109   if an array index was not a number
    @throw out_of_range.402  if the array index '-' is used
    @throw out_of_range.404  if the JSON pointer can not be resolved
    */
    template<typename BasicJsonType>
    BasicJsonType& get_checked(BasicJsonType* ptr) const
    {
        for (const auto& reference_token : reference_tokens)
        {
            switch (ptr->type())
            {
                case detail::value_t::object:
                {
                    // note: at performs range check
                    ptr = &ptr->at(reference_token);
                    break;
                }

                case detail::value_t::array:
                {
                    if (JSON_HEDLEY_UNLIKELY(reference_token == "-"))
                    {
                        // "-" always fails the range check
                        JSON_THROW(detail::out_of_range::create(402, detail::concat(
                                "array index '-' (", std::to_string(ptr->m_data.m_value.array->size()),
                                ") is out of range"), ptr));
                    }

                    // note: at performs range check
                    ptr = &ptr->at(array_index<BasicJsonType>(reference_token));
                    break;
                }

                case detail::value_t::null:
                case detail::value_t::string:
                case detail::value_t::boolean:
                case detail::value_t::number_integer:
                case detail::value_t::number_unsigned:
                case detail::value_t::number_float:
                case detail::value_t::binary:
                case detail::value_t::discarded:
                default:
                    JSON_THROW(detail::out_of_range::create(404, detail::concat("unresolved reference token '", reference_token, "'"), ptr));
            }
        }

        return *ptr;
    }

    /*!
    @brief return a const reference to the pointed to value

    @param[in] ptr  a JSON value

    @return const reference to the JSON value pointed to by the JSON
    pointer

    @throw parse_error.106   if an array index begins with '0'
    @throw parse_error.109   if an array index was not a number
    @throw out_of_range.402  if the array index '-' is used
    @throw out_of_range.404  if the JSON pointer can not be resolved
    */
    template<typename BasicJsonType>
    const BasicJsonType& get_unchecked(const BasicJsonType* ptr) const
    {
        for (const auto& reference_token : reference_tokens)
        {
            switch (ptr->type())
            {
                case detail::value_t::object:
                {
                    // use unchecked object access
                    ptr = &ptr->operator[](reference_token);
                    break;
                }

                case detail::value_t::array:
                {
                    if (JSON_HEDLEY_UNLIKELY(reference_token == "-"))
                    {
                        // "-" cannot be used for const access
                        JSON_THROW(detail::out_of_range::create(402, detail::concat("array index '-' (", std::to_string(ptr->m_data.m_value.array->size()), ") is out of range"), ptr));
                    }

                    // use unchecked array access
                    ptr = &ptr->operator[](array_index<BasicJsonType>(reference_token));
                    break;
                }

                case detail::value_t::null:
                case detail::value_t::string:
                case detail::value_t::boolean:
                case detail::value_t::number_integer:
                case detail::value_t::number_unsigned:
                case detail::value_t::number_float:
                case detail::value_t::binary:
                case detail::value_t::discarded:
                default:
                    JSON_THROW(detail::out_of_range::create(404, detail::concat("unresolved reference token '", reference_token, "'"), ptr));
            }
        }

        return *ptr;
    }

    /*!
    @throw parse_error.106   if an array index begins with '0'
    @throw parse_error.109   if an array index was not a number
    @throw out_of_range.402  if the array index '-' is used
    @throw out_of_range.404  if the JSON pointer can not be resolved
    */
    template<typename BasicJsonType>
    const BasicJsonType& get_checked(const BasicJsonType* ptr) const
    {
        for (const auto& reference_token : reference_tokens)
        {
            switch (ptr->type())
            {
                case detail::value_t::object:
                {
                    // note: at performs range check
                    ptr = &ptr->at(reference_token);
                    break;
                }

                case detail::value_t::array:
                {
                    if (JSON_HEDLEY_UNLIKELY(reference_token == "-"))
                    {
                        // "-" always fails the range check
                        JSON_THROW(detail::out_of_range::create(402, detail::concat(
                                "array index '-' (", std::to_string(ptr->m_data.m_value.array->size()),
                                ") is out of range"), ptr));
                    }

                    // note: at performs range check
                    ptr = &ptr->at(array_index<BasicJsonType>(reference_token));
                    break;
                }

                case detail::value_t::null:
                case detail::value_t::string:
                case detail::value_t::boolean:
                case detail::value_t::number_integer:
                case detail::value_t::number_unsigned:
                case detail::value_t::number_float:
                case detail::value_t::binary:
                case detail::value_t::discarded:
                default:
                    JSON_THROW(detail::out_of_range::create(404, detail::concat("unresolved reference token '", reference_token, "'"), ptr));
            }
        }

        return *ptr;
    }

    /*!
    @throw parse_error.106   if an array index begins with '0'
    @throw parse_error.109   if an array index was not a number
    */
    template<typename BasicJsonType>
    bool contains(const BasicJsonType* ptr) const
    {
        for (const auto& reference_token : reference_tokens)
        {
            switch (ptr->type())
            {
                case detail::value_t::object:
                {
                    if (!ptr->contains(reference_token))
                    {
                        // we did not find the key in the object
                        return false;
                    }

                    ptr = &ptr->operator[](reference_token);
                    break;
                }

                case detail::value_t::array:
                {
                    if (JSON_HEDLEY_UNLIKELY(reference_token == "-"))
                    {
                        // "-" always fails the range check
                        return false;
                    }
                    if (JSON_HEDLEY_UNLIKELY(reference_token.size() == 1 && !("0" <= reference_token && reference_token <= "9")))
                    {
                        // invalid char
                        return false;
                    }
                    if (JSON_HEDLEY_UNLIKELY(reference_token.size() > 1))
                    {
                        if (JSON_HEDLEY_UNLIKELY(!('1' <= reference_token[0] && reference_token[0] <= '9')))
                        {
                            // first char should be between '1' and '9'
                            return false;
                        }
                        for (std::size_t i = 1; i < reference_token.size(); i++)
                        {
                            if (JSON_HEDLEY_UNLIKELY(!('0' <= reference_token[i] && reference_token[i] <= '9')))
                            {
                                // other char should be between '0' and '9'
                                return false;
                            }
                        }
                    }

                    const auto idx = array_index<BasicJsonType>(reference_token);
                    if (idx >= ptr->size())
                    {
                        // index out of range
                        return false;
                    }

                    ptr = &ptr->operator[](idx);
                    break;
                }

                case detail::value_t::null:
                case detail::value_t::string:
                case detail::value_t::boolean:
                case detail::value_t::number_integer:
                case detail::value_t::number_unsigned:
                case detail::value_t::number_float:
                case detail::value_t::binary:
                case detail::value_t::discarded:
                default:
                {
                    // we do not expect primitive values if there is still a
                    // reference token to process
                    return false;
                }
            }
        }

        // no reference token left means we found a primitive value
        return true;
    }

    /*!
    @brief split the string input to reference tokens

    @note This function is only called by the json_pointer constructor.
          All exceptions below are documented there.

    @throw parse_error.107  if the pointer is not empty or begins with '/'
    @throw parse_error.108  if character '~' is not followed by '0' or '1'
    */
    static std::vector<string_t> split(const string_t& reference_string)
    {
        std::vector<string_t> result;

        // special case: empty reference string -> no reference tokens
        if (reference_string.empty())
        {
            return result;
        }

        // check if nonempty reference string begins with slash
        if (JSON_HEDLEY_UNLIKELY(reference_string[0] != '/'))
        {
            JSON_THROW(detail::parse_error::create(107, 1, detail::concat("JSON pointer must be empty or begin with '/' - was: '", reference_string, "'"), nullptr));
        }

        // extract the reference tokens:
        // - slash: position of the last read slash (or end of string)
        // - start: position after the previous slash
        for (
            // search for the first slash after the first character
            std::size_t slash = reference_string.find_first_of('/', 1),
            // set the beginning of the first reference token
            start = 1;
            // we can stop if start == 0 (if slash == string_t::npos)
            start != 0;
            // set the beginning of the next reference token
            // (will eventually be 0 if slash == string_t::npos)
            start = (slash == string_t::npos) ? 0 : slash + 1,
            // find next slash
            slash = reference_string.find_first_of('/', start))
        {
            // use the text between the beginning of the reference token
            // (start) and the last slash (slash).
            auto reference_token = reference_string.substr(start, slash - start);

            // check reference tokens are properly escaped
            for (std::size_t pos = reference_token.find_first_of('~');
                    pos != string_t::npos;
                    pos = reference_token.find_first_of('~', pos + 1))
            {
                JSON_ASSERT(reference_token[pos] == '~');

                // ~ must be followed by 0 or 1
                if (JSON_HEDLEY_UNLIKELY(pos == reference_token.size() - 1 ||
                                         (reference_token[pos + 1] != '0' &&
                                          reference_token[pos + 1] != '1')))
                {
                    JSON_THROW(detail::parse_error::create(108, 0, "escape character '~' must be followed with '0' or '1'", nullptr));
                }
            }

            // finally, store the reference token
            detail::unescape(reference_token);
            result.push_back(reference_token);
        }

        return result;
    }

  private:
    /*!
    @param[in] reference_string  the reference string to the current value
    @param[in] value             the value to consider
    @param[in,out] result        the result object to insert values to

    @note Empty objects or arrays are flattened to `null`.
    */
    template<typename BasicJsonType>
    static void flatten(const string_t& reference_string,
                        const BasicJsonType& value,
                        BasicJsonType& result)
    {
        switch (value.type())
        {
            case detail::value_t::array:
            {
                if (value.m_data.m_value.array->empty())
                {
                    // flatten empty array as null
                    result[reference_string] = nullptr;
                }
                else
                {
                    // iterate array and use index as reference string
                    for (std::size_t i = 0; i < value.m_data.m_value.array->size(); ++i)
                    {
                        flatten(detail::concat(reference_string, '/', std::to_string(i)),
                                value.m_data.m_value.array->operator[](i), result);
                    }
                }
                break;
            }

            case detail::value_t::object:
            {
                if (value.m_data.m_value.object->empty())
                {
                    // flatten empty object as null
                    result[reference_string] = nullptr;
                }
                else
                {
                    // iterate object and use keys as reference string
                    for (const auto& element : *value.m_data.m_value.object)
                    {
                        flatten(detail::concat(reference_string, '/', detail::escape(element.first)), element.second, result);
                    }
                }
                break;
            }

            case detail::value_t::null:
            case detail::value_t::string:
            case detail::value_t::boolean:
            case detail::value_t::number_integer:
            case detail::value_t::number_unsigned:
            case detail::value_t::number_float:
            case detail::value_t::binary:
            case detail::value_t::discarded:
            default:
            {
                // add primitive value with its reference string
                result[reference_string] = value;
                break;
            }
        }
    }

    /*!
    @param[in] value  flattened JSON

    @return unflattened JSON

    @throw parse_error.109 if array index is not a number
    @throw type_error.314  if value is not an object
    @throw type_error.315  if object values are not primitive
    @throw type_error.313  if value cannot be unflattened
    */
    template<typename BasicJsonType>
    static BasicJsonType
    unflatten(const BasicJsonType& value)
    {
        if (JSON_HEDLEY_UNLIKELY(!value.is_object()))
        {
            JSON_THROW(detail::type_error::create(314, "only objects can be unflattened", &value));
        }

        BasicJsonType result;

        // iterate the JSON object values
        for (const auto& element : *value.m_data.m_value.object)
        {
            if (JSON_HEDLEY_UNLIKELY(!element.second.is_primitive()))
            {
                JSON_THROW(detail::type_error::create(315, "values in object must be primitive", &element.second));
            }

            // assign value to reference pointed to by JSON pointer; Note that if
            // the JSON pointer is "" (i.e., points to the whole value), function
            // get_and_create returns a reference to result itself. An assignment
            // will then create a primitive value.
            json_pointer(element.first).get_and_create(result) = element.second;
        }

        return result;
    }

    // can't use conversion operator because of ambiguity
    json_pointer<string_t> convert() const&
    {
        json_pointer<string_t> result;
        result.reference_tokens = reference_tokens;
        return result;
    }

    json_pointer<string_t> convert()&&
    {
        json_pointer<string_t> result;
        result.reference_tokens = std::move(reference_tokens);
        return result;
    }

  public:
#if JSON_HAS_THREE_WAY_COMPARISON
    /// @brief compares two JSON pointers for equality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_eq/
    template<typename RefStringTypeRhs>
    bool operator==(const json_pointer<RefStringTypeRhs>& rhs) const noexcept
    {
        return reference_tokens == rhs.reference_tokens;
    }

    /// @brief compares JSON pointer and string for equality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_eq/
    JSON_HEDLEY_DEPRECATED_FOR(3.11.2, operator==(json_pointer))
    bool operator==(const string_t& rhs) const
    {
        return *this == json_pointer(rhs);
    }

    /// @brief 3-way compares two JSON pointers
    template<typename RefStringTypeRhs>
    std::strong_ordering operator<=>(const json_pointer<RefStringTypeRhs>& rhs) const noexcept // *NOPAD*
    {
        return  reference_tokens <=> rhs.reference_tokens; // *NOPAD*
    }
#else
    /// @brief compares two JSON pointers for equality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_eq/
    template<typename RefStringTypeLhs, typename RefStringTypeRhs>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator==(const json_pointer<RefStringTypeLhs>& lhs,
                           const json_pointer<RefStringTypeRhs>& rhs) noexcept;

    /// @brief compares JSON pointer and string for equality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_eq/
    template<typename RefStringTypeLhs, typename StringType>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator==(const json_pointer<RefStringTypeLhs>& lhs,
                           const StringType& rhs);

    /// @brief compares string and JSON pointer for equality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_eq/
    template<typename RefStringTypeRhs, typename StringType>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator==(const StringType& lhs,
                           const json_pointer<RefStringTypeRhs>& rhs);

    /// @brief compares two JSON pointers for inequality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_ne/
    template<typename RefStringTypeLhs, typename RefStringTypeRhs>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator!=(const json_pointer<RefStringTypeLhs>& lhs,
                           const json_pointer<RefStringTypeRhs>& rhs) noexcept;

    /// @brief compares JSON pointer and string for inequality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_ne/
    template<typename RefStringTypeLhs, typename StringType>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator!=(const json_pointer<RefStringTypeLhs>& lhs,
                           const StringType& rhs);

    /// @brief compares string and JSON pointer for inequality
    /// @sa https://json.nlohmann.me/api/json_pointer/operator_ne/
    template<typename RefStringTypeRhs, typename StringType>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator!=(const StringType& lhs,
                           const json_pointer<RefStringTypeRhs>& rhs);

    /// @brief compares two JSON pointer for less-than
    template<typename RefStringTypeLhs, typename RefStringTypeRhs>
    // NOLINTNEXTLINE(readability-redundant-declaration)
    friend bool operator<(const json_pointer<RefStringTypeLhs>& lhs,
                          const json_pointer<RefStringTypeRhs>& rhs) noexcept;
#endif

  private:
    /// the reference tokens
    std::vector<string_t> reference_tokens;
};

#if !JSON_HAS_THREE_WAY_COMPARISON
// functions cannot be defined inside class due to ODR violations
template<typename RefStringTypeLhs, typename RefStringTypeRhs>
inline bool operator==(const json_pointer<RefStringTypeLhs>& lhs,
                       const json_pointer<RefStringTypeRhs>& rhs) noexcept
{
    return lhs.reference_tokens == rhs.reference_tokens;
}

template<typename RefStringTypeLhs,
         typename StringType = typename json_pointer<RefStringTypeLhs>::string_t>
JSON_HEDLEY_DEPRECATED_FOR(3.11.2, operator==(json_pointer, json_pointer))
inline bool operator==(const json_pointer<RefStringTypeLhs>& lhs,
                       const StringType& rhs)
{
    return lhs == json_pointer<RefStringTypeLhs>(rhs);
}

template<typename RefStringTypeRhs,
         typename StringType = typename json_pointer<RefStringTypeRhs>::string_t>
JSON_HEDLEY_DEPRECATED_FOR(3.11.2, operator==(json_pointer, json_pointer))
inline bool operator==(const StringType& lhs,
                       const json_pointer<RefStringTypeRhs>& rhs)
{
    return json_pointer<RefStringTypeRhs>(lhs) == rhs;
}

template<typename RefStringTypeLhs, typename RefStringTypeRhs>
inline bool operator!=(const json_pointer<RefStringTypeLhs>& lhs,
                       const json_pointer<RefStringTypeRhs>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename RefStringTypeLhs,
         typename StringType = typename json_pointer<RefStringTypeLhs>::string_t>
JSON_HEDLEY_DEPRECATED_FOR(3.11.2, operator!=(json_pointer, json_pointer))
inline bool operator!=(const json_pointer<RefStringTypeLhs>& lhs,
                       const StringType& rhs)
{
    return !(lhs == rhs);
}

template<typename RefStringTypeRhs,
         typename StringType = typename json_pointer<RefStringTypeRhs>::string_t>
JSON_HEDLEY_DEPRECATED_FOR(3.11.2, operator!=(json_pointer, json_pointer))
inline bool operator!=(const StringType& lhs,
                       const json_pointer<RefStringTypeRhs>& rhs)
{
    return !(lhs == rhs);
}

template<typename RefStringTypeLhs, typename RefStringTypeRhs>
inline bool operator<(const json_pointer<RefStringTypeLhs>& lhs,
                      const json_pointer<RefStringTypeRhs>& rhs) noexcept
{
    return lhs.reference_tokens < rhs.reference_tokens;
}
#endif

NLOHMANN_JSON_NAMESPACE_END

// #include <nlohmann/detail/json_ref.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <initializer_list>
#include <utility>

// #include <nlohmann/detail/abi_macros.hpp>

// #include <nlohmann/detail/meta/type_traits.hpp>


NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

template<typename BasicJsonType>
class json_ref
{
  public:
    using value_type = BasicJsonType;

    json_ref(value_type&& value)
        : owned_value(std::move(value))
    {}

    json_ref(const value_type& value)
        : value_ref(&value)
    {}

    json_ref(std::initializer_list<json_ref> init)
        : owned_value(init)
    {}

    template <
        class... Args,
        enable_if_t<std::is_constructible<value_type, Args...>::value, int> = 0 >
    json_ref(Args && ... args)
        : owned_value(std::forward<Args>(args)...)
    {}

    // class should be movable only
    json_ref(json_ref&&) noexcept = default;
    json_ref(const json_ref&) = delete;
    json_ref& operator=(const json_ref&) = delete;
    json_ref& operator=(json_ref&&) = delete;
    ~json_ref() = default;

    value_type moved_or_copied() const
    {
        if (value_ref == nullptr)
        {
            return std::move(owned_value);
        }
        return *value_ref;
    }

    value_type const& operator*() const
    {
        return value_ref ? *value_ref : owned_value;
    }

    value_type const* operator->() const
    {
        return &** this;
    }

  private:
    mutable value_type owned_value = nullptr;
    value_type const* value_ref = nullptr;
};

}  // namespace detail
NLOHMANN_JSON_NAMESPACE_END

// #include <nlohmann/detail/macro_scope.hpp>

// #include <nlohmann/detail/string_concat.hpp>

// #include <nlohmann/detail/string_escape.hpp>

// #include <nlohmann/detail/meta/cpp_future.hpp>

// #include <nlohmann/detail/meta/type_traits.hpp>

// #include <nlohmann/detail/output/binary_writer.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <algorithm> // reverse
#include <array> // array
#include <map> // map
#include <cmath> // isnan, isinf
#include <cstdint> // uint8_t, uint16_t, uint32_t, uint64_t
#include <cstring> // memcpy
#include <limits> // numeric_limits
#include <string> // string
#include <utility> // move
#include <vector> // vector

// #include <nlohmann/detail/input/binary_reader.hpp>

// #include <nlohmann/detail/macro_scope.hpp>

// #include <nlohmann/detail/output/output_adapters.hpp>
//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++
// |  |  |__   |  |  | | | |  version 3.11.3
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2023 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT



#include <algorithm> // copy
#include <cstddef> // size_t
#include <iterator> // back_inserter
#include <memory> // shared_ptr, make_shared
#include <string> // basic_string
#include <vector> // vector

#ifndef JSON_NO_IO
    #include <ios>      // streamsize
    #include <ostream>  // basic_ostream
#endif  // JSON_NO_IO

// #include <nlohmann/detail/macro_scope.hpp>


NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

/// abstract output adapter interface
template<typename CharType> struct output_adapter_protocol
{
    virtual void write_character(CharType c) = 0;
    virtual void write_characters(const CharType* s, std::size_t length) = 0;
    virtual ~output_adapter_protocol() = default;

    output_adapter_protocol() = default;
    output_adapter_protocol(const output_adapter_protocol&) = default;
    output_adapter_protocol(output_adapter_protocol&&) noexcept = default;
    output_adapter_protocol& operator=(const output_adapter_protocol&) = default;
    output_adapter_protocol& operator=(output_adapter_protocol&&) noexcept = default;
};

/// a type to simplify interfaces
template<typename CharType>
using output_adapter_t = std::shared_ptr<output_adapter_protocol<CharType>>;

/// output adapter for byte vectors
template<typename CharType, typename AllocatorType = std::allocator<CharType>>
class output_vector_adapter : public output_adapter_protocol<CharType>
{
  public:
    explicit output_vector_adapter(std::vector<CharType, AllocatorType>& vec) noexcept
        : v(vec)
    {}

    void write_character(CharType c) override
    {
        v.push_back(c);
    }

    JSON_HEDLEY_NON_NULL(2)
    void write_characters(const CharType* s, std::size_t length) override
    {
        v.insert(v.end(), s, s + length);
    }

  private:
    std::vector<CharType, AllocatorType>& v;
};

#ifndef JSON_NO_IO
/// output adapter for output streams
template<typename CharType>
class output_stream_adapter : public output_adapter_protocol<CharType>
{
  public:
    explicit output_stream_adapter(std::basic_ostream<CharType>& s) noexcept
        : stream(s)
    {}

    void write_character(CharType c) override
    {
        stream.put(c);
    }

    JSON_HEDLEY_NON_NULL(2)
    void write_characters(const CharType* s, std::size_t length) override
    {
        stream.write(s, static_cast<std::streamsize>(length));
    }

  private:
    std::basic_ostream<CharType>& stream;
};
#endif  // JSON_NO_IO

/// output adapter for basic_string
template<typename CharType, typename StringType = std::basic_string<CharType>>
class output_string_adapter : public output_adapter_protocol<CharType>
{
  public:
    explicit output_string_adapter(StringType& s) noexcept
        : str(s)
    {}

    void write_character(CharType c) override
    {
        str.push_back(c);
    }

    JSON_HEDLEY_NON_NULL(2)
    void write_characters(const CharType* s, std::size_t length) override
    {
        str.append(s, length);
    }

  private:
    StringType& str;
};

template<typename CharType, typename StringType = std::basic_string<CharType>>
class output_adapter
{
  public:
    template<typename AllocatorType = std::allocator<CharType>>
    output_adapter(std::vector<CharType, AllocatorType>& vec)
        : oa(std::make_shared<output_vector_adapter<CharType, AllocatorType>>(vec)) {}

#ifndef JSON_NO_IO
    output_adapter(std::basic_ostream<CharType>& s)
        : oa(std::make_shared<output_stream_adapter<CharType>>(s)) {}
#endif  // JSON_NO_IO

    output_adapter(StringType& s)
        : oa(std::make_shared<output_string_adapter<CharType, StringType>>(s)) {}

    operator output_adapter_t<CharType>()
    {
        return oa;
    }

  private:
    output_adapter_t<CharType> oa = nullptr;
};

}  // namespace detail
NLOHMANN_JSON_NAMESPACE_END

// #include <nlohmann/detail/string_concat.hpp>


NLOHMANN_JSON_NAMESPACE_BEGIN
namespace detail
{

///////////////////
// binary writer //
///////////////////

/*!
@brief serialization to CBOR and MessagePack values
*/
template<typename BasicJsonType, typename CharType>
class binary_writer
{
    using string_t = typename BasicJsonType::string_t;
    using binary_t = typename BasicJsonType::binary_t;
    using number_float_t = typena@brief access specified array element with bounds checking
    /// @sa https://json.nlohmann.me/api/basic_json/at/
    const_reference at(size_type idx) const
    {
        // at only works for arrays
        if (JSON_HEDLEY_LIKELY(is_array()))
        {
            JSON_TRY
            {
                return m_data.m_value.array->at(idx);
            }
            JSON_CATCH (std::out_of_range&)
            {
                // create better exception explanation
                JSON_THROW(out_of_range::create(401, detail::concat("array index ", std::to_string(idx), " is out of range"), this));
            }
        }
        else
        {
            JSON_THROW(type_error::create(304, detail::concat("cannot use at() with ", type_name()), this));
        }
    }

    /// @brief access specified object element with bounds checking
    /// @sa https://json.nlohmann.me/api/basic_json/at/
    reference at(const typename object_t::key_type& key)
    {
        // at only works for objects
        if (JSON_HEDLEY_UNLIKELY(!is_object()))
        {
            JSON_THROW(type_error::create(304, detail::concat("cannot use at() with ", type_name()), this));
        }

        auto it = m_data.m_value.object->find(key);
        if (it == m_data.m_value.object->end())
        {
            JSON_THROW(out_of_range::create(403, detail::concat("key '", key, "' not found"), this));
        }
        return set_parent(it->second);
    }

    /// @brief access specified object element with bounds checking
    /// @sa https://json.nlohmann.me/api/basic_json/at/
    template<class KeyType, detail::enable_if_t<
                 detail::is_usable_as_basic_json_key_type<basic_json_t, KeyType>::value, int> = 0>
    reference at(KeyType && key)
    {
        // at only works for objects
        if (JSON_HEDLEY_UNLIKELY(!is_object()))
        {
            JSON_THROW(type_error::create(304, detail::concat("cannot use at() with ", type_name()), this));
        }

        auto it = m_data.m_value.object->find(std::forward<KeyType>(key));
        if (it == m_data.m_value.object->end())
        {
            JSON_THROW(out_of_range::create(403, detail::concat("key '", string_t(std::forward<KeyType>(key)), "' not found"), this));
        }
        return set_parent(it->second);
    }

    /// @brief access specified object element with bounds checking
    /// @sa https://json.nlohmann.me/api/basic_json/at/
    const_reference at(const typename object_t::key_type& key) const
    {
        // at only works for objects
        if (JSON_HEDLEY_UNLIKELY(!is_object()))
        {
            JSON_THROW(type_error::create(304, detail::concat("cannot use at() with ", type_name()), this));
        }

        auto it = m_data.m_value.object->find(key);
        if (it == m_data.m_value.object->end())
        {
            JSON_THROW(out_of_range::create(403, detail::concat("key '", key, "' not found"), this));
        }
        return it->second;
    }

    /// @brief access specified object element with bounds checking
    /// @sa https://json.nlohmann.me/api/basic_json/at/
    template<class KeyType, detail::enable_if_t<
                 detail::is_usable_as_basic_json_key_type<basic_json_t, KeyType>::value, int> = 0>
    const_reference at(KeyType && key) const
    {
        // at only works for objects
        if (JSON_HEDLEY_UNLIKELY(!is_object()))
        {
            JSON_THROW(type_error::create(304, detail::concat("cannot use at() with ", type_name()), this));
        }

        auto it = m_data.m_value.object->find(std::forward<KeyType>(key));
        if (it == m_data.m_value.object->end())
        {
            JSON_THROW(out_of_range::create(403, detail::concat("key '", string_t(std::forward<KeyType>(key)), "' not found"), this));
        }
        return it->second;
    }

    /// @brief access specified array element
    /// @sa https://json.nlohmann.me/api/basic_json/operator%5B%5D/
    reference operator[](size_type idx)
    {
        // implicitly convert null value to an empty array
        if (is_null())
        {
            m_data.m_type = value_t::array;
            m_data.m_value.array = create<array_t>();
            assert_invariant();
        }

        // operator[] only works for arrays
        if (JSON_HEDLEY_LIKELY(is_array()))
        {
            // fill up array with null values if given idx is outside range
            if (idx >= m_data.m_value.array->size())
            {
#if JSON_DIAGNOSTICS
                // remember array size & capacity before resizing
                const auto old_size = m_data.m_value.array->size();
                const auto old_capacity = m_data.m_value.array->capacity();
#endif
                m_data.m_value.array->resize(idx + 1);

#if JSON_DIAGNOSTICS
                if (JSON_HEDLEY_UNLIKELY(m_data.m_value.array->capacity() != old_capacity))
                {
                    // capacity has changed: update all parents
                    set_parents();
                }
                else
                {
                    // set parent for values added above
                    set_parents(begin() + static_cast<typename iterator::difference_type>(old_size), static_cast<typename iterator::difference_type>(idx + 1 - old_size));
                }
#endif
                assert_invariant();
            }

            return m_data.m_value.array->operator[](idx);
        }

        JSON_THROW(type_error::create(305, detail::concat("cannot use operator[] with a numeric argument with ", type_name()), this));
    }

    /// @brief access specified array element
    /// @sa https://json.nlohmann.me/api/basic_json/operator%5B%5D/
    const_reference operator[](size_type idx) const
    {
        // const operator[] only works for arrays
        if (JSON_HEDLEY_LIKELY(is_array()))
        {
            return m_data.m_value.array->operator[](idx);
        }

        JSON_THROW(type_error::create(305, detail::concat("cannot use operator[] with a numeric argument with ", type_name()), this));
    }

    /// @brief access specified object element
    /// @sa https://json.nlohmann.me/api/basic_json/operator%5B%5D/
    reference operator[](typename object_t::key_type key)
    {
        // implicitly convert null value to an empty object
        if (is_null())
        {
            m_data.m_type = value_t::object;
            m_data.m_value.object = create<object_t>();
            assert_invariant();
        }

        // operator[] only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            auto result = m_data.m_value.object->emplace(std::move(key), nullptr);
            return set_parent(result.first->second);
        }

        JSON_THROW(type_error::create(305, detail::concat("cannot use operator[] with a string argument with ", type_name()), this));
    }

    /// @brief access specified object element
    /// @sa https://json.nlohmann.me/api/basic_json/operator%5B%5D/
    const_reference operator[](const typename object_t::key_type& key) const
    {
        // const operator[] only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            auto it = m_data.m_value.object->find(key);
            JSON_ASSERT(it != m_data.m_value.object->end());
            return it->second;
        }

        JSON_THROW(type_error::create(305, detail::concat("cannot use operator[] with a string argument with ", type_name()), this));
    }

    // these two functions resolve a (const) char * ambiguity affecting Clang and MSVC
    // (they seemingly cannot be constrained to resolve the ambiguity)
    template<typename T>
    reference operator[](T* key)
    {
        return operator[](typename object_t::key_type(key));
    }

    template<typename T>
    const_reference operator[](T* key) const
    {
        return operator[](typename object_t::key_type(key));
    }

    /// @brief access specified object element
    /// @sa https://json.nlohmann.me/api/basic_json/operator%5B%5D/
    template<class KeyType, detail::enable_if_t<
                 detail::is_usable_as_basic_json_key_type<basic_json_t, KeyType>::value, int > = 0 >
    reference operator[](KeyType && key)
    {
        // implicitly convert null value to an empty object
        if (is_null())
        {
            m_data.m_type = value_t::object;
            m_data.m_value.object = create<object_t>();
            assert_invariant();
        }

        // operator[] only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            auto result = m_data.m_value.object->emplace(std::forward<KeyType>(key), nullptr);
            return set_parent(result.first->second);
        }

        JSON_THROW(type_error::create(305, detail::concat("cannot use operator[] with a string argument with ", type_name()), this));
    }

    /// @brief access specified object element
    /// @sa https://json.nlohmann.me/api/basic_json/operator%5B%5D/
    template<class KeyType, detail::enable_if_t<
                 detail::is_usable_as_basic_json_key_type<basic_json_t, KeyType>::value, int > = 0 >
    const_reference operator[](KeyType && key) const
    {
        // const operator[] only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            auto it = m_data.m_value.object->find(std::forward<KeyType>(key));
            JSON_ASSERT(it != m_data.m_value.object->end());
            return it->second;
        }

        JSON_THROW(type_error::create(305, detail::concat("cannot use operator[] with a string argument with ", type_name()), this));
    }

  private:
    template<typename KeyType>
    using is_comparable_with_object_key = detail::is_comparable <
        object_comparator_t, const typename object_t::key_type&, KeyType >;

    template<typename ValueType>
    using value_return_type = std::conditional <
        detail::is_c_string_uncvref<ValueType>::value,
        string_t, typename std::decay<ValueType>::type >;

  public:
    /// @brief access specified object element with default value
    /// @sa https://json.nlohmann.me/api/basic_json/value/
    template < class ValueType, detail::enable_if_t <
                   !detail::is_transparent<object_comparator_t>::value
                   && detail::is_getable<basic_json_t, ValueType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    ValueType value(const typename object_t::key_type& key, const ValueType& default_value) const
    {
        // value only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            // if key is found, return value and given default value otherwise
            const auto it = find(key);
            if (it != end())
            {
                return it->template get<ValueType>();
            }

            return default_value;
        }

        JSON_THROW(type_error::create(306, detail::concat("cannot use value() with ", type_name()), this));
    }

    /// @brief access specified object element with default value
    /// @sa https://json.nlohmann.me/api/basic_json/value/
    template < class ValueType, class ReturnType = typename value_return_type<ValueType>::type,
               detail::enable_if_t <
                   !detail::is_transparent<object_comparator_t>::value
                   && detail::is_getable<basic_json_t, ReturnType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    ReturnType value(const typename object_t::key_type& key, ValueType && default_value) const
    {
        // value only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            // if key is found, return value and given default value otherwise
            const auto it = find(key);
            if (it != end())
            {
                return it->template get<ReturnType>();
            }

            return std::forward<ValueType>(default_value);
        }

        JSON_THROW(type_error::create(306, detail::concat("cannot use value() with ", type_name()), this));
    }

    /// @brief access specified object element with default value
    /// @sa https://json.nlohmann.me/api/basic_json/value/
    template < class ValueType, class KeyType, detail::enable_if_t <
                   detail::is_transparent<object_comparator_t>::value
                   && !detail::is_json_pointer<KeyType>::value
                   && is_comparable_with_object_key<KeyType>::value
                   && detail::is_getable<basic_json_t, ValueType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    ValueType value(KeyType && key, const ValueType& default_value) const
    {
        // value only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            // if key is found, return value and given default value otherwise
            const auto it = find(std::forward<KeyType>(key));
            if (it != end())
            {
                return it->template get<ValueType>();
            }

            return default_value;
        }

        JSON_THROW(type_error::create(306, detail::concat("cannot use value() with ", type_name()), this));
    }

    /// @brief access specified object element via JSON Pointer with default value
    /// @sa https://json.nlohmann.me/api/basic_json/value/
    template < class ValueType, class KeyType, class ReturnType = typename value_return_type<ValueType>::type,
               detail::enable_if_t <
                   detail::is_transparent<object_comparator_t>::value
                   && !detail::is_json_pointer<KeyType>::value
                   && is_comparable_with_object_key<KeyType>::value
                   && detail::is_getable<basic_json_t, ReturnType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    ReturnType value(KeyType && key, ValueType && default_value) const
    {
        // value only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            // if key is found, return value and given default value otherwise
            const auto it = find(std::forward<KeyType>(key));
            if (it != end())
            {
                return it->template get<ReturnType>();
            }

            return std::forward<ValueType>(default_value);
        }

        JSON_THROW(type_error::create(306, detail::concat("cannot use value() with ", type_name()), this));
    }

    /// @brief access specified object element via JSON Pointer with default value
    /// @sa https://json.nlohmann.me/api/basic_json/value/
    template < class ValueType, detail::enable_if_t <
                   detail::is_getable<basic_json_t, ValueType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    ValueType value(const json_pointer& ptr, const ValueType& default_value) const
    {
        // value only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            // if pointer resolves a value, return it or use default value
            JSON_TRY
            {
                return ptr.get_checked(this).template get<ValueType>();
            }
            JSON_INTERNAL_CATCH (out_of_range&)
            {
                return default_value;
            }
        }

        JSON_THROW(type_error::create(306, detail::concat("cannot use value() with ", type_name()), this));
    }

    /// @brief access specified object element via JSON Pointer with default value
    /// @sa https://json.nlohmann.me/api/basic_json/value/
    template < class ValueType, class ReturnType = typename value_return_type<ValueType>::type,
               detail::enable_if_t <
                   detail::is_getable<basic_json_t, ReturnType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    ReturnType value(const json_pointer& ptr, ValueType && default_value) const
    {
        // value only works for objects
        if (JSON_HEDLEY_LIKELY(is_object()))
        {
            // if pointer resolves a value, return it or use default value
            JSON_TRY
            {
                return ptr.get_checked(this).template get<ReturnType>();
            }
            JSON_INTERNAL_CATCH (out_of_range&)
            {
                return std::forward<ValueType>(default_value);
            }
        }

        JSON_THROW(type_error::create(306, detail::concat("cannot use value() with ", type_name()), this));
    }

    template < class ValueType, class BasicJsonType, detail::enable_if_t <
                   detail::is_basic_json<BasicJsonType>::value
                   && detail::is_getable<basic_json_t, ValueType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    JSON_HEDLEY_DEPRECATED_FOR(3.11.0, basic_json::json_pointer or nlohmann::json_pointer<basic_json::string_t>) // NOLINT(readability/alt_tokens)
    ValueType value(const ::nlohmann::json_pointer<BasicJsonType>& ptr, const ValueType& default_value) const
    {
        return value(ptr.convert(), default_value);
    }

    template < class ValueType, class BasicJsonType, class ReturnType = typename value_return_type<ValueType>::type,
               detail::enable_if_t <
                   detail::is_basic_json<BasicJsonType>::value
                   && detail::is_getable<basic_json_t, ReturnType>::value
                   && !std::is_same<value_t, detail::uncvref_t<ValueType>>::value, int > = 0 >
    JSON_HEDLEY_DEPRECATED_FOR(3.11.0, basic_json::json_pointer or nlohmann::json_pointer<basic_json::string_t>) // NOLINT(readability/alt_tokens)
    ReturnType value(const ::nlohmann::json_pointer<BasicJsonType>& ptr, ValueType && default_value) const
    {
        return value(ptr.convert(), std::forward<ValueType>(default_value));
    }

    /// @brief access the first element
    /// @sa https://json.nlohmann.me/api/basic_json/front/
    reference front()
    {
        return *begin();
    }

    /// @brief access the first element
    /// @sa https://json.nlohmann.me/api/basic_json/front/
    const_reference front() const
    {
        return *cbegin();
    }

    /// @brief access the last element
    /// @sa https://json.nlohmann.me/api/basic_json/back/
    reference back()
    {
        auto tmp = end();
        --tmp;
        return *tmp;
    }

    /// @brief access the last element
    /// @sa https://json.nlohmann.me/api/basic_json/back/
    const_reference back() const
    {
        auto tmp = cend();
        --tmp;
        return *tmp;
    }

    /// @brief remove element given an iterator
    /// @sa https://json.nlohmann.me/api/basic_json/erase/
    template < class IteratorType, detail::enable_if_t <
                   std::is_same<IteratorType, typename basic_json_t::iterator>::value ||
                   std::is_same<IteratorType, typename basic_json_t::const_iterator>::value, int > = 0 >
    IteratorType erase(IteratorType pos)
    {
        // make sure iterator fits the current value
        if (JSON_HEDLEY_UNLIKELY(this != pos.m_object))
        {
            JSON_THROW(invalid_iterator::create(202, "iterator does not fit current value", this));
        }

        IteratorType result = end();

        switch (m_data.m_type)
        {
            case value_t::boolean:
            case value_t::number_float:
            case value_t::number_integer:
            case value_t::number_unsigned:
            case value_t::string:
            case value_t::binary:
            {
                if (JSON_HEDLEY_UNLIKELY(!pos.m_it.primitive_iterator.is_begin()))
                {
                    JSON_THROW(invalid_iterator::create(205, "iterator out of range", this));
                }

                if (is_string())
                {
                    AllocatorType<string_t> alloc;
                    std::allocator_traits<decltype(alloc)>::destroy(alloc, m_data.m_value.string);
                    std::allocator_traits<decltype(alloc)>::deallocate(alloc, m_data.m_value.string, 1);
                    m_data.m_value.string = nullptr;
                }
                else if (is_binary())
                {
                    AllocatorType<binary_t> alloc;
                    std::allocator_traits<decltype(alloc)>::destroy(alloc, m_data.m_value.binary);
                    std::allocator_traits<decltype(alloc)>::deall