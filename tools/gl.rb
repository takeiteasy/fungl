#!/usr/bin/env ruby
=begin
gl.rb -- https://github.com/takeiteasy/fungl

The MIT License (MIT)

Copyright (c) 2022 George Watson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
=end

require "nokogiri"
require "net/http"

$EnableHeaderFileOut = true
$EnableSourceFileOut = true
$EnableSeperateFiles = true
$DisableGLLoaderOut = false
$EnableDebugWrapper = false

# Download required files if needed
xml = ["https://registry.khronos.org/EGL/api/KHR/khrplatform.h",
       "https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/xml/gl.xml"]
xml.each do |x|
  xp = "tools/" + File.basename(x)
  next if File.exist? xp
  resp = Net::HTTP.get_response URI(x)
  abort "ERROR: #{resp.code}" unless resp.is_a? Net::HTTPSuccess
  File.write xp, resp.body
end

# Parse gl.xml with Nokogiri
doc = Nokogiri::XML File.open("tools/gl.xml") do |config|
  config.options = Nokogiri::XML::ParseOptions::STRICT | Nokogiri::XML::ParseOptions::NOBLANKS
end

# Parse <types>
$GLhandleARB=nil
types = doc.xpath("//types").children.reject { |c| c.class.to_s == "Nokogiri::XML::Comment" }.map do |t|
  html = t.inner_html.gsub "<apientry></apientry>", "APIENTRY"
  /<name>(?<name>.*)<\/name>/ =~ html
  unless name
    $GLhandleARB = t.text unless t.text.start_with?("#include")
  end
  [name, t.text]
end.to_h.delete_if { |k, v| k.nil? }

# Parse <enums>
enums = doc.xpath("//enums").children.map { |e| [e.attr('name'), e.attr('value')] }.to_h
# Parse <commands>
commands = doc.xpath("//commands").children.map do |f|
  parts = {:params => []}
  name = nil
  f.children.each do |ff|
    case ff.to_s
    when /^<proto/
      fff = ff.children.to_a
      name = fff.pop.text
      parts[:result] = fff.map { |ffff| ffff.text.strip }.join " "
    when /^<param/
      parts[:params] << ff.children.to_a.map { |ffff| ffff.text.strip }.join(" ")
    end
  end
  parts[:params] = ["void"] if parts[:params].empty?
  [name, parts]
end.to_h

# INFO: Start of writing header

$OriginalOut = $>.clone

def RestoreOriginalOut
  $>.close
  $> = $OriginalOut
end

$> = File.open($EnableSeperateFiles ? "fungl/gl.h" : "fungl/gl_single.h", 'w') if $EnableHeaderFileOut

puts <<HEADER
/*

 This file was generated by https://github.com/takeiteasy/fungl/blob/master/tools/gl.rb

 The MIT License (MIT)

 Copyright (c) 2022 George Watson

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FUNGL_WRAPPER_HEADER
#define FUNGL_WRAPPER_HEADER
#ifdef __cplusplus
extern "C" {
#endif

#define FUNGL_POSIX
#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
#define FUNGL_MAC
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#define FUNGL_WINDOWS
#if !defined(FS_PLATFORM_FORCE_POSIX)
#undef FUNGL_POSIX
#endif
#elif defined(__gnu_linux__) || defined(__linux__) || defined(__unix__)
#define FUNGL_LINUX
#else
error "Unknown platform :("
#endif

#define __gl_glcorearb_h_ 1  /* Khronos core */
#define __gl_glext_h_ 1      /* Khronos compatibility */
#define __gl_h_ 1            /* Windows/Linux */
#define __GL_H__ 1           /* Windows */
#define __gl3_h_ 1           /* Mac */

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#define APIENTRY __stdcall
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#if defined(GLAD_GLAPI_EXPORT)
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(GLAD_GLAPI_EXPORT_BUILD)
#if defined(__GNUC__)
#define GLAPI __attribute__ ((dllexport)) extern
#else
#define GLAPI __declspec(dllexport) extern
#endif
#else
#if defined(__GNUC__)
#define GLAPI __attribute__ ((dllimport)) extern
#else
#define GLAPI __declspec(dllimport) extern
#endif
#endif
#elif defined(__GNUC__) && defined(GLAD_GLAPI_EXPORT_BUILD)
#define GLAPI __attribute__ ((visibility ("default"))) extern
#else
#define GLAPI extern
#endif
#else
#define GLAPI extern
#endif
#endif

#if !defined(EXPORT)
#if defined(FUNGL_WINDOWS)
#define EXPORT __declspec(dllexport)
#elif defined(FUNGL_EMSCRIPTEN)
#define EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define EXPORT
#endif
#endif

#if !defined(FUNGL_VERSION)
#define FUNGL_VERSION 1000
#endif
HEADER

# Find <feature>, remove any unused gl versions
features = doc.xpath("//feature").select { |f| f.attr('name') =~ /^GL_VERSION_\d_\d$/ }

# Print out version macros
features.each do |f|
  maj, min = f.attr('number').split('.')
  puts "#define #{f.attr 'name'} #{maj}0#{min}0"
end

# Parse and print khrplatform.h
khr = File.readlines("tools/khrplatform.h").map { |l| l.rstrip }[4..-2]
puts "\n/* khrplatform.h -- [https://registry.khronos.org/EGL/api/KHR/khrplatform.h]", khr.join("\n"), "/* end of khrplatform.h */", ""

# Format features for each OpenGL version
defined = []
$functions = {}
features.each do |f|
  ver = f.attr 'number'
  $functions[ver] = []
  output = []

  f.children.each do |ff|
    ff.children.each do |fff|
      name = fff.attr("name")
      if fff.to_s =~ /^<command/
        commands[name][:params].each do |p|
          t = p.split(" ").select! { |pp| pp =~ /GL/ }
          unless t.nil?
            t.each do |tt|
              unless defined.include? tt
                puts types[tt]
                types.delete(tt)
                defined << tt
              end
            end
          end
        end
      end
    end
  end

  f.children.each do |ff|
    ff.children.each do |fff|
      name = fff.attr("name")
      next if defined.include? name
      case fff.to_s
      when /^<type/
        output.append(types[name])
      when /^<enum/
        puts "#define #{name} #{enums[name]}"
      when /^<command/
        proc = "PFN#{name.upcase}PROC"
        $functions[ver].append [proc, name]
      end
      defined << name
    end
  end

  unless output.empty?
    puts "#if FUNGL_VERSION >= #{f.attr 'name'}"
    puts output.join("\n")
    puts "#endif"
  end
end

puts "", "typedef khronos_intptr_t GLintptr;"
types.each do |k, v|
  if v.start_with?("typedef")
    puts v unless v.start_with?("typedef void (")
  end
end
puts $GLhandleARB, "" if $GLhandleARB

# Define OpenGL functions
features.each do |f|
  puts "#if FUNGL_VERSION >= #{f.attr 'name'}"
  f.children.each do |ff|
    ff.children.each do |fff|
      name = fff.attr("name")
      next unless defined.include? name
      if fff.to_s =~ /^<command/
        proc = "PFN#{name.upcase}PROC"
        puts "typedef #{commands[name][:result]} (APIENTRYP #{proc})(#{commands[name][:params].join ', '});"
        puts "#define #{name} __#{name}"
      end
      defined << name
    end
  end
  puts "#endif", ""
end

# Store functions in macros for later
def PrintGLVersionsMacro
  $functions.each do |k, v|
    puts "", "#define GL_FUNCTIONS_#{k.gsub '.', '_'} \\"
    v.each do |vv|
      puts "\tX(#{vv[0]}, #{vv[1]}) \\"
    end
  end
  puts
end

# Define extern functions
PrintGLVersionsMacro()
puts "#define X(T, N) extern T __##N;"
$functions.each do |k, v|
  maj, min = k.split '.'
  puts "#if FUNGL_VERSION >= GL_VERSION_#{maj}_#{min}"
  puts "GL_FUNCTIONS_#{maj}_#{min}"
  puts "#endif"
end
puts "#undef X", ""

puts <<FOOTER
EXPORT int glInit(void);

#ifdef __cplusplus
}
#endif
#endif // FUNGL_WRAPPER_HEADER
FOOTER

# INFO: Start of writing source

if not $EnableSourceFileOut
  RestoreOriginalOut() if $EnableHeaderFileOut
  exit 0
end

if $EnableSeperateFiles
  $> = File.open("fungl/gl.c", "w")
  puts <<SOURCE
/*

 This file was generated by https://github.com/takeiteasy/fungl/blob/master/tools/gl.rb

 The MIT License (MIT)

 Copyright (c) 2022 George Watson

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "gl.h"
#include <stdlib.h>

SOURCE
else
  puts "\n#if defined(FUNGL_IMPLEMENTATION)"
  puts "#include <stdlib.h>\n\n"
end

# Define extern implementations
puts "#define X(T, N) T __##N = (T)((void*)0);"
$functions.each do |k, v|
  maj, min = k.split '.'
  puts "#if FUNGL_VERSION >= GL_VERSION_#{maj}_#{min}"
  puts "GL_FUNCTIONS_#{maj}_#{min}"
  puts "#endif"
end
puts "#undef X", ""

PrintGLVersionsMacro() if $OutputGLBindings

unless $DisableGLLoaderOut
  puts <<LOADER
#if !defined(NULL)
#define NULL ((void*)0)
#endif

#if defined(FUNGL_WINDOWS) || defined(__CYGWIN__)
#ifndef _WINDOWS_
#undef APIENTRY
#endif
#include <windows.h>
typedef void* (APIENTRYP PFNWGLGETPROCADDRESSPROC_PRIVATE)(const char*);
static PFNWGLGETPROCADDRESSPROC_PRIVATE glLoaderGetProcAddressPtr;
static HMODULE libGL = NULL;

#ifdef _MSC_VER
#ifdef __has_include
#if __has_include(<winapifamily.h>)
#define HAVE_WINAPIFAMILY 1
#endif
#elif _MSC_VER >= 1700 && !_USING_V110_SDK71_
#define HAVE_WINAPIFAMILY 1
#endif
#endif

#ifdef HAVE_WINAPIFAMILY
#include <winapifamily.h>
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#define IS_UWP 1
#endif
#endif

static int LoadGLLibrary(void) {
#ifndef IS_UWP
    if ((libGL = LoadLibraryW(L"opengl32.dll"))) {
        void(*tmp)(void);
        tmp = (void(*)(void)) GetProcAddress(libGL, "wglGetProcAddress");
        glLoaderGetProcAddressPtr = (PFNWGLGETPROCADDRESSPROC_PRIVATE)tmp;
        return glLoaderGetProcAddressPtr != NULL;
    }
#endif
    return 0;
}

static void CloseGLLibrary(void) {
    if (libGL) {
        FreeLibrary((HMODULE)libGL);
        libGL = NULL;
    }
}
#else
#include <dlfcn.h>
#if !defined(FUNGL_MAC) && !defined(__HAIKU__)
typedef void* (APIENTRYP PFNGLXGETPROCADDRESSPROC_PRIVATE)(const char*);
static PFNGLXGETPROCADDRESSPROC_PRIVATE glLoaderGetProcAddressPtr;
#endif
static void* libGL = NULL;

static int LoadGLLibrary(void) {
#if defined(FUNGL_MAC)
    static const char *NAMES[] = {
        "../Frameworks/OpenGL.framework/OpenGL",
        "/Library/Frameworks/OpenGL.framework/OpenGL",
        "/System/Library/Frameworks/OpenGL.framework/OpenGL",
        "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL"
    };
#else
    static const char *NAMES[] = {"libGL.so.1", "libGL.so"};
#endif

    unsigned int index = 0;
    for (index = 0; index < (sizeof(NAMES) / sizeof(NAMES[0])); index++) {
        if ((libGL = dlopen(NAMES[index], RTLD_NOW | RTLD_GLOBAL))) {
#if defined(FUNGL_MAC) || defined(__HAIKU__)
            return 1;
#else
            glLoaderGetProcAddressPtr = (PFNGLXGETPROCADDRESSPROC_PRIVATE)dlsym(libGL,
                "glXGetProcAddressARB");
            return glLoaderGetProcAddressPtr != NULL;
#endif
        }
    }
    return 0;
}

static void CloseGLLibrary(void) {
    if (libGL) {
        dlclose(libGL);
        libGL = NULL;
    }
}
#endif

static void* LoadGLProc(const char *namez) {
    void* result = NULL;
    if (libGL == NULL)
        return NULL;

#if !defined(FUNGL_MAC) && !defined(__HAIKU__)
    if (glLoaderGetProcAddressPtr)
        result = glLoaderGetProcAddressPtr(namez);
#endif
    if (!result) {
#if defined(FUNGL_WINDOWS) || defined(__CYGWIN__)
        result = (void*)GetProcAddress((HMODULE)libGL, namez);
#else
        result = dlsym(libGL, namez);
#endif
    }

    return result;
}

#define X(T, N) \\
    if (!(__##N = (T)LoadGLProc(#N))) \\
        failures++;

int glInit(void) {
    int failures = 0;
    if (LoadGLLibrary()) {
LOADER

  $functions.each do |k, v|
    maj, min = k.split '.'
    puts "#if FUNGL_VERSION >= GL_VERSION_#{maj}_#{min}"
    puts "GL_FUNCTIONS_#{maj}_#{min}"
    puts "#endif"
  end

  puts <<LOADER_FOOTER
        CloseGLLibrary();
    }
    return failures;
}
#undef X
LOADER_FOOTER
end

puts "#endif // FUNGL_IMPLEMENTATION" if not $EnableSeperateFiles

RestoreOriginalOut() if $EnableSeperateFiles or $EnableHeaderFileOut
