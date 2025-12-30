# Inconsistencies I found when trying to switch to the Muon build system.

I have provided here a minimal POC for these inconsistencies. To run them just
run `build.sh` in this repository's root.

(Be sure to have both `meson` and `muon` installed)

## compiler.has_header()

### Testing the existence of header files.

Meson tries to test the availability of a given header file by using the
built-in compiler macro `__has_include`. Alongside being faster `__has_include`
*just* checks if the file is available to be included instead of actually
including it.
This means that none of the pre processor lines in the checked files are
parsed, and hence can't fail due to various reasons like missing
`#define`s, `#error`s, problems in deeper `#include`s and ... .

Here is the snippet that Meson uses to check the existence of a header.

```c
#ifdef __has_include
 #if !__has_include("legolas.h")
  #error "Header 'legolas.h' could not be found"
 #endif
#else
 #include <legolas.h>
#endif
```

Muon on the other hand, tries to fully `#include` the file.
This can fail in many scenarios where the build script writer relied on Meson's
behavior and has not passed in the proper configuration to make a full
`#include` work.

Here is the snippet that Muon uses to check the existence of a header.

```c

#include <legolas.h>
int main(void) {}
```

I believe Meson's behavior is correct in this case, because
`compiler.has_header()` should just be checking for the existence of a header
(i.e. if a given `compiler` `has` a `header`), and further, more through
checking of a header's inclusion, and proper function, should be left for the
`compiler.check_header()` method.

I skimmed through Muon's source and found out that the `compiler.has_header()`
method has been implemented by wrapping the compiler\_check\_header function
using it for `compiler.has_header()` too.
A dedicated "compiler\_has\_header" function can fix this issue.

## Context

I am amazed by Muon's speed and lack of dependencies compared to Meson, and
trying to write a FreeBSD ports "framework" for it, but I am currently facing
the above problems in some of the ports.

---

P.S FreeBSD is generally a good platform for finding out bugs related to
include and library paths in build systems, because 3rd party software is
installed into a separate prefix `/usr/local/`, while base (system) libraries
and software are installed in `/usr/`.

## Links:

*  https://gcc.gnu.org/onlinedocs/gcc-10.1.0/cpp/_005f_005fhas_005finclude.html
