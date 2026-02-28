#ifdef OS_BROWSER
/*
 * Copyright 2012 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#ifndef EMSCRIPTEN_H
#define EMSCRIPTEN_H

/**
 * This file contains a few useful things for compiling C/C++ code
 * with Emscripten.
 *
 * Documentation for the public APIs defined in this file must be updated in: 
 *    site/source/docs/api_reference/emscripten.h.rst
 * A prebuilt local version of the documentation is available at: 
 *    site/build/text/docs/api_reference/emscripten.h.txt
 * You can also build docs locally as HTML or other formats in site/
 * An online HTML version (which may be of a different version of Emscripten)
 *    is up at http://kripken.github.io/emscripten-site/docs/api_reference/emscripten.h.html
 */

/*
 * Copyright 2017 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// You can use these functions by passing format string to arg_sigs.
// Note that `code` requires you to provide a const C string known at compile
// time, otherwise the "unable to find data for ASM/EM_JS const" error will be
// thrown.
// https://github.com/WebAssembly/binaryen/blob/51c8f2469f8fd05197b7694c65041b1567f2c6b5/src/wasm/wasm-emscripten.cpp#L183

// C++ needs the nothrow attribute so -O0 doesn't lower these calls as invokes.
__attribute__((nothrow))
int emscripten_asm_const_int(const char* code, const char* arg_sigs, ...);
__attribute__((nothrow))
void* emscripten_asm_const_ptr(const char* code, const char* arg_sigs, ...);
__attribute__((nothrow))
double emscripten_asm_const_double(const char* code, const char* arg_sigs, ...);

__attribute__((nothrow))
int emscripten_asm_const_int_sync_on_main_thread(
  const char* code, const char* arg_sigs, ...);
__attribute__((nothrow))
void* emscripten_asm_const_ptr_sync_on_main_thread(
  const char* code, const char* arg_sigs, ...);
__attribute__((nothrow))
double emscripten_asm_const_double_sync_on_main_thread(
  const char* code, const char* arg_sigs, ...);

__attribute__((nothrow))
void emscripten_asm_const_async_on_main_thread(
  const char* code, const char* arg_sigs, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

// EM_ASM does not work in strict C mode.
#if !defined(__cplusplus) && defined(__STRICT_ANSI__)

#define EM_ASM_ERROR _Pragma("GCC error(\"EM_ASM does not work in -std=c* modes, use -std=gnu* modes instead\")")
#define EM_ASM(...) EM_ASM_ERROR
#define EM_ASM_INT(...) EM_ASM_ERROR
#define EM_ASM_PTR(...) EM_ASM_ERROR
#define EM_ASM_DOUBLE(...) EM_ASM_ERROR
#define MAIN_THREAD_EM_ASM(...) EM_ASM_ERROR
#define MAIN_THREAD_EM_ASM_INT(...) EM_ASM_ERROR
#define MAIN_THREAD_EM_ASM_PTR(...) EM_ASM_ERROR
#define MAIN_THREAD_EM_ASM_DOUBLE(...) EM_ASM_ERROR
#define MAIN_THREAD_ASYNC_EM_ASM(...) EM_ASM_ERROR
#define EM_ASM_(...) EM_ASM_ERROR
#define EM_ASM_ARGS(...) EM_ASM_ERROR
#define EM_ASM_INT_V(...) EM_ASM_ERROR
#define EM_ASM_DOUBLE_V(...) EM_ASM_ERROR

#else

// In wasm backend, we need to call the emscripten_asm_const_* functions with
// the C vararg calling convention, because we will call it with a variety of
// arguments, but need to generate a coherent import for the wasm module before
// binaryen can run over it to fix up any calls to emscripten_asm_const_*.  In
// order to read from a vararg buffer, we need to know the signatures to read.
// We can use compile-time trickery to generate a format string, and read that
// in JS in order to correctly handle the vararg buffer.

#ifndef __cplusplus

// We can use the generic selection C11 feature (that clang supports pre-C11
// as an extension) to emulate function overloading in C.
// All other types, including *all* pointer types go through the default case
#ifdef __wasm64__
#define LONG_CODE 'j'
#else
#define LONG_CODE 'i'
#endif
#define _EM_ASM_SIG_CHAR(x) _Generic((x), \
    float: 'f', \
    double: 'd', \
    char: 'i', \
    unsigned char: 'i', \
    unsigned short: 'i', \
    unsigned int: 'i', \
    unsigned long: LONG_CODE, \
    unsigned long long: 'j', \
    signed char: 'i', \
    signed short: 'i', \
    signed int: 'i', \
    signed long: LONG_CODE, \
    signed long long: 'j', \
    default: 'p')

// This indirection is needed to allow us to concatenate computed results, e.g.
//   #define BAR(N) _EM_ASM_CONCATENATE(FOO_, N)
//   BAR(3) // rewritten to BAR_3
// whereas using ## or _EM_ASM_CONCATENATE_ directly would result in BAR_N
#define _EM_ASM_CONCATENATE(a, b) _EM_ASM_CONCATENATE_(a, b)
#define _EM_ASM_CONCATENATE_(a, b) a##b

// Counts arguments. We use $$ as a sentinel value to enable using ##__VA_ARGS__
// which omits a comma in the event that we have 0 arguments passed, which is
// necessary to keep the count correct.
#define _EM_ASM_COUNT_ARGS_EXP(_$,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,n,...) n
#define _EM_ASM_COUNT_ARGS(...) \
    _EM_ASM_COUNT_ARGS_EXP($$,##__VA_ARGS__,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

// Find the corresponding char for each argument.
#define _EM_ASM_ARG_SIGS_0(x, ...)
#define _EM_ASM_ARG_SIGS_1(x, ...) _EM_ASM_SIG_CHAR(x),
#define _EM_ASM_ARG_SIGS_2(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_1(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_3(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_2(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_4(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_3(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_5(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_4(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_6(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_5(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_7(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_6(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_8(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_7(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_9(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_8(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_10(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_9(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_11(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_10(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_12(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_11(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_13(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_12(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_14(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_13(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_15(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_14(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_16(x, ...) _EM_ASM_SIG_CHAR(x), _EM_ASM_ARG_SIGS_15(__VA_ARGS__)
#define _EM_ASM_ARG_SIGS_(N, ...) \
    ((char[]){ _EM_ASM_CONCATENATE(_EM_ASM_ARG_SIGS_,N)(__VA_ARGS__) '\0' })

#define _EM_ASM_ARG_SIGS(...) \
    _EM_ASM_ARG_SIGS_(_EM_ASM_COUNT_ARGS(__VA_ARGS__), ##__VA_ARGS__)

// We lead with commas to avoid adding an extra comma in the 0-argument case.
#define _EM_ASM_PREP_ARGS(...) , _EM_ASM_ARG_SIGS(__VA_ARGS__), ##__VA_ARGS__

#else // __cplusplus

// C++ needs to support vararg template parameter packs, e.g. like in
// test/core/test_em_asm_parameter_pack.cpp. Because of that, a macro-only
// approach doesn't work (a macro applied to a parameter pack would expand
// incorrectly). So we can use a template class instead to build a temporary
// buffer of characters.

// As emscripten is required to build successfully with -std=c++03, we cannot
// use std::tuple or std::integral_constant. Using C++11 features is only a
// warning in modern Clang, which are ignored in system headers.
template<typename, typename = void> struct __em_asm_sig {};
template<> struct __em_asm_sig<float> { static const char value = 'f'; };
template<> struct __em_asm_sig<double> { static const char value = 'd'; };
template<> struct __em_asm_sig<char> { static const char value = 'i'; };
template<> struct __em_asm_sig<signed char> { static const char value = 'i'; };
template<> struct __em_asm_sig<unsigned char> { static const char value = 'i'; };
template<> struct __em_asm_sig<short> { static const char value = 'i'; };
template<> struct __em_asm_sig<unsigned short> { static const char value = 'i'; };
template<> struct __em_asm_sig<int> { static const char value = 'i'; };
template<> struct __em_asm_sig<unsigned int> { static const char value = 'i'; };
#if __wasm64__
template<> struct __em_asm_sig<long> { static const char value = 'j'; };
template<> struct __em_asm_sig<unsigned long> { static const char value = 'j'; };
#else
template<> struct __em_asm_sig<long> { static const char value = 'i'; };
template<> struct __em_asm_sig<unsigned long> { static const char value = 'i'; };
#endif
template<> struct __em_asm_sig<bool> { static const char value = 'i'; };
template<> struct __em_asm_sig<wchar_t> { static const char value = 'i'; };
template<> struct __em_asm_sig<long long> { static const char value = 'j'; };
template<> struct __em_asm_sig<unsigned long long> { static const char value = 'j'; };
template<typename T> struct __em_asm_sig<T*> { static const char value = 'p'; };

// Explicit support for enums, they're passed as int via variadic arguments.
template<bool> struct __em_asm_if { };
template<> struct __em_asm_if<true> { typedef void type; };
template<typename T> struct __em_asm_sig<T, typename __em_asm_if<__is_enum(T)>::type> {
    static const char value = 'i';
};

// Instead of std::tuple
template<typename... Args>
struct __em_asm_type_tuple {};

// Instead of std::make_tuple
template<typename... Args>
__em_asm_type_tuple<Args...> __em_asm_make_type_tuple(Args... args) {
    return {};
}

template<typename>
struct __em_asm_sig_builder {};

template<typename... Args>
struct __em_asm_sig_builder<__em_asm_type_tuple<Args...> > {
  inline static const char buffer[sizeof...(Args) + 1] = { __em_asm_sig<Args>::value..., 0 };
};

// We move to type level with decltype(make_tuple(...)) to avoid double
// evaluation of arguments. Use __typeof__ instead of decltype, though,
// because the header should be able to compile with clang's -std=c++03.
#define _EM_ASM_PREP_ARGS(...) \
    , __em_asm_sig_builder<__typeof__(__em_asm_make_type_tuple(__VA_ARGS__))>::buffer, ##__VA_ARGS__
#endif // __cplusplus

// Note: If the code block in the EM_ASM() family of functions below contains a
// comma, then wrap the whole code block inside parentheses (). See
// test/core/test_em_asm_2.cpp for example code snippets.

#define CODE_EXPR(code) (__extension__({           \
    __attribute__((section("em_asm"), aligned(1))) \
    static const char x[] = code;                  \
    x;                                             \
  }))

// Runs the given JavaScript code on the calling thread (synchronously), and
// returns no value back.
#define EM_ASM(code, ...) ((void)emscripten_asm_const_int(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__)))

// Runs the given JavaScript code on the calling thread (synchronously), and
// returns an i32 back.
#define EM_ASM_INT(code, ...) emscripten_asm_const_int(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))

// Runs the given JavaScript code on the calling thread (synchronously), and
// returns an pointer back.
// On wasm32 this is the same as emscripten_asm_const_int but on wasm64 it
// returns an i64.
#define EM_ASM_PTR(code, ...) emscripten_asm_const_ptr(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))

// Runs the given JavaScript code on the calling thread (synchronously), and
// returns a double back.
#define EM_ASM_DOUBLE(code, ...) emscripten_asm_const_double(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))

// Runs the given JavaScript code synchronously on the main browser thread, and
// returns no value back.
// Call this function for example to access DOM elements in a pthread when
// building with -pthread.
// Avoid calling this function in performance sensitive code, because this will
// effectively sleep the calling thread until the main browser thread is able to
// service the proxied function call. If you have multiple MAIN_THREAD_EM_ASM()
// code blocks to call in succession, it will likely be much faster to coalesce
// all the calls to a single MAIN_THREAD_EM_ASM() block. If you do not need
// synchronization nor a return value back, consider using the function
// MAIN_THREAD_ASYNC_EM_ASM() instead, which will not block.
// In single-threaded builds (including proxy-to-worker), MAIN_THREAD_EM_ASM*()
// functions are direct aliases to the corresponding EM_ASM*() family of
// functions.
#define MAIN_THREAD_EM_ASM(code, ...) ((void)emscripten_asm_const_int_sync_on_main_thread(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__)))

// Runs the given JavaScript code synchronously on the main browser thread, and
// returns an integer back.
// The same considerations apply as with MAIN_THREAD_EM_ASM().
#define MAIN_THREAD_EM_ASM_INT(code, ...) emscripten_asm_const_int_sync_on_main_thread(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))

// Runs the given JavaScript code synchronously on the main browser thread, and
// returns an pointer back.
// The same considerations apply as with MAIN_THREAD_EM_ASM().
// On wasm32 this is the same as emscripten_asm_const_int but on wasm64 it
// returns an i64.
#define MAIN_THREAD_EM_ASM_PTR(code, ...) emscripten_asm_const_ptr_sync_on_main_thread(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))

// Runs the given JavaScript code synchronously on the main browser thread, and
// returns a double back.
// The same considerations apply as with MAIN_THREAD_EM_ASM().
#define MAIN_THREAD_EM_ASM_DOUBLE(code, ...) emscripten_asm_const_double_sync_on_main_thread(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))

// Asynchronously dispatches the given JavaScript code to be run on the main
// browser thread.
// If the calling thread is the main browser thread, then the specified
// JavaScript code is executed synchronously. Otherwise an event will be queued
// on the main browser thread to execute the call later (think postMessage()),
// and this call will immediately return without waiting. Be sure to guard any
// accesses to shared memory on the heap inside the JavaScript code with
// appropriate locking.
#define MAIN_THREAD_ASYNC_EM_ASM(code, ...) ((void)emscripten_asm_const_async_on_main_thread(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__)))

// Old forms for compatibility, no need to use these.
// Replace EM_ASM_, EM_ASM_ARGS and EM_ASM_INT_V with EM_ASM_INT,
// and EM_ASM_DOUBLE_V with EM_ASM_DOUBLE.
#define EM_ASM_(code, ...) emscripten_asm_const_int(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))
#define EM_ASM_ARGS(code, ...) emscripten_asm_const_int(CODE_EXPR(#code) _EM_ASM_PREP_ARGS(__VA_ARGS__))
#define EM_ASM_INT_V(code) EM_ASM_INT(code)
#define EM_ASM_DOUBLE_V(code) EM_ASM_DOUBLE(code)


// Normally macros like `true` and `false` are not expanded inside
// of `EM_JS` or `EM_ASM` blocks.  However, in the case then an
// additional macro later is added these will be expanded and we want
// to make sure the resulting expansion doesn't break the expectations
// of JS code
#if defined(true) && defined(false)
#undef true
#undef false
// These work for both C and javascript.
// In C !!0 ==> 0 and in javascript !!0 ==> false
// In C !!1 ==> 1 and in javascript !!1 ==> true
#define true (!!1)
#define false (!!0)
#endif

#endif // !defined(__cplusplus) && defined(__STRICT_ANSI__)

/*
 * Copyright 2020 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#define EMSCRIPTEN_KEEPALIVE __attribute__((used))

#ifdef __wasm__
#define EM_IMPORT(NAME) __attribute__((import_module("env"), import_name(#NAME)))
#else
#define EM_IMPORT(NAME)
#endif

#ifdef __cplusplus
#define _EM_BEGIN_CDECL extern "C" {
#define _EM_END_CDECL   }
#else // __cplusplus
#define _EM_BEGIN_CDECL
#define _EM_END_CDECL
#endif // __cplusplus

/*
 * EM_JS_DEPS: Use this macro to declare indirect dependencies on JS symbols.
 * The first argument is just a unique name for the set of dependencies.  The
 * second argument is a C string that lists JS library symbols in the same way
 * they would be specified in the DEFAULT_LIBRARY_FUNCS_TO_INCLUDE command line
 * setting.
 *
 * For example, if your code contains an EM_ASM or EM_JS block that make use of
 * the allocate and stackSave JS library functions then you might write this in
 * your library source code:
 *
 *   EM_JS_DEPS(mylib_dep, "$allocate,$stackSave");
 *
 * The emscripten linker will then pick this up and make sure those symbols get
 * included in the JS support library.
 *
 * Dependencies declared in this way will be included if-and-only-if the object
 * file (translation unit) in which they exist is included by the linker, so
 * it makes sense co-locate them with the EM_JS or EM_ASM code they correspond
 * to.
 */
#define EM_JS_DEPS(tag, deps)             \
  _EM_BEGIN_CDECL                         \
  EMSCRIPTEN_KEEPALIVE                    \
  __attribute__((section("em_lib_deps"))) \
  __attribute__((aligned(1)))             \
  char __em_lib_deps_##tag[] = deps;      \
  _EM_END_CDECL

/*
 * Copyright 2012 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

/* Typedefs */

typedef short __attribute__((aligned(1))) emscripten_align1_short;

typedef long long __attribute__((aligned(4))) emscripten_align4_int64;
typedef long long __attribute__((aligned(2))) emscripten_align2_int64;
typedef long long __attribute__((aligned(1))) emscripten_align1_int64;

typedef int __attribute__((aligned(2))) emscripten_align2_int;
typedef int __attribute__((aligned(1))) emscripten_align1_int;

typedef float __attribute__((aligned(2))) emscripten_align2_float;
typedef float __attribute__((aligned(1))) emscripten_align1_float;

typedef double __attribute__((aligned(4))) emscripten_align4_double;
typedef double __attribute__((aligned(2))) emscripten_align2_double;
typedef double __attribute__((aligned(1))) emscripten_align1_double;

typedef void (*em_callback_func)(void);
typedef void (*em_arg_callback_func)(void*);
typedef void (*em_str_callback_func)(const char *);

/* Legacy EM_BOOL type. Emscripten no longer uses this */
#define EM_BOOL bool
#define EM_TRUE true
#define EM_FALSE false

#define EM_UTF8 char

#define EMSCRIPTEN_RESULT int

#define EMSCRIPTEN_RESULT_SUCCESS              0
#define EMSCRIPTEN_RESULT_DEFERRED             1
#define EMSCRIPTEN_RESULT_NOT_SUPPORTED       -1
#define EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED -2
#define EMSCRIPTEN_RESULT_INVALID_TARGET      -3
#define EMSCRIPTEN_RESULT_UNKNOWN_TARGET      -4
#define EMSCRIPTEN_RESULT_INVALID_PARAM       -5
#define EMSCRIPTEN_RESULT_FAILED              -6
#define EMSCRIPTEN_RESULT_NO_DATA             -7
#define EMSCRIPTEN_RESULT_TIMED_OUT           -8

/*
 * Copyright 2018 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

// EM_JS declares JS functions in C code.
// Example uses can be found in test/core/test_em_js.cpp

// Implementation details:

// The EM_JS macro is specified as:
//   EM_JS(return type, function name, (arguments), {body})
// but the macro uses __VA_ARGS__ instead of a fourth argument. This is so that
// function bodies containing commas are seamlessly handled.

// EM_JS declares the JS function with a C function prototype, which becomes a
// function import in wasm. It also declares an __em_js__-prefixed string constant,
// which we can use to pass information to the Emscripten compiler that survives
// going through LLVM.
// Example:
//
//   EM_JS(int, foo, (int x, int y), { return 2 * x + y; })
//
// would get translated into:
//
//   __attribute__((import_name("foo"))) int foo(int x, int y);
//
//   __attribute__((used)) static void* __em_js_ref_foo = (void*)&foo;
//
//   __attribute__((used, visibility("default")))
//   char __em_js__foo[] = "(int x, int y)<::>{ return 2 * x + y; }";
//
// We pack the arguments and function body into a constant string so it's
// readable during wasm post-processing.
// Later we see an export called __em_js__foo, meaning we need to create a JS
// function:
//   function foo(x, y) { return 2 * x + y; }
// We use <::> to separate the arguments from the function body because it isn't
// valid anywhere in a C function declaration.

// The __em_js_ref_foo pointer simply exists in order to force a reference to
// `foo` to exist in the object file, even if there are no other local uses.
// This means the linker will always use the import_name attribute for this
// function even if it is not locally used.

// Generated __em_js__-prefixed symbols are read by binaryen, and the string
// data is extracted into the Emscripten metadata dictionary under the
// "emJsFuncs" key. emJsFuncs itself is a dictionary where the keys are function
// names (not prefixed with __em_js__), and the values are the <::>-including
// description strings.

// emJsFuncs metadata is read in emscripten.py's create_em_js, which creates an
// array of JS function strings to be included in the JS output.

#define _EM_JS(ret, c_name, js_name, params, code)                             \
  _EM_BEGIN_CDECL                                                              \
  ret c_name params EM_IMPORT(js_name);                                        \
  __attribute__((visibility("hidden")))                                        \
  void* __em_js_ref_##c_name = (void*)&c_name;                                 \
  EMSCRIPTEN_KEEPALIVE                                                         \
  __attribute__((section("em_js"), aligned(1))) char __em_js__##js_name[] =    \
    #params "<::>" code;                                                       \
  _EM_END_CDECL

#define EM_JS(ret, name, params, ...) _EM_JS(ret, name, name, params, #__VA_ARGS__)

#define EM_ASYNC_JS(ret, name, params, ...) _EM_JS(ret, name, __asyncjs__##name, params,          \
  "{ return Asyncify.handleAsync(async () => " #__VA_ARGS__ "); }")


// Normally macros like `true` and `false` are not expanded inside
// of `EM_JS` or `EM_ASM` blocks.  However, in the case when an
// additional macro later is added these will be expanded and we want
// to make sure the resulting expansion doesn't break the expectations
// of JS code
#if defined(true) && defined(false)
#undef true
#undef false
// These work for both C and javascript.
// In C !!0 ==> 0 and in javascript !!0 ==> false
// In C !!1 ==> 1 and in javascript !!1 ==> true
#define true (!!1)
#define false (!!0)
#endif

/*
 * Copyright 2023 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#ifdef __cplusplus
extern "C" {
#endif

// EXPERIMENTAL AND SUBJECT TO CHANGE!

// An opaque handle to a JS Promise object.
typedef struct _em_promise* em_promise_t;

typedef enum em_promise_result_t {
  EM_PROMISE_FULFILL,
  EM_PROMISE_MATCH,
  EM_PROMISE_MATCH_RELEASE,
  EM_PROMISE_REJECT,
} em_promise_result_t;

// A callback passed to `emscripten_promise_then` to be invoked once a promise
// is fulfilled or rejected. `data` is arbitrary user-provided data provided
// when `emscripten_promise_then` is called to install the callback and `value`
// is the value the promise was fulfilled or rejected with.
//
// The callback can signal how to resolve the new promise returned from
// `emscripten_promise_then` via its return and by writing a new result to
// outparam `result`. The behavior depends on the returned `em_promise_result_t`
// value:
//
//  - `EM_PROMISE_FULFILL`: The new promise is fulfilled with the value written
//    to `result` or NULL if no value is written.
//
//  - `EM_PROMISE_MATCH` or `EM_PROMISE_MATCH_RELEASE`: The callback must write
//    a promise handle to `result` and the new promise is resolved to match the
//    eventual state of that promise. `EM_PROMISE_MATCH_RELEASE` will also cause
//    the written promise handle to be destroyed so that the user does not have
//    to arrange for it to be destroyed after the callback is executed.
//
//  - `EM_PROMISE_REJECT`: The new promise is rejected with the reason written
//    to `result` or NULL if no reason is written.
//
// If the callback throws a number (or bigint in the case of memory64), the new
// promise will be rejected with that number converted to a pointer as its
// rejection reason. If the callback throws any other value, the new promise
// will be rejected with a NULL rejection reason.
typedef em_promise_result_t (*em_promise_callback_t)(void** result,
                                                     void* data,
                                                     void* value);

// Create a new promise that can be explicitly resolved or rejected using
// `emscripten_promise_resolve`. The returned promise handle must eventually be
// freed with `emscripten_promise_destroy`.
__attribute__((warn_unused_result)) em_promise_t
emscripten_promise_create(void);

// Release the resources associated with this promise. This must be called on
// every promise handle created, whether by `emscripten_promise_create` or any
// other function that returns a fresh promise, such as
// `emscripten_promise_then`. It is fine to call `emscripten_promise_destroy` on
// a promise handle before the promise is resolved; the configured callbacks
// will still be called.
void emscripten_promise_destroy(em_promise_t promise);

// Explicitly resolve the `promise` created by `emscripten_promise_create`. If
// `result` is `EM_PROMISE_FULFILL`, then the promise is fulfilled with the
// given `value`. If `result` is `EM_PROMISE_MATCH`, then the promise is
// resolved to match the eventual state of `value` interpreted as a promise
// handle. Finally, if `result` is `EM_PROMISE_REJECT`, then the promise is
// rejected with the given value. Promises not created by
// `emscripten_promise_create` should not be passed to this function.
void emscripten_promise_resolve(em_promise_t promise,
                                em_promise_result_t result,
                                void* value);

// Install `on_fulfilled` and `on_rejected` callbacks on the given `promise`,
// creating and returning a handle to a new promise. See `em_promise_callback_t`
// for documentation on how the callbacks work. `data` is arbitrary user data
// that will be passed to the callbacks. The returned promise handle must
// eventually be freed with `emscripten_promise_destroy`.
__attribute__((warn_unused_result)) em_promise_t
emscripten_promise_then(em_promise_t promise,
                        em_promise_callback_t on_fulfilled,
                        em_promise_callback_t on_rejected,
                        void* data);

// Call Promise.all to create and return a new promise that is either fulfilled
// once the `num_promises` input promises passed in `promises` have been
// fulfilled or is rejected once any of the input promises has been rejected.
// When the returned promise is fulfilled, the values each of the input promises
// were resolved with will be written to the `results` array if it is non-null
// and the returned promise will be fulfilled with the address of that array as
// well.
__attribute__((warn_unused_result)) em_promise_t emscripten_promise_all(
  em_promise_t* promises, void** results, size_t num_promises);

typedef struct em_settled_result_t {
  em_promise_result_t result;
  void* value;
} em_settled_result_t;

// Call Promise.allSettled to create and return a new promise that is fulfilled
// once the `num_promises` input promises passed in `promises` have been
// settled. When the returned promise is fulfilled, the `results` buffer will be
// filled with the result comprising of either EM_PROMISE_FULFILL and the
// fulfilled value or EM_PROMISE_REJECT and the rejection reason for each of the
// input promises if `results` is non-null. The returned promise will be
// fulfilled with the value of `results` as well.
__attribute__((warn_unused_result)) em_promise_t emscripten_promise_all_settled(
  em_promise_t* promises, em_settled_result_t* results, size_t num_promises);

// Call Promise.any to create and return a new promise that is fulfilled once
// any of the `num_promises` input promises passed in `promises` has been
// fulfilled or is rejected once all of the input promises have been rejected.
// If the returned promise is fulfilled, it will be fulfilled with the same
// value as the first fulfilled input promise. Otherwise, if the returned
// promise is rejected, the rejection reasons for each input promise will be
// written to the `errors` buffer if it is non-null. The rejection reason for
// the returned promise will also be the address of the `errors` buffer.
__attribute__((warn_unused_result)) em_promise_t emscripten_promise_any(
  em_promise_t* promises, void** errors, size_t num_promises);

// Call Promise.race to create and return a new promise that settles once any of
// the `num_promises` input promises passed in `promises` has been settled. If
// the first input promise to settle is fulfilled, the resulting promise is
// fulfilled with the same value. Otherwise, if the first input promise to
// settle is rejected, the resulting promise is rejected with the same reason.
__attribute__((warn_unused_result)) em_promise_t
emscripten_promise_race(em_promise_t* promises, size_t num_promises);

// Suspend the current Wasm execution context until the given promise has been
// settled.
//
// Since the stack is not unwound while Wasm execution is suspended, it is
// safe to pass pointers to the stack to asynchronous work that is waited on
// with this function.
//
// This function can only be used in programs that were built with `-sASYNCIFY`.
__attribute__((warn_unused_result)) em_settled_result_t
emscripten_promise_await(em_promise_t promise);

#ifdef __cplusplus
}
#endif

/*
 * Copyright 2012 The Emscripten Authors.  All rights reserved.
 * Emscripten is available under two separate licenses, the MIT license and the
 * University of Illinois/NCSA Open Source License.  Both these licenses can be
 * found in the LICENSE file.
 */

#ifdef __cplusplus
extern "C" {
#endif

// wget

void emscripten_async_wget(const char* url, const char* file, em_str_callback_func onload, em_str_callback_func onerror);

typedef void (*em_async_wget_onload_func)(void* userdata, void* data, int size);
void emscripten_async_wget_data(const char* url, void *userdata, em_async_wget_onload_func onload, em_arg_callback_func onerror);

typedef void (*em_async_wget2_onload_func)(unsigned handle, void* userdata, const char* data);
typedef void (*em_async_wget2_onstatus_func)(unsigned handle, void* userdata, int status);

int emscripten_async_wget2(const char* url, const char* file,  const char* requesttype, const char* param, void *userdata, em_async_wget2_onload_func onload, em_async_wget2_onstatus_func onerror, em_async_wget2_onstatus_func onprogress);

typedef void (*em_async_wget2_data_onload_func)(unsigned handle, void* userdata, void* data, unsigned size);
typedef void (*em_async_wget2_data_onerror_func)(unsigned handle, void* userdata, int status, const char* status_text);
typedef void (*em_async_wget2_data_onprogress_func)(unsigned handle, void* userdata, int loaded, int total);

int emscripten_async_wget2_data(const char* url, const char* requesttype, const char* param, void *arg, int free, em_async_wget2_data_onload_func onload, em_async_wget2_data_onerror_func onerror, em_async_wget2_data_onprogress_func onprogress);

void emscripten_async_wget2_abort(int handle);

// wget "sync"

int emscripten_wget(const char* url, const char* file);

void emscripten_wget_data(const char* url, void** pbuffer, int* pnum, int *perror);

#ifdef __cplusplus
}
#endif

/* Automatically generated by tools/system_libs.py */
#define __EMSCRIPTEN_MAJOR__ 5
#define __EMSCRIPTEN_MINOR__ 0
#define __EMSCRIPTEN_TINY__ 2

// Legacy mixed-case macros:
#define __EMSCRIPTEN_major__ __EMSCRIPTEN_MAJOR__
#define __EMSCRIPTEN_minor__ __EMSCRIPTEN_MINOR__
#define __EMSCRIPTEN_tiny__ __EMSCRIPTEN_TINY__
#pragma clang deprecated(__EMSCRIPTEN_major__, "Use __EMSCRIPTEN_MAJOR__ instead")
#pragma clang deprecated(__EMSCRIPTEN_minor__, "Use __EMSCRIPTEN_MINOR__ instead")
#pragma clang deprecated(__EMSCRIPTEN_tiny__, "Use __EMSCRIPTEN_TINY__ instead")

#ifdef __cplusplus
extern "C" {
#endif

void emscripten_run_script(const char *script __attribute__((nonnull)));
int emscripten_run_script_int(const char *script __attribute__((nonnull)));
char *emscripten_run_script_string(const char *script __attribute__((nonnull)));
void emscripten_async_run_script(const char *script __attribute__((nonnull)), int millis);
void emscripten_async_load_script(const char *script __attribute__((nonnull)), em_callback_func onload, em_callback_func onerror);

void emscripten_set_main_loop(em_callback_func func, int fps, bool simulate_infinite_loop);

#define EM_TIMING_SETTIMEOUT 0
#define EM_TIMING_RAF 1
#define EM_TIMING_SETIMMEDIATE 2

int emscripten_set_main_loop_timing(int mode, int value);
void emscripten_get_main_loop_timing(int *mode, int *value); // Pass a null pointer to skip receiving that particular value
void emscripten_set_main_loop_arg(em_arg_callback_func func, void *arg, int fps, bool simulate_infinite_loop);
void emscripten_pause_main_loop(void);
void emscripten_resume_main_loop(void);
void emscripten_cancel_main_loop(void);

typedef void (*em_socket_callback)(int fd, void *userData);
typedef void (*em_socket_error_callback)(int fd, int err, const char* msg, void *userData);

void emscripten_set_socket_error_callback(void *userData, em_socket_error_callback callback);
void emscripten_set_socket_open_callback(void *userData, em_socket_callback callback);
void emscripten_set_socket_listen_callback(void *userData, em_socket_callback callback);
void emscripten_set_socket_connection_callback(void *userData, em_socket_callback callback);
void emscripten_set_socket_message_callback(void *userData, em_socket_callback callback);
void emscripten_set_socket_close_callback(void *userData, em_socket_callback callback);

void _emscripten_push_main_loop_blocker(em_arg_callback_func func, void *arg, const char *name);
void _emscripten_push_uncounted_main_loop_blocker(em_arg_callback_func func, void *arg, const char *name);
#define emscripten_push_main_loop_blocker(func, arg) \
  _emscripten_push_main_loop_blocker(func, arg, #func)
#define emscripten_push_uncounted_main_loop_blocker(func, arg) \
  _emscripten_push_uncounted_main_loop_blocker(func, arg, #func)

void emscripten_set_main_loop_expected_blockers(int num);

void emscripten_async_call(em_arg_callback_func func, void *arg, int millis);

void emscripten_exit_with_live_runtime(void) __attribute__((__noreturn__));
void emscripten_force_exit(int status) __attribute__((__noreturn__));

double emscripten_get_device_pixel_ratio(void);

char *emscripten_get_window_title(void);
void emscripten_set_window_title(const char *);
void emscripten_get_screen_size(int *width __attribute__((nonnull)), int *height __attribute__((nonnull)));
void emscripten_hide_mouse(void);
void emscripten_set_canvas_size(int width, int height) __attribute__((deprecated("This variant does not allow specifying the target canvas", "Use emscripten_set_canvas_element_size() instead")));
void emscripten_get_canvas_size(int *width __attribute__((nonnull)), int *height __attribute__((nonnull)), int *isFullscreen __attribute__((nonnull))) __attribute__((deprecated("This variant does not allow specifying the target canvas", "Use emscripten_get_canvas_element_size() and emscripten_get_fullscreen_status() instead")));

double emscripten_get_now(void);
float emscripten_random(void);

// IDB

typedef void (*em_idb_onload_func)(void*, void*, int);
void emscripten_idb_async_load(const char *db_name __attribute__((nonnull)), const char *file_id __attribute__((nonnull)), void* arg, em_idb_onload_func onload, em_arg_callback_func onerror);
void emscripten_idb_async_store(const char *db_name __attribute__((nonnull)), const char *file_id __attribute__((nonnull)), void* ptr, int num, void* arg, em_arg_callback_func onstore, em_arg_callback_func onerror);
void emscripten_idb_async_delete(const char *db_name __attribute__((nonnull)), const char *file_id __attribute__((nonnull)), void* arg, em_arg_callback_func ondelete, em_arg_callback_func onerror);
typedef void (*em_idb_exists_func)(void*, int);
void emscripten_idb_async_exists(const char *db_name __attribute__((nonnull)), const char *file_id __attribute__((nonnull)), void* arg, em_idb_exists_func oncheck, em_arg_callback_func onerror);
void emscripten_idb_async_clear(const char *db_name __attribute__((nonnull)), void* arg, em_arg_callback_func onclear, em_arg_callback_func onerror);

// IDB "sync"

void emscripten_idb_load(const char *db_name, const char *file_id, void** pbuffer, int* pnum, int *perror);
void emscripten_idb_store(const char *db_name, const char *file_id, void* buffer, int num, int *perror);
void emscripten_idb_delete(const char *db_name, const char *file_id, int *perror);
void emscripten_idb_exists(const char *db_name, const char *file_id, int* pexists, int *perror);
void emscripten_idb_clear(const char *db_name, int *perror);

// other async utilities

int emscripten_run_preload_plugins(const char* file, em_str_callback_func onload, em_str_callback_func onerror);

typedef void (*em_run_preload_plugins_data_onload_func)(void*, const char*);
void emscripten_run_preload_plugins_data(char* data, int size, const char *suffix, void *arg, em_run_preload_plugins_data_onload_func onload, em_arg_callback_func onerror);

// show an error on some renamed methods
#define emscripten_async_prepare(...) _Pragma("GCC error(\"emscripten_async_prepare has been replaced by emscripten_run_preload_plugins\")")
#define emscripten_async_prepare_data(...) _Pragma("GCC error(\"emscripten_async_prepare_data has been replaced by emscripten_run_preload_plugins_data\")")

// worker APIs

typedef int worker_handle;

worker_handle emscripten_create_worker(const char *url);
void emscripten_destroy_worker(worker_handle worker);

typedef void (*em_worker_callback_func)(char*, int, void*);
void emscripten_call_worker(worker_handle worker, const char *funcname, char *data, int size, em_worker_callback_func callback, void *arg);
void emscripten_worker_respond(char *data, int size);
void emscripten_worker_respond_provisionally(char *data, int size);

int emscripten_get_worker_queue_size(worker_handle worker);

// misc.

long emscripten_get_compiler_setting(const char *name);

// Returns the value of -sASYNCIFY.  Can be 0, 1, or 2 (in the case of JSPI).
int emscripten_has_asyncify(void);

void emscripten_debugger(void);

// Forward declare FILE from musl libc headers to avoid needing to #include <stdio.h> from emscripten.h
struct _IO_FILE;
typedef struct _IO_FILE FILE;

char *emscripten_get_preloaded_image_data(const char *path, int *w, int *h);
char *emscripten_get_preloaded_image_data_from_FILE(FILE *file, int *w, int *h);

#define EM_LOG_CONSOLE   1
#define EM_LOG_WARN      2
#define EM_LOG_ERROR     4
#define EM_LOG_C_STACK   8
#define EM_LOG_JS_STACK 16
#define EM_LOG_DEMANGLE 32  // deprecated
#pragma clang deprecated(EM_LOG_DEMANGLE)
#define EM_LOG_NO_PATHS 64
#define EM_LOG_FUNC_PARAMS 128  // deprecated
#pragma clang deprecated(EM_LOG_FUNC_PARAMS)
#define EM_LOG_DEBUG    256
#define EM_LOG_INFO     512

void emscripten_log(int flags, const char* format, ...);

int emscripten_get_callstack(int flags, char *out, int maxbytes);

int emscripten_print_double(double x, char *to, signed max);

typedef void (*em_scan_func)(void*, void*);
void emscripten_scan_registers(em_scan_func func);
void emscripten_scan_stack(em_scan_func func);

// Asynchronous version of dlopen.  Since WebAssembly module loading in general
// is asynchronous the normal dlopen function can't be used in all situations.
typedef void (*em_dlopen_callback)(void* user_data, void* handle);
void emscripten_dlopen(const char *filename, int flags, void* user_data, em_dlopen_callback onsuccess, em_arg_callback_func onerror);

// Promisified version of emscripten_dlopen
// The returned promise will resolve once the dso has been loaded.  It's up to
// the caller to call emscripten_promise_destroy on this promise.
em_promise_t emscripten_dlopen_promise(const char *filename, int flags);

void emscripten_throw_number(double number);
void emscripten_throw_string(const char *utf8String);

/* ===================================== */
/* Internal APIs. Be careful with these. */
/* ===================================== */

void emscripten_sleep(unsigned int ms);

#ifdef __cplusplus
}
#endif
#endif /* EMSCRIPTEN_H */
#endif /* OS_BROWSER */
