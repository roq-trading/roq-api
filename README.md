# Roq Trading Solutions

*A C++ HFT Toolkit for Algo Traders*


## What is it?

This is the API used to communicate between your trading
strategies and other components (market gateways)
developed by Roq.


## Overview

Primary motivation

* Allows you to test without first having
  to sign a NDA
* An open sourced API giving anyone a free
  option to implement alternative solutions
* Easy access to closed source binaries as
  either Conda packages or Docker images

> Some of the solutions are free to use,
> e.g. `roq-client` and `roq-influxdb`.
>
> Other solutions may require you to enter
> a license agreement with Roq to enable more
> advanced features, e.g. `roq-coinbase-pro`
> and `roq-deribit`.
>
> All solutions are completely free to download.


## Build

The API is not very useful by itself.
You will need an implementation of the interface
such as the closed source `roq-client` library.

> You will not achieve much by compiling this project.

Head over to
[Roq Samples](https://github.com/roq-trading/roq-api)
and follow the instrutions there to install the
`roq-client` library and compile some simple examples.


## Benefits

* Ultra low latency (single digit microsecond response time)
* Very high message throughput (only bound by single-message processing time)
* Messages communicated to clients using a broadcast design (efficient publish-once implementation)
* Automatic connection and download management (helps you manage state)
* Message normalization (a single unified interface used to access all markets)
* Market data (L1, L2, trade summary, session/daily statistics, etc).
* Order management (create, modify, cancel, ack, update, trade)
* Positions and funds (when available)
* Automatically persist all messages to storage device (zero latency impact)
* Integrate well with popular network kernel-bypass solutions (having epoll support)
* Suitable for fully autonomous algorithmic trading (e.g. bot trading)

## Constraints

* C++17
* Linux (RHEL, CentOS, Debian, Ubuntu)
* Same-host deployment
* Shared memory for communication
* Micro-service design
* Busy polling causing 100% CPU usage (to achieve ultra low latency)
* CPU isolation, disable hyperthreading, use thread affinity, etc.
  are strongly recommended practices (and therefore not suitable for VM deployment)
* Enough CPU cores to support your use-case


## Solutions

Gateways

* [`roq-bitmex`](https://roq-trading.com/docs/gateways/bitmex/index.html)
* [`roq-bitstamp`](https://roq-trading.com/docs/gateways/bitstamp/index.html)
* [`roq-coinbase-pro`](https://roq-trading.com/docs/gateways/coinbase-pro/index.html)
* [`roq-deribit`](https://roq-trading.com/docs/gateways/deribit/index.html)
* [`roq-ftx`](https://roq-trading.com/docs/gateways/ftx/index.html)
* [`roq-gemini`](https://roq-trading.com/docs/gateways/gemini/index.html)
* [`roq-hitbtc`](https://roq-trading.com/docs/gateways/hitbtc/index.html)

Support

* `roq-client`
  * Ultra low latency client-server communication
  * In-process simulation framework, including FIFO order matching
* `roq-simulator`
  * Gateway simulator
* `roq-influxdb`
  * InfluxDB exporter
* `roq-ansible`
  * Server provisioning using Ansible
  * Gateway configuration
* `roq-client-template`
  * Ansible Playbook (using `roq-ansible`)
* `roq-vagrant`
  * Deployment using Vagrant and VirtualBox
* `roq-grafana`
  * Grafana dashboards


## Design

We refer to the online [documentation](https://roq-trading.com/docs)
for a more comprehensive desciption.

This image provides an overview.

![overview](assets/overview.png)

## Performance

> The following examples have been tested on a relatively
> low-powered AMD EPYC 3251 SoC running at 2.5 GHz with
> hyperthreading disabled.
> Linux is Ubuntu Server 18.04 LTS with the `isolcpus`
> kernel parameter set.
> Server is physically located is Switzerland.

The Grafana example dashboard allows you to easily
monitor the performance of the gateways.

![gateways](assets/gateways.png)

This is the heartbeat latency between gateways and
connected clients.

![heartbeat](assets/heartbeat.png)

When hovering, you'll be able to see the attributes
of each measurement

> We collect a histogram of the measurements.
> This allows us to visualize e.g. "how many % of the events
> exceed 5 microseconds" (the tail distribution).

![heartbeat-details](assets/heartbeat_details.png)

> NUMA configuration and thread affinity affects latency.
> This example shows there are two distinct latencies
> depending on the deployment.

The gateways will automatically collect profiling
information.

![profile](assets/profile.png)

When hovering, you'll be able to see the the "cost"
of each operation

![profile-details](assets/profile_details.png)

> The cost of processing JSON updates (`ws_l2update`)
> is about 2x that of processing FIX (`market_data_incremental_refresh`).


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
