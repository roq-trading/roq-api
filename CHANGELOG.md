# Change Log

All notable changes will be documented in this file.

## Head

Unchanged

## 0.3.5 &ndash; 2020-04-22

Unchanged

## 0.3.4 &ndash; 2020-04-08

### Added
* `ExecutionInstruction` (enum)
  * Similar to FIX `ExecInst`
* `Error` (enum)
  * `EXECUTION_INSTRUCTION_NOT_SUPPORTED`
* `Account` (struct)
  * `user`
* Enums
  * Ensure all have `UNDEFINED`

### Removed
* `OrderUpdate` (struct)
  * `commissions`

### Changed

* Log non-zero application exit codes as warning
* Prefer C++ raw strings

## 0.3.3 &ndash; 2020-03-04

### Added

* `TopOfBook` (struct)
* `price_from_side` (function)
* `span` (struct)

### Removed
* `MBOUpdate` (struct)
  * `side`

### Changed

* `TradeUpdate` (struct)
  * Now containing array of `Fill`'s (struct)
* Logging
  * Require `FMT_STRING` usage to catch more logging errors at compile time
* `MarketByPrice` (struct)
  * Separately maintain bids and asks
* `MBOUpdate` (struct)
  * `order_id` (string) replaces `order_id_ext` (integer)
* Arrays
  * Use `roq::span` everywhere
