/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <chrono>
#include <utility>

#include "roq/precision.hpp"
#include "roq/side.hpp"

#include "roq/layer.hpp"
#include "roq/mbo_update.hpp"

#include "roq/market_by_order_update.hpp"
#include "roq/reference_data.hpp"

namespace roq {
namespace cache {

struct ROQ_PUBLIC MarketByOrder {
  virtual ~MarketByOrder() {}

  // meta data:

  virtual std::string_view exchange() const = 0;
  virtual std::string_view symbol() const = 0;

  // maximum depth (used when maintaining a view of top N price levels, zero means unbounded)
  virtual uint16_t max_depth() const = 0;

  // increments (conversion between floating point and integer representation)
  virtual double price_increment() const = 0;
  virtual double quantity_increment() const = 0;

  // precision (required to show all significant decimal places)
  virtual Precision price_precision() const = 0;
  virtual Precision quantity_precision() const = 0;

  // last update
  virtual uint16_t stream_id() const = 0;
  virtual std::chrono::nanoseconds exchange_time_utc() const = 0;
  virtual uint64_t exchange_sequence() const = 0;

  // checksum (useful for validating correct application of incremental updates)
  virtual uint32_t checksum() const = 0;

  // storage methods:

  // current depth
  //   returns {size(bids), size(asks)}
  virtual std::pair<size_t, size_t> size() const = 0;

  // is empty?
  virtual bool empty() const = 0;

  // reset internal state
  virtual void clear() = 0;

  // generic update interface using operator()
  template <typename... Args>
  inline void operator()(Args &&...args) {
    update_helper(std::forward<Args>(args)...);
  }

  // extract methods:

  // extract full snapshot of orders
  //   note! max_depth == 0 means full snapshot
  virtual void extract(std::vector<MBOUpdate> &, size_t max_depth = 0) const = 0;

  // extract all orders for price level
  virtual void extract(std::vector<MBOUpdate> &, Side, double price) const = 0;

  // extract depth
  //   note! max_depth == 0 means full snapshot
  //   note! bid/ask quantities may return +infinity when internal accounting overflows
  virtual void extract(std::vector<Layer> &, size_t max_depth = 0) const = 0;

  // extract depth
  virtual std::span<Layer const> extract_2(std::span<Layer> const &) const = 0;

  // computation methods:

  // check if price level exists
  virtual bool exists(Side, double price) const = 0;

  // find price-level index (if exists)
  //   returns {index, exists?}
  // note! index is computed in a similar way to using std::lower_bound
  virtual std::pair<size_t, bool> find_index(Side, double price) const = 0;

  // total quantity at price level
  //   returns NaN when price level does not exist
  virtual double total_quantity(Side, double price) const = 0;

  // accumulated quantity between best and price
  //   returns NaN when price level does not exist
  virtual double accumulated_quantity(Side, double price, bool excluding_price = false) const = 0;
  virtual double accumulated_quantity_price_index(Side, size_t index, bool excluding_price = false) const = 0;

  // find order (if exists)
  //   returns {order, exists?}
  virtual std::pair<MBOUpdate, bool> find_order(Side, std::string_view const &order_id) const = 0;

  // order update details computed from applying MarketByOrderUpdate to existing state
  struct OrderUpdate final {
    // order
    Side side = {};
    UpdateAction action = {};             // update action (undefined when unknown order requested to be deleted)
    double remaining_quantity = NaN;      // remaining quantity after update was applied
    double last_modified_quantity = NaN;  // last change to remaining_quantity, could be less than
                                          // last_traded_quantity
    double total_traded_quantity = NaN;   // life-time traded quantity, potentially tracking iceberg orders
    double last_traded_quantity = NaN;    // last traded quantity, could be more than
                                          // previous remaining_quantity
    // price-level (queue)
    double total_queue_quantity = NaN;     // sum of remaining_quantity for all orders in queue
    double queue_position_quantity = NaN;  // sum of remaining_quantity for orders
                                           // with better priority than this order
    size_t queue_position = {};            // relative position (index) of this order
    // order
    bool iceberg = false;
  };

  // order's queue position
  //   returns {position, exists?}
  struct Position final {
    double quantity = NaN;  // quantity for order
    double before = NaN;    // total quantity ahead in queue priority
    double total = NaN;     // total quantity
  };
  virtual std::pair<Position, bool> get_queue_position(Side, std::string_view const &order_id) const = 0;

  // simple update
  //   used when applying sequential updates, e.g. when caching
  inline void operator()(std::span<MBOUpdate const> const &orders) { update_helper(orders); }

  // create normalized update (used when origin is an external "noisy" source)
  template <typename Callback>
  inline void operator()(MarketByOrderUpdate const &market_by_order_update, std::vector<MBOUpdate> &orders, Callback callback) {
    auto market_by_order_update_2 = create_update_helper(market_by_order_update, orders);
    callback(std::as_const(market_by_order_update_2));
  }

  // create snapshot
  template <typename Callback>
  inline void create_snapshot(std::vector<MBOUpdate> &orders, Callback callback) const {
    auto market_by_order_update = create_snapshot_helper(orders);
    callback(std::as_const(market_by_order_update));
  }

 protected:
  virtual void update_helper(roq::ReferenceData const &) = 0;

  virtual void update_helper(MarketByOrderUpdate const &, std::span<OrderUpdate> const &) = 0;

  inline void update_helper(MarketByOrderUpdate const &market_by_order_update) { update_helper(market_by_order_update, {}); }

  virtual MarketByOrderUpdate create_update_helper(MarketByOrderUpdate const &, std::vector<MBOUpdate> &) = 0;

  virtual MarketByOrderUpdate create_snapshot_helper(std::vector<MBOUpdate> &) const = 0;

  // note! used when applying sequential updates
  virtual void update_helper(std::span<MBOUpdate const> const &) = 0;
};

}  // namespace cache
}  // namespace roq

template <>
struct fmt::formatter<roq::cache::MarketByOrder::OrderUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::cache::MarketByOrder::OrderUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(side={}, )"
        R"(remaining_quantity={}, )"
        R"(last_modified_quantity={}, )"
        R"(total_traded_quantity={}, )"
        R"(last_traded_quantity={}, )"
        R"(total_queue_quantity={},)"
        R"(queue_position_quantity={},)"
        R"(queue_position={},)"
        R"(iceberg={})"
        R"(}})"sv,
        value.side,
        value.remaining_quantity,
        value.last_modified_quantity,
        value.total_traded_quantity,
        value.last_traded_quantity,
        value.total_queue_quantity,
        value.queue_position_quantity,
        value.queue_position,
        value.iceberg);
  }
};
