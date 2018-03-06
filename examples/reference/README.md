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

	libtool --mode=execute gdb --args reference-strategy --config-file strategy.conf --mode simulation --simulation-file USTP_Multi_20180223.csv
	libtool --mode=execute gdb --args reference-strategy --config-file strategy.conf --mode trading --local-address /var/tmp/femasapi.sock

This [link](https://www.gnu.org/software/libtool/manual/html_node/Debugging-executables.html)
is a good place to get started with debugging.

## Conda

The strategy is distributed with the `quinclas-tradingapi` package.

  conda install quinclas-tradingapi

You will then find the config file here: `$CONDA_PREFIX/share/quinclas/examples/strategy.conf`

## Simulation Files

The format is controlled by the *last* column (an integer).

### Level 1

| Column | Name              | Type      | Comment            |
|:------:| ----------------- | --------- | ------------------ |
|      0 | Symbol            | String    |                    |
|      1 | Exchange Time     | Timestamp | %Y%m%d %H:%M::%E3S |
|      2 | Receive Time      | Timestamp | %Y%m%d %H:%M::%E9S |
|      3 | Best Ask Price    | Float     |                    |
|      4 | Best Ask Quantity | Float     |                    |
|      5 | Best Bid Price    | Float     |                    |
|      6 | Best Bid Quantity | Float     |                    |
|      7 | ?                 | Float     | Price              |
|      8 | ? Last Price ?    | Float     | Price              |
|      9 | ?                 | Float     | Price              |
|     10 | ?                 | Float     | Price              |
|     11 | ?                 | Float     | Price              |
|     12 | ? SeqNo ?         | Integer   | Seqno?             |
|     13 | ?                 | Float     | Price              |
|     14 | ?                 | Float     | Price              |
|     15 | ?                 | Float     | ???                |
|     16 | ?                 | Float     | Price              |
|     17 | ? Turnover ?      | Float     | Turnover?          |
|     18 | ? SeqNo ?         | Integer   | SeqNo?             |
|     19 | Update Name       | String    | USTP\_L1           |
|     20 | Update Type       | Integer   | 0                  |

### Level 2

| Column | Name              | Type      | Comment            |
|:------:| ----------------- | --------- | ------------------ |
|      0 | Symbol            | String    |                    |
|      1 | Exchange Time     | Timestamp | %Y%m%d %H:%M::%E3S |
|      2 | Receive Time      | Timestamp | %Y%m%d %H:%M::%E9S |
|      3 | Ask Price 0       | Float     |                    |
|      4 | Ask Quantity 0    | Float     |                    |
|      5 | Bid Price 0       | Float     |                    |
|      6 | Bid Quantity 0    | Float     |                    |
|      7 | Ask Price 1       | Float     |                    |
|      8 | Ask Quantity 1    | Float     |                    |
|      9 | Bid Price 1       | Float     |                    |
|     10 | Bid Quantity 1    | Float     |                    |
|     11 | Ask Price 2       | Float     |                    |
|     12 | Ask Quantity 2    | Float     |                    |
|     13 | Bid Price 2       | Float     |                    |
|     14 | Bid Quantity 2    | Float     |                    |
|     15 | Ask Price 3       | Float     |                    |
|     16 | Ask Quantity 3    | Float     |                    |
|     17 | Bid Price 3       | Float     |                    |
|     18 | Bid Quantity 3    | Float     |                    |
|     19 | Ask Price 4       | Float     |                    |
|     20 | Ask Quantity 4    | Float     |                    |
|     21 | Bid Price 4       | Float     |                    |
|     22 | Bid Quantity 4    | Float     |                    |
|     23 | ?                 | Float     |                    |
|     24 | ? Last Price ?    | Float     |                    |
|     25 | ?                 | Float     |                    |
|     26 | ?                 | Float     |                    |
|     27 | ?                 | Float     |                    |
|     28 | ? SeqNo ?         | Integer   |                    |
|     29 | ?                 | Float     |                    |
|     30 | ?                 | Float     |                    |
|     31 | ?                 | Float     |                    |
|     32 | ?                 | Float     |                    |
|     33 | ? Turnover ?      | Float     |                    |
|     34 | ? SeqNo ?         | Integer   |                    |
|     35 | Update Name       | String    | USTP\_L2           |
|     36 | Update Type       | Integer   | 1                  |

*It looks like column 23-34 match column 7-18 from L1 updates*.
