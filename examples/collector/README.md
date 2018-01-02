# Example: Collector

## What does it do?

Connects to a gateway and writes top of book to stdout.

Fields

* Exchange time (UTC).
* Gateway receive time (UTC).
* Client receive time (UTC).
* Exchange name (if available).
* Instrument name.
* Best bid price.
* Quantity offered at best bid price.
* Best ask price.
* Quantity offered at best ask price.

## How to use?

For example, to collect market data from the Femas API

	example-collector --local-address /var/tmp/femas

## Enable logging?

You can redirect logs to stderr like this

	export GLOG_logtostderr=1

Then start the program as already described.

This [link](http://rpg.ifi.uzh.ch/docs/glog.html) is a good place to read about Google's Logging Library.

## How do I debug?

On Linux

	libtool --mode=execute gdb --args example-collector --local-address /var/tmp/femas

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.
