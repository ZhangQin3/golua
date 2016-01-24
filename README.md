Go Bindings for the lua C API
=========================

```
This is a fork from https://github.com/aarzilli/golua

The lua multi-thread support has been added to the repository, it can be used to extend lua multi-thread 
via golang's goroutine and there is a chennel function to transfer data betwen lua threads via send and 
recv functions. please refer to the other repository https://github.com/ZhangQin3/golua_standalone.



CONTRIBUTORS
---------------------

* Adam Fitzgerald (original author)
* Alessandro Arzilli
* Steve Donovan
* Harley Laue
* James Nurmi
* Ruitao
* Xushiwei
* Isaint
* hsinhoyeh
* Viktor Palmkvist
* HongZhen Peng
* Admin36
* Pierre Neidhardt (@Ambrevar)

SEE ALSO
---------------------

[Luar](https://github.com/stevedonovan/luar/) is a reflection layer on top of golua API providing a simplified way to publish go functions to a Lua VM.

Licensing
-------------
GoLua is released under the MIT license.
Please see the LICENSE file for more information.

Lua is Copyright (c) Lua.org, PUC-Rio.  All rights reserved.
