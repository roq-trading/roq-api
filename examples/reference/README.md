# Example: Reference

## What does it do?

This is a reference implementation of a mostly realistic trading strategy.

Please note, the documentation is intentionally kept to a minimum.
Reading and understanding this example should direct the attention to the logic.

Please refer to the [strategy example](../strategy/README.md) for documented details about interfaces.

## How to use?

For example, to execute the strategy using the Femas API

	reference-strategy --config-file strategy.conf --local-address /var/tmp/femasapi.sock

## Enable logging?

You can redirect logs to stderr like this

	export GLOG_logtostderr=1

Then start the program as already described.

This [link](http://rpg.ifi.uzh.ch/docs/glog.html) is a good place to read about Google's Logging Library.

## How do I debug?

On Linux

	libtool --mode=execute gdb --args reference-strategy --config-file strategy.conf --local-address /var/tmp/femasapi.sock

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.
