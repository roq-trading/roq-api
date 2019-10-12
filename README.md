# Roq Trading Solutions

Solutions focused on development, testing and deployment of
algorithmic trading strategies.


## Overview

Solutions

* Market access
* Simulation tools
* Data collection
* Consultancy

Features

* Programming language is C++17
* Standardized market access API
* Design is similar to that of
  [microservices](https://en.wikipedia.org/wiki/Microservices)
* User code (your trading strategies) must implement event handling
* Framework will manage complexities, e.g. threading, asynchronous
  messaging, connectivity, download phases, order templates, trading
  segregation, trading policies, request timeouts, positions, etc.
* Predictable ultra low latency with *internal* tick-to-trade
  response time in the low microseconds on high-end hardware
* Samples on [GitHub](https://github.com/roq-trading/roq-samples)
* Binaries compiled for [Conda](https://docs.conda.io)
* Docker images published to
  [Docker Hub](https://cloud.docker.com/u/roqtrading)
* Ansible playbook on
  [GitHub](https://github.com/roq-trading/roq-ansible)

You can find more information by browsing our online
[documentation](https://roq-trading.com/docs).

## What is it?

An open-sourced API used by the various Roq developed solutions.
By itself, the API offers very little functionality:
mainly data structures and very trivial utilities.

This document is meant to describe high level design choices.
Please refer to
[roq-samples](https://github.com/roq-trading/roq-samples)
for how to use.

## Who is it for?

> **PROFESSIONAL INVESTORS, ONLY**
>
> *Automated algorithmic trading is very complex and requires serious
> investment into hardware, software, research, testing, monitoring
> and support.
> In all probability it will be a loss-making activity if you don't
> invest enough time and money to prepare yourself for live trading*.

Typical requirements

* 100% automation
* Intra-day trading
* Latency sensitive strategies
* High Frequency Trading (HFT)
* Arbitrage
* Market Taking
* Market Making
* Auto Hedging
* Spread Trading (relative value or multi-leg strategies)
* Execution strategies (iceberg, VWAP, etc).
* Simulation of market micro-structure (with probabilistic FIFO
  execution based on order priority)
* Full control of the technology stack


## FAQ

<dl>
<dt><strong>
Why have you developed this product?
</strong></dt>
<dd>
We see an opportunity to offer a complete trading solution for
smaller investement firms.
Our solutions offer all the relevant tooling from initial research
and testing all the way to the deployment of fully automated trading
strategies.
Our solutions work well with popular open sourced solutions to allow
our clients to reduce total cost of ownership.
</dd>
<dt><strong>
Do I have to sign a contract or NDA to start using your product?
</strong></dt>
Trivial examples, demonstrating how to implement various aspects of
trading strategies, are free and can be accessed from
<a href="https://github.com/roq-trading/roq-samples">GitHub</a>.
You can also use the free to download simulator binaries for testing
your trading strategies without having to contact us.
However, **the gateways are *not* free** and will require a license
file issued only after you have entered into a contractual license
agreement with us.
</dd>
<dt><strong>
I am not a professional investor, should I use your product?
</strong></dt>
You can always use the API's and our simulation tools to
<i>research</i> and <i/>test</i> ideas.
However, you should partner with a professional investment firm to
implement live trading.
</dd>
<dt><strong>
What market access do you currently support?
</strong></dt>
<dd>
<ul>
<li>Coinbase Pro (FIX, WebSocket and REST)</li>
<li>Deribit (FIX)</li>
</ul>
</dd>
<dt><strong>
How do I obtain the market data needed for historical simulation?
</strong></dt>
Preferably using the market gateways to collect your own data.
You are then guaranteed native support and the correct ordering of
events during replay.
It is also possible to convert third-party data sources to the
internal format.
Please contact us for further details.
</dd>
<dt><strong>
How is tick-to-trade latency measured?
</strong></dt>
<dd>
From receiving a network packet (or an update event, if using
third-party API) to an order action request can be forwarded to
the network stack (or a request to the third-party API).
We do <i>not</i> include external contributors to latency.
In particular:
network configuration, hardware configuration, kernel configuration,
performance of required third-party API's or
performance of specific trading strategies.
</dd>
<dt><strong>
What are the hardware requirements?
</strong></dt>
<dd>
The generic answer:
<i>A high-end server with enough cores to support your use case</i>.
However, it very much depends on the specific use case.
Please contact us to discuss further.
</dd>
<dt><strong>
Why do you not open source the client library and the market
gateways?
</strong></dt>
<dd>
An earlier version included an open sourced client API
implementation.
This, unfortunately, introduced recurring support issues which
lead us to the conclusion that all parties are better served with
a well-defined client library only available in binary form.
License and support agreements allows us to invest into the ongoing
maintenance and support of market gateways to the benefits of all.
</dd>
<dt><strong>
What is your business model?
</strong></dt>
<ul>
<li>License and support agreements for the gateways.</li>
<li>Consultancy. For example, implementation of bespoke trading
strategies.</li>
<dd>
</dd>
</dl>


## First steps

> Please refer to [roq-samples](https://github.com/roq-samples) if
> you need a guide on how to get started with your own strategy
> implementation.

The benchmark suite was developed to demonstrate typical resource
requirements as well as the performance and monitoring capabilities.

These are the relevant binaries

* `roq-benchmark` is used to profile key functionality
  * Message encoding/decoding
  * String formatting operations
  * Other benchmarking, e.g. operations on containers
* `roq-ping` is a service used to simulate the key execution paths
  of a typical "server" component
  * Broadcast random market data (L2 with a depth of 5)
  * Handle order creation requests
  * Respond with order creation ack
  * Provide access to internal metrics
* `roq-pong` is a service used to simulate the key execution paths
  of a typical "client" component
  * Receive and process market data
  * Optionally request order creation
  * Provide access to internal metrics

> Internal profiling metrics use the
> [Prometheus Exposition Format](https://prometheus.io/docs/instrumenting/exposition_formats/).


### Install Miniconda

> *This section will demonstrate how to install Miniconda*.

A Conda environment allows you to install up-to-date binary packages
on your development box *without* requiring root access.

```bash
# download the miniconda installer
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

# install miniconda (to your home directory)
bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

# add roq-trading unstable as a conda channel
cat > ~/miniconda3/.condarc << EOF
channels:
  - defaults
  - https://roq-trading.com/conda/unstable
  - conda-forge
EOF
```

### Activate Miniconda

> *This section will demonstrate how to activate your Conda
> environment*.

This is how you activate your Conda environment

```bash
source ~/miniconda3/bin/activate
```

> Note! The following sections will assume you have *already*
> activated your Conda environment.


### Install the benchmark suite

> *This section will demonstrate how to install the binaries
> using Conda.*

First install the benchmark suite

```bash
conda install -y roq-benchmark
```

### Test (local)

You can test the speed of key functions:

Run the benchmark suite

```bash
roq-benchmark
```

You should now see something like this

![overview](assets/Benchmark.png)

You can also simulate a real network of gateways and clients:

This is our generic test case

![overview](assets/PingPongSimple.png)

> The design is meant to be a realistic simulation with
> regular random market data updates being broadcast from
> `roq-ping` to `roq-pong`.
> It is *not* a test of maximum throughput since that's not
> typical for a live trading environment.
> It is much more important to test for a regular mix of
> "sleep" and "wake" patterns.
> For example, cached memory may be evicted during sleep and
> waking a thread involves an expensive context switch.

This is how to launch the ping service

```bash
# launch the ping service
# - dispatch thread pinned to core #1
# - market data thread pinned to core #2
# - metrics exposed to port 1234 (on localhost)
ROQ_v=1 roq-ping \
  --name "server-1" \
  --dispatcher-affinity 1 \
  --market-data-affinity 2 \
  --metrics 1234 \
  --listen ~/ping.sock
```

> `ROQ_v=1` sets an environment variable used to specify the
> verbosity of application logging. You may drop this part from the
> command-line (or use `ROQ_v=0`) to only generate minimal logging.
>
> Verbose logging can be relatively costly, though.
> The previous section describing `roq-benchmark` will give you more
> information about the "cost" of typical messages being logged.

Now open another terminal (same host) and activate your Miniconda
environment.

This is how to launch the pong service

```bash
# launch the pong service
# - dispatch thread pinned to core #3
# - metrics exposed to port 1234 (on localhost)
# - create an order request on each market data update
ROQ_v=1 roq-pong \
  --name "client-1" \
  --dispatcher-affinity 3 \
  --create-orders 1 \
  --metrics 2345 \
  ~/ping.sock
```

You can query for profiling metrics

```bash
# query the ping service
wget -q -O - http://localhost:1234/metrics | less
```

or

```bash
# query the pong service
wget -q -O - http://localhost:2345/metrics | less
```

You should see something like this

![overview](assets/Metrics.png)

> Refer to [Prometheus Exposition Format](https://prometheus.io/docs/instrumenting/exposition_formats/).


### Test (server)

> *This section will demonstrate how to provision a server using the
> Ansible playbook*.

This is the test case

![overview](assets/PingPongServer.png)

Requirements

* Physical access (**not** using a VM)
* At least 8 physical cores
* CentOS/7 or Ubuntu Server 18.04, or better
* An `ansible` sudo user with ssh access rights

For better results

* Add `isolcpus=1-6` to `/proc/cmdline` and reboot the server.
* Disable hyper-threading in the BIOS.
* Tune your system for low latency performance,
  e.g. `tuned-adm profile network-latency`.

The Ansible playbook has the following steps

* Configure the server for low latency performance
* Install benchmark application configurations, application launch
  scripts and systemd service scripts
* Install Prometheus, Grafana and Nginx (using Docker)

#### Step 1: Prepare your environment

> You don't need this step if recent versions of git and ansible are
> available on your system.

The assumption is that you have already activated your Conda
environment (details above).

```bash
conda install -y git ansible
```

#### Step 2: Prepare the playbook

> The assumption here is that you're running the playbook from the
> host (`ansible_host="localhost"`) you want to provision and
> that the ansible user (`ansible_user="ansible"`) user already
> exists and it has been given `sudo` rights.

```bash
# clone the roq playbook
git clone https://github.com/roq-trading/roq-ansible

# change into the new directory
cd roq-ansible

# define your inventory
# note!
#  you can replace "localhost" with your server's network address
cat > server << EOF
[SERVER]
server ansible_host="localhost" ansible_user="ansible" become_user="root"

[roq_benchmark]
server
EOF
```

#### Step 3: Run the playbook

```bash
ansible-playbook -i server site.yml --ask-become-pass
```

#### Step 4: Start the services

> At this point you should log on to the host you have installed to.

```bash
sudo systemctl start roq-ping-1
sudo systemctl start roq-ping-2
sudo systemctl start roq-pong-1
sudo systemctl start roq-pong-2
```

#### Step 5: Test everything works correctly

You should now see core 1-6 consume 100% CPU (if everything was
installed correctly)

![overview](assets/htop.png)

You can tail the logs

```bash
sudo journalctl -f -u roq-p[io]ng-[12]*
```

#### Step 5: Monitoring

Navigate to this URL using your favourite browser

```
http://localhost/grafana/
```

> Remember to replace `localhost` with your server's network address

Find the pre-installed `roq-benchmark (ping-pong)` dashboard and you
should now be able to monitor latency distributions like this

![overview](assets/Grafana.png)

> These metrics are collected from an Atom C3758 8-core processor.


## Next steps

* [Contact us](mailto:info@roq-trading.com)
* [Roq Trading Solutions (website)](https://roq-trading.com)
* [Online documentation](https://roq-trading.com/docs)
* [Development samples](https://github.com/roq-trading/roq-samples)
* [Ansible playbook](https://github.com/roq-trading/roq-ansible)
* [Grafana dashboards](https://github.com/roq-trading/roq-grafana)
* [Vagrant development environments](https://github.com/roq-trading/roq-vagrant)
