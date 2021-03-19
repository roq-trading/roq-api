# Change Log

All notable changes will be documented in this file.

## Head

### Changed

* All server (gateway) originated structs now include `stream_id` to indicate
  the origin of a message.
  `MarketDataStatus` and `OrderManagerStatus` have been replaced with
  `StreamUpdate`.
  Clients must use the `supports` bit-mask from `StreamUpdate` to maintain
  availability of cached objects.
  This change was done to allow gateways to manage load balance e.g. by
  maintaining multiple connections.

### Removed

* Python API (this is not the right place)
* Removed `name` from `ExternalLatency`
* Removed `MarketDataStatus` and `OrderManagerStatus`

## 0.6.1 &ndash; 2021-02-19

### Added

* Convenience functions making it easy to encode Flatbuffers objects

### Changed

* Consistent use string literals
* Enforce usage of the `_fmt` literal when formatting

### Removed

* Enumerations no longer include `MAX`


## 0.6.0 &ndash; 2021-02-02

### Changed

* Repo now includes the auto-generated header files (for better discoverability)

* Miniforge (instead of Miniconda)

* `MessageInfo.source_session_id` now using a more efficient UUID representation

* Now using span-lite (instead of own implementation, C++20 preparation)

### Added

* `Settings`
* `ExternalLatency`

### Removed

* `User` and `Account` removed (not public interfaces)

## 0.5.0 &ndash; 2020-12-04

### Changed

* Time management has been changed

  * `std::chrono::nanoseconds` used for all precision timestamps
  * `std::chrono::seconds` used for all date-times
  * `std::chrono::days` (using `roq::chrono::days` until C++20 is supported)
    used for all dates

### Added

* `ReferenceData`

   * `description`, `underlying`,
     `time_zone`, `issue_date`, `settlement_date`,
     `expiry_datetime`, `expiry_datetime_utc

### Removed

* `ReferenceData::limit_{up|down}`

   * Upper and lower trading limits belongs to daily session statistics,
     not reference data.
     These fields were anyway already managed by `StatisticsUpdate`.

## 0.4.5 &ndash; 2020-11-09

## 0.4.4 &ndash; 2020-09-20

## 0.4.3 &ndash; 2020-09-02

### Changed

* `client::Collector` interface now implements default handlers instead of
   having pure virtual functions.
* `is_{order|request}_completed` replacing `is_completed`.

### Removed

* `client::Collector::{extract|write}` were too specific.

## 0.4.2 &ndash; 2020-07-27

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
