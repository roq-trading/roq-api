# Changelog

All notable changes to this project will be documented in this file.


## 0.1.3 - ?

### Fixed
* Race condition for message encoding.
  The encoding buffer is now external to `codec::Encoder`.

### Changed
* `Ready` message now includes the highest order id previously sent by the client.
* `MarketByPrice` and `TradeSummary` reduced the channel size to `uint16_t`.


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
