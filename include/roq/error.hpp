/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of error types
enum class Error : uint8_t {
  UNDEFINED = 0,
  UNKNOWN,
  NOT_SUPPORTED,                   //!< Request type not supported
  INVALID_ACCOUNT,                 //!< Invalid account
  INVALID_ORDER_ID,                //!< Invalid order ID
  INVALID_EXCHANGE,                //!< Invalid exchange
  INVALID_SYMBOL,                  //!< Invalid symbol
  INVALID_SIDE,                    //!< Invalid side
  INVALID_POSITION_EFFECT,         //!< Invalid position effect
  INVALID_QUANTITY,                //!< Invalid quantity
  INVALID_MAX_SHOW_QUANTITY,       //!< Invalid max show quantity
  INVALID_ORDER_TYPE,              //!< Invalid order type
  INVALID_TIME_IN_FORCE,           //!< Invalid time in force
  INVALID_EXECUTION_INSTRUCTION,   //!< Invalid execution instruction
  INVALID_REQUEST_TEMPLATE,        //!< Invalid request template
  INVALID_PRICE,                   //!< Invalid price
  INVALID_STOP_PRICE,              //!< Invalid stop price
  INVALID_ROUTING_ID,              //!< Invalid routing ID
  INVALID_REQUEST_VERSION,         //!< Invalid request version
  INVALID_REQUEST_ID,              //!< Invalid request ID
  INVALID_REQUEST_TYPE,            //!< Invalid request type
  INVALID_REQUEST_STATUS,          //!< Invalid request status
  INVALID_REQUEST_ARGS,            //!< Request arguments did not meet validation rules
  UNKNOWN_EXTERNAL_ORDER_ID,       //!< An external order identifier has not yet been received
  NOT_AUTHORIZED,                  //!< User is not authorized
  REQUEST_RATE_LIMIT_REACHED,      //!< Request rate limit has been reached
  GATEWAY_NOT_READY,               //!< Gateway not ready (could be disconnected or currently downloading)
  NETWORK_ERROR,                   //!< Network disconnect event (unknown request/order status)
  TIMEOUT,                         //!< Response has not been received (unknown request/order status)
  PARSE_ERROR,                     //!< Message parse error (unknown request/order status)
  MODIFY_HAS_NO_EFFECT,            //!< Modify request has no effect on order
  TOO_LATE_TO_MODIFY_OR_CANCEL,    //!< Too late to modify or cancel (order has been filled or is already canceled)
  CONDITIONAL_REQUEST_HAS_FAILED,  //!< Happens when gateway detects chain failure
  UNKNOWN_ORDER_ID,                //!< Unknown order ID
  INSUFFICIENT_FUNDS,              //!< Insufficient funds for order action
  RISK_LIMIT_REACHED,              //!< Risk limit reached
  INVALID_FILTER,                  //!< Invalid filter
  INVALID_MARGIN_MODE,             //!< Invalid margin mode
  INVALID_QUANTITY_TYPE,           //!< Invalid quantity type
};

}  // namespace roq
