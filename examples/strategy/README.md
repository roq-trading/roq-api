# Example: Strategy

## What does it do?

Logs every event received from the gateway.

Sends an order when

* Market data connection has been established and the user has been successfully logged in.
* Order management connection has been established and the user has been successfully logged in.

## How to use?

For example, to execute the strategy using the Femas API

	./example-strategy --local-address /var/tmp/femas

## Enable logging?

You can redirect logs to stderr like this

	export GLOG_logtostderr=1

Then start the program as already described.

This [link](http://rpg.ifi.uzh.ch/docs/glog.html) is a good place to read about Google's Logging Library.

## How do I debug?

On Linux

	libtool --mode=execute gdb --args ./example-strategy --local-address /var/tmp/femas

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.
