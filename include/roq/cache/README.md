# PLEASE READ THIS !!!

This directory contains utility classes useful for clients to manage a cache of
objects.
In particular, these classes are being used by the FIX and JSON bridges and are
considered general-purpose.

> Note! Everything here is about "convenience" and not necessarily meant for low latency.

Some effort has gone into reducing allocations by using heterogenous lookups
and increased cache locality.

> This directory will likely later be moved to a separate utility library.
