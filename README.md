# Roq Trading Solutions

*A C++ HFT Toolkit for Algo Traders*


## What is it?

This is the API used to communicate between your trading
strategies and other components (market gateways)
developed by Roq.


## Overview

Primary motivations

* You can test without first having to sign a NDA
  * Remove barriers
* We publish this open sourced API giving anyone
  the free option to implement alternative solutions
  * Avoid unnecessary lock-ins
* Easy access to closed source binaries as either
  [Conda packages](https://roq-trading.com/conda/stable/index.html) or
  [Docker images](https://hub.docker.com/u/roqtrading)
  * Lets you focus on your trading strategies

> Some of the solutions are **free to use**,
> e.g. `roq-client` and `roq-influxdb`.
>
> Other solutions may require you to enter
> a **license agreement** with Roq to enable more
> advanced features, e.g. `roq-coinbase-pro`
> and `roq-deribit`.
>
> All solutions are completely free to download.

The gateways are very reasonably priced.
Please [contact us](mailto:info@roq-trading.com) for
further details.

## Build

> You will not achieve much by compiling this project.

The API is not very useful by itself.
You will need some implementation to make it useful.

> Roq offers a 100% free implementation named `roq-client`.

Head over to [Roq Samples](https://github.com/roq-trading/roq-api)
and follow the instrutions there to install the `roq-client` library.
You can then compile and test some simple examples.


## Benefits

* Ultra low latency
  * Single digit microsecond response time
* Very high message throughput
  * Only bound by single-message processing time
* Messages communicated to clients using a broadcast design
  * Efficient publish-once implementation
* Automatic connection and download management
  * Helps you more easily manage state
* Message normalization
  * A single unified interface used to access all markets
* Reference data
  * Multiplier, tick size, minimum quantity, etc.
* Market data
  * L1, L2, trade summary, session/daily statistics, etc.
* Order management
  * Create, modify, cancel, ack, update, trade (fill)
* Positions and funds
* Automatically persist all messages to storage device
  * With zero impact on trading latency
* Integrate well with popular network kernel-bypass solutions
  * Requires epoll support
* Suitable for fully autonomous algorithmic trading

## Constraints

* C++17
* **Linux** (RHEL, CentOS, Debian, Ubuntu)
* Deployment on **same host**
  * Shared memory for all latency sensitive communication
* Micro-service design
* Busy polling causing **100% CPU usage**
  * Required to achieve ultra low latency
* CPU isolation, disable hyperthreading, use thread affinity, etc.
  are strongly recommended practices
  * Therefore **not suitable for VM deployment**
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
  * In-process simulation framework (including order matching with simulated priority)
* `roq-simulator`
  * Simulation as a real-time service
* `roq-influxdb`
  * Export captured events to the InfluxDB time-series database
* `roq-ansible`
  * Server provisioning using Ansible
  * Manage your gateway configurations
* `roq-vagrant`
  * Test your deployment using Vagrant and VirtualBox
* `roq-grafana`
  * Grafana dashboards


## Design

This image is only an overview

![overview](assets/overview.png)

Please refer to the online [documentation](https://roq-trading.com/docs)
for a more comprehensive desciption.

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
