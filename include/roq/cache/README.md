> These interfaces are work in progress!

Some updates will contain the full image, e.g. `ReferenceData`.
Other updates are incremental, e.g. `MarketByPriceUpdate`.

All updates will generally contain "views" of the underlying message storage
and are therefore not valid outside the callback handler.

The utility classes found here deal with incremental updates and "views" when
last known state is needed outside the callback handler.

> Not all caching logic is open source (in particular `MarketByPrice` and `MarketByOrder`).
