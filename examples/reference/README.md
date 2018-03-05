# Example: Reference Implementation

## What does it do?

This is a reference implementation of a (mostly) realistic trading system.

Note!
This solution is neither complete nor optimal.
The example is only meant to demonstrate how to use the Quinclas Trading API.

There are safer and more performant designs.
Please contact us for consultancy.

## How to use?

The strategy can be started in one of two modes: trading or simulation.

	reference-strategy --config-file strategy.conf --mode simulation --simulation-file USTP_Multi_20180223.csv
	reference-strategy --config-file strategy.conf --mode trading --local-address /var/tmp/femasapi.sock

## How do I debug?

On Linux

	libtool --mode=execute gdb --args reference-strategy --config-file strategy.conf --mode trading --local-address /var/tmp/femasapi.sock

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.

## Conda

The strategy is distributed with the `quinclas-tradingapi` package.

  conda install quinclas-tradingapi

You will then find the config file here: `$CONDA_PREFIX/share/quinclas/examples/strategy.conf`
