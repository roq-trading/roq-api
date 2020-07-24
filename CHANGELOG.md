# Change Log

All notable changes will be documented in this file.

## Head

### Removed

* Automake support

## 0.4.1 &ndash; 2020-07-17

**Note!**
*CMake is now the default build system for all Roq solutions.
This makes it significantly easier to integrate with other CMake based
solutions*.

### Changed

* CMake is now the default build system for all Roq solutions
* Replace ROQ\_PREDICT\_{TRUE,FALSE} with ROQ\_LIKELY and
  ROQ\_UNLIKLEY to better match c++20 naming
* Removed (again) the use of FMT\_STRING since it will not be
  compatible with c++20

### Removed

* `roq/format.h`

## 0.4.0 &ndash; 2020-06-30

**Note!**
*FlatBuffers has been introduced to compliment the C++ interface for
non-latency sensitive use-cases.
Event-logs are now using FlatBuffers for all encoding.
This allows for backwards compatibility and thereby dropping the
requirement for decoding to be binary compatible with encoding.
An upcoming release will introduce a FlatBuffers interface into
the gateways thereby allowing other languages than C++ languages
to communicate with the gateway*.

### Added

* FlatBuffers schema (experimental!)
* `StatisticsUpdate` (to replace `DailyStatistics` and `SessionStatistics`)

### Changed

* Events are now wrapped with the `Event` template
* Replaced `MarketByPrice` and `MarketByOrder` with `MarketByPriceUpdate` and
  `MarketByOrderUpdate` to more clearly reflect changes being communicated

## 0.3.9 &ndash; 2020-06-09

### Fixed

* `roq::event_value<ConnectionStatusEvent>` was broken in release 0.3.8

### Changed

* Metrics interfaces and utilities have been moved into the
  `roq::metrics` namespace
* Metric collectors now using std::atomic variables

### Removed

* Logging has been moved into a separate library `roq-logging`

## 0.3.8 &ndash; 2020-06-06

### Changed

* Auto-generate enums, structs, classes, formatting, etc.
* Major parts of the client API have been moved here

## 0.3.7 &ndash; 2020-05-27

### Added

* Dependency on FlatBuffers
* `SessionsStatistics::index_value` and
  `SessionsStatistics::margin_rate`

### Changed

* Inherit all network-related exceptions from `NetworkError`

## 0.3.6 &ndash; 2020-05-02

### Added

* `TimedOut` (exception)

## 0.3.5 &ndash; 2020-04-22

## 0.3.4 &ndash; 2020-04-08

### Added

* `ExecutionInstruction` (enum) similar to FIX `ExecInst`
* `Error::EXECUTION_INSTRUCTION_NOT_SUPPORTED`
* `Account::user`
* `OrderUpdate::execution_instruction`, `OrderUpdate::stop_price` and
  `OrderUpdate::max_show_quantity`
* Ensure all enums have an `UNDEFINED`

### Removed

* `OrderUpdate::commissions`

### Changed

* Log non-zero application exit codes as warning
* Prefer C++ raw strings

## 0.3.3 &ndash; 2020-03-04

### Added

* `Fill` (struct)
* `TopOfBook` (struct)
* `price_from_side` (function)
* `span` (struct)

### Removed

* `MBOUpdate::side`

### Changed

* `TradeUpdate` (struct) now containing an array of `Fill`'s (struct)
* Logging now requires `FMT_STRING` to better formatting errors at
  compile time
* `MarketByPrice` (struct) now maintaining `bids` and `asks` separately
* `MBOUpdate::order_id` (string) replaces `order_id_ext` (integer)
* Use `roq::span` everywhere
