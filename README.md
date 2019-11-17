# Roq Trading Solutions

*A C++ HFT Toolkit for Algo Traders*.


## What is it?

The API used to communicate between clients (the
implementation of your trading strategy) and other
components developed by Roq, e.g. market gateways.


## Overview

An open sourced API motivated by

* No requirement to sign a NDA
* Anyone should be able to implement solutions against the interface

> Closed source solutions include
> * `roq-coinbase-pro` market gateway
> * `roq-deribit` market gateway
> * `roq-client` library
>   * Low latency communication between trading strategy
>     and market gateways
>   * Simulation framework, including FIFO order matching
> * `roq-influxdb` exporter
>
> Some solutions are free to use, e.g. `roq-client` and `roq-influxdb`.
> Other solutions will require you to enter a license agreement, e.g.
> `roq-coinbase-pro` and `roq-deribit`.


## Build

The API is not very useful by itself.
You will need an implementation, such as the
`roq-client` library.

[Roq Samples](https://github.com/roq-trading/roq-api)
will help you with installation and getting started.


## Benefits

* Ultra low latency (single digit microsecond response time)
* Very high message throughput
* Messages are being communicated to clients using a broadcast design
* Automatic connection management
* Message normalization
* Market data (L1, L2, trade summary, session/daily statistics, etc).
* Order management (create, modify, cancel, ack, update, trade)
* Positions and funds (when available)
* Automatically persist all messages to storage device (zero latency impact)
* Integrate well with popular network kernel-bypass solutions

## Constraints

* C++17
* Linux (RHEL, CentOS, Debian, Ubuntu)
* Same-host deployment
* Communication over shared memory
* Micro-service design
* Requires CPU isolation and use of thread affinity
* Enough CPU cores to support your use-case


## Solutions

These are some of our solutions

* `roq-client`
  * Low latency client-server communication
  * Simulation framework, including FIFO order matching
* `roq-coinbase-pro`
  * Market gateway
  * FIX, WebSocket and REST
* `roq-deribit`
  * Market gateway
  * FIX and WebSocket
* `roq-influxdb`
  * InfluxDB exporter
* `roq-ansible`
  * Server provisioning using Ansible
  * Configuration
* `roq-client-template`
  * Ansible Playbook (using `roq-ansible`)
* `roq-vagrant`
  * Deployment using Vagrant and VirtualBox
* `roq-grafana`
  * Grafana dashboards
* `roq-simulator`
  * Market gateway simulator
* `roq-benchmark`
  * Performance testing


## Design

This is not a comprehensive description of the design.
For that we refer to the online documentation (see
links below).

![overview](assets/overview.png)

## Performance

> The following examples have been tested on a relatively
> low-powered AMD EPYC 3251 SoC running at 2.5 GHz with
> hyperthreading disabled.
> Linux is Ubuntu Server 18.04 LTS with the `isolcpus`
> kernel parameter set.

The Grafana example dashboard allows you to easily
monitor the performance of the gateways.

![gateways](assets/gateways.png)

> This example is running on a relatively low-powered
> AMD EPYC 3251 SoC.

This is the heartbeat latency between gateways and
connected clients.

![heartbeat](assets/heartbeat.png)

When hovering, you'll be able to see the attributes
of each measurement

![heartbeat-details](assets/heartbeat_details.png)

> NUMA configuration affects latency.
> This example shows there are two distinct latencies
> depending on the deployment configuration.

The gateways will automatically collect profiling
information.

![profile](assets/profile.png)

When hovering, you'll be able to see the the "cost"
of each operation

![profile-details](assets/profile_details.png)

> The cost of parsing JSON (e.g. Coinbase Pro's
> `l2update`) is about 2x that of parsing FIX
> (e.g. Deribit's `market_data_incremental_refresh`).


## Links

* [Documentation](https://roq-trading.com/docs)
* [Contact us](mailto:info@roq-trading.com)
