# Example: Collector

## What does it do?

Connects to a gateway and writes market-by-price and trade summary to stdout.

Fields

| Column | Name                 | Type      | Comment            |
|:------:| -------------------- | --------- | ------------------ |
|      0 | Symbol               | String    |                    |
|      1 | Exchange Time        | Timestamp | UTC                |
|      2 | Receive Time         | Timestamp | UTC                |
|      3 | Ask Price 0          | Float     |                    |
|      4 | Ask Quantity 0       | Float     |                    |
|      5 | Bid Price 0          | Float     |                    |
|      6 | Bid Quantity 0       | Float     |                    |
|      7 | Ask Price 1          | Float     |                    |
|      8 | Ask Quantity 1       | Float     |                    |
|      9 | Bid Price 1          | Float     |                    |
|     10 | Bid Quantity 1       | Float     |                    |
|     11 | Ask Price 2          | Float     |                    |
|     12 | Ask Quantity 2       | Float     |                    |
|     13 | Bid Price 2          | Float     |                    |
|     14 | Bid Quantity 2       | Float     |                    |
|     15 | Ask Price 3          | Float     |                    |
|     16 | Ask Quantity 3       | Float     |                    |
|     17 | Bid Price 3          | Float     |                    |
|     18 | Bid Quantity 3       | Float     |                    |
|     19 | Ask Price 4          | Float     |                    |
|     20 | Ask Quantity 4       | Float     |                    |
|     21 | Bid Price 4          | Float     |                    |
|     22 | Bid Quantity 4       | Float     |                    |
|     23 | High Price           | Float     | Not available      |
|     24 | Last Price           | Float     |                    |
|     25 | Lower Limit Price    | Float     | Not available      |
|     26 | Upper Limit Price    | Float     | Not available      |
|     27 | Low Price            | Float     | Not available      |
|     28 | Open Interest        | Integer   | Not available      |
|     29 | Open Price           | Float     | Not available      |
|     30 | Pre-Close Price      | Float     | Not available      |
|     31 | Pre-Open Interest    | Integer   | Not available      |
|     32 | Pre-Settlement Price | Float     | Not available      |
|     33 | Turnover             | Float     |                    |
|     34 | Volume               | Integer   |                    |
|     35 | Update Name          | String    | USTP\_L2           |
|     36 | Update Type          | Integer   | 1                  |

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
