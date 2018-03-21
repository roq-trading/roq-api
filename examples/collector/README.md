# Example: Collector

## What does it do?

Connects to a gateway and writes top of book (L1) and trade summary to stdout.

Fields

* Exchange time (UTC).
* Receive time (UTC).
* Instrument name.
* Best bid price.
* Best bid quantity.
* Best ask price.
* Best ask quantity.
* Last traded price.
* Traded volume.
* Turnover.

## How to use?

For example, to collect market data from the Femas API

	./example-collector --mode simulation --simulation-file USTP_Multi_20180223.csv
	./example-collector --mode-trading --gateways femasapi=test:1234@/var/tmp/femasapi.sock

## How do I debug?

On Linux

	libtool --mode=execute gdb --args ./example-collector --mode simulation --simulation-file USTP_Multi_20180223.csv
	libtool --mode=execute gdb --args ./example-collector --mode-trading --gateways femasapi=test:1234@/var/tmp/femasapi.sock

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.
