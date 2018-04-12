# Changelog

All notable changes to this project will be documented in this file.


## 0.1.3 - ?

### Added
* `DownloadBegin` event.
* Exception classes.

### Fixed
* Race condition for message encoding.
  The encoding buffer is now external to `codec::Encoder`.

### Changed
* Conventions for all structures relating orders and trades.
  This was done to better reflect meaning and intention.
* Field `max_order_id` added to the `Ready` struct.
* Field `channel` of the `MarketByPrice` and `TradeSummary` structs changed to `uint16_t`.
* Field `is_cache` of the `MessageInfo` struct renamed to `from_cache`.
* Renamed `Ready` to `DownloadEnd`

### Removed
* Order attributes from create/modify/cancel order ack messages.


## 0.1.2 - 2018-03-31

### Changed
* Namespace changed from `roq::common::` to `roq::`.
* Output stream operators moved from `roq/api.h` to `<roq/stream.h>`.
* Autoconf `m4` macros now imported as submodule (see `https://github.com/roq-trading/roq-m4`).


## 0.1.1 - 2018-03-27

### Added
* First release.

### Changed
* Source code moved from `https://github.com/quinclas/tradingapi` to `https://github.com/roq-trading/roq`.
