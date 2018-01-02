# Example: Gateway

## What does it do?

Currently no real functionality -- only showing the skeleton of a gateway application.

## How to use?

To start the gateway and bind it to a unix domain socket

	./example-gateway --local-address /var/tmp/xyz

## Enable logging?

You can redirect logs to stderr like this

	export GLOG_logtostderr=1

Then start the program as already described.

This [link](http://rpg.ifi.uzh.ch/docs/glog.html) is a good place to read about Google's Logging Library.

## How do I debug?

On Linux

	libtool --mode=execute gdb --args ./example-collector --local-address /var/tmp/femas

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.
