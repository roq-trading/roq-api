# Change Log

All notable changes will be documented in this file.

## Head

### Changed

* `MBPUpdate` now supports `UpdateAction` (#236)

## 0.8.5 &ndash; 2022-06-06

### Added

* `cache::Manager::get_market_with_id` and `cache::Manager::get_market_id`
* `ReferenceData::discard` (#225)

### Changed

* Promote `utils::DateTime_iso8601` formatter to API (from core)
* Make `cache::Manager` a non-template (#230)
* `cache::MarketByPrice::stream_id()`

## 0.8.4 &ndash; 2022-05-14

### Added

* `cache::MarketByPrice::find_index`
* `cache::MarketByPrice::compute_vwap`
* RequestIdType added to GatewaySettings (from server)

### Changed

* Increase `Symbol` fixed-length size to 48 (#214)
* `Event<>` and `Trace<>` formatting has move the value to the front
* C++17 compatibility (std::span and "using enum" workarounds)
* `Trace` now supports non-const (and retains const-safety when const)
* `TopOfBook` now includes an exhange sequence number (to correlate with MbP updates)
* `StreamStatus` now provides more detailed information
* Promote `debug::hex::Message` and `debug::fix::Message` (from core library)
* Comparisons now use (or return) `std::strong_ordering` (instead of `int`)
* Simplify enums (#199)
* Proper bit-mask support (#198)
* Promote Trace and TraceInfo to API (from server)

## 0.8.3 &ndash; 2022-03-22

### Changed

* Enforce fixed-length strings for standard use-cases (account, exchange, symbol, ...)
* Rename `string` (from `string_buffer`)
* Promote Mask to API (from utils)
* Rename headers to .hpp (#195)
* Conda packaging should pin versions to match x.x.x (#189)
* Added `update_type` to `OrderUpdate` and `TradeUpdate` (#39)

## 0.8.2 &ndash; 2022-02-18

### Changed

* Added account to ExternalLatency (#170)
* Source file name are now evaluated and stored at compile time (#160)

## 0.8.1 &ndash; 2022-01-16

### Changed                                                                                                                
                                                                                                                           
* Upgrade to C++20 (#158)

## 0.8.0 &ndash; 2022-01-12

### Added

* `cache::MarketByPrice::exists` (#153)
* `SecurityType::SWAP`
* Capture `origin_create_time` for externally triggered events (#140)

### Changed

* Increase `MAX_LENGTH_CURRENCY`
* `ReferenceData::margin_currency` (#150)
* Support macOS/ARM64 (#149)

### Removed

* `client::DepthBuilder` (#152)

## 0.7.9 &ndash; 2021-12-08

### Changed

* Align oms with new use of Decimals enum
* `MarketByPriceUpdate` now include max-depth (#123)
* `MarketByPriceUpdate` and `MarketByOrderUpdate` now include checksum (#74)
* `MarketByPriceUpdate` and `MarketByOrderUpdate` now include price/quantity decimals (#119)

### Added

* Capture request round-trip latency (#130)
* Starting to move object cache logic into API (#128)

## 0.7.8 &ndash; 2021-11-02

### Added

* Move cache utilities to API (#111)
* Promoted `server::TraceInfo` to API
* Add exchange sequence number to `MarketByPrice` and `MarketByOrder` (#101)
* Add `max_trade_vol` and `trade_vol_step_size` to ReferenceData (#100)
* New method to update `market::MarketByPrice` (#17)
* Price/quantity decimal digits added to `oms::Order` (#46)
* Conversion to/from internal price used by `market::MarketByPrice` (#21)
* Interface to extract bid/ask `MBPUpdate` from `market::MarketByPrice`
* New `UpdateType` (will eventually be used with `MarketByPriceUpdate`) (#93)
* Add `StatisticsType::TRADE_VOLUME` (#88)

### Changed

* Remove custom literals (#110)
* ReferenceData currencies should follow FX conventions (#99)
* Replace `snapshot` (bool) with `update_type` (UpdateType) (#97)
* Align type used to represent decimal digits (#46)
* Adding more `MAX_LENGTH` constants (#91)
* Align `PositionUpdate` with the FIX protocol (#89)

### Fixed

* Utility functions did not correctly handle `RequestStatus::FAILED` (#82)

## 0.7.7 &ndash; 2021-09-20

### Added

* New `market::MarketByPrice` interface (#56)

### Changed

* `RequestStatus::TIMEOUT` should not be a final order request state (#59)
* `GatewaySettings` has changed structure and new fields (#56)
* `client::DepthBuilder` now derives from `market::MarketByPrice` (#56)
* `string_buffer` did not have O(1) `length()` (#53)
* Move OMS interfaces to API (#51)
* `RateLimitTrigger` must support const members (#34)

### Fixed

* API exceptions should compile with C++14 (#69)

## 0.7.6 &ndash; 2021-09-02

### Added

* `Error::INSUFFICIENT_FUNDS` (#32)
* `RateLimitTrigger` replaces `RateLimitUsage` (#34)
* Promoting `RateLimitType` to API (#34)
* `NotSupported` exception (clean-up)
* `CustomMetrics` (publish) and `CustomMetricsUpdate` (receive) (#8)
* `NotImplemented` exception (clean-up)
* `Error::CONDITIONAL_REQUEST_HAS_FAILED` and `Error::UNKNOWN_ORDER_ID` (#25)
* `utils::was_order_received` and `utils::to_request_status` (#25)
* `OrderAck::side` (#11)
* `client::EventLogReader` interface (#10)

### Changed

* The various interfaces used for simulation has been updated (#7)

### Removed

* `Subscribe` has been removed from API (#14)

## 0.7.5 &ndash; 2021-08-08

### Changes

* `Error` has been extended to communicate certain exchange errors
* `RequestStatus` now includes `DISCONNECTED`, `TIMEOUT`, and `FAILED`
* `OrderAck` now includes `exchange` and `symbol`
* `CustomMessage` now uses `span` to represent the message

### Fixed

* `DepthBuilder` was unsafe because the constructor cached a pointer to a span
  of `Layer`s, aka. the depth.
  This has been changed and applying a `MarketByPriceUpdate` will now require
  you to also pass a reference to the depth.

### Added

* `Error::REQUEST_RATE_LIMIT_REACHED`
* `StatisticsType::FUNDING_RATE_PREDICTION`

### Removed

* Dropped all wrappers for fmt. (Reason: `fmt::format_string` is now a template
  solution -- previously it was a macro).

## 0.7.4 &ndash; 2021-07-20

### Changed

* The exception hierarchy no longer tries to mirror `std`.

## 0.7.3 &ndash; 2021-07-06

### Added

* `OrderManagement` used to instruct a gateway of the order management style
* `MAX_ORDER_ID` and `MAX_REQUEST_VERSION` to reflect 24 bit limits

### Changed

* `OrderAck`, `OrderUpdate`, `ModifyOrder` and `CancelOrder`
   now use `uint32_t` for all version fields
* The format functions no longer require use of the `_fmt` literal
* Reduced `MAX_LENGTH_ROUTING_ID` to 16
* `GatewaySettings` now includes `oms_download_has_state` and
  `oms_download_has_routing_id`

### Removed

* The `_fmt` literal
* The `format_str` wrapper class has been moved to the roq-logging library


## 0.7.2 &ndash; 2021-06-20

### Changed

* `Error` has been updated
* `OrderAck` has been updated and extended with `version`
* `OrderUpdate` has been updated and extended with `max_request_version`,
  `max_response_version` and `max_accepted_version`
* `ModifyOrder` and `CancelOrder` has been updated with `version` and
  `conditional_on_version`
* `CreateOrder` and `OrderUpdate` has changed field ordering to better group
  constant vs. possibly changing fields
* `TradeUpdate` and `Fill` has changed field ordering and `Fill` has dropped
  artificially generated `trade_id`'s
* `MBPUpdate` has been updated with `implied_quantity`, `price_level` and
  `number_of_orders`

## 0.7.1 &ndash; 2021-05-30

### Added

* `RateLimitUsage` to allow strategy to back off when it high-water mark on
   rate-limiting has been detected
* `CancelAllOrders` when all orders must be cancelled immediately
* `GatewaySettings.mbp_allow_remove_non_existing` to indicate if an exchange
   could possibly send updates to unknown price levels
* `StatisticsType::FUNDING_RATE` and `StatisticsType::DAILY_FUNDING_RATE`
* `routing_id` to `ModifyOrder` and `CancelOrder`
* `SupportType.ORDER_STATE` to indicate if gateway supports FIX 4.4 style
   order state management (through ClOrdId/OrigClOrdId transitions)
* `GatewaySettings.supports` to allow users to detect what is supported
* Added `Liquidity` to indicate if last fill was as maker or taker
* Extend `OrderUpdate` with more fields (required for FIX bridge and download)
* `TradingStatus` has been completely reviewed to match the trading session state
  changes which can occur on most exchanges
* `ExecutionInstruction` will now support bit-masks (future change)
* The layout of many structs have changed

### Changed

* `client::Config::Handler` now includes a method to configur
* `client::Settings`. This makes it possible to override `cancel_policy` (on
   disconnect)
* `Disconnected` now adds a flag useful to detect if disconnect could trigger an
   order cancellation request
* `OrderStatus` better aligned with FIX enum, but excluding state transitions
* `TimeInForce` better aligned with FIX enum


## 0.7.0 &ndash; 2021-04-15

### Added

* Promoted a number of generic utilities to the `roq::utils` namespace.
  Although these are useful, they should not be considered "API".
* The `routing_id` field has been added to the relevant order management
  structures.


### Changed

* All server (gateway) originated structs now include `stream_id` to indicate
  the origin of a message.
  `MarketDataStatus` and `OrderManagerStatus` have been replaced with
  `StreamUpdate`.
  Clients must use the `supports` bit-mask from `StreamUpdate` to maintain
  availability of cached objects.
  This change was done to allow gateways to manage load balance e.g. by
  maintaining multiple connections.
* Replaced `Connection` with `Connected` and `Disconnected`
* Renamed `GatewayStatus` to `ConnectionStatus`

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
