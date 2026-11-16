# Code Style
## Includes
Rollick has a specific include structure. To start, all includes MUST be relative paths from the root of the current module. For example, if you wanted to include `Kernel/Prekernel/Arch/Foo.h`, you would write
```cpp
#include "Prekernel/Arch/Foo.h"
```
This also includes the top include of Header/Source pairs. For example, you would write the above in the file `Kernel/Prekernel/Arch/Foo.cpp`

### Ordering
Includes should be ordered as the following: same directory, from root going alphabetically. Includes MUST be ordered in blocks. These blocks represent a single subdirectory of the module. Subdirectories MUST come before their parent directory, however they can be included in the same block as their parent. Includes from within the project MUST make use of quotes (""). Any includes from outside the project MUST always appear as the last block, sorted alphabetically. These includes MUST make use of angular brackets (<>)

As an example, let's say that the file Foo.cpp needs to include a set of headers. An example of how you might do this is as follows:
```cpp
#include "Prekernel/Arch/Foo.h"

#include "Prekernel/Arch/Bar.h"

#include "Arch/i386/Interrupts/IDT.h
#include "Arch/i386/Interrupts/PIC.h
#include "Arch/i386/Terminal.h
#include "Arch/i386/VGA.h

#include "Library/kStandardLib.h

#include <stdlib.h>
```