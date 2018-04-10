# Changelog

All notable changes to this project will be documented in this file.


## 0.1.3 - ?

### Added
* New `DownloadBegin` event.
* Exception classes.

### Fixed
* Race condition for message encoding.
  The encoding buffer is now external to `codec::Encoder`.

### Changed
* Changed all order and trade related templates to better reflect the intention of each field.
* Changed `Ready` to include `max_order_id` (highest order id previously used by the client).
* Changed `MarketByPrice` and `TradeSummary` field `channel` to `uint16_t`.
* Renamed `MessageInfo` field `is_cache` to `from_cache`.
* Renamed `Ready` to `DownloadEnd`


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


***

### Added
### Fixed
### Changed
### Removed
