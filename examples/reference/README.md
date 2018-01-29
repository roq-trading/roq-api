# Example: Reference

## What does it do?

This is a reference implementation of a (mostly) realistic trading system.
The idea is to *demonstrate* how one could implement such a system.

Note!
The example is *not* optimal.
The design is only meant to demonstrate *some* features of real trading when using the Quinclas Trading API.

There are safer and more performant designs.
Please contact us for consultancy.

## Documentation

Please note, the documentation is intentionally kept to a minimum.

The hope is that by reading the actual code, one gets an idea of how to use the API.

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

## Conda

The strategy is distributed with the `quinclas-tradingapi` package.
You will find the config file here: `$CONDA_PREFIX/share/quinclas/examples/strategy.conf`
