/* 
05.09.2026
Stitched together all relevant glfw files in order to remove glfw static library from my project
*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

#ifdef OS_LINUX
#include <X11/Xresource.h>
#include <X11/cursorfont.h>
#include <X11/Xmd.h>
#include <locale.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <dirent.h>
#define _GLFW_X11
#endif /* OS_LINUX */

#include "glfw.h"

//========================================================================
// GLFW 3.3 - www.glfw.org - context.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2016 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Checks whether the desired context attributes are valid
//
// This function checks things like whether the specified client API version
// exists and whether all relevant options have supported and non-conflicting
// values
//
GLFWbool _glfwIsValidContextConfig(const _GLFWctxconfig* ctxconfig)
{
    if (ctxconfig->share)
    {
        if (ctxconfig->client == GLFW_NO_API ||
            ctxconfig->share->context.client == GLFW_NO_API)
        {
            _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
            return GLFW_FALSE;
        }
    }

    if (ctxconfig->source != GLFW_NATIVE_CONTEXT_API &&
        ctxconfig->source != GLFW_EGL_CONTEXT_API &&
        ctxconfig->source != GLFW_OSMESA_CONTEXT_API)
    {
        _glfwInputError(GLFW_INVALID_ENUM,
                        "Invalid context creation API 0x%08X",
                        ctxconfig->source);
        return GLFW_FALSE;
    }

    if (ctxconfig->client != GLFW_NO_API &&
        ctxconfig->client != GLFW_OPENGL_API &&
        ctxconfig->client != GLFW_OPENGL_ES_API)
    {
        _glfwInputError(GLFW_INVALID_ENUM,
                        "Invalid client API 0x%08X",
                        ctxconfig->client);
        return GLFW_FALSE;
    }

    if (ctxconfig->client == GLFW_OPENGL_API)
    {
        if ((ctxconfig->major < 1 || ctxconfig->minor < 0) ||
            (ctxconfig->major == 1 && ctxconfig->minor > 5) ||
            (ctxconfig->major == 2 && ctxconfig->minor > 1) ||
            (ctxconfig->major == 3 && ctxconfig->minor > 3))
        {
            // OpenGL 1.0 is the smallest valid version
            // OpenGL 1.x series ended with version 1.5
            // OpenGL 2.x series ended with version 2.1
            // OpenGL 3.x series ended with version 3.3
            // For now, let everything else through

            _glfwInputError(GLFW_INVALID_VALUE,
                            "Invalid OpenGL version %i.%i",
                            ctxconfig->major, ctxconfig->minor);
            return GLFW_FALSE;
        }

        if (ctxconfig->profile)
        {
            if (ctxconfig->profile != GLFW_OPENGL_CORE_PROFILE &&
                ctxconfig->profile != GLFW_OPENGL_COMPAT_PROFILE)
            {
                _glfwInputError(GLFW_INVALID_ENUM,
                                "Invalid OpenGL profile 0x%08X",
                                ctxconfig->profile);
                return GLFW_FALSE;
            }

            if (ctxconfig->major <= 2 ||
                (ctxconfig->major == 3 && ctxconfig->minor < 2))
            {
                // Desktop OpenGL context profiles are only defined for version 3.2
                // and above

                _glfwInputError(GLFW_INVALID_VALUE,
                                "Context profiles are only defined for OpenGL version 3.2 and above");
                return GLFW_FALSE;
            }
        }

        if (ctxconfig->forward && ctxconfig->major <= 2)
        {
            // Forward-compatible contexts are only defined for OpenGL version 3.0 and above
            _glfwInputError(GLFW_INVALID_VALUE,
                            "Forward-compatibility is only defined for OpenGL version 3.0 and above");
            return GLFW_FALSE;
        }
    }
    else if (ctxconfig->client == GLFW_OPENGL_ES_API)
    {
        if (ctxconfig->major < 1 || ctxconfig->minor < 0 ||
            (ctxconfig->major == 1 && ctxconfig->minor > 1) ||
            (ctxconfig->major == 2 && ctxconfig->minor > 0))
        {
            // OpenGL ES 1.0 is the smallest valid version
            // OpenGL ES 1.x series ended with version 1.1
            // OpenGL ES 2.x series ended with version 2.0
            // For now, let everything else through

            _glfwInputError(GLFW_INVALID_VALUE,
                            "Invalid OpenGL ES version %i.%i",
                            ctxconfig->major, ctxconfig->minor);
            return GLFW_FALSE;
        }
    }

    if (ctxconfig->robustness)
    {
        if (ctxconfig->robustness != GLFW_NO_RESET_NOTIFICATION &&
            ctxconfig->robustness != GLFW_LOSE_CONTEXT_ON_RESET)
        {
            _glfwInputError(GLFW_INVALID_ENUM,
                            "Invalid context robustness mode 0x%08X",
                            ctxconfig->robustness);
            return GLFW_FALSE;
        }
    }

    if (ctxconfig->release)
    {
        if (ctxconfig->release != GLFW_RELEASE_BEHAVIOR_NONE &&
            ctxconfig->release != GLFW_RELEASE_BEHAVIOR_FLUSH)
        {
            _glfwInputError(GLFW_INVALID_ENUM,
                            "Invalid context release behavior 0x%08X",
                            ctxconfig->release);
            return GLFW_FALSE;
        }
    }

    return GLFW_TRUE;
}

// Chooses the framebuffer config that best matches the desired one
//
const _GLFWfbconfig* _glfwChooseFBConfig(const _GLFWfbconfig* desired,
                                         const _GLFWfbconfig* alternatives,
                                         unsigned int count)
{
    unsigned int i;
    unsigned int missing, leastMissing = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    unsigned int extraDiff, leastExtraDiff = UINT_MAX;
    const _GLFWfbconfig* current;
    const _GLFWfbconfig* closest = NULL;

    for (i = 0;  i < count;  i++)
    {
        current = alternatives + i;

        if (desired->stereo > 0 && current->stereo == 0)
        {
            // Stereo is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->alphaBits > 0 && current->alphaBits == 0)
                missing++;

            if (desired->depthBits > 0 && current->depthBits == 0)
                missing++;

            if (desired->stencilBits > 0 && current->stencilBits == 0)
                missing++;

            if (desired->auxBuffers > 0 &&
                current->auxBuffers < desired->auxBuffers)
            {
                missing += desired->auxBuffers - current->auxBuffers;
            }

            if (desired->samples > 0 && current->samples == 0)
            {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }

            if (desired->transparent != current->transparent)
                missing++;
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->redBits != GLFW_DONT_CARE)
            {
                colorDiff += (desired->redBits - current->redBits) *
                             (desired->redBits - current->redBits);
            }

            if (desired->greenBits != GLFW_DONT_CARE)
            {
                colorDiff += (desired->greenBits - current->greenBits) *
                             (desired->greenBits - current->greenBits);
            }

            if (desired->blueBits != GLFW_DONT_CARE)
            {
                colorDiff += (desired->blueBits - current->blueBits) *
                             (desired->blueBits - current->blueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->alphaBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->alphaBits - current->alphaBits) *
                             (desired->alphaBits - current->alphaBits);
            }

            if (desired->depthBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->depthBits - current->depthBits) *
                             (desired->depthBits - current->depthBits);
            }

            if (desired->stencilBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->stencilBits - current->stencilBits) *
                             (desired->stencilBits - current->stencilBits);
            }

            if (desired->accumRedBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->accumRedBits - current->accumRedBits) *
                             (desired->accumRedBits - current->accumRedBits);
            }

            if (desired->accumGreenBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->accumGreenBits - current->accumGreenBits) *
                             (desired->accumGreenBits - current->accumGreenBits);
            }

            if (desired->accumBlueBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->accumBlueBits - current->accumBlueBits) *
                             (desired->accumBlueBits - current->accumBlueBits);
            }

            if (desired->accumAlphaBits != GLFW_DONT_CARE)
            {
                extraDiff += (desired->accumAlphaBits - current->accumAlphaBits) *
                             (desired->accumAlphaBits - current->accumAlphaBits);
            }

            if (desired->samples != GLFW_DONT_CARE)
            {
                extraDiff += (desired->samples - current->samples) *
                             (desired->samples - current->samples);
            }

            if (desired->sRGB && !current->sRGB)
                extraDiff++;
        }

        // Figure out if the current one is better than the best one found so far
        // Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing)
        {
            if ((colorDiff < leastColorDiff) ||
                (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
            {
                closest = current;
            }
        }

        if (current == closest)
        {
            leastMissing = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}

// Retrieves the attributes of the current context
//
GLFWbool _glfwRefreshContextAttribs(_GLFWwindow* window,
                                    const _GLFWctxconfig* ctxconfig)
{
    int i;
    _GLFWwindow* previous;
    const char* version;
    const char* prefixes[] =
    {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES ",
        NULL
    };

    window->context.source = ctxconfig->source;
    window->context.client = GLFW_OPENGL_API;

    previous = _glfwPlatformGetTls(&_glfw.contextSlot);
    glfwMakeContextCurrent((GLFWwindow*) window);

    window->context.GetIntegerv = (PFNGLGETINTEGERVPROC)
        window->context.getProcAddress("glGetIntegerv");
    window->context.GetString = (PFNGLGETSTRINGPROC)
        window->context.getProcAddress("glGetString");
    if (!window->context.GetIntegerv || !window->context.GetString)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR, "Entry point retrieval is broken");
        glfwMakeContextCurrent((GLFWwindow*) previous);
        return GLFW_FALSE;
    }

    version = (const char*) window->context.GetString(GL_VERSION);
    if (!version)
    {
        if (ctxconfig->client == GLFW_OPENGL_API)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "OpenGL version string retrieval is broken");
        }
        else
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "OpenGL ES version string retrieval is broken");
        }

        glfwMakeContextCurrent((GLFWwindow*) previous);
        return GLFW_FALSE;
    }

    for (i = 0;  prefixes[i];  i++)
    {
        const size_t length = strlen(prefixes[i]);

        if (strncmp(version, prefixes[i], length) == 0)
        {
            version += length;
            window->context.client = GLFW_OPENGL_ES_API;
            break;
        }
    }

    if (!sscanf(version, "%d.%d.%d",
                &window->context.major,
                &window->context.minor,
                &window->context.revision))
    {
        if (window->context.client == GLFW_OPENGL_API)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "No version found in OpenGL version string");
        }
        else
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "No version found in OpenGL ES version string");
        }

        glfwMakeContextCurrent((GLFWwindow*) previous);
        return GLFW_FALSE;
    }

    if (window->context.major < ctxconfig->major ||
        (window->context.major == ctxconfig->major &&
         window->context.minor < ctxconfig->minor))
    {
        // The desired OpenGL version is greater than the actual version
        // This only happens if the machine lacks {GLX|WGL}_ARB_create_context
        // /and/ the user has requested an OpenGL version greater than 1.0

        // For API consistency, we emulate the behavior of the
        // {GLX|WGL}_ARB_create_context extension and fail here

        if (window->context.client == GLFW_OPENGL_API)
        {
            _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                            "Requested OpenGL version %i.%i, got version %i.%i",
                            ctxconfig->major, ctxconfig->minor,
                            window->context.major, window->context.minor);
        }
        else
        {
            _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                            "Requested OpenGL ES version %i.%i, got version %i.%i",
                            ctxconfig->major, ctxconfig->minor,
                            window->context.major, window->context.minor);
        }

        glfwMakeContextCurrent((GLFWwindow*) previous);
        return GLFW_FALSE;
    }

    if (window->context.major >= 3)
    {
        // OpenGL 3.0+ uses a different function for extension string retrieval
        // We cache it here instead of in glfwExtensionSupported mostly to alert
        // users as early as possible that their build may be broken

        window->context.GetStringi = (PFNGLGETSTRINGIPROC)
            window->context.getProcAddress("glGetStringi");
        if (!window->context.GetStringi)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "Entry point retrieval is broken");
            glfwMakeContextCurrent((GLFWwindow*) previous);
            return GLFW_FALSE;
        }
    }

    if (window->context.client == GLFW_OPENGL_API)
    {
        // Read back context flags (OpenGL 3.0 and above)
        if (window->context.major >= 3)
        {
            GLint flags;
            window->context.GetIntegerv(GL_CONTEXT_FLAGS, &flags);

            if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
                window->context.forward = GLFW_TRUE;

            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
                window->context.debug = GLFW_TRUE;
            else if (glfwExtensionSupported("GL_ARB_debug_output") &&
                     ctxconfig->debug)
            {
                // HACK: This is a workaround for older drivers (pre KHR_debug)
                //       not setting the debug bit in the context flags for
                //       debug contexts
                window->context.debug = GLFW_TRUE;
            }

            if (flags & GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR)
                window->context.noerror = GLFW_TRUE;
        }

        // Read back OpenGL context profile (OpenGL 3.2 and above)
        if (window->context.major >= 4 ||
            (window->context.major == 3 && window->context.minor >= 2))
        {
            GLint mask;
            window->context.GetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);

            if (mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
                window->context.profile = GLFW_OPENGL_COMPAT_PROFILE;
            else if (mask & GL_CONTEXT_CORE_PROFILE_BIT)
                window->context.profile = GLFW_OPENGL_CORE_PROFILE;
            else if (glfwExtensionSupported("GL_ARB_compatibility"))
            {
                // HACK: This is a workaround for the compatibility profile bit
                //       not being set in the context flags if an OpenGL 3.2+
                //       context was created without having requested a specific
                //       version
                window->context.profile = GLFW_OPENGL_COMPAT_PROFILE;
            }
        }

        // Read back robustness strategy
        if (glfwExtensionSupported("GL_ARB_robustness"))
        {
            // NOTE: We avoid using the context flags for detection, as they are
            //       only present from 3.0 while the extension applies from 1.1

            GLint strategy;
            window->context.GetIntegerv(GL_RESET_NOTIFICATION_STRATEGY_ARB,
                                        &strategy);

            if (strategy == GL_LOSE_CONTEXT_ON_RESET_ARB)
                window->context.robustness = GLFW_LOSE_CONTEXT_ON_RESET;
            else if (strategy == GL_NO_RESET_NOTIFICATION_ARB)
                window->context.robustness = GLFW_NO_RESET_NOTIFICATION;
        }
    }
    else
    {
        // Read back robustness strategy
        if (glfwExtensionSupported("GL_EXT_robustness"))
        {
            // NOTE: The values of these constants match those of the OpenGL ARB
            //       one, so we can reuse them here

            GLint strategy;
            window->context.GetIntegerv(GL_RESET_NOTIFICATION_STRATEGY_ARB,
                                        &strategy);

            if (strategy == GL_LOSE_CONTEXT_ON_RESET_ARB)
                window->context.robustness = GLFW_LOSE_CONTEXT_ON_RESET;
            else if (strategy == GL_NO_RESET_NOTIFICATION_ARB)
                window->context.robustness = GLFW_NO_RESET_NOTIFICATION;
        }
    }

    if (glfwExtensionSupported("GL_KHR_context_flush_control"))
    {
        GLint behavior;
        window->context.GetIntegerv(GL_CONTEXT_RELEASE_BEHAVIOR, &behavior);

        if (behavior == GL_NONE)
            window->context.release = GLFW_RELEASE_BEHAVIOR_NONE;
        else if (behavior == GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH)
            window->context.release = GLFW_RELEASE_BEHAVIOR_FLUSH;
    }

    // Clearing the front buffer to black to avoid garbage pixels left over from
    // previous uses of our bit of VRAM
    {
        PFNGLCLEARPROC glClear = (PFNGLCLEARPROC)
            window->context.getProcAddress("glClear");
        glClear(GL_COLOR_BUFFER_BIT);

        if (window->doublebuffer)
            window->context.swapBuffers(window);
    }

    glfwMakeContextCurrent((GLFWwindow*) previous);
    return GLFW_TRUE;
}

// Searches an extension string for the specified extension
//
GLFWbool _glfwStringInExtensionString(const char* string, const char* extensions)
{
    const char* start = extensions;

    for (;;)
    {
        const char* where;
        const char* terminator;

        where = strstr(start, string);
        if (!where)
            return GLFW_FALSE;

        terminator = where + strlen(string);
        if (where == start || *(where - 1) == ' ')
        {
            if (*terminator == ' ' || *terminator == '\0')
                break;
        }

        start = terminator;
    }

    return GLFW_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI void glfwMakeContextCurrent(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFWwindow* previous;

    _GLFW_REQUIRE_INIT();

    previous = _glfwPlatformGetTls(&_glfw.contextSlot);

    if (window && window->context.client == GLFW_NO_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT,
                        "Cannot make current with a window that has no OpenGL or OpenGL ES context");
        return;
    }

    if (previous)
    {
        if (!window || window->context.source != previous->context.source)
            previous->context.makeCurrent(NULL);
    }

    if (window)
        window->context.makeCurrent(window);
}

GLFWAPI GLFWwindow* glfwGetCurrentContext(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return _glfwPlatformGetTls(&_glfw.contextSlot);
}

GLFWAPI void glfwSwapBuffers(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (window->context.client == GLFW_NO_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT,
                        "Cannot swap buffers of a window that has no OpenGL or OpenGL ES context");
        return;
    }

    window->context.swapBuffers(window);
}

GLFWAPI void glfwSwapInterval(int interval)
{
    _GLFWwindow* window;

    _GLFW_REQUIRE_INIT();

    window = _glfwPlatformGetTls(&_glfw.contextSlot);
    if (!window)
    {
        _glfwInputError(GLFW_NO_CURRENT_CONTEXT,
                        "Cannot set swap interval without a current OpenGL or OpenGL ES context");
        return;
    }

    window->context.swapInterval(interval);
}

GLFWAPI int glfwExtensionSupported(const char* extension)
{
    _GLFWwindow* window;
    assert(extension != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    window = _glfwPlatformGetTls(&_glfw.contextSlot);
    if (!window)
    {
        _glfwInputError(GLFW_NO_CURRENT_CONTEXT,
                        "Cannot query extension without a current OpenGL or OpenGL ES context");
        return GLFW_FALSE;
    }

    if (*extension == '\0')
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Extension name cannot be an empty string");
        return GLFW_FALSE;
    }

    if (window->context.major >= 3)
    {
        int i;
        GLint count;

        // Check if extension is in the modern OpenGL extensions string list

        window->context.GetIntegerv(GL_NUM_EXTENSIONS, &count);

        for (i = 0;  i < count;  i++)
        {
            const char* en = (const char*)
                window->context.GetStringi(GL_EXTENSIONS, i);
            if (!en)
            {
                _glfwInputError(GLFW_PLATFORM_ERROR,
                                "Extension string retrieval is broken");
                return GLFW_FALSE;
            }

            if (strcmp(en, extension) == 0)
                return GLFW_TRUE;
        }
    }
    else
    {
        // Check if extension is in the old style OpenGL extensions string

        const char* extensions = (const char*)
            window->context.GetString(GL_EXTENSIONS);
        if (!extensions)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "Extension string retrieval is broken");
            return GLFW_FALSE;
        }

        if (_glfwStringInExtensionString(extension, extensions))
            return GLFW_TRUE;
    }

    // Check if extension is in the platform-specific string
    return window->context.extensionSupported(extension);
}

GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname)
{
    _GLFWwindow* window;
    assert(procname != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    window = _glfwPlatformGetTls(&_glfw.contextSlot);
    if (!window)
    {
        _glfwInputError(GLFW_NO_CURRENT_CONTEXT,
                        "Cannot query entry point without a current OpenGL or OpenGL ES context");
        return NULL;
    }

    return window->context.getProcAddress(procname);
}

//========================================================================
// GLFW 3.3 - www.glfw.org - init.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2018 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================


// NOTE: The global variables below comprise all mutable global data in GLFW
//       Any other mutable global variable is a bug

// This contains all mutable state shared between compilation units of GLFW
//
_GLFWlibrary _glfw = { GLFW_FALSE };

// These are outside of _glfw so they can be used before initialization and
// after termination without special handling when _glfw is cleared to zero
//
static _GLFWerror _glfwMainThreadError;
static GLFWerrorfun _glfwErrorCallback;
static _GLFWinitconfig _glfwInitHints =
{
    GLFW_TRUE,      // hat buttons
    {
        GLFW_TRUE,  // macOS menu bar
        GLFW_TRUE   // macOS bundle chdir
    }
};

// Terminate the library
//
static void terminate(void)
{
    int i;

    memset(&_glfw.callbacks, 0, sizeof(_glfw.callbacks));

    while (_glfw.windowListHead)
        glfwDestroyWindow((GLFWwindow*) _glfw.windowListHead);

    while (_glfw.cursorListHead)
        glfwDestroyCursor((GLFWcursor*) _glfw.cursorListHead);

    for (i = 0;  i < _glfw.monitorCount;  i++)
    {
        _GLFWmonitor* monitor = _glfw.monitors[i];
        if (monitor->originalRamp.size)
            _glfwPlatformSetGammaRamp(monitor, &monitor->originalRamp);
        _glfwFreeMonitor(monitor);
    }

    free(_glfw.monitors);
    _glfw.monitors = NULL;
    _glfw.monitorCount = 0;

    free(_glfw.mappings);
    _glfw.mappings = NULL;
    _glfw.mappingCount = 0;

    _glfwTerminateVulkan();
    _glfwPlatformTerminate();

    _glfw.initialized = GLFW_FALSE;

    while (_glfw.errorListHead)
    {
        _GLFWerror* error = _glfw.errorListHead;
        _glfw.errorListHead = error->next;
        free(error);
    }

    _glfwPlatformDestroyTls(&_glfw.contextSlot);
    _glfwPlatformDestroyTls(&_glfw.errorSlot);
    _glfwPlatformDestroyMutex(&_glfw.errorLock);

    memset(&_glfw, 0, sizeof(_glfw));
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Encode a Unicode code point to a UTF-8 stream
// Based on cutef8 by Jeff Bezanson (Public Domain)
//
size_t _glfwEncodeUTF8(char* s, uint32_t codepoint)
{
    size_t count = 0;

    if (codepoint < 0x80)
        s[count++] = (char) codepoint;
    else if (codepoint < 0x800)
    {
        s[count++] = (codepoint >> 6) | 0xc0;
        s[count++] = (codepoint & 0x3f) | 0x80;
    }
    else if (codepoint < 0x10000)
    {
        s[count++] = (codepoint >> 12) | 0xe0;
        s[count++] = ((codepoint >> 6) & 0x3f) | 0x80;
        s[count++] = (codepoint & 0x3f) | 0x80;
    }
    else if (codepoint < 0x110000)
    {
        s[count++] = (codepoint >> 18) | 0xf0;
        s[count++] = ((codepoint >> 12) & 0x3f) | 0x80;
        s[count++] = ((codepoint >> 6) & 0x3f) | 0x80;
        s[count++] = (codepoint & 0x3f) | 0x80;
    }

    return count;
}

// Splits and translates a text/uri-list into separate file paths
// NOTE: This function destroys the provided string
//
char** _glfwParseUriList(char* text, int* count)
{
    const char* prefix = "file://";
    char** paths = NULL;
    char* line;

    *count = 0;

    while ((line = strtok(text, "\r\n")))
    {
        char* path;

        text = NULL;

        if (line[0] == '#')
            continue;

        if (strncmp(line, prefix, strlen(prefix)) == 0)
        {
            line += strlen(prefix);
            // TODO: Validate hostname
            while (*line != '/')
                line++;
        }

        (*count)++;

        path = calloc(strlen(line) + 1, 1);
        paths = realloc(paths, *count * sizeof(char*));
        paths[*count - 1] = path;

        while (*line)
        {
            if (line[0] == '%' && line[1] && line[2])
            {
                const char digits[3] = { line[1], line[2], '\0' };
                *path = (char) strtol(digits, NULL, 16);
                line += 2;
            }
            else
                *path = *line;

            path++;
            line++;
        }
    }

    return paths;
}

char* _glfw_strdup(const char* source)
{
    const size_t length = strlen(source);
    char* result = calloc(length + 1, 1);
    strcpy(result, source);
    return result;
}

int _glfw_min(int a, int b)
{
    return a < b ? a : b;
}

int _glfw_max(int a, int b)
{
    return a > b ? a : b;
}

float _glfw_fminf(float a, float b)
{
    if (a != a)
        return b;
    else if (b != b)
        return a;
    else if (a < b)
        return a;
    else
        return b;
}

float _glfw_fmaxf(float a, float b)
{
    if (a != a)
        return b;
    else if (b != b)
        return a;
    else if (a > b)
        return a;
    else
        return b;
}


//////////////////////////////////////////////////////////////////////////
//////                         GLFW event API                       //////
//////////////////////////////////////////////////////////////////////////

// Notifies shared code of an error
//
void _glfwInputError(int code, const char* format, ...)
{
    _GLFWerror* error;
    char description[_GLFW_MESSAGE_SIZE];

    if (format)
    {
        va_list vl;

        va_start(vl, format);
        vsnprintf(description, sizeof(description), format, vl);
        va_end(vl);

        description[sizeof(description) - 1] = '\0';
    }
    else
    {
        if (code == GLFW_NOT_INITIALIZED)
            strcpy(description, "The GLFW library is not initialized");
        else if (code == GLFW_NO_CURRENT_CONTEXT)
            strcpy(description, "There is no current context");
        else if (code == GLFW_INVALID_ENUM)
            strcpy(description, "Invalid argument for enum parameter");
        else if (code == GLFW_INVALID_VALUE)
            strcpy(description, "Invalid value for parameter");
        else if (code == GLFW_OUT_OF_MEMORY)
            strcpy(description, "Out of memory");
        else if (code == GLFW_API_UNAVAILABLE)
            strcpy(description, "The requested API is unavailable");
        else if (code == GLFW_VERSION_UNAVAILABLE)
            strcpy(description, "The requested API version is unavailable");
        else if (code == GLFW_PLATFORM_ERROR)
            strcpy(description, "A platform-specific error occurred");
        else if (code == GLFW_FORMAT_UNAVAILABLE)
            strcpy(description, "The requested format is unavailable");
        else if (code == GLFW_NO_WINDOW_CONTEXT)
            strcpy(description, "The specified window has no context");
        else
            strcpy(description, "ERROR: UNKNOWN GLFW ERROR");
    }

    if (_glfw.initialized)
    {
        error = _glfwPlatformGetTls(&_glfw.errorSlot);
        if (!error)
        {
            error = calloc(1, sizeof(_GLFWerror));
            _glfwPlatformSetTls(&_glfw.errorSlot, error);
            _glfwPlatformLockMutex(&_glfw.errorLock);
            error->next = _glfw.errorListHead;
            _glfw.errorListHead = error;
            _glfwPlatformUnlockMutex(&_glfw.errorLock);
        }
    }
    else
        error = &_glfwMainThreadError;

    error->code = code;
    strcpy(error->description, description);

    if (_glfwErrorCallback)
        _glfwErrorCallback(code, description);
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI int glfwInit(void)
{
    if (_glfw.initialized)
        return GLFW_TRUE;

    memset(&_glfw, 0, sizeof(_glfw));
    _glfw.hints.init = _glfwInitHints;

    if (!_glfwPlatformInit())
    {
        terminate();
        return GLFW_FALSE;
    }

    if (!_glfwPlatformCreateMutex(&_glfw.errorLock) ||
        !_glfwPlatformCreateTls(&_glfw.errorSlot) ||
        !_glfwPlatformCreateTls(&_glfw.contextSlot))
    {
        terminate();
        return GLFW_FALSE;
    }

    _glfwPlatformSetTls(&_glfw.errorSlot, &_glfwMainThreadError);

    _glfwInitGamepadMappings();

    _glfw.initialized = GLFW_TRUE;
    _glfw.timer.offset = _glfwPlatformGetTimerValue();

    glfwDefaultWindowHints();
    return GLFW_TRUE;
}

GLFWAPI void glfwTerminate(void)
{
    if (!_glfw.initialized)
        return;

    terminate();
}

GLFWAPI void glfwInitHint(int hint, int value)
{
    switch (hint)
    {
        case GLFW_JOYSTICK_HAT_BUTTONS:
            _glfwInitHints.hatButtons = value;
            return;
        case GLFW_COCOA_CHDIR_RESOURCES:
            _glfwInitHints.ns.chdir = value;
            return;
        case GLFW_COCOA_MENUBAR:
            _glfwInitHints.ns.menubar = value;
            return;
    }

    _glfwInputError(GLFW_INVALID_ENUM,
                    "Invalid init hint 0x%08X", hint);
}

GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev)
{
    if (major != NULL)
        *major = GLFW_VERSION_MAJOR;
    if (minor != NULL)
        *minor = GLFW_VERSION_MINOR;
    if (rev != NULL)
        *rev = GLFW_VERSION_REVISION;
}

GLFWAPI const char* glfwGetVersionString(void)
{
    return _glfwPlatformGetVersionString();
}

GLFWAPI int glfwGetError(const char** description)
{
    _GLFWerror* error;
    int code = GLFW_NO_ERROR;

    if (description)
        *description = NULL;

    if (_glfw.initialized)
        error = _glfwPlatformGetTls(&_glfw.errorSlot);
    else
        error = &_glfwMainThreadError;

    if (error)
    {
        code = error->code;
        error->code = GLFW_NO_ERROR;
        if (description && code)
            *description = error->description;
    }

    return code;
}

GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun)
{
    _GLFW_SWAP_POINTERS(_glfwErrorCallback, cbfun);
    return cbfun;
}

//========================================================================
// GLFW 3.3 - www.glfw.org - input.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================

// Internal key state used for sticky keys
#define _GLFW_STICK 3

// Internal constants for gamepad mapping source types
#define _GLFW_JOYSTICK_AXIS     1
#define _GLFW_JOYSTICK_BUTTON   2
#define _GLFW_JOYSTICK_HATBIT   3

// Finds a mapping based on joystick GUID
//
static _GLFWmapping* findMapping(const char* guid)
{
    int i;

    for (i = 0;  i < _glfw.mappingCount;  i++)
    {
        if (strcmp(_glfw.mappings[i].guid, guid) == 0)
            return _glfw.mappings + i;
    }

    return NULL;
}

// Checks whether a gamepad mapping element is present in the hardware
//
static GLFWbool isValidElementForJoystick(const _GLFWmapelement* e,
                                          const _GLFWjoystick* js)
{
    if (e->type == _GLFW_JOYSTICK_HATBIT && (e->index >> 4) >= js->hatCount)
        return GLFW_FALSE;
    else if (e->type == _GLFW_JOYSTICK_BUTTON && e->index >= js->buttonCount)
        return GLFW_FALSE;
    else if (e->type == _GLFW_JOYSTICK_AXIS && e->index >= js->axisCount)
        return GLFW_FALSE;

    return GLFW_TRUE;
}

// Finds a mapping based on joystick GUID and verifies element indices
//
static _GLFWmapping* findValidMapping(const _GLFWjoystick* js)
{
    _GLFWmapping* mapping = findMapping(js->guid);
    if (mapping)
    {
        int i;

        for (i = 0;  i <= GLFW_GAMEPAD_BUTTON_LAST;  i++)
        {
            if (!isValidElementForJoystick(mapping->buttons + i, js))
                return NULL;
        }

        for (i = 0;  i <= GLFW_GAMEPAD_AXIS_LAST;  i++)
        {
            if (!isValidElementForJoystick(mapping->axes + i, js))
                return NULL;
        }
    }

    return mapping;
}

// Parses an SDL_GameControllerDB line and adds it to the mapping list
//
static GLFWbool parseMapping(_GLFWmapping* mapping, const char* string)
{
    const char* c = string;
    size_t i, length;
    struct
    {
        const char* name;
        _GLFWmapelement* element;
    } fields[] =
    {
        { "platform",      NULL },
        { "a",             mapping->buttons + GLFW_GAMEPAD_BUTTON_A },
        { "b",             mapping->buttons + GLFW_GAMEPAD_BUTTON_B },
        { "x",             mapping->buttons + GLFW_GAMEPAD_BUTTON_X },
        { "y",             mapping->buttons + GLFW_GAMEPAD_BUTTON_Y },
        { "back",          mapping->buttons + GLFW_GAMEPAD_BUTTON_BACK },
        { "start",         mapping->buttons + GLFW_GAMEPAD_BUTTON_START },
        { "guide",         mapping->buttons + GLFW_GAMEPAD_BUTTON_GUIDE },
        { "leftshoulder",  mapping->buttons + GLFW_GAMEPAD_BUTTON_LEFT_BUMPER },
        { "rightshoulder", mapping->buttons + GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER },
        { "leftstick",     mapping->buttons + GLFW_GAMEPAD_BUTTON_LEFT_THUMB },
        { "rightstick",    mapping->buttons + GLFW_GAMEPAD_BUTTON_RIGHT_THUMB },
        { "dpup",          mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_UP },
        { "dpright",       mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
        { "dpdown",        mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_DOWN },
        { "dpleft",        mapping->buttons + GLFW_GAMEPAD_BUTTON_DPAD_LEFT },
        { "lefttrigger",   mapping->axes + GLFW_GAMEPAD_AXIS_LEFT_TRIGGER },
        { "righttrigger",  mapping->axes + GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER },
        { "leftx",         mapping->axes + GLFW_GAMEPAD_AXIS_LEFT_X },
        { "lefty",         mapping->axes + GLFW_GAMEPAD_AXIS_LEFT_Y },
        { "rightx",        mapping->axes + GLFW_GAMEPAD_AXIS_RIGHT_X },
        { "righty",        mapping->axes + GLFW_GAMEPAD_AXIS_RIGHT_Y }
    };

    length = strcspn(c, ",");
    if (length != 32 || c[length] != ',')
    {
        _glfwInputError(GLFW_INVALID_VALUE, NULL);
        return GLFW_FALSE;
    }

    memcpy(mapping->guid, c, length);
    c += length + 1;

    length = strcspn(c, ",");
    if (length >= sizeof(mapping->name) || c[length] != ',')
    {
        _glfwInputError(GLFW_INVALID_VALUE, NULL);
        return GLFW_FALSE;
    }

    memcpy(mapping->name, c, length);
    c += length + 1;

    while (*c)
    {
        // TODO: Implement output modifiers
        if (*c == '+' || *c == '-')
            return GLFW_FALSE;

        for (i = 0;  i < sizeof(fields) / sizeof(fields[0]);  i++)
        {
            length = strlen(fields[i].name);
            if (strncmp(c, fields[i].name, length) != 0 || c[length] != ':')
                continue;

            c += length + 1;

            if (fields[i].element)
            {
                _GLFWmapelement* e = fields[i].element;
                int8_t minimum = -1;
                int8_t maximum = 1;

                if (*c == '+')
                {
                    minimum = 0;
                    c += 1;
                }
                else if (*c == '-')
                {
                    maximum = 0;
                    c += 1;
                }

                if (*c == 'a')
                    e->type = _GLFW_JOYSTICK_AXIS;
                else if (*c == 'b')
                    e->type = _GLFW_JOYSTICK_BUTTON;
                else if (*c == 'h')
                    e->type = _GLFW_JOYSTICK_HATBIT;
                else
                    break;

                if (e->type == _GLFW_JOYSTICK_HATBIT)
                {
                    const unsigned long hat = strtoul(c + 1, (char**) &c, 10);
                    const unsigned long bit = strtoul(c + 1, (char**) &c, 10);
                    e->index = (uint8_t) ((hat << 4) | bit);
                }
                else
                    e->index = (uint8_t) strtoul(c + 1, (char**) &c, 10);

                if (e->type == _GLFW_JOYSTICK_AXIS)
                {
                    e->axisScale = 2 / (maximum - minimum);
                    e->axisOffset = -(maximum + minimum);

                    if (*c == '~')
                    {
                        e->axisScale = -e->axisScale;
                        e->axisOffset = -e->axisOffset;
                    }
                }
            }
            else
            {
                length = strlen(_GLFW_PLATFORM_MAPPING_NAME);
                if (strncmp(c, _GLFW_PLATFORM_MAPPING_NAME, length) != 0)
                    return GLFW_FALSE;
            }

            break;
        }

        c += strcspn(c, ",");
        c += strspn(c, ",");
    }

    for (i = 0;  i < 32;  i++)
    {
        if (mapping->guid[i] >= 'A' && mapping->guid[i] <= 'F')
            mapping->guid[i] += 'a' - 'A';
    }

    _glfwPlatformUpdateGamepadGUID(mapping->guid);
    return GLFW_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                         GLFW event API                       //////
//////////////////////////////////////////////////////////////////////////

// Notifies shared code of a physical key event
//
void _glfwInputKey(_GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key >= 0 && key <= GLFW_KEY_LAST)
    {
        GLFWbool repeated = GLFW_FALSE;

        if (action == GLFW_RELEASE && window->keys[key] == GLFW_RELEASE)
            return;

        if (action == GLFW_PRESS && window->keys[key] == GLFW_PRESS)
            repeated = GLFW_TRUE;

        if (action == GLFW_RELEASE && window->stickyKeys)
            window->keys[key] = _GLFW_STICK;
        else
            window->keys[key] = (char) action;

        if (repeated)
            action = GLFW_REPEAT;
    }

    if (!window->lockKeyMods)
        mods &= ~(GLFW_MOD_CAPS_LOCK | GLFW_MOD_NUM_LOCK);

    if (window->callbacks.key)
        window->callbacks.key((GLFWwindow*) window, key, scancode, action, mods);
}

// Notifies shared code of a Unicode codepoint input event
// The 'plain' parameter determines whether to emit a regular character event
//
void _glfwInputChar(_GLFWwindow* window, uint32_t codepoint, int mods, GLFWbool plain)
{
    if (codepoint < 32 || (codepoint > 126 && codepoint < 160))
        return;

    if (!window->lockKeyMods)
        mods &= ~(GLFW_MOD_CAPS_LOCK | GLFW_MOD_NUM_LOCK);

    if (window->callbacks.charmods)
        window->callbacks.charmods((GLFWwindow*) window, codepoint, mods);

    if (plain)
    {
        if (window->callbacks.character)
            window->callbacks.character((GLFWwindow*) window, codepoint);
    }
}

// Notifies shared code of a scroll event
//
void _glfwInputScroll(_GLFWwindow* window, double xoffset, double yoffset)
{
    if (window->callbacks.scroll)
        window->callbacks.scroll((GLFWwindow*) window, xoffset, yoffset);
}

// Notifies shared code of a mouse button click event
//
void _glfwInputMouseClick(_GLFWwindow* window, int button, int action, int mods)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return;

    if (!window->lockKeyMods)
        mods &= ~(GLFW_MOD_CAPS_LOCK | GLFW_MOD_NUM_LOCK);

    if (action == GLFW_RELEASE && window->stickyMouseButtons)
        window->mouseButtons[button] = _GLFW_STICK;
    else
        window->mouseButtons[button] = (char) action;

    if (window->callbacks.mouseButton)
        window->callbacks.mouseButton((GLFWwindow*) window, button, action, mods);
}

// Notifies shared code of a cursor motion event
// The position is specified in content area relative screen coordinates
//
void _glfwInputCursorPos(_GLFWwindow* window, double xpos, double ypos)
{
    if (window->virtualCursorPosX == xpos && window->virtualCursorPosY == ypos)
        return;

    window->virtualCursorPosX = xpos;
    window->virtualCursorPosY = ypos;

    if (window->callbacks.cursorPos)
        window->callbacks.cursorPos((GLFWwindow*) window, xpos, ypos);
}

// Notifies shared code of a cursor enter/leave event
//
void _glfwInputCursorEnter(_GLFWwindow* window, GLFWbool entered)
{
    if (window->callbacks.cursorEnter)
        window->callbacks.cursorEnter((GLFWwindow*) window, entered);
}

// Notifies shared code of files or directories dropped on a window
//
void _glfwInputDrop(_GLFWwindow* window, int count, const char** paths)
{
    if (window->callbacks.drop)
        window->callbacks.drop((GLFWwindow*) window, count, paths);
}

// Notifies shared code of a joystick connection or disconnection
//
void _glfwInputJoystick(_GLFWjoystick* js, int event)
{
    const int jid = (int) (js - _glfw.joysticks);

    if (event == GLFW_CONNECTED)
        js->connected = GLFW_TRUE;
    else if (event == GLFW_DISCONNECTED)
        js->connected = GLFW_FALSE;

    if (_glfw.callbacks.joystick)
        _glfw.callbacks.joystick(jid, event);
}

// Notifies shared code of the new value of a joystick axis
//
void _glfwInputJoystickAxis(_GLFWjoystick* js, int axis, float value)
{
    js->axes[axis] = value;
}

// Notifies shared code of the new value of a joystick button
//
void _glfwInputJoystickButton(_GLFWjoystick* js, int button, char value)
{
    js->buttons[button] = value;
}

// Notifies shared code of the new value of a joystick hat
//
void _glfwInputJoystickHat(_GLFWjoystick* js, int hat, char value)
{
    const int base = js->buttonCount + hat * 4;

    js->buttons[base + 0] = (value & 0x01) ? GLFW_PRESS : GLFW_RELEASE;
    js->buttons[base + 1] = (value & 0x02) ? GLFW_PRESS : GLFW_RELEASE;
    js->buttons[base + 2] = (value & 0x04) ? GLFW_PRESS : GLFW_RELEASE;
    js->buttons[base + 3] = (value & 0x08) ? GLFW_PRESS : GLFW_RELEASE;

    js->hats[hat] = value;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Adds the built-in set of gamepad mappings
//
void _glfwInitGamepadMappings(void)
{
    int jid;
    size_t i;
    const size_t count = sizeof(_glfwDefaultMappings) / sizeof(char*);
    _glfw.mappings = calloc(count, sizeof(_GLFWmapping));

    for (i = 0;  i < count;  i++)
    {
        if (parseMapping(&_glfw.mappings[_glfw.mappingCount], _glfwDefaultMappings[i]))
            _glfw.mappingCount++;
    }

    for (jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        _GLFWjoystick* js = _glfw.joysticks + jid;
        if (js->connected)
            js->mapping = findValidMapping(js);
    }
}

// Returns an available joystick object with arrays and name allocated
//
_GLFWjoystick* _glfwAllocJoystick(const char* name,
                                  const char* guid,
                                  int axisCount,
                                  int buttonCount,
                                  int hatCount)
{
    int jid;
    _GLFWjoystick* js;

    for (jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        if (!_glfw.joysticks[jid].allocated)
            break;
    }

    if (jid > GLFW_JOYSTICK_LAST)
        return NULL;

    js = _glfw.joysticks + jid;
    js->allocated   = GLFW_TRUE;
    js->axes        = calloc(axisCount, sizeof(float));
    js->buttons     = calloc(buttonCount + (size_t) hatCount * 4, 1);
    js->hats        = calloc(hatCount, 1);
    js->axisCount   = axisCount;
    js->buttonCount = buttonCount;
    js->hatCount    = hatCount;

    strncpy(js->name, name, sizeof(js->name) - 1);
    strncpy(js->guid, guid, sizeof(js->guid) - 1);
    js->mapping = findValidMapping(js);

    return js;
}

// Frees arrays and name and flags the joystick object as unused
//
void _glfwFreeJoystick(_GLFWjoystick* js)
{
    free(js->axes);
    free(js->buttons);
    free(js->hats);
    memset(js, 0, sizeof(_GLFWjoystick));
}

// Center the cursor in the content area of the specified window
//
void _glfwCenterCursorInContentArea(_GLFWwindow* window)
{
    int width, height;

    _glfwPlatformGetWindowSize(window, &width, &height);
    _glfwPlatformSetCursorPos(window, width / 2.0, height / 2.0);
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI int glfwGetInputMode(GLFWwindow* handle, int mode)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(0);

    switch (mode)
    {
        case GLFW_CURSOR:
            return window->cursorMode;
        case GLFW_STICKY_KEYS:
            return window->stickyKeys;
        case GLFW_STICKY_MOUSE_BUTTONS:
            return window->stickyMouseButtons;
        case GLFW_LOCK_KEY_MODS:
            return window->lockKeyMods;
        case GLFW_RAW_MOUSE_MOTION:
            return window->rawMouseMotion;
    }

    _glfwInputError(GLFW_INVALID_ENUM, "Invalid input mode 0x%08X", mode);
    return 0;
}

GLFWAPI void glfwSetInputMode(GLFWwindow* handle, int mode, int value)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (mode == GLFW_CURSOR)
    {
        if (value != GLFW_CURSOR_NORMAL &&
            value != GLFW_CURSOR_HIDDEN &&
            value != GLFW_CURSOR_DISABLED)
        {
            _glfwInputError(GLFW_INVALID_ENUM,
                            "Invalid cursor mode 0x%08X",
                            value);
            return;
        }

        if (window->cursorMode == value)
            return;

        window->cursorMode = value;

        _glfwPlatformGetCursorPos(window,
                                  &window->virtualCursorPosX,
                                  &window->virtualCursorPosY);
        _glfwPlatformSetCursorMode(window, value);
    }
    else if (mode == GLFW_STICKY_KEYS)
    {
        value = value ? GLFW_TRUE : GLFW_FALSE;
        if (window->stickyKeys == value)
            return;

        if (!value)
        {
            int i;

            // Release all sticky keys
            for (i = 0;  i <= GLFW_KEY_LAST;  i++)
            {
                if (window->keys[i] == _GLFW_STICK)
                    window->keys[i] = GLFW_RELEASE;
            }
        }

        window->stickyKeys = value;
    }
    else if (mode == GLFW_STICKY_MOUSE_BUTTONS)
    {
        value = value ? GLFW_TRUE : GLFW_FALSE;
        if (window->stickyMouseButtons == value)
            return;

        if (!value)
        {
            int i;

            // Release all sticky mouse buttons
            for (i = 0;  i <= GLFW_MOUSE_BUTTON_LAST;  i++)
            {
                if (window->mouseButtons[i] == _GLFW_STICK)
                    window->mouseButtons[i] = GLFW_RELEASE;
            }
        }

        window->stickyMouseButtons = value;
    }
    else if (mode == GLFW_LOCK_KEY_MODS)
    {
        window->lockKeyMods = value ? GLFW_TRUE : GLFW_FALSE;
    }
    else if (mode == GLFW_RAW_MOUSE_MOTION)
    {
        if (!_glfwPlatformRawMouseMotionSupported())
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "Raw mouse motion is not supported on this system");
            return;
        }

        value = value ? GLFW_TRUE : GLFW_FALSE;
        if (window->rawMouseMotion == value)
            return;

        window->rawMouseMotion = value;
        _glfwPlatformSetRawMouseMotion(window, value);
    }
    else
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid input mode 0x%08X", mode);
}

GLFWAPI int glfwRawMouseMotionSupported(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);
    return _glfwPlatformRawMouseMotionSupported();
}

GLFWAPI const char* glfwGetKeyName(int key, int scancode)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (key != GLFW_KEY_UNKNOWN)
    {
        if (key != GLFW_KEY_KP_EQUAL &&
            (key < GLFW_KEY_KP_0 || key > GLFW_KEY_KP_ADD) &&
            (key < GLFW_KEY_APOSTROPHE || key > GLFW_KEY_WORLD_2))
        {
            return NULL;
        }

        scancode = _glfwPlatformGetKeyScancode(key);
    }

    return _glfwPlatformGetScancodeName(scancode);
}

GLFWAPI int glfwGetKeyScancode(int key)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(-1);

    if (key < GLFW_KEY_SPACE || key > GLFW_KEY_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid key %i", key);
        return GLFW_RELEASE;
    }

    return _glfwPlatformGetKeyScancode(key);
}

GLFWAPI int glfwGetKey(GLFWwindow* handle, int key)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_RELEASE);

    if (key < GLFW_KEY_SPACE || key > GLFW_KEY_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid key %i", key);
        return GLFW_RELEASE;
    }

    if (window->keys[key] == _GLFW_STICK)
    {
        // Sticky mode: release key now
        window->keys[key] = GLFW_RELEASE;
        return GLFW_PRESS;
    }

    return (int) window->keys[key];
}

GLFWAPI int glfwGetMouseButton(GLFWwindow* handle, int button)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_RELEASE);

    if (button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid mouse button %i", button);
        return GLFW_RELEASE;
    }

    if (window->mouseButtons[button] == _GLFW_STICK)
    {
        // Sticky mode: release mouse button now
        window->mouseButtons[button] = GLFW_RELEASE;
        return GLFW_PRESS;
    }

    return (int) window->mouseButtons[button];
}

GLFWAPI void glfwGetCursorPos(GLFWwindow* handle, double* xpos, double* ypos)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;

    _GLFW_REQUIRE_INIT();

    if (window->cursorMode == GLFW_CURSOR_DISABLED)
    {
        if (xpos)
            *xpos = window->virtualCursorPosX;
        if (ypos)
            *ypos = window->virtualCursorPosY;
    }
    else
        _glfwPlatformGetCursorPos(window, xpos, ypos);
}

GLFWAPI void glfwSetCursorPos(GLFWwindow* handle, double xpos, double ypos)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (xpos != xpos || xpos < -DBL_MAX || xpos > DBL_MAX ||
        ypos != ypos || ypos < -DBL_MAX || ypos > DBL_MAX)
    {
        _glfwInputError(GLFW_INVALID_VALUE,
                        "Invalid cursor position %f %f",
                        xpos, ypos);
        return;
    }

    if (!_glfwPlatformWindowFocused(window))
        return;

    if (window->cursorMode == GLFW_CURSOR_DISABLED)
    {
        // Only update the accumulated position if the cursor is disabled
        window->virtualCursorPosX = xpos;
        window->virtualCursorPosY = ypos;
    }
    else
    {
        // Update system cursor position
        _glfwPlatformSetCursorPos(window, xpos, ypos);
    }
}

GLFWAPI GLFWcursor* glfwCreateCursor(const GLFWimage* image, int xhot, int yhot)
{
    _GLFWcursor* cursor;

    assert(image != NULL);
    assert(image->pixels != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (image->width <= 0 || image->height <= 0)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid image dimensions for cursor");
        return NULL;
    }

    cursor = calloc(1, sizeof(_GLFWcursor));
    cursor->next = _glfw.cursorListHead;
    _glfw.cursorListHead = cursor;

    if (!_glfwPlatformCreateCursor(cursor, image, xhot, yhot))
    {
        glfwDestroyCursor((GLFWcursor*) cursor);
        return NULL;
    }

    return (GLFWcursor*) cursor;
}

GLFWAPI GLFWcursor* glfwCreateStandardCursor(int shape)
{
    _GLFWcursor* cursor;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (shape != GLFW_ARROW_CURSOR &&
        shape != GLFW_IBEAM_CURSOR &&
        shape != GLFW_CROSSHAIR_CURSOR &&
        shape != GLFW_HAND_CURSOR &&
        shape != GLFW_HRESIZE_CURSOR &&
        shape != GLFW_VRESIZE_CURSOR)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid standard cursor 0x%08X", shape);
        return NULL;
    }

    cursor = calloc(1, sizeof(_GLFWcursor));
    cursor->next = _glfw.cursorListHead;
    _glfw.cursorListHead = cursor;

    if (!_glfwPlatformCreateStandardCursor(cursor, shape))
    {
        glfwDestroyCursor((GLFWcursor*) cursor);
        return NULL;
    }

    return (GLFWcursor*) cursor;
}

GLFWAPI void glfwDestroyCursor(GLFWcursor* handle)
{
    _GLFWcursor* cursor = (_GLFWcursor*) handle;

    _GLFW_REQUIRE_INIT();

    if (cursor == NULL)
        return;

    // Make sure the cursor is not being used by any window
    {
        _GLFWwindow* window;

        for (window = _glfw.windowListHead;  window;  window = window->next)
        {
            if (window->cursor == cursor)
                glfwSetCursor((GLFWwindow*) window, NULL);
        }
    }

    _glfwPlatformDestroyCursor(cursor);

    // Unlink cursor from global linked list
    {
        _GLFWcursor** prev = &_glfw.cursorListHead;

        while (*prev != cursor)
            prev = &((*prev)->next);

        *prev = cursor->next;
    }

    free(cursor);
}

GLFWAPI void glfwSetCursor(GLFWwindow* windowHandle, GLFWcursor* cursorHandle)
{
    _GLFWwindow* window = (_GLFWwindow*) windowHandle;
    _GLFWcursor* cursor = (_GLFWcursor*) cursorHandle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    window->cursor = cursor;

    _glfwPlatformSetCursor(window, cursor);
}

GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* handle, GLFWkeyfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.key, cbfun);
    return cbfun;
}

GLFWAPI GLFWcharfun glfwSetCharCallback(GLFWwindow* handle, GLFWcharfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.character, cbfun);
    return cbfun;
}

GLFWAPI GLFWcharmodsfun glfwSetCharModsCallback(GLFWwindow* handle, GLFWcharmodsfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.charmods, cbfun);
    return cbfun;
}

GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* handle,
                                                      GLFWmousebuttonfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.mouseButton, cbfun);
    return cbfun;
}

GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* handle,
                                                  GLFWcursorposfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.cursorPos, cbfun);
    return cbfun;
}

GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* handle,
                                                      GLFWcursorenterfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.cursorEnter, cbfun);
    return cbfun;
}

GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* handle,
                                            GLFWscrollfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.scroll, cbfun);
    return cbfun;
}

GLFWAPI GLFWdropfun glfwSetDropCallback(GLFWwindow* handle, GLFWdropfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.drop, cbfun);
    return cbfun;
}

GLFWAPI int glfwJoystickPresent(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return GLFW_FALSE;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return GLFW_FALSE;

    return _glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE);
}

GLFWAPI const float* glfwGetJoystickAxes(int jid, int* count)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_AXES))
        return NULL;

    *count = js->axisCount;
    return js->axes;
}

GLFWAPI const unsigned char* glfwGetJoystickButtons(int jid, int* count)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_BUTTONS))
        return NULL;

    if (_glfw.hints.init.hatButtons)
        *count = js->buttonCount + js->hatCount * 4;
    else
        *count = js->buttonCount;

    return js->buttons;
}

GLFWAPI const unsigned char* glfwGetJoystickHats(int jid, int* count)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_BUTTONS))
        return NULL;

    *count = js->hatCount;
    return js->hats;
}

GLFWAPI const char* glfwGetJoystickName(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return NULL;

    return js->name;
}

GLFWAPI const char* glfwGetJoystickGUID(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return NULL;

    return js->guid;
}

GLFWAPI void glfwSetJoystickUserPointer(int jid, void* pointer)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT();

    js = _glfw.joysticks + jid;
    if (!js->allocated)
        return;

    js->userPointer = pointer;
}

GLFWAPI void* glfwGetJoystickUserPointer(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    js = _glfw.joysticks + jid;
    if (!js->allocated)
        return NULL;

    return js->userPointer;
}

GLFWAPI GLFWjoystickfun glfwSetJoystickCallback(GLFWjoystickfun cbfun)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(_glfw.callbacks.joystick, cbfun);
    return cbfun;
}

GLFWAPI int glfwUpdateGamepadMappings(const char* string)
{
    int jid;
    const char* c = string;

    assert(string != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    while (*c)
    {
        if ((*c >= '0' && *c <= '9') ||
            (*c >= 'a' && *c <= 'f') ||
            (*c >= 'A' && *c <= 'F'))
        {
            char line[1024];

            const size_t length = strcspn(c, "\r\n");
            if (length < sizeof(line))
            {
                _GLFWmapping mapping = {{0}};

                memcpy(line, c, length);
                line[length] = '\0';

                if (parseMapping(&mapping, line))
                {
                    _GLFWmapping* previous = findMapping(mapping.guid);
                    if (previous)
                        *previous = mapping;
                    else
                    {
                        _glfw.mappingCount++;
                        _glfw.mappings =
                            realloc(_glfw.mappings,
                                    sizeof(_GLFWmapping) * _glfw.mappingCount);
                        _glfw.mappings[_glfw.mappingCount - 1] = mapping;
                    }
                }
            }

            c += length;
        }
        else
        {
            c += strcspn(c, "\r\n");
            c += strspn(c, "\r\n");
        }
    }

    for (jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        _GLFWjoystick* js = _glfw.joysticks + jid;
        if (js->connected)
            js->mapping = findValidMapping(js);
    }

    return GLFW_TRUE;
}

GLFWAPI int glfwJoystickIsGamepad(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return GLFW_FALSE;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return GLFW_FALSE;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return GLFW_FALSE;

    return js->mapping != NULL;
}

GLFWAPI const char* glfwGetGamepadName(int jid)
{
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return NULL;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return NULL;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_PRESENCE))
        return NULL;

    if (!js->mapping)
        return NULL;

    return js->mapping->name;
}

GLFWAPI int glfwGetGamepadState(int jid, GLFWgamepadstate* state)
{
    int i;
    _GLFWjoystick* js;

    assert(jid >= GLFW_JOYSTICK_1);
    assert(jid <= GLFW_JOYSTICK_LAST);
    assert(state != NULL);

    memset(state, 0, sizeof(GLFWgamepadstate));

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    if (jid < 0 || jid > GLFW_JOYSTICK_LAST)
    {
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid joystick ID %i", jid);
        return GLFW_FALSE;
    }

    js = _glfw.joysticks + jid;
    if (!js->connected)
        return GLFW_FALSE;

    if (!_glfwPlatformPollJoystick(js, _GLFW_POLL_ALL))
        return GLFW_FALSE;

    if (!js->mapping)
        return GLFW_FALSE;

    for (i = 0;  i <= GLFW_GAMEPAD_BUTTON_LAST;  i++)
    {
        const _GLFWmapelement* e = js->mapping->buttons + i;
        if (e->type == _GLFW_JOYSTICK_AXIS)
        {
            const float value = js->axes[e->index] * e->axisScale + e->axisOffset;
            // HACK: This should be baked into the value transform
            // TODO: Bake into transform when implementing output modifiers
            if (e->axisOffset < 0 || (e->axisOffset == 0 && e->axisScale > 0))
            {
                if (value >= 0.f)
                    state->buttons[i] = GLFW_PRESS;
            }
            else
            {
                if (value <= 0.f)
                    state->buttons[i] = GLFW_PRESS;
            }
        }
        else if (e->type == _GLFW_JOYSTICK_HATBIT)
        {
            const unsigned int hat = e->index >> 4;
            const unsigned int bit = e->index & 0xf;
            if (js->hats[hat] & bit)
                state->buttons[i] = GLFW_PRESS;
        }
        else if (e->type == _GLFW_JOYSTICK_BUTTON)
            state->buttons[i] = js->buttons[e->index];
    }

    for (i = 0;  i <= GLFW_GAMEPAD_AXIS_LAST;  i++)
    {
        const _GLFWmapelement* e = js->mapping->axes + i;
        if (e->type == _GLFW_JOYSTICK_AXIS)
        {
            const float value = js->axes[e->index] * e->axisScale + e->axisOffset;
            state->axes[i] = _glfw_fminf(_glfw_fmaxf(value, -1.f), 1.f);
        }
        else if (e->type == _GLFW_JOYSTICK_HATBIT)
        {
            const unsigned int hat = e->index >> 4;
            const unsigned int bit = e->index & 0xf;
            if (js->hats[hat] & bit)
                state->axes[i] = 1.f;
            else
                state->axes[i] = -1.f;
        }
        else if (e->type == _GLFW_JOYSTICK_BUTTON)
            state->axes[i] = js->buttons[e->index] * 2.f - 1.f;
    }

    return GLFW_TRUE;
}

GLFWAPI void glfwSetClipboardString(GLFWwindow* handle, const char* string)
{
    assert(string != NULL);

    _GLFW_REQUIRE_INIT();
    _glfwPlatformSetClipboardString(string);
}

GLFWAPI const char* glfwGetClipboardString(GLFWwindow* handle)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return _glfwPlatformGetClipboardString();
}

GLFWAPI double glfwGetTime(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(0.0);
    return (double) (_glfwPlatformGetTimerValue() - _glfw.timer.offset) /
        _glfwPlatformGetTimerFrequency();
}

GLFWAPI void glfwSetTime(double time)
{
    _GLFW_REQUIRE_INIT();

    if (time != time || time < 0.0 || time > 18446744073.0)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid time %f", time);
        return;
    }

    _glfw.timer.offset = _glfwPlatformGetTimerValue() -
        (uint64_t) (time * _glfwPlatformGetTimerFrequency());
}

GLFWAPI uint64_t glfwGetTimerValue(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(0);
    return _glfwPlatformGetTimerValue();
}

GLFWAPI uint64_t glfwGetTimerFrequency(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(0);
    return _glfwPlatformGetTimerFrequency();
}

//========================================================================
// GLFW 3.3 - www.glfw.org - monitor.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================


// Lexically compare video modes, used by qsort
//
static int compareVideoModes(const void* fp, const void* sp)
{
    const GLFWvidmode* fm = fp;
    const GLFWvidmode* sm = sp;
    const int fbpp = fm->redBits + fm->greenBits + fm->blueBits;
    const int sbpp = sm->redBits + sm->greenBits + sm->blueBits;
    const int farea = fm->width * fm->height;
    const int sarea = sm->width * sm->height;

    // First sort on color bits per pixel
    if (fbpp != sbpp)
        return fbpp - sbpp;

    // Then sort on screen area
    if (farea != sarea)
        return farea - sarea;

    // Then sort on width
    if (fm->width != sm->width)
        return fm->width - sm->width;

    // Lastly sort on refresh rate
    return fm->refreshRate - sm->refreshRate;
}

// Retrieves the available modes for the specified monitor
//
static GLFWbool refreshVideoModes(_GLFWmonitor* monitor)
{
    int modeCount;
    GLFWvidmode* modes;

    if (monitor->modes)
        return GLFW_TRUE;

    modes = _glfwPlatformGetVideoModes(monitor, &modeCount);
    if (!modes)
        return GLFW_FALSE;

    qsort(modes, modeCount, sizeof(GLFWvidmode), compareVideoModes);

    free(monitor->modes);
    monitor->modes = modes;
    monitor->modeCount = modeCount;

    return GLFW_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                         GLFW event API                       //////
//////////////////////////////////////////////////////////////////////////

// Notifies shared code of a monitor connection or disconnection
//
void _glfwInputMonitor(_GLFWmonitor* monitor, int action, int placement)
{
    if (action == GLFW_CONNECTED)
    {
        _glfw.monitorCount++;
        _glfw.monitors =
            realloc(_glfw.monitors, sizeof(_GLFWmonitor*) * _glfw.monitorCount);

        if (placement == _GLFW_INSERT_FIRST)
        {
            memmove(_glfw.monitors + 1,
                    _glfw.monitors,
                    ((size_t) _glfw.monitorCount - 1) * sizeof(_GLFWmonitor*));
            _glfw.monitors[0] = monitor;
        }
        else
            _glfw.monitors[_glfw.monitorCount - 1] = monitor;
    }
    else if (action == GLFW_DISCONNECTED)
    {
        int i;
        _GLFWwindow* window;

        for (window = _glfw.windowListHead;  window;  window = window->next)
        {
            if (window->monitor == monitor)
            {
                int width, height, xoff, yoff;
                _glfwPlatformGetWindowSize(window, &width, &height);
                _glfwPlatformSetWindowMonitor(window, NULL, 0, 0, width, height, 0);
                _glfwPlatformGetWindowFrameSize(window, &xoff, &yoff, NULL, NULL);
                _glfwPlatformSetWindowPos(window, xoff, yoff);
            }
        }

        for (i = 0;  i < _glfw.monitorCount;  i++)
        {
            if (_glfw.monitors[i] == monitor)
            {
                _glfw.monitorCount--;
                memmove(_glfw.monitors + i,
                        _glfw.monitors + i + 1,
                        ((size_t) _glfw.monitorCount - i) * sizeof(_GLFWmonitor*));
                break;
            }
        }
    }

    if (_glfw.callbacks.monitor)
        _glfw.callbacks.monitor((GLFWmonitor*) monitor, action);

    if (action == GLFW_DISCONNECTED)
        _glfwFreeMonitor(monitor);
}

// Notifies shared code that a full screen window has acquired or released
// a monitor
//
void _glfwInputMonitorWindow(_GLFWmonitor* monitor, _GLFWwindow* window)
{
    monitor->window = window;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Allocates and returns a monitor object with the specified name and dimensions
//
_GLFWmonitor* _glfwAllocMonitor(const char* name, int widthMM, int heightMM)
{
    _GLFWmonitor* monitor = calloc(1, sizeof(_GLFWmonitor));
    monitor->widthMM = widthMM;
    monitor->heightMM = heightMM;

    strncpy(monitor->name, name, sizeof(monitor->name) - 1);

    return monitor;
}

// Frees a monitor object and any data associated with it
//
void _glfwFreeMonitor(_GLFWmonitor* monitor)
{
    if (monitor == NULL)
        return;

    _glfwPlatformFreeMonitor(monitor);

    _glfwFreeGammaArrays(&monitor->originalRamp);
    _glfwFreeGammaArrays(&monitor->currentRamp);

    free(monitor->modes);
    free(monitor);
}

// Allocates red, green and blue value arrays of the specified size
//
void _glfwAllocGammaArrays(GLFWgammaramp* ramp, unsigned int size)
{
    ramp->red = calloc(size, sizeof(unsigned short));
    ramp->green = calloc(size, sizeof(unsigned short));
    ramp->blue = calloc(size, sizeof(unsigned short));
    ramp->size = size;
}

// Frees the red, green and blue value arrays and clears the struct
//
void _glfwFreeGammaArrays(GLFWgammaramp* ramp)
{
    free(ramp->red);
    free(ramp->green);
    free(ramp->blue);

    memset(ramp, 0, sizeof(GLFWgammaramp));
}

// Chooses the video mode most closely matching the desired one
//
const GLFWvidmode* _glfwChooseVideoMode(_GLFWmonitor* monitor,
                                        const GLFWvidmode* desired)
{
    int i;
    unsigned int sizeDiff, leastSizeDiff = UINT_MAX;
    unsigned int rateDiff, leastRateDiff = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    const GLFWvidmode* current;
    const GLFWvidmode* closest = NULL;

    if (!refreshVideoModes(monitor))
        return NULL;

    for (i = 0;  i < monitor->modeCount;  i++)
    {
        current = monitor->modes + i;

        colorDiff = 0;

        if (desired->redBits != GLFW_DONT_CARE)
            colorDiff += abs(current->redBits - desired->redBits);
        if (desired->greenBits != GLFW_DONT_CARE)
            colorDiff += abs(current->greenBits - desired->greenBits);
        if (desired->blueBits != GLFW_DONT_CARE)
            colorDiff += abs(current->blueBits - desired->blueBits);

        sizeDiff = abs((current->width - desired->width) *
                       (current->width - desired->width) +
                       (current->height - desired->height) *
                       (current->height - desired->height));

        if (desired->refreshRate != GLFW_DONT_CARE)
            rateDiff = abs(current->refreshRate - desired->refreshRate);
        else
            rateDiff = UINT_MAX - current->refreshRate;

        if ((colorDiff < leastColorDiff) ||
            (colorDiff == leastColorDiff && sizeDiff < leastSizeDiff) ||
            (colorDiff == leastColorDiff && sizeDiff == leastSizeDiff && rateDiff < leastRateDiff))
        {
            closest = current;
            leastSizeDiff = sizeDiff;
            leastRateDiff = rateDiff;
            leastColorDiff = colorDiff;
        }
    }

    return closest;
}

// Performs lexical comparison between two @ref GLFWvidmode structures
//
int _glfwCompareVideoModes(const GLFWvidmode* fm, const GLFWvidmode* sm)
{
    return compareVideoModes(fm, sm);
}

// Splits a color depth into red, green and blue bit depths
//
void _glfwSplitBPP(int bpp, int* red, int* green, int* blue)
{
    int delta;

    // We assume that by 32 the user really meant 24
    if (bpp == 32)
        bpp = 24;

    // Convert "bits per pixel" to red, green & blue sizes

    *red = *green = *blue = bpp / 3;
    delta = bpp - (*red * 3);
    if (delta >= 1)
        *green = *green + 1;

    if (delta == 2)
        *red = *red + 1;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI GLFWmonitor** glfwGetMonitors(int* count)
{
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    *count = _glfw.monitorCount;
    return (GLFWmonitor**) _glfw.monitors;
}

GLFWAPI GLFWmonitor* glfwGetPrimaryMonitor(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (!_glfw.monitorCount)
        return NULL;

    return (GLFWmonitor*) _glfw.monitors[0];
}

GLFWAPI void glfwGetMonitorPos(GLFWmonitor* handle, int* xpos, int* ypos)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;

    _GLFW_REQUIRE_INIT();

    _glfwPlatformGetMonitorPos(monitor, xpos, ypos);
}

GLFWAPI void glfwGetMonitorWorkarea(GLFWmonitor* handle,
                                    int* xpos, int* ypos,
                                    int* width, int* height)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;
    if (width)
        *width = 0;
    if (height)
        *height = 0;

    _GLFW_REQUIRE_INIT();

    _glfwPlatformGetMonitorWorkarea(monitor, xpos, ypos, width, height);
}

GLFWAPI void glfwGetMonitorPhysicalSize(GLFWmonitor* handle, int* widthMM, int* heightMM)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    if (widthMM)
        *widthMM = 0;
    if (heightMM)
        *heightMM = 0;

    _GLFW_REQUIRE_INIT();

    if (widthMM)
        *widthMM = monitor->widthMM;
    if (heightMM)
        *heightMM = monitor->heightMM;
}

GLFWAPI void glfwGetMonitorContentScale(GLFWmonitor* handle,
                                        float* xscale, float* yscale)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    if (xscale)
        *xscale = 0.f;
    if (yscale)
        *yscale = 0.f;

    _GLFW_REQUIRE_INIT();
    _glfwPlatformGetMonitorContentScale(monitor, xscale, yscale);
}

GLFWAPI const char* glfwGetMonitorName(GLFWmonitor* handle)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return monitor->name;
}

GLFWAPI void glfwSetMonitorUserPointer(GLFWmonitor* handle, void* pointer)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    _GLFW_REQUIRE_INIT();
    monitor->userPointer = pointer;
}

GLFWAPI void* glfwGetMonitorUserPointer(GLFWmonitor* handle)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return monitor->userPointer;
}

GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun cbfun)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(_glfw.callbacks.monitor, cbfun);
    return cbfun;
}

GLFWAPI const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* handle, int* count)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (!refreshVideoModes(monitor))
        return NULL;

    *count = monitor->modeCount;
    return monitor->modes;
}

GLFWAPI const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* handle)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    _glfwPlatformGetVideoMode(monitor, &monitor->currentMode);
    return &monitor->currentMode;
}

GLFWAPI void glfwSetGamma(GLFWmonitor* handle, float gamma)
{
    unsigned int i;
    unsigned short* values;
    GLFWgammaramp ramp;
    const GLFWgammaramp* original;
    assert(handle != NULL);
    assert(gamma > 0.f);
    assert(gamma <= FLT_MAX);

    _GLFW_REQUIRE_INIT();

    if (gamma != gamma || gamma <= 0.f || gamma > FLT_MAX)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid gamma value %f", gamma);
        return;
    }

    original = glfwGetGammaRamp(handle);
    if (!original)
        return;

    values = calloc(original->size, sizeof(unsigned short));

    for (i = 0;  i < original->size;  i++)
    {
        float value;

        // Calculate intensity
        value = i / (float) (original->size - 1);
        // Apply gamma curve
        value = powf(value, 1.f / gamma) * 65535.f + 0.5f;
        // Clamp to value range
        value = _glfw_fminf(value, 65535.f);

        values[i] = (unsigned short) value;
    }

    ramp.red = values;
    ramp.green = values;
    ramp.blue = values;
    ramp.size = original->size;

    glfwSetGammaRamp(handle, &ramp);
    free(values);
}

GLFWAPI const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* handle)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    _glfwFreeGammaArrays(&monitor->currentRamp);
    if (!_glfwPlatformGetGammaRamp(monitor, &monitor->currentRamp))
        return NULL;

    return &monitor->currentRamp;
}

GLFWAPI void glfwSetGammaRamp(GLFWmonitor* handle, const GLFWgammaramp* ramp)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    assert(monitor != NULL);
    assert(ramp != NULL);
    assert(ramp->size > 0);
    assert(ramp->red != NULL);
    assert(ramp->green != NULL);
    assert(ramp->blue != NULL);

    _GLFW_REQUIRE_INIT();

    if (ramp->size <= 0)
    {
        _glfwInputError(GLFW_INVALID_VALUE,
                        "Invalid gamma ramp size %i",
                        ramp->size);
        return;
    }

    if (!monitor->originalRamp.size)
    {
        if (!_glfwPlatformGetGammaRamp(monitor, &monitor->originalRamp))
            return;
    }

    _glfwPlatformSetGammaRamp(monitor, ramp);
}

/* TEMP - see if vulkan can be removed */

//========================================================================
// GLFW 3.3 - www.glfw.org - vulkan.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2018 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================

#define _GLFW_FIND_LOADER    1
#define _GLFW_REQUIRE_LOADER 2


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

GLFWbool _glfwInitVulkan(int mode)
{
    VkResult err;
    VkExtensionProperties* ep;
    uint32_t i, count;

    if (_glfw.vk.available)
        return GLFW_TRUE;

#if !defined(_GLFW_VULKAN_STATIC)
#if defined(_GLFW_VULKAN_LIBRARY)
    _glfw.vk.handle = _glfw_dlopen(_GLFW_VULKAN_LIBRARY);
#elif defined(_GLFW_WIN32)
    _glfw.vk.handle = _glfw_dlopen("vulkan-1.dll");
#elif defined(_GLFW_COCOA)
    _glfw.vk.handle = _glfw_dlopen("libvulkan.1.dylib");
    if (!_glfw.vk.handle)
        _glfw.vk.handle = _glfwLoadLocalVulkanLoaderNS();
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.vk.handle = _glfw_dlopen("libvulkan.so");
#else
    _glfw.vk.handle = _glfw_dlopen("libvulkan.so.1");
#endif
    if (!_glfw.vk.handle)
    {
        if (mode == _GLFW_REQUIRE_LOADER)
            _glfwInputError(GLFW_API_UNAVAILABLE, "Vulkan: Loader not found");

        return GLFW_FALSE;
    }

    _glfw.vk.GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)
        _glfw_dlsym(_glfw.vk.handle, "vkGetInstanceProcAddr");
    if (!_glfw.vk.GetInstanceProcAddr)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "Vulkan: Loader does not export vkGetInstanceProcAddr");

        _glfwTerminateVulkan();
        return GLFW_FALSE;
    }

    _glfw.vk.EnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)
        vkGetInstanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
    if (!_glfw.vk.EnumerateInstanceExtensionProperties)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "Vulkan: Failed to retrieve vkEnumerateInstanceExtensionProperties");

        _glfwTerminateVulkan();
        return GLFW_FALSE;
    }
#endif // _GLFW_VULKAN_STATIC

    err = vkEnumerateInstanceExtensionProperties(NULL, &count, NULL);
    if (err)
    {
        // NOTE: This happens on systems with a loader but without any Vulkan ICD
        if (mode == _GLFW_REQUIRE_LOADER)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "Vulkan: Failed to query instance extension count: %s",
                            _glfwGetVulkanResultString(err));
        }

        _glfwTerminateVulkan();
        return GLFW_FALSE;
    }

    ep = calloc(count, sizeof(VkExtensionProperties));

    err = vkEnumerateInstanceExtensionProperties(NULL, &count, ep);
    if (err)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "Vulkan: Failed to query instance extensions: %s",
                        _glfwGetVulkanResultString(err));

        free(ep);
        _glfwTerminateVulkan();
        return GLFW_FALSE;
    }

    for (i = 0;  i < count;  i++)
    {
        if (strcmp(ep[i].extensionName, "VK_KHR_surface") == 0)
            _glfw.vk.KHR_surface = GLFW_TRUE;
#if defined(_GLFW_WIN32)
        else if (strcmp(ep[i].extensionName, "VK_KHR_win32_surface") == 0)
            _glfw.vk.KHR_win32_surface = GLFW_TRUE;
#elif defined(_GLFW_COCOA)
        else if (strcmp(ep[i].extensionName, "VK_MVK_macos_surface") == 0)
            _glfw.vk.MVK_macos_surface = GLFW_TRUE;
        else if (strcmp(ep[i].extensionName, "VK_EXT_metal_surface") == 0)
            _glfw.vk.EXT_metal_surface = GLFW_TRUE;
#elif defined(_GLFW_X11)
        else if (strcmp(ep[i].extensionName, "VK_KHR_xlib_surface") == 0)
            _glfw.vk.KHR_xlib_surface = GLFW_TRUE;
        else if (strcmp(ep[i].extensionName, "VK_KHR_xcb_surface") == 0)
            _glfw.vk.KHR_xcb_surface = GLFW_TRUE;
#elif defined(_GLFW_WAYLAND)
        else if (strcmp(ep[i].extensionName, "VK_KHR_wayland_surface") == 0)
            _glfw.vk.KHR_wayland_surface = GLFW_TRUE;
#endif
    }

    free(ep);

    _glfw.vk.available = GLFW_TRUE;

    _glfwPlatformGetRequiredInstanceExtensions(_glfw.vk.extensions);

    return GLFW_TRUE;
}

void _glfwTerminateVulkan(void)
{
#if !defined(_GLFW_VULKAN_STATIC)
    if (_glfw.vk.handle)
        _glfw_dlclose(_glfw.vk.handle);
#endif
}

const char* _glfwGetVulkanResultString(VkResult result)
{
    switch (result)
    {
        case VK_SUCCESS:
            return "Success";
        case VK_NOT_READY:
            return "A fence or query has not yet completed";
        case VK_TIMEOUT:
            return "A wait operation has not completed in the specified time";
        case VK_EVENT_SET:
            return "An event is signaled";
        case VK_EVENT_RESET:
            return "An event is unsignaled";
        case VK_INCOMPLETE:
            return "A return array was too small for the result";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "A host memory allocation has failed";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "A device memory allocation has failed";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "Initialization of an object could not be completed for implementation-specific reasons";
        case VK_ERROR_DEVICE_LOST:
            return "The logical or physical device has been lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "Mapping of a memory object has failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "A requested layer is not present or could not be loaded";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "A requested extension is not supported";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "A requested feature is not supported";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "Too many objects of the type have already been created";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "A requested format is not supported on this device";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "A surface is no longer available";
        case VK_SUBOPTIMAL_KHR:
            return "A swapchain no longer matches the surface properties exactly, but can still be used";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "A surface has changed in such a way that it is no longer compatible with the swapchain";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "The display used by a swapchain does not use the same presentable image layout";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "A validation layer found an error";
        default:
            return "ERROR: UNKNOWN VULKAN ERROR";
    }
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI int glfwVulkanSupported(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);
    return _glfwInitVulkan(_GLFW_FIND_LOADER);
}

GLFWAPI const char** glfwGetRequiredInstanceExtensions(uint32_t* count)
{
    assert(count != NULL);

    *count = 0;

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (!_glfwInitVulkan(_GLFW_REQUIRE_LOADER))
        return NULL;

    if (!_glfw.vk.extensions[0])
        return NULL;

    *count = 2;
    return (const char**) _glfw.vk.extensions;
}

GLFWAPI GLFWvkproc glfwGetInstanceProcAddress(VkInstance instance,
                                              const char* procname)
{
    GLFWvkproc proc;
    assert(procname != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (!_glfwInitVulkan(_GLFW_REQUIRE_LOADER))
        return NULL;

    proc = (GLFWvkproc) vkGetInstanceProcAddr(instance, procname);
#if defined(_GLFW_VULKAN_STATIC)
    if (!proc)
    {
        if (strcmp(procname, "vkGetInstanceProcAddr") == 0)
            return (GLFWvkproc) vkGetInstanceProcAddr;
    }
#else
    if (!proc)
        proc = (GLFWvkproc) _glfw_dlsym(_glfw.vk.handle, procname);
#endif

    return proc;
}

GLFWAPI int glfwGetPhysicalDevicePresentationSupport(VkInstance instance,
                                                     VkPhysicalDevice device,
                                                     uint32_t queuefamily)
{
    assert(instance != VK_NULL_HANDLE);
    assert(device != VK_NULL_HANDLE);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    if (!_glfwInitVulkan(_GLFW_REQUIRE_LOADER))
        return GLFW_FALSE;

    if (!_glfw.vk.extensions[0])
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "Vulkan: Window surface creation extensions not found");
        return GLFW_FALSE;
    }

    return _glfwPlatformGetPhysicalDevicePresentationSupport(instance,
                                                             device,
                                                             queuefamily);
}

GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance,
                                         GLFWwindow* handle,
                                         const VkAllocationCallbacks* allocator,
                                         VkSurfaceKHR* surface)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(instance != VK_NULL_HANDLE);
    assert(window != NULL);
    assert(surface != NULL);

    *surface = VK_NULL_HANDLE;

    _GLFW_REQUIRE_INIT_OR_RETURN(VK_ERROR_INITIALIZATION_FAILED);

    if (!_glfwInitVulkan(_GLFW_REQUIRE_LOADER))
        return VK_ERROR_INITIALIZATION_FAILED;

    if (!_glfw.vk.extensions[0])
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "Vulkan: Window surface creation extensions not found");
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    if (window->context.client != GLFW_NO_API)
    {
        _glfwInputError(GLFW_INVALID_VALUE,
                        "Vulkan: Window surface creation requires the window to have the client API set to GLFW_NO_API");
        return VK_ERROR_NATIVE_WINDOW_IN_USE_KHR;
    }

    return _glfwPlatformCreateWindowSurface(instance, window, allocator, surface);
}

/* <TEMP> */

//========================================================================
// GLFW 3.3 - www.glfw.org - window.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
// Copyright (c) 2012 Torsten Walluhn <tw@mad-cad.net>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================


//////////////////////////////////////////////////////////////////////////
//////                         GLFW event API                       //////
//////////////////////////////////////////////////////////////////////////

// Notifies shared code that a window has lost or received input focus
//
void _glfwInputWindowFocus(_GLFWwindow* window, GLFWbool focused)
{
    if (window->callbacks.focus)
        window->callbacks.focus((GLFWwindow*) window, focused);

    if (!focused)
    {
        int key, button;

        for (key = 0;  key <= GLFW_KEY_LAST;  key++)
        {
            if (window->keys[key] == GLFW_PRESS)
            {
                const int scancode = _glfwPlatformGetKeyScancode(key);
                _glfwInputKey(window, key, scancode, GLFW_RELEASE, 0);
            }
        }

        for (button = 0;  button <= GLFW_MOUSE_BUTTON_LAST;  button++)
        {
            if (window->mouseButtons[button] == GLFW_PRESS)
                _glfwInputMouseClick(window, button, GLFW_RELEASE, 0);
        }
    }
}

// Notifies shared code that a window has moved
// The position is specified in content area relative screen coordinates
//
void _glfwInputWindowPos(_GLFWwindow* window, int x, int y)
{
    if (window->callbacks.pos)
        window->callbacks.pos((GLFWwindow*) window, x, y);
}

// Notifies shared code that a window has been resized
// The size is specified in screen coordinates
//
void _glfwInputWindowSize(_GLFWwindow* window, int width, int height)
{
    if (window->callbacks.size)
        window->callbacks.size((GLFWwindow*) window, width, height);
}

// Notifies shared code that a window has been iconified or restored
//
void _glfwInputWindowIconify(_GLFWwindow* window, GLFWbool iconified)
{
    if (window->callbacks.iconify)
        window->callbacks.iconify((GLFWwindow*) window, iconified);
}

// Notifies shared code that a window has been maximized or restored
//
void _glfwInputWindowMaximize(_GLFWwindow* window, GLFWbool maximized)
{
    if (window->callbacks.maximize)
        window->callbacks.maximize((GLFWwindow*) window, maximized);
}

// Notifies shared code that a window framebuffer has been resized
// The size is specified in pixels
//
void _glfwInputFramebufferSize(_GLFWwindow* window, int width, int height)
{
    if (window->callbacks.fbsize)
        window->callbacks.fbsize((GLFWwindow*) window, width, height);
}

// Notifies shared code that a window content scale has changed
// The scale is specified as the ratio between the current and default DPI
//
void _glfwInputWindowContentScale(_GLFWwindow* window, float xscale, float yscale)
{
    if (window->callbacks.scale)
        window->callbacks.scale((GLFWwindow*) window, xscale, yscale);
}

// Notifies shared code that the window contents needs updating
//
void _glfwInputWindowDamage(_GLFWwindow* window)
{
    if (window->callbacks.refresh)
        window->callbacks.refresh((GLFWwindow*) window);
}

// Notifies shared code that the user wishes to close a window
//
void _glfwInputWindowCloseRequest(_GLFWwindow* window)
{
    window->shouldClose = GLFW_TRUE;

    if (window->callbacks.close)
        window->callbacks.close((GLFWwindow*) window);
}

// Notifies shared code that a window has changed its desired monitor
//
void _glfwInputWindowMonitor(_GLFWwindow* window, _GLFWmonitor* monitor)
{
    window->monitor = monitor;
}

//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height,
                                     const char* title,
                                     GLFWmonitor* monitor,
                                     GLFWwindow* share)
{
    _GLFWfbconfig fbconfig;
    _GLFWctxconfig ctxconfig;
    _GLFWwndconfig wndconfig;
    _GLFWwindow* window;

    assert(title != NULL);
    assert(width >= 0);
    assert(height >= 0);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (width <= 0 || height <= 0)
    {
        _glfwInputError(GLFW_INVALID_VALUE,
                        "Invalid window size %ix%i",
                        width, height);

        return NULL;
    }

    fbconfig  = _glfw.hints.framebuffer;
    ctxconfig = _glfw.hints.context;
    wndconfig = _glfw.hints.window;

    wndconfig.width   = width;
    wndconfig.height  = height;
    wndconfig.title   = title;
    ctxconfig.share   = (_GLFWwindow*) share;

    if (!_glfwIsValidContextConfig(&ctxconfig))
        return NULL;

    window = calloc(1, sizeof(_GLFWwindow));
    window->next = _glfw.windowListHead;
    _glfw.windowListHead = window;

    window->videoMode.width       = width;
    window->videoMode.height      = height;
    window->videoMode.redBits     = fbconfig.redBits;
    window->videoMode.greenBits   = fbconfig.greenBits;
    window->videoMode.blueBits    = fbconfig.blueBits;
    window->videoMode.refreshRate = _glfw.hints.refreshRate;

    window->monitor     = (_GLFWmonitor*) monitor;
    window->resizable   = wndconfig.resizable;
    window->decorated   = wndconfig.decorated;
    window->autoIconify = wndconfig.autoIconify;
    window->floating    = wndconfig.floating;
    window->focusOnShow = wndconfig.focusOnShow;
    window->cursorMode  = GLFW_CURSOR_NORMAL;

    window->doublebuffer = fbconfig.doublebuffer;

    window->minwidth    = GLFW_DONT_CARE;
    window->minheight   = GLFW_DONT_CARE;
    window->maxwidth    = GLFW_DONT_CARE;
    window->maxheight   = GLFW_DONT_CARE;
    window->numer       = GLFW_DONT_CARE;
    window->denom       = GLFW_DONT_CARE;

    // Open the actual window and create its context
    if (!_glfwPlatformCreateWindow(window, &wndconfig, &ctxconfig, &fbconfig))
    {
        glfwDestroyWindow((GLFWwindow*) window);
        return NULL;
    }

    return (GLFWwindow*) window;
}

void glfwDefaultWindowHints(void)
{
    _GLFW_REQUIRE_INIT();

    // The default is OpenGL with minimum version 1.0
    memset(&_glfw.hints.context, 0, sizeof(_glfw.hints.context));
    _glfw.hints.context.client = GLFW_OPENGL_API;
    _glfw.hints.context.source = GLFW_NATIVE_CONTEXT_API;
    _glfw.hints.context.major  = 1;
    _glfw.hints.context.minor  = 0;

    // The default is a focused, visible, resizable window with decorations
    memset(&_glfw.hints.window, 0, sizeof(_glfw.hints.window));
    _glfw.hints.window.resizable    = GLFW_TRUE;
    _glfw.hints.window.visible      = GLFW_TRUE;
    _glfw.hints.window.decorated    = GLFW_TRUE;
    _glfw.hints.window.focused      = GLFW_TRUE;
    _glfw.hints.window.autoIconify  = GLFW_TRUE;
    _glfw.hints.window.centerCursor = GLFW_TRUE;
    _glfw.hints.window.focusOnShow  = GLFW_TRUE;

    // The default is 24 bits of color, 24 bits of depth and 8 bits of stencil,
    // double buffered
    memset(&_glfw.hints.framebuffer, 0, sizeof(_glfw.hints.framebuffer));
    _glfw.hints.framebuffer.redBits      = 8;
    _glfw.hints.framebuffer.greenBits    = 8;
    _glfw.hints.framebuffer.blueBits     = 8;
    _glfw.hints.framebuffer.alphaBits    = 8;
    _glfw.hints.framebuffer.depthBits    = 24;
    _glfw.hints.framebuffer.stencilBits  = 8;
    _glfw.hints.framebuffer.doublebuffer = GLFW_TRUE;

    // The default is to select the highest available refresh rate
    _glfw.hints.refreshRate = GLFW_DONT_CARE;

    // The default is to use full Retina resolution framebuffers
    _glfw.hints.window.ns.retina = GLFW_TRUE;
}

GLFWAPI void glfwWindowHint(int hint, int value)
{
    _GLFW_REQUIRE_INIT();

    switch (hint)
    {
        case GLFW_RED_BITS:
            _glfw.hints.framebuffer.redBits = value;
            return;
        case GLFW_GREEN_BITS:
            _glfw.hints.framebuffer.greenBits = value;
            return;
        case GLFW_BLUE_BITS:
            _glfw.hints.framebuffer.blueBits = value;
            return;
        case GLFW_ALPHA_BITS:
            _glfw.hints.framebuffer.alphaBits = value;
            return;
        case GLFW_DEPTH_BITS:
            _glfw.hints.framebuffer.depthBits = value;
            return;
        case GLFW_STENCIL_BITS:
            _glfw.hints.framebuffer.stencilBits = value;
            return;
        case GLFW_ACCUM_RED_BITS:
            _glfw.hints.framebuffer.accumRedBits = value;
            return;
        case GLFW_ACCUM_GREEN_BITS:
            _glfw.hints.framebuffer.accumGreenBits = value;
            return;
        case GLFW_ACCUM_BLUE_BITS:
            _glfw.hints.framebuffer.accumBlueBits = value;
            return;
        case GLFW_ACCUM_ALPHA_BITS:
            _glfw.hints.framebuffer.accumAlphaBits = value;
            return;
        case GLFW_AUX_BUFFERS:
            _glfw.hints.framebuffer.auxBuffers = value;
            return;
        case GLFW_STEREO:
            _glfw.hints.framebuffer.stereo = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_DOUBLEBUFFER:
            _glfw.hints.framebuffer.doublebuffer = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_TRANSPARENT_FRAMEBUFFER:
            _glfw.hints.framebuffer.transparent = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_SAMPLES:
            _glfw.hints.framebuffer.samples = value;
            return;
        case GLFW_SRGB_CAPABLE:
            _glfw.hints.framebuffer.sRGB = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_RESIZABLE:
            _glfw.hints.window.resizable = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_DECORATED:
            _glfw.hints.window.decorated = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_FOCUSED:
            _glfw.hints.window.focused = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_AUTO_ICONIFY:
            _glfw.hints.window.autoIconify = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_FLOATING:
            _glfw.hints.window.floating = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_MAXIMIZED:
            _glfw.hints.window.maximized = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_VISIBLE:
            _glfw.hints.window.visible = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_COCOA_RETINA_FRAMEBUFFER:
            _glfw.hints.window.ns.retina = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_COCOA_GRAPHICS_SWITCHING:
            _glfw.hints.context.nsgl.offline = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_SCALE_TO_MONITOR:
            _glfw.hints.window.scaleToMonitor = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_CENTER_CURSOR:
            _glfw.hints.window.centerCursor = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_FOCUS_ON_SHOW:
            _glfw.hints.window.focusOnShow = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_CLIENT_API:
            _glfw.hints.context.client = value;
            return;
        case GLFW_CONTEXT_CREATION_API:
            _glfw.hints.context.source = value;
            return;
        case GLFW_CONTEXT_VERSION_MAJOR:
            _glfw.hints.context.major = value;
            return;
        case GLFW_CONTEXT_VERSION_MINOR:
            _glfw.hints.context.minor = value;
            return;
        case GLFW_CONTEXT_ROBUSTNESS:
            _glfw.hints.context.robustness = value;
            return;
        case GLFW_OPENGL_FORWARD_COMPAT:
            _glfw.hints.context.forward = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_OPENGL_DEBUG_CONTEXT:
            _glfw.hints.context.debug = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_CONTEXT_NO_ERROR:
            _glfw.hints.context.noerror = value ? GLFW_TRUE : GLFW_FALSE;
            return;
        case GLFW_OPENGL_PROFILE:
            _glfw.hints.context.profile = value;
            return;
        case GLFW_CONTEXT_RELEASE_BEHAVIOR:
            _glfw.hints.context.release = value;
            return;
        case GLFW_REFRESH_RATE:
            _glfw.hints.refreshRate = value;
            return;
    }

    _glfwInputError(GLFW_INVALID_ENUM, "Invalid window hint 0x%08X", hint);
}

GLFWAPI void glfwWindowHintString(int hint, const char* value)
{
    assert(value != NULL);

    _GLFW_REQUIRE_INIT();

    switch (hint)
    {
        case GLFW_COCOA_FRAME_NAME:
            strncpy(_glfw.hints.window.ns.frameName, value,
                    sizeof(_glfw.hints.window.ns.frameName) - 1);
            return;
        case GLFW_X11_CLASS_NAME:
            strncpy(_glfw.hints.window.x11.className, value,
                    sizeof(_glfw.hints.window.x11.className) - 1);
            return;
        case GLFW_X11_INSTANCE_NAME:
            strncpy(_glfw.hints.window.x11.instanceName, value,
                    sizeof(_glfw.hints.window.x11.instanceName) - 1);
            return;
    }

    _glfwInputError(GLFW_INVALID_ENUM, "Invalid window hint string 0x%08X", hint);
}

GLFWAPI void glfwDestroyWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;

    _GLFW_REQUIRE_INIT();

    // Allow closing of NULL (to match the behavior of free)
    if (window == NULL)
        return;

    // Clear all callbacks to avoid exposing a half torn-down window object
    memset(&window->callbacks, 0, sizeof(window->callbacks));

    // The window's context must not be current on another thread when the
    // window is destroyed
    if (window == _glfwPlatformGetTls(&_glfw.contextSlot))
        glfwMakeContextCurrent(NULL);

    _glfwPlatformDestroyWindow(window);

    // Unlink window from global linked list
    {
        _GLFWwindow** prev = &_glfw.windowListHead;

        while (*prev != window)
            prev = &((*prev)->next);

        *prev = window->next;
    }

    free(window);
}

GLFWAPI int glfwWindowShouldClose(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(0);
    return window->shouldClose;
}

GLFWAPI void glfwSetWindowShouldClose(GLFWwindow* handle, int value)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();
    window->shouldClose = value;
}

GLFWAPI void glfwSetWindowTitle(GLFWwindow* handle, const char* title)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);
    assert(title != NULL);

    _GLFW_REQUIRE_INIT();
    _glfwPlatformSetWindowTitle(window, title);
}

GLFWAPI void glfwSetWindowIcon(GLFWwindow* handle,
                               int count, const GLFWimage* images)
{
    int i;
    _GLFWwindow* window = (_GLFWwindow*) handle;

    assert(window != NULL);
    assert(count >= 0);
    assert(count == 0 || images != NULL);

    _GLFW_REQUIRE_INIT();

    if (count < 0)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid image count for window icon");
        return;
    }

    for (i = 0; i < count; i++)
    {
        assert(images[i].pixels != NULL);

        if (images[i].width <= 0 || images[i].height <= 0)
        {
            _glfwInputError(GLFW_INVALID_VALUE,
                            "Invalid image dimensions for window icon");
            return;
        }
    }

    _glfwPlatformSetWindowIcon(window, count, images);
}

GLFWAPI void glfwGetWindowPos(GLFWwindow* handle, int* xpos, int* ypos)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    if (xpos)
        *xpos = 0;
    if (ypos)
        *ypos = 0;

    _GLFW_REQUIRE_INIT();
    _glfwPlatformGetWindowPos(window, xpos, ypos);
}

GLFWAPI void glfwSetWindowPos(GLFWwindow* handle, int xpos, int ypos)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (window->monitor)
        return;

    _glfwPlatformSetWindowPos(window, xpos, ypos);
}

GLFWAPI void glfwGetWindowSize(GLFWwindow* handle, int* width, int* height)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    if (width)
        *width = 0;
    if (height)
        *height = 0;

    _GLFW_REQUIRE_INIT();
    _glfwPlatformGetWindowSize(window, width, height);
}

GLFWAPI void glfwSetWindowSize(GLFWwindow* handle, int width, int height)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);
    assert(width >= 0);
    assert(height >= 0);

    _GLFW_REQUIRE_INIT();

    window->videoMode.width  = width;
    window->videoMode.height = height;

    _glfwPlatformSetWindowSize(window, width, height);
}

GLFWAPI void glfwSetWindowSizeLimits(GLFWwindow* handle,
                                     int minwidth, int minheight,
                                     int maxwidth, int maxheight)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (minwidth != GLFW_DONT_CARE && minheight != GLFW_DONT_CARE)
    {
        if (minwidth < 0 || minheight < 0)
        {
            _glfwInputError(GLFW_INVALID_VALUE,
                            "Invalid window minimum size %ix%i",
                            minwidth, minheight);
            return;
        }
    }

    if (maxwidth != GLFW_DONT_CARE && maxheight != GLFW_DONT_CARE)
    {
        if (maxwidth < 0 || maxheight < 0 ||
            maxwidth < minwidth || maxheight < minheight)
        {
            _glfwInputError(GLFW_INVALID_VALUE,
                            "Invalid window maximum size %ix%i",
                            maxwidth, maxheight);
            return;
        }
    }

    window->minwidth  = minwidth;
    window->minheight = minheight;
    window->maxwidth  = maxwidth;
    window->maxheight = maxheight;

    if (window->monitor || !window->resizable)
        return;

    _glfwPlatformSetWindowSizeLimits(window,
                                     minwidth, minheight,
                                     maxwidth, maxheight);
}

GLFWAPI void glfwSetWindowAspectRatio(GLFWwindow* handle, int numer, int denom)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);
    assert(numer != 0);
    assert(denom != 0);

    _GLFW_REQUIRE_INIT();

    if (numer != GLFW_DONT_CARE && denom != GLFW_DONT_CARE)
    {
        if (numer <= 0 || denom <= 0)
        {
            _glfwInputError(GLFW_INVALID_VALUE,
                            "Invalid window aspect ratio %i:%i",
                            numer, denom);
            return;
        }
    }

    window->numer = numer;
    window->denom = denom;

    if (window->monitor || !window->resizable)
        return;

    _glfwPlatformSetWindowAspectRatio(window, numer, denom);
}

GLFWAPI void glfwGetFramebufferSize(GLFWwindow* handle, int* width, int* height)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    if (width)
        *width = 0;
    if (height)
        *height = 0;

    _GLFW_REQUIRE_INIT();
    _glfwPlatformGetFramebufferSize(window, width, height);
}

GLFWAPI void glfwGetWindowFrameSize(GLFWwindow* handle,
                                    int* left, int* top,
                                    int* right, int* bottom)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    if (left)
        *left = 0;
    if (top)
        *top = 0;
    if (right)
        *right = 0;
    if (bottom)
        *bottom = 0;

    _GLFW_REQUIRE_INIT();
    _glfwPlatformGetWindowFrameSize(window, left, top, right, bottom);
}

GLFWAPI void glfwGetWindowContentScale(GLFWwindow* handle,
                                       float* xscale, float* yscale)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    if (xscale)
        *xscale = 0.f;
    if (yscale)
        *yscale = 0.f;

    _GLFW_REQUIRE_INIT();
    _glfwPlatformGetWindowContentScale(window, xscale, yscale);
}

GLFWAPI float glfwGetWindowOpacity(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(1.f);
    return _glfwPlatformGetWindowOpacity(window);
}

GLFWAPI void glfwSetWindowOpacity(GLFWwindow* handle, float opacity)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);
    assert(opacity == opacity);
    assert(opacity >= 0.f);
    assert(opacity <= 1.f);

    _GLFW_REQUIRE_INIT();

    if (opacity != opacity || opacity < 0.f || opacity > 1.f)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid window opacity %f", opacity);
        return;
    }

    _glfwPlatformSetWindowOpacity(window, opacity);
}

GLFWAPI void glfwIconifyWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();
    _glfwPlatformIconifyWindow(window);
}

GLFWAPI void glfwRestoreWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();
    _glfwPlatformRestoreWindow(window);
}

GLFWAPI void glfwMaximizeWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (window->monitor)
        return;

    _glfwPlatformMaximizeWindow(window);
}

GLFWAPI void glfwShowWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (window->monitor)
        return;

    _glfwPlatformShowWindow(window);

    if (window->focusOnShow)
        _glfwPlatformFocusWindow(window);
}

GLFWAPI void glfwRequestWindowAttention(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    _glfwPlatformRequestWindowAttention(window);
}

GLFWAPI void glfwHideWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    if (window->monitor)
        return;

    _glfwPlatformHideWindow(window);
}

GLFWAPI void glfwFocusWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    _glfwPlatformFocusWindow(window);
}

GLFWAPI int glfwGetWindowAttrib(GLFWwindow* handle, int attrib)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(0);

    switch (attrib)
    {
        case GLFW_FOCUSED:
            return _glfwPlatformWindowFocused(window);
        case GLFW_ICONIFIED:
            return _glfwPlatformWindowIconified(window);
        case GLFW_VISIBLE:
            return _glfwPlatformWindowVisible(window);
        case GLFW_MAXIMIZED:
            return _glfwPlatformWindowMaximized(window);
        case GLFW_HOVERED:
            return _glfwPlatformWindowHovered(window);
        case GLFW_FOCUS_ON_SHOW:
            return window->focusOnShow;
        case GLFW_TRANSPARENT_FRAMEBUFFER:
            return _glfwPlatformFramebufferTransparent(window);
        case GLFW_RESIZABLE:
            return window->resizable;
        case GLFW_DECORATED:
            return window->decorated;
        case GLFW_FLOATING:
            return window->floating;
        case GLFW_AUTO_ICONIFY:
            return window->autoIconify;
        case GLFW_CLIENT_API:
            return window->context.client;
        case GLFW_CONTEXT_CREATION_API:
            return window->context.source;
        case GLFW_CONTEXT_VERSION_MAJOR:
            return window->context.major;
        case GLFW_CONTEXT_VERSION_MINOR:
            return window->context.minor;
        case GLFW_CONTEXT_REVISION:
            return window->context.revision;
        case GLFW_CONTEXT_ROBUSTNESS:
            return window->context.robustness;
        case GLFW_OPENGL_FORWARD_COMPAT:
            return window->context.forward;
        case GLFW_OPENGL_DEBUG_CONTEXT:
            return window->context.debug;
        case GLFW_OPENGL_PROFILE:
            return window->context.profile;
        case GLFW_CONTEXT_RELEASE_BEHAVIOR:
            return window->context.release;
        case GLFW_CONTEXT_NO_ERROR:
            return window->context.noerror;
    }

    _glfwInputError(GLFW_INVALID_ENUM, "Invalid window attribute 0x%08X", attrib);
    return 0;
}

GLFWAPI void glfwSetWindowAttrib(GLFWwindow* handle, int attrib, int value)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();

    value = value ? GLFW_TRUE : GLFW_FALSE;

    if (attrib == GLFW_AUTO_ICONIFY)
        window->autoIconify = value;
    else if (attrib == GLFW_RESIZABLE)
    {
        if (window->resizable == value)
            return;

        window->resizable = value;
        if (!window->monitor)
            _glfwPlatformSetWindowResizable(window, value);
    }
    else if (attrib == GLFW_DECORATED)
    {
        if (window->decorated == value)
            return;

        window->decorated = value;
        if (!window->monitor)
            _glfwPlatformSetWindowDecorated(window, value);
    }
    else if (attrib == GLFW_FLOATING)
    {
        if (window->floating == value)
            return;

        window->floating = value;
        if (!window->monitor)
            _glfwPlatformSetWindowFloating(window, value);
    }
    else if (attrib == GLFW_FOCUS_ON_SHOW)
        window->focusOnShow = value;
    else
        _glfwInputError(GLFW_INVALID_ENUM, "Invalid window attribute 0x%08X", attrib);
}

GLFWAPI GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return (GLFWmonitor*) window->monitor;
}

GLFWAPI void glfwSetWindowMonitor(GLFWwindow* wh,
                                  GLFWmonitor* mh,
                                  int xpos, int ypos,
                                  int width, int height,
                                  int refreshRate)
{
    _GLFWwindow* window = (_GLFWwindow*) wh;
    _GLFWmonitor* monitor = (_GLFWmonitor*) mh;
    assert(window != NULL);
    assert(width >= 0);
    assert(height >= 0);

    _GLFW_REQUIRE_INIT();

    if (width <= 0 || height <= 0)
    {
        _glfwInputError(GLFW_INVALID_VALUE,
                        "Invalid window size %ix%i",
                        width, height);
        return;
    }

    if (refreshRate < 0 && refreshRate != GLFW_DONT_CARE)
    {
        _glfwInputError(GLFW_INVALID_VALUE,
                        "Invalid refresh rate %i",
                        refreshRate);
        return;
    }

    window->videoMode.width       = width;
    window->videoMode.height      = height;
    window->videoMode.refreshRate = refreshRate;

    _glfwPlatformSetWindowMonitor(window, monitor,
                                  xpos, ypos, width, height,
                                  refreshRate);
}

GLFWAPI void glfwSetWindowUserPointer(GLFWwindow* handle, void* pointer)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT();
    window->userPointer = pointer;
}

GLFWAPI void* glfwGetWindowUserPointer(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return window->userPointer;
}

GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* handle,
                                                  GLFWwindowposfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.pos, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* handle,
                                                    GLFWwindowsizefun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.size, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* handle,
                                                      GLFWwindowclosefun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.close, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* handle,
                                                          GLFWwindowrefreshfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.refresh, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* handle,
                                                      GLFWwindowfocusfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.focus, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* handle,
                                                          GLFWwindowiconifyfun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.iconify, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowmaximizefun glfwSetWindowMaximizeCallback(GLFWwindow* handle,
                                                            GLFWwindowmaximizefun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.maximize, cbfun);
    return cbfun;
}

GLFWAPI GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* handle,
                                                              GLFWframebuffersizefun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.fbsize, cbfun);
    return cbfun;
}

GLFWAPI GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* handle,
                                                                    GLFWwindowcontentscalefun cbfun)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    _GLFW_SWAP_POINTERS(window->callbacks.scale, cbfun);
    return cbfun;
}

GLFWAPI void glfwPollEvents(void)
{
    _GLFW_REQUIRE_INIT();
    _glfwPlatformPollEvents();
}

GLFWAPI void glfwWaitEvents(void)
{
    _GLFW_REQUIRE_INIT();
    _glfwPlatformWaitEvents();
}

GLFWAPI void glfwWaitEventsTimeout(double timeout)
{
    _GLFW_REQUIRE_INIT();
    assert(timeout == timeout);
    assert(timeout >= 0.0);
    assert(timeout <= DBL_MAX);

    if (timeout != timeout || timeout < 0.0 || timeout > DBL_MAX)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid time %f", timeout);
        return;
    }

    _glfwPlatformWaitEventsTimeout(timeout);
}

GLFWAPI void glfwPostEmptyEvent(void)
{
    _GLFW_REQUIRE_INIT();
    _glfwPlatformPostEmptyEvent();
}

#ifdef OS_LINUX
//========================================================================
// GLFW 3.3 X11 - www.glfw.org
// GLFW 3.3 - www.glfw.org - x11_init.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================

// Translate the X11 KeySyms for a key to a GLFW key code
// NOTE: This is only used as a fallback, in case the XKB method fails
//       It is layout-dependent and will fail partially on most non-US layouts
//
static int translateKeySyms(const KeySym* keysyms, int width)
{
    if (width > 1)
    {
        switch (keysyms[1])
        {
            case XK_KP_0:           return GLFW_KEY_KP_0;
            case XK_KP_1:           return GLFW_KEY_KP_1;
            case XK_KP_2:           return GLFW_KEY_KP_2;
            case XK_KP_3:           return GLFW_KEY_KP_3;
            case XK_KP_4:           return GLFW_KEY_KP_4;
            case XK_KP_5:           return GLFW_KEY_KP_5;
            case XK_KP_6:           return GLFW_KEY_KP_6;
            case XK_KP_7:           return GLFW_KEY_KP_7;
            case XK_KP_8:           return GLFW_KEY_KP_8;
            case XK_KP_9:           return GLFW_KEY_KP_9;
            case XK_KP_Separator:
            case XK_KP_Decimal:     return GLFW_KEY_KP_DECIMAL;
            case XK_KP_Equal:       return GLFW_KEY_KP_EQUAL;
            case XK_KP_Enter:       return GLFW_KEY_KP_ENTER;
            default:                break;
        }
    }

    switch (keysyms[0])
    {
        case XK_Escape:         return GLFW_KEY_ESCAPE;
        case XK_Tab:            return GLFW_KEY_TAB;
        case XK_Shift_L:        return GLFW_KEY_LEFT_SHIFT;
        case XK_Shift_R:        return GLFW_KEY_RIGHT_SHIFT;
        case XK_Control_L:      return GLFW_KEY_LEFT_CONTROL;
        case XK_Control_R:      return GLFW_KEY_RIGHT_CONTROL;
        case XK_Meta_L:
        case XK_Alt_L:          return GLFW_KEY_LEFT_ALT;
        case XK_Mode_switch: // Mapped to Alt_R on many keyboards
        case XK_ISO_Level3_Shift: // AltGr on at least some machines
        case XK_Meta_R:
        case XK_Alt_R:          return GLFW_KEY_RIGHT_ALT;
        case XK_Super_L:        return GLFW_KEY_LEFT_SUPER;
        case XK_Super_R:        return GLFW_KEY_RIGHT_SUPER;
        case XK_Menu:           return GLFW_KEY_MENU;
        case XK_Num_Lock:       return GLFW_KEY_NUM_LOCK;
        case XK_Caps_Lock:      return GLFW_KEY_CAPS_LOCK;
        case XK_Print:          return GLFW_KEY_PRINT_SCREEN;
        case XK_Scroll_Lock:    return GLFW_KEY_SCROLL_LOCK;
        case XK_Pause:          return GLFW_KEY_PAUSE;
        case XK_Delete:         return GLFW_KEY_DELETE;
        case XK_BackSpace:      return GLFW_KEY_BACKSPACE;
        case XK_Return:         return GLFW_KEY_ENTER;
        case XK_Home:           return GLFW_KEY_HOME;
        case XK_End:            return GLFW_KEY_END;
        case XK_Page_Up:        return GLFW_KEY_PAGE_UP;
        case XK_Page_Down:      return GLFW_KEY_PAGE_DOWN;
        case XK_Insert:         return GLFW_KEY_INSERT;
        case XK_Left:           return GLFW_KEY_LEFT;
        case XK_Right:          return GLFW_KEY_RIGHT;
        case XK_Down:           return GLFW_KEY_DOWN;
        case XK_Up:             return GLFW_KEY_UP;
        case XK_F1:             return GLFW_KEY_F1;
        case XK_F2:             return GLFW_KEY_F2;
        case XK_F3:             return GLFW_KEY_F3;
        case XK_F4:             return GLFW_KEY_F4;
        case XK_F5:             return GLFW_KEY_F5;
        case XK_F6:             return GLFW_KEY_F6;
        case XK_F7:             return GLFW_KEY_F7;
        case XK_F8:             return GLFW_KEY_F8;
        case XK_F9:             return GLFW_KEY_F9;
        case XK_F10:            return GLFW_KEY_F10;
        case XK_F11:            return GLFW_KEY_F11;
        case XK_F12:            return GLFW_KEY_F12;
        case XK_F13:            return GLFW_KEY_F13;
        case XK_F14:            return GLFW_KEY_F14;
        case XK_F15:            return GLFW_KEY_F15;
        case XK_F16:            return GLFW_KEY_F16;
        case XK_F17:            return GLFW_KEY_F17;
        case XK_F18:            return GLFW_KEY_F18;
        case XK_F19:            return GLFW_KEY_F19;
        case XK_F20:            return GLFW_KEY_F20;
        case XK_F21:            return GLFW_KEY_F21;
        case XK_F22:            return GLFW_KEY_F22;
        case XK_F23:            return GLFW_KEY_F23;
        case XK_F24:            return GLFW_KEY_F24;
        case XK_F25:            return GLFW_KEY_F25;

        // Numeric keypad
        case XK_KP_Divide:      return GLFW_KEY_KP_DIVIDE;
        case XK_KP_Multiply:    return GLFW_KEY_KP_MULTIPLY;
        case XK_KP_Subtract:    return GLFW_KEY_KP_SUBTRACT;
        case XK_KP_Add:         return GLFW_KEY_KP_ADD;

        // These should have been detected in secondary keysym test above!
        case XK_KP_Insert:      return GLFW_KEY_KP_0;
        case XK_KP_End:         return GLFW_KEY_KP_1;
        case XK_KP_Down:        return GLFW_KEY_KP_2;
        case XK_KP_Page_Down:   return GLFW_KEY_KP_3;
        case XK_KP_Left:        return GLFW_KEY_KP_4;
        case XK_KP_Right:       return GLFW_KEY_KP_6;
        case XK_KP_Home:        return GLFW_KEY_KP_7;
        case XK_KP_Up:          return GLFW_KEY_KP_8;
        case XK_KP_Page_Up:     return GLFW_KEY_KP_9;
        case XK_KP_Delete:      return GLFW_KEY_KP_DECIMAL;
        case XK_KP_Equal:       return GLFW_KEY_KP_EQUAL;
        case XK_KP_Enter:       return GLFW_KEY_KP_ENTER;

        // Last resort: Check for printable keys (should not happen if the XKB
        // extension is available). This will give a layout dependent mapping
        // (which is wrong, and we may miss some keys, especially on non-US
        // keyboards), but it's better than nothing...
        case XK_a:              return GLFW_KEY_A;
        case XK_b:              return GLFW_KEY_B;
        case XK_c:              return GLFW_KEY_C;
        case XK_d:              return GLFW_KEY_D;
        case XK_e:              return GLFW_KEY_E;
        case XK_f:              return GLFW_KEY_F;
        case XK_g:              return GLFW_KEY_G;
        case XK_h:              return GLFW_KEY_H;
        case XK_i:              return GLFW_KEY_I;
        case XK_j:              return GLFW_KEY_J;
        case XK_k:              return GLFW_KEY_K;
        case XK_l:              return GLFW_KEY_L;
        case XK_m:              return GLFW_KEY_M;
        case XK_n:              return GLFW_KEY_N;
        case XK_o:              return GLFW_KEY_O;
        case XK_p:              return GLFW_KEY_P;
        case XK_q:              return GLFW_KEY_Q;
        case XK_r:              return GLFW_KEY_R;
        case XK_s:              return GLFW_KEY_S;
        case XK_t:              return GLFW_KEY_T;
        case XK_u:              return GLFW_KEY_U;
        case XK_v:              return GLFW_KEY_V;
        case XK_w:              return GLFW_KEY_W;
        case XK_x:              return GLFW_KEY_X;
        case XK_y:              return GLFW_KEY_Y;
        case XK_z:              return GLFW_KEY_Z;
        case XK_1:              return GLFW_KEY_1;
        case XK_2:              return GLFW_KEY_2;
        case XK_3:              return GLFW_KEY_3;
        case XK_4:              return GLFW_KEY_4;
        case XK_5:              return GLFW_KEY_5;
        case XK_6:              return GLFW_KEY_6;
        case XK_7:              return GLFW_KEY_7;
        case XK_8:              return GLFW_KEY_8;
        case XK_9:              return GLFW_KEY_9;
        case XK_0:              return GLFW_KEY_0;
        case XK_space:          return GLFW_KEY_SPACE;
        case XK_minus:          return GLFW_KEY_MINUS;
        case XK_equal:          return GLFW_KEY_EQUAL;
        case XK_bracketleft:    return GLFW_KEY_LEFT_BRACKET;
        case XK_bracketright:   return GLFW_KEY_RIGHT_BRACKET;
        case XK_backslash:      return GLFW_KEY_BACKSLASH;
        case XK_semicolon:      return GLFW_KEY_SEMICOLON;
        case XK_apostrophe:     return GLFW_KEY_APOSTROPHE;
        case XK_grave:          return GLFW_KEY_GRAVE_ACCENT;
        case XK_comma:          return GLFW_KEY_COMMA;
        case XK_period:         return GLFW_KEY_PERIOD;
        case XK_slash:          return GLFW_KEY_SLASH;
        case XK_less:           return GLFW_KEY_WORLD_1; // At least in some layouts...
        default:                break;
    }

    // No matching translation was found
    return GLFW_KEY_UNKNOWN;
}

// Create key code translation tables
//
static void createKeyTables(void)
{
    int scancode, scancodeMin, scancodeMax;

    memset(_glfw.x11.keycodes, -1, sizeof(_glfw.x11.keycodes));
    memset(_glfw.x11.scancodes, -1, sizeof(_glfw.x11.scancodes));

    if (_glfw.x11.xkb.available)
    {
        // Use XKB to determine physical key locations independently of the
        // current keyboard layout

        XkbDescPtr desc = XkbGetMap(_glfw.x11.display, 0, XkbUseCoreKbd);
        XkbGetNames(_glfw.x11.display, XkbKeyNamesMask | XkbKeyAliasesMask, desc);

        scancodeMin = desc->min_key_code;
        scancodeMax = desc->max_key_code;

        const struct
        {
            int key;
            char* name;
        } keymap[] =
        {
            { GLFW_KEY_GRAVE_ACCENT, "TLDE" },
            { GLFW_KEY_1, "AE01" },
            { GLFW_KEY_2, "AE02" },
            { GLFW_KEY_3, "AE03" },
            { GLFW_KEY_4, "AE04" },
            { GLFW_KEY_5, "AE05" },
            { GLFW_KEY_6, "AE06" },
            { GLFW_KEY_7, "AE07" },
            { GLFW_KEY_8, "AE08" },
            { GLFW_KEY_9, "AE09" },
            { GLFW_KEY_0, "AE10" },
            { GLFW_KEY_MINUS, "AE11" },
            { GLFW_KEY_EQUAL, "AE12" },
            { GLFW_KEY_Q, "AD01" },
            { GLFW_KEY_W, "AD02" },
            { GLFW_KEY_E, "AD03" },
            { GLFW_KEY_R, "AD04" },
            { GLFW_KEY_T, "AD05" },
            { GLFW_KEY_Y, "AD06" },
            { GLFW_KEY_U, "AD07" },
            { GLFW_KEY_I, "AD08" },
            { GLFW_KEY_O, "AD09" },
            { GLFW_KEY_P, "AD10" },
            { GLFW_KEY_LEFT_BRACKET, "AD11" },
            { GLFW_KEY_RIGHT_BRACKET, "AD12" },
            { GLFW_KEY_A, "AC01" },
            { GLFW_KEY_S, "AC02" },
            { GLFW_KEY_D, "AC03" },
            { GLFW_KEY_F, "AC04" },
            { GLFW_KEY_G, "AC05" },
            { GLFW_KEY_H, "AC06" },
            { GLFW_KEY_J, "AC07" },
            { GLFW_KEY_K, "AC08" },
            { GLFW_KEY_L, "AC09" },
            { GLFW_KEY_SEMICOLON, "AC10" },
            { GLFW_KEY_APOSTROPHE, "AC11" },
            { GLFW_KEY_Z, "AB01" },
            { GLFW_KEY_X, "AB02" },
            { GLFW_KEY_C, "AB03" },
            { GLFW_KEY_V, "AB04" },
            { GLFW_KEY_B, "AB05" },
            { GLFW_KEY_N, "AB06" },
            { GLFW_KEY_M, "AB07" },
            { GLFW_KEY_COMMA, "AB08" },
            { GLFW_KEY_PERIOD, "AB09" },
            { GLFW_KEY_SLASH, "AB10" },
            { GLFW_KEY_BACKSLASH, "BKSL" },
            { GLFW_KEY_WORLD_1, "LSGT" },
            { GLFW_KEY_SPACE, "SPCE" },
            { GLFW_KEY_ESCAPE, "ESC" },
            { GLFW_KEY_ENTER, "RTRN" },
            { GLFW_KEY_TAB, "TAB" },
            { GLFW_KEY_BACKSPACE, "BKSP" },
            { GLFW_KEY_INSERT, "INS" },
            { GLFW_KEY_DELETE, "DELE" },
            { GLFW_KEY_RIGHT, "RGHT" },
            { GLFW_KEY_LEFT, "LEFT" },
            { GLFW_KEY_DOWN, "DOWN" },
            { GLFW_KEY_UP, "UP" },
            { GLFW_KEY_PAGE_UP, "PGUP" },
            { GLFW_KEY_PAGE_DOWN, "PGDN" },
            { GLFW_KEY_HOME, "HOME" },
            { GLFW_KEY_END, "END" },
            { GLFW_KEY_CAPS_LOCK, "CAPS" },
            { GLFW_KEY_SCROLL_LOCK, "SCLK" },
            { GLFW_KEY_NUM_LOCK, "NMLK" },
            { GLFW_KEY_PRINT_SCREEN, "PRSC" },
            { GLFW_KEY_PAUSE, "PAUS" },
            { GLFW_KEY_F1, "FK01" },
            { GLFW_KEY_F2, "FK02" },
            { GLFW_KEY_F3, "FK03" },
            { GLFW_KEY_F4, "FK04" },
            { GLFW_KEY_F5, "FK05" },
            { GLFW_KEY_F6, "FK06" },
            { GLFW_KEY_F7, "FK07" },
            { GLFW_KEY_F8, "FK08" },
            { GLFW_KEY_F9, "FK09" },
            { GLFW_KEY_F10, "FK10" },
            { GLFW_KEY_F11, "FK11" },
            { GLFW_KEY_F12, "FK12" },
            { GLFW_KEY_F13, "FK13" },
            { GLFW_KEY_F14, "FK14" },
            { GLFW_KEY_F15, "FK15" },
            { GLFW_KEY_F16, "FK16" },
            { GLFW_KEY_F17, "FK17" },
            { GLFW_KEY_F18, "FK18" },
            { GLFW_KEY_F19, "FK19" },
            { GLFW_KEY_F20, "FK20" },
            { GLFW_KEY_F21, "FK21" },
            { GLFW_KEY_F22, "FK22" },
            { GLFW_KEY_F23, "FK23" },
            { GLFW_KEY_F24, "FK24" },
            { GLFW_KEY_F25, "FK25" },
            { GLFW_KEY_KP_0, "KP0" },
            { GLFW_KEY_KP_1, "KP1" },
            { GLFW_KEY_KP_2, "KP2" },
            { GLFW_KEY_KP_3, "KP3" },
            { GLFW_KEY_KP_4, "KP4" },
            { GLFW_KEY_KP_5, "KP5" },
            { GLFW_KEY_KP_6, "KP6" },
            { GLFW_KEY_KP_7, "KP7" },
            { GLFW_KEY_KP_8, "KP8" },
            { GLFW_KEY_KP_9, "KP9" },
            { GLFW_KEY_KP_DECIMAL, "KPDL" },
            { GLFW_KEY_KP_DIVIDE, "KPDV" },
            { GLFW_KEY_KP_MULTIPLY, "KPMU" },
            { GLFW_KEY_KP_SUBTRACT, "KPSU" },
            { GLFW_KEY_KP_ADD, "KPAD" },
            { GLFW_KEY_KP_ENTER, "KPEN" },
            { GLFW_KEY_KP_EQUAL, "KPEQ" },
            { GLFW_KEY_LEFT_SHIFT, "LFSH" },
            { GLFW_KEY_LEFT_CONTROL, "LCTL" },
            { GLFW_KEY_LEFT_ALT, "LALT" },
            { GLFW_KEY_LEFT_SUPER, "LWIN" },
            { GLFW_KEY_RIGHT_SHIFT, "RTSH" },
            { GLFW_KEY_RIGHT_CONTROL, "RCTL" },
            { GLFW_KEY_RIGHT_ALT, "RALT" },
            { GLFW_KEY_RIGHT_ALT, "LVL3" },
            { GLFW_KEY_RIGHT_ALT, "MDSW" },
            { GLFW_KEY_RIGHT_SUPER, "RWIN" },
            { GLFW_KEY_MENU, "MENU" }
        };

        // Find the X11 key code -> GLFW key code mapping
        for (scancode = scancodeMin;  scancode <= scancodeMax;  scancode++)
        {
            int key = GLFW_KEY_UNKNOWN;

            // Map the key name to a GLFW key code. Note: We use the US
            // keyboard layout. Because function keys aren't mapped correctly
            // when using traditional KeySym translations, they are mapped
            // here instead.
            for (int i = 0;  i < sizeof(keymap) / sizeof(keymap[0]);  i++)
            {
                if (strncmp(desc->names->keys[scancode].name,
                            keymap[i].name,
                            XkbKeyNameLength) == 0)
                {
                    key = keymap[i].key;
                    break;
                }
            }

            // Fall back to key aliases in case the key name did not match
            for (int i = 0;  i < desc->names->num_key_aliases;  i++)
            {
                if (key != GLFW_KEY_UNKNOWN)
                    break;

                if (strncmp(desc->names->key_aliases[i].real,
                            desc->names->keys[scancode].name,
                            XkbKeyNameLength) != 0)
                {
                    continue;
                }

                for (int j = 0;  j < sizeof(keymap) / sizeof(keymap[0]);  j++)
                {
                    if (strncmp(desc->names->key_aliases[i].alias,
                                keymap[j].name,
                                XkbKeyNameLength) == 0)
                    {
                        key = keymap[j].key;
                        break;
                    }
                }
            }

            _glfw.x11.keycodes[scancode] = key;
        }

        XkbFreeNames(desc, XkbKeyNamesMask, True);
        XkbFreeKeyboard(desc, 0, True);
    }
    else
        XDisplayKeycodes(_glfw.x11.display, &scancodeMin, &scancodeMax);

    int width;
    KeySym* keysyms = XGetKeyboardMapping(_glfw.x11.display,
                                          scancodeMin,
                                          scancodeMax - scancodeMin + 1,
                                          &width);

    for (scancode = scancodeMin;  scancode <= scancodeMax;  scancode++)
    {
        // Translate the un-translated key codes using traditional X11 KeySym
        // lookups
        if (_glfw.x11.keycodes[scancode] < 0)
        {
            const size_t base = (scancode - scancodeMin) * width;
            _glfw.x11.keycodes[scancode] = translateKeySyms(&keysyms[base], width);
        }

        // Store the reverse translation for faster key name lookup
        if (_glfw.x11.keycodes[scancode] > 0)
            _glfw.x11.scancodes[_glfw.x11.keycodes[scancode]] = scancode;
    }

    XFree(keysyms);
}

// Check whether the IM has a usable style
//
static GLFWbool hasUsableInputMethodStyle(void)
{
    GLFWbool found = GLFW_FALSE;
    XIMStyles* styles = NULL;

    if (XGetIMValues(_glfw.x11.im, XNQueryInputStyle, &styles, NULL) != NULL)
        return GLFW_FALSE;

    for (unsigned int i = 0;  i < styles->count_styles;  i++)
    {
        if (styles->supported_styles[i] == (XIMPreeditNothing | XIMStatusNothing))
        {
            found = GLFW_TRUE;
            break;
        }
    }

    XFree(styles);
    return found;
}

// Check whether the specified atom is supported
//
static Atom getAtomIfSupported(Atom* supportedAtoms,
                               unsigned long atomCount,
                               const char* atomName)
{
    const Atom atom = XInternAtom(_glfw.x11.display, atomName, False);

    for (unsigned long i = 0;  i < atomCount;  i++)
    {
        if (supportedAtoms[i] == atom)
            return atom;
    }

    return None;
}

// Check whether the running window manager is EWMH-compliant
//
static void detectEWMH(void)
{
    // First we read the _NET_SUPPORTING_WM_CHECK property on the root window

    Window* windowFromRoot = NULL;
    if (!_glfwGetWindowPropertyX11(_glfw.x11.root,
                                   _glfw.x11.NET_SUPPORTING_WM_CHECK,
                                   XA_WINDOW,
                                   (unsigned char**) &windowFromRoot))
    {
        return;
    }

    _glfwGrabErrorHandlerX11();

    // If it exists, it should be the XID of a top-level window
    // Then we look for the same property on that window

    Window* windowFromChild = NULL;
    if (!_glfwGetWindowPropertyX11(*windowFromRoot,
                                   _glfw.x11.NET_SUPPORTING_WM_CHECK,
                                   XA_WINDOW,
                                   (unsigned char**) &windowFromChild))
    {
        XFree(windowFromRoot);
        return;
    }

    _glfwReleaseErrorHandlerX11();

    // If the property exists, it should contain the XID of the window

    if (*windowFromRoot != *windowFromChild)
    {
        XFree(windowFromRoot);
        XFree(windowFromChild);
        return;
    }

    XFree(windowFromRoot);
    XFree(windowFromChild);

    // We are now fairly sure that an EWMH-compliant WM is currently running
    // We can now start querying the WM about what features it supports by
    // looking in the _NET_SUPPORTED property on the root window
    // It should contain a list of supported EWMH protocol and state atoms

    Atom* supportedAtoms = NULL;
    const unsigned long atomCount =
        _glfwGetWindowPropertyX11(_glfw.x11.root,
                                  _glfw.x11.NET_SUPPORTED,
                                  XA_ATOM,
                                  (unsigned char**) &supportedAtoms);

    // See which of the atoms we support that are supported by the WM

    _glfw.x11.NET_WM_STATE =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE");
    _glfw.x11.NET_WM_STATE_ABOVE =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_ABOVE");
    _glfw.x11.NET_WM_STATE_FULLSCREEN =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_FULLSCREEN");
    _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_VERT");
    _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_HORZ");
    _glfw.x11.NET_WM_STATE_DEMANDS_ATTENTION =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_STATE_DEMANDS_ATTENTION");
    _glfw.x11.NET_WM_FULLSCREEN_MONITORS =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_FULLSCREEN_MONITORS");
    _glfw.x11.NET_WM_WINDOW_TYPE =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE");
    _glfw.x11.NET_WM_WINDOW_TYPE_NORMAL =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE_NORMAL");
    _glfw.x11.NET_WORKAREA =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_WORKAREA");
    _glfw.x11.NET_CURRENT_DESKTOP =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_CURRENT_DESKTOP");
    _glfw.x11.NET_ACTIVE_WINDOW =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_ACTIVE_WINDOW");
    _glfw.x11.NET_FRAME_EXTENTS =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_FRAME_EXTENTS");
    _glfw.x11.NET_REQUEST_FRAME_EXTENTS =
        getAtomIfSupported(supportedAtoms, atomCount, "_NET_REQUEST_FRAME_EXTENTS");

    if (supportedAtoms)
        XFree(supportedAtoms);
}

// Look for and initialize supported X11 extensions
//
static GLFWbool initExtensions(void)
{
#if defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.vidmode.handle = _glfw_dlopen("libXxf86vm.so");
#else
    _glfw.x11.vidmode.handle = _glfw_dlopen("libXxf86vm.so.1");
#endif
    if (_glfw.x11.vidmode.handle)
    {
        _glfw.x11.vidmode.QueryExtension = (PFN_XF86VidModeQueryExtension)
            _glfw_dlsym(_glfw.x11.vidmode.handle, "XF86VidModeQueryExtension");
        _glfw.x11.vidmode.GetGammaRamp = (PFN_XF86VidModeGetGammaRamp)
            _glfw_dlsym(_glfw.x11.vidmode.handle, "XF86VidModeGetGammaRamp");
        _glfw.x11.vidmode.SetGammaRamp = (PFN_XF86VidModeSetGammaRamp)
            _glfw_dlsym(_glfw.x11.vidmode.handle, "XF86VidModeSetGammaRamp");
        _glfw.x11.vidmode.GetGammaRampSize = (PFN_XF86VidModeGetGammaRampSize)
            _glfw_dlsym(_glfw.x11.vidmode.handle, "XF86VidModeGetGammaRampSize");

        _glfw.x11.vidmode.available =
            XF86VidModeQueryExtension(_glfw.x11.display,
                                      &_glfw.x11.vidmode.eventBase,
                                      &_glfw.x11.vidmode.errorBase);
    }

#if defined(__CYGWIN__)
    _glfw.x11.xi.handle = _glfw_dlopen("libXi-6.so");
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.xi.handle = _glfw_dlopen("libXi.so");
#else
    _glfw.x11.xi.handle = _glfw_dlopen("libXi.so.6");
#endif
    if (_glfw.x11.xi.handle)
    {
        _glfw.x11.xi.QueryVersion = (PFN_XIQueryVersion)
            _glfw_dlsym(_glfw.x11.xi.handle, "XIQueryVersion");
        _glfw.x11.xi.SelectEvents = (PFN_XISelectEvents)
            _glfw_dlsym(_glfw.x11.xi.handle, "XISelectEvents");

        if (XQueryExtension(_glfw.x11.display,
                            "XInputExtension",
                            &_glfw.x11.xi.majorOpcode,
                            &_glfw.x11.xi.eventBase,
                            &_glfw.x11.xi.errorBase))
        {
            _glfw.x11.xi.major = 2;
            _glfw.x11.xi.minor = 0;

            if (XIQueryVersion(_glfw.x11.display,
                               &_glfw.x11.xi.major,
                               &_glfw.x11.xi.minor) == Success)
            {
                _glfw.x11.xi.available = GLFW_TRUE;
            }
        }
    }

#if defined(__CYGWIN__)
    _glfw.x11.randr.handle = _glfw_dlopen("libXrandr-2.so");
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.randr.handle = _glfw_dlopen("libXrandr.so");
#else
    _glfw.x11.randr.handle = _glfw_dlopen("libXrandr.so.2");
#endif
    if (_glfw.x11.randr.handle)
    {
        _glfw.x11.randr.AllocGamma = (PFN_XRRAllocGamma)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRAllocGamma");
        _glfw.x11.randr.FreeGamma = (PFN_XRRFreeGamma)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRFreeGamma");
        _glfw.x11.randr.FreeCrtcInfo = (PFN_XRRFreeCrtcInfo)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRFreeCrtcInfo");
        _glfw.x11.randr.FreeGamma = (PFN_XRRFreeGamma)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRFreeGamma");
        _glfw.x11.randr.FreeOutputInfo = (PFN_XRRFreeOutputInfo)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRFreeOutputInfo");
        _glfw.x11.randr.FreeScreenResources = (PFN_XRRFreeScreenResources)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRFreeScreenResources");
        _glfw.x11.randr.GetCrtcGamma = (PFN_XRRGetCrtcGamma)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRGetCrtcGamma");
        _glfw.x11.randr.GetCrtcGammaSize = (PFN_XRRGetCrtcGammaSize)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRGetCrtcGammaSize");
        _glfw.x11.randr.GetCrtcInfo = (PFN_XRRGetCrtcInfo)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRGetCrtcInfo");
        _glfw.x11.randr.GetOutputInfo = (PFN_XRRGetOutputInfo)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRGetOutputInfo");
        _glfw.x11.randr.GetOutputPrimary = (PFN_XRRGetOutputPrimary)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRGetOutputPrimary");
        _glfw.x11.randr.GetScreenResourcesCurrent = (PFN_XRRGetScreenResourcesCurrent)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRGetScreenResourcesCurrent");
        _glfw.x11.randr.QueryExtension = (PFN_XRRQueryExtension)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRQueryExtension");
        _glfw.x11.randr.QueryVersion = (PFN_XRRQueryVersion)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRQueryVersion");
        _glfw.x11.randr.SelectInput = (PFN_XRRSelectInput)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRSelectInput");
        _glfw.x11.randr.SetCrtcConfig = (PFN_XRRSetCrtcConfig)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRSetCrtcConfig");
        _glfw.x11.randr.SetCrtcGamma = (PFN_XRRSetCrtcGamma)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRSetCrtcGamma");
        _glfw.x11.randr.UpdateConfiguration = (PFN_XRRUpdateConfiguration)
            _glfw_dlsym(_glfw.x11.randr.handle, "XRRUpdateConfiguration");

        if (XRRQueryExtension(_glfw.x11.display,
                              &_glfw.x11.randr.eventBase,
                              &_glfw.x11.randr.errorBase))
        {
            if (XRRQueryVersion(_glfw.x11.display,
                                &_glfw.x11.randr.major,
                                &_glfw.x11.randr.minor))
            {
                // The GLFW RandR path requires at least version 1.3
                if (_glfw.x11.randr.major > 1 || _glfw.x11.randr.minor >= 3)
                    _glfw.x11.randr.available = GLFW_TRUE;
            }
            else
            {
                _glfwInputError(GLFW_PLATFORM_ERROR,
                                "X11: Failed to query RandR version");
            }
        }
    }

    if (_glfw.x11.randr.available)
    {
        XRRScreenResources* sr = XRRGetScreenResourcesCurrent(_glfw.x11.display,
                                                              _glfw.x11.root);

        if (!sr->ncrtc || !XRRGetCrtcGammaSize(_glfw.x11.display, sr->crtcs[0]))
        {
            // This is likely an older Nvidia driver with broken gamma support
            // Flag it as useless and fall back to xf86vm gamma, if available
            _glfw.x11.randr.gammaBroken = GLFW_TRUE;
        }

        if (!sr->ncrtc)
        {
            // A system without CRTCs is likely a system with broken RandR
            // Disable the RandR monitor path and fall back to core functions
            _glfw.x11.randr.monitorBroken = GLFW_TRUE;
        }

        XRRFreeScreenResources(sr);
    }

    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        XRRSelectInput(_glfw.x11.display, _glfw.x11.root,
                       RROutputChangeNotifyMask);
    }

#if defined(__CYGWIN__)
    _glfw.x11.xcursor.handle = _glfw_dlopen("libXcursor-1.so");
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.xcursor.handle = _glfw_dlopen("libXcursor.so");
#else
    _glfw.x11.xcursor.handle = _glfw_dlopen("libXcursor.so.1");
#endif
    if (_glfw.x11.xcursor.handle)
    {
        _glfw.x11.xcursor.ImageCreate = (PFN_XcursorImageCreate)
            _glfw_dlsym(_glfw.x11.xcursor.handle, "XcursorImageCreate");
        _glfw.x11.xcursor.ImageDestroy = (PFN_XcursorImageDestroy)
            _glfw_dlsym(_glfw.x11.xcursor.handle, "XcursorImageDestroy");
        _glfw.x11.xcursor.ImageLoadCursor = (PFN_XcursorImageLoadCursor)
            _glfw_dlsym(_glfw.x11.xcursor.handle, "XcursorImageLoadCursor");
    }

#if defined(__CYGWIN__)
    _glfw.x11.xinerama.handle = _glfw_dlopen("libXinerama-1.so");
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.xinerama.handle = _glfw_dlopen("libXinerama.so");
#else
    _glfw.x11.xinerama.handle = _glfw_dlopen("libXinerama.so.1");
#endif
    if (_glfw.x11.xinerama.handle)
    {
        _glfw.x11.xinerama.IsActive = (PFN_XineramaIsActive)
            _glfw_dlsym(_glfw.x11.xinerama.handle, "XineramaIsActive");
        _glfw.x11.xinerama.QueryExtension = (PFN_XineramaQueryExtension)
            _glfw_dlsym(_glfw.x11.xinerama.handle, "XineramaQueryExtension");
        _glfw.x11.xinerama.QueryScreens = (PFN_XineramaQueryScreens)
            _glfw_dlsym(_glfw.x11.xinerama.handle, "XineramaQueryScreens");

        if (XineramaQueryExtension(_glfw.x11.display,
                                   &_glfw.x11.xinerama.major,
                                   &_glfw.x11.xinerama.minor))
        {
            if (XineramaIsActive(_glfw.x11.display))
                _glfw.x11.xinerama.available = GLFW_TRUE;
        }
    }

    _glfw.x11.xkb.major = 1;
    _glfw.x11.xkb.minor = 0;
    _glfw.x11.xkb.available =
        XkbQueryExtension(_glfw.x11.display,
                          &_glfw.x11.xkb.majorOpcode,
                          &_glfw.x11.xkb.eventBase,
                          &_glfw.x11.xkb.errorBase,
                          &_glfw.x11.xkb.major,
                          &_glfw.x11.xkb.minor);

    if (_glfw.x11.xkb.available)
    {
        Bool supported;

        if (XkbSetDetectableAutoRepeat(_glfw.x11.display, True, &supported))
        {
            if (supported)
                _glfw.x11.xkb.detectable = GLFW_TRUE;
        }

        XkbStateRec state;
        if (XkbGetState(_glfw.x11.display, XkbUseCoreKbd, &state) == Success)
            _glfw.x11.xkb.group = (unsigned int)state.group;

        XkbSelectEventDetails(_glfw.x11.display, XkbUseCoreKbd, XkbStateNotify,
                              XkbGroupStateMask, XkbGroupStateMask);
    }

#if defined(__CYGWIN__)
    _glfw.x11.x11xcb.handle = _glfw_dlopen("libX11-xcb-1.so");
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.x11xcb.handle = _glfw_dlopen("libX11-xcb.so");
#else
    _glfw.x11.x11xcb.handle = _glfw_dlopen("libX11-xcb.so.1");
#endif
    if (_glfw.x11.x11xcb.handle)
    {
        _glfw.x11.x11xcb.GetXCBConnection = (PFN_XGetXCBConnection)
            _glfw_dlsym(_glfw.x11.x11xcb.handle, "XGetXCBConnection");
    }

#if defined(__CYGWIN__)
    _glfw.x11.xrender.handle = _glfw_dlopen("libXrender-1.so");
#elif defined(__OpenBSD__) || defined(__NetBSD__)
    _glfw.x11.xrender.handle = _glfw_dlopen("libXrender.so");
#else
    _glfw.x11.xrender.handle = _glfw_dlopen("libXrender.so.1");
#endif
    if (_glfw.x11.xrender.handle)
    {
        _glfw.x11.xrender.QueryExtension = (PFN_XRenderQueryExtension)
            _glfw_dlsym(_glfw.x11.xrender.handle, "XRenderQueryExtension");
        _glfw.x11.xrender.QueryVersion = (PFN_XRenderQueryVersion)
            _glfw_dlsym(_glfw.x11.xrender.handle, "XRenderQueryVersion");
        _glfw.x11.xrender.FindVisualFormat = (PFN_XRenderFindVisualFormat)
            _glfw_dlsym(_glfw.x11.xrender.handle, "XRenderFindVisualFormat");

        if (XRenderQueryExtension(_glfw.x11.display,
                                  &_glfw.x11.xrender.errorBase,
                                  &_glfw.x11.xrender.eventBase))
        {
            if (XRenderQueryVersion(_glfw.x11.display,
                                    &_glfw.x11.xrender.major,
                                    &_glfw.x11.xrender.minor))
            {
                _glfw.x11.xrender.available = GLFW_TRUE;
            }
        }
    }

    // Update the key code LUT
    // FIXME: We should listen to XkbMapNotify events to track changes to
    // the keyboard mapping.
    createKeyTables();

    // String format atoms
    _glfw.x11.NULL_ = XInternAtom(_glfw.x11.display, "NULL", False);
    _glfw.x11.UTF8_STRING = XInternAtom(_glfw.x11.display, "UTF8_STRING", False);
    _glfw.x11.ATOM_PAIR = XInternAtom(_glfw.x11.display, "ATOM_PAIR", False);

    // Custom selection property atom
    _glfw.x11.GLFW_SELECTION =
        XInternAtom(_glfw.x11.display, "GLFW_SELECTION", False);

    // ICCCM standard clipboard atoms
    _glfw.x11.TARGETS = XInternAtom(_glfw.x11.display, "TARGETS", False);
    _glfw.x11.MULTIPLE = XInternAtom(_glfw.x11.display, "MULTIPLE", False);
    _glfw.x11.PRIMARY = XInternAtom(_glfw.x11.display, "PRIMARY", False);
    _glfw.x11.INCR = XInternAtom(_glfw.x11.display, "INCR", False);
    _glfw.x11.CLIPBOARD = XInternAtom(_glfw.x11.display, "CLIPBOARD", False);

    // Clipboard manager atoms
    _glfw.x11.CLIPBOARD_MANAGER =
        XInternAtom(_glfw.x11.display, "CLIPBOARD_MANAGER", False);
    _glfw.x11.SAVE_TARGETS =
        XInternAtom(_glfw.x11.display, "SAVE_TARGETS", False);

    // Xdnd (drag and drop) atoms
    _glfw.x11.XdndAware = XInternAtom(_glfw.x11.display, "XdndAware", False);
    _glfw.x11.XdndEnter = XInternAtom(_glfw.x11.display, "XdndEnter", False);
    _glfw.x11.XdndPosition = XInternAtom(_glfw.x11.display, "XdndPosition", False);
    _glfw.x11.XdndStatus = XInternAtom(_glfw.x11.display, "XdndStatus", False);
    _glfw.x11.XdndActionCopy = XInternAtom(_glfw.x11.display, "XdndActionCopy", False);
    _glfw.x11.XdndDrop = XInternAtom(_glfw.x11.display, "XdndDrop", False);
    _glfw.x11.XdndFinished = XInternAtom(_glfw.x11.display, "XdndFinished", False);
    _glfw.x11.XdndSelection = XInternAtom(_glfw.x11.display, "XdndSelection", False);
    _glfw.x11.XdndTypeList = XInternAtom(_glfw.x11.display, "XdndTypeList", False);
    _glfw.x11.text_uri_list = XInternAtom(_glfw.x11.display, "text/uri-list", False);

    // ICCCM, EWMH and Motif window property atoms
    // These can be set safely even without WM support
    // The EWMH atoms that require WM support are handled in detectEWMH
    _glfw.x11.WM_PROTOCOLS =
        XInternAtom(_glfw.x11.display, "WM_PROTOCOLS", False);
    _glfw.x11.WM_STATE =
        XInternAtom(_glfw.x11.display, "WM_STATE", False);
    _glfw.x11.WM_DELETE_WINDOW =
        XInternAtom(_glfw.x11.display, "WM_DELETE_WINDOW", False);
    _glfw.x11.NET_SUPPORTED =
        XInternAtom(_glfw.x11.display, "_NET_SUPPORTED", False);
    _glfw.x11.NET_SUPPORTING_WM_CHECK =
        XInternAtom(_glfw.x11.display, "_NET_SUPPORTING_WM_CHECK", False);
    _glfw.x11.NET_WM_ICON =
        XInternAtom(_glfw.x11.display, "_NET_WM_ICON", False);
    _glfw.x11.NET_WM_PING =
        XInternAtom(_glfw.x11.display, "_NET_WM_PING", False);
    _glfw.x11.NET_WM_PID =
        XInternAtom(_glfw.x11.display, "_NET_WM_PID", False);
    _glfw.x11.NET_WM_NAME =
        XInternAtom(_glfw.x11.display, "_NET_WM_NAME", False);
    _glfw.x11.NET_WM_ICON_NAME =
        XInternAtom(_glfw.x11.display, "_NET_WM_ICON_NAME", False);
    _glfw.x11.NET_WM_BYPASS_COMPOSITOR =
        XInternAtom(_glfw.x11.display, "_NET_WM_BYPASS_COMPOSITOR", False);
    _glfw.x11.NET_WM_WINDOW_OPACITY =
        XInternAtom(_glfw.x11.display, "_NET_WM_WINDOW_OPACITY", False);
    _glfw.x11.MOTIF_WM_HINTS =
        XInternAtom(_glfw.x11.display, "_MOTIF_WM_HINTS", False);

    // The compositing manager selection name contains the screen number
    {
        char name[32];
        snprintf(name, sizeof(name), "_NET_WM_CM_S%u", _glfw.x11.screen);
        _glfw.x11.NET_WM_CM_Sx = XInternAtom(_glfw.x11.display, name, False);
    }

    // Detect whether an EWMH-conformant window manager is running
    detectEWMH();

    return GLFW_TRUE;
}

// Retrieve system content scale via folklore heuristics
//
static void getSystemContentScale(float* xscale, float* yscale)
{
    // Start by assuming the default X11 DPI
    // NOTE: Some desktop environments (KDE) may remove the Xft.dpi field when it
    //       would be set to 96, so assume that is the case if we cannot find it
    float xdpi = 96.f, ydpi = 96.f;

    // NOTE: Basing the scale on Xft.dpi where available should provide the most
    //       consistent user experience (matches Qt, Gtk, etc), although not
    //       always the most accurate one
    char* rms = XResourceManagerString(_glfw.x11.display);
    if (rms)
    {
        XrmDatabase db = XrmGetStringDatabase(rms);
        if (db)
        {
            XrmValue value;
            char* type = NULL;

            if (XrmGetResource(db, "Xft.dpi", "Xft.Dpi", &type, &value))
            {
                if (type && strcmp(type, "String") == 0)
                    xdpi = ydpi = atof(value.addr);
            }

            XrmDestroyDatabase(db);
        }
    }

    *xscale = xdpi / 96.f;
    *yscale = ydpi / 96.f;
}

// Create a blank cursor for hidden and disabled cursor modes
//
static Cursor createHiddenCursor(void)
{
    unsigned char pixels[16 * 16 * 4] = { 0 };
    GLFWimage image = { 16, 16, pixels };
    return _glfwCreateCursorX11(&image, 0, 0);
}

// Create a helper window for IPC
//
static Window createHelperWindow(void)
{
    XSetWindowAttributes wa;
    wa.event_mask = PropertyChangeMask;

    return XCreateWindow(_glfw.x11.display, _glfw.x11.root,
                         0, 0, 1, 1, 0, 0,
                         InputOnly,
                         DefaultVisual(_glfw.x11.display, _glfw.x11.screen),
                         CWEventMask, &wa);
}

// Create the pipe for empty events without assumuing the OS has pipe2(2)
//
static GLFWbool createEmptyEventPipe(void)
{
    if (pipe(_glfw.x11.emptyEventPipe) != 0)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Failed to create empty event pipe: %s",
                        strerror(errno));
        return GLFW_FALSE;
    }

    for (int i = 0; i < 2; i++)
    {
        const int sf = fcntl(_glfw.x11.emptyEventPipe[i], F_GETFL, 0);
        const int df = fcntl(_glfw.x11.emptyEventPipe[i], F_GETFD, 0);

        if (sf == -1 || df == -1 ||
            fcntl(_glfw.x11.emptyEventPipe[i], F_SETFL, sf | O_NONBLOCK) == -1 ||
            fcntl(_glfw.x11.emptyEventPipe[i], F_SETFD, df | FD_CLOEXEC) == -1)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Failed to set flags for empty event pipe: %s",
                            strerror(errno));
            return GLFW_FALSE;
        }
    }

    return GLFW_TRUE;
}

// X error handler
//
static int errorHandler(Display *display, XErrorEvent* event)
{
    if (_glfw.x11.display != display)
        return 0;

    _glfw.x11.errorCode = event->error_code;
    return 0;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Sets the X error handler callback
//
void _glfwGrabErrorHandlerX11(void)
{
    assert(_glfw.x11.errorHandler == NULL);
    _glfw.x11.errorCode = Success;
    _glfw.x11.errorHandler = XSetErrorHandler(errorHandler);
}

// Clears the X error handler callback
//
void _glfwReleaseErrorHandlerX11(void)
{
    // Synchronize to make sure all commands are processed
    XSync(_glfw.x11.display, False);
    XSetErrorHandler(_glfw.x11.errorHandler);
    _glfw.x11.errorHandler = NULL;
}

// Reports the specified error, appending information about the last X error
//
void _glfwInputErrorX11(int error, const char* message)
{
    char buffer[_GLFW_MESSAGE_SIZE];
    XGetErrorText(_glfw.x11.display, _glfw.x11.errorCode,
                  buffer, sizeof(buffer));

    _glfwInputError(error, "%s: %s", message, buffer);
}

// Creates a native cursor object from the specified image and hotspot
//
Cursor _glfwCreateCursorX11(const GLFWimage* image, int xhot, int yhot)
{
    int i;
    Cursor cursor;

    if (!_glfw.x11.xcursor.handle)
        return None;

    XcursorImage* native = XcursorImageCreate(image->width, image->height);
    if (native == NULL)
        return None;

    native->xhot = xhot;
    native->yhot = yhot;

    unsigned char* source = (unsigned char*) image->pixels;
    XcursorPixel* target = native->pixels;

    for (i = 0;  i < image->width * image->height;  i++, target++, source += 4)
    {
        unsigned int alpha = source[3];

        *target = (alpha << 24) |
                  ((unsigned char) ((source[0] * alpha) / 255) << 16) |
                  ((unsigned char) ((source[1] * alpha) / 255) <<  8) |
                  ((unsigned char) ((source[2] * alpha) / 255) <<  0);
    }

    cursor = XcursorImageLoadCursor(_glfw.x11.display, native);
    XcursorImageDestroy(native);

    return cursor;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformInit(void)
{
    // HACK: If the application has left the locale as "C" then both wide
    //       character text input and explicit UTF-8 input via XIM will break
    //       This sets the CTYPE part of the current locale from the environment
    //       in the hope that it is set to something more sane than "C"
    if (strcmp(setlocale(LC_CTYPE, NULL), "C") == 0)
        setlocale(LC_CTYPE, "");

    XInitThreads();
    XrmInitialize();

    _glfw.x11.display = XOpenDisplay(NULL);
    if (!_glfw.x11.display)
    {
        const char* display = getenv("DISPLAY");
        if (display)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Failed to open display %s", display);
        }
        else
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: The DISPLAY environment variable is missing");
        }

        return GLFW_FALSE;
    }

    _glfw.x11.screen = DefaultScreen(_glfw.x11.display);
    _glfw.x11.root = RootWindow(_glfw.x11.display, _glfw.x11.screen);
    _glfw.x11.context = XUniqueContext();

    getSystemContentScale(&_glfw.x11.contentScaleX, &_glfw.x11.contentScaleY);

    if (!createEmptyEventPipe())
        return GLFW_FALSE;

    if (!initExtensions())
        return GLFW_FALSE;

    _glfw.x11.helperWindowHandle = createHelperWindow();
    _glfw.x11.hiddenCursorHandle = createHiddenCursor();

    if (XSupportsLocale())
    {
        XSetLocaleModifiers("");

        _glfw.x11.im = XOpenIM(_glfw.x11.display, 0, NULL, NULL);
        if (_glfw.x11.im)
        {
            if (!hasUsableInputMethodStyle())
            {
                XCloseIM(_glfw.x11.im);
                _glfw.x11.im = NULL;
            }
        }
    }

#if defined(__linux__)
    if (!_glfwInitJoysticksLinux())
        return GLFW_FALSE;
#endif

    _glfwInitTimerPOSIX();

    _glfwPollMonitorsX11();
    return GLFW_TRUE;
}

void _glfwPlatformTerminate(void)
{
    if (_glfw.x11.helperWindowHandle)
    {
        if (XGetSelectionOwner(_glfw.x11.display, _glfw.x11.CLIPBOARD) ==
            _glfw.x11.helperWindowHandle)
        {
            _glfwPushSelectionToManagerX11();
        }

        XDestroyWindow(_glfw.x11.display, _glfw.x11.helperWindowHandle);
        _glfw.x11.helperWindowHandle = None;
    }

    if (_glfw.x11.hiddenCursorHandle)
    {
        XFreeCursor(_glfw.x11.display, _glfw.x11.hiddenCursorHandle);
        _glfw.x11.hiddenCursorHandle = (Cursor) 0;
    }

    free(_glfw.x11.primarySelectionString);
    free(_glfw.x11.clipboardString);

    if (_glfw.x11.im)
    {
        XCloseIM(_glfw.x11.im);
        _glfw.x11.im = NULL;
    }

    if (_glfw.x11.display)
    {
        XCloseDisplay(_glfw.x11.display);
        _glfw.x11.display = NULL;
    }

    if (_glfw.x11.x11xcb.handle)
    {
        _glfw_dlclose(_glfw.x11.x11xcb.handle);
        _glfw.x11.x11xcb.handle = NULL;
    }

    if (_glfw.x11.xcursor.handle)
    {
        _glfw_dlclose(_glfw.x11.xcursor.handle);
        _glfw.x11.xcursor.handle = NULL;
    }

    if (_glfw.x11.randr.handle)
    {
        _glfw_dlclose(_glfw.x11.randr.handle);
        _glfw.x11.randr.handle = NULL;
    }

    if (_glfw.x11.xinerama.handle)
    {
        _glfw_dlclose(_glfw.x11.xinerama.handle);
        _glfw.x11.xinerama.handle = NULL;
    }

    if (_glfw.x11.xrender.handle)
    {
        _glfw_dlclose(_glfw.x11.xrender.handle);
        _glfw.x11.xrender.handle = NULL;
    }

    if (_glfw.x11.vidmode.handle)
    {
        _glfw_dlclose(_glfw.x11.vidmode.handle);
        _glfw.x11.vidmode.handle = NULL;
    }

    if (_glfw.x11.xi.handle)
    {
        _glfw_dlclose(_glfw.x11.xi.handle);
        _glfw.x11.xi.handle = NULL;
    }

    _glfwTerminateOSMesa();
    // NOTE: These need to be unloaded after XCloseDisplay, as they register
    //       cleanup callbacks that get called by that function
    _glfwTerminateEGL();
    _glfwTerminateGLX();

#if defined(__linux__)
    _glfwTerminateJoysticksLinux();
#endif

    if (_glfw.x11.emptyEventPipe[0] || _glfw.x11.emptyEventPipe[1])
    {
        close(_glfw.x11.emptyEventPipe[0]);
        close(_glfw.x11.emptyEventPipe[1]);
    }
}

const char* _glfwPlatformGetVersionString(void)
{
    return _GLFW_VERSION_NUMBER " X11 GLX EGL OSMesa"
#if defined(_POSIX_TIMERS) && defined(_POSIX_MONOTONIC_CLOCK)
        " clock_gettime"
#else
        " gettimeofday"
#endif
#if defined(__linux__)
        " evdev"
#endif
#if defined(_GLFW_BUILD_DLL)
        " shared"
#endif
        ;
}

//========================================================================
// GLFW 3.3 X11 - www.glfw.org
// GLFW 3.3 - www.glfw.org - x11_monitor.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================


// Check whether the display mode should be included in enumeration
//
static GLFWbool modeIsGood(const XRRModeInfo* mi)
{
    return (mi->modeFlags & RR_Interlace) == 0;
}

// Calculates the refresh rate, in Hz, from the specified RandR mode info
//
static int calculateRefreshRate(const XRRModeInfo* mi)
{
    if (mi->hTotal && mi->vTotal)
        return (int) round((double) mi->dotClock / ((double) mi->hTotal * (double) mi->vTotal));
    else
        return 0;
}

// Returns the mode info for a RandR mode XID
//
static const XRRModeInfo* getModeInfo(const XRRScreenResources* sr, RRMode id)
{
    for (int i = 0;  i < sr->nmode;  i++)
    {
        if (sr->modes[i].id == id)
            return sr->modes + i;
    }

    return NULL;
}

// Convert RandR mode info to GLFW video mode
//
static GLFWvidmode vidmodeFromModeInfo(const XRRModeInfo* mi,
                                       const XRRCrtcInfo* ci)
{
    GLFWvidmode mode;

    if (ci->rotation == RR_Rotate_90 || ci->rotation == RR_Rotate_270)
    {
        mode.width  = mi->height;
        mode.height = mi->width;
    }
    else
    {
        mode.width  = mi->width;
        mode.height = mi->height;
    }

    mode.refreshRate = calculateRefreshRate(mi);

    _glfwSplitBPP(DefaultDepth(_glfw.x11.display, _glfw.x11.screen),
                  &mode.redBits, &mode.greenBits, &mode.blueBits);

    return mode;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Poll for changes in the set of connected monitors
//
void _glfwPollMonitorsX11(void)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        int disconnectedCount, screenCount = 0;
        _GLFWmonitor** disconnected = NULL;
        XineramaScreenInfo* screens = NULL;
        XRRScreenResources* sr = XRRGetScreenResourcesCurrent(_glfw.x11.display,
                                                              _glfw.x11.root);
        RROutput primary = XRRGetOutputPrimary(_glfw.x11.display,
                                               _glfw.x11.root);

        if (_glfw.x11.xinerama.available)
            screens = XineramaQueryScreens(_glfw.x11.display, &screenCount);

        disconnectedCount = _glfw.monitorCount;
        if (disconnectedCount)
        {
            disconnected = calloc(_glfw.monitorCount, sizeof(_GLFWmonitor*));
            memcpy(disconnected,
                   _glfw.monitors,
                   _glfw.monitorCount * sizeof(_GLFWmonitor*));
        }

        for (int i = 0;  i < sr->noutput;  i++)
        {
            int j, type, widthMM, heightMM;

            XRROutputInfo* oi = XRRGetOutputInfo(_glfw.x11.display, sr, sr->outputs[i]);
            if (oi->connection != RR_Connected || oi->crtc == None)
            {
                XRRFreeOutputInfo(oi);
                continue;
            }

            for (j = 0;  j < disconnectedCount;  j++)
            {
                if (disconnected[j] &&
                    disconnected[j]->x11.output == sr->outputs[i])
                {
                    disconnected[j] = NULL;
                    break;
                }
            }

            if (j < disconnectedCount)
            {
                XRRFreeOutputInfo(oi);
                continue;
            }

            XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, oi->crtc);
            if (ci->rotation == RR_Rotate_90 || ci->rotation == RR_Rotate_270)
            {
                widthMM  = oi->mm_height;
                heightMM = oi->mm_width;
            }
            else
            {
                widthMM  = oi->mm_width;
                heightMM = oi->mm_height;
            }

            if (widthMM <= 0 || heightMM <= 0)
            {
                // HACK: If RandR does not provide a physical size, assume the
                //       X11 default 96 DPI and calculate from the CRTC viewport
                // NOTE: These members are affected by rotation, unlike the mode
                //       info and output info members
                widthMM  = (int) (ci->width * 25.4f / 96.f);
                heightMM = (int) (ci->height * 25.4f / 96.f);
            }

            _GLFWmonitor* monitor = _glfwAllocMonitor(oi->name, widthMM, heightMM);
            monitor->x11.output = sr->outputs[i];
            monitor->x11.crtc   = oi->crtc;

            for (j = 0;  j < screenCount;  j++)
            {
                if (screens[j].x_org == ci->x &&
                    screens[j].y_org == ci->y &&
                    screens[j].width == ci->width &&
                    screens[j].height == ci->height)
                {
                    monitor->x11.index = j;
                    break;
                }
            }

            if (monitor->x11.output == primary)
                type = _GLFW_INSERT_FIRST;
            else
                type = _GLFW_INSERT_LAST;

            _glfwInputMonitor(monitor, GLFW_CONNECTED, type);

            XRRFreeOutputInfo(oi);
            XRRFreeCrtcInfo(ci);
        }

        XRRFreeScreenResources(sr);

        if (screens)
            XFree(screens);

        for (int i = 0;  i < disconnectedCount;  i++)
        {
            if (disconnected[i])
                _glfwInputMonitor(disconnected[i], GLFW_DISCONNECTED, 0);
        }

        free(disconnected);
    }
    else
    {
        const int widthMM = DisplayWidthMM(_glfw.x11.display, _glfw.x11.screen);
        const int heightMM = DisplayHeightMM(_glfw.x11.display, _glfw.x11.screen);

        _glfwInputMonitor(_glfwAllocMonitor("Display", widthMM, heightMM),
                          GLFW_CONNECTED,
                          _GLFW_INSERT_FIRST);
    }
}

// Set the current video mode for the specified monitor
//
void _glfwSetVideoModeX11(_GLFWmonitor* monitor, const GLFWvidmode* desired)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        GLFWvidmode current;
        RRMode native = None;

        const GLFWvidmode* best = _glfwChooseVideoMode(monitor, desired);
        _glfwPlatformGetVideoMode(monitor, &current);
        if (_glfwCompareVideoModes(&current, best) == 0)
            return;

        XRRScreenResources* sr =
            XRRGetScreenResourcesCurrent(_glfw.x11.display, _glfw.x11.root);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, monitor->x11.crtc);
        XRROutputInfo* oi = XRRGetOutputInfo(_glfw.x11.display, sr, monitor->x11.output);

        for (int i = 0;  i < oi->nmode;  i++)
        {
            const XRRModeInfo* mi = getModeInfo(sr, oi->modes[i]);
            if (!modeIsGood(mi))
                continue;

            const GLFWvidmode mode = vidmodeFromModeInfo(mi, ci);
            if (_glfwCompareVideoModes(best, &mode) == 0)
            {
                native = mi->id;
                break;
            }
        }

        if (native)
        {
            if (monitor->x11.oldMode == None)
                monitor->x11.oldMode = ci->mode;

            XRRSetCrtcConfig(_glfw.x11.display,
                             sr, monitor->x11.crtc,
                             CurrentTime,
                             ci->x, ci->y,
                             native,
                             ci->rotation,
                             ci->outputs,
                             ci->noutput);
        }

        XRRFreeOutputInfo(oi);
        XRRFreeCrtcInfo(ci);
        XRRFreeScreenResources(sr);
    }
}

// Restore the saved (original) video mode for the specified monitor
//
void _glfwRestoreVideoModeX11(_GLFWmonitor* monitor)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        if (monitor->x11.oldMode == None)
            return;

        XRRScreenResources* sr =
            XRRGetScreenResourcesCurrent(_glfw.x11.display, _glfw.x11.root);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, monitor->x11.crtc);

        XRRSetCrtcConfig(_glfw.x11.display,
                         sr, monitor->x11.crtc,
                         CurrentTime,
                         ci->x, ci->y,
                         monitor->x11.oldMode,
                         ci->rotation,
                         ci->outputs,
                         ci->noutput);

        XRRFreeCrtcInfo(ci);
        XRRFreeScreenResources(sr);

        monitor->x11.oldMode = None;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

void _glfwPlatformFreeMonitor(_GLFWmonitor* monitor)
{
}

void _glfwPlatformGetMonitorPos(_GLFWmonitor* monitor, int* xpos, int* ypos)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        XRRScreenResources* sr =
            XRRGetScreenResourcesCurrent(_glfw.x11.display, _glfw.x11.root);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, monitor->x11.crtc);

        if (ci)
        {
            if (xpos)
                *xpos = ci->x;
            if (ypos)
                *ypos = ci->y;

            XRRFreeCrtcInfo(ci);
        }

        XRRFreeScreenResources(sr);
    }
}

void _glfwPlatformGetMonitorContentScale(_GLFWmonitor* monitor,
                                         float* xscale, float* yscale)
{
    if (xscale)
        *xscale = _glfw.x11.contentScaleX;
    if (yscale)
        *yscale = _glfw.x11.contentScaleY;
}

void _glfwPlatformGetMonitorWorkarea(_GLFWmonitor* monitor, int* xpos, int* ypos, int* width, int* height)
{
    int areaX = 0, areaY = 0, areaWidth = 0, areaHeight = 0;

    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        XRRScreenResources* sr =
            XRRGetScreenResourcesCurrent(_glfw.x11.display, _glfw.x11.root);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, monitor->x11.crtc);

        areaX = ci->x;
        areaY = ci->y;

        const XRRModeInfo* mi = getModeInfo(sr, ci->mode);

        if (ci->rotation == RR_Rotate_90 || ci->rotation == RR_Rotate_270)
        {
            areaWidth  = mi->height;
            areaHeight = mi->width;
        }
        else
        {
            areaWidth  = mi->width;
            areaHeight = mi->height;
        }

        XRRFreeCrtcInfo(ci);
        XRRFreeScreenResources(sr);
    }
    else
    {
        areaWidth  = DisplayWidth(_glfw.x11.display, _glfw.x11.screen);
        areaHeight = DisplayHeight(_glfw.x11.display, _glfw.x11.screen);
    }

    if (_glfw.x11.NET_WORKAREA && _glfw.x11.NET_CURRENT_DESKTOP)
    {
        Atom* extents = NULL;
        Atom* desktop = NULL;
        const unsigned long extentCount =
            _glfwGetWindowPropertyX11(_glfw.x11.root,
                                      _glfw.x11.NET_WORKAREA,
                                      XA_CARDINAL,
                                      (unsigned char**) &extents);

        if (_glfwGetWindowPropertyX11(_glfw.x11.root,
                                      _glfw.x11.NET_CURRENT_DESKTOP,
                                      XA_CARDINAL,
                                      (unsigned char**) &desktop) > 0)
        {
            if (extentCount >= 4 && *desktop < extentCount / 4)
            {
                const int globalX = extents[*desktop * 4 + 0];
                const int globalY = extents[*desktop * 4 + 1];
                const int globalWidth  = extents[*desktop * 4 + 2];
                const int globalHeight = extents[*desktop * 4 + 3];

                if (areaX < globalX)
                {
                    areaWidth -= globalX - areaX;
                    areaX = globalX;
                }

                if (areaY < globalY)
                {
                    areaHeight -= globalY - areaY;
                    areaY = globalY;
                }

                if (areaX + areaWidth > globalX + globalWidth)
                    areaWidth = globalX - areaX + globalWidth;
                if (areaY + areaHeight > globalY + globalHeight)
                    areaHeight = globalY - areaY + globalHeight;
            }
        }

        if (extents)
            XFree(extents);
        if (desktop)
            XFree(desktop);
    }

    if (xpos)
        *xpos = areaX;
    if (ypos)
        *ypos = areaY;
    if (width)
        *width = areaWidth;
    if (height)
        *height = areaHeight;
}

GLFWvidmode* _glfwPlatformGetVideoModes(_GLFWmonitor* monitor, int* count)
{
    GLFWvidmode* result;

    *count = 0;

    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        XRRScreenResources* sr =
            XRRGetScreenResourcesCurrent(_glfw.x11.display, _glfw.x11.root);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, monitor->x11.crtc);
        XRROutputInfo* oi = XRRGetOutputInfo(_glfw.x11.display, sr, monitor->x11.output);

        result = calloc(oi->nmode, sizeof(GLFWvidmode));

        for (int i = 0;  i < oi->nmode;  i++)
        {
            const XRRModeInfo* mi = getModeInfo(sr, oi->modes[i]);
            if (!modeIsGood(mi))
                continue;

            const GLFWvidmode mode = vidmodeFromModeInfo(mi, ci);
            int j;

            for (j = 0;  j < *count;  j++)
            {
                if (_glfwCompareVideoModes(result + j, &mode) == 0)
                    break;
            }

            // Skip duplicate modes
            if (j < *count)
                continue;

            (*count)++;
            result[*count - 1] = mode;
        }

        XRRFreeOutputInfo(oi);
        XRRFreeCrtcInfo(ci);
        XRRFreeScreenResources(sr);
    }
    else
    {
        *count = 1;
        result = calloc(1, sizeof(GLFWvidmode));
        _glfwPlatformGetVideoMode(monitor, result);
    }

    return result;
}

void _glfwPlatformGetVideoMode(_GLFWmonitor* monitor, GLFWvidmode* mode)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.monitorBroken)
    {
        XRRScreenResources* sr =
            XRRGetScreenResourcesCurrent(_glfw.x11.display, _glfw.x11.root);
        XRRCrtcInfo* ci = XRRGetCrtcInfo(_glfw.x11.display, sr, monitor->x11.crtc);

        if (ci)
        {
            const XRRModeInfo* mi = getModeInfo(sr, ci->mode);
            if (mi)  // mi can be NULL if the monitor has been disconnected
                *mode = vidmodeFromModeInfo(mi, ci);

            XRRFreeCrtcInfo(ci);
        }

        XRRFreeScreenResources(sr);
    }
    else
    {
        mode->width = DisplayWidth(_glfw.x11.display, _glfw.x11.screen);
        mode->height = DisplayHeight(_glfw.x11.display, _glfw.x11.screen);
        mode->refreshRate = 0;

        _glfwSplitBPP(DefaultDepth(_glfw.x11.display, _glfw.x11.screen),
                      &mode->redBits, &mode->greenBits, &mode->blueBits);
    }
}

GLFWbool _glfwPlatformGetGammaRamp(_GLFWmonitor* monitor, GLFWgammaramp* ramp)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.gammaBroken)
    {
        const size_t size = XRRGetCrtcGammaSize(_glfw.x11.display,
                                                monitor->x11.crtc);
        XRRCrtcGamma* gamma = XRRGetCrtcGamma(_glfw.x11.display,
                                              monitor->x11.crtc);

        _glfwAllocGammaArrays(ramp, size);

        memcpy(ramp->red,   gamma->red,   size * sizeof(unsigned short));
        memcpy(ramp->green, gamma->green, size * sizeof(unsigned short));
        memcpy(ramp->blue,  gamma->blue,  size * sizeof(unsigned short));

        XRRFreeGamma(gamma);
        return GLFW_TRUE;
    }
    else if (_glfw.x11.vidmode.available)
    {
        int size;
        XF86VidModeGetGammaRampSize(_glfw.x11.display, _glfw.x11.screen, &size);

        _glfwAllocGammaArrays(ramp, size);

        XF86VidModeGetGammaRamp(_glfw.x11.display,
                                _glfw.x11.screen,
                                ramp->size, ramp->red, ramp->green, ramp->blue);
        return GLFW_TRUE;
    }
    else
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Gamma ramp access not supported by server");
        return GLFW_FALSE;
    }
}

void _glfwPlatformSetGammaRamp(_GLFWmonitor* monitor, const GLFWgammaramp* ramp)
{
    if (_glfw.x11.randr.available && !_glfw.x11.randr.gammaBroken)
    {
        if (XRRGetCrtcGammaSize(_glfw.x11.display, monitor->x11.crtc) != ramp->size)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Gamma ramp size must match current ramp size");
            return;
        }

        XRRCrtcGamma* gamma = XRRAllocGamma(ramp->size);

        memcpy(gamma->red,   ramp->red,   ramp->size * sizeof(unsigned short));
        memcpy(gamma->green, ramp->green, ramp->size * sizeof(unsigned short));
        memcpy(gamma->blue,  ramp->blue,  ramp->size * sizeof(unsigned short));

        XRRSetCrtcGamma(_glfw.x11.display, monitor->x11.crtc, gamma);
        XRRFreeGamma(gamma);
    }
    else if (_glfw.x11.vidmode.available)
    {
        XF86VidModeSetGammaRamp(_glfw.x11.display,
                                _glfw.x11.screen,
                                ramp->size,
                                (unsigned short*) ramp->red,
                                (unsigned short*) ramp->green,
                                (unsigned short*) ramp->blue);
    }
    else
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Gamma ramp access not supported by server");
    }
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI RRCrtc glfwGetX11Adapter(GLFWmonitor* handle)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(None);
    return monitor->x11.crtc;
}

GLFWAPI RROutput glfwGetX11Monitor(GLFWmonitor* handle)
{
    _GLFWmonitor* monitor = (_GLFWmonitor*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(None);
    return monitor->x11.output;
}

//========================================================================
// GLFW 3.3 X11 - www.glfw.org
// GLFW 3.3 - www.glfw.org - x11_window.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================

#define _GNU_SOURCE

// Action for EWMH client messages
#define _NET_WM_STATE_REMOVE        0
#define _NET_WM_STATE_ADD           1
#define _NET_WM_STATE_TOGGLE        2

// Additional mouse button names for XButtonEvent
#define Button6            6
#define Button7            7

// Motif WM hints flags
#define MWM_HINTS_DECORATIONS   2
#define MWM_DECOR_ALL           1

#define _GLFW_XDND_VERSION 5

// Wait for data to arrive on any of the specified file descriptors
//
static GLFWbool waitForData(struct pollfd* fds, nfds_t count, double* timeout)
{
    for (;;)
    {
        if (timeout)
        {
            const uint64_t base = _glfwPlatformGetTimerValue();

#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__CYGWIN__)
            const time_t seconds = (time_t) *timeout;
            const long nanoseconds = (long) ((*timeout - seconds) * 1e9);
            const struct timespec ts = { seconds, nanoseconds };
            const int result = ppoll(fds, count, &ts, NULL);
#elif defined(__NetBSD__)
            const time_t seconds = (time_t) *timeout;
            const long nanoseconds = (long) ((*timeout - seconds) * 1e9);
            const struct timespec ts = { seconds, nanoseconds };
            const int result = pollts(fds, count, &ts, NULL);
#else
            const int milliseconds = (int) (*timeout * 1e3);
            const int result = poll(fds, count, milliseconds);
#endif
            const int error = errno; // clock_gettime may overwrite our error

            *timeout -= (_glfwPlatformGetTimerValue() - base) /
                (double) _glfwPlatformGetTimerFrequency();

            if (result > 0)
                return GLFW_TRUE;
            else if (result == -1 && error != EINTR && error != EAGAIN)
                return GLFW_FALSE;
            else if (*timeout <= 0.0)
                return GLFW_FALSE;
        }
        else
        {
            const int result = poll(fds, count, -1);
            if (result > 0)
                return GLFW_TRUE;
            else if (result == -1 && errno != EINTR && errno != EAGAIN)
                return GLFW_FALSE;
        }
    }
}

// Wait for event data to arrive on the X11 display socket
// This avoids blocking other threads via the per-display Xlib lock that also
// covers GLX functions
//
static GLFWbool waitForX11Event(double* timeout)
{
    struct pollfd fd = { ConnectionNumber(_glfw.x11.display), POLLIN };

    while (!XPending(_glfw.x11.display))
    {
        if (!waitForData(&fd, 1, timeout))
            return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

// Wait for event data to arrive on any event file descriptor
// This avoids blocking other threads via the per-display Xlib lock that also
// covers GLX functions
//
static GLFWbool waitForAnyEvent(double* timeout)
{
    nfds_t count = 2;
    struct pollfd fds[3] =
    {
        { ConnectionNumber(_glfw.x11.display), POLLIN },
        { _glfw.x11.emptyEventPipe[0], POLLIN }
    };

#if defined(__linux__)
    if (_glfw.linjs.inotify > 0)
        fds[count++] = (struct pollfd) { _glfw.linjs.inotify, POLLIN };
#endif

    while (!XPending(_glfw.x11.display))
    {
        if (!waitForData(fds, count, timeout))
            return GLFW_FALSE;

        for (int i = 1; i < count; i++)
        {
            if (fds[i].revents & POLLIN)
                return GLFW_TRUE;
        }
    }

    return GLFW_TRUE;
}

// Writes a byte to the empty event pipe
//
static void writeEmptyEvent(void)
{
    for (;;)
    {
        const char byte = 0;
        const ssize_t result = write(_glfw.x11.emptyEventPipe[1], &byte, 1);
        if (result == 1 || (result == -1 && errno != EINTR))
            break;
    }
}

// Drains available data from the empty event pipe
//
static void drainEmptyEvents(void)
{
    for (;;)
    {
        char dummy[64];
        const ssize_t result = read(_glfw.x11.emptyEventPipe[0], dummy, sizeof(dummy));
        if (result == -1 && errno != EINTR)
            break;
    }
}

// Waits until a VisibilityNotify event arrives for the specified window or the
// timeout period elapses (ICCCM section 4.2.2)
//
static GLFWbool waitForVisibilityNotify(_GLFWwindow* window)
{
    XEvent dummy;
    double timeout = 0.1;

    while (!XCheckTypedWindowEvent(_glfw.x11.display,
                                   window->x11.handle,
                                   VisibilityNotify,
                                   &dummy))
    {
        if (!waitForX11Event(&timeout))
            return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

// Returns whether the window is iconified
//
static int getWindowState(_GLFWwindow* window)
{
    int result = WithdrawnState;
    struct {
        CARD32 state;
        Window icon;
    } *state = NULL;

    if (_glfwGetWindowPropertyX11(window->x11.handle,
                                  _glfw.x11.WM_STATE,
                                  _glfw.x11.WM_STATE,
                                  (unsigned char**) &state) >= 2)
    {
        result = state->state;
    }

    if (state)
        XFree(state);

    return result;
}

// Returns whether the event is a selection event
//
static Bool isSelectionEvent(Display* display, XEvent* event, XPointer pointer)
{
    if (event->xany.window != _glfw.x11.helperWindowHandle)
        return False;

    return event->type == SelectionRequest ||
           event->type == SelectionNotify ||
           event->type == SelectionClear;
}

// Returns whether it is a _NET_FRAME_EXTENTS event for the specified window
//
static Bool isFrameExtentsEvent(Display* display, XEvent* event, XPointer pointer)
{
    _GLFWwindow* window = (_GLFWwindow*) pointer;
    return event->type == PropertyNotify &&
           event->xproperty.state == PropertyNewValue &&
           event->xproperty.window == window->x11.handle &&
           event->xproperty.atom == _glfw.x11.NET_FRAME_EXTENTS;
}

// Returns whether it is a property event for the specified selection transfer
//
static Bool isSelPropNewValueNotify(Display* display, XEvent* event, XPointer pointer)
{
    XEvent* notification = (XEvent*) pointer;
    return event->type == PropertyNotify &&
           event->xproperty.state == PropertyNewValue &&
           event->xproperty.window == notification->xselection.requestor &&
           event->xproperty.atom == notification->xselection.property;
}

// Translates an X event modifier state mask
//
static int translateState(int state)
{
    int mods = 0;

    if (state & ShiftMask)
        mods |= GLFW_MOD_SHIFT;
    if (state & ControlMask)
        mods |= GLFW_MOD_CONTROL;
    if (state & Mod1Mask)
        mods |= GLFW_MOD_ALT;
    if (state & Mod4Mask)
        mods |= GLFW_MOD_SUPER;
    if (state & LockMask)
        mods |= GLFW_MOD_CAPS_LOCK;
    if (state & Mod2Mask)
        mods |= GLFW_MOD_NUM_LOCK;

    return mods;
}

// Translates an X11 key code to a GLFW key token
//
static int translateKey(int scancode)
{
    // Use the pre-filled LUT (see createKeyTables() in x11_init.c)
    if (scancode < 0 || scancode > 255)
        return GLFW_KEY_UNKNOWN;

    return _glfw.x11.keycodes[scancode];
}

// Sends an EWMH or ICCCM event to the window manager
//
static void sendEventToWM(_GLFWwindow* window, Atom type,
                          long a, long b, long c, long d, long e)
{
    XEvent event = { ClientMessage };
    event.xclient.window = window->x11.handle;
    event.xclient.format = 32; // Data is 32-bit longs
    event.xclient.message_type = type;
    event.xclient.data.l[0] = a;
    event.xclient.data.l[1] = b;
    event.xclient.data.l[2] = c;
    event.xclient.data.l[3] = d;
    event.xclient.data.l[4] = e;

    XSendEvent(_glfw.x11.display, _glfw.x11.root,
               False,
               SubstructureNotifyMask | SubstructureRedirectMask,
               &event);
}

// Updates the normal hints according to the window settings
//
static void updateNormalHints(_GLFWwindow* window, int width, int height)
{
    XSizeHints* hints = XAllocSizeHints();

    if (!window->monitor)
    {
        if (window->resizable)
        {
            if (window->minwidth != GLFW_DONT_CARE &&
                window->minheight != GLFW_DONT_CARE)
            {
                hints->flags |= PMinSize;
                hints->min_width = window->minwidth;
                hints->min_height = window->minheight;
            }

            if (window->maxwidth != GLFW_DONT_CARE &&
                window->maxheight != GLFW_DONT_CARE)
            {
                hints->flags |= PMaxSize;
                hints->max_width = window->maxwidth;
                hints->max_height = window->maxheight;
            }

            if (window->numer != GLFW_DONT_CARE &&
                window->denom != GLFW_DONT_CARE)
            {
                hints->flags |= PAspect;
                hints->min_aspect.x = hints->max_aspect.x = window->numer;
                hints->min_aspect.y = hints->max_aspect.y = window->denom;
            }
        }
        else
        {
            hints->flags |= (PMinSize | PMaxSize);
            hints->min_width  = hints->max_width  = width;
            hints->min_height = hints->max_height = height;
        }
    }

    hints->flags |= PWinGravity;
    hints->win_gravity = StaticGravity;

    XSetWMNormalHints(_glfw.x11.display, window->x11.handle, hints);
    XFree(hints);
}

// Updates the full screen status of the window
//
static void updateWindowMode(_GLFWwindow* window)
{
    if (window->monitor)
    {
        if (_glfw.x11.xinerama.available &&
            _glfw.x11.NET_WM_FULLSCREEN_MONITORS)
        {
            sendEventToWM(window,
                          _glfw.x11.NET_WM_FULLSCREEN_MONITORS,
                          window->monitor->x11.index,
                          window->monitor->x11.index,
                          window->monitor->x11.index,
                          window->monitor->x11.index,
                          0);
        }

        if (_glfw.x11.NET_WM_STATE && _glfw.x11.NET_WM_STATE_FULLSCREEN)
        {
            sendEventToWM(window,
                          _glfw.x11.NET_WM_STATE,
                          _NET_WM_STATE_ADD,
                          _glfw.x11.NET_WM_STATE_FULLSCREEN,
                          0, 1, 0);
        }
        else
        {
            // This is the butcher's way of removing window decorations
            // Setting the override-redirect attribute on a window makes the
            // window manager ignore the window completely (ICCCM, section 4)
            // The good thing is that this makes undecorated full screen windows
            // easy to do; the bad thing is that we have to do everything
            // manually and some things (like iconify/restore) won't work at
            // all, as those are tasks usually performed by the window manager

            XSetWindowAttributes attributes;
            attributes.override_redirect = True;
            XChangeWindowAttributes(_glfw.x11.display,
                                    window->x11.handle,
                                    CWOverrideRedirect,
                                    &attributes);

            window->x11.overrideRedirect = GLFW_TRUE;
        }

        // Enable compositor bypass
        if (!window->x11.transparent)
        {
            const unsigned long value = 1;

            XChangeProperty(_glfw.x11.display,  window->x11.handle,
                            _glfw.x11.NET_WM_BYPASS_COMPOSITOR, XA_CARDINAL, 32,
                            PropModeReplace, (unsigned char*) &value, 1);
        }
    }
    else
    {
        if (_glfw.x11.xinerama.available &&
            _glfw.x11.NET_WM_FULLSCREEN_MONITORS)
        {
            XDeleteProperty(_glfw.x11.display, window->x11.handle,
                            _glfw.x11.NET_WM_FULLSCREEN_MONITORS);
        }

        if (_glfw.x11.NET_WM_STATE && _glfw.x11.NET_WM_STATE_FULLSCREEN)
        {
            sendEventToWM(window,
                          _glfw.x11.NET_WM_STATE,
                          _NET_WM_STATE_REMOVE,
                          _glfw.x11.NET_WM_STATE_FULLSCREEN,
                          0, 1, 0);
        }
        else
        {
            XSetWindowAttributes attributes;
            attributes.override_redirect = False;
            XChangeWindowAttributes(_glfw.x11.display,
                                    window->x11.handle,
                                    CWOverrideRedirect,
                                    &attributes);

            window->x11.overrideRedirect = GLFW_FALSE;
        }

        // Disable compositor bypass
        if (!window->x11.transparent)
        {
            XDeleteProperty(_glfw.x11.display, window->x11.handle,
                            _glfw.x11.NET_WM_BYPASS_COMPOSITOR);
        }
    }
}

// Decode a Unicode code point from a UTF-8 stream
// Based on cutef8 by Jeff Bezanson (Public Domain)
//
#if defined(X_HAVE_UTF8_STRING)
static uint32_t decodeUTF8(const char** s)
{
    uint32_t codepoint = 0, count = 0;
    static const uint32_t offsets[] =
    {
        0x00000000u, 0x00003080u, 0x000e2080u,
        0x03c82080u, 0xfa082080u, 0x82082080u
    };

    do
    {
        codepoint = (codepoint << 6) + (unsigned char) **s;
        (*s)++;
        count++;
    } while ((**s & 0xc0) == 0x80);

    assert(count <= 6);
    return codepoint - offsets[count - 1];
}
#endif /*X_HAVE_UTF8_STRING*/

// Convert the specified Latin-1 string to UTF-8
//
static char* convertLatin1toUTF8(const char* source)
{
    size_t size = 1;
    const char* sp;

    for (sp = source;  *sp;  sp++)
        size += (*sp & 0x80) ? 2 : 1;

    char* target = calloc(size, 1);
    char* tp = target;

    for (sp = source;  *sp;  sp++)
        tp += _glfwEncodeUTF8(tp, *sp);

    return target;
}

// Updates the cursor image according to its cursor mode
//
static void updateCursorImage(_GLFWwindow* window)
{
    if (window->cursorMode == GLFW_CURSOR_NORMAL)
    {
        if (window->cursor)
        {
            XDefineCursor(_glfw.x11.display, window->x11.handle,
                          window->cursor->x11.handle);
        }
        else
            XUndefineCursor(_glfw.x11.display, window->x11.handle);
    }
    else
    {
        XDefineCursor(_glfw.x11.display, window->x11.handle,
                      _glfw.x11.hiddenCursorHandle);
    }
}

// Enable XI2 raw mouse motion events
//
static void enableRawMouseMotion(_GLFWwindow* window)
{
    XIEventMask em;
    unsigned char mask[XIMaskLen(XI_RawMotion)] = { 0 };

    em.deviceid = XIAllMasterDevices;
    em.mask_len = sizeof(mask);
    em.mask = mask;
    XISetMask(mask, XI_RawMotion);

    XISelectEvents(_glfw.x11.display, _glfw.x11.root, &em, 1);
}

// Disable XI2 raw mouse motion events
//
static void disableRawMouseMotion(_GLFWwindow* window)
{
    XIEventMask em;
    unsigned char mask[] = { 0 };

    em.deviceid = XIAllMasterDevices;
    em.mask_len = sizeof(mask);
    em.mask = mask;

    XISelectEvents(_glfw.x11.display, _glfw.x11.root, &em, 1);
}

// Apply disabled cursor mode to a focused window
//
static void disableCursor(_GLFWwindow* window)
{
    if (window->rawMouseMotion)
        enableRawMouseMotion(window);

    _glfw.x11.disabledCursorWindow = window;
    _glfwPlatformGetCursorPos(window,
                              &_glfw.x11.restoreCursorPosX,
                              &_glfw.x11.restoreCursorPosY);
    updateCursorImage(window);
    _glfwCenterCursorInContentArea(window);
    XGrabPointer(_glfw.x11.display, window->x11.handle, True,
                 ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
                 GrabModeAsync, GrabModeAsync,
                 window->x11.handle,
                 _glfw.x11.hiddenCursorHandle,
                 CurrentTime);
}

// Exit disabled cursor mode for the specified window
//
static void enableCursor(_GLFWwindow* window)
{
    if (window->rawMouseMotion)
        disableRawMouseMotion(window);

    _glfw.x11.disabledCursorWindow = NULL;
    XUngrabPointer(_glfw.x11.display, CurrentTime);
    _glfwPlatformSetCursorPos(window,
                              _glfw.x11.restoreCursorPosX,
                              _glfw.x11.restoreCursorPosY);
    updateCursorImage(window);
}

// Create the X11 window (and its colormap)
//
static GLFWbool createNativeWindow(_GLFWwindow* window,
                                   const _GLFWwndconfig* wndconfig,
                                   Visual* visual, int depth)
{
    int width = wndconfig->width;
    int height = wndconfig->height;

    if (wndconfig->scaleToMonitor)
    {
        width *= _glfw.x11.contentScaleX;
        height *= _glfw.x11.contentScaleY;
    }

    // Create a colormap based on the visual used by the current context
    window->x11.colormap = XCreateColormap(_glfw.x11.display,
                                           _glfw.x11.root,
                                           visual,
                                           AllocNone);

    window->x11.transparent = _glfwIsVisualTransparentX11(visual);

    XSetWindowAttributes wa = { 0 };
    wa.colormap = window->x11.colormap;
    wa.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    ExposureMask | FocusChangeMask | VisibilityChangeMask |
                    EnterWindowMask | LeaveWindowMask | PropertyChangeMask;

    _glfwGrabErrorHandlerX11();

    window->x11.parent = _glfw.x11.root;
    window->x11.handle = XCreateWindow(_glfw.x11.display,
                                       _glfw.x11.root,
                                       0, 0,   // Position
                                       width, height,
                                       0,      // Border width
                                       depth,  // Color depth
                                       InputOutput,
                                       visual,
                                       CWBorderPixel | CWColormap | CWEventMask,
                                       &wa);

    _glfwReleaseErrorHandlerX11();

    if (!window->x11.handle)
    {
        _glfwInputErrorX11(GLFW_PLATFORM_ERROR,
                           "X11: Failed to create window");
        return GLFW_FALSE;
    }

    XSaveContext(_glfw.x11.display,
                 window->x11.handle,
                 _glfw.x11.context,
                 (XPointer) window);

    if (!wndconfig->decorated)
        _glfwPlatformSetWindowDecorated(window, GLFW_FALSE);

    if (_glfw.x11.NET_WM_STATE && !window->monitor)
    {
        Atom states[3];
        int count = 0;

        if (wndconfig->floating)
        {
            if (_glfw.x11.NET_WM_STATE_ABOVE)
                states[count++] = _glfw.x11.NET_WM_STATE_ABOVE;
        }

        if (wndconfig->maximized)
        {
            if (_glfw.x11.NET_WM_STATE_MAXIMIZED_VERT &&
                _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ)
            {
                states[count++] = _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT;
                states[count++] = _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ;
                window->x11.maximized = GLFW_TRUE;
            }
        }

        if (count)
        {
            XChangeProperty(_glfw.x11.display, window->x11.handle,
                            _glfw.x11.NET_WM_STATE, XA_ATOM, 32,
                            PropModeReplace, (unsigned char*) states, count);
        }
    }

    // Declare the WM protocols supported by GLFW
    {
        Atom protocols[] =
        {
            _glfw.x11.WM_DELETE_WINDOW,
            _glfw.x11.NET_WM_PING
        };

        XSetWMProtocols(_glfw.x11.display, window->x11.handle,
                        protocols, sizeof(protocols) / sizeof(Atom));
    }

    // Declare our PID
    {
        const long pid = getpid();

        XChangeProperty(_glfw.x11.display,  window->x11.handle,
                        _glfw.x11.NET_WM_PID, XA_CARDINAL, 32,
                        PropModeReplace,
                        (unsigned char*) &pid, 1);
    }

    if (_glfw.x11.NET_WM_WINDOW_TYPE && _glfw.x11.NET_WM_WINDOW_TYPE_NORMAL)
    {
        Atom type = _glfw.x11.NET_WM_WINDOW_TYPE_NORMAL;
        XChangeProperty(_glfw.x11.display,  window->x11.handle,
                        _glfw.x11.NET_WM_WINDOW_TYPE, XA_ATOM, 32,
                        PropModeReplace, (unsigned char*) &type, 1);
    }

    // Set ICCCM WM_HINTS property
    {
        XWMHints* hints = XAllocWMHints();
        if (!hints)
        {
            _glfwInputError(GLFW_OUT_OF_MEMORY,
                            "X11: Failed to allocate WM hints");
            return GLFW_FALSE;
        }

        hints->flags = StateHint;
        hints->initial_state = NormalState;

        XSetWMHints(_glfw.x11.display, window->x11.handle, hints);
        XFree(hints);
    }

    updateNormalHints(window, width, height);

    // Set ICCCM WM_CLASS property
    {
        XClassHint* hint = XAllocClassHint();

        if (strlen(wndconfig->x11.instanceName) &&
            strlen(wndconfig->x11.className))
        {
            hint->res_name = (char*) wndconfig->x11.instanceName;
            hint->res_class = (char*) wndconfig->x11.className;
        }
        else
        {
            const char* resourceName = getenv("RESOURCE_NAME");
            if (resourceName && strlen(resourceName))
                hint->res_name = (char*) resourceName;
            else if (strlen(wndconfig->title))
                hint->res_name = (char*) wndconfig->title;
            else
                hint->res_name = (char*) "glfw-application";

            if (strlen(wndconfig->title))
                hint->res_class = (char*) wndconfig->title;
            else
                hint->res_class = (char*) "GLFW-Application";
        }

        XSetClassHint(_glfw.x11.display, window->x11.handle, hint);
        XFree(hint);
    }

    // Announce support for Xdnd (drag and drop)
    {
        const Atom version = _GLFW_XDND_VERSION;
        XChangeProperty(_glfw.x11.display, window->x11.handle,
                        _glfw.x11.XdndAware, XA_ATOM, 32,
                        PropModeReplace, (unsigned char*) &version, 1);
    }

    _glfwPlatformSetWindowTitle(window, wndconfig->title);

    if (_glfw.x11.im)
    {
        window->x11.ic = XCreateIC(_glfw.x11.im,
                                   XNInputStyle,
                                   XIMPreeditNothing | XIMStatusNothing,
                                   XNClientWindow,
                                   window->x11.handle,
                                   XNFocusWindow,
                                   window->x11.handle,
                                   NULL);
    }

    if (window->x11.ic)
    {
        unsigned long filter = 0;
        if (XGetICValues(window->x11.ic, XNFilterEvents, &filter, NULL) == NULL)
            XSelectInput(_glfw.x11.display, window->x11.handle, wa.event_mask | filter);
    }

    _glfwPlatformGetWindowPos(window, &window->x11.xpos, &window->x11.ypos);
    _glfwPlatformGetWindowSize(window, &window->x11.width, &window->x11.height);

    return GLFW_TRUE;
}

// Set the specified property to the selection converted to the requested target
//
static Atom writeTargetToProperty(const XSelectionRequestEvent* request)
{
    int i;
    char* selectionString = NULL;
    const Atom formats[] = { _glfw.x11.UTF8_STRING, XA_STRING };
    const int formatCount = sizeof(formats) / sizeof(formats[0]);

    if (request->selection == _glfw.x11.PRIMARY)
        selectionString = _glfw.x11.primarySelectionString;
    else
        selectionString = _glfw.x11.clipboardString;

    if (request->property == None)
    {
        // The requester is a legacy client (ICCCM section 2.2)
        // We don't support legacy clients, so fail here
        return None;
    }

    if (request->target == _glfw.x11.TARGETS)
    {
        // The list of supported targets was requested

        const Atom targets[] = { _glfw.x11.TARGETS,
                                 _glfw.x11.MULTIPLE,
                                 _glfw.x11.UTF8_STRING,
                                 XA_STRING };

        XChangeProperty(_glfw.x11.display,
                        request->requestor,
                        request->property,
                        XA_ATOM,
                        32,
                        PropModeReplace,
                        (unsigned char*) targets,
                        sizeof(targets) / sizeof(targets[0]));

        return request->property;
    }

    if (request->target == _glfw.x11.MULTIPLE)
    {
        // Multiple conversions were requested

        Atom* targets;
        unsigned long i, count;

        count = _glfwGetWindowPropertyX11(request->requestor,
                                          request->property,
                                          _glfw.x11.ATOM_PAIR,
                                          (unsigned char**) &targets);

        for (i = 0;  i < count;  i += 2)
        {
            int j;

            for (j = 0;  j < formatCount;  j++)
            {
                if (targets[i] == formats[j])
                    break;
            }

            if (j < formatCount)
            {
                XChangeProperty(_glfw.x11.display,
                                request->requestor,
                                targets[i + 1],
                                targets[i],
                                8,
                                PropModeReplace,
                                (unsigned char *) selectionString,
                                strlen(selectionString));
            }
            else
                targets[i + 1] = None;
        }

        XChangeProperty(_glfw.x11.display,
                        request->requestor,
                        request->property,
                        _glfw.x11.ATOM_PAIR,
                        32,
                        PropModeReplace,
                        (unsigned char*) targets,
                        count);

        XFree(targets);

        return request->property;
    }

    if (request->target == _glfw.x11.SAVE_TARGETS)
    {
        // The request is a check whether we support SAVE_TARGETS
        // It should be handled as a no-op side effect target

        XChangeProperty(_glfw.x11.display,
                        request->requestor,
                        request->property,
                        _glfw.x11.NULL_,
                        32,
                        PropModeReplace,
                        NULL,
                        0);

        return request->property;
    }

    // Conversion to a data target was requested

    for (i = 0;  i < formatCount;  i++)
    {
        if (request->target == formats[i])
        {
            // The requested target is one we support

            XChangeProperty(_glfw.x11.display,
                            request->requestor,
                            request->property,
                            request->target,
                            8,
                            PropModeReplace,
                            (unsigned char *) selectionString,
                            strlen(selectionString));

            return request->property;
        }
    }

    // The requested target is not supported

    return None;
}

static void handleSelectionRequest(XEvent* event)
{
    const XSelectionRequestEvent* request = &event->xselectionrequest;

    XEvent reply = { SelectionNotify };
    reply.xselection.property = writeTargetToProperty(request);
    reply.xselection.display = request->display;
    reply.xselection.requestor = request->requestor;
    reply.xselection.selection = request->selection;
    reply.xselection.target = request->target;
    reply.xselection.time = request->time;

    XSendEvent(_glfw.x11.display, request->requestor, False, 0, &reply);
}

static const char* getSelectionString(Atom selection)
{
    char** selectionString = NULL;
    const Atom targets[] = { _glfw.x11.UTF8_STRING, XA_STRING };
    const size_t targetCount = sizeof(targets) / sizeof(targets[0]);

    if (selection == _glfw.x11.PRIMARY)
        selectionString = &_glfw.x11.primarySelectionString;
    else
        selectionString = &_glfw.x11.clipboardString;

    if (XGetSelectionOwner(_glfw.x11.display, selection) ==
        _glfw.x11.helperWindowHandle)
    {
        // Instead of doing a large number of X round-trips just to put this
        // string into a window property and then read it back, just return it
        return *selectionString;
    }

    free(*selectionString);
    *selectionString = NULL;

    for (size_t i = 0;  i < targetCount;  i++)
    {
        char* data;
        Atom actualType;
        int actualFormat;
        unsigned long itemCount, bytesAfter;
        XEvent notification, dummy;

        XConvertSelection(_glfw.x11.display,
                          selection,
                          targets[i],
                          _glfw.x11.GLFW_SELECTION,
                          _glfw.x11.helperWindowHandle,
                          CurrentTime);

        while (!XCheckTypedWindowEvent(_glfw.x11.display,
                                       _glfw.x11.helperWindowHandle,
                                       SelectionNotify,
                                       &notification))
        {
            waitForX11Event(NULL);
        }

        if (notification.xselection.property == None)
            continue;

        XCheckIfEvent(_glfw.x11.display,
                      &dummy,
                      isSelPropNewValueNotify,
                      (XPointer) &notification);

        XGetWindowProperty(_glfw.x11.display,
                           notification.xselection.requestor,
                           notification.xselection.property,
                           0,
                           LONG_MAX,
                           True,
                           AnyPropertyType,
                           &actualType,
                           &actualFormat,
                           &itemCount,
                           &bytesAfter,
                           (unsigned char**) &data);

        if (actualType == _glfw.x11.INCR)
        {
            size_t size = 1;
            char* string = NULL;

            for (;;)
            {
                while (!XCheckIfEvent(_glfw.x11.display,
                                      &dummy,
                                      isSelPropNewValueNotify,
                                      (XPointer) &notification))
                {
                    waitForX11Event(NULL);
                }

                XFree(data);
                XGetWindowProperty(_glfw.x11.display,
                                   notification.xselection.requestor,
                                   notification.xselection.property,
                                   0,
                                   LONG_MAX,
                                   True,
                                   AnyPropertyType,
                                   &actualType,
                                   &actualFormat,
                                   &itemCount,
                                   &bytesAfter,
                                   (unsigned char**) &data);

                if (itemCount)
                {
                    size += itemCount;
                    string = realloc(string, size);
                    string[size - itemCount - 1] = '\0';
                    strcat(string, data);
                }

                if (!itemCount)
                {
                    if (string)
                    {
                        if (targets[i] == XA_STRING)
                        {
                            *selectionString = convertLatin1toUTF8(string);
                            free(string);
                        }
                        else
                            *selectionString = string;
                    }

                    break;
                }
            }
        }
        else if (actualType == targets[i])
        {
            if (targets[i] == XA_STRING)
                *selectionString = convertLatin1toUTF8(data);
            else
                *selectionString = _glfw_strdup(data);
        }

        XFree(data);

        if (*selectionString)
            break;
    }

    if (!*selectionString)
    {
        _glfwInputError(GLFW_FORMAT_UNAVAILABLE,
                        "X11: Failed to convert selection to string");
    }

    return *selectionString;
}

// Make the specified window and its video mode active on its monitor
//
static void acquireMonitor(_GLFWwindow* window)
{
    if (_glfw.x11.saver.count == 0)
    {
        // Remember old screen saver settings
        XGetScreenSaver(_glfw.x11.display,
                        &_glfw.x11.saver.timeout,
                        &_glfw.x11.saver.interval,
                        &_glfw.x11.saver.blanking,
                        &_glfw.x11.saver.exposure);

        // Disable screen saver
        XSetScreenSaver(_glfw.x11.display, 0, 0, DontPreferBlanking,
                        DefaultExposures);
    }

    if (!window->monitor->window)
        _glfw.x11.saver.count++;

    _glfwSetVideoModeX11(window->monitor, &window->videoMode);

    if (window->x11.overrideRedirect)
    {
        int xpos, ypos;
        GLFWvidmode mode;

        // Manually position the window over its monitor
        _glfwPlatformGetMonitorPos(window->monitor, &xpos, &ypos);
        _glfwPlatformGetVideoMode(window->monitor, &mode);

        XMoveResizeWindow(_glfw.x11.display, window->x11.handle,
                          xpos, ypos, mode.width, mode.height);
    }

    _glfwInputMonitorWindow(window->monitor, window);
}

// Remove the window and restore the original video mode
//
static void releaseMonitor(_GLFWwindow* window)
{
    if (window->monitor->window != window)
        return;

    _glfwInputMonitorWindow(window->monitor, NULL);
    _glfwRestoreVideoModeX11(window->monitor);

    _glfw.x11.saver.count--;

    if (_glfw.x11.saver.count == 0)
    {
        // Restore old screen saver settings
        XSetScreenSaver(_glfw.x11.display,
                        _glfw.x11.saver.timeout,
                        _glfw.x11.saver.interval,
                        _glfw.x11.saver.blanking,
                        _glfw.x11.saver.exposure);
    }
}

// Process the specified X event
//
static void processEvent(XEvent *event)
{
    int keycode = 0;
    Bool filtered = False;

    // HACK: Save scancode as some IMs clear the field in XFilterEvent
    if (event->type == KeyPress || event->type == KeyRelease)
        keycode = event->xkey.keycode;

    if (_glfw.x11.im)
        filtered = XFilterEvent(event, None);

    if (_glfw.x11.randr.available)
    {
        if (event->type == _glfw.x11.randr.eventBase + RRNotify)
        {
            XRRUpdateConfiguration(event);
            _glfwPollMonitorsX11();
            return;
        }
    }

    if (_glfw.x11.xkb.available)
    {
        if (event->type == _glfw.x11.xkb.eventBase + XkbEventCode)
        {
            if (((XkbEvent*) event)->any.xkb_type == XkbStateNotify &&
                (((XkbEvent*) event)->state.changed & XkbGroupStateMask))
            {
                _glfw.x11.xkb.group = ((XkbEvent*) event)->state.group;
            }

            return;
        }
    }

    if (event->type == GenericEvent)
    {
        if (_glfw.x11.xi.available)
        {
            _GLFWwindow* window = _glfw.x11.disabledCursorWindow;

            if (window &&
                window->rawMouseMotion &&
                event->xcookie.extension == _glfw.x11.xi.majorOpcode &&
                XGetEventData(_glfw.x11.display, &event->xcookie) &&
                event->xcookie.evtype == XI_RawMotion)
            {
                XIRawEvent* re = event->xcookie.data;
                if (re->valuators.mask_len)
                {
                    const double* values = re->raw_values;
                    double xpos = window->virtualCursorPosX;
                    double ypos = window->virtualCursorPosY;

                    if (XIMaskIsSet(re->valuators.mask, 0))
                    {
                        xpos += *values;
                        values++;
                    }

                    if (XIMaskIsSet(re->valuators.mask, 1))
                        ypos += *values;

                    _glfwInputCursorPos(window, xpos, ypos);
                }
            }

            XFreeEventData(_glfw.x11.display, &event->xcookie);
        }

        return;
    }

    if (event->type == SelectionRequest)
    {
        handleSelectionRequest(event);
        return;
    }

    _GLFWwindow* window = NULL;
    if (XFindContext(_glfw.x11.display,
                     event->xany.window,
                     _glfw.x11.context,
                     (XPointer*) &window) != 0)
    {
        // This is an event for a window that has already been destroyed
        return;
    }

    switch (event->type)
    {
        case ReparentNotify:
        {
            window->x11.parent = event->xreparent.parent;
            return;
        }

        case KeyPress:
        {
            const int key = translateKey(keycode);
            const int mods = translateState(event->xkey.state);
            const int plain = !(mods & (GLFW_MOD_CONTROL | GLFW_MOD_ALT));

            if (window->x11.ic)
            {
                // HACK: Do not report the key press events duplicated by XIM
                //       Duplicate key releases are filtered out implicitly by
                //       the GLFW key repeat logic in _glfwInputKey
                //       A timestamp per key is used to handle simultaneous keys
                // NOTE: Always allow the first event for each key through
                //       (the server never sends a timestamp of zero)
                // NOTE: Timestamp difference is compared to handle wrap-around
                Time diff = event->xkey.time - window->x11.keyPressTimes[keycode];
                if (diff == event->xkey.time || (diff > 0 && diff < ((Time)1 << 31)))
                {
                    if (keycode)
                        _glfwInputKey(window, key, keycode, GLFW_PRESS, mods);

                    window->x11.keyPressTimes[keycode] = event->xkey.time;
                }

                if (!filtered)
                {
                    int count;
                    Status status;
#if defined(X_HAVE_UTF8_STRING)
                    char buffer[100];
                    char* chars = buffer;

                    count = Xutf8LookupString(window->x11.ic,
                                              &event->xkey,
                                              buffer, sizeof(buffer) - 1,
                                              NULL, &status);

                    if (status == XBufferOverflow)
                    {
                        chars = calloc(count + 1, 1);
                        count = Xutf8LookupString(window->x11.ic,
                                                  &event->xkey,
                                                  chars, count,
                                                  NULL, &status);
                    }

                    if (status == XLookupChars || status == XLookupBoth)
                    {
                        const char* c = chars;
                        chars[count] = '\0';
                        while (c - chars < count)
                            _glfwInputChar(window, decodeUTF8(&c), mods, plain);
                    }
#else /*X_HAVE_UTF8_STRING*/
                    wchar_t buffer[16];
                    wchar_t* chars = buffer;

                    count = XwcLookupString(window->x11.ic,
                                            &event->xkey,
                                            buffer,
                                            sizeof(buffer) / sizeof(wchar_t),
                                            NULL,
                                            &status);

                    if (status == XBufferOverflow)
                    {
                        chars = calloc(count, sizeof(wchar_t));
                        count = XwcLookupString(window->x11.ic,
                                                &event->xkey,
                                                chars, count,
                                                NULL, &status);
                    }

                    if (status == XLookupChars || status == XLookupBoth)
                    {
                        int i;
                        for (i = 0;  i < count;  i++)
                            _glfwInputChar(window, chars[i], mods, plain);
                    }
#endif /*X_HAVE_UTF8_STRING*/

                    if (chars != buffer)
                        free(chars);
                }
            }
            else
            {
                KeySym keysym;
                XLookupString(&event->xkey, NULL, 0, &keysym, NULL);

                _glfwInputKey(window, key, keycode, GLFW_PRESS, mods);

                const uint32_t codepoint = _glfwKeySym2Unicode(keysym);
                if (codepoint != GLFW_INVALID_CODEPOINT)
                    _glfwInputChar(window, codepoint, mods, plain);
            }

            return;
        }

        case KeyRelease:
        {
            const int key = translateKey(keycode);
            const int mods = translateState(event->xkey.state);

            if (!_glfw.x11.xkb.detectable)
            {
                // HACK: Key repeat events will arrive as KeyRelease/KeyPress
                //       pairs with similar or identical time stamps
                //       The key repeat logic in _glfwInputKey expects only key
                //       presses to repeat, so detect and discard release events
                if (XEventsQueued(_glfw.x11.display, QueuedAfterReading))
                {
                    XEvent next;
                    XPeekEvent(_glfw.x11.display, &next);

                    if (next.type == KeyPress &&
                        next.xkey.window == event->xkey.window &&
                        next.xkey.keycode == keycode)
                    {
                        // HACK: The time of repeat events sometimes doesn't
                        //       match that of the press event, so add an
                        //       epsilon
                        //       Toshiyuki Takahashi can press a button
                        //       16 times per second so it's fairly safe to
                        //       assume that no human is pressing the key 50
                        //       times per second (value is ms)
                        if ((next.xkey.time - event->xkey.time) < 20)
                        {
                            // This is very likely a server-generated key repeat
                            // event, so ignore it
                            return;
                        }
                    }
                }
            }

            _glfwInputKey(window, key, keycode, GLFW_RELEASE, mods);
            return;
        }

        case ButtonPress:
        {
            const int mods = translateState(event->xbutton.state);

            if (event->xbutton.button == Button1)
                _glfwInputMouseClick(window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, mods);
            else if (event->xbutton.button == Button2)
                _glfwInputMouseClick(window, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, mods);
            else if (event->xbutton.button == Button3)
                _glfwInputMouseClick(window, GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, mods);

            // Modern X provides scroll events as mouse button presses
            else if (event->xbutton.button == Button4)
                _glfwInputScroll(window, 0.0, 1.0);
            else if (event->xbutton.button == Button5)
                _glfwInputScroll(window, 0.0, -1.0);
            else if (event->xbutton.button == Button6)
                _glfwInputScroll(window, 1.0, 0.0);
            else if (event->xbutton.button == Button7)
                _glfwInputScroll(window, -1.0, 0.0);

            else
            {
                // Additional buttons after 7 are treated as regular buttons
                // We subtract 4 to fill the gap left by scroll input above
                _glfwInputMouseClick(window,
                                     event->xbutton.button - Button1 - 4,
                                     GLFW_PRESS,
                                     mods);
            }

            return;
        }

        case ButtonRelease:
        {
            const int mods = translateState(event->xbutton.state);

            if (event->xbutton.button == Button1)
            {
                _glfwInputMouseClick(window,
                                     GLFW_MOUSE_BUTTON_LEFT,
                                     GLFW_RELEASE,
                                     mods);
            }
            else if (event->xbutton.button == Button2)
            {
                _glfwInputMouseClick(window,
                                     GLFW_MOUSE_BUTTON_MIDDLE,
                                     GLFW_RELEASE,
                                     mods);
            }
            else if (event->xbutton.button == Button3)
            {
                _glfwInputMouseClick(window,
                                     GLFW_MOUSE_BUTTON_RIGHT,
                                     GLFW_RELEASE,
                                     mods);
            }
            else if (event->xbutton.button > Button7)
            {
                // Additional buttons after 7 are treated as regular buttons
                // We subtract 4 to fill the gap left by scroll input above
                _glfwInputMouseClick(window,
                                     event->xbutton.button - Button1 - 4,
                                     GLFW_RELEASE,
                                     mods);
            }

            return;
        }

        case EnterNotify:
        {
            // XEnterWindowEvent is XCrossingEvent
            const int x = event->xcrossing.x;
            const int y = event->xcrossing.y;

            // HACK: This is a workaround for WMs (KWM, Fluxbox) that otherwise
            //       ignore the defined cursor for hidden cursor mode
            if (window->cursorMode == GLFW_CURSOR_HIDDEN)
                updateCursorImage(window);

            _glfwInputCursorEnter(window, GLFW_TRUE);
            _glfwInputCursorPos(window, x, y);

            window->x11.lastCursorPosX = x;
            window->x11.lastCursorPosY = y;
            return;
        }

        case LeaveNotify:
        {
            _glfwInputCursorEnter(window, GLFW_FALSE);
            return;
        }

        case MotionNotify:
        {
            const int x = event->xmotion.x;
            const int y = event->xmotion.y;

            if (x != window->x11.warpCursorPosX ||
                y != window->x11.warpCursorPosY)
            {
                // The cursor was moved by something other than GLFW

                if (window->cursorMode == GLFW_CURSOR_DISABLED)
                {
                    if (_glfw.x11.disabledCursorWindow != window)
                        return;
                    if (window->rawMouseMotion)
                        return;

                    const int dx = x - window->x11.lastCursorPosX;
                    const int dy = y - window->x11.lastCursorPosY;

                    _glfwInputCursorPos(window,
                                        window->virtualCursorPosX + dx,
                                        window->virtualCursorPosY + dy);
                }
                else
                    _glfwInputCursorPos(window, x, y);
            }

            window->x11.lastCursorPosX = x;
            window->x11.lastCursorPosY = y;
            return;
        }

        case ConfigureNotify:
        {
            if (event->xconfigure.width != window->x11.width ||
                event->xconfigure.height != window->x11.height)
            {
                _glfwInputFramebufferSize(window,
                                          event->xconfigure.width,
                                          event->xconfigure.height);

                _glfwInputWindowSize(window,
                                     event->xconfigure.width,
                                     event->xconfigure.height);

                window->x11.width = event->xconfigure.width;
                window->x11.height = event->xconfigure.height;
            }

            int xpos = event->xconfigure.x;
            int ypos = event->xconfigure.y;

            // NOTE: ConfigureNotify events from the server are in local
            //       coordinates, so if we are reparented we need to translate
            //       the position into root (screen) coordinates
            if (!event->xany.send_event && window->x11.parent != _glfw.x11.root)
            {
                _glfwGrabErrorHandlerX11();

                Window dummy;
                XTranslateCoordinates(_glfw.x11.display,
                                      window->x11.parent,
                                      _glfw.x11.root,
                                      xpos, ypos,
                                      &xpos, &ypos,
                                      &dummy);

                _glfwReleaseErrorHandlerX11();
                if (_glfw.x11.errorCode == BadWindow)
                    return;
            }

            if (xpos != window->x11.xpos || ypos != window->x11.ypos)
            {
                _glfwInputWindowPos(window, xpos, ypos);
                window->x11.xpos = xpos;
                window->x11.ypos = ypos;
            }

            return;
        }

        case ClientMessage:
        {
            // Custom client message, probably from the window manager

            if (filtered)
                return;

            if (event->xclient.message_type == None)
                return;

            if (event->xclient.message_type == _glfw.x11.WM_PROTOCOLS)
            {
                const Atom protocol = event->xclient.data.l[0];
                if (protocol == None)
                    return;

                if (protocol == _glfw.x11.WM_DELETE_WINDOW)
                {
                    // The window manager was asked to close the window, for
                    // example by the user pressing a 'close' window decoration
                    // button
                    _glfwInputWindowCloseRequest(window);
                }
                else if (protocol == _glfw.x11.NET_WM_PING)
                {
                    // The window manager is pinging the application to ensure
                    // it's still responding to events

                    XEvent reply = *event;
                    reply.xclient.window = _glfw.x11.root;

                    XSendEvent(_glfw.x11.display, _glfw.x11.root,
                               False,
                               SubstructureNotifyMask | SubstructureRedirectMask,
                               &reply);
                }
            }
            else if (event->xclient.message_type == _glfw.x11.XdndEnter)
            {
                // A drag operation has entered the window
                unsigned long i, count;
                Atom* formats = NULL;
                const GLFWbool list = event->xclient.data.l[1] & 1;

                _glfw.x11.xdnd.source  = event->xclient.data.l[0];
                _glfw.x11.xdnd.version = event->xclient.data.l[1] >> 24;
                _glfw.x11.xdnd.format  = None;

                if (_glfw.x11.xdnd.version > _GLFW_XDND_VERSION)
                    return;

                if (list)
                {
                    count = _glfwGetWindowPropertyX11(_glfw.x11.xdnd.source,
                                                      _glfw.x11.XdndTypeList,
                                                      XA_ATOM,
                                                      (unsigned char**) &formats);
                }
                else
                {
                    count = 3;
                    formats = (Atom*) event->xclient.data.l + 2;
                }

                for (i = 0;  i < count;  i++)
                {
                    if (formats[i] == _glfw.x11.text_uri_list)
                    {
                        _glfw.x11.xdnd.format = _glfw.x11.text_uri_list;
                        break;
                    }
                }

                if (list && formats)
                    XFree(formats);
            }
            else if (event->xclient.message_type == _glfw.x11.XdndDrop)
            {
                // The drag operation has finished by dropping on the window
                Time time = CurrentTime;

                if (_glfw.x11.xdnd.version > _GLFW_XDND_VERSION)
                    return;

                if (_glfw.x11.xdnd.format)
                {
                    if (_glfw.x11.xdnd.version >= 1)
                        time = event->xclient.data.l[2];

                    // Request the chosen format from the source window
                    XConvertSelection(_glfw.x11.display,
                                      _glfw.x11.XdndSelection,
                                      _glfw.x11.xdnd.format,
                                      _glfw.x11.XdndSelection,
                                      window->x11.handle,
                                      time);
                }
                else if (_glfw.x11.xdnd.version >= 2)
                {
                    XEvent reply = { ClientMessage };
                    reply.xclient.window = _glfw.x11.xdnd.source;
                    reply.xclient.message_type = _glfw.x11.XdndFinished;
                    reply.xclient.format = 32;
                    reply.xclient.data.l[0] = window->x11.handle;
                    reply.xclient.data.l[1] = 0; // The drag was rejected
                    reply.xclient.data.l[2] = None;

                    XSendEvent(_glfw.x11.display, _glfw.x11.xdnd.source,
                               False, NoEventMask, &reply);
                    XFlush(_glfw.x11.display);
                }
            }
            else if (event->xclient.message_type == _glfw.x11.XdndPosition)
            {
                // The drag operation has moved over the window
                const int xabs = (event->xclient.data.l[2] >> 16) & 0xffff;
                const int yabs = (event->xclient.data.l[2]) & 0xffff;
                Window dummy;
                int xpos, ypos;

                if (_glfw.x11.xdnd.version > _GLFW_XDND_VERSION)
                    return;

                XTranslateCoordinates(_glfw.x11.display,
                                      _glfw.x11.root,
                                      window->x11.handle,
                                      xabs, yabs,
                                      &xpos, &ypos,
                                      &dummy);

                _glfwInputCursorPos(window, xpos, ypos);

                XEvent reply = { ClientMessage };
                reply.xclient.window = _glfw.x11.xdnd.source;
                reply.xclient.message_type = _glfw.x11.XdndStatus;
                reply.xclient.format = 32;
                reply.xclient.data.l[0] = window->x11.handle;
                reply.xclient.data.l[2] = 0; // Specify an empty rectangle
                reply.xclient.data.l[3] = 0;

                if (_glfw.x11.xdnd.format)
                {
                    // Reply that we are ready to copy the dragged data
                    reply.xclient.data.l[1] = 1; // Accept with no rectangle
                    if (_glfw.x11.xdnd.version >= 2)
                        reply.xclient.data.l[4] = _glfw.x11.XdndActionCopy;
                }

                XSendEvent(_glfw.x11.display, _glfw.x11.xdnd.source,
                           False, NoEventMask, &reply);
                XFlush(_glfw.x11.display);
            }

            return;
        }

        case SelectionNotify:
        {
            if (event->xselection.property == _glfw.x11.XdndSelection)
            {
                // The converted data from the drag operation has arrived
                char* data;
                const unsigned long result =
                    _glfwGetWindowPropertyX11(event->xselection.requestor,
                                              event->xselection.property,
                                              event->xselection.target,
                                              (unsigned char**) &data);

                if (result)
                {
                    int i, count;
                    char** paths = _glfwParseUriList(data, &count);

                    _glfwInputDrop(window, count, (const char**) paths);

                    for (i = 0;  i < count;  i++)
                        free(paths[i]);
                    free(paths);
                }

                if (data)
                    XFree(data);

                if (_glfw.x11.xdnd.version >= 2)
                {
                    XEvent reply = { ClientMessage };
                    reply.xclient.window = _glfw.x11.xdnd.source;
                    reply.xclient.message_type = _glfw.x11.XdndFinished;
                    reply.xclient.format = 32;
                    reply.xclient.data.l[0] = window->x11.handle;
                    reply.xclient.data.l[1] = result;
                    reply.xclient.data.l[2] = _glfw.x11.XdndActionCopy;

                    XSendEvent(_glfw.x11.display, _glfw.x11.xdnd.source,
                               False, NoEventMask, &reply);
                    XFlush(_glfw.x11.display);
                }
            }

            return;
        }

        case FocusIn:
        {
            if (event->xfocus.mode == NotifyGrab ||
                event->xfocus.mode == NotifyUngrab)
            {
                // Ignore focus events from popup indicator windows, window menu
                // key chords and window dragging
                return;
            }

            if (window->cursorMode == GLFW_CURSOR_DISABLED)
                disableCursor(window);

            if (window->x11.ic)
                XSetICFocus(window->x11.ic);

            _glfwInputWindowFocus(window, GLFW_TRUE);
            return;
        }

        case FocusOut:
        {
            if (event->xfocus.mode == NotifyGrab ||
                event->xfocus.mode == NotifyUngrab)
            {
                // Ignore focus events from popup indicator windows, window menu
                // key chords and window dragging
                return;
            }

            if (window->cursorMode == GLFW_CURSOR_DISABLED)
                enableCursor(window);

            if (window->x11.ic)
                XUnsetICFocus(window->x11.ic);

            if (window->monitor && window->autoIconify)
                _glfwPlatformIconifyWindow(window);

            _glfwInputWindowFocus(window, GLFW_FALSE);
            return;
        }

        case Expose:
        {
            _glfwInputWindowDamage(window);
            return;
        }

        case PropertyNotify:
        {
            if (event->xproperty.state != PropertyNewValue)
                return;

            if (event->xproperty.atom == _glfw.x11.WM_STATE)
            {
                const int state = getWindowState(window);
                if (state != IconicState && state != NormalState)
                    return;

                const GLFWbool iconified = (state == IconicState);
                if (window->x11.iconified != iconified)
                {
                    if (window->monitor)
                    {
                        if (iconified)
                            releaseMonitor(window);
                        else
                            acquireMonitor(window);
                    }

                    window->x11.iconified = iconified;
                    _glfwInputWindowIconify(window, iconified);
                }
            }
            else if (event->xproperty.atom == _glfw.x11.NET_WM_STATE)
            {
                const GLFWbool maximized = _glfwPlatformWindowMaximized(window);
                if (window->x11.maximized != maximized)
                {
                    window->x11.maximized = maximized;
                    _glfwInputWindowMaximize(window, maximized);
                }
            }

            return;
        }

        case DestroyNotify:
            return;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Retrieve a single window property of the specified type
// Inspired by fghGetWindowProperty from freeglut
//
unsigned long _glfwGetWindowPropertyX11(Window window,
                                        Atom property,
                                        Atom type,
                                        unsigned char** value)
{
    Atom actualType;
    int actualFormat;
    unsigned long itemCount, bytesAfter;

    XGetWindowProperty(_glfw.x11.display,
                       window,
                       property,
                       0,
                       LONG_MAX,
                       False,
                       type,
                       &actualType,
                       &actualFormat,
                       &itemCount,
                       &bytesAfter,
                       value);

    return itemCount;
}

GLFWbool _glfwIsVisualTransparentX11(Visual* visual)
{
    if (!_glfw.x11.xrender.available)
        return GLFW_FALSE;

    XRenderPictFormat* pf = XRenderFindVisualFormat(_glfw.x11.display, visual);
    return pf && pf->direct.alphaMask;
}

// Push contents of our selection to clipboard manager
//
void _glfwPushSelectionToManagerX11(void)
{
    XConvertSelection(_glfw.x11.display,
                      _glfw.x11.CLIPBOARD_MANAGER,
                      _glfw.x11.SAVE_TARGETS,
                      None,
                      _glfw.x11.helperWindowHandle,
                      CurrentTime);

    for (;;)
    {
        XEvent event;

        while (XCheckIfEvent(_glfw.x11.display, &event, isSelectionEvent, NULL))
        {
            switch (event.type)
            {
                case SelectionRequest:
                    handleSelectionRequest(&event);
                    break;

                case SelectionNotify:
                {
                    if (event.xselection.target == _glfw.x11.SAVE_TARGETS)
                    {
                        // This means one of two things; either the selection
                        // was not owned, which means there is no clipboard
                        // manager, or the transfer to the clipboard manager has
                        // completed
                        // In either case, it means we are done here
                        return;
                    }

                    break;
                }
            }
        }

        waitForX11Event(NULL);
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformCreateWindow(_GLFWwindow* window,
                              const _GLFWwndconfig* wndconfig,
                              const _GLFWctxconfig* ctxconfig,
                              const _GLFWfbconfig* fbconfig)
{
    Visual* visual = NULL;
    int depth;

    if (ctxconfig->client != GLFW_NO_API)
    {
        if (ctxconfig->source == GLFW_NATIVE_CONTEXT_API)
        {
            if (!_glfwInitGLX())
                return GLFW_FALSE;
            if (!_glfwChooseVisualGLX(wndconfig, ctxconfig, fbconfig, &visual, &depth))
                return GLFW_FALSE;
        }
        else if (ctxconfig->source == GLFW_EGL_CONTEXT_API)
        {
            if (!_glfwInitEGL())
                return GLFW_FALSE;
            if (!_glfwChooseVisualEGL(wndconfig, ctxconfig, fbconfig, &visual, &depth))
                return GLFW_FALSE;
        }
        else if (ctxconfig->source == GLFW_OSMESA_CONTEXT_API)
        {
            if (!_glfwInitOSMesa())
                return GLFW_FALSE;
        }
    }

    if (!visual)
    {
        visual = DefaultVisual(_glfw.x11.display, _glfw.x11.screen);
        depth = DefaultDepth(_glfw.x11.display, _glfw.x11.screen);
    }

    if (!createNativeWindow(window, wndconfig, visual, depth))
        return GLFW_FALSE;

    if (ctxconfig->client != GLFW_NO_API)
    {
        if (ctxconfig->source == GLFW_NATIVE_CONTEXT_API)
        {
            if (!_glfwCreateContextGLX(window, ctxconfig, fbconfig))
                return GLFW_FALSE;
        }
        else if (ctxconfig->source == GLFW_EGL_CONTEXT_API)
        {
            if (!_glfwCreateContextEGL(window, ctxconfig, fbconfig))
                return GLFW_FALSE;
        }
        else if (ctxconfig->source == GLFW_OSMESA_CONTEXT_API)
        {
            if (!_glfwCreateContextOSMesa(window, ctxconfig, fbconfig))
                return GLFW_FALSE;
        }

        if (!_glfwRefreshContextAttribs(window, ctxconfig))
            return GLFW_FALSE;
    }

    if (window->monitor)
    {
        _glfwPlatformShowWindow(window);
        updateWindowMode(window);
        acquireMonitor(window);

        if (wndconfig->centerCursor)
            _glfwCenterCursorInContentArea(window);
    }
    else
    {
        if (wndconfig->visible)
        {
            _glfwPlatformShowWindow(window);
            if (wndconfig->focused)
                _glfwPlatformFocusWindow(window);
        }
    }

    XFlush(_glfw.x11.display);
    return GLFW_TRUE;
}

void _glfwPlatformDestroyWindow(_GLFWwindow* window)
{
    if (_glfw.x11.disabledCursorWindow == window)
        _glfw.x11.disabledCursorWindow = NULL;

    if (window->monitor)
        releaseMonitor(window);

    if (window->x11.ic)
    {
        XDestroyIC(window->x11.ic);
        window->x11.ic = NULL;
    }

    if (window->context.destroy)
        window->context.destroy(window);

    if (window->x11.handle)
    {
        XDeleteContext(_glfw.x11.display, window->x11.handle, _glfw.x11.context);
        XUnmapWindow(_glfw.x11.display, window->x11.handle);
        XDestroyWindow(_glfw.x11.display, window->x11.handle);
        window->x11.handle = (Window) 0;
    }

    if (window->x11.colormap)
    {
        XFreeColormap(_glfw.x11.display, window->x11.colormap);
        window->x11.colormap = (Colormap) 0;
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformSetWindowTitle(_GLFWwindow* window, const char* title)
{
#if defined(X_HAVE_UTF8_STRING)
    Xutf8SetWMProperties(_glfw.x11.display,
                         window->x11.handle,
                         title, title,
                         NULL, 0,
                         NULL, NULL, NULL);
#else
    // This may be a slightly better fallback than using XStoreName and
    // XSetIconName, which always store their arguments using STRING
    XmbSetWMProperties(_glfw.x11.display,
                       window->x11.handle,
                       title, title,
                       NULL, 0,
                       NULL, NULL, NULL);
#endif

    XChangeProperty(_glfw.x11.display,  window->x11.handle,
                    _glfw.x11.NET_WM_NAME, _glfw.x11.UTF8_STRING, 8,
                    PropModeReplace,
                    (unsigned char*) title, strlen(title));

    XChangeProperty(_glfw.x11.display,  window->x11.handle,
                    _glfw.x11.NET_WM_ICON_NAME, _glfw.x11.UTF8_STRING, 8,
                    PropModeReplace,
                    (unsigned char*) title, strlen(title));

    XFlush(_glfw.x11.display);
}

void _glfwPlatformSetWindowIcon(_GLFWwindow* window,
                                int count, const GLFWimage* images)
{
    if (count)
    {
        int i, j, longCount = 0;

        for (i = 0;  i < count;  i++)
            longCount += 2 + images[i].width * images[i].height;

        unsigned long* icon = calloc(longCount, sizeof(unsigned long));
        unsigned long* target = icon;

        for (i = 0;  i < count;  i++)
        {
            *target++ = images[i].width;
            *target++ = images[i].height;

            for (j = 0;  j < images[i].width * images[i].height;  j++)
            {
                *target++ = (((unsigned long) images[i].pixels[j * 4 + 0]) << 16) |
                            (((unsigned long) images[i].pixels[j * 4 + 1]) <<  8) |
                            (((unsigned long) images[i].pixels[j * 4 + 2]) <<  0) |
                            (((unsigned long) images[i].pixels[j * 4 + 3]) << 24);
            }
        }

        // NOTE: XChangeProperty expects 32-bit values like the image data above to be
        //       placed in the 32 least significant bits of individual longs.  This is
        //       true even if long is 64-bit and a WM protocol calls for "packed" data.
        //       This is because of a historical mistake that then became part of the Xlib
        //       ABI.  Xlib will pack these values into a regular array of 32-bit values
        //       before sending it over the wire.
        XChangeProperty(_glfw.x11.display, window->x11.handle,
                        _glfw.x11.NET_WM_ICON,
                        XA_CARDINAL, 32,
                        PropModeReplace,
                        (unsigned char*) icon,
                        longCount);

        free(icon);
    }
    else
    {
        XDeleteProperty(_glfw.x11.display, window->x11.handle,
                        _glfw.x11.NET_WM_ICON);
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformGetWindowPos(_GLFWwindow* window, int* xpos, int* ypos)
{
    Window dummy;
    int x, y;

    XTranslateCoordinates(_glfw.x11.display, window->x11.handle, _glfw.x11.root,
                          0, 0, &x, &y, &dummy);

    if (xpos)
        *xpos = x;
    if (ypos)
        *ypos = y;
}

void _glfwPlatformSetWindowPos(_GLFWwindow* window, int xpos, int ypos)
{
    // HACK: Explicitly setting PPosition to any value causes some WMs, notably
    //       Compiz and Metacity, to honor the position of unmapped windows
    if (!_glfwPlatformWindowVisible(window))
    {
        long supplied;
        XSizeHints* hints = XAllocSizeHints();

        if (XGetWMNormalHints(_glfw.x11.display, window->x11.handle, hints, &supplied))
        {
            hints->flags |= PPosition;
            hints->x = hints->y = 0;

            XSetWMNormalHints(_glfw.x11.display, window->x11.handle, hints);
        }

        XFree(hints);
    }

    XMoveWindow(_glfw.x11.display, window->x11.handle, xpos, ypos);
    XFlush(_glfw.x11.display);
}

void _glfwPlatformGetWindowSize(_GLFWwindow* window, int* width, int* height)
{
    XWindowAttributes attribs;
    XGetWindowAttributes(_glfw.x11.display, window->x11.handle, &attribs);

    if (width)
        *width = attribs.width;
    if (height)
        *height = attribs.height;
}

void _glfwPlatformSetWindowSize(_GLFWwindow* window, int width, int height)
{
    if (window->monitor)
    {
        if (window->monitor->window == window)
            acquireMonitor(window);
    }
    else
    {
        if (!window->resizable)
            updateNormalHints(window, width, height);

        XResizeWindow(_glfw.x11.display, window->x11.handle, width, height);
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformSetWindowSizeLimits(_GLFWwindow* window,
                                      int minwidth, int minheight,
                                      int maxwidth, int maxheight)
{
    int width, height;
    _glfwPlatformGetWindowSize(window, &width, &height);
    updateNormalHints(window, width, height);
    XFlush(_glfw.x11.display);
}

void _glfwPlatformSetWindowAspectRatio(_GLFWwindow* window, int numer, int denom)
{
    int width, height;
    _glfwPlatformGetWindowSize(window, &width, &height);
    updateNormalHints(window, width, height);
    XFlush(_glfw.x11.display);
}

void _glfwPlatformGetFramebufferSize(_GLFWwindow* window, int* width, int* height)
{
    _glfwPlatformGetWindowSize(window, width, height);
}

void _glfwPlatformGetWindowFrameSize(_GLFWwindow* window,
                                     int* left, int* top,
                                     int* right, int* bottom)
{
    long* extents = NULL;

    if (window->monitor || !window->decorated)
        return;

    if (_glfw.x11.NET_FRAME_EXTENTS == None)
        return;

    if (!_glfwPlatformWindowVisible(window) &&
        _glfw.x11.NET_REQUEST_FRAME_EXTENTS)
    {
        XEvent event;
        double timeout = 0.5;

        // Ensure _NET_FRAME_EXTENTS is set, allowing glfwGetWindowFrameSize to
        // function before the window is mapped
        sendEventToWM(window, _glfw.x11.NET_REQUEST_FRAME_EXTENTS,
                      0, 0, 0, 0, 0);

        // HACK: Use a timeout because earlier versions of some window managers
        //       (at least Unity, Fluxbox and Xfwm) failed to send the reply
        //       They have been fixed but broken versions are still in the wild
        //       If you are affected by this and your window manager is NOT
        //       listed above, PLEASE report it to their and our issue trackers
        while (!XCheckIfEvent(_glfw.x11.display,
                              &event,
                              isFrameExtentsEvent,
                              (XPointer) window))
        {
            if (!waitForX11Event(&timeout))
            {
                _glfwInputError(GLFW_PLATFORM_ERROR,
                                "X11: The window manager has a broken _NET_REQUEST_FRAME_EXTENTS implementation; please report this issue");
                return;
            }
        }
    }

    if (_glfwGetWindowPropertyX11(window->x11.handle,
                                  _glfw.x11.NET_FRAME_EXTENTS,
                                  XA_CARDINAL,
                                  (unsigned char**) &extents) == 4)
    {
        if (left)
            *left = extents[0];
        if (top)
            *top = extents[2];
        if (right)
            *right = extents[1];
        if (bottom)
            *bottom = extents[3];
    }

    if (extents)
        XFree(extents);
}

void _glfwPlatformGetWindowContentScale(_GLFWwindow* window,
                                        float* xscale, float* yscale)
{
    if (xscale)
        *xscale = _glfw.x11.contentScaleX;
    if (yscale)
        *yscale = _glfw.x11.contentScaleY;
}

void _glfwPlatformIconifyWindow(_GLFWwindow* window)
{
    if (window->x11.overrideRedirect)
    {
        // Override-redirect windows cannot be iconified or restored, as those
        // tasks are performed by the window manager
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Iconification of full screen windows requires a WM that supports EWMH full screen");
        return;
    }

    XIconifyWindow(_glfw.x11.display, window->x11.handle, _glfw.x11.screen);
    XFlush(_glfw.x11.display);
}

void _glfwPlatformRestoreWindow(_GLFWwindow* window)
{
    if (window->x11.overrideRedirect)
    {
        // Override-redirect windows cannot be iconified or restored, as those
        // tasks are performed by the window manager
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Iconification of full screen windows requires a WM that supports EWMH full screen");
        return;
    }

    if (_glfwPlatformWindowIconified(window))
    {
        XMapWindow(_glfw.x11.display, window->x11.handle);
        waitForVisibilityNotify(window);
    }
    else if (_glfwPlatformWindowVisible(window))
    {
        if (_glfw.x11.NET_WM_STATE &&
            _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT &&
            _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ)
        {
            sendEventToWM(window,
                          _glfw.x11.NET_WM_STATE,
                          _NET_WM_STATE_REMOVE,
                          _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT,
                          _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ,
                          1, 0);
        }
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformMaximizeWindow(_GLFWwindow* window)
{
    if (!_glfw.x11.NET_WM_STATE ||
        !_glfw.x11.NET_WM_STATE_MAXIMIZED_VERT ||
        !_glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ)
    {
        return;
    }

    if (_glfwPlatformWindowVisible(window))
    {
        sendEventToWM(window,
                    _glfw.x11.NET_WM_STATE,
                    _NET_WM_STATE_ADD,
                    _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT,
                    _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ,
                    1, 0);
    }
    else
    {
        Atom* states = NULL;
        unsigned long count =
            _glfwGetWindowPropertyX11(window->x11.handle,
                                      _glfw.x11.NET_WM_STATE,
                                      XA_ATOM,
                                      (unsigned char**) &states);

        // NOTE: We don't check for failure as this property may not exist yet
        //       and that's fine (and we'll create it implicitly with append)

        Atom missing[2] =
        {
            _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT,
            _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ
        };
        unsigned long missingCount = 2;

        for (unsigned long i = 0;  i < count;  i++)
        {
            for (unsigned long j = 0;  j < missingCount;  j++)
            {
                if (states[i] == missing[j])
                {
                    missing[j] = missing[missingCount - 1];
                    missingCount--;
                }
            }
        }

        if (states)
            XFree(states);

        if (!missingCount)
            return;

        XChangeProperty(_glfw.x11.display, window->x11.handle,
                        _glfw.x11.NET_WM_STATE, XA_ATOM, 32,
                        PropModeAppend,
                        (unsigned char*) missing,
                        missingCount);
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformShowWindow(_GLFWwindow* window)
{
    if (_glfwPlatformWindowVisible(window))
        return;

    XMapWindow(_glfw.x11.display, window->x11.handle);
    waitForVisibilityNotify(window);
}

void _glfwPlatformHideWindow(_GLFWwindow* window)
{
    XUnmapWindow(_glfw.x11.display, window->x11.handle);
    XFlush(_glfw.x11.display);
}

void _glfwPlatformRequestWindowAttention(_GLFWwindow* window)
{
    if (!_glfw.x11.NET_WM_STATE || !_glfw.x11.NET_WM_STATE_DEMANDS_ATTENTION)
        return;

    sendEventToWM(window,
                  _glfw.x11.NET_WM_STATE,
                  _NET_WM_STATE_ADD,
                  _glfw.x11.NET_WM_STATE_DEMANDS_ATTENTION,
                  0, 1, 0);
}

void _glfwPlatformFocusWindow(_GLFWwindow* window)
{
    if (_glfw.x11.NET_ACTIVE_WINDOW)
        sendEventToWM(window, _glfw.x11.NET_ACTIVE_WINDOW, 1, 0, 0, 0, 0);
    else if (_glfwPlatformWindowVisible(window))
    {
        XRaiseWindow(_glfw.x11.display, window->x11.handle);
        XSetInputFocus(_glfw.x11.display, window->x11.handle,
                       RevertToParent, CurrentTime);
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformSetWindowMonitor(_GLFWwindow* window,
                                   _GLFWmonitor* monitor,
                                   int xpos, int ypos,
                                   int width, int height,
                                   int refreshRate)
{
    if (window->monitor == monitor)
    {
        if (monitor)
        {
            if (monitor->window == window)
                acquireMonitor(window);
        }
        else
        {
            if (!window->resizable)
                updateNormalHints(window, width, height);

            XMoveResizeWindow(_glfw.x11.display, window->x11.handle,
                              xpos, ypos, width, height);
        }

        XFlush(_glfw.x11.display);
        return;
    }

    if (window->monitor)
    {
        _glfwPlatformSetWindowDecorated(window, window->decorated);
        _glfwPlatformSetWindowFloating(window, window->floating);
        releaseMonitor(window);
    }

    _glfwInputWindowMonitor(window, monitor);
    updateNormalHints(window, width, height);

    if (window->monitor)
    {
        if (!_glfwPlatformWindowVisible(window))
        {
            XMapRaised(_glfw.x11.display, window->x11.handle);
            waitForVisibilityNotify(window);
        }

        updateWindowMode(window);
        acquireMonitor(window);
    }
    else
    {
        updateWindowMode(window);
        XMoveResizeWindow(_glfw.x11.display, window->x11.handle,
                          xpos, ypos, width, height);
    }

    XFlush(_glfw.x11.display);
}

int _glfwPlatformWindowFocused(_GLFWwindow* window)
{
    Window focused;
    int state;

    XGetInputFocus(_glfw.x11.display, &focused, &state);
    return window->x11.handle == focused;
}

int _glfwPlatformWindowIconified(_GLFWwindow* window)
{
    return getWindowState(window) == IconicState;
}

int _glfwPlatformWindowVisible(_GLFWwindow* window)
{
    XWindowAttributes wa;
    XGetWindowAttributes(_glfw.x11.display, window->x11.handle, &wa);
    return wa.map_state == IsViewable;
}

int _glfwPlatformWindowMaximized(_GLFWwindow* window)
{
    Atom* states;
    unsigned long i;
    GLFWbool maximized = GLFW_FALSE;

    if (!_glfw.x11.NET_WM_STATE ||
        !_glfw.x11.NET_WM_STATE_MAXIMIZED_VERT ||
        !_glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ)
    {
        return maximized;
    }

    const unsigned long count =
        _glfwGetWindowPropertyX11(window->x11.handle,
                                  _glfw.x11.NET_WM_STATE,
                                  XA_ATOM,
                                  (unsigned char**) &states);

    for (i = 0;  i < count;  i++)
    {
        if (states[i] == _glfw.x11.NET_WM_STATE_MAXIMIZED_VERT ||
            states[i] == _glfw.x11.NET_WM_STATE_MAXIMIZED_HORZ)
        {
            maximized = GLFW_TRUE;
            break;
        }
    }

    if (states)
        XFree(states);

    return maximized;
}

int _glfwPlatformWindowHovered(_GLFWwindow* window)
{
    Window w = _glfw.x11.root;
    while (w)
    {
        Window root;
        int rootX, rootY, childX, childY;
        unsigned int mask;

        _glfwGrabErrorHandlerX11();

        const Bool result = XQueryPointer(_glfw.x11.display, w,
                                          &root, &w, &rootX, &rootY,
                                          &childX, &childY, &mask);

        _glfwReleaseErrorHandlerX11();

        if (_glfw.x11.errorCode == BadWindow)
            w = _glfw.x11.root;
        else if (!result)
            return GLFW_FALSE;
        else if (w == window->x11.handle)
            return GLFW_TRUE;
    }

    return GLFW_FALSE;
}

int _glfwPlatformFramebufferTransparent(_GLFWwindow* window)
{
    if (!window->x11.transparent)
        return GLFW_FALSE;

    return XGetSelectionOwner(_glfw.x11.display, _glfw.x11.NET_WM_CM_Sx) != None;
}

void _glfwPlatformSetWindowResizable(_GLFWwindow* window, GLFWbool enabled)
{
    int width, height;
    _glfwPlatformGetWindowSize(window, &width, &height);
    updateNormalHints(window, width, height);
}

void _glfwPlatformSetWindowDecorated(_GLFWwindow* window, GLFWbool enabled)
{
    struct
    {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long input_mode;
        unsigned long status;
    } hints = {0};

    hints.flags = MWM_HINTS_DECORATIONS;
    hints.decorations = enabled ? MWM_DECOR_ALL : 0;

    XChangeProperty(_glfw.x11.display, window->x11.handle,
                    _glfw.x11.MOTIF_WM_HINTS,
                    _glfw.x11.MOTIF_WM_HINTS, 32,
                    PropModeReplace,
                    (unsigned char*) &hints,
                    sizeof(hints) / sizeof(long));
}

void _glfwPlatformSetWindowFloating(_GLFWwindow* window, GLFWbool enabled)
{
    if (!_glfw.x11.NET_WM_STATE || !_glfw.x11.NET_WM_STATE_ABOVE)
        return;

    if (_glfwPlatformWindowVisible(window))
    {
        const long action = enabled ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
        sendEventToWM(window,
                      _glfw.x11.NET_WM_STATE,
                      action,
                      _glfw.x11.NET_WM_STATE_ABOVE,
                      0, 1, 0);
    }
    else
    {
        Atom* states = NULL;
        unsigned long i, count;

        count = _glfwGetWindowPropertyX11(window->x11.handle,
                                          _glfw.x11.NET_WM_STATE,
                                          XA_ATOM,
                                          (unsigned char**) &states);

        // NOTE: We don't check for failure as this property may not exist yet
        //       and that's fine (and we'll create it implicitly with append)

        if (enabled)
        {
            for (i = 0;  i < count;  i++)
            {
                if (states[i] == _glfw.x11.NET_WM_STATE_ABOVE)
                    break;
            }

            if (i == count)
            {
                XChangeProperty(_glfw.x11.display, window->x11.handle,
                                _glfw.x11.NET_WM_STATE, XA_ATOM, 32,
                                PropModeAppend,
                                (unsigned char*) &_glfw.x11.NET_WM_STATE_ABOVE,
                                1);
            }
        }
        else if (states)
        {
            for (i = 0;  i < count;  i++)
            {
                if (states[i] == _glfw.x11.NET_WM_STATE_ABOVE)
                    break;
            }

            if (i < count)
            {
                states[i] = states[count - 1];
                count--;

                XChangeProperty(_glfw.x11.display, window->x11.handle,
                                _glfw.x11.NET_WM_STATE, XA_ATOM, 32,
                                PropModeReplace, (unsigned char*) states, count);
            }
        }

        if (states)
            XFree(states);
    }

    XFlush(_glfw.x11.display);
}

float _glfwPlatformGetWindowOpacity(_GLFWwindow* window)
{
    float opacity = 1.f;

    if (XGetSelectionOwner(_glfw.x11.display, _glfw.x11.NET_WM_CM_Sx))
    {
        CARD32* value = NULL;

        if (_glfwGetWindowPropertyX11(window->x11.handle,
                                      _glfw.x11.NET_WM_WINDOW_OPACITY,
                                      XA_CARDINAL,
                                      (unsigned char**) &value))
        {
            opacity = (float) (*value / (double) 0xffffffffu);
        }

        if (value)
            XFree(value);
    }

    return opacity;
}

void _glfwPlatformSetWindowOpacity(_GLFWwindow* window, float opacity)
{
    const CARD32 value = (CARD32) (0xffffffffu * (double) opacity);
    XChangeProperty(_glfw.x11.display, window->x11.handle,
                    _glfw.x11.NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char*) &value, 1);
}

void _glfwPlatformSetRawMouseMotion(_GLFWwindow *window, GLFWbool enabled)
{
    if (!_glfw.x11.xi.available)
        return;

    if (_glfw.x11.disabledCursorWindow != window)
        return;

    if (enabled)
        enableRawMouseMotion(window);
    else
        disableRawMouseMotion(window);
}

GLFWbool _glfwPlatformRawMouseMotionSupported(void)
{
    return _glfw.x11.xi.available;
}

void _glfwPlatformPollEvents(void)
{
    drainEmptyEvents();

#if defined(__linux__)
    _glfwDetectJoystickConnectionLinux();
#endif
    XPending(_glfw.x11.display);

    while (XQLength(_glfw.x11.display))
    {
        XEvent event;
        XNextEvent(_glfw.x11.display, &event);
        processEvent(&event);
    }

    _GLFWwindow* window = _glfw.x11.disabledCursorWindow;
    if (window)
    {
        int width, height;
        _glfwPlatformGetWindowSize(window, &width, &height);

        // NOTE: Re-center the cursor only if it has moved since the last call,
        //       to avoid breaking glfwWaitEvents with MotionNotify
        if (window->x11.lastCursorPosX != width / 2 ||
            window->x11.lastCursorPosY != height / 2)
        {
            _glfwPlatformSetCursorPos(window, width / 2, height / 2);
        }
    }

    XFlush(_glfw.x11.display);
}

void _glfwPlatformWaitEvents(void)
{
    waitForAnyEvent(NULL);
    _glfwPlatformPollEvents();
}

void _glfwPlatformWaitEventsTimeout(double timeout)
{
    waitForAnyEvent(&timeout);
    _glfwPlatformPollEvents();
}

void _glfwPlatformPostEmptyEvent(void)
{
    writeEmptyEvent();
}

void _glfwPlatformGetCursorPos(_GLFWwindow* window, double* xpos, double* ypos)
{
    Window root, child;
    int rootX, rootY, childX, childY;
    unsigned int mask;

    XQueryPointer(_glfw.x11.display, window->x11.handle,
                  &root, &child,
                  &rootX, &rootY, &childX, &childY,
                  &mask);

    if (xpos)
        *xpos = childX;
    if (ypos)
        *ypos = childY;
}

void _glfwPlatformSetCursorPos(_GLFWwindow* window, double x, double y)
{
    // Store the new position so it can be recognized later
    window->x11.warpCursorPosX = (int) x;
    window->x11.warpCursorPosY = (int) y;

    XWarpPointer(_glfw.x11.display, None, window->x11.handle,
                 0,0,0,0, (int) x, (int) y);
    XFlush(_glfw.x11.display);
}

void _glfwPlatformSetCursorMode(_GLFWwindow* window, int mode)
{
    if (mode == GLFW_CURSOR_DISABLED)
    {
        if (_glfwPlatformWindowFocused(window))
            disableCursor(window);
    }
    else if (_glfw.x11.disabledCursorWindow == window)
        enableCursor(window);
    else
        updateCursorImage(window);

    XFlush(_glfw.x11.display);
}

const char* _glfwPlatformGetScancodeName(int scancode)
{
    if (!_glfw.x11.xkb.available)
        return NULL;

    if (scancode < 0 || scancode > 0xff ||
        _glfw.x11.keycodes[scancode] == GLFW_KEY_UNKNOWN)
    {
        _glfwInputError(GLFW_INVALID_VALUE, "Invalid scancode %i", scancode);
        return NULL;
    }

    const int key = _glfw.x11.keycodes[scancode];
    const KeySym keysym = XkbKeycodeToKeysym(_glfw.x11.display,
                                             scancode, _glfw.x11.xkb.group, 0);
    if (keysym == NoSymbol)
        return NULL;

    const uint32_t codepoint = _glfwKeySym2Unicode(keysym);
    if (codepoint == GLFW_INVALID_CODEPOINT)
        return NULL;

    const size_t count = _glfwEncodeUTF8(_glfw.x11.keynames[key], codepoint);
    if (count == 0)
        return NULL;

    _glfw.x11.keynames[key][count] = '\0';
    return _glfw.x11.keynames[key];
}

int _glfwPlatformGetKeyScancode(int key)
{
    return _glfw.x11.scancodes[key];
}

int _glfwPlatformCreateCursor(_GLFWcursor* cursor,
                              const GLFWimage* image,
                              int xhot, int yhot)
{
    cursor->x11.handle = _glfwCreateCursorX11(image, xhot, yhot);
    if (!cursor->x11.handle)
        return GLFW_FALSE;

    return GLFW_TRUE;
}

int _glfwPlatformCreateStandardCursor(_GLFWcursor* cursor, int shape)
{
    int native = 0;

    if (shape == GLFW_ARROW_CURSOR)
        native = XC_left_ptr;
    else if (shape == GLFW_IBEAM_CURSOR)
        native = XC_xterm;
    else if (shape == GLFW_CROSSHAIR_CURSOR)
        native = XC_crosshair;
    else if (shape == GLFW_HAND_CURSOR)
        native = XC_hand2;
    else if (shape == GLFW_HRESIZE_CURSOR)
        native = XC_sb_h_double_arrow;
    else if (shape == GLFW_VRESIZE_CURSOR)
        native = XC_sb_v_double_arrow;
    else
        return GLFW_FALSE;

    cursor->x11.handle = XCreateFontCursor(_glfw.x11.display, native);
    if (!cursor->x11.handle)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Failed to create standard cursor");
        return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

void _glfwPlatformDestroyCursor(_GLFWcursor* cursor)
{
    if (cursor->x11.handle)
        XFreeCursor(_glfw.x11.display, cursor->x11.handle);
}

void _glfwPlatformSetCursor(_GLFWwindow* window, _GLFWcursor* cursor)
{
    if (window->cursorMode == GLFW_CURSOR_NORMAL)
    {
        updateCursorImage(window);
        XFlush(_glfw.x11.display);
    }
}

void _glfwPlatformSetClipboardString(const char* string)
{
    char* copy = _glfw_strdup(string);
    free(_glfw.x11.clipboardString);
    _glfw.x11.clipboardString = copy;

    XSetSelectionOwner(_glfw.x11.display,
                       _glfw.x11.CLIPBOARD,
                       _glfw.x11.helperWindowHandle,
                       CurrentTime);

    if (XGetSelectionOwner(_glfw.x11.display, _glfw.x11.CLIPBOARD) !=
        _glfw.x11.helperWindowHandle)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Failed to become owner of clipboard selection");
    }
}

const char* _glfwPlatformGetClipboardString(void)
{
    return getSelectionString(_glfw.x11.CLIPBOARD);
}

void _glfwPlatformGetRequiredInstanceExtensions(char** extensions)
{
    if (!_glfw.vk.KHR_surface)
        return;

    if (!_glfw.vk.KHR_xcb_surface || !_glfw.x11.x11xcb.handle)
    {
        if (!_glfw.vk.KHR_xlib_surface)
            return;
    }

    extensions[0] = "VK_KHR_surface";

    // NOTE: VK_KHR_xcb_surface is preferred due to some early ICDs exposing but
    //       not correctly implementing VK_KHR_xlib_surface
    if (_glfw.vk.KHR_xcb_surface && _glfw.x11.x11xcb.handle)
        extensions[1] = "VK_KHR_xcb_surface";
    else
        extensions[1] = "VK_KHR_xlib_surface";
}

int _glfwPlatformGetPhysicalDevicePresentationSupport(VkInstance instance,
                                                      VkPhysicalDevice device,
                                                      uint32_t queuefamily)
{
    VisualID visualID = XVisualIDFromVisual(DefaultVisual(_glfw.x11.display,
                                                          _glfw.x11.screen));

    if (_glfw.vk.KHR_xcb_surface && _glfw.x11.x11xcb.handle)
    {
        PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR
            vkGetPhysicalDeviceXcbPresentationSupportKHR =
            (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)
            vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
        if (!vkGetPhysicalDeviceXcbPresentationSupportKHR)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "X11: Vulkan instance missing VK_KHR_xcb_surface extension");
            return GLFW_FALSE;
        }

        xcb_connection_t* connection = XGetXCBConnection(_glfw.x11.display);
        if (!connection)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Failed to retrieve XCB connection");
            return GLFW_FALSE;
        }

        return vkGetPhysicalDeviceXcbPresentationSupportKHR(device,
                                                            queuefamily,
                                                            connection,
                                                            visualID);
    }
    else
    {
        PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR
            vkGetPhysicalDeviceXlibPresentationSupportKHR =
            (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)
            vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
        if (!vkGetPhysicalDeviceXlibPresentationSupportKHR)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "X11: Vulkan instance missing VK_KHR_xlib_surface extension");
            return GLFW_FALSE;
        }

        return vkGetPhysicalDeviceXlibPresentationSupportKHR(device,
                                                             queuefamily,
                                                             _glfw.x11.display,
                                                             visualID);
    }
}

VkResult _glfwPlatformCreateWindowSurface(VkInstance instance,
                                          _GLFWwindow* window,
                                          const VkAllocationCallbacks* allocator,
                                          VkSurfaceKHR* surface)
{
    if (_glfw.vk.KHR_xcb_surface && _glfw.x11.x11xcb.handle)
    {
        VkResult err;
        VkXcbSurfaceCreateInfoKHR sci;
        PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;

        xcb_connection_t* connection = XGetXCBConnection(_glfw.x11.display);
        if (!connection)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Failed to retrieve XCB connection");
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        vkCreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)
            vkGetInstanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
        if (!vkCreateXcbSurfaceKHR)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "X11: Vulkan instance missing VK_KHR_xcb_surface extension");
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        memset(&sci, 0, sizeof(sci));
        sci.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        sci.connection = connection;
        sci.window = window->x11.handle;

        err = vkCreateXcbSurfaceKHR(instance, &sci, allocator, surface);
        if (err)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Failed to create Vulkan XCB surface: %s",
                            _glfwGetVulkanResultString(err));
        }

        return err;
    }
    else
    {
        VkResult err;
        VkXlibSurfaceCreateInfoKHR sci;
        PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;

        vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)
            vkGetInstanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
        if (!vkCreateXlibSurfaceKHR)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "X11: Vulkan instance missing VK_KHR_xlib_surface extension");
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }

        memset(&sci, 0, sizeof(sci));
        sci.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        sci.dpy = _glfw.x11.display;
        sci.window = window->x11.handle;

        err = vkCreateXlibSurfaceKHR(instance, &sci, allocator, surface);
        if (err)
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "X11: Failed to create Vulkan X11 surface: %s",
                            _glfwGetVulkanResultString(err));
        }

        return err;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI Display* glfwGetX11Display(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return _glfw.x11.display;
}

GLFWAPI Window glfwGetX11Window(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(None);
    return window->x11.handle;
}

GLFWAPI void glfwSetX11SelectionString(const char* string)
{
    _GLFW_REQUIRE_INIT();

    free(_glfw.x11.primarySelectionString);
    _glfw.x11.primarySelectionString = _glfw_strdup(string);

    XSetSelectionOwner(_glfw.x11.display,
                       _glfw.x11.PRIMARY,
                       _glfw.x11.helperWindowHandle,
                       CurrentTime);

    if (XGetSelectionOwner(_glfw.x11.display, _glfw.x11.PRIMARY) !=
        _glfw.x11.helperWindowHandle)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "X11: Failed to become owner of primary selection");
    }
}

GLFWAPI const char* glfwGetX11SelectionString(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);
    return getSelectionString(_glfw.x11.PRIMARY);
}

//========================================================================
// GLFW 3.3 X11 - www.glfw.org
// GLFW 3.3 - www.glfw.org - xkb_unicode.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================


/*
 * Marcus: This code was originally written by Markus G. Kuhn.
 * I have made some slight changes (trimmed it down a bit from >60 KB to
 * 20 KB), but the functionality is the same.
 */

/*
 * This module converts keysym values into the corresponding ISO 10646
 * (UCS, Unicode) values.
 *
 * The array keysymtab[] contains pairs of X11 keysym values for graphical
 * characters and the corresponding Unicode value. The function
 * _glfwKeySym2Unicode() maps a keysym onto a Unicode value using a binary
 * search, therefore keysymtab[] must remain SORTED by keysym value.
 *
 * We allow to represent any UCS character in the range U-00000000 to
 * U-00FFFFFF by a keysym value in the range 0x01000000 to 0x01ffffff.
 * This admittedly does not cover the entire 31-bit space of UCS, but
 * it does cover all of the characters up to U-10FFFF, which can be
 * represented by UTF-16, and more, and it is very unlikely that higher
 * UCS codes will ever be assigned by ISO. So to get Unicode character
 * U+ABCD you can directly use keysym 0x0100abcd.
 *
 * Original author: Markus G. Kuhn <mkuhn@acm.org>, University of
 *                  Cambridge, April 2001
 *
 * Special thanks to Richard Verhoeven <river@win.tue.nl> for preparing
 * an initial draft of the mapping table.
 *
 */


//************************************************************************
//****                KeySym to Unicode mapping table                 ****
//************************************************************************

static const struct codepair {
  unsigned short keysym;
  unsigned short ucs;
} keysymtab[] = {
  { 0x01a1, 0x0104 },
  { 0x01a2, 0x02d8 },
  { 0x01a3, 0x0141 },
  { 0x01a5, 0x013d },
  { 0x01a6, 0x015a },
  { 0x01a9, 0x0160 },
  { 0x01aa, 0x015e },
  { 0x01ab, 0x0164 },
  { 0x01ac, 0x0179 },
  { 0x01ae, 0x017d },
  { 0x01af, 0x017b },
  { 0x01b1, 0x0105 },
  { 0x01b2, 0x02db },
  { 0x01b3, 0x0142 },
  { 0x01b5, 0x013e },
  { 0x01b6, 0x015b },
  { 0x01b7, 0x02c7 },
  { 0x01b9, 0x0161 },
  { 0x01ba, 0x015f },
  { 0x01bb, 0x0165 },
  { 0x01bc, 0x017a },
  { 0x01bd, 0x02dd },
  { 0x01be, 0x017e },
  { 0x01bf, 0x017c },
  { 0x01c0, 0x0154 },
  { 0x01c3, 0x0102 },
  { 0x01c5, 0x0139 },
  { 0x01c6, 0x0106 },
  { 0x01c8, 0x010c },
  { 0x01ca, 0x0118 },
  { 0x01cc, 0x011a },
  { 0x01cf, 0x010e },
  { 0x01d0, 0x0110 },
  { 0x01d1, 0x0143 },
  { 0x01d2, 0x0147 },
  { 0x01d5, 0x0150 },
  { 0x01d8, 0x0158 },
  { 0x01d9, 0x016e },
  { 0x01db, 0x0170 },
  { 0x01de, 0x0162 },
  { 0x01e0, 0x0155 },
  { 0x01e3, 0x0103 },
  { 0x01e5, 0x013a },
  { 0x01e6, 0x0107 },
  { 0x01e8, 0x010d },
  { 0x01ea, 0x0119 },
  { 0x01ec, 0x011b },
  { 0x01ef, 0x010f },
  { 0x01f0, 0x0111 },
  { 0x01f1, 0x0144 },
  { 0x01f2, 0x0148 },
  { 0x01f5, 0x0151 },
  { 0x01f8, 0x0159 },
  { 0x01f9, 0x016f },
  { 0x01fb, 0x0171 },
  { 0x01fe, 0x0163 },
  { 0x01ff, 0x02d9 },
  { 0x02a1, 0x0126 },
  { 0x02a6, 0x0124 },
  { 0x02a9, 0x0130 },
  { 0x02ab, 0x011e },
  { 0x02ac, 0x0134 },
  { 0x02b1, 0x0127 },
  { 0x02b6, 0x0125 },
  { 0x02b9, 0x0131 },
  { 0x02bb, 0x011f },
  { 0x02bc, 0x0135 },
  { 0x02c5, 0x010a },
  { 0x02c6, 0x0108 },
  { 0x02d5, 0x0120 },
  { 0x02d8, 0x011c },
  { 0x02dd, 0x016c },
  { 0x02de, 0x015c },
  { 0x02e5, 0x010b },
  { 0x02e6, 0x0109 },
  { 0x02f5, 0x0121 },
  { 0x02f8, 0x011d },
  { 0x02fd, 0x016d },
  { 0x02fe, 0x015d },
  { 0x03a2, 0x0138 },
  { 0x03a3, 0x0156 },
  { 0x03a5, 0x0128 },
  { 0x03a6, 0x013b },
  { 0x03aa, 0x0112 },
  { 0x03ab, 0x0122 },
  { 0x03ac, 0x0166 },
  { 0x03b3, 0x0157 },
  { 0x03b5, 0x0129 },
  { 0x03b6, 0x013c },
  { 0x03ba, 0x0113 },
  { 0x03bb, 0x0123 },
  { 0x03bc, 0x0167 },
  { 0x03bd, 0x014a },
  { 0x03bf, 0x014b },
  { 0x03c0, 0x0100 },
  { 0x03c7, 0x012e },
  { 0x03cc, 0x0116 },
  { 0x03cf, 0x012a },
  { 0x03d1, 0x0145 },
  { 0x03d2, 0x014c },
  { 0x03d3, 0x0136 },
  { 0x03d9, 0x0172 },
  { 0x03dd, 0x0168 },
  { 0x03de, 0x016a },
  { 0x03e0, 0x0101 },
  { 0x03e7, 0x012f },
  { 0x03ec, 0x0117 },
  { 0x03ef, 0x012b },
  { 0x03f1, 0x0146 },
  { 0x03f2, 0x014d },
  { 0x03f3, 0x0137 },
  { 0x03f9, 0x0173 },
  { 0x03fd, 0x0169 },
  { 0x03fe, 0x016b },
  { 0x047e, 0x203e },
  { 0x04a1, 0x3002 },
  { 0x04a2, 0x300c },
  { 0x04a3, 0x300d },
  { 0x04a4, 0x3001 },
  { 0x04a5, 0x30fb },
  { 0x04a6, 0x30f2 },
  { 0x04a7, 0x30a1 },
  { 0x04a8, 0x30a3 },
  { 0x04a9, 0x30a5 },
  { 0x04aa, 0x30a7 },
  { 0x04ab, 0x30a9 },
  { 0x04ac, 0x30e3 },
  { 0x04ad, 0x30e5 },
  { 0x04ae, 0x30e7 },
  { 0x04af, 0x30c3 },
  { 0x04b0, 0x30fc },
  { 0x04b1, 0x30a2 },
  { 0x04b2, 0x30a4 },
  { 0x04b3, 0x30a6 },
  { 0x04b4, 0x30a8 },
  { 0x04b5, 0x30aa },
  { 0x04b6, 0x30ab },
  { 0x04b7, 0x30ad },
  { 0x04b8, 0x30af },
  { 0x04b9, 0x30b1 },
  { 0x04ba, 0x30b3 },
  { 0x04bb, 0x30b5 },
  { 0x04bc, 0x30b7 },
  { 0x04bd, 0x30b9 },
  { 0x04be, 0x30bb },
  { 0x04bf, 0x30bd },
  { 0x04c0, 0x30bf },
  { 0x04c1, 0x30c1 },
  { 0x04c2, 0x30c4 },
  { 0x04c3, 0x30c6 },
  { 0x04c4, 0x30c8 },
  { 0x04c5, 0x30ca },
  { 0x04c6, 0x30cb },
  { 0x04c7, 0x30cc },
  { 0x04c8, 0x30cd },
  { 0x04c9, 0x30ce },
  { 0x04ca, 0x30cf },
  { 0x04cb, 0x30d2 },
  { 0x04cc, 0x30d5 },
  { 0x04cd, 0x30d8 },
  { 0x04ce, 0x30db },
  { 0x04cf, 0x30de },
  { 0x04d0, 0x30df },
  { 0x04d1, 0x30e0 },
  { 0x04d2, 0x30e1 },
  { 0x04d3, 0x30e2 },
  { 0x04d4, 0x30e4 },
  { 0x04d5, 0x30e6 },
  { 0x04d6, 0x30e8 },
  { 0x04d7, 0x30e9 },
  { 0x04d8, 0x30ea },
  { 0x04d9, 0x30eb },
  { 0x04da, 0x30ec },
  { 0x04db, 0x30ed },
  { 0x04dc, 0x30ef },
  { 0x04dd, 0x30f3 },
  { 0x04de, 0x309b },
  { 0x04df, 0x309c },
  { 0x05ac, 0x060c },
  { 0x05bb, 0x061b },
  { 0x05bf, 0x061f },
  { 0x05c1, 0x0621 },
  { 0x05c2, 0x0622 },
  { 0x05c3, 0x0623 },
  { 0x05c4, 0x0624 },
  { 0x05c5, 0x0625 },
  { 0x05c6, 0x0626 },
  { 0x05c7, 0x0627 },
  { 0x05c8, 0x0628 },
  { 0x05c9, 0x0629 },
  { 0x05ca, 0x062a },
  { 0x05cb, 0x062b },
  { 0x05cc, 0x062c },
  { 0x05cd, 0x062d },
  { 0x05ce, 0x062e },
  { 0x05cf, 0x062f },
  { 0x05d0, 0x0630 },
  { 0x05d1, 0x0631 },
  { 0x05d2, 0x0632 },
  { 0x05d3, 0x0633 },
  { 0x05d4, 0x0634 },
  { 0x05d5, 0x0635 },
  { 0x05d6, 0x0636 },
  { 0x05d7, 0x0637 },
  { 0x05d8, 0x0638 },
  { 0x05d9, 0x0639 },
  { 0x05da, 0x063a },
  { 0x05e0, 0x0640 },
  { 0x05e1, 0x0641 },
  { 0x05e2, 0x0642 },
  { 0x05e3, 0x0643 },
  { 0x05e4, 0x0644 },
  { 0x05e5, 0x0645 },
  { 0x05e6, 0x0646 },
  { 0x05e7, 0x0647 },
  { 0x05e8, 0x0648 },
  { 0x05e9, 0x0649 },
  { 0x05ea, 0x064a },
  { 0x05eb, 0x064b },
  { 0x05ec, 0x064c },
  { 0x05ed, 0x064d },
  { 0x05ee, 0x064e },
  { 0x05ef, 0x064f },
  { 0x05f0, 0x0650 },
  { 0x05f1, 0x0651 },
  { 0x05f2, 0x0652 },
  { 0x06a1, 0x0452 },
  { 0x06a2, 0x0453 },
  { 0x06a3, 0x0451 },
  { 0x06a4, 0x0454 },
  { 0x06a5, 0x0455 },
  { 0x06a6, 0x0456 },
  { 0x06a7, 0x0457 },
  { 0x06a8, 0x0458 },
  { 0x06a9, 0x0459 },
  { 0x06aa, 0x045a },
  { 0x06ab, 0x045b },
  { 0x06ac, 0x045c },
  { 0x06ae, 0x045e },
  { 0x06af, 0x045f },
  { 0x06b0, 0x2116 },
  { 0x06b1, 0x0402 },
  { 0x06b2, 0x0403 },
  { 0x06b3, 0x0401 },
  { 0x06b4, 0x0404 },
  { 0x06b5, 0x0405 },
  { 0x06b6, 0x0406 },
  { 0x06b7, 0x0407 },
  { 0x06b8, 0x0408 },
  { 0x06b9, 0x0409 },
  { 0x06ba, 0x040a },
  { 0x06bb, 0x040b },
  { 0x06bc, 0x040c },
  { 0x06be, 0x040e },
  { 0x06bf, 0x040f },
  { 0x06c0, 0x044e },
  { 0x06c1, 0x0430 },
  { 0x06c2, 0x0431 },
  { 0x06c3, 0x0446 },
  { 0x06c4, 0x0434 },
  { 0x06c5, 0x0435 },
  { 0x06c6, 0x0444 },
  { 0x06c7, 0x0433 },
  { 0x06c8, 0x0445 },
  { 0x06c9, 0x0438 },
  { 0x06ca, 0x0439 },
  { 0x06cb, 0x043a },
  { 0x06cc, 0x043b },
  { 0x06cd, 0x043c },
  { 0x06ce, 0x043d },
  { 0x06cf, 0x043e },
  { 0x06d0, 0x043f },
  { 0x06d1, 0x044f },
  { 0x06d2, 0x0440 },
  { 0x06d3, 0x0441 },
  { 0x06d4, 0x0442 },
  { 0x06d5, 0x0443 },
  { 0x06d6, 0x0436 },
  { 0x06d7, 0x0432 },
  { 0x06d8, 0x044c },
  { 0x06d9, 0x044b },
  { 0x06da, 0x0437 },
  { 0x06db, 0x0448 },
  { 0x06dc, 0x044d },
  { 0x06dd, 0x0449 },
  { 0x06de, 0x0447 },
  { 0x06df, 0x044a },
  { 0x06e0, 0x042e },
  { 0x06e1, 0x0410 },
  { 0x06e2, 0x0411 },
  { 0x06e3, 0x0426 },
  { 0x06e4, 0x0414 },
  { 0x06e5, 0x0415 },
  { 0x06e6, 0x0424 },
  { 0x06e7, 0x0413 },
  { 0x06e8, 0x0425 },
  { 0x06e9, 0x0418 },
  { 0x06ea, 0x0419 },
  { 0x06eb, 0x041a },
  { 0x06ec, 0x041b },
  { 0x06ed, 0x041c },
  { 0x06ee, 0x041d },
  { 0x06ef, 0x041e },
  { 0x06f0, 0x041f },
  { 0x06f1, 0x042f },
  { 0x06f2, 0x0420 },
  { 0x06f3, 0x0421 },
  { 0x06f4, 0x0422 },
  { 0x06f5, 0x0423 },
  { 0x06f6, 0x0416 },
  { 0x06f7, 0x0412 },
  { 0x06f8, 0x042c },
  { 0x06f9, 0x042b },
  { 0x06fa, 0x0417 },
  { 0x06fb, 0x0428 },
  { 0x06fc, 0x042d },
  { 0x06fd, 0x0429 },
  { 0x06fe, 0x0427 },
  { 0x06ff, 0x042a },
  { 0x07a1, 0x0386 },
  { 0x07a2, 0x0388 },
  { 0x07a3, 0x0389 },
  { 0x07a4, 0x038a },
  { 0x07a5, 0x03aa },
  { 0x07a7, 0x038c },
  { 0x07a8, 0x038e },
  { 0x07a9, 0x03ab },
  { 0x07ab, 0x038f },
  { 0x07ae, 0x0385 },
  { 0x07af, 0x2015 },
  { 0x07b1, 0x03ac },
  { 0x07b2, 0x03ad },
  { 0x07b3, 0x03ae },
  { 0x07b4, 0x03af },
  { 0x07b5, 0x03ca },
  { 0x07b6, 0x0390 },
  { 0x07b7, 0x03cc },
  { 0x07b8, 0x03cd },
  { 0x07b9, 0x03cb },
  { 0x07ba, 0x03b0 },
  { 0x07bb, 0x03ce },
  { 0x07c1, 0x0391 },
  { 0x07c2, 0x0392 },
  { 0x07c3, 0x0393 },
  { 0x07c4, 0x0394 },
  { 0x07c5, 0x0395 },
  { 0x07c6, 0x0396 },
  { 0x07c7, 0x0397 },
  { 0x07c8, 0x0398 },
  { 0x07c9, 0x0399 },
  { 0x07ca, 0x039a },
  { 0x07cb, 0x039b },
  { 0x07cc, 0x039c },
  { 0x07cd, 0x039d },
  { 0x07ce, 0x039e },
  { 0x07cf, 0x039f },
  { 0x07d0, 0x03a0 },
  { 0x07d1, 0x03a1 },
  { 0x07d2, 0x03a3 },
  { 0x07d4, 0x03a4 },
  { 0x07d5, 0x03a5 },
  { 0x07d6, 0x03a6 },
  { 0x07d7, 0x03a7 },
  { 0x07d8, 0x03a8 },
  { 0x07d9, 0x03a9 },
  { 0x07e1, 0x03b1 },
  { 0x07e2, 0x03b2 },
  { 0x07e3, 0x03b3 },
  { 0x07e4, 0x03b4 },
  { 0x07e5, 0x03b5 },
  { 0x07e6, 0x03b6 },
  { 0x07e7, 0x03b7 },
  { 0x07e8, 0x03b8 },
  { 0x07e9, 0x03b9 },
  { 0x07ea, 0x03ba },
  { 0x07eb, 0x03bb },
  { 0x07ec, 0x03bc },
  { 0x07ed, 0x03bd },
  { 0x07ee, 0x03be },
  { 0x07ef, 0x03bf },
  { 0x07f0, 0x03c0 },
  { 0x07f1, 0x03c1 },
  { 0x07f2, 0x03c3 },
  { 0x07f3, 0x03c2 },
  { 0x07f4, 0x03c4 },
  { 0x07f5, 0x03c5 },
  { 0x07f6, 0x03c6 },
  { 0x07f7, 0x03c7 },
  { 0x07f8, 0x03c8 },
  { 0x07f9, 0x03c9 },
  { 0x08a1, 0x23b7 },
  { 0x08a2, 0x250c },
  { 0x08a3, 0x2500 },
  { 0x08a4, 0x2320 },
  { 0x08a5, 0x2321 },
  { 0x08a6, 0x2502 },
  { 0x08a7, 0x23a1 },
  { 0x08a8, 0x23a3 },
  { 0x08a9, 0x23a4 },
  { 0x08aa, 0x23a6 },
  { 0x08ab, 0x239b },
  { 0x08ac, 0x239d },
  { 0x08ad, 0x239e },
  { 0x08ae, 0x23a0 },
  { 0x08af, 0x23a8 },
  { 0x08b0, 0x23ac },
  { 0x08bc, 0x2264 },
  { 0x08bd, 0x2260 },
  { 0x08be, 0x2265 },
  { 0x08bf, 0x222b },
  { 0x08c0, 0x2234 },
  { 0x08c1, 0x221d },
  { 0x08c2, 0x221e },
  { 0x08c5, 0x2207 },
  { 0x08c8, 0x223c },
  { 0x08c9, 0x2243 },
  { 0x08cd, 0x21d4 },
  { 0x08ce, 0x21d2 },
  { 0x08cf, 0x2261 },
  { 0x08d6, 0x221a },
  { 0x08da, 0x2282 },
  { 0x08db, 0x2283 },
  { 0x08dc, 0x2229 },
  { 0x08dd, 0x222a },
  { 0x08de, 0x2227 },
  { 0x08df, 0x2228 },
  { 0x08ef, 0x2202 },
  { 0x08f6, 0x0192 },
  { 0x08fb, 0x2190 },
  { 0x08fc, 0x2191 },
  { 0x08fd, 0x2192 },
  { 0x08fe, 0x2193 },
  { 0x09e0, 0x25c6 },
  { 0x09e1, 0x2592 },
  { 0x09e2, 0x2409 },
  { 0x09e3, 0x240c },
  { 0x09e4, 0x240d },
  { 0x09e5, 0x240a },
  { 0x09e8, 0x2424 },
  { 0x09e9, 0x240b },
  { 0x09ea, 0x2518 },
  { 0x09eb, 0x2510 },
  { 0x09ec, 0x250c },
  { 0x09ed, 0x2514 },
  { 0x09ee, 0x253c },
  { 0x09ef, 0x23ba },
  { 0x09f0, 0x23bb },
  { 0x09f1, 0x2500 },
  { 0x09f2, 0x23bc },
  { 0x09f3, 0x23bd },
  { 0x09f4, 0x251c },
  { 0x09f5, 0x2524 },
  { 0x09f6, 0x2534 },
  { 0x09f7, 0x252c },
  { 0x09f8, 0x2502 },
  { 0x0aa1, 0x2003 },
  { 0x0aa2, 0x2002 },
  { 0x0aa3, 0x2004 },
  { 0x0aa4, 0x2005 },
  { 0x0aa5, 0x2007 },
  { 0x0aa6, 0x2008 },
  { 0x0aa7, 0x2009 },
  { 0x0aa8, 0x200a },
  { 0x0aa9, 0x2014 },
  { 0x0aaa, 0x2013 },
  { 0x0aae, 0x2026 },
  { 0x0aaf, 0x2025 },
  { 0x0ab0, 0x2153 },
  { 0x0ab1, 0x2154 },
  { 0x0ab2, 0x2155 },
  { 0x0ab3, 0x2156 },
  { 0x0ab4, 0x2157 },
  { 0x0ab5, 0x2158 },
  { 0x0ab6, 0x2159 },
  { 0x0ab7, 0x215a },
  { 0x0ab8, 0x2105 },
  { 0x0abb, 0x2012 },
  { 0x0abc, 0x2329 },
  { 0x0abe, 0x232a },
  { 0x0ac3, 0x215b },
  { 0x0ac4, 0x215c },
  { 0x0ac5, 0x215d },
  { 0x0ac6, 0x215e },
  { 0x0ac9, 0x2122 },
  { 0x0aca, 0x2613 },
  { 0x0acc, 0x25c1 },
  { 0x0acd, 0x25b7 },
  { 0x0ace, 0x25cb },
  { 0x0acf, 0x25af },
  { 0x0ad0, 0x2018 },
  { 0x0ad1, 0x2019 },
  { 0x0ad2, 0x201c },
  { 0x0ad3, 0x201d },
  { 0x0ad4, 0x211e },
  { 0x0ad6, 0x2032 },
  { 0x0ad7, 0x2033 },
  { 0x0ad9, 0x271d },
  { 0x0adb, 0x25ac },
  { 0x0adc, 0x25c0 },
  { 0x0add, 0x25b6 },
  { 0x0ade, 0x25cf },
  { 0x0adf, 0x25ae },
  { 0x0ae0, 0x25e6 },
  { 0x0ae1, 0x25ab },
  { 0x0ae2, 0x25ad },
  { 0x0ae3, 0x25b3 },
  { 0x0ae4, 0x25bd },
  { 0x0ae5, 0x2606 },
  { 0x0ae6, 0x2022 },
  { 0x0ae7, 0x25aa },
  { 0x0ae8, 0x25b2 },
  { 0x0ae9, 0x25bc },
  { 0x0aea, 0x261c },
  { 0x0aeb, 0x261e },
  { 0x0aec, 0x2663 },
  { 0x0aed, 0x2666 },
  { 0x0aee, 0x2665 },
  { 0x0af0, 0x2720 },
  { 0x0af1, 0x2020 },
  { 0x0af2, 0x2021 },
  { 0x0af3, 0x2713 },
  { 0x0af4, 0x2717 },
  { 0x0af5, 0x266f },
  { 0x0af6, 0x266d },
  { 0x0af7, 0x2642 },
  { 0x0af8, 0x2640 },
  { 0x0af9, 0x260e },
  { 0x0afa, 0x2315 },
  { 0x0afb, 0x2117 },
  { 0x0afc, 0x2038 },
  { 0x0afd, 0x201a },
  { 0x0afe, 0x201e },
  { 0x0ba3, 0x003c },
  { 0x0ba6, 0x003e },
  { 0x0ba8, 0x2228 },
  { 0x0ba9, 0x2227 },
  { 0x0bc0, 0x00af },
  { 0x0bc2, 0x22a5 },
  { 0x0bc3, 0x2229 },
  { 0x0bc4, 0x230a },
  { 0x0bc6, 0x005f },
  { 0x0bca, 0x2218 },
  { 0x0bcc, 0x2395 },
  { 0x0bce, 0x22a4 },
  { 0x0bcf, 0x25cb },
  { 0x0bd3, 0x2308 },
  { 0x0bd6, 0x222a },
  { 0x0bd8, 0x2283 },
  { 0x0bda, 0x2282 },
  { 0x0bdc, 0x22a2 },
  { 0x0bfc, 0x22a3 },
  { 0x0cdf, 0x2017 },
  { 0x0ce0, 0x05d0 },
  { 0x0ce1, 0x05d1 },
  { 0x0ce2, 0x05d2 },
  { 0x0ce3, 0x05d3 },
  { 0x0ce4, 0x05d4 },
  { 0x0ce5, 0x05d5 },
  { 0x0ce6, 0x05d6 },
  { 0x0ce7, 0x05d7 },
  { 0x0ce8, 0x05d8 },
  { 0x0ce9, 0x05d9 },
  { 0x0cea, 0x05da },
  { 0x0ceb, 0x05db },
  { 0x0cec, 0x05dc },
  { 0x0ced, 0x05dd },
  { 0x0cee, 0x05de },
  { 0x0cef, 0x05df },
  { 0x0cf0, 0x05e0 },
  { 0x0cf1, 0x05e1 },
  { 0x0cf2, 0x05e2 },
  { 0x0cf3, 0x05e3 },
  { 0x0cf4, 0x05e4 },
  { 0x0cf5, 0x05e5 },
  { 0x0cf6, 0x05e6 },
  { 0x0cf7, 0x05e7 },
  { 0x0cf8, 0x05e8 },
  { 0x0cf9, 0x05e9 },
  { 0x0cfa, 0x05ea },
  { 0x0da1, 0x0e01 },
  { 0x0da2, 0x0e02 },
  { 0x0da3, 0x0e03 },
  { 0x0da4, 0x0e04 },
  { 0x0da5, 0x0e05 },
  { 0x0da6, 0x0e06 },
  { 0x0da7, 0x0e07 },
  { 0x0da8, 0x0e08 },
  { 0x0da9, 0x0e09 },
  { 0x0daa, 0x0e0a },
  { 0x0dab, 0x0e0b },
  { 0x0dac, 0x0e0c },
  { 0x0dad, 0x0e0d },
  { 0x0dae, 0x0e0e },
  { 0x0daf, 0x0e0f },
  { 0x0db0, 0x0e10 },
  { 0x0db1, 0x0e11 },
  { 0x0db2, 0x0e12 },
  { 0x0db3, 0x0e13 },
  { 0x0db4, 0x0e14 },
  { 0x0db5, 0x0e15 },
  { 0x0db6, 0x0e16 },
  { 0x0db7, 0x0e17 },
  { 0x0db8, 0x0e18 },
  { 0x0db9, 0x0e19 },
  { 0x0dba, 0x0e1a },
  { 0x0dbb, 0x0e1b },
  { 0x0dbc, 0x0e1c },
  { 0x0dbd, 0x0e1d },
  { 0x0dbe, 0x0e1e },
  { 0x0dbf, 0x0e1f },
  { 0x0dc0, 0x0e20 },
  { 0x0dc1, 0x0e21 },
  { 0x0dc2, 0x0e22 },
  { 0x0dc3, 0x0e23 },
  { 0x0dc4, 0x0e24 },
  { 0x0dc5, 0x0e25 },
  { 0x0dc6, 0x0e26 },
  { 0x0dc7, 0x0e27 },
  { 0x0dc8, 0x0e28 },
  { 0x0dc9, 0x0e29 },
  { 0x0dca, 0x0e2a },
  { 0x0dcb, 0x0e2b },
  { 0x0dcc, 0x0e2c },
  { 0x0dcd, 0x0e2d },
  { 0x0dce, 0x0e2e },
  { 0x0dcf, 0x0e2f },
  { 0x0dd0, 0x0e30 },
  { 0x0dd1, 0x0e31 },
  { 0x0dd2, 0x0e32 },
  { 0x0dd3, 0x0e33 },
  { 0x0dd4, 0x0e34 },
  { 0x0dd5, 0x0e35 },
  { 0x0dd6, 0x0e36 },
  { 0x0dd7, 0x0e37 },
  { 0x0dd8, 0x0e38 },
  { 0x0dd9, 0x0e39 },
  { 0x0dda, 0x0e3a },
  { 0x0ddf, 0x0e3f },
  { 0x0de0, 0x0e40 },
  { 0x0de1, 0x0e41 },
  { 0x0de2, 0x0e42 },
  { 0x0de3, 0x0e43 },
  { 0x0de4, 0x0e44 },
  { 0x0de5, 0x0e45 },
  { 0x0de6, 0x0e46 },
  { 0x0de7, 0x0e47 },
  { 0x0de8, 0x0e48 },
  { 0x0de9, 0x0e49 },
  { 0x0dea, 0x0e4a },
  { 0x0deb, 0x0e4b },
  { 0x0dec, 0x0e4c },
  { 0x0ded, 0x0e4d },
  { 0x0df0, 0x0e50 },
  { 0x0df1, 0x0e51 },
  { 0x0df2, 0x0e52 },
  { 0x0df3, 0x0e53 },
  { 0x0df4, 0x0e54 },
  { 0x0df5, 0x0e55 },
  { 0x0df6, 0x0e56 },
  { 0x0df7, 0x0e57 },
  { 0x0df8, 0x0e58 },
  { 0x0df9, 0x0e59 },
  { 0x0ea1, 0x3131 },
  { 0x0ea2, 0x3132 },
  { 0x0ea3, 0x3133 },
  { 0x0ea4, 0x3134 },
  { 0x0ea5, 0x3135 },
  { 0x0ea6, 0x3136 },
  { 0x0ea7, 0x3137 },
  { 0x0ea8, 0x3138 },
  { 0x0ea9, 0x3139 },
  { 0x0eaa, 0x313a },
  { 0x0eab, 0x313b },
  { 0x0eac, 0x313c },
  { 0x0ead, 0x313d },
  { 0x0eae, 0x313e },
  { 0x0eaf, 0x313f },
  { 0x0eb0, 0x3140 },
  { 0x0eb1, 0x3141 },
  { 0x0eb2, 0x3142 },
  { 0x0eb3, 0x3143 },
  { 0x0eb4, 0x3144 },
  { 0x0eb5, 0x3145 },
  { 0x0eb6, 0x3146 },
  { 0x0eb7, 0x3147 },
  { 0x0eb8, 0x3148 },
  { 0x0eb9, 0x3149 },
  { 0x0eba, 0x314a },
  { 0x0ebb, 0x314b },
  { 0x0ebc, 0x314c },
  { 0x0ebd, 0x314d },
  { 0x0ebe, 0x314e },
  { 0x0ebf, 0x314f },
  { 0x0ec0, 0x3150 },
  { 0x0ec1, 0x3151 },
  { 0x0ec2, 0x3152 },
  { 0x0ec3, 0x3153 },
  { 0x0ec4, 0x3154 },
  { 0x0ec5, 0x3155 },
  { 0x0ec6, 0x3156 },
  { 0x0ec7, 0x3157 },
  { 0x0ec8, 0x3158 },
  { 0x0ec9, 0x3159 },
  { 0x0eca, 0x315a },
  { 0x0ecb, 0x315b },
  { 0x0ecc, 0x315c },
  { 0x0ecd, 0x315d },
  { 0x0ece, 0x315e },
  { 0x0ecf, 0x315f },
  { 0x0ed0, 0x3160 },
  { 0x0ed1, 0x3161 },
  { 0x0ed2, 0x3162 },
  { 0x0ed3, 0x3163 },
  { 0x0ed4, 0x11a8 },
  { 0x0ed5, 0x11a9 },
  { 0x0ed6, 0x11aa },
  { 0x0ed7, 0x11ab },
  { 0x0ed8, 0x11ac },
  { 0x0ed9, 0x11ad },
  { 0x0eda, 0x11ae },
  { 0x0edb, 0x11af },
  { 0x0edc, 0x11b0 },
  { 0x0edd, 0x11b1 },
  { 0x0ede, 0x11b2 },
  { 0x0edf, 0x11b3 },
  { 0x0ee0, 0x11b4 },
  { 0x0ee1, 0x11b5 },
  { 0x0ee2, 0x11b6 },
  { 0x0ee3, 0x11b7 },
  { 0x0ee4, 0x11b8 },
  { 0x0ee5, 0x11b9 },
  { 0x0ee6, 0x11ba },
  { 0x0ee7, 0x11bb },
  { 0x0ee8, 0x11bc },
  { 0x0ee9, 0x11bd },
  { 0x0eea, 0x11be },
  { 0x0eeb, 0x11bf },
  { 0x0eec, 0x11c0 },
  { 0x0eed, 0x11c1 },
  { 0x0eee, 0x11c2 },
  { 0x0eef, 0x316d },
  { 0x0ef0, 0x3171 },
  { 0x0ef1, 0x3178 },
  { 0x0ef2, 0x317f },
  { 0x0ef3, 0x3181 },
  { 0x0ef4, 0x3184 },
  { 0x0ef5, 0x3186 },
  { 0x0ef6, 0x318d },
  { 0x0ef7, 0x318e },
  { 0x0ef8, 0x11eb },
  { 0x0ef9, 0x11f0 },
  { 0x0efa, 0x11f9 },
  { 0x0eff, 0x20a9 },
  { 0x13a4, 0x20ac },
  { 0x13bc, 0x0152 },
  { 0x13bd, 0x0153 },
  { 0x13be, 0x0178 },
  { 0x20ac, 0x20ac },
  { 0xfe50,    '`' },
  { 0xfe51, 0x00b4 },
  { 0xfe52,    '^' },
  { 0xfe53,    '~' },
  { 0xfe54, 0x00af },
  { 0xfe55, 0x02d8 },
  { 0xfe56, 0x02d9 },
  { 0xfe57, 0x00a8 },
  { 0xfe58, 0x02da },
  { 0xfe59, 0x02dd },
  { 0xfe5a, 0x02c7 },
  { 0xfe5b, 0x00b8 },
  { 0xfe5c, 0x02db },
  { 0xfe5d, 0x037a },
  { 0xfe5e, 0x309b },
  { 0xfe5f, 0x309c },
  { 0xfe63,    '/' },
  { 0xfe64, 0x02bc },
  { 0xfe65, 0x02bd },
  { 0xfe66, 0x02f5 },
  { 0xfe67, 0x02f3 },
  { 0xfe68, 0x02cd },
  { 0xfe69, 0xa788 },
  { 0xfe6a, 0x02f7 },
  { 0xfe6e,    ',' },
  { 0xfe6f, 0x00a4 },
  { 0xfe80,    'a' }, // XK_dead_a
  { 0xfe81,    'A' }, // XK_dead_A
  { 0xfe82,    'e' }, // XK_dead_e
  { 0xfe83,    'E' }, // XK_dead_E
  { 0xfe84,    'i' }, // XK_dead_i
  { 0xfe85,    'I' }, // XK_dead_I
  { 0xfe86,    'o' }, // XK_dead_o
  { 0xfe87,    'O' }, // XK_dead_O
  { 0xfe88,    'u' }, // XK_dead_u
  { 0xfe89,    'U' }, // XK_dead_U
  { 0xfe8a, 0x0259 },
  { 0xfe8b, 0x018f },
  { 0xfe8c, 0x00b5 },
  { 0xfe90,    '_' },
  { 0xfe91, 0x02c8 },
  { 0xfe92, 0x02cc },
  { 0xff80 /*XKB_KEY_KP_Space*/,     ' ' },
  { 0xff95 /*XKB_KEY_KP_7*/, 0x0037 },
  { 0xff96 /*XKB_KEY_KP_4*/, 0x0034 },
  { 0xff97 /*XKB_KEY_KP_8*/, 0x0038 },
  { 0xff98 /*XKB_KEY_KP_6*/, 0x0036 },
  { 0xff99 /*XKB_KEY_KP_2*/, 0x0032 },
  { 0xff9a /*XKB_KEY_KP_9*/, 0x0039 },
  { 0xff9b /*XKB_KEY_KP_3*/, 0x0033 },
  { 0xff9c /*XKB_KEY_KP_1*/, 0x0031 },
  { 0xff9d /*XKB_KEY_KP_5*/, 0x0035 },
  { 0xff9e /*XKB_KEY_KP_0*/, 0x0030 },
  { 0xffaa /*XKB_KEY_KP_Multiply*/,  '*' },
  { 0xffab /*XKB_KEY_KP_Add*/,       '+' },
  { 0xffac /*XKB_KEY_KP_Separator*/, ',' },
  { 0xffad /*XKB_KEY_KP_Subtract*/,  '-' },
  { 0xffae /*XKB_KEY_KP_Decimal*/,   '.' },
  { 0xffaf /*XKB_KEY_KP_Divide*/,    '/' },
  { 0xffb0 /*XKB_KEY_KP_0*/, 0x0030 },
  { 0xffb1 /*XKB_KEY_KP_1*/, 0x0031 },
  { 0xffb2 /*XKB_KEY_KP_2*/, 0x0032 },
  { 0xffb3 /*XKB_KEY_KP_3*/, 0x0033 },
  { 0xffb4 /*XKB_KEY_KP_4*/, 0x0034 },
  { 0xffb5 /*XKB_KEY_KP_5*/, 0x0035 },
  { 0xffb6 /*XKB_KEY_KP_6*/, 0x0036 },
  { 0xffb7 /*XKB_KEY_KP_7*/, 0x0037 },
  { 0xffb8 /*XKB_KEY_KP_8*/, 0x0038 },
  { 0xffb9 /*XKB_KEY_KP_9*/, 0x0039 },
  { 0xffbd /*XKB_KEY_KP_Equal*/,     '=' }
};


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Convert XKB KeySym to Unicode
//
uint32_t _glfwKeySym2Unicode(unsigned int keysym)
{
    int min = 0;
    int max = sizeof(keysymtab) / sizeof(struct codepair) - 1;
    int mid;

    // First check for Latin-1 characters (1:1 mapping)
    if ((keysym >= 0x0020 && keysym <= 0x007e) ||
        (keysym >= 0x00a0 && keysym <= 0x00ff))
    {
        return keysym;
    }

    // Also check for directly encoded 24-bit UCS characters
    if ((keysym & 0xff000000) == 0x01000000)
        return keysym & 0x00ffffff;

    // Binary search in table
    while (max >= min)
    {
        mid = (min + max) / 2;
        if (keysymtab[mid].keysym < keysym)
            min = mid + 1;
        else if (keysymtab[mid].keysym > keysym)
            max = mid - 1;
        else
            return keysymtab[mid].ucs;
    }

    // No matching Unicode value found
    return GLFW_INVALID_CODEPOINT;
}

//========================================================================
// GLFW 3.3 POSIX - www.glfw.org
// GLFW 3.3 - www.glfw.org - posix_time.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Initialise timer
//
void _glfwInitTimerPOSIX(void)
{
#if defined(CLOCK_MONOTONIC)
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
    {
        _glfw.timer.posix.monotonic = GLFW_TRUE;
        _glfw.timer.posix.frequency = 1000000000;
    }
    else
#endif
    {
        _glfw.timer.posix.monotonic = GLFW_FALSE;
        _glfw.timer.posix.frequency = 1000000;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

uint64_t _glfwPlatformGetTimerValue(void)
{
#if defined(CLOCK_MONOTONIC)
    if (_glfw.timer.posix.monotonic)
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t) ts.tv_sec * (uint64_t) 1000000000 + (uint64_t) ts.tv_nsec;
    }
    else
#endif
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (uint64_t) tv.tv_sec * (uint64_t) 1000000 + (uint64_t) tv.tv_usec;
    }
}

uint64_t _glfwPlatformGetTimerFrequency(void)
{
    return _glfw.timer.posix.frequency;
}

//========================================================================
// GLFW 3.3 POSIX - www.glfw.org
// GLFW 3.3 - www.glfw.org - posix_thread.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

GLFWbool _glfwPlatformCreateTls(_GLFWtls* tls)
{
    assert(tls->posix.allocated == GLFW_FALSE);

    if (pthread_key_create(&tls->posix.key, NULL) != 0)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "POSIX: Failed to create context TLS");
        return GLFW_FALSE;
    }

    tls->posix.allocated = GLFW_TRUE;
    return GLFW_TRUE;
}

void _glfwPlatformDestroyTls(_GLFWtls* tls)
{
    if (tls->posix.allocated)
        pthread_key_delete(tls->posix.key);
    memset(tls, 0, sizeof(_GLFWtls));
}

void* _glfwPlatformGetTls(_GLFWtls* tls)
{
    assert(tls->posix.allocated == GLFW_TRUE);
    return pthread_getspecific(tls->posix.key);
}

void _glfwPlatformSetTls(_GLFWtls* tls, void* value)
{
    assert(tls->posix.allocated == GLFW_TRUE);
    pthread_setspecific(tls->posix.key, value);
}

GLFWbool _glfwPlatformCreateMutex(_GLFWmutex* mutex)
{
    assert(mutex->posix.allocated == GLFW_FALSE);

    if (pthread_mutex_init(&mutex->posix.handle, NULL) != 0)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR, "POSIX: Failed to create mutex");
        return GLFW_FALSE;
    }

    return mutex->posix.allocated = GLFW_TRUE;
}

void _glfwPlatformDestroyMutex(_GLFWmutex* mutex)
{
    if (mutex->posix.allocated)
        pthread_mutex_destroy(&mutex->posix.handle);
    memset(mutex, 0, sizeof(_GLFWmutex));
}

void _glfwPlatformLockMutex(_GLFWmutex* mutex)
{
    assert(mutex->posix.allocated == GLFW_TRUE);
    pthread_mutex_lock(&mutex->posix.handle);
}

void _glfwPlatformUnlockMutex(_GLFWmutex* mutex)
{
    assert(mutex->posix.allocated == GLFW_TRUE);
    pthread_mutex_unlock(&mutex->posix.handle);
}

//========================================================================
// GLFW 3.3 GLX - www.glfw.org
// GLFW 3.3 - www.glfw.org - glx_context.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================

#ifndef GLXBadProfileARB
 #define GLXBadProfileARB 13
#endif


// Returns the specified attribute of the specified GLXFBConfig
//
static int getGLXFBConfigAttrib(GLXFBConfig fbconfig, int attrib)
{
    int value;
    glXGetFBConfigAttrib(_glfw.x11.display, fbconfig, attrib, &value);
    return value;
}

// Return the GLXFBConfig most closely matching the specified hints
//
static GLFWbool chooseGLXFBConfig(const _GLFWfbconfig* desired,
                                  GLXFBConfig* result)
{
    GLXFBConfig* nativeConfigs;
    _GLFWfbconfig* usableConfigs;
    const _GLFWfbconfig* closest;
    int i, nativeCount, usableCount;
    const char* vendor;
    GLFWbool trustWindowBit = GLFW_TRUE;

    // HACK: This is a (hopefully temporary) workaround for Chromium
    //       (VirtualBox GL) not setting the window bit on any GLXFBConfigs
    vendor = glXGetClientString(_glfw.x11.display, GLX_VENDOR);
    if (vendor && strcmp(vendor, "Chromium") == 0)
        trustWindowBit = GLFW_FALSE;

    nativeConfigs =
        glXGetFBConfigs(_glfw.x11.display, _glfw.x11.screen, &nativeCount);
    if (!nativeConfigs || !nativeCount)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "GLX: No GLXFBConfigs returned");
        return GLFW_FALSE;
    }

    usableConfigs = calloc(nativeCount, sizeof(_GLFWfbconfig));
    usableCount = 0;

    for (i = 0;  i < nativeCount;  i++)
    {
        const GLXFBConfig n = nativeConfigs[i];
        _GLFWfbconfig* u = usableConfigs + usableCount;

        // Only consider RGBA GLXFBConfigs
        if (!(getGLXFBConfigAttrib(n, GLX_RENDER_TYPE) & GLX_RGBA_BIT))
            continue;

        // Only consider window GLXFBConfigs
        if (!(getGLXFBConfigAttrib(n, GLX_DRAWABLE_TYPE) & GLX_WINDOW_BIT))
        {
            if (trustWindowBit)
                continue;
        }

        if (getGLXFBConfigAttrib(n, GLX_DOUBLEBUFFER) != desired->doublebuffer)
            continue;

        if (desired->transparent)
        {
            XVisualInfo* vi = glXGetVisualFromFBConfig(_glfw.x11.display, n);
            if (vi)
            {
                u->transparent = _glfwIsVisualTransparentX11(vi->visual);
                XFree(vi);
            }
        }

        u->redBits = getGLXFBConfigAttrib(n, GLX_RED_SIZE);
        u->greenBits = getGLXFBConfigAttrib(n, GLX_GREEN_SIZE);
        u->blueBits = getGLXFBConfigAttrib(n, GLX_BLUE_SIZE);

        u->alphaBits = getGLXFBConfigAttrib(n, GLX_ALPHA_SIZE);
        u->depthBits = getGLXFBConfigAttrib(n, GLX_DEPTH_SIZE);
        u->stencilBits = getGLXFBConfigAttrib(n, GLX_STENCIL_SIZE);

        u->accumRedBits = getGLXFBConfigAttrib(n, GLX_ACCUM_RED_SIZE);
        u->accumGreenBits = getGLXFBConfigAttrib(n, GLX_ACCUM_GREEN_SIZE);
        u->accumBlueBits = getGLXFBConfigAttrib(n, GLX_ACCUM_BLUE_SIZE);
        u->accumAlphaBits = getGLXFBConfigAttrib(n, GLX_ACCUM_ALPHA_SIZE);

        u->auxBuffers = getGLXFBConfigAttrib(n, GLX_AUX_BUFFERS);

        if (getGLXFBConfigAttrib(n, GLX_STEREO))
            u->stereo = GLFW_TRUE;

        if (_glfw.glx.ARB_multisample)
            u->samples = getGLXFBConfigAttrib(n, GLX_SAMPLES);

        if (_glfw.glx.ARB_framebuffer_sRGB || _glfw.glx.EXT_framebuffer_sRGB)
            u->sRGB = getGLXFBConfigAttrib(n, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB);

        u->handle = (uintptr_t) n;
        usableCount++;
    }

    closest = _glfwChooseFBConfig(desired, usableConfigs, usableCount);
    if (closest)
        *result = (GLXFBConfig) closest->handle;

    XFree(nativeConfigs);
    free(usableConfigs);

    return closest != NULL;
}

// Create the OpenGL context using legacy API
//
static GLXContext createLegacyContextGLX(_GLFWwindow* window,
                                         GLXFBConfig fbconfig,
                                         GLXContext share)
{
    return glXCreateNewContext(_glfw.x11.display,
                               fbconfig,
                               GLX_RGBA_TYPE,
                               share,
                               True);
}

static void makeContextCurrentGLX(_GLFWwindow* window)
{
    if (window)
    {
        if (!glXMakeCurrent(_glfw.x11.display,
                            window->context.glx.window,
                            window->context.glx.handle))
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "GLX: Failed to make context current");
            return;
        }
    }
    else
    {
        if (!glXMakeCurrent(_glfw.x11.display, None, NULL))
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "GLX: Failed to clear current context");
            return;
        }
    }

    _glfwPlatformSetTls(&_glfw.contextSlot, window);
}

static void swapBuffersGLX(_GLFWwindow* window)
{
    glXSwapBuffers(_glfw.x11.display, window->context.glx.window);
}

static void swapIntervalGLX(int interval)
{
    _GLFWwindow* window = _glfwPlatformGetTls(&_glfw.contextSlot);

    if (_glfw.glx.EXT_swap_control)
    {
        _glfw.glx.SwapIntervalEXT(_glfw.x11.display,
                                  window->context.glx.window,
                                  interval);
    }
    else if (_glfw.glx.MESA_swap_control)
        _glfw.glx.SwapIntervalMESA(interval);
    else if (_glfw.glx.SGI_swap_control)
    {
        if (interval > 0)
            _glfw.glx.SwapIntervalSGI(interval);
    }
}

static int extensionSupportedGLX(const char* extension)
{
    const char* extensions =
        glXQueryExtensionsString(_glfw.x11.display, _glfw.x11.screen);
    if (extensions)
    {
        if (_glfwStringInExtensionString(extension, extensions))
            return GLFW_TRUE;
    }

    return GLFW_FALSE;
}

static GLFWglproc getProcAddressGLX(const char* procname)
{
    if (_glfw.glx.GetProcAddress)
        return _glfw.glx.GetProcAddress((const GLubyte*) procname);
    else if (_glfw.glx.GetProcAddressARB)
        return _glfw.glx.GetProcAddressARB((const GLubyte*) procname);
    else
        return _glfw_dlsym(_glfw.glx.handle, procname);
}

static void destroyContextGLX(_GLFWwindow* window)
{
    if (window->context.glx.window)
    {
        glXDestroyWindow(_glfw.x11.display, window->context.glx.window);
        window->context.glx.window = None;
    }

    if (window->context.glx.handle)
    {
        glXDestroyContext(_glfw.x11.display, window->context.glx.handle);
        window->context.glx.handle = NULL;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Initialize GLX
//
GLFWbool _glfwInitGLX(void)
{
    int i;
    const char* sonames[] =
    {
#if defined(_GLFW_GLX_LIBRARY)
        _GLFW_GLX_LIBRARY,
#elif defined(__CYGWIN__)
        "libGL-1.so",
#elif defined(__OpenBSD__) || defined(__NetBSD__)
        "libGL.so",
#else
        "libGL.so.1",
        "libGL.so",
#endif
        NULL
    };

    if (_glfw.glx.handle)
        return GLFW_TRUE;

    for (i = 0;  sonames[i];  i++)
    {
        _glfw.glx.handle = _glfw_dlopen(sonames[i]);
        if (_glfw.glx.handle)
            break;
    }

    if (!_glfw.glx.handle)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "GLX: Failed to load GLX");
        return GLFW_FALSE;
    }

    _glfw.glx.GetFBConfigs =
        _glfw_dlsym(_glfw.glx.handle, "glXGetFBConfigs");
    _glfw.glx.GetFBConfigAttrib =
        _glfw_dlsym(_glfw.glx.handle, "glXGetFBConfigAttrib");
    _glfw.glx.GetClientString =
        _glfw_dlsym(_glfw.glx.handle, "glXGetClientString");
    _glfw.glx.QueryExtension =
        _glfw_dlsym(_glfw.glx.handle, "glXQueryExtension");
    _glfw.glx.QueryVersion =
        _glfw_dlsym(_glfw.glx.handle, "glXQueryVersion");
    _glfw.glx.DestroyContext =
        _glfw_dlsym(_glfw.glx.handle, "glXDestroyContext");
    _glfw.glx.MakeCurrent =
        _glfw_dlsym(_glfw.glx.handle, "glXMakeCurrent");
    _glfw.glx.SwapBuffers =
        _glfw_dlsym(_glfw.glx.handle, "glXSwapBuffers");
    _glfw.glx.QueryExtensionsString =
        _glfw_dlsym(_glfw.glx.handle, "glXQueryExtensionsString");
    _glfw.glx.CreateNewContext =
        _glfw_dlsym(_glfw.glx.handle, "glXCreateNewContext");
    _glfw.glx.CreateWindow =
        _glfw_dlsym(_glfw.glx.handle, "glXCreateWindow");
    _glfw.glx.DestroyWindow =
        _glfw_dlsym(_glfw.glx.handle, "glXDestroyWindow");
    _glfw.glx.GetVisualFromFBConfig =
        _glfw_dlsym(_glfw.glx.handle, "glXGetVisualFromFBConfig");

    if (!_glfw.glx.GetFBConfigs ||
        !_glfw.glx.GetFBConfigAttrib ||
        !_glfw.glx.GetClientString ||
        !_glfw.glx.QueryExtension ||
        !_glfw.glx.QueryVersion ||
        !_glfw.glx.DestroyContext ||
        !_glfw.glx.MakeCurrent ||
        !_glfw.glx.SwapBuffers ||
        !_glfw.glx.QueryExtensionsString ||
        !_glfw.glx.CreateNewContext ||
        !_glfw.glx.CreateWindow ||
        !_glfw.glx.DestroyWindow ||
        !_glfw.glx.GetVisualFromFBConfig)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "GLX: Failed to load required entry points");
        return GLFW_FALSE;
    }

    // NOTE: Unlike GLX 1.3 entry points these are not required to be present
    _glfw.glx.GetProcAddress = (PFNGLXGETPROCADDRESSPROC)
        _glfw_dlsym(_glfw.glx.handle, "glXGetProcAddress");
    _glfw.glx.GetProcAddressARB = (PFNGLXGETPROCADDRESSPROC)
        _glfw_dlsym(_glfw.glx.handle, "glXGetProcAddressARB");

    if (!glXQueryExtension(_glfw.x11.display,
                           &_glfw.glx.errorBase,
                           &_glfw.glx.eventBase))
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "GLX: GLX extension not found");
        return GLFW_FALSE;
    }

    if (!glXQueryVersion(_glfw.x11.display, &_glfw.glx.major, &_glfw.glx.minor))
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "GLX: Failed to query GLX version");
        return GLFW_FALSE;
    }

    if (_glfw.glx.major == 1 && _glfw.glx.minor < 3)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "GLX: GLX version 1.3 is required");
        return GLFW_FALSE;
    }

    if (extensionSupportedGLX("GLX_EXT_swap_control"))
    {
        _glfw.glx.SwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)
            getProcAddressGLX("glXSwapIntervalEXT");

        if (_glfw.glx.SwapIntervalEXT)
            _glfw.glx.EXT_swap_control = GLFW_TRUE;
    }

    if (extensionSupportedGLX("GLX_SGI_swap_control"))
    {
        _glfw.glx.SwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC)
            getProcAddressGLX("glXSwapIntervalSGI");

        if (_glfw.glx.SwapIntervalSGI)
            _glfw.glx.SGI_swap_control = GLFW_TRUE;
    }

    if (extensionSupportedGLX("GLX_MESA_swap_control"))
    {
        _glfw.glx.SwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC)
            getProcAddressGLX("glXSwapIntervalMESA");

        if (_glfw.glx.SwapIntervalMESA)
            _glfw.glx.MESA_swap_control = GLFW_TRUE;
    }

    if (extensionSupportedGLX("GLX_ARB_multisample"))
        _glfw.glx.ARB_multisample = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_ARB_framebuffer_sRGB"))
        _glfw.glx.ARB_framebuffer_sRGB = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_EXT_framebuffer_sRGB"))
        _glfw.glx.EXT_framebuffer_sRGB = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_ARB_create_context"))
    {
        _glfw.glx.CreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)
            getProcAddressGLX("glXCreateContextAttribsARB");

        if (_glfw.glx.CreateContextAttribsARB)
            _glfw.glx.ARB_create_context = GLFW_TRUE;
    }

    if (extensionSupportedGLX("GLX_ARB_create_context_robustness"))
        _glfw.glx.ARB_create_context_robustness = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_ARB_create_context_profile"))
        _glfw.glx.ARB_create_context_profile = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_EXT_create_context_es2_profile"))
        _glfw.glx.EXT_create_context_es2_profile = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_ARB_create_context_no_error"))
        _glfw.glx.ARB_create_context_no_error = GLFW_TRUE;

    if (extensionSupportedGLX("GLX_ARB_context_flush_control"))
        _glfw.glx.ARB_context_flush_control = GLFW_TRUE;

    return GLFW_TRUE;
}

// Terminate GLX
//
void _glfwTerminateGLX(void)
{
    // NOTE: This function must not call any X11 functions, as it is called
    //       after XCloseDisplay (see _glfwPlatformTerminate for details)

    if (_glfw.glx.handle)
    {
        _glfw_dlclose(_glfw.glx.handle);
        _glfw.glx.handle = NULL;
    }
}

#define setAttrib(a, v) \
{ \
    assert(((size_t) index + 1) < sizeof(attribs) / sizeof(attribs[0])); \
    attribs[index++] = a; \
    attribs[index++] = v; \
}

// Create the OpenGL or OpenGL ES context
//
GLFWbool _glfwCreateContextGLX(_GLFWwindow* window,
                               const _GLFWctxconfig* ctxconfig,
                               const _GLFWfbconfig* fbconfig)
{
    int attribs[40];
    GLXFBConfig native = NULL;
    GLXContext share = NULL;

    if (ctxconfig->share)
        share = ctxconfig->share->context.glx.handle;

    if (!chooseGLXFBConfig(fbconfig, &native))
    {
        _glfwInputError(GLFW_FORMAT_UNAVAILABLE,
                        "GLX: Failed to find a suitable GLXFBConfig");
        return GLFW_FALSE;
    }

    if (ctxconfig->client == GLFW_OPENGL_ES_API)
    {
        if (!_glfw.glx.ARB_create_context ||
            !_glfw.glx.ARB_create_context_profile ||
            !_glfw.glx.EXT_create_context_es2_profile)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "GLX: OpenGL ES requested but GLX_EXT_create_context_es2_profile is unavailable");
            return GLFW_FALSE;
        }
    }

    if (ctxconfig->forward)
    {
        if (!_glfw.glx.ARB_create_context)
        {
            _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                            "GLX: Forward compatibility requested but GLX_ARB_create_context_profile is unavailable");
            return GLFW_FALSE;
        }
    }

    if (ctxconfig->profile)
    {
        if (!_glfw.glx.ARB_create_context ||
            !_glfw.glx.ARB_create_context_profile)
        {
            _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                            "GLX: An OpenGL profile requested but GLX_ARB_create_context_profile is unavailable");
            return GLFW_FALSE;
        }
    }

    _glfwGrabErrorHandlerX11();

    if (_glfw.glx.ARB_create_context)
    {
        int index = 0, mask = 0, flags = 0;

        if (ctxconfig->client == GLFW_OPENGL_API)
        {
            if (ctxconfig->forward)
                flags |= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

            if (ctxconfig->profile == GLFW_OPENGL_CORE_PROFILE)
                mask |= GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
            else if (ctxconfig->profile == GLFW_OPENGL_COMPAT_PROFILE)
                mask |= GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
        }
        else
            mask |= GLX_CONTEXT_ES2_PROFILE_BIT_EXT;

        if (ctxconfig->debug)
            flags |= GLX_CONTEXT_DEBUG_BIT_ARB;

        if (ctxconfig->robustness)
        {
            if (_glfw.glx.ARB_create_context_robustness)
            {
                if (ctxconfig->robustness == GLFW_NO_RESET_NOTIFICATION)
                {
                    setAttrib(GLX_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB,
                              GLX_NO_RESET_NOTIFICATION_ARB);
                }
                else if (ctxconfig->robustness == GLFW_LOSE_CONTEXT_ON_RESET)
                {
                    setAttrib(GLX_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB,
                              GLX_LOSE_CONTEXT_ON_RESET_ARB);
                }

                flags |= GLX_CONTEXT_ROBUST_ACCESS_BIT_ARB;
            }
        }

        if (ctxconfig->release)
        {
            if (_glfw.glx.ARB_context_flush_control)
            {
                if (ctxconfig->release == GLFW_RELEASE_BEHAVIOR_NONE)
                {
                    setAttrib(GLX_CONTEXT_RELEASE_BEHAVIOR_ARB,
                              GLX_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB);
                }
                else if (ctxconfig->release == GLFW_RELEASE_BEHAVIOR_FLUSH)
                {
                    setAttrib(GLX_CONTEXT_RELEASE_BEHAVIOR_ARB,
                              GLX_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB);
                }
            }
        }

        if (ctxconfig->noerror)
        {
            if (_glfw.glx.ARB_create_context_no_error)
                setAttrib(GLX_CONTEXT_OPENGL_NO_ERROR_ARB, GLFW_TRUE);
        }

        // NOTE: Only request an explicitly versioned context when necessary, as
        //       explicitly requesting version 1.0 does not always return the
        //       highest version supported by the driver
        if (ctxconfig->major != 1 || ctxconfig->minor != 0)
        {
            setAttrib(GLX_CONTEXT_MAJOR_VERSION_ARB, ctxconfig->major);
            setAttrib(GLX_CONTEXT_MINOR_VERSION_ARB, ctxconfig->minor);
        }

        if (mask)
            setAttrib(GLX_CONTEXT_PROFILE_MASK_ARB, mask);

        if (flags)
            setAttrib(GLX_CONTEXT_FLAGS_ARB, flags);

        setAttrib(None, None);

        window->context.glx.handle =
            _glfw.glx.CreateContextAttribsARB(_glfw.x11.display,
                                              native,
                                              share,
                                              True,
                                              attribs);

        // HACK: This is a fallback for broken versions of the Mesa
        //       implementation of GLX_ARB_create_context_profile that fail
        //       default 1.0 context creation with a GLXBadProfileARB error in
        //       violation of the extension spec
        if (!window->context.glx.handle)
        {
            if (_glfw.x11.errorCode == _glfw.glx.errorBase + GLXBadProfileARB &&
                ctxconfig->client == GLFW_OPENGL_API &&
                ctxconfig->profile == GLFW_OPENGL_ANY_PROFILE &&
                ctxconfig->forward == GLFW_FALSE)
            {
                window->context.glx.handle =
                    createLegacyContextGLX(window, native, share);
            }
        }
    }
    else
    {
        window->context.glx.handle =
            createLegacyContextGLX(window, native, share);
    }

    _glfwReleaseErrorHandlerX11();

    if (!window->context.glx.handle)
    {
        _glfwInputErrorX11(GLFW_VERSION_UNAVAILABLE, "GLX: Failed to create context");
        return GLFW_FALSE;
    }

    window->context.glx.window =
        glXCreateWindow(_glfw.x11.display, native, window->x11.handle, NULL);
    if (!window->context.glx.window)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR, "GLX: Failed to create window");
        return GLFW_FALSE;
    }

    window->context.makeCurrent = makeContextCurrentGLX;
    window->context.swapBuffers = swapBuffersGLX;
    window->context.swapInterval = swapIntervalGLX;
    window->context.extensionSupported = extensionSupportedGLX;
    window->context.getProcAddress = getProcAddressGLX;
    window->context.destroy = destroyContextGLX;

    return GLFW_TRUE;
}

#undef setAttrib

// Returns the Visual and depth of the chosen GLXFBConfig
//
GLFWbool _glfwChooseVisualGLX(const _GLFWwndconfig* wndconfig,
                              const _GLFWctxconfig* ctxconfig,
                              const _GLFWfbconfig* fbconfig,
                              Visual** visual, int* depth)
{
    GLXFBConfig native;
    XVisualInfo* result;

    if (!chooseGLXFBConfig(fbconfig, &native))
    {
        _glfwInputError(GLFW_FORMAT_UNAVAILABLE,
                        "GLX: Failed to find a suitable GLXFBConfig");
        return GLFW_FALSE;
    }

    result = glXGetVisualFromFBConfig(_glfw.x11.display, native);
    if (!result)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "GLX: Failed to retrieve Visual for GLXFBConfig");
        return GLFW_FALSE;
    }

    *visual = result->visual;
    *depth  = result->depth;

    XFree(result);
    return GLFW_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI GLXContext glfwGetGLXContext(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (window->context.source != GLFW_NATIVE_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return NULL;
    }

    return window->context.glx.handle;
}

GLFWAPI GLXWindow glfwGetGLXWindow(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(None);

    if (window->context.source != GLFW_NATIVE_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return None;
    }

    return window->context.glx.window;
}

//========================================================================
// GLFW 3.3 EGL - www.glfw.org
// GLFW 3.3 - www.glfw.org - egl_context.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2019 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================


// Return a description of the specified EGL error
//
static const char* getEGLErrorString(EGLint error)
{
    switch (error)
    {
        case EGL_SUCCESS:
            return "Success";
        case EGL_NOT_INITIALIZED:
            return "EGL is not or could not be initialized";
        case EGL_BAD_ACCESS:
            return "EGL cannot access a requested resource";
        case EGL_BAD_ALLOC:
            return "EGL failed to allocate resources for the requested operation";
        case EGL_BAD_ATTRIBUTE:
            return "An unrecognized attribute or attribute value was passed in the attribute list";
        case EGL_BAD_CONTEXT:
            return "An EGLContext argument does not name a valid EGL rendering context";
        case EGL_BAD_CONFIG:
            return "An EGLConfig argument does not name a valid EGL frame buffer configuration";
        case EGL_BAD_CURRENT_SURFACE:
            return "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid";
        case EGL_BAD_DISPLAY:
            return "An EGLDisplay argument does not name a valid EGL display connection";
        case EGL_BAD_SURFACE:
            return "An EGLSurface argument does not name a valid surface configured for GL rendering";
        case EGL_BAD_MATCH:
            return "Arguments are inconsistent";
        case EGL_BAD_PARAMETER:
            return "One or more argument values are invalid";
        case EGL_BAD_NATIVE_PIXMAP:
            return "A NativePixmapType argument does not refer to a valid native pixmap";
        case EGL_BAD_NATIVE_WINDOW:
            return "A NativeWindowType argument does not refer to a valid native window";
        case EGL_CONTEXT_LOST:
            return "The application must destroy all contexts and reinitialise";
        default:
            return "ERROR: UNKNOWN EGL ERROR";
    }
}

// Returns the specified attribute of the specified EGLConfig
//
static int getEGLConfigAttrib(EGLConfig config, int attrib)
{
    int value;
    eglGetConfigAttrib(_glfw.egl.display, config, attrib, &value);
    return value;
}

// Return the EGLConfig most closely matching the specified hints
//
static GLFWbool chooseEGLConfig(const _GLFWctxconfig* ctxconfig,
                                const _GLFWfbconfig* desired,
                                EGLConfig* result)
{
    EGLConfig* nativeConfigs;
    _GLFWfbconfig* usableConfigs;
    const _GLFWfbconfig* closest;
    int i, nativeCount, usableCount;

    eglGetConfigs(_glfw.egl.display, NULL, 0, &nativeCount);
    if (!nativeCount)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "EGL: No EGLConfigs returned");
        return GLFW_FALSE;
    }

    nativeConfigs = calloc(nativeCount, sizeof(EGLConfig));
    eglGetConfigs(_glfw.egl.display, nativeConfigs, nativeCount, &nativeCount);

    usableConfigs = calloc(nativeCount, sizeof(_GLFWfbconfig));
    usableCount = 0;

    for (i = 0;  i < nativeCount;  i++)
    {
        const EGLConfig n = nativeConfigs[i];
        _GLFWfbconfig* u = usableConfigs + usableCount;

        // Only consider RGB(A) EGLConfigs
        if (getEGLConfigAttrib(n, EGL_COLOR_BUFFER_TYPE) != EGL_RGB_BUFFER)
            continue;

        // Only consider window EGLConfigs
        if (!(getEGLConfigAttrib(n, EGL_SURFACE_TYPE) & EGL_WINDOW_BIT))
            continue;

#if defined(_GLFW_X11)
        {
            XVisualInfo vi = {0};

            // Only consider EGLConfigs with associated Visuals
            vi.visualid = getEGLConfigAttrib(n, EGL_NATIVE_VISUAL_ID);
            if (!vi.visualid)
                continue;

            if (desired->transparent)
            {
                int count;
                XVisualInfo* vis =
                    XGetVisualInfo(_glfw.x11.display, VisualIDMask, &vi, &count);
                if (vis)
                {
                    u->transparent = _glfwIsVisualTransparentX11(vis[0].visual);
                    XFree(vis);
                }
            }
        }
#endif // _GLFW_X11

        if (ctxconfig->client == GLFW_OPENGL_ES_API)
        {
            if (ctxconfig->major == 1)
            {
                if (!(getEGLConfigAttrib(n, EGL_RENDERABLE_TYPE) & EGL_OPENGL_ES_BIT))
                    continue;
            }
            else
            {
                if (!(getEGLConfigAttrib(n, EGL_RENDERABLE_TYPE) & EGL_OPENGL_ES2_BIT))
                    continue;
            }
        }
        else if (ctxconfig->client == GLFW_OPENGL_API)
        {
            if (!(getEGLConfigAttrib(n, EGL_RENDERABLE_TYPE) & EGL_OPENGL_BIT))
                continue;
        }

        u->redBits = getEGLConfigAttrib(n, EGL_RED_SIZE);
        u->greenBits = getEGLConfigAttrib(n, EGL_GREEN_SIZE);
        u->blueBits = getEGLConfigAttrib(n, EGL_BLUE_SIZE);

        u->alphaBits = getEGLConfigAttrib(n, EGL_ALPHA_SIZE);
        u->depthBits = getEGLConfigAttrib(n, EGL_DEPTH_SIZE);
        u->stencilBits = getEGLConfigAttrib(n, EGL_STENCIL_SIZE);

        u->samples = getEGLConfigAttrib(n, EGL_SAMPLES);
        u->doublebuffer = desired->doublebuffer;

        u->handle = (uintptr_t) n;
        usableCount++;
    }

    closest = _glfwChooseFBConfig(desired, usableConfigs, usableCount);
    if (closest)
        *result = (EGLConfig) closest->handle;

    free(nativeConfigs);
    free(usableConfigs);

    return closest != NULL;
}

static void makeContextCurrentEGL(_GLFWwindow* window)
{
    if (window)
    {
        if (!eglMakeCurrent(_glfw.egl.display,
                            window->context.egl.surface,
                            window->context.egl.surface,
                            window->context.egl.handle))
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "EGL: Failed to make context current: %s",
                            getEGLErrorString(eglGetError()));
            return;
        }
    }
    else
    {
        if (!eglMakeCurrent(_glfw.egl.display,
                            EGL_NO_SURFACE,
                            EGL_NO_SURFACE,
                            EGL_NO_CONTEXT))
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "EGL: Failed to clear current context: %s",
                            getEGLErrorString(eglGetError()));
            return;
        }
    }

    _glfwPlatformSetTls(&_glfw.contextSlot, window);
}

static void swapBuffersEGL(_GLFWwindow* window)
{
    if (window != _glfwPlatformGetTls(&_glfw.contextSlot))
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "EGL: The context must be current on the calling thread when swapping buffers");
        return;
    }

#if defined(_GLFW_WAYLAND)
    // NOTE: Swapping buffers on a hidden window on Wayland makes it visible
    if (!window->wl.visible)
        return;
#endif

    eglSwapBuffers(_glfw.egl.display, window->context.egl.surface);
}

static void swapIntervalEGL(int interval)
{
    eglSwapInterval(_glfw.egl.display, interval);
}

static int extensionSupportedEGL(const char* extension)
{
    const char* extensions = eglQueryString(_glfw.egl.display, EGL_EXTENSIONS);
    if (extensions)
    {
        if (_glfwStringInExtensionString(extension, extensions))
            return GLFW_TRUE;
    }

    return GLFW_FALSE;
}

static GLFWglproc getProcAddressEGL(const char* procname)
{
    _GLFWwindow* window = _glfwPlatformGetTls(&_glfw.contextSlot);

    if (window->context.egl.client)
    {
        GLFWglproc proc = (GLFWglproc) _glfw_dlsym(window->context.egl.client,
                                                   procname);
        if (proc)
            return proc;
    }

    return eglGetProcAddress(procname);
}

static void destroyContextEGL(_GLFWwindow* window)
{
#if defined(_GLFW_X11)
    // NOTE: Do not unload libGL.so.1 while the X11 display is still open,
    //       as it will make XCloseDisplay segfault
    if (window->context.client != GLFW_OPENGL_API)
#endif // _GLFW_X11
    {
        if (window->context.egl.client)
        {
            _glfw_dlclose(window->context.egl.client);
            window->context.egl.client = NULL;
        }
    }

    if (window->context.egl.surface)
    {
        eglDestroySurface(_glfw.egl.display, window->context.egl.surface);
        window->context.egl.surface = EGL_NO_SURFACE;
    }

    if (window->context.egl.handle)
    {
        eglDestroyContext(_glfw.egl.display, window->context.egl.handle);
        window->context.egl.handle = EGL_NO_CONTEXT;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Initialize EGL
//
GLFWbool _glfwInitEGL(void)
{
    int i;
    const char* sonames[] =
    {
#if defined(_GLFW_EGL_LIBRARY)
        _GLFW_EGL_LIBRARY,
#elif defined(_GLFW_WIN32)
        "libEGL.dll",
        "EGL.dll",
#elif defined(_GLFW_COCOA)
        "libEGL.dylib",
#elif defined(__CYGWIN__)
        "libEGL-1.so",
#elif defined(__OpenBSD__) || defined(__NetBSD__)
        "libEGL.so",
#else
        "libEGL.so.1",
#endif
        NULL
    };

    if (_glfw.egl.handle)
        return GLFW_TRUE;

    for (i = 0;  sonames[i];  i++)
    {
        _glfw.egl.handle = _glfw_dlopen(sonames[i]);
        if (_glfw.egl.handle)
            break;
    }

    if (!_glfw.egl.handle)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "EGL: Library not found");
        return GLFW_FALSE;
    }

    _glfw.egl.prefix = (strncmp(sonames[i], "lib", 3) == 0);

    _glfw.egl.GetConfigAttrib = (PFN_eglGetConfigAttrib)
        _glfw_dlsym(_glfw.egl.handle, "eglGetConfigAttrib");
    _glfw.egl.GetConfigs = (PFN_eglGetConfigs)
        _glfw_dlsym(_glfw.egl.handle, "eglGetConfigs");
    _glfw.egl.GetDisplay = (PFN_eglGetDisplay)
        _glfw_dlsym(_glfw.egl.handle, "eglGetDisplay");
    _glfw.egl.GetError = (PFN_eglGetError)
        _glfw_dlsym(_glfw.egl.handle, "eglGetError");
    _glfw.egl.Initialize = (PFN_eglInitialize)
        _glfw_dlsym(_glfw.egl.handle, "eglInitialize");
    _glfw.egl.Terminate = (PFN_eglTerminate)
        _glfw_dlsym(_glfw.egl.handle, "eglTerminate");
    _glfw.egl.BindAPI = (PFN_eglBindAPI)
        _glfw_dlsym(_glfw.egl.handle, "eglBindAPI");
    _glfw.egl.CreateContext = (PFN_eglCreateContext)
        _glfw_dlsym(_glfw.egl.handle, "eglCreateContext");
    _glfw.egl.DestroySurface = (PFN_eglDestroySurface)
        _glfw_dlsym(_glfw.egl.handle, "eglDestroySurface");
    _glfw.egl.DestroyContext = (PFN_eglDestroyContext)
        _glfw_dlsym(_glfw.egl.handle, "eglDestroyContext");
    _glfw.egl.CreateWindowSurface = (PFN_eglCreateWindowSurface)
        _glfw_dlsym(_glfw.egl.handle, "eglCreateWindowSurface");
    _glfw.egl.MakeCurrent = (PFN_eglMakeCurrent)
        _glfw_dlsym(_glfw.egl.handle, "eglMakeCurrent");
    _glfw.egl.SwapBuffers = (PFN_eglSwapBuffers)
        _glfw_dlsym(_glfw.egl.handle, "eglSwapBuffers");
    _glfw.egl.SwapInterval = (PFN_eglSwapInterval)
        _glfw_dlsym(_glfw.egl.handle, "eglSwapInterval");
    _glfw.egl.QueryString = (PFN_eglQueryString)
        _glfw_dlsym(_glfw.egl.handle, "eglQueryString");
    _glfw.egl.GetProcAddress = (PFN_eglGetProcAddress)
        _glfw_dlsym(_glfw.egl.handle, "eglGetProcAddress");

    if (!_glfw.egl.GetConfigAttrib ||
        !_glfw.egl.GetConfigs ||
        !_glfw.egl.GetDisplay ||
        !_glfw.egl.GetError ||
        !_glfw.egl.Initialize ||
        !_glfw.egl.Terminate ||
        !_glfw.egl.BindAPI ||
        !_glfw.egl.CreateContext ||
        !_glfw.egl.DestroySurface ||
        !_glfw.egl.DestroyContext ||
        !_glfw.egl.CreateWindowSurface ||
        !_glfw.egl.MakeCurrent ||
        !_glfw.egl.SwapBuffers ||
        !_glfw.egl.SwapInterval ||
        !_glfw.egl.QueryString ||
        !_glfw.egl.GetProcAddress)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "EGL: Failed to load required entry points");

        _glfwTerminateEGL();
        return GLFW_FALSE;
    }

    _glfw.egl.display = eglGetDisplay(_GLFW_EGL_NATIVE_DISPLAY);
    if (_glfw.egl.display == EGL_NO_DISPLAY)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "EGL: Failed to get EGL display: %s",
                        getEGLErrorString(eglGetError()));

        _glfwTerminateEGL();
        return GLFW_FALSE;
    }

    if (!eglInitialize(_glfw.egl.display, &_glfw.egl.major, &_glfw.egl.minor))
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "EGL: Failed to initialize EGL: %s",
                        getEGLErrorString(eglGetError()));

        _glfwTerminateEGL();
        return GLFW_FALSE;
    }

    _glfw.egl.KHR_create_context =
        extensionSupportedEGL("EGL_KHR_create_context");
    _glfw.egl.KHR_create_context_no_error =
        extensionSupportedEGL("EGL_KHR_create_context_no_error");
    _glfw.egl.KHR_gl_colorspace =
        extensionSupportedEGL("EGL_KHR_gl_colorspace");
    _glfw.egl.KHR_get_all_proc_addresses =
        extensionSupportedEGL("EGL_KHR_get_all_proc_addresses");
    _glfw.egl.KHR_context_flush_control =
        extensionSupportedEGL("EGL_KHR_context_flush_control");
    _glfw.egl.EXT_present_opaque =
        extensionSupportedEGL("EGL_EXT_present_opaque");

    return GLFW_TRUE;
}

// Terminate EGL
//
void _glfwTerminateEGL(void)
{
    if (_glfw.egl.display)
    {
        eglTerminate(_glfw.egl.display);
        _glfw.egl.display = EGL_NO_DISPLAY;
    }

    if (_glfw.egl.handle)
    {
        _glfw_dlclose(_glfw.egl.handle);
        _glfw.egl.handle = NULL;
    }
}

#define setAttrib(a, v) \
{ \
    assert(((size_t) index + 1) < sizeof(attribs) / sizeof(attribs[0])); \
    attribs[index++] = a; \
    attribs[index++] = v; \
}

// Create the OpenGL or OpenGL ES context
//
GLFWbool _glfwCreateContextEGL(_GLFWwindow* window,
                               const _GLFWctxconfig* ctxconfig,
                               const _GLFWfbconfig* fbconfig)
{
    EGLint attribs[40];
    EGLConfig config;
    EGLContext share = NULL;
    int index = 0;

    if (!_glfw.egl.display)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "EGL: API not available");
        return GLFW_FALSE;
    }

    if (ctxconfig->share)
        share = ctxconfig->share->context.egl.handle;

    if (!chooseEGLConfig(ctxconfig, fbconfig, &config))
    {
        _glfwInputError(GLFW_FORMAT_UNAVAILABLE,
                        "EGL: Failed to find a suitable EGLConfig");
        return GLFW_FALSE;
    }

    if (ctxconfig->client == GLFW_OPENGL_ES_API)
    {
        if (!eglBindAPI(EGL_OPENGL_ES_API))
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "EGL: Failed to bind OpenGL ES: %s",
                            getEGLErrorString(eglGetError()));
            return GLFW_FALSE;
        }
    }
    else
    {
        if (!eglBindAPI(EGL_OPENGL_API))
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "EGL: Failed to bind OpenGL: %s",
                            getEGLErrorString(eglGetError()));
            return GLFW_FALSE;
        }
    }

    if (_glfw.egl.KHR_create_context)
    {
        int mask = 0, flags = 0;

        if (ctxconfig->client == GLFW_OPENGL_API)
        {
            if (ctxconfig->forward)
                flags |= EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR;

            if (ctxconfig->profile == GLFW_OPENGL_CORE_PROFILE)
                mask |= EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR;
            else if (ctxconfig->profile == GLFW_OPENGL_COMPAT_PROFILE)
                mask |= EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR;
        }

        if (ctxconfig->debug)
            flags |= EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR;

        if (ctxconfig->robustness)
        {
            if (ctxconfig->robustness == GLFW_NO_RESET_NOTIFICATION)
            {
                setAttrib(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR,
                          EGL_NO_RESET_NOTIFICATION_KHR);
            }
            else if (ctxconfig->robustness == GLFW_LOSE_CONTEXT_ON_RESET)
            {
                setAttrib(EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR,
                          EGL_LOSE_CONTEXT_ON_RESET_KHR);
            }

            flags |= EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR;
        }

        if (ctxconfig->noerror)
        {
            if (_glfw.egl.KHR_create_context_no_error)
                setAttrib(EGL_CONTEXT_OPENGL_NO_ERROR_KHR, GLFW_TRUE);
        }

        if (ctxconfig->major != 1 || ctxconfig->minor != 0)
        {
            setAttrib(EGL_CONTEXT_MAJOR_VERSION_KHR, ctxconfig->major);
            setAttrib(EGL_CONTEXT_MINOR_VERSION_KHR, ctxconfig->minor);
        }

        if (mask)
            setAttrib(EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, mask);

        if (flags)
            setAttrib(EGL_CONTEXT_FLAGS_KHR, flags);
    }
    else
    {
        if (ctxconfig->client == GLFW_OPENGL_ES_API)
            setAttrib(EGL_CONTEXT_CLIENT_VERSION, ctxconfig->major);
    }

    if (_glfw.egl.KHR_context_flush_control)
    {
        if (ctxconfig->release == GLFW_RELEASE_BEHAVIOR_NONE)
        {
            setAttrib(EGL_CONTEXT_RELEASE_BEHAVIOR_KHR,
                      EGL_CONTEXT_RELEASE_BEHAVIOR_NONE_KHR);
        }
        else if (ctxconfig->release == GLFW_RELEASE_BEHAVIOR_FLUSH)
        {
            setAttrib(EGL_CONTEXT_RELEASE_BEHAVIOR_KHR,
                      EGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_KHR);
        }
    }

    setAttrib(EGL_NONE, EGL_NONE);

    window->context.egl.handle = eglCreateContext(_glfw.egl.display,
                                                  config, share, attribs);

    if (window->context.egl.handle == EGL_NO_CONTEXT)
    {
        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                        "EGL: Failed to create context: %s",
                        getEGLErrorString(eglGetError()));
        return GLFW_FALSE;
    }

    // Set up attributes for surface creation
    index = 0;

    if (fbconfig->sRGB)
    {
        if (_glfw.egl.KHR_gl_colorspace)
            setAttrib(EGL_GL_COLORSPACE_KHR, EGL_GL_COLORSPACE_SRGB_KHR);
    }

    if (!fbconfig->doublebuffer)
        setAttrib(EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER);

    if (_glfw.egl.EXT_present_opaque)
        setAttrib(EGL_PRESENT_OPAQUE_EXT, !fbconfig->transparent);

    setAttrib(EGL_NONE, EGL_NONE);

    window->context.egl.surface =
        eglCreateWindowSurface(_glfw.egl.display,
                               config,
                               _GLFW_EGL_NATIVE_WINDOW,
                               attribs);
    if (window->context.egl.surface == EGL_NO_SURFACE)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "EGL: Failed to create window surface: %s",
                        getEGLErrorString(eglGetError()));
        return GLFW_FALSE;
    }

    window->context.egl.config = config;

    // Load the appropriate client library
    if (!_glfw.egl.KHR_get_all_proc_addresses)
    {
        int i;
        const char** sonames;
        const char* es1sonames[] =
        {
#if defined(_GLFW_GLESV1_LIBRARY)
            _GLFW_GLESV1_LIBRARY,
#elif defined(_GLFW_WIN32)
            "GLESv1_CM.dll",
            "libGLES_CM.dll",
#elif defined(_GLFW_COCOA)
            "libGLESv1_CM.dylib",
#elif defined(__OpenBSD__) || defined(__NetBSD__)
            "libGLESv1_CM.so",
#else
            "libGLESv1_CM.so.1",
            "libGLES_CM.so.1",
#endif
            NULL
        };
        const char* es2sonames[] =
        {
#if defined(_GLFW_GLESV2_LIBRARY)
            _GLFW_GLESV2_LIBRARY,
#elif defined(_GLFW_WIN32)
            "GLESv2.dll",
            "libGLESv2.dll",
#elif defined(_GLFW_COCOA)
            "libGLESv2.dylib",
#elif defined(__CYGWIN__)
            "libGLESv2-2.so",
#elif defined(__OpenBSD__) || defined(__NetBSD__)
            "libGLESv2.so",
#else
            "libGLESv2.so.2",
#endif
            NULL
        };
        const char* glsonames[] =
        {
#if defined(_GLFW_OPENGL_LIBRARY)
            _GLFW_OPENGL_LIBRARY,
#elif defined(_GLFW_WIN32)
#elif defined(_GLFW_COCOA)
#elif defined(__OpenBSD__) || defined(__NetBSD__)
            "libGL.so",
#else
            "libGL.so.1",
#endif
            NULL
        };

        if (ctxconfig->client == GLFW_OPENGL_ES_API)
        {
            if (ctxconfig->major == 1)
                sonames = es1sonames;
            else
                sonames = es2sonames;
        }
        else
            sonames = glsonames;

        for (i = 0;  sonames[i];  i++)
        {
            // HACK: Match presence of lib prefix to increase chance of finding
            //       a matching pair in the jungle that is Win32 EGL/GLES
            if (_glfw.egl.prefix != (strncmp(sonames[i], "lib", 3) == 0))
                continue;

            window->context.egl.client = _glfw_dlopen(sonames[i]);
            if (window->context.egl.client)
                break;
        }

        if (!window->context.egl.client)
        {
            _glfwInputError(GLFW_API_UNAVAILABLE,
                            "EGL: Failed to load client library");
            return GLFW_FALSE;
        }
    }

    window->context.makeCurrent = makeContextCurrentEGL;
    window->context.swapBuffers = swapBuffersEGL;
    window->context.swapInterval = swapIntervalEGL;
    window->context.extensionSupported = extensionSupportedEGL;
    window->context.getProcAddress = getProcAddressEGL;
    window->context.destroy = destroyContextEGL;

    return GLFW_TRUE;
}

#undef setAttrib

// Returns the Visual and depth of the chosen EGLConfig
//
#if defined(_GLFW_X11)
GLFWbool _glfwChooseVisualEGL(const _GLFWwndconfig* wndconfig,
                              const _GLFWctxconfig* ctxconfig,
                              const _GLFWfbconfig* fbconfig,
                              Visual** visual, int* depth)
{
    XVisualInfo* result;
    XVisualInfo desired;
    EGLConfig native;
    EGLint visualID = 0, count = 0;
    const long vimask = VisualScreenMask | VisualIDMask;

    if (!chooseEGLConfig(ctxconfig, fbconfig, &native))
    {
        _glfwInputError(GLFW_FORMAT_UNAVAILABLE,
                        "EGL: Failed to find a suitable EGLConfig");
        return GLFW_FALSE;
    }

    eglGetConfigAttrib(_glfw.egl.display, native,
                       EGL_NATIVE_VISUAL_ID, &visualID);

    desired.screen = _glfw.x11.screen;
    desired.visualid = visualID;

    result = XGetVisualInfo(_glfw.x11.display, vimask, &desired, &count);
    if (!result)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "EGL: Failed to retrieve Visual for EGLConfig");
        return GLFW_FALSE;
    }

    *visual = result->visual;
    *depth = result->depth;

    XFree(result);
    return GLFW_TRUE;
}
#endif // _GLFW_X11


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI EGLDisplay glfwGetEGLDisplay(void)
{
    _GLFW_REQUIRE_INIT_OR_RETURN(EGL_NO_DISPLAY);
    return _glfw.egl.display;
}

GLFWAPI EGLContext glfwGetEGLContext(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(EGL_NO_CONTEXT);

    if (window->context.source != GLFW_EGL_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return EGL_NO_CONTEXT;
    }

    return window->context.egl.handle;
}

GLFWAPI EGLSurface glfwGetEGLSurface(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(EGL_NO_SURFACE);

    if (window->context.source != GLFW_EGL_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return EGL_NO_SURFACE;
    }

    return window->context.egl.surface;
}

//========================================================================
// GLFW 3.3 OSMesa - www.glfw.org
// GLFW 3.3 - www.glfw.org - osmesa.c
//------------------------------------------------------------------------
// Copyright (c) 2016 Google Inc.
// Copyright (c) 2016-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// Please use C89 style variable declarations in this file because VS 2010
//========================================================================


static void makeContextCurrentOSMesa(_GLFWwindow* window)
{
    if (window)
    {
        int width, height;
        _glfwPlatformGetFramebufferSize(window, &width, &height);

        // Check to see if we need to allocate a new buffer
        if ((window->context.osmesa.buffer == NULL) ||
            (width != window->context.osmesa.width) ||
            (height != window->context.osmesa.height))
        {
            free(window->context.osmesa.buffer);

            // Allocate the new buffer (width * height * 8-bit RGBA)
            window->context.osmesa.buffer = calloc(4, (size_t) width * height);
            window->context.osmesa.width  = width;
            window->context.osmesa.height = height;
        }

        if (!OSMesaMakeCurrent(window->context.osmesa.handle,
                               window->context.osmesa.buffer,
                               GL_UNSIGNED_BYTE,
                               width, height))
        {
            _glfwInputError(GLFW_PLATFORM_ERROR,
                            "OSMesa: Failed to make context current");
            return;
        }
    }

    _glfwPlatformSetTls(&_glfw.contextSlot, window);
}

static GLFWglproc getProcAddressOSMesa(const char* procname)
{
    return (GLFWglproc) OSMesaGetProcAddress(procname);
}

static void destroyContextOSMesa(_GLFWwindow* window)
{
    if (window->context.osmesa.handle)
    {
        OSMesaDestroyContext(window->context.osmesa.handle);
        window->context.osmesa.handle = NULL;
    }

    if (window->context.osmesa.buffer)
    {
        free(window->context.osmesa.buffer);
        window->context.osmesa.width = 0;
        window->context.osmesa.height = 0;
    }
}

static void swapBuffersOSMesa(_GLFWwindow* window)
{
    // No double buffering on OSMesa
}

static void swapIntervalOSMesa(int interval)
{
    // No swap interval on OSMesa
}

static int extensionSupportedOSMesa(const char* extension)
{
    // OSMesa does not have extensions
    return GLFW_FALSE;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

GLFWbool _glfwInitOSMesa(void)
{
    int i;
    const char* sonames[] =
    {
#if defined(_GLFW_OSMESA_LIBRARY)
        _GLFW_OSMESA_LIBRARY,
#elif defined(_WIN32)
        "libOSMesa.dll",
        "OSMesa.dll",
#elif defined(__APPLE__)
        "libOSMesa.8.dylib",
#elif defined(__CYGWIN__)
        "libOSMesa-8.so",
#elif defined(__OpenBSD__) || defined(__NetBSD__)
        "libOSMesa.so",
#else
        "libOSMesa.so.8",
        "libOSMesa.so.6",
#endif
        NULL
    };

    if (_glfw.osmesa.handle)
        return GLFW_TRUE;

    for (i = 0;  sonames[i];  i++)
    {
        _glfw.osmesa.handle = _glfw_dlopen(sonames[i]);
        if (_glfw.osmesa.handle)
            break;
    }

    if (!_glfw.osmesa.handle)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE, "OSMesa: Library not found");
        return GLFW_FALSE;
    }

    _glfw.osmesa.CreateContextExt = (PFN_OSMesaCreateContextExt)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaCreateContextExt");
    _glfw.osmesa.CreateContextAttribs = (PFN_OSMesaCreateContextAttribs)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaCreateContextAttribs");
    _glfw.osmesa.DestroyContext = (PFN_OSMesaDestroyContext)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaDestroyContext");
    _glfw.osmesa.MakeCurrent = (PFN_OSMesaMakeCurrent)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaMakeCurrent");
    _glfw.osmesa.GetColorBuffer = (PFN_OSMesaGetColorBuffer)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetColorBuffer");
    _glfw.osmesa.GetDepthBuffer = (PFN_OSMesaGetDepthBuffer)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetDepthBuffer");
    _glfw.osmesa.GetProcAddress = (PFN_OSMesaGetProcAddress)
        _glfw_dlsym(_glfw.osmesa.handle, "OSMesaGetProcAddress");

    if (!_glfw.osmesa.CreateContextExt ||
        !_glfw.osmesa.DestroyContext ||
        !_glfw.osmesa.MakeCurrent ||
        !_glfw.osmesa.GetColorBuffer ||
        !_glfw.osmesa.GetDepthBuffer ||
        !_glfw.osmesa.GetProcAddress)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "OSMesa: Failed to load required entry points");

        _glfwTerminateOSMesa();
        return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

void _glfwTerminateOSMesa(void)
{
    if (_glfw.osmesa.handle)
    {
        _glfw_dlclose(_glfw.osmesa.handle);
        _glfw.osmesa.handle = NULL;
    }
}

#define setAttrib(a, v) \
{ \
    assert(((size_t) index + 1) < sizeof(attribs) / sizeof(attribs[0])); \
    attribs[index++] = a; \
    attribs[index++] = v; \
}

GLFWbool _glfwCreateContextOSMesa(_GLFWwindow* window,
                                  const _GLFWctxconfig* ctxconfig,
                                  const _GLFWfbconfig* fbconfig)
{
    OSMesaContext share = NULL;
    const int accumBits = fbconfig->accumRedBits +
                          fbconfig->accumGreenBits +
                          fbconfig->accumBlueBits +
                          fbconfig->accumAlphaBits;

    if (ctxconfig->client == GLFW_OPENGL_ES_API)
    {
        _glfwInputError(GLFW_API_UNAVAILABLE,
                        "OSMesa: OpenGL ES is not available on OSMesa");
        return GLFW_FALSE;
    }

    if (ctxconfig->share)
        share = ctxconfig->share->context.osmesa.handle;

    if (OSMesaCreateContextAttribs)
    {
        int index = 0, attribs[40];

        setAttrib(OSMESA_FORMAT, OSMESA_RGBA);
        setAttrib(OSMESA_DEPTH_BITS, fbconfig->depthBits);
        setAttrib(OSMESA_STENCIL_BITS, fbconfig->stencilBits);
        setAttrib(OSMESA_ACCUM_BITS, accumBits);

        if (ctxconfig->profile == GLFW_OPENGL_CORE_PROFILE)
        {
            setAttrib(OSMESA_PROFILE, OSMESA_CORE_PROFILE);
        }
        else if (ctxconfig->profile == GLFW_OPENGL_COMPAT_PROFILE)
        {
            setAttrib(OSMESA_PROFILE, OSMESA_COMPAT_PROFILE);
        }

        if (ctxconfig->major != 1 || ctxconfig->minor != 0)
        {
            setAttrib(OSMESA_CONTEXT_MAJOR_VERSION, ctxconfig->major);
            setAttrib(OSMESA_CONTEXT_MINOR_VERSION, ctxconfig->minor);
        }

        if (ctxconfig->forward)
        {
            _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                            "OSMesa: Forward-compatible contexts not supported");
            return GLFW_FALSE;
        }

        setAttrib(0, 0);

        window->context.osmesa.handle =
            OSMesaCreateContextAttribs(attribs, share);
    }
    else
    {
        if (ctxconfig->profile)
        {
            _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                            "OSMesa: OpenGL profiles unavailable");
            return GLFW_FALSE;
        }

        window->context.osmesa.handle =
            OSMesaCreateContextExt(OSMESA_RGBA,
                                   fbconfig->depthBits,
                                   fbconfig->stencilBits,
                                   accumBits,
                                   share);
    }

    if (window->context.osmesa.handle == NULL)
    {
        _glfwInputError(GLFW_VERSION_UNAVAILABLE,
                        "OSMesa: Failed to create context");
        return GLFW_FALSE;
    }

    window->context.makeCurrent = makeContextCurrentOSMesa;
    window->context.swapBuffers = swapBuffersOSMesa;
    window->context.swapInterval = swapIntervalOSMesa;
    window->context.extensionSupported = extensionSupportedOSMesa;
    window->context.getProcAddress = getProcAddressOSMesa;
    window->context.destroy = destroyContextOSMesa;

    return GLFW_TRUE;
}

#undef setAttrib


//////////////////////////////////////////////////////////////////////////
//////                        GLFW native API                       //////
//////////////////////////////////////////////////////////////////////////

GLFWAPI int glfwGetOSMesaColorBuffer(GLFWwindow* handle, int* width,
                                     int* height, int* format, void** buffer)
{
    void* mesaBuffer;
    GLint mesaWidth, mesaHeight, mesaFormat;
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    if (window->context.source != GLFW_OSMESA_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return GLFW_FALSE;
    }

    if (!OSMesaGetColorBuffer(window->context.osmesa.handle,
                              &mesaWidth, &mesaHeight,
                              &mesaFormat, &mesaBuffer))
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "OSMesa: Failed to retrieve color buffer");
        return GLFW_FALSE;
    }

    if (width)
        *width = mesaWidth;
    if (height)
        *height = mesaHeight;
    if (format)
        *format = mesaFormat;
    if (buffer)
        *buffer = mesaBuffer;

    return GLFW_TRUE;
}

GLFWAPI int glfwGetOSMesaDepthBuffer(GLFWwindow* handle,
                                     int* width, int* height,
                                     int* bytesPerValue,
                                     void** buffer)
{
    void* mesaBuffer;
    GLint mesaWidth, mesaHeight, mesaBytes;
    _GLFWwindow* window = (_GLFWwindow*) handle;
    assert(window != NULL);

    _GLFW_REQUIRE_INIT_OR_RETURN(GLFW_FALSE);

    if (window->context.source != GLFW_OSMESA_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return GLFW_FALSE;
    }

    if (!OSMesaGetDepthBuffer(window->context.osmesa.handle,
                              &mesaWidth, &mesaHeight,
                              &mesaBytes, &mesaBuffer))
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "OSMesa: Failed to retrieve depth buffer");
        return GLFW_FALSE;
    }

    if (width)
        *width = mesaWidth;
    if (height)
        *height = mesaHeight;
    if (bytesPerValue)
        *bytesPerValue = mesaBytes;
    if (buffer)
        *buffer = mesaBuffer;

    return GLFW_TRUE;
}

GLFWAPI OSMesaContext glfwGetOSMesaContext(GLFWwindow* handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;
    _GLFW_REQUIRE_INIT_OR_RETURN(NULL);

    if (window->context.source != GLFW_OSMESA_CONTEXT_API)
    {
        _glfwInputError(GLFW_NO_WINDOW_CONTEXT, NULL);
        return NULL;
    }

    return window->context.osmesa.handle;
}

//========================================================================
// GLFW 3.3 Linux - www.glfw.org
// GLFW 3.3 - www.glfw.org - linux_joystick.c
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2017 Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
// It is fine to use C99 in this file because it will not be built with VS
//========================================================================


#ifndef SYN_DROPPED // < v2.6.39 kernel headers
// Workaround for CentOS-6, which is supported till 2020-11-30, but still on v2.6.32
#define SYN_DROPPED 3
#endif

// Apply an EV_KEY event to the specified joystick
//
static void handleKeyEvent(_GLFWjoystick* js, int code, int value)
{
    _glfwInputJoystickButton(js,
                             js->linjs.keyMap[code - BTN_MISC],
                             value ? GLFW_PRESS : GLFW_RELEASE);
}

// Apply an EV_ABS event to the specified joystick
//
static void handleAbsEvent(_GLFWjoystick* js, int code, int value)
{
    const int index = js->linjs.absMap[code];

    if (code >= ABS_HAT0X && code <= ABS_HAT3Y)
    {
        static const char stateMap[3][3] =
        {
            { GLFW_HAT_CENTERED, GLFW_HAT_UP,       GLFW_HAT_DOWN },
            { GLFW_HAT_LEFT,     GLFW_HAT_LEFT_UP,  GLFW_HAT_LEFT_DOWN },
            { GLFW_HAT_RIGHT,    GLFW_HAT_RIGHT_UP, GLFW_HAT_RIGHT_DOWN },
        };

        const int hat = (code - ABS_HAT0X) / 2;
        const int axis = (code - ABS_HAT0X) % 2;
        int* state = js->linjs.hats[hat];

        // NOTE: Looking at several input drivers, it seems all hat events use
        //       -1 for left / up, 0 for centered and 1 for right / down
        if (value == 0)
            state[axis] = 0;
        else if (value < 0)
            state[axis] = 1;
        else if (value > 0)
            state[axis] = 2;

        _glfwInputJoystickHat(js, index, stateMap[state[0]][state[1]]);
    }
    else
    {
        const struct input_absinfo* info = &js->linjs.absInfo[code];
        float normalized = value;

        const int range = info->maximum - info->minimum;
        if (range)
        {
            // Normalize to 0.0 -> 1.0
            normalized = (normalized - info->minimum) / range;
            // Normalize to -1.0 -> 1.0
            normalized = normalized * 2.0f - 1.0f;
        }

        _glfwInputJoystickAxis(js, index, normalized);
    }
}

// Poll state of absolute axes
//
static void pollAbsState(_GLFWjoystick* js)
{
    for (int code = 0;  code < ABS_CNT;  code++)
    {
        if (js->linjs.absMap[code] < 0)
            continue;

        struct input_absinfo* info = &js->linjs.absInfo[code];

        if (ioctl(js->linjs.fd, EVIOCGABS(code), info) < 0)
            continue;

        handleAbsEvent(js, code, info->value);
    }
}

#define isBitSet(bit, arr) (arr[(bit) / 8] & (1 << ((bit) % 8)))

// Attempt to open the specified joystick device
//
static GLFWbool openJoystickDevice(const char* path)
{
    for (int jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        if (!_glfw.joysticks[jid].connected)
            continue;
        if (strcmp(_glfw.joysticks[jid].linjs.path, path) == 0)
            return GLFW_FALSE;
    }

    _GLFWjoystickLinux linjs = {0};
    linjs.fd = open(path, O_RDONLY | O_NONBLOCK);
    if (linjs.fd == -1)
        return GLFW_FALSE;

    char evBits[(EV_CNT + 7) / 8] = {0};
    char keyBits[(KEY_CNT + 7) / 8] = {0};
    char absBits[(ABS_CNT + 7) / 8] = {0};
    struct input_id id;

    if (ioctl(linjs.fd, EVIOCGBIT(0, sizeof(evBits)), evBits) < 0 ||
        ioctl(linjs.fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits) < 0 ||
        ioctl(linjs.fd, EVIOCGBIT(EV_ABS, sizeof(absBits)), absBits) < 0 ||
        ioctl(linjs.fd, EVIOCGID, &id) < 0)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR,
                        "Linux: Failed to query input device: %s",
                        strerror(errno));
        close(linjs.fd);
        return GLFW_FALSE;
    }

    // Ensure this device supports the events expected of a joystick
    if (!isBitSet(EV_KEY, evBits) || !isBitSet(EV_ABS, evBits))
    {
        close(linjs.fd);
        return GLFW_FALSE;
    }

    char name[256] = "";

    if (ioctl(linjs.fd, EVIOCGNAME(sizeof(name)), name) < 0)
        strncpy(name, "Unknown", sizeof(name));

    char guid[33] = "";

    // Generate a joystick GUID that matches the SDL 2.0.5+ one
    if (id.vendor && id.product && id.version)
    {
        sprintf(guid, "%02x%02x0000%02x%02x0000%02x%02x0000%02x%02x0000",
                id.bustype & 0xff, id.bustype >> 8,
                id.vendor & 0xff,  id.vendor >> 8,
                id.product & 0xff, id.product >> 8,
                id.version & 0xff, id.version >> 8);
    }
    else
    {
        sprintf(guid, "%02x%02x0000%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x00",
                id.bustype & 0xff, id.bustype >> 8,
                name[0], name[1], name[2], name[3],
                name[4], name[5], name[6], name[7],
                name[8], name[9], name[10]);
    }

    int axisCount = 0, buttonCount = 0, hatCount = 0;

    for (int code = BTN_MISC;  code < KEY_CNT;  code++)
    {
        if (!isBitSet(code, keyBits))
            continue;

        linjs.keyMap[code - BTN_MISC] = buttonCount;
        buttonCount++;
    }

    for (int code = 0;  code < ABS_CNT;  code++)
    {
        linjs.absMap[code] = -1;
        if (!isBitSet(code, absBits))
            continue;

        if (code >= ABS_HAT0X && code <= ABS_HAT3Y)
        {
            linjs.absMap[code] = hatCount;
            hatCount++;
            // Skip the Y axis
            code++;
        }
        else
        {
            if (ioctl(linjs.fd, EVIOCGABS(code), &linjs.absInfo[code]) < 0)
                continue;

            linjs.absMap[code] = axisCount;
            axisCount++;
        }
    }

    _GLFWjoystick* js =
        _glfwAllocJoystick(name, guid, axisCount, buttonCount, hatCount);
    if (!js)
    {
        close(linjs.fd);
        return GLFW_FALSE;
    }

    strncpy(linjs.path, path, sizeof(linjs.path) - 1);
    memcpy(&js->linjs, &linjs, sizeof(linjs));

    pollAbsState(js);

    _glfwInputJoystick(js, GLFW_CONNECTED);
    return GLFW_TRUE;
}

#undef isBitSet

// Frees all resources associated with the specified joystick
//
static void closeJoystick(_GLFWjoystick* js)
{
    _glfwInputJoystick(js, GLFW_DISCONNECTED);
    close(js->linjs.fd);
    _glfwFreeJoystick(js);
}

// Lexically compare joysticks by name; used by qsort
//
static int compareJoysticks(const void* fp, const void* sp)
{
    const _GLFWjoystick* fj = fp;
    const _GLFWjoystick* sj = sp;
    return strcmp(fj->linjs.path, sj->linjs.path);
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

// Initialize joystick interface
//
GLFWbool _glfwInitJoysticksLinux(void)
{
    const char* dirname = "/dev/input";

    _glfw.linjs.inotify = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
    if (_glfw.linjs.inotify > 0)
    {
        // HACK: Register for IN_ATTRIB to get notified when udev is done
        //       This works well in practice but the true way is libudev

        _glfw.linjs.watch = inotify_add_watch(_glfw.linjs.inotify,
                                              dirname,
                                              IN_CREATE | IN_ATTRIB | IN_DELETE);
    }

    // Continue without device connection notifications if inotify fails

    if (regcomp(&_glfw.linjs.regex, "^event[0-9]\\+$", 0) != 0)
    {
        _glfwInputError(GLFW_PLATFORM_ERROR, "Linux: Failed to compile regex");
        return GLFW_FALSE;
    }

    int count = 0;

    DIR* dir = opendir(dirname);
    if (dir)
    {
        struct dirent* entry;

        while ((entry = readdir(dir)))
        {
            regmatch_t match;

            if (regexec(&_glfw.linjs.regex, entry->d_name, 1, &match, 0) != 0)
                continue;

            char path[PATH_MAX];

            snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

            if (openJoystickDevice(path))
                count++;
        }

        closedir(dir);
    }

    // Continue with no joysticks if enumeration fails

    qsort(_glfw.joysticks, count, sizeof(_GLFWjoystick), compareJoysticks);
    return GLFW_TRUE;
}

// Close all opened joystick handles
//
void _glfwTerminateJoysticksLinux(void)
{
    int jid;

    for (jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
    {
        _GLFWjoystick* js = _glfw.joysticks + jid;
        if (js->connected)
            closeJoystick(js);
    }

    regfree(&_glfw.linjs.regex);

    if (_glfw.linjs.inotify > 0)
    {
        if (_glfw.linjs.watch > 0)
            inotify_rm_watch(_glfw.linjs.inotify, _glfw.linjs.watch);

        close(_glfw.linjs.inotify);
    }
}

void _glfwDetectJoystickConnectionLinux(void)
{
    if (_glfw.linjs.inotify <= 0)
        return;

    ssize_t offset = 0;
    char buffer[16384];
    const ssize_t size = read(_glfw.linjs.inotify, buffer, sizeof(buffer));

    while (size > offset)
    {
        regmatch_t match;
        const struct inotify_event* e = (struct inotify_event*) (buffer + offset);

        offset += sizeof(struct inotify_event) + e->len;

        if (regexec(&_glfw.linjs.regex, e->name, 1, &match, 0) != 0)
            continue;

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "/dev/input/%s", e->name);

        if (e->mask & (IN_CREATE | IN_ATTRIB))
            openJoystickDevice(path);
        else if (e->mask & IN_DELETE)
        {
            for (int jid = 0;  jid <= GLFW_JOYSTICK_LAST;  jid++)
            {
                if (strcmp(_glfw.joysticks[jid].linjs.path, path) == 0)
                {
                    closeJoystick(_glfw.joysticks + jid);
                    break;
                }
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////

int _glfwPlatformPollJoystick(_GLFWjoystick* js, int mode)
{
    // Read all queued events (non-blocking)
    for (;;)
    {
        struct input_event e;

        errno = 0;
        if (read(js->linjs.fd, &e, sizeof(e)) < 0)
        {
            // Reset the joystick slot if the device was disconnected
            if (errno == ENODEV)
                closeJoystick(js);

            break;
        }

        if (e.type == EV_SYN)
        {
            if (e.code == SYN_DROPPED)
                _glfw.linjs.dropped = GLFW_TRUE;
            else if (e.code == SYN_REPORT)
            {
                _glfw.linjs.dropped = GLFW_FALSE;
                pollAbsState(js);
            }
        }

        if (_glfw.linjs.dropped)
            continue;

        if (e.type == EV_KEY)
            handleKeyEvent(js, e.code, e.value);
        else if (e.type == EV_ABS)
            handleAbsEvent(js, e.code, e.value);
    }

    return js->connected;
}

void _glfwPlatformUpdateGamepadGUID(char* guid)
{
}

#endif /* OS_LINUX */