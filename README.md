# Strings

## Summary

Purpose is to build complete CMake environmenet project with the following spec:

* C language project.
* Target windows and linux.
* Use of CMake to achieve cross platform build.
* CUnit for unitary testing.
* Generation of both static and dynamic shared libraries.

## Commands

* **--lower** string1 [ string2 string3 ...]: put all strings in lowercase.
* **--upper** string1 [ string2 string3 ...]: put all strings in uppercase.
* **--rot13** string1 [ string2 string3 ...]: do a rot13 on strings parameters.
* **--load** <shared library> string1 [ string2 string3 ...]: load module dynamically and apply function
	** **libupper.so** does uppercase
	** **liblower.so** does lowercase
	** **librot13.so** does rot13
