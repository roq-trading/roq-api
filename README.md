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

* Programming language is C++17.
* Standardized API for all market access.
* Design is similar to that of
  [microservices](https://en.wikipedia.org/wiki/Microservices).
* Strategy implementation is single-threaded and must implement
  asynchronous event handling.
* Complexities are managed (connectivity, download, templates,
  segregation, policies, timeout, positions, etc).
* Ultra-low latency with internal round-trip response time around
  1 microsecond (depending on hardware).
* Samples on [GitHub](https://github.com/roq-trading/roq-samples).
* Binaries compiled for [Conda](https://docs.conda.io).
* Docker images published to
  [Docker Hub](https://cloud.docker.com/u/roqtrading).
* Ansible playbook on
  [GitHub](https://github.com/roq-trading/roq-ansible).

You can find more information on our
[website](https://roq-trading.com) or by browsing our online
[documentation](https://roq-trading.com/docs).


## Who Is This For?

> **PROFESSIONAL INVESTORS, ONLY!**
> 
> *Automated algorithmic trading is very complex and requires serious
> investment into hardware, software, research, testing, monitoring and
> support.
> In all probability, it will be a loss-making activity if you don't
> invest enough time and money towards simulation and infrastructure*.

Typical requirements

* 100% automation
* Intra-day trading
* Latency sensitive strategies
* High Frequency Trading (HFT)
* Market Taking
* Market Making
* Hedging
* Spread Trading (relative value or multi-leg strategies)
* Simulation of market micro-structure (probabilistic execution
  based on order priorities)
* Control the technology stack


## FAQ

<dl>
<dt><strong>Why have you developed this product?</strong></dt>
<dd>
We see an opportunity to offer complete trading solutions for start-up
investement firms.
In particular, we believe we have an advantage compared to the multitude
of existing and specialized solutions because our solution offers
tooling from research and testing all the way to real trading.
All the solutions are designed such that the quant and/or trader can 
retain the control of the entire technology stack.
</dd>
<dt><strong>Do I have to sign a NDA to start using the product?</strong></dt>
<dd>
No.
Strategy examples can be found on
<a href="https://github.com/roq-trading/roq-samples">GitHub</a>.
Follow the instructions there to get started with your own strategy
implementation.
You can use the simulators without contacting us.
Market access is different: You can download the gateways, but you will
need to contact us to obtain a valid license file.
</dd>
<dt><strong>I am not a professional investor, should I use the product?</strong></dt>
<dd>
Yes.
We promote that you use our simulation tools for your research.
Testing ideas is a good way to gain experience and perhaps you can
later use the results to partner with a professional investment firm.
</dd>
<dt><strong>What markets do you support?</strong></dt>
<dd>
Currently CFFEX/FEMAS (including multicast market data feed).
Work is ongoing for Shengli/REM (FPGA based solution).
Please contact us to discuss specific requirements.
</dd>
<dt><strong>How do I obtain the market data needed for simulation?</strong></dt>
<dd>
We don't resell market data.
You must obtain historical market data directly from the exchange or
from third-party data vendors.
We can help with data conversion, should that be necessary.
</dd>
<dt><strong>How is round-trip latency measured?</strong></dt>
<dd>
The design is that of microservices: Components are loosely coupled.
We measure from event handover from NIC or FPGA to gateway, then through
shared memory to client, internal processing of client <i>excluding user
code</i>, back through shared memory to gateway, and finally event
handover to NIC or FPGA.
This round-trip latency can be less than 1 microsecond depending on
hardware and system configuration.
</dd>
<dt><strong>What are the hardware requirements?</strong></dt>
<dd>
It depends on your use case.
Please contact us to discuss further.
However, the solutions are designed for ultra low latency and therefore
using busy polling to avoid involving the kernel scheduler.
Generally speaking: These solutions are very CPU hungry. The more cores
you have, the better.
</dd>
<dt><strong>Why is there no source code for the API?</strong></dt>
<dd>
An earlier version actually included an open sourced client API
implementation.
This unfortunately introduced several support issues which lead us to
the conclusion that all parties are better served with a well defined
client API distributed only in binary form.
We do however publish development examples to
<a href="https://github.com/roq-trading/roq-samples">GitHub</a>.
</dd>
</dl>


## First Steps

The benchmark suite is useful for demonstrating resource requirements,
profiling and monitoring.

In particular, the following features are being demonstrated

* `roq-ping` a synthetic "gateway" simulating key execution paths
  * Broadcast market data
  * Handle order creation requests
  * Respond with order creation ack 
  * Access to internal metrics

* `roq-pong` a synthetic "client" simulating key execution paths
  * Receive and process market data
  * Optionally request order creation
  * Access to internal metrics

* `roq-benchmark` profiling the speed of key functions
  * Message encoding/decoding
  * String formatting (stream and format operations)
  * Other benchmarking useful for selecting e.g. containers

> Internal metrics are made available over HTTP using the
> [Prometheus Exposition Format](https://prometheus.io/docs/instrumenting/exposition_formats/).


### Install Miniconda

> *This section will demonstrate how to install Miniconda*.

A Conda environment allows you to install up-to-date binary packages
on your development box without requiring root access.

```bash
# download the miniconda installer
wget -N https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh

# install miniconda (to your home directory)
bash Miniconda3-latest-Linux-x86_64.sh -b -u -p ~/miniconda3

# define conda channels
cat > ~/miniconda3/.condarc << EOF
channels:
  - defaults
  - https://roq-trading.com/conda/unstable
EOF
```

### Activate Miniconda

> *This section will demonstrate how to activate your Conda environment*.

This is how you activate your Conda environment

```bash
source ~/miniconda3/bin/activate
```

The following sections will assume you have already activated your
Conda environment.


### Local Testing

> *This section will demonstrate how to install the binaries
> using Conda.*

First install the benchmark suite

```bash
conda install -y roq-benchmark
```

You can test the speed of key functions:

Run the benchmark suite

```bash
roq-benchmark
```

You should now see the profiling results

![overview](assets/Benchmark.png)

And you can simulate a real network of gateways and clients:

This is the test case

![overview](assets/PingPongSimple.png)

Launch the ping service

```bash
ROQ_v=1 roq-ping \
  --name "server-1" \
  --dispatcher-affinity 1 \
  --market-data-affinity 2 \
  --metrics 1234 \
  --listen ~/ping.sock
```

Launch the pong service (from another terminal)

```bash
ROQ_v=1 roq-pong \
  --name "client-1" \
  --dispatcher-affinity 5 \
  --create-orders 1 \
  --metrics 2345 \
  ~/ping.sock
```

> `ROQ_v=1` sets an environment variable used to specify the
> verbosity of application logging. You may drop this part from the
> command-line (or use `ROQ_v=0`) to only see minimal logging.
>
> Verbose logging can be very costly: `roq-benchmark` will give you
> more details about the cost of each line being logged.

You can now query for the metrics

```bash
wget -q -O - http://localhost:1234/metrics | less
```

or

```bash
wget -q -O - http://localhost:2345/metrics | less
```

You should see something like this

![overview](assets/Metrics.png)


### Deploy to Server

> *This section will demonstrate how to provision a server using the 
> Ansible playbook*.

This is the test case

![overview](assets/PingPongServer.png)

Requirements

* Physical access (not a VM)
* At least 8 physical cores
* Linux (CentOS/7, Ubuntu 18.04, or better)
* An `ansible` sudo user with ssh access rights

For best results 

* Add `isolcpus=1-6` to `/proc/cmdline` and reboot the server.
* Disable hyper-threading in the BIOS.
* Tune your system for low latency performance,
  e.g. `tuned-adm profile network-latency`.

The Ansible playbook includes these following steps

* Configure the server for low latency performance
* Install benchmark application configurations, application launch
  scripts and systemd service scripts
* Install Prometheus, Grafana and Nginx (using Docker)

```bash
# install git and ansible
conda install -y git ansible-playbook

# clone the playbook
git clone https://github.com/roq-trading/roq-ansible

# change into the new directory
cd roq-ansible

# define your inventory (replace "x.x.x.x" with your server's network address)
cat > server << EOF
[SERVER]
server ansible_host="x.x.x.x"
[roq-benchmark]
server
EOF

# run the ansible playbook
ansible-playbook -i server site.yml --ask-become-pass
```

On your server

```bash
sudo systemctl start roq-ping-1
sudo systemctl start roq-ping-2
sudo systemctl start roq-pong-1
sudo systemctl start roq-pong-2
```

You should now see core 1-6 consume 100% CPU (if everything was
installed correctly)

![overview](assets/htop.png)

You can tail the logs

```bash
sudo journalctl -f -u roq-p[io]ng-[12]*
```

Navigate to this URL using your favourite browser (replace `x.x.x.x`
with your server's network address)

```
http://x.x.x.x/grafana/
```

Find the `roq-benchmark (ping-pong)` dashboard and you should be able
to monitor latency distributions like this

![overview](assets/Grafana.png)

> These metrics are collected from an Atom C3758 8-core processor.


## Next Steps

* [Contact us](mailto:info@roq-trading.com)
* [Roq Trading Solutions (website)](https://roq-trading.com)
* [Online documentation](https://roq-trading.com/docs)
* [Development examples](https://github.com/roq-trading/roq-samples)
* [Ansible playbook](https://github.com/roq-trading/roq-ansible)
* [Grafana dashboards](https://github.com/roq-trading/roq-grafana)
* [Vagrant development environments](https://github.com/roq-trading/roq-vagrant)
