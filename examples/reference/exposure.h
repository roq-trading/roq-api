/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

namespace examples {
namespace reference {

// meant to catch programming mistakes
class Exposure {
 public:
  typedef enum { Create, Fill, Cancel, Reject } update_t;
  void update(update_t type, roq::common::TradeDirection direction,
              double quantity);
  double get(roq::common::TradeDirection direction) const;

 private:
  typedef struct {
    double created = 0.0;
    double filled = 0.0;
    double cancelled = 0.0;
    double rejected = 0.0;
  } exposure_t;
  exposure_t _bid;
  exposure_t _offer;
  exposure_t& get_exposure(roq::common::TradeDirection direction);
  const exposure_t& get_exposure(roq::common::TradeDirection direction) const;
};

}  // namespace reference
}  // namespace examples
