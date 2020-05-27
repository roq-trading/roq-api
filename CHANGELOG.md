# Change Log

All notable changes will be documented in this file.

## Head

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
